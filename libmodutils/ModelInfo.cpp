#include "stdafx.h"
#include "ModelInfo.h"
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

ModelInfo::ModelInfo(const char *filename)
{
	ifstream str(filename, ios::binary);
	init(str);
	str.close();
}

ModelInfo::ModelInfo(const string &filename)
{
	ifstream str(filename, ios::binary);
	init(str);
	str.close();
}

#ifdef _MSC_VER
ModelInfo::ModelInfo(const wchar_t *filename)
{
	ifstream str(filename, ios::binary);
	init(str);
	str.close();
}

ModelInfo::ModelInfo(const wstring &filename)
{
	ifstream str(filename, ios::binary);
	init(str);
	str.close();
}
#endif /* _MSC_VER */

ModelInfo::ModelInfo(istream &stream) { init(stream); }

ModelFormat ModelInfo::getformat() { return format; }

NJS_OBJECT *ModelInfo::getmodel() { return model; }

const string &ModelInfo::getauthor() { return author; }

const string &ModelInfo::gettool() { return tool; }

const string &ModelInfo::getdescription() { return description; }

const uint8_t *ModelInfo::getmetadata(uint32_t identifier, uint32_t &size)
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
const string &ModelInfo::getlabel(void *data)
{
	auto elem = labels1.find(data);
	if (elem == labels1.end())
		return empty;
	else
		return elem->second;
}

void *ModelInfo::getdata(const string &label)
{
	auto elem = labels2.find(label);
	if (elem == labels2.end())
		return nullptr;
	else
		return elem->second;
}

const std::unordered_map<string, void *> *ModelInfo::getlabels() { return &labels2; }

const list<string> &ModelInfo::getanimations() { return animations; }

const list<string> &ModelInfo::getmorphs() { return morphs; }

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

void ModelInfo::fixbasicmodelpointers(NJS_MODEL_SADX *model, intptr_t base)
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

void ModelInfo::fixchunkmodelpointers(NJS_CNK_MODEL *model, intptr_t base)
{
	fixptr(model->vlist, base);
	fixptr(model->plist, base);
}

void ModelInfo::fixobjectpointers(NJS_OBJECT *object, intptr_t base)
{
	if (object->model != nullptr)
	{
		object->model = (uint8_t *)object->model + base;
		if (format == ModelFormat_Basic)
		{
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
				fixbasicmodelpointers(object->basicdxmodel, base);
			}
		}
		else if (format == ModelFormat_Chunk && fixedpointers.find(object->model) == fixedpointers.end())
		{
			fixedpointers.insert(object->model);
			fixchunkmodelpointers(object->chunkmodel, base);
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
static inline void readdata(istream &stream, T &data)
{
	stream.read((char *)&data, sizeof(T));
}

void ModelInfo::init(istream &stream)
{
	uint64_t magic;
	readdata(stream, magic);
	uint8_t version = magic >> 56;
	magic &= FormatMask;
	if (version != CurrentVersion) // unrecognized file version
		return;
	switch (magic)
	{
	case SA1MDL:
		format = ModelFormat_Basic;
		break;
	case SA2MDL:
		format = ModelFormat_Chunk;
		break;
	default:
		return;
	}
	uint32_t modeloff;
	readdata(stream, modeloff);
	modeloff -= headersize;
	uint32_t tmpaddr;
	readdata(stream, tmpaddr);
	int mdlsize = tmpaddr - headersize;
	uint8_t *modelbuf = new uint8_t[mdlsize];
	allocatedmem.push_back(shared_ptr<uint8_t>(modelbuf, default_delete<uint8_t[]>()));
	stream.read((char *)modelbuf, mdlsize);
	model = (NJS_OBJECT *)(modelbuf + modeloff);
	intptr_t modelbase = (intptr_t)modelbuf - headersize;
	fixobjectpointers(model, modelbase);
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
				dataptr = (uint8_t *)dataptr + modelbase;
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
		case ChunkTypes_Animation:
			while (true)
			{
				uint32_t labelptr;
				readdata(stream, labelptr);
				if (labelptr == UINT32_MAX)
					break;
				tmpaddr = (uint32_t)stream.tellg();
				stream.seekg((uint32_t)chunkbase + labelptr);
				animations.push_back(getstring(stream));
				stream.seekg(tmpaddr);
			}
			break;
		case ChunkTypes_Morph:
			while (true)
			{
				uint32_t labelptr;
				readdata(stream, labelptr);
				if (labelptr == UINT32_MAX)
					break;
				tmpaddr = (uint32_t)stream.tellg();
				stream.seekg((uint32_t)chunkbase + labelptr);
				morphs.push_back(getstring(stream));
				stream.seekg(tmpaddr);
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
