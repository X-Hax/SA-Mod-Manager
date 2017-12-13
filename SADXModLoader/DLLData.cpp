#include "stdafx.h"
#include <unordered_map>
#include <vector>
#include <string>
#include "IniFile.hpp"
#include "TextConv.hpp"
#include "SADXModLoader.h"
#include "LandTableInfo.h"
#include "ModelInfo.h"
#include "AnimationFile.h"
#include "DLLData.h"

using std::unordered_map;
using std::vector;
using std::string;
using std::wstring;

static unordered_map<string, void *> dlllabels;
static unordered_map<NJS_TEXLIST *, NJS_TEXLIST *> dlltexlists;

static void LoadDLLLandTable(const wstring &path)
{
	LandTableInfo *info = new LandTableInfo(path);
	LandTable *land = info->getlandtable();
	auto tl = dlltexlists.find(land->TexList);
	if (tl != dlltexlists.end())
		land->TexList = tl->second;
	auto labels = info->getlabels();
	for (auto iter = labels->cbegin(); iter != labels->cend(); ++iter)
		dlllabels[iter->first] = iter->second;
}

static void LoadDLLModel(const wstring &path)
{
	ModelInfo *info = new ModelInfo(path);
	auto labels = info->getlabels();
	for (auto iter = labels->cbegin(); iter != labels->cend(); ++iter)
		dlllabels[iter->first] = iter->second;
}

static void LoadDLLAnimation(const wstring &path)
{
	AnimationFile *info = new AnimationFile(path);
	dlllabels[info->getlabel()] = info->getmotion();
}

typedef void(__cdecl *dllfilefunc_t)(const wstring &path);
static const unordered_map<string, dllfilefunc_t> dllfilefuncmap = {
	{ "landtable", LoadDLLLandTable },
	{ "model", LoadDLLModel },
	{ "basicdxmodel", LoadDLLModel },
	{ "chunkmodel", LoadDLLModel },
	{ "animation", LoadDLLAnimation }
};

static void ProcessLandTableDLL(const IniGroup *group, void *exp)
{
	memcpy(exp, dlllabels[group->getString("Label")], sizeof(LandTable));
}

static void ProcessLandTableArrayDLL(const IniGroup *group, void *exp)
{
	((LandTable **)exp)[group->getInt("Index")] = (LandTable *)dlllabels[group->getString("Label")];
}

static void ProcessModelDLL(const IniGroup *group, void *exp)
{
	memcpy(exp, dlllabels[group->getString("Label")], sizeof(NJS_OBJECT));
}

static void ProcessModelArrayDLL(const IniGroup *group, void *exp)
{
	((NJS_OBJECT **)exp)[group->getInt("Index")] = (NJS_OBJECT *)dlllabels[group->getString("Label")];
}

static void ProcessMorphDLL(const IniGroup *group, void *exp)
{
	// won't work for chunk models, but no DLL exports this type of data anyway
	memcpy(exp, dlllabels[group->getString("Label")], sizeof(NJS_MODEL_SADX));
}

static void ProcessModelsArrayDLL(const IniGroup *group, void *exp)
{
	((NJS_MODEL_SADX **)exp)[group->getInt("Index")] = (NJS_MODEL_SADX *)dlllabels[group->getString("Label")];
}

static void ProcessActionArrayDLL(const IniGroup *group, void *exp)
{
	string field = group->getString("Field");
	NJS_ACTION *act = ((NJS_ACTION **)exp)[group->getInt("Index")];
	if (field == "object")
		act->object = (NJS_OBJECT *)dlllabels[group->getString("Label")];
	else if (field == "motion")
		act->motion = (NJS_MOTION *)dlllabels[group->getString("Label")];
}

typedef void(__cdecl *dlldatafunc_t)(const IniGroup *group, void *exp);
static const unordered_map<string, dlldatafunc_t> dlldatafuncmap = {
	{ "landtable", ProcessLandTableDLL },
	{ "landtablearray", ProcessLandTableArrayDLL },
	{ "model", ProcessModelDLL },
	{ "modelarray", ProcessModelArrayDLL },
	{ "basicdxmodel", ProcessModelDLL },
	{ "basicdxmodelarray", ProcessModelArrayDLL },
	{ "chunkmodel", ProcessModelDLL },
	{ "chunkmodelarray", ProcessModelArrayDLL },
	{ "morph", ProcessMorphDLL },
	{ "modelsarray", ProcessModelsArrayDLL },
	{ "actionarray", ProcessActionArrayDLL },
};

static const char *const dlldatakeys[] = {
	"CHRMODELSData",
	"ADV00MODELSData",
	"ADV01MODELSData",
	"ADV01CMODELSData",
	"ADV02MODELSData",
	"ADV03MODELSData",
	"BOSSCHAOS0MODELSData",
	"CHAOSTGGARDEN02MR_DAYTIMEData",
	"CHAOSTGGARDEN02MR_EVENINGData",
	"CHAOSTGGARDEN02MR_NIGHTData"
};

static unordered_map<wstring, HMODULE> dllhandles;

struct dllexportinfo { void *address; string type; };
struct dllexportcontainer { unordered_map<string, dllexportinfo> exports; };
static unordered_map<wstring, dllexportcontainer> dllexports;

static vector<string> &split(const string &s, char delim, vector<string> &elems)
{
	std::stringstream ss(s);
	string item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}


static vector<string> split(const string &s, char delim)
{
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

void ProcessDLLData(const wchar_t *filename, const wstring &mod_dir)
{
	const IniFile *const dlldata = new IniFile(filename);
	const IniGroup *group;
	wstring dllname = dlldata->getWString("", "name");
	HMODULE dllhandle;
	if (dllhandles.find(dllname) != dllhandles.cend())
		dllhandle = dllhandles[dllname];
	else
	{
		dllhandle = GetModuleHandle(dllname.c_str());
		dllhandles[dllname] = dllhandle;
	}
	if (dllexports.find(dllname) == dllexports.end())
	{
		group = dlldata->getGroup("Exports");
		dllexportcontainer exp;
		for (auto iter = group->cbegin(); iter != group->cend(); ++iter)
		{
			dllexportinfo inf;
			inf.address = GetProcAddress(dllhandle, iter->first.c_str());
			inf.type = iter->second;
			exp.exports[iter->first] = inf;
		}
		dllexports[dllname] = exp;
	}
	const auto exports = &dllexports[dllname].exports;
	dlltexlists.clear();
	if (dlldata->hasGroup("TexLists"))
	{
		group = dlldata->getGroup("TexLists");
		for (auto iter = group->cbegin(); iter != group->cend(); ++iter)
		{
			NJS_TEXLIST *key = (NJS_TEXLIST*)std::stoul(iter->first, nullptr, 16);
			vector<string> valstr = split(iter->second, ',');
			NJS_TEXLIST *value;
			if (valstr.size() > 1)
				value = ((NJS_TEXLIST**)(*exports)[valstr[0]].address)[std::stoul(valstr[1])];
			else
				value = (NJS_TEXLIST*)(*exports)[valstr[0]].address;
			dlltexlists[key] = value;
		}
	}
	dlllabels.clear();
	group = dlldata->getGroup("Files");
	for (auto iter = group->cbegin(); iter != group->cend(); ++iter)
	{
		auto type = dllfilefuncmap.find(split(iter->second, '|')[0]);
		if (type != dllfilefuncmap.end())
			type->second(mod_dir + L'\\' + MBStoUTF16(iter->first, CP_UTF8));
	}
	char buf[16];
	for (unsigned int k = 0; k < 9999; k++)
	{
		snprintf(buf, sizeof(buf), "Item%u", k);
		if (dlldata->hasGroup(buf))
		{
			group = dlldata->getGroup(buf);
			const dllexportinfo &exp = (*exports)[group->getString("Export")];
			auto type = dlldatafuncmap.find(exp.type);
			if (type != dlldatafuncmap.end())
				type->second(group, exp.address);
		}
	}
	delete dlldata;
}

void SetChrmodelsDLLHandle(HMODULE handle)
{
	dllhandles[L"CHRMODELS_orig"] = handle;
	dllhandles[L"CHRMODELS"] = handle;
}