#include "stdafx.h"
#include "LandTableInfo.h"
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

LandTableInfo::LandTableInfo(const char *filename)
{
	ifstream str(filename, ios::binary);
	init(str);
	str.close();
}

LandTableInfo::LandTableInfo(const string &filename)
{
	ifstream str(filename, ios::binary);
	init(str);
	str.close();
}

#ifdef _MSC_VER
LandTableInfo::LandTableInfo(const wchar_t *filename)
{
	ifstream str(filename, ios::binary);
	init(str);
	str.close();
}

LandTableInfo::LandTableInfo(const wstring &filename)
{
	ifstream str(filename, ios::binary);
	init(str);
	str.close();
}
#endif /* _MSC_VER */

LandTableInfo::LandTableInfo(istream &stream) { init(stream); }

LandTable *LandTableInfo::getlandtable() { return landtable; }

const string &LandTableInfo::getauthor() { return author; }

const string &LandTableInfo::gettool() { return tool; }

const string &LandTableInfo::getdescription() { return description; }

const uint8_t *LandTableInfo::getmetadata(uint32_t identifier, uint32_t &size)
{
	auto elem = metadata.find(identifier);
	if (elem == metadata.end())
	{
		size = 0;
		return nullptr;
	}
	else
	{
		size = elem->second.size;
		return elem->second.data;
	}
}

static const string empty;
const string &LandTableInfo::getlabel(void *data)
{
	auto elem = labels1.find(data);
	if (elem == labels1.end())
		return empty;
	else
		return elem->second;
}

void *LandTableInfo::getdata(const string &label)
{
	auto elem = labels2.find(label);
	if (elem == labels2.end())
		return nullptr;
	else
		return elem->second;
}

const std::unordered_map<string, void *> *LandTableInfo::getlabels()
{
	return &labels2;
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

void LandTableInfo::fixmodelpointers(NJS_MODEL_SADX *model, intptr_t base)
{
	fixptr(model->points, base);
	fixptr(model->normals, base);
	if (model->meshsets != nullptr)
	{
		fixptr(model->meshsets, base);
		if (reallocateddata.find(model->meshsets) != reallocateddata.end())
			model->meshsets = (NJS_MESHSET_SADX *)reallocateddata[model->meshsets];
		else
		{
			NJS_MESHSET_SADX *tmp = new NJS_MESHSET_SADX[model->nbMeshset];
			reallocateddata[model->meshsets] = tmp;
			for (int i = 0; i < model->nbMeshset; i++)
			{
				memcpy(&tmp[i], &((NJS_MESHSET *)model->meshsets)[i], sizeof(NJS_MESHSET));
				tmp[i].buffer = nullptr;
			}
			model->meshsets = tmp;
			allocatedmem.push_back(shared_ptr<NJS_MESHSET_SADX>(model->meshsets, default_delete<NJS_MESHSET_SADX[]>()));
			for (int i = 0; i < model->nbMeshset; i++)
			{
				fixptr(model->meshsets[i].meshes, base);
				fixptr(model->meshsets[i].attrs, base);
				fixptr(model->meshsets[i].normals, base);
				fixptr(model->meshsets[i].vertcolor, base);
				fixptr(model->meshsets[i].vertuv, base);
			}
		}
	}
	fixptr(model->mats, base);
}

void LandTableInfo::fixobjectpointers(NJS_OBJECT *object, intptr_t base)
{
	if (object->model != nullptr)
	{
		object->model = (uint8_t *)object->model + base;
		if (reallocateddata.find(object->model) != reallocateddata.end())
			object->model = reallocateddata[object->model];
		else
		{
			NJS_MODEL_SADX *tmp = new NJS_MODEL_SADX;
			reallocateddata[object->model] = tmp;
			memcpy(tmp, object->model, sizeof(NJS_MODEL));
			tmp->buffer = nullptr;
			object->basicdxmodel = tmp;
			allocatedmem.push_back(shared_ptr<NJS_MODEL_SADX>(object->basicdxmodel, default_delete<NJS_MODEL_SADX>()));
			fixmodelpointers(object->basicdxmodel, base);
		}
	}
	if (object->child != nullptr)
	{
		object->child = (NJS_OBJECT *)((uint8_t *)object->child + base);
		if (fixedpointers.find(object->child) == fixedpointers.end())
		{
			fixedpointers.insert(object->child);
			fixobjectpointers(object->child, base);
		}
	}
	if (object->sibling != nullptr)
	{
		object->sibling = (NJS_OBJECT *)((uint8_t *)object->sibling + base);
		if (fixedpointers.find(object->sibling) == fixedpointers.end())
		{
			fixedpointers.insert(object->sibling);
			fixobjectpointers(object->sibling, base);
		}
	}
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

void LandTableInfo::fixmotionpointers(NJS_MOTION *motion, intptr_t base, int count)
{
	if (motion->mdata != nullptr)
	{
		motion->mdata = (uint8_t *)motion->mdata + base;
		if (fixedpointers.find(motion->mdata) == fixedpointers.end())
		{
			fixedpointers.insert(motion->mdata);
			switch (motion->inp_fn & 0xF)
			{
			case 1:
				fixmdatapointers((NJS_MDATA1 *)motion->mdata, base, count);
				break;
			case 2:
				fixmdatapointers((NJS_MDATA2 *)motion->mdata, base, count);
				break;
			case 3:
				fixmdatapointers((NJS_MDATA3 *)motion->mdata, base, count);
				break;
			case 4:
				fixmdatapointers((NJS_MDATA4 *)motion->mdata, base, count);
				break;
			case 5:
				fixmdatapointers((NJS_MDATA5 *)motion->mdata, base, count);
				break;
			}
		}
	}
}

void LandTableInfo::fixactionpointers(NJS_ACTION *action, intptr_t base)
{
	int count = 0;
	if (action->object != nullptr)
	{
		action->object = (NJS_OBJECT *)((uint8_t *)action->object + base);
		if (fixedpointers.find(action->object) == fixedpointers.end())
		{
			fixedpointers.insert(action->object);
			fixobjectpointers(action->object, base);
		}
		count = action->object->countanimated();
	}
	if (action->motion != nullptr)
	{
		action->motion = (NJS_MOTION *)((uint8_t *)action->motion + base);
		if (fixedpointers.find(action->motion) == fixedpointers.end())
		{
			fixedpointers.insert(action->motion);
			fixmotionpointers(action->motion, base, count);
		}
	}
}

void LandTableInfo::fixlandtablepointers(LandTable *landtable, intptr_t base)
{
	if (landtable->Col != nullptr)
	{
		landtable->Col = (COL *)((uint8_t *)landtable->Col + base);
		for (int i = 0; i < landtable->COLCount; i++)
			if (landtable->Col[i].Model != nullptr)
			{
				landtable->Col[i].Model = (NJS_OBJECT *)((uint8_t *)landtable->Col[i].Model + base);
				if (fixedpointers.find(landtable->Col[i].Model) == fixedpointers.end())
				{
					fixedpointers.insert(landtable->Col[i].Model);
					fixobjectpointers(landtable->Col[i].Model, base);
				}
			}
	}
	if (landtable->AnimData != nullptr)
	{
		landtable->AnimData = (GeoAnimData *)((uint8_t *)landtable->AnimData + base);
		for (int i = 0; i < landtable->AnimCount; i++)
		{
			if (landtable->AnimData[i].Model != nullptr)
			{
				landtable->AnimData[i].Model = (NJS_OBJECT *)((uint8_t *)landtable->AnimData[i].Model + base);
				if (fixedpointers.find(landtable->AnimData[i].Model) == fixedpointers.end())
				{
					fixedpointers.insert(landtable->AnimData[i].Model);
					fixobjectpointers(landtable->AnimData[i].Model, base);
				}
			}
			if (landtable->AnimData[i].Animation != nullptr)
			{
				landtable->AnimData[i].Animation = (NJS_ACTION *)((uint8_t *)landtable->AnimData[i].Animation + base);
				if (fixedpointers.find(landtable->AnimData[i].Animation) == fixedpointers.end())
				{
					fixedpointers.insert(landtable->AnimData[i].Animation);
					fixactionpointers(landtable->AnimData[i].Animation, base);
				}
			}
		}
	}
	fixptr(landtable->TexName, base);
}

template<typename T>
static inline void readdata(istream &stream, T &data)
{
	stream.read((char *)&data, sizeof(T));
}

void LandTableInfo::init(istream &stream)
{
	uint64_t magic;
	readdata(stream, magic);
	uint8_t version = magic >> 56;
	magic &= FormatMask;
	if (version != CurrentVersion) // unrecognized file version
		return;
	if (magic != SA1LVL)
		return;
	uint32_t landtableoff;
	readdata(stream, landtableoff);
	landtableoff -= headersize;
	uint32_t tmpaddr;
	readdata(stream, tmpaddr);
	int mdlsize = tmpaddr - headersize;
	uint8_t *landtablebuf = new uint8_t[mdlsize];
	allocatedmem.push_back(shared_ptr<uint8_t>(landtablebuf, default_delete<uint8_t[]>()));
	stream.read((char *)landtablebuf, mdlsize);
	landtable = (LandTable *)(landtablebuf + landtableoff);
	intptr_t landtablebase = (intptr_t)landtablebuf - headersize;
	fixlandtablepointers(landtable, landtablebase);
	fixedpointers.clear();
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
				void *dataptr;
				readdata(stream, dataptr);
				uint32_t labelptr;
				readdata(stream, labelptr);
				if (dataptr == (void *)-1 && labelptr == UINT32_MAX)
					break;
				dataptr = (uint8_t *)dataptr + landtablebase;
				if (reallocateddata.find(dataptr) != reallocateddata.end())
					dataptr = reallocateddata[dataptr];
				tmpaddr = (uint32_t)stream.tellg();
				stream.seekg((uint32_t)chunkbase + labelptr);
				string label = getstring(stream);
				stream.seekg(tmpaddr);
				labels1[dataptr] = label;
				labels2[label] = dataptr;
			}
			break;
		case ChunkTypes_Author:
			author = getstring(stream);
			break;
		case ChunkTypes_Tool:
			tool = getstring(stream);
			break;
		case ChunkTypes_Description:
			description = getstring(stream);
			break;
		default:
			uint8_t *buf = new uint8_t[chunksz];
			allocatedmem.push_back(shared_ptr<uint8_t>(buf, default_delete<uint8_t[]>()));
			stream.read((char *)buf, chunksz);
			Metadata meta = { chunksz, buf };
			metadata[chunktype] = meta;
			break;
		}
		stream.seekg(nextchunk);
		readdata(stream, chunktype);
	}
	reallocateddata.clear();
}
