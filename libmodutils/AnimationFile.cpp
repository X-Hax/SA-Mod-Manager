#include "stdafx.h"
#include "AnimationFile.h"
#include <fstream>
#include <iostream>
using std::default_delete;
using std::ifstream;
using std::ios;
using std::istream;
using std::list;
using std::shared_ptr;
using std::streamoff;
using std::string;
#ifdef _MSC_VER
using std::wstring;
#endif /* _MSC_VER */

AnimationFile::AnimationFile(const char* filename)
{
	ifstream str(filename, ios::binary);
	init(str);
	str.close();
}

#ifdef _MSC_VER
AnimationFile::AnimationFile(const wchar_t* filename)
{
	ifstream str(filename, ios::binary);
	init(str);
	str.close();
}
#endif /* _MSC_VER */

AnimationFile::AnimationFile(const string& filename)
{
	ifstream str(filename, ios::binary);
	init(str);
	str.close();
}

#ifdef _MSC_VER
AnimationFile::AnimationFile(const wstring& filename)
{
	ifstream str(filename, ios::binary);
	init(str);
	str.close();
}
#endif /* _MSC_VER */

AnimationFile::AnimationFile(istream& stream) { init(stream); }

NJS_MOTION* AnimationFile::getmotion() const { return motion; }

int AnimationFile::getmodelcount() const { return modelcount; }

bool AnimationFile::isshortrot() const { return shortrot; }

const string& AnimationFile::getlabel()
{
	return getlabel(motion);
}

static const string empty;
const string& AnimationFile::getlabel(void* data)
{
	auto elem = labels1.find(data);
	if (elem == labels1.end())
		return empty;
	else
		return elem->second;
}

void* AnimationFile::getdata(const string& label)
{
	auto elem = labels2.find(label);
	if (elem == labels2.end())
		return nullptr;
	else
		return elem->second;
}

const std::unordered_map<std::string, void*>* AnimationFile::getlabels() const
{
	return &labels2;
}

static string getstring(istream& stream)
{
	auto start = stream.tellg();
	while (stream.get() != 0) {}
	const auto size = static_cast<size_t>(stream.tellg() - start);
	char* buf = new char[size];
	stream.seekg(start);
	stream.read(buf, size);
	string result(buf);
	delete[] buf;
	return result;
}

template <typename T>
static inline void fixptr(T*& ptr, intptr_t base)
{
	if (ptr != nullptr)
	{
		ptr = (T*)((uint8_t*)ptr + base);
	}
}

inline void fixmkeypointers(NJS_MKEY_P* mkey, intptr_t base, int count)
{
	for (int i = 0; i < count; i++)
		fixptr(mkey[i].key, base);
}

template <typename T>
inline void fixmdatapointers(T* mdata, intptr_t base, int count, int vertoff, int normoff)
{
	for (int c = 0; c < count; c++)
	{
		for (int i = 0; i < (int)LengthOfArray(mdata->p); i++)
		{
			fixptr(mdata->p[i], base);
			if (i == vertoff || i == normoff)
				fixmkeypointers((NJS_MKEY_P*)mdata->p[i], base, mdata->nb[i]);
		}

		++mdata;
	}
}

template <typename T>
static inline void readdata(istream& stream, T& data)
{
	stream.read((char*)&data, sizeof(T));
}

void AnimationFile::init(istream& stream)
{
	uint64_t magic;
	readdata(stream, magic);
	uint8_t version = magic >> 56;
	magic &= FormatMask;

	if (version > CurrentVersion) // unrecognized file version
	{
		return;
	}

	if (magic != SAANIM)
	{
		return;
	}

	uint32_t landtableoff;
	readdata(stream, landtableoff);
	landtableoff -= headersize;

	uint32_t tmpaddr;
	readdata(stream, tmpaddr);
	readdata(stream, modelcount);

	shortrot = modelcount < 0;
	modelcount &= INT32_MAX;

	size_t mdlsize = tmpaddr - headersize;
	auto motionbuf = new uint8_t[mdlsize];

	allocatedmem.push_back(shared_ptr<uint8_t>(motionbuf, default_delete<uint8_t[]>()));

	stream.read((char*)motionbuf, mdlsize);
	motion = (NJS_MOTION*)(motionbuf + landtableoff);

	intptr_t motionbase = (intptr_t)motionbuf - headersize;

	if (motion->mdata != nullptr)
	{
		motion->mdata = (uint8_t*)motion->mdata + motionbase;

		if (fixedpointers.find(motion->mdata) == fixedpointers.end())
		{
			fixedpointers.insert(motion->mdata);

			int vertoff = -1;
			int normoff = -1;

			if (motion->type & NJD_MTYPE_VERT_4)
			{
				vertoff = 0;
				if (motion->type & NJD_MTYPE_POS_0)
					++vertoff;
				if (motion->type & NJD_MTYPE_ANG_1)
					++vertoff;
				if (motion->type & NJD_MTYPE_QUAT_1)
					++vertoff;
				if (motion->type & NJD_MTYPE_SCL_2)
					++vertoff;
				if (motion->type & NJD_MTYPE_TARGET_3)
					++vertoff;
				if (motion->type & NJD_MTYPE_VEC_3)
					++vertoff;
			}
			if (motion->type & NJD_MTYPE_NORM_5)
			{
				normoff = 0;
				if (motion->type & NJD_MTYPE_POS_0)
					++normoff;
				if (motion->type & NJD_MTYPE_ANG_1)
					++normoff;
				if (motion->type & NJD_MTYPE_QUAT_1)
					++normoff;
				if (motion->type & NJD_MTYPE_SCL_2)
					++normoff;
				if (motion->type & NJD_MTYPE_TARGET_3)
					++normoff;
				if (motion->type & NJD_MTYPE_VEC_3)
					++normoff;
				if (motion->type & NJD_MTYPE_VERT_4)
					++normoff;
			}
			switch (motion->inp_fn & 0xF)
			{
			case 1:
				fixmdatapointers((NJS_MDATA1*)motion->mdata, motionbase, modelcount, vertoff, normoff);
				break;
			case 2:
				fixmdatapointers((NJS_MDATA2*)motion->mdata, motionbase, modelcount, vertoff, normoff);
				break;
			case 3:
				fixmdatapointers((NJS_MDATA3*)motion->mdata, motionbase, modelcount, vertoff, normoff);
				break;
			case 4:
				fixmdatapointers((NJS_MDATA4*)motion->mdata, motionbase, modelcount, vertoff, normoff);
				break;
			case 5:
				fixmdatapointers((NJS_MDATA5*)motion->mdata, motionbase, modelcount, vertoff, normoff);
				break;
			default:
				throw;
			}
		}
	}

	fixedpointers.clear();
	if (version < 2)
	{
		string label = getstring(stream);
		labels1[motion] = label;
		labels2[label] = motion;
	}
	else
	{
		uint32_t chunktype;
		readdata(stream, chunktype);
		while (chunktype != ChunkTypes_End)
		{
			uint32_t chunksz;
			readdata(stream, chunksz);
			auto chunkbase = stream.tellg();
			auto nextchunk = chunkbase + (streamoff)chunksz;
			switch (chunktype)
			{
			case ChunkTypes_Label:
				while (true)
				{
					void* dataptr;
					readdata(stream, dataptr);
					uint32_t labelptr;
					readdata(stream, labelptr);
					if (dataptr == (void*)-1 && labelptr == UINT32_MAX)
						break;
					dataptr = (uint8_t*)dataptr + motionbase;
					tmpaddr = (uint32_t)stream.tellg();
					stream.seekg((uint32_t)chunkbase + labelptr);
					string label = getstring(stream);
					stream.seekg(tmpaddr);
					labels1[dataptr] = label;
					labels2[label] = dataptr;
				}
				break;
			}
			stream.seekg(nextchunk);
			readdata(stream, chunktype);
		}
	}
}
