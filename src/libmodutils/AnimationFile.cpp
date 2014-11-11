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

AnimationFile::AnimationFile(const char *filename)
{
	ifstream str(filename, ios::binary);
	init(str);
	str.close();
}

#ifdef _MSC_VER
AnimationFile::AnimationFile(const wchar_t *filename)
{
	ifstream str(filename, ios::binary);
	init(str);
	str.close();
}
#endif /* _MSC_VER */

AnimationFile::AnimationFile(const string &filename)
{
	ifstream str(filename, ios::binary);
	init(str);
	str.close();
}

#ifdef _MSC_VER
AnimationFile::AnimationFile(const wstring &filename)
{
	ifstream str(filename, ios::binary);
	init(str);
	str.close();
}
#endif /* _MSC_VER */

AnimationFile::AnimationFile(istream &stream) { init(stream); }

NJS_MOTION *AnimationFile::getmotion() { return motion; }

int AnimationFile::getmodelcount() { return modelcount; }

const string &AnimationFile::getlabel()
{
	return label;
}

static string getstring(istream &stream)
{
	auto start = stream.tellg();
	while (stream.get() != 0)
		;
	auto size = stream.tellg() - start;
	char *buf = new char[(unsigned int)size];
	stream.seekg(start);
	stream.read(buf, size);
	string result(buf);
	delete[] buf;
	return result;
}

template<typename T>
static inline void fixptr(T *&ptr, intptr_t base)
{
	if (ptr != nullptr)
		ptr = (T *)((uint8_t *)ptr + base);
}

template<typename T>
inline void fixmdatapointers(T *mdata, intptr_t base, int count)
{
	for (int c = 0; c < count; c++)
	{
		for (int i = 0; i < (int)LengthOfArray(mdata->p); i++)
			fixptr(mdata->p[i], base);
		mdata++;
	}
}

template<typename T>
static inline void readdata(istream &stream, T &data)
{
	stream.read((char *)&data, sizeof(T));
}

void AnimationFile::init(istream &stream)
{
	uint64_t magic;
	readdata(stream, magic);
	uint8_t version = magic >> 56;
	magic &= FormatMask;
	if (version != CurrentVersion) // unrecognized file version
		return;
	if (magic != SAANIM)
		return;
	uint32_t landtableoff;
	readdata(stream, landtableoff);
	landtableoff -= headersize;
	uint32_t tmpaddr;
	readdata(stream, tmpaddr);
	readdata(stream, modelcount);
	int mdlsize = tmpaddr - headersize;
	uint8_t *motionbuf = new uint8_t[mdlsize];
	allocatedmem.push_back(shared_ptr<uint8_t>(motionbuf, default_delete<uint8_t[]>()));
	stream.read((char *)motionbuf, mdlsize);
	motion = (NJS_MOTION *)(motionbuf + landtableoff);
	intptr_t motionbase = (intptr_t)motionbuf - headersize;
	if (motion->mdata != nullptr)
	{
		motion->mdata = (uint8_t *)motion->mdata + motionbase;
		if (fixedpointers.find(motion->mdata) == fixedpointers.end())
		{
			fixedpointers.insert(motion->mdata);
			switch (motion->inp_fn & 0xF)
			{
			case 1:
				fixmdatapointers((NJS_MDATA1 *)motion->mdata, motionbase, modelcount);
				break;
			case 2:
				fixmdatapointers((NJS_MDATA2 *)motion->mdata, motionbase, modelcount);
				break;
			case 3:
				fixmdatapointers((NJS_MDATA3 *)motion->mdata, motionbase, modelcount);
				break;
			case 4:
				fixmdatapointers((NJS_MDATA4 *)motion->mdata, motionbase, modelcount);
				break;
			case 5:
				fixmdatapointers((NJS_MDATA5 *)motion->mdata, motionbase, modelcount);
				break;
			}
		}
	}
	fixedpointers.clear();
	label = getstring(stream);
}