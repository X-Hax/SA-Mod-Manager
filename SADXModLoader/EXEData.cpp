#include "stdafx.h"
#include <Shlwapi.h>
#include <cassert>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include "IniFile.hpp"
#include "TextConv.hpp"
#include "SADXModLoader.h"
#include "LandTableInfo.h"
#include "ModelInfo.h"
#include "AnimationFile.h"
#include "EXEData.h"

using std::unordered_map;
using std::vector;
using std::string;
using std::wstring;
using std::ifstream;

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

static string trim(const string &s)
{
	auto st = s.find_first_not_of(' ');
	if (st == string::npos)
		st = 0;
	auto ed = s.find_last_not_of(' ');
	if (ed == string::npos)
		ed = s.size() - 1;
	return s.substr(st, (ed + 1) - st);
}

template<typename T>
static inline T *arrcpy(T *dst, const T *src, size_t cnt)
{
	return (T *)memcpy(dst, src, cnt * sizeof(T));
}

template<typename T>
static inline void clrmem(T *mem)
{
	ZeroMemory(mem, sizeof(T));
}

static const unordered_map<string, uint8_t> levelidsnamemap = {
	{ "hedgehoghammer", LevelIDs_HedgehogHammer },
	{ "emeraldcoast", LevelIDs_EmeraldCoast },
	{ "windyvalley", LevelIDs_WindyValley },
	{ "twinklepark", LevelIDs_TwinklePark },
	{ "speedhighway", LevelIDs_SpeedHighway },
	{ "redmountain", LevelIDs_RedMountain },
	{ "skydeck", LevelIDs_SkyDeck },
	{ "lostworld", LevelIDs_LostWorld },
	{ "icecap", LevelIDs_IceCap },
	{ "casinopolis", LevelIDs_Casinopolis },
	{ "finalegg", LevelIDs_FinalEgg },
	{ "hotshelter", LevelIDs_HotShelter },
	{ "chaos0", LevelIDs_Chaos0 },
	{ "chaos2", LevelIDs_Chaos2 },
	{ "chaos4", LevelIDs_Chaos4 },
	{ "chaos6", LevelIDs_Chaos6 },
	{ "perfectchaos", LevelIDs_PerfectChaos },
	{ "egghornet", LevelIDs_EggHornet },
	{ "eggwalker", LevelIDs_EggWalker },
	{ "eggviper", LevelIDs_EggViper },
	{ "zero", LevelIDs_Zero },
	{ "e101", LevelIDs_E101 },
	{ "e101r", LevelIDs_E101R },
	{ "stationsquare", LevelIDs_StationSquare },
	{ "eggcarrieroutside", LevelIDs_EggCarrierOutside },
	{ "eggcarrierinside", LevelIDs_EggCarrierInside },
	{ "mysticruins", LevelIDs_MysticRuins },
	{ "past", LevelIDs_Past },
	{ "twinklecircuit", LevelIDs_TwinkleCircuit },
	{ "skychase1", LevelIDs_SkyChase1 },
	{ "skychase2", LevelIDs_SkyChase2 },
	{ "sandhill", LevelIDs_SandHill },
	{ "ssgarden", LevelIDs_SSGarden },
	{ "ecgarden", LevelIDs_ECGarden },
	{ "mrgarden", LevelIDs_MRGarden },
	{ "chaorace", LevelIDs_ChaoRace },
	{ "invalid", LevelIDs_Invalid }
};

static uint8_t ParseLevelID(const string &str)
{
	string str2 = trim(str);
	transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
	auto lv = levelidsnamemap.find(str2);
	if (lv != levelidsnamemap.end())
		return lv->second;
	else
		return (uint8_t)strtol(str.c_str(), nullptr, 10);
}

static uint16_t ParseLevelAndActID(const string &str)
{
	if (str.size() == 4)
	{
		const char *cstr = str.c_str();
		char buf[3];
		buf[2] = 0;
		memcpy(buf, cstr, 2);
		uint16_t result = (uint16_t)(strtol(buf, nullptr, 10) << 8);
		memcpy(buf, cstr + 2, 2);
		return result | (uint16_t)strtol(buf, nullptr, 10);
	}
	else
	{
		vector<string> strs = split(str, ' ');
		return (uint16_t)((ParseLevelID(strs[0]) << 8) | strtol(strs[1].c_str(), nullptr, 10));
	}
}

static const unordered_map<string, uint8_t> charflagsnamemap = {
	{ "sonic", CharacterFlags_Sonic },
	{ "eggman", CharacterFlags_Eggman },
	{ "tails", CharacterFlags_Tails },
	{ "knuckles", CharacterFlags_Knuckles },
	{ "tikal", CharacterFlags_Tikal },
	{ "amy", CharacterFlags_Amy },
	{ "gamma", CharacterFlags_Gamma },
	{ "big", CharacterFlags_Big }
};

static uint8_t ParseCharacterFlags(const string &str)
{
	vector<string> strflags = split(str, ',');
	uint8_t flag = 0;
	for (auto iter = strflags.cbegin(); iter != strflags.cend(); ++iter)
	{
		string s = trim(*iter);
		transform(s.begin(), s.end(), s.begin(), ::tolower);
		auto ch = charflagsnamemap.find(s);
		if (ch != charflagsnamemap.end())
			flag |= ch->second;
	}
	return flag;
}

static const unordered_map<string, uint8_t> languagesnamemap = {
	{ "japanese", Languages_Japanese },
	{ "english", Languages_English },
	{ "french", Languages_French },
	{ "spanish", Languages_Spanish },
	{ "german", Languages_German }
};

static uint8_t ParseLanguage(const string &str)
{
	string str2 = trim(str);
	transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
	auto lv = languagesnamemap.find(str2);
	if (lv != languagesnamemap.end())
		return lv->second;
	return Languages_Japanese;
}

static string DecodeUTF8(const string &str, int language)
{
	if (language <= Languages_English)
		return UTF8toSJIS(str);
	else
		return UTF8to1252(str);
}

static string UnescapeNewlines(const string &str)
{
	string result;
	result.reserve(str.size());
	for (unsigned int c = 0; c < str.size(); c++)
		switch (str[c])
		{
		case '\\': // escape character
			if (c + 1 == str.size())
			{
				result.push_back(str[c]);
				continue;
			}
			switch (str[++c])
			{
			case 'n': // line feed
				result.push_back('\n');
				break;
			case 'r': // carriage return
				result.push_back('\r');
				break;
			default: // literal character
				result.push_back(str[c]);
				break;
			}
			break;
		default:
			result.push_back(str[c]);
			break;
		}
	return result;
}

static void ParseVertex(const string &str, NJS_VECTOR &vert)
{
	vector<string> vals = split(str, ',');
	assert(vals.size() == 3);
	if (vals.size() < 3)
		return;
	vert.x = (float)strtod(vals[0].c_str(), nullptr);
	vert.y = (float)strtod(vals[1].c_str(), nullptr);
	vert.z = (float)strtod(vals[2].c_str(), nullptr);
}

static void ParseRotation(const string str, Rotation &rot)
{
	vector<string> vals = split(str, ',');
	assert(vals.size() == 3);
	if (vals.size() < 3)
		return;
	rot.x = (int)strtol(vals[0].c_str(), nullptr, 16);
	rot.y = (int)strtol(vals[1].c_str(), nullptr, 16);
	rot.z = (int)strtol(vals[2].c_str(), nullptr, 16);
}

template<typename T>
static void ProcessPointerList(const string &list, T *item)
{
	vector<string> ptrs = split(list, ',');
	for (unsigned int i = 0; i < ptrs.size(); i++)
		WriteData((T **)(strtol(ptrs[i].c_str(), nullptr, 16) + 0x400000), item);
}

static void ProcessLandTableINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	LandTableInfo *const landtableinfo = new LandTableInfo(filename);
	LandTable *const landtable = landtableinfo->getlandtable();
	ProcessPointerList(group->getString("pointer"), landtable);
}

static unordered_map<string, NJS_OBJECT *> inimodels;
static void GetModelLabels(ModelInfo *mdlinf, NJS_OBJECT *obj)
{
	while (obj)
	{
		inimodels[mdlinf->getlabel(obj)] = obj;
		if (obj->child)
			GetModelLabels(mdlinf, obj->child);
		obj = obj->sibling;
	}
}

static void ProcessModelINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	ModelInfo *const mdlinf = new ModelInfo(filename);
	NJS_OBJECT *model = mdlinf->getmodel();
	GetModelLabels(mdlinf, model);
	ProcessPointerList(group->getString("pointer"), model);
}

static void ProcessActionINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	NJS_ACTION *action = new NJS_ACTION;
	AnimationFile *const animationFile = new AnimationFile(filename);
	action->motion = animationFile->getmotion();
	action->object = inimodels.find(group->getString("model"))->second;
	ProcessPointerList(group->getString("pointer"), action);
}

static void ProcessAnimationINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	AnimationFile *const animationFile = new AnimationFile(filename);
	NJS_MOTION *animation = animationFile->getmotion();
	ProcessPointerList(group->getString("pointer"), animation);
}

static void ProcessObjListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile *const objlistdata = new IniFile(filename);
	vector<ObjectListEntry> objs;
	for (unsigned int i = 0; i < 999; i++)
	{
		char key[8];
		snprintf(key, sizeof(key), "%u", i);
		if (!objlistdata->hasGroup(key)) break;
		const IniGroup *objdata = objlistdata->getGroup(key);
		ObjectListEntry entry;
		// TODO: Make these ini fields match structure names
		entry.Flags = objdata->getInt("Arg1");
		entry.ObjectListIndex = objdata->getInt("Arg2");
		entry.UseDistance = objdata->getInt("Flags");
		entry.Distance = objdata->getFloat("Distance");
		entry.LoadSub = (ObjectFuncPtr)objdata->getIntRadix("Code", 16);
		entry.Name = UTF8toSJIS(objdata->getString("Name").c_str());
		objs.push_back(entry);
	}
	delete objlistdata;
	ObjectList *list = new ObjectList;
	list->Count = objs.size();
	list->List = new ObjectListEntry[list->Count];
	arrcpy(list->List, objs.data(), list->Count);
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessStartPosINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile *const startposdata = new IniFile(filename);
	vector<StartPosition> poss;
	for (auto iter = startposdata->cbegin(); iter != startposdata->cend(); ++iter)
	{
		if (iter->first.empty()) continue;
		StartPosition pos;
		uint16_t levelact = ParseLevelAndActID(iter->first);
		pos.LevelID = (int16_t)(levelact >> 8);
		pos.ActID = (int16_t)(levelact & 0xFF);
		ParseVertex(iter->second->getString("Position", "0,0,0"), pos.Position);
		pos.YRot = iter->second->getIntRadix("YRotation", 16);
		poss.push_back(pos);
	}
	delete startposdata;
	auto numents = poss.size();
	StartPosition *list = new StartPosition[numents + 1];
	arrcpy(list, poss.data(), numents);
	clrmem(&list[numents]);
	list[numents].LevelID = LevelIDs_Invalid;
	ProcessPointerList(group->getString("pointer"), list);
}

static vector<PVMEntry> ProcessTexListINI_Internal(const IniFile *texlistdata)
{
	vector<PVMEntry> texs;
	for (unsigned int i = 0; i < 999; i++)
	{
		char key[8];
		snprintf(key, sizeof(key), "%u", i);
		if (!texlistdata->hasGroup(key)) break;
		const IniGroup *pvmdata = texlistdata->getGroup(key);
		PVMEntry entry;
		entry.Name = strdup(pvmdata->getString("Name").c_str());
		entry.TexList = (NJS_TEXLIST *)pvmdata->getIntRadix("Textures", 16);
		texs.push_back(entry);
	}
	return texs;
}

static void ProcessTexListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile *const texlistdata = new IniFile(filename);
	vector<PVMEntry> texs = ProcessTexListINI_Internal(texlistdata);
	delete texlistdata;
	auto numents = texs.size();
	PVMEntry *list = new PVMEntry[numents + 1];
	arrcpy(list, texs.data(), numents);
	clrmem(&list[numents]);
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessLevelTexListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile *const texlistdata = new IniFile(filename);
	vector<PVMEntry> texs = ProcessTexListINI_Internal(texlistdata);
	auto numents = texs.size();
	PVMEntry *list = new PVMEntry[numents];
	arrcpy(list, texs.data(), numents);
	LevelPVMList *lvl = new LevelPVMList;
	lvl->Level = (int16_t)ParseLevelAndActID(texlistdata->getString("", "Level", "0000"));
	delete texlistdata;
	lvl->NumTextures = (int16_t)numents;
	lvl->PVMList = list;
	ProcessPointerList(group->getString("pointer"), lvl);
}

static void ProcessTrialLevelListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("address")) return;
	ifstream fstr(mod_dir + L'\\' + group->getWString("filename"));
	vector<TrialLevelListEntry> lvls;
	while (fstr.good())
	{
		string str;
		getline(fstr, str);
		if (!str.empty())
		{
			TrialLevelListEntry ent;
			uint16_t lvl = ParseLevelAndActID(str);
			ent.Level = (char)(lvl >> 8);
			ent.Act = (char)lvl;
			lvls.push_back(ent);
		}
	}
	fstr.close();
	auto numents = lvls.size();
	TrialLevelList *list = (TrialLevelList*)(group->getIntRadix("address", 16) + 0x400000);
	list->Levels = new TrialLevelListEntry[numents];
	arrcpy(list->Levels, lvls.data(), numents);
	list->Count = (int)numents;
}

static void ProcessBossLevelListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	ifstream fstr(mod_dir + L'\\' + group->getWString("filename"));
	vector<uint16_t> lvls;
	while (fstr.good())
	{
		string str;
		getline(fstr, str);
		if (!str.empty())
			lvls.push_back(ParseLevelAndActID(str));
	}
	fstr.close();
	auto numents = lvls.size();
	uint16_t *list = new uint16_t[numents + 1];
	arrcpy(list, lvls.data(), numents);
	list[numents] = levelact(LevelIDs_Invalid, 0);
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessFieldStartPosINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile *const startposdata = new IniFile(filename);
	vector<FieldStartPosition> poss;
	for (auto iter = startposdata->cbegin(); iter != startposdata->cend(); ++iter)
	{
		if (iter->first.empty()) continue;
		FieldStartPosition pos = { ParseLevelID(iter->first) };
		pos.FieldID = ParseLevelID(iter->second->getString("Field", "Invalid"));
		ParseVertex(iter->second->getString("Position", "0,0,0"), pos.Position);
		pos.YRot = iter->second->getIntRadix("YRotation", 16);
		poss.push_back(pos);
	}
	delete startposdata;
	auto numents = poss.size();
	FieldStartPosition *list = new FieldStartPosition[numents + 1];
	arrcpy(list, poss.data(), numents);
	clrmem(&list[numents]);
	list[numents].LevelID = LevelIDs_Invalid;
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessSoundTestListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("address")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile *const inidata = new IniFile(filename);
	vector<SoundTestEntry> sounds;
	for (unsigned int i = 0; i < 999; i++)
	{
		char key[8];
		snprintf(key, sizeof(key), "%u", i);
		if (!inidata->hasGroup(key)) break;
		const IniGroup *snddata = inidata->getGroup(key);
		SoundTestEntry entry;
		entry.Name = UTF8toSJIS(snddata->getString("Title").c_str());
		entry.ID = snddata->getInt("Track");
		sounds.push_back(entry);
	}
	delete inidata;
	auto numents = sounds.size();
	SoundTestCategory *cat = (SoundTestCategory*)(group->getIntRadix("address", 16) + 0x400000);;
	cat->Entries = new SoundTestEntry[numents];
	arrcpy(cat->Entries, sounds.data(), numents);
	cat->Count = (int)numents;
}

static void ProcessMusicListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("address")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile *const inidata = new IniFile(filename);
	vector<MusicInfo> songs;
	for (unsigned int i = 0; i < 999; i++)
	{
		char key[8];
		snprintf(key, sizeof(key), "%u", i);
		if (!inidata->hasGroup(key)) break;
		const IniGroup *musdata = inidata->getGroup(key);
		MusicInfo entry;
		entry.Name = strdup(musdata->getString("Filename").c_str());
		entry.Loop = (int)musdata->getBool("Loop");
		songs.push_back(entry);
	}
	delete inidata;
	auto numents = songs.size();
	arrcpy((MusicInfo*)(group->getIntRadix("address", 16) + 0x400000), songs.data(), numents);
}

static void ProcessSoundListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("address")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile *const inidata = new IniFile(filename);
	vector<SoundFileInfo> sounds;
	for (unsigned int i = 0; i < 999; i++)
	{
		char key[8];
		snprintf(key, sizeof(key), "%u", i);
		if (!inidata->hasGroup(key)) break;
		const IniGroup *snddata = inidata->getGroup(key);
		SoundFileInfo entry;
		entry.Bank = snddata->getInt("Bank");
		entry.Filename = strdup(snddata->getString("Filename").c_str());
		sounds.push_back(entry);
	}
	delete inidata;
	auto numents = sounds.size();
	SoundList *list = (SoundList*)(group->getIntRadix("address", 16) + 0x400000);;
	list->List = new SoundFileInfo[numents];
	arrcpy(list->List, sounds.data(), numents);
	list->Count = (int)numents;
}

static vector<char *> ProcessStringArrayINI_Internal(const wchar_t *filename, uint8_t language)
{
	ifstream fstr(filename);
	vector<char *> strs;
	while (fstr.good())
	{
		string str;
		getline(fstr, str);
		str = DecodeUTF8(UnescapeNewlines(str), language);
		strs.push_back(strdup(str.c_str()));
	}
	fstr.close();
	return strs;
}

static void ProcessStringArrayINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("address")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	vector<char *> strs = ProcessStringArrayINI_Internal(filename,
		ParseLanguage(group->getString("language")));
	auto numents = strs.size();
	char **list = (char**)(group->getIntRadix("address", 16) + 0x400000);;
	arrcpy(list, strs.data(), numents);
}

static void ProcessNextLevelListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile *const inidata = new IniFile(filename);
	vector<NextLevelData> ents;
	for (unsigned int i = 0; i < 999; i++)
	{
		char key[8];
		snprintf(key, sizeof(key), "%u", i);
		if (!inidata->hasGroup(key)) break;
		const IniGroup *entdata = inidata->getGroup(key);
		NextLevelData entry;
		entry.CGMovie = (char)entdata->getInt("CGMovie");
		entry.CurrentLevel = (char)ParseLevelID(entdata->getString("Level"));
		entry.NextLevelAdventure = (char)ParseLevelID(entdata->getString("NextLevel"));
		entry.NextActAdventure = (char)entdata->getInt("NextAct");
		entry.StartPointAdventure = (char)entdata->getInt("StartPos");
		entry.AltNextLevel = (char)ParseLevelID(entdata->getString("AltNextLevel"));
		entry.AltNextAct = (char)entdata->getInt("AltNextAct");
		entry.AltStartPoint = (char)entdata->getInt("AltStartPos");
		ents.push_back(entry);
	}
	delete inidata;
	auto numents = ents.size();
	NextLevelData *list = new NextLevelData[numents + 1];
	arrcpy(list, ents.data(), numents);
	clrmem(&list[numents]);
	list[numents].CurrentLevel = -1;
	ProcessPointerList(group->getString("pointer"), list);
}

static const wchar_t *const languagenames[] = { L"Japanese", L"English", L"French", L"Spanish", L"German" };
static void ProcessCutsceneTextINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename")) return;
	char ***addr = (char ***)group->getIntRadix("address", 16);
	if (addr == nullptr) return;
	addr = (char ***)((int)addr + 0x400000);
	const wstring pathbase = mod_dir + L'\\' + group->getWString("filename") + L'\\';
	for (unsigned int i = 0; i < LengthOfArray(languagenames); i++)
	{
		wchar_t filename[MAX_PATH];
		swprintf(filename, LengthOfArray(filename), L"%s%s.txt",
			pathbase.c_str(), languagenames[i]);
		vector<char *> strs = ProcessStringArrayINI_Internal(filename, i);
		auto numents = strs.size();
		char **list = new char *[numents];
		arrcpy(list, strs.data(), numents);
		*addr++ = list;
	}
}

static void ProcessRecapScreenINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename")) return;
	int length = group->getInt("length");
	RecapScreen **addr = (RecapScreen **)group->getIntRadix("address", 16);
	if (addr == nullptr) return;
	addr = (RecapScreen **)((int)addr + 0x400000);
	const wstring pathbase = mod_dir + L'\\' + group->getWString("filename") + L'\\';
	for (unsigned int l = 0; l < LengthOfArray(languagenames); l++)
	{
		RecapScreen *list = new RecapScreen[length];
		for (int i = 0; i < length; i++)
		{
			wchar_t filename[MAX_PATH];
			swprintf(filename, LengthOfArray(filename), L"%s%d\\%s.ini",
				pathbase.c_str(), i + 1, languagenames[l]);
			const IniFile *const inidata = new IniFile(filename);
			vector<string> strs = split(inidata->getString("", "Text"), '\n');
			auto numents = strs.size();
			list[i].TextData = new const char *[numents];
			for (unsigned int j = 0; j < numents; j++)
				list[i].TextData[j] = strdup(DecodeUTF8(strs[j], l).c_str());
			list[i].LineCount = (int)numents;
			list[i].Speed = inidata->getFloat("", "Speed", 1);
			delete inidata;
		}
		*addr++ = list;
	}
}

static void ProcessNPCTextINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename")) return;
	int length = group->getInt("length");
	HintText_Entry **addr = (HintText_Entry **)group->getIntRadix("address", 16);
	if (addr == nullptr) return;
	addr = (HintText_Entry **)((int)addr + 0x400000);
	const wstring pathbase = mod_dir + L'\\' + group->getWString("filename") + L'\\';
	for (unsigned int l = 0; l < LengthOfArray(languagenames); l++)
	{
		HintText_Entry *list = new HintText_Entry[length];
		for (int i = 0; i < length; i++)
		{
			wchar_t filename[MAX_PATH];
			swprintf(filename, LengthOfArray(filename), L"%s%d\\%s.ini",
				pathbase.c_str(), i + 1, languagenames[l]);
			const IniFile *const inidata = new IniFile(filename);
			vector<int16_t> props;
			vector<HintText_Text> text;
			for (unsigned int j = 0; j < 999; j++)
			{
				char buf[8];
				snprintf(buf, sizeof(buf), "%u", j);
				if (!inidata->hasGroup(buf)) break;
				if (props.size() > 0)
					props.push_back(NPCTextControl_NewGroup);
				const IniGroup *entdata = inidata->getGroup(buf);
				if (entdata->hasKeyNonEmpty("EventFlags"))
				{
					vector<string> strs = split(entdata->getString("EventFlags"), ',');
					for (unsigned int k = 0; k < strs.size(); k++)
					{
						props.push_back(NPCTextControl_EventFlag);
						props.push_back((int16_t)strtol(strs[k].c_str(), nullptr, 10));
					}
				}
				if (entdata->hasKeyNonEmpty("NPCFlags"))
				{
					vector<string> strs = split(entdata->getString("NPCFlags"), ',');
					for (unsigned int k = 0; k < strs.size(); k++)
					{
						props.push_back(NPCTextControl_NPCFlag);
						props.push_back((int16_t)strtol(strs[k].c_str(), nullptr, 10));
					}
				}
				if (entdata->hasKeyNonEmpty("Character"))
				{
					props.push_back(NPCTextControl_Character);
					props.push_back((int16_t)ParseCharacterFlags(entdata->getString("Character")));
				}
				if (entdata->hasKeyNonEmpty("Voice"))
				{
					props.push_back(NPCTextControl_Voice);
					props.push_back((int16_t)entdata->getInt("Voice"));
				}
				if (entdata->hasKeyNonEmpty("SetEventFlag"))
				{
					props.push_back(NPCTextControl_SetEventFlag);
					props.push_back((int16_t)entdata->getInt("SetEventFlag"));
				}
				string linekey = buf;
				linekey += ".Lines[%u]";
				char *buf2 = new char[linekey.size() + 2];
				bool hasText = false;
				for (unsigned int k = 0; k < 999; k++)
				{
					snprintf(buf2, linekey.size() + 2, linekey.c_str(), k);
					if (!inidata->hasGroup(buf2)) break;
					hasText = true;
					entdata = inidata->getGroup(buf2);
					HintText_Text entry;
					entry.Message = strdup(DecodeUTF8(entdata->getString("Line"), l).c_str());
					entry.Time = entdata->getInt("Time");
					text.push_back(entry);
				}
				delete[] buf2;
				if (hasText)
				{
					HintText_Text t = {};
					text.push_back(t);
				}
			}
			delete inidata;
			if (props.size() > 0)
			{
				props.push_back(NPCTextControl_End);
				list[i].Properties = new int16_t[props.size()];
				arrcpy(list[i].Properties, props.data(), props.size());
			}
			else
				list[i].Properties = nullptr;
			if (text.size() > 0)
			{
				list[i].Text = new HintText_Text[text.size()];
				arrcpy(list[i].Text, text.data(), text.size());
			}
			else
				list[i].Text = nullptr;
		}
		*addr++ = list;
	}
}

static void ProcessLevelClearFlagListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	ifstream fstr(filename);
	vector<LevelClearFlagData> lvls;
	while (fstr.good())
	{
		string str;
		getline(fstr, str);
		if (!str.empty())
		{
			LevelClearFlagData ent;
			vector<string> parts = split(str, ' ');
			ent.Level = ParseLevelID(parts[0]);
			ent.FlagOffset = (int16_t)strtol(parts[1].c_str(), nullptr, 16);
			lvls.push_back(ent);
		}
	}
	fstr.close();
	auto numents = lvls.size();
	LevelClearFlagData *list = new LevelClearFlagData[numents + 1];
	arrcpy(list, lvls.data(), numents);
	list[numents].Level = -1;
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessDeathZoneINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t dzinipath[MAX_PATH];
	swprintf(dzinipath, LengthOfArray(dzinipath), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile *const dzdata = new IniFile(dzinipath);

	// Remove the filename portion of the path.
	// NOTE: This might be a lower directory than mod_dir,
	// since the filename can have subdirectories.
	PathRemoveFileSpec(dzinipath);

	vector<DeathZone> deathzones;
	for (unsigned int i = 0; i < 999; i++)
	{
		char key[8];
		snprintf(key, sizeof(key), "%u", i);
		if (!dzdata->hasGroup(key)) break;
		uint8_t flag = ParseCharacterFlags(dzdata->getString(key, "Flags"));

		wchar_t dzpath[MAX_PATH];
		swprintf(dzpath, LengthOfArray(dzpath), L"%s\\%u.sa1mdl", dzinipath, i);
		ModelInfo *dzmdl = new ModelInfo(dzpath);
		DeathZone dz = { flag, dzmdl->getmodel() };
		deathzones.push_back(dz);
		// NOTE: dzmdl is not deleted because NJS_OBJECT
		// points to allocated memory in the ModelInfo.
	}

	DeathZone *newlist = new DeathZone[deathzones.size() + 1];
	arrcpy(newlist, deathzones.data(), deathzones.size());
	clrmem(&newlist[deathzones.size()]);
	ProcessPointerList(group->getString("pointer"), newlist);
}

static void ProcessSkyboxScaleINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename")) return;
	int count = group->getInt("count");
	SkyboxScale **addr = (SkyboxScale **)group->getIntRadix("address", 16);
	if (addr == nullptr) return;
	addr = (SkyboxScale **)((int)addr + 0x400000);

	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile *const inidata = new IniFile(filename);
	for (int i = 0; i < count; i++)
	{
		char key[8];
		snprintf(key, sizeof(key), "%d", i);
		if (!inidata->hasGroup(key))
		{
			*addr++ = nullptr;
			continue;
		}
		const IniGroup *const entdata = inidata->getGroup(key);
		SkyboxScale *entry = new SkyboxScale;
		ParseVertex(entdata->getString("Far", "1,1,1"), entry->Far);
		ParseVertex(entdata->getString("Normal", "1,1,1"), entry->Normal);
		ParseVertex(entdata->getString("Near", "1,1,1"), entry->Near);
		*addr++ = entry;
	}
	delete inidata;
}

static void ProcessLevelPathListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t inipath[MAX_PATH];
	swprintf(inipath, LengthOfArray(inipath), L"%s\\%s\\",
		mod_dir.c_str(), group->getWString("filename").c_str());
	vector<PathDataPtr> pathlist;
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFileEx(inipath, FindExInfoStandard, &data, FindExSearchLimitToDirectories, nullptr, 0);
	if (hFind == INVALID_HANDLE_VALUE) return;
	do
	{
		if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) continue;
		uint16_t levelact;
		try
		{
			levelact = ParseLevelAndActID(UTF16toMBS(data.cFileName, CP_UTF8));
		}
		catch (...)
		{
			continue;
		}

		vector<LoopHead *> paths;
		for (unsigned int i = 0; i < 999; i++)
		{
			wchar_t levelpath[MAX_PATH];
			swprintf(levelpath, LengthOfArray(levelpath), L"%s\\%s\\%u.ini",
				inipath, data.cFileName, i);
			if (!Exists(levelpath))
				break;

			const IniFile *const inidata = new IniFile(levelpath);
			const IniGroup *entdata;
			vector<Loop> points;
			char buf2[8];
			for (unsigned int j = 0; j < 999; j++)
			{
				snprintf(buf2, LengthOfArray(buf2), "%u", j);
				if (!inidata->hasGroup(buf2)) break;
				entdata = inidata->getGroup(buf2);
				Loop point;
				point.Ang_X = (int16_t)entdata->getIntRadix("XRotation", 16);
				point.Ang_Y = (int16_t)entdata->getIntRadix("YRotation", 16);
				point.Dist = entdata->getFloat("Distance");
				ParseVertex(entdata->getString("Position", "0,0,0"), point.Position);
				points.push_back(point);
			}
			entdata = inidata->getGroup("");
			LoopHead *path = new LoopHead;
			path->Unknown_0 = (int16_t)entdata->getInt("Unknown");
			path->Count = (int16_t)points.size();
			path->TotalDist = entdata->getFloat("TotalDistance");
			path->LoopList = new Loop[path->Count];
			arrcpy(path->LoopList, points.data(), path->Count);
			path->Object = (ObjectFuncPtr)entdata->getIntRadix("Code", 16);
			paths.push_back(path);
			delete inidata;
		}
		auto numents = paths.size();
		PathDataPtr ptr;
		ptr.LevelAct = levelact;
		ptr.PathList = new LoopHead *[numents + 1];
		arrcpy(ptr.PathList, paths.data(), numents);
		ptr.PathList[numents] = nullptr;
		pathlist.push_back(ptr);
	} while (FindNextFile(hFind, &data));
	FindClose(hFind);
	PathDataPtr *newlist = new PathDataPtr[pathlist.size() + 1];
	arrcpy(newlist, pathlist.data(), pathlist.size());
	clrmem(&newlist[pathlist.size()]);
	newlist[pathlist.size()].LevelAct = -1;
	ProcessPointerList(group->getString("pointer"), newlist);
}

static void ProcessStageLightDataListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile *const inidata = new IniFile(filename);
	vector<StageLightData> ents;
	for (unsigned int i = 0; i < 999; i++)
	{
		char key[8];
		snprintf(key, sizeof(key), "%u", i);
		if (!inidata->hasGroup(key)) break;
		const IniGroup *const entdata = inidata->getGroup(key);
		StageLightData entry;
		entry.level = (char)ParseLevelID(entdata->getString("Level"));
		entry.act = (char)entdata->getInt("Act");
		entry.index = (char)entdata->getInt("LightNum");
		entry.use_direction = (char)entdata->getBool("UseDirection");
		ParseVertex(entdata->getString("Direction"), entry.direction);

		// FIXME: all these INI strings should match the light field names
		entry.specular = entdata->getFloat("Dif");
		entry.multiplier = entdata->getFloat("Multiplier");
		ParseVertex(entdata->getString("RGB"), *(NJS_VECTOR *)entry.diffuse);
		ParseVertex(entdata->getString("AmbientRGB"), *(NJS_VECTOR *)entry.ambient);

		ents.push_back(entry);
	}
	delete inidata;
	auto numents = ents.size();
	StageLightData *list = new StageLightData[numents + 1];
	arrcpy(list, ents.data(), numents);
	clrmem(&list[numents]);
	list[numents].level = -1;
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessWeldListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename") || !group->hasKeyNonEmpty("pointer")) return;
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile *const inidata = new IniFile(filename);
	vector<WeldInfo> ents;
	for (unsigned int i = 0; i < 999; i++)
	{
		char key[8];
		snprintf(key, sizeof(key), "%u", i);
		if (!inidata->hasGroup(key)) break;
		const IniGroup *const entdata = inidata->getGroup(key);
		uint16_t *vilist = nullptr;
		WeldInfo entry;
		entry.BaseModel = inimodels[trim(entdata->getString("BaseModel"))];
		entry.ModelA = inimodels[trim(entdata->getString("ModelA"))];
		entry.ModelB = inimodels[trim(entdata->getString("ModelB"))];
		if (entdata->hasKeyNonEmpty("VertIndexes"))
		{
			vector<string> strs = split(entdata->getString("VertIndexes"), ',');
			vilist = new uint16_t[strs.size()];
			for (unsigned int j = 0; j < strs.size(); j++)
				vilist[j] = (int16_t)strtol(strs[j].c_str(), nullptr, 10);
			entry.VertexPairCount = strs.size() / 2;
		}
		else
			entry.VertexPairCount = 0;
		entry.WeldType = (uint8_t)entdata->getInt("WeldType");
		entry.anonymous_5 = (int16_t)entdata->getInt("Unknown");
		entry.VertexBuffer = nullptr;
		entry.VertIndexes = vilist;
		ents.push_back(entry);
	}
	delete inidata;
	auto numents = ents.size();
	WeldInfo *list = new WeldInfo[numents + 1];
	arrcpy(list, ents.data(), numents);
	clrmem(&list[numents]);
	ProcessPointerList(group->getString("pointer"), list);
}

static void ProcessCreditsTextListINI(const IniGroup *group, const wstring &mod_dir)
{
	if (!group->hasKeyNonEmpty("filename")) return;
	CreditsList *addr = (CreditsList *)group->getIntRadix("address", 16);
	if (addr == nullptr) return;
	addr = (CreditsList *)((intptr_t)addr + 0x400000);
	wchar_t filename[MAX_PATH];
	swprintf(filename, LengthOfArray(filename), L"%s\\%s",
		mod_dir.c_str(), group->getWString("filename").c_str());
	const IniFile *const inidata = new IniFile(filename);
	vector<CreditsEntry> ents;
	for (unsigned int i = 0; i < 999; i++)
	{
		char key[8];
		snprintf(key, sizeof(key), "%u", i);
		if (!inidata->hasGroup(key)) break;
		const IniGroup *const entdata = inidata->getGroup(key);
		CreditsEntry entry;
		entry.SomeIndex = entdata->getInt("Type");
		entry.field_1 = entdata->getInt("TexID", -1);
		entry.field_2 = entdata->getInt("Unknown1");
		entry.field_3 = entdata->getInt("Unknown2");
		entry.field_3 = entdata->getInt("Unknown2");
		entry.Line = strdup(DecodeUTF8(entdata->getString("Text"), 0).c_str());
		ents.push_back(entry);
	}
	delete inidata;
	auto numents = ents.size();
	addr->Entries = new CreditsEntry[numents];
	addr->Count = numents;
	arrcpy(addr->Entries, ents.data(), numents);
}

typedef void(__cdecl *exedatafunc_t)(const IniGroup *group, const wstring &mod_dir);
static const unordered_map<string, exedatafunc_t> exedatafuncmap = {
	{ "landtable", ProcessLandTableINI },
	{ "model", ProcessModelINI },
	{ "basicdxmodel", ProcessModelINI },
	{ "chunkmodel", ProcessModelINI },
	{ "action", ProcessActionINI },
	{ "animation", ProcessAnimationINI },
	{ "objlist", ProcessObjListINI },
	{ "startpos", ProcessStartPosINI },
	{ "texlist", ProcessTexListINI },
	{ "leveltexlist", ProcessLevelTexListINI },
	{ "triallevellist", ProcessTrialLevelListINI },
	{ "bosslevellist", ProcessBossLevelListINI },
	{ "fieldstartpos", ProcessFieldStartPosINI },
	{ "soundtestlist", ProcessSoundTestListINI },
	{ "musiclist", ProcessMusicListINI },
	{ "soundlist", ProcessSoundListINI },
	{ "stringarray", ProcessStringArrayINI },
	{ "nextlevellist", ProcessNextLevelListINI },
	{ "cutscenetext", ProcessCutsceneTextINI },
	{ "recapscreen", ProcessRecapScreenINI },
	{ "npctext", ProcessNPCTextINI },
	{ "levelclearflaglist", ProcessLevelClearFlagListINI },
	{ "deathzone", ProcessDeathZoneINI },
	{ "skyboxscale", ProcessSkyboxScaleINI },
	{ "levelpathlist", ProcessLevelPathListINI },
	{ "stagelightdatalist", ProcessStageLightDataListINI },
	{ "weldlist", ProcessWeldListINI },
	//{ "bmitemattrlist", ProcessBMItemAttrListINI },
	{ "creditstextlist", ProcessCreditsTextListINI }
};

void ProcessEXEData(const wchar_t *filename, const wstring &mod_dir)
{
	const IniFile *const exedata = new IniFile(filename);
	for (auto iter = exedata->cbegin(); iter != exedata->cend(); ++iter)
	{
		IniGroup *group = iter->second;
		auto type = exedatafuncmap.find(group->getString("type"));
		if (type != exedatafuncmap.end())
			type->second(group, mod_dir);
	}
	delete exedata;
}