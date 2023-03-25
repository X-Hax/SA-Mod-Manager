#include "stdafx.h"
#include "IniFile.hpp"
#include "UsercallFunctionHandler.h"

std::vector<PL_JOIN_VERTEX> so_jvlist;
std::vector<PL_JOIN_VERTEX> egg_jvlist;
std::vector<PL_JOIN_VERTEX> miles_jvlist;
std::vector<PL_JOIN_VERTEX> knux_jvlist;
std::vector<PL_JOIN_VERTEX> tikal_jvlist;
std::vector<PL_JOIN_VERTEX> amy_jvlist;
std::vector<PL_JOIN_VERTEX> big_jvlist;

DataPointer(NJS_ACTION, action_g_g0001_eggman, 0x0089E254);
DataPointer(NJS_ACTION, action_ti_dame, 0x008F46BC);

NJS_OBJECT* eggman_objects[17];
NJS_OBJECT* tikal_objects[23];

static UsercallFuncVoid(Knuckles_Upgrades_t, (playerwk* a1), (a1), 0x4726A0, rEAX);
static std::vector<uint16_t> Knux_HandIndices;
static std::vector<uint16_t> Knux_ShovelClawIndices;

void SetIndices(std::string indices, std::vector<uint16_t>& points)
{
	std::stringstream ss(indices);
	std::string out;
	const char de = ', ';
	while (std::getline(ss, out, de))
	{
		points.push_back(stoi(out));
	}
}

PL_JOIN_VERTEX BuildJVListEntry(NJS_OBJECT* arr[], int base, int mdlA, int mdlB, int type, std::string indices)
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

void CreateJVList(NJS_OBJECT* arr[], IniFile* ini, std::vector<PL_JOIN_VERTEX>& jvlist, bool isKnux = false)
{
	int i = 0;
	while (ini->hasGroup(std::to_string(i)))
	{
		std::string s = std::to_string(i);

		if (isKnux && ini->hasKey(s, "shovel"))
		{
			std::string shovelIndice = ini->getString(s, "shovel", "");
			SetIndices(shovelIndice, Knux_ShovelClawIndices);
		}
		else
		{
			int base = ini->getInt(s, "BaseModel");
			int mdlA = ini->getInt(s, "ModelA");
			int mdlB = ini->getInt(s, "ModelB");
			int type = ini->getInt(s, "WeldType");
			std::string indices = ini->getString(s, "VertIndexes", "");

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

void SetSonicNewWelds(IniFile* file)
{
	so_jvlist.clear();

	CreateJVList(SONIC_OBJECTS, file, so_jvlist);
	WriteData((PL_JOIN_VERTEX**)0x49AB7E, so_jvlist.data());
	WriteData((PL_JOIN_VERTEX**)0x49ABAC, so_jvlist.data());
	WriteData((PL_JOIN_VERTEX**)0x49AC3C, so_jvlist.data());
	WriteData((PL_JOIN_VERTEX**)0x49ACB6, so_jvlist.data());

	const auto sizeNPC = NPCSonicWeldInfo.size();
	memcpy(NPCSonicWeldInfo, so_jvlist.data(), sizeof(PL_JOIN_VERTEX) * sizeNPC);
	NPCSonicWeldInfo[sizeNPC - 1] = { 0 };
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
	memcpy(NPCTailsWeldInfo, miles_jvlist.data(), sizeof(PL_JOIN_VERTEX) * size);
	NPCTailsWeldInfo[size - 1] = { 0 };
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
	memcpy(NPCKnucklesWeldInfo, knux_jvlist.data(), sizeof(PL_JOIN_VERTEX) * size);
	NPCKnucklesWeldInfo[size - 1] = { 0 };
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
	memcpy(NPCAmyWeldInfo, amy_jvlist.data(), sizeof(PL_JOIN_VERTEX) * size);
	NPCAmyWeldInfo[size - 1] = { 0 };
	WriteData<1>((int*)0x7CD000, 0xC3);
}

void SetBigNewWelds(IniFile* file)
{
	big_jvlist.clear();

	CreateJVList(BIG_OBJECTS, file, big_jvlist);
	WriteData((PL_JOIN_VERTEX**)0x490C14, big_jvlist.data());

	const auto size = NPCBigWeldInfo.size();
	memcpy(NPCBigWeldInfo, big_jvlist.data(), sizeof(PL_JOIN_VERTEX) * size);
	NPCBigWeldInfo[size - 1] = { 0 };
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