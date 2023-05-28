#include "stdafx.h"
#include "IniFile.hpp"
#include "UsercallFunctionHandler.h"

using std::string;
using std::vector;

vector<PL_JOIN_VERTEX> so_jvlist;
vector<PL_JOIN_VERTEX> egg_jvlist;
vector<PL_JOIN_VERTEX> miles_jvlist;
vector<PL_JOIN_VERTEX> knux_jvlist;
vector<PL_JOIN_VERTEX> tikal_jvlist;
vector<PL_JOIN_VERTEX> amy_jvlist;
vector<PL_JOIN_VERTEX> big_jvlist;

DataPointer(NJS_ACTION, action_g_g0001_eggman, 0x0089E254);
DataPointer(NJS_ACTION, action_ti_dame, 0x008F46BC);

NJS_OBJECT* eggman_objects[17];
NJS_OBJECT* tikal_objects[23];

static UsercallFuncVoid(Knuckles_Upgrades_t, (playerwk* a1), (a1), 0x4726A0, rEAX);
static vector<uint16_t> Knux_HandIndices;
static vector<uint16_t> Knux_ShovelClawIndices;

static bool isWhiteSpace(const string s) 
{
	if (s.empty())
		return true;

	if (s.find_first_not_of(' ') != std::string::npos)
	{
		return false;
	}

	return true;
}

static string RemoveAnySpace(string str)
{
	std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
	str.erase(end_pos, str.end());
	return str;
}

void SetIndices(string indices, vector<uint16_t>& points)
{
	indices = RemoveAnySpace(indices);
	std::stringstream ss(indices);
	string out = "";
	const char de = ',';
	while (std::getline(ss, out, de))
	{
		points.push_back(stoi(out));
	}
}

PL_JOIN_VERTEX BuildJVListEntry(NJS_OBJECT* arr[], int base, int mdlA, int mdlB, int type, string indices)
{
	PL_JOIN_VERTEX jvEntry = {};

	jvEntry.objptr = (NJS_OBJECT*)arr[base];
	jvEntry.srcobj = (NJS_OBJECT*)arr[mdlA];
	jvEntry.inpmode = type;
	jvEntry.dstdepth = 0;
	jvEntry.org = 0;
	jvEntry.srcdepth = 0;

	if (mdlB == 0)
	{
		jvEntry.dstobj = 0;
		jvEntry.numVertex = 0;
		jvEntry.pnum = 0;
	}
	else
	{
		std::vector<uint16_t> points;
		SetIndices(indices, points);
		jvEntry.dstobj = (NJS_OBJECT*)arr[mdlB];
		jvEntry.numVertex = (points.size() / 2);
		uint16_t* ind_arr = new uint16_t[points.size()];
		for (int i = 0; i != points.size(); i++)
			ind_arr[i] = points.data()[i];
		jvEntry.pnum = ind_arr;
	}

	return jvEntry;
}

static int GetIntData(const string id, IniFile* ini, const string st)
{
	string s = ini->getString(id, st);
	s = RemoveAnySpace(s);
	return isWhiteSpace(s) ? 0 : stoi(s); //return number if it exists
}

void CreateJVList(NJS_OBJECT* arr[], IniFile* ini, vector<PL_JOIN_VERTEX>& jvlist, bool isKnux = false)
{
	int i = 0;
	while (ini->hasGroup(std::to_string(i)))
	{
		string s = std::to_string(i);

		if (isKnux && ini->hasKey(s, "shovel"))
		{
			string shovelIndice = ini->getString(s, "shovel", "");
			SetIndices(shovelIndice, Knux_ShovelClawIndices);
		}
		else
		{
			int base = GetIntData(s, ini, "BaseModel"); //ini->getInt(s, "BaseModel");
			int mdlA = GetIntData(s, ini, "ModelA"); // ini->getInt(s, "ModelA");
			int mdlB = GetIntData(s, ini, "ModelB"); //ini->getInt(s, "ModelB");
			int type = GetIntData(s, ini, "WeldType");// ini->getInt(s, "WeldType");
			string indices = ini->getString(s, "VertIndexes", "");

			PL_JOIN_VERTEX jvEntry = BuildJVListEntry(arr, base, mdlA, mdlB, type, indices);
			jvlist.push_back(jvEntry);

			if (mdlA == 20 && isKnux)
			{
				SetIndices(indices, Knux_HandIndices);
			}
		}

		i++;
	}

	jvlist.push_back({ 0 });

	delete ini;
}

void SetNPCWelds(PL_JOIN_VERTEX* dest, vector<PL_JOIN_VERTEX> &origin, const uint16_t size)
{
	if (!dest || origin.empty())
		return;

	uint16_t count = 0;

	for (uint16_t i = 0; i < origin.size(); i++)
	{
		if (origin.at(i).inpmode <= 3 && origin.at(i).srcobj && origin.at(i).dstobj) //npcs only support a specific type of welds with valid models
		{
			dest[count] = origin.at(i);
			count++;
		}

		if (count >= size)
		{
			dest[count + 1] = { 0 };
			break;
		}		
	}
}

void SetSonicNewWelds(IniFile* file)
{
	so_jvlist.clear();

	CreateJVList(SONIC_OBJECTS, file, so_jvlist);
	WriteData((PL_JOIN_VERTEX**)0x49AB7E, so_jvlist.data());
	WriteData((PL_JOIN_VERTEX**)0x49ABAC, so_jvlist.data());
	WriteData((PL_JOIN_VERTEX**)0x49AC3C, so_jvlist.data());
	WriteData((PL_JOIN_VERTEX**)0x49ACB6, so_jvlist.data());

	const auto sizeNPC = NPCSonicWeldInfo.size();
	SetNPCWelds((PL_JOIN_VERTEX*)&NPCSonicWeldInfo, so_jvlist, sizeNPC);
	WriteData<1>((int*)0x7D14D0, 0xC3); //remove init NPC welds so they don't overwrite the changes here
}

void SetEggmanNewWelds(IniFile* file)
{
	egg_jvlist.clear();

	auto obj = action_g_g0001_eggman.object;
	eggman_objects[0] = obj;
	eggman_objects[1] = obj->getnode(9);
	eggman_objects[2] = obj->getnode(10);
	eggman_objects[3] = obj->getnode(4);
	eggman_objects[4] = obj->getnode(5);
	eggman_objects[5] = obj->getnode(11);
	eggman_objects[6] = obj->getnode(13);
	eggman_objects[7] = obj->getnode(6);
	eggman_objects[8] = obj->getnode(8);

	eggman_objects[9] = obj->getnode(20);
	eggman_objects[10] = obj->getnode(21);
	eggman_objects[11] = obj->getnode(16);
	eggman_objects[12] = obj->getnode(17);
	eggman_objects[13] = obj->getnode(22);
	eggman_objects[14] = obj->getnode(23);
	eggman_objects[15] = obj->getnode(18);
	eggman_objects[16] = obj->getnode(19);

	CreateJVList(eggman_objects, file, egg_jvlist);
	WriteData((PL_JOIN_VERTEX**)0x7B4FBF, egg_jvlist.data());
}

void SetMilesNewWelds(IniFile* file)
{
	miles_jvlist.clear();

	CreateJVList(MILES_OBJECTS, file, miles_jvlist);
	WriteData((PL_JOIN_VERTEX**)0x461896, miles_jvlist.data());

	const auto size = NPCTailsWeldInfo.size();
	SetNPCWelds((PL_JOIN_VERTEX*)&NPCTailsWeldInfo, miles_jvlist, size);
	WriteData<1>((int*)0x7C7560, 0xC3);
}

void Knuckles_Upgrades_r(playerwk* pwp)
{
	Knuckles_Upgrades_t.Original(pwp);

	uint16_t* indice = Knux_HandIndices.data();

	if (!indice)
		return;

	switch (pwp->equipment & (Upgrades_ShovelClaw | Upgrades_FightingGloves))
	{
	case Upgrades_ShovelClaw:
	case Upgrades_ShovelClaw | Upgrades_FightingGloves:

		if (Knux_ShovelClawIndices.size() > 0)
		{
			indice = Knux_ShovelClawIndices.data();
		}
		else
		{
			indice = (uint16_t*)&Knuckles_ShovelClawIndices;
		}

		break;
	}

	knux_jvlist.at(23).pnum = indice;
	knux_jvlist.at(22).pnum = indice;
	knux_jvlist.at(11).pnum = indice;
	knux_jvlist.at(10).pnum = indice;

	NPCKnucklesWeldInfo[23].VertIndexes = indice;
	NPCKnucklesWeldInfo[22].VertIndexes = indice;
	NPCKnucklesWeldInfo[11].VertIndexes = indice;
	NPCKnucklesWeldInfo[10].VertIndexes = indice;
}

void SetKnucklesNewWelds(IniFile* file)
{
	knux_jvlist.clear();

	CreateJVList(KNUCKLES_OBJECTS, file, knux_jvlist, true);
	WriteData((PL_JOIN_VERTEX**)0x47A89E, knux_jvlist.data());

	const auto size = NPCKnucklesWeldInfo.size();
	SetNPCWelds((PL_JOIN_VERTEX*)&NPCKnucklesWeldInfo, knux_jvlist, size);
	WriteData<1>((int*)0x7C9C80, 0xc3);
	Knuckles_Upgrades_t.Hook(Knuckles_Upgrades_r);
}

void SetTikalNewWelds(IniFile* file)
{
	tikal_jvlist.clear();

	auto obj = action_ti_dame.object;

	tikal_objects[0] = obj;

	//right arm
	tikal_objects[1] = obj->getnode(86);
	tikal_objects[2] = obj->getnode(85);
	tikal_objects[3] = obj->getnode(84);
	tikal_objects[4] = obj->getnode(82);
	tikal_objects[5] = obj->getnode(81);
	//left arm
	tikal_objects[6] = obj->getnode(72);
	tikal_objects[7] = obj->getnode(71);
	tikal_objects[8] = obj->getnode(70);
	tikal_objects[9] = obj->getnode(68);
	tikal_objects[10] = obj->getnode(67);
	//right leg
	tikal_objects[11] = obj->getnode(41);
	tikal_objects[12] = obj->getnode(40);
	tikal_objects[13] = obj->getnode(39);
	tikal_objects[14] = obj->getnode(38);
	tikal_objects[15] = obj->getnode(37);
	//left leg
	tikal_objects[16] = obj->getnode(25);
	tikal_objects[17] = obj->getnode(24);
	tikal_objects[18] = obj->getnode(23);
	tikal_objects[19] = obj->getnode(22);
	tikal_objects[20] = obj->getnode(21);
	//dress, torso
	tikal_objects[21] = obj->getnode(42);
	tikal_objects[22] = obj->getnode(4);

	CreateJVList(tikal_objects, file, tikal_jvlist);
	WriteData((PL_JOIN_VERTEX**)0x7B41AB, tikal_jvlist.data());
}

void SetAmyNewWelds(IniFile* file)
{
	amy_jvlist.clear();

	CreateJVList(AMY_OBJECTS, file, amy_jvlist);
	WriteData((PL_JOIN_VERTEX**)0x48AD0B, amy_jvlist.data());

	const auto size = NPCAmyWeldInfo.size();
	SetNPCWelds((PL_JOIN_VERTEX*)&NPCAmyWeldInfo, amy_jvlist, size);
	WriteData<1>((int*)0x7CD000, 0xC3);
}

void SetBigNewWelds(IniFile* file)
{
	big_jvlist.clear();

	CreateJVList(BIG_OBJECTS, file, big_jvlist);
	WriteData((PL_JOIN_VERTEX**)0x490C14, big_jvlist.data());

	const auto size = NPCBigWeldInfo.size();
	SetNPCWelds((PL_JOIN_VERTEX*)&NPCBigWeldInfo, big_jvlist, size);
	WriteData<1>((int*)0x7D5EB0, 0xC3);
}

void SetNewWelds(const uint8_t character, IniFile* file)
{
	switch (character)
	{
	case Characters_Sonic:
		SetSonicNewWelds(file);
		break;
	case Characters_Eggman:
		SetEggmanNewWelds(file);
		break;
	case Characters_Tails:
		SetMilesNewWelds(file);
		break;
	case Characters_Knuckles:
		SetKnucklesNewWelds(file);
		break;
	case Characters_Tikal:
		SetTikalNewWelds(file);
		break;
	case Characters_Amy:
		SetAmyNewWelds(file);
		break;
	case Characters_Big:
		SetBigNewWelds(file);
		break;
	}
}