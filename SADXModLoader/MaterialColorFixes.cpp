#include "stdafx.h"
#include "MaterialColorFixes.h"

enum DLLExportType
{
	ExportMODEL = 0,
	ExportOBJECT = 1,
	ExportACTION = 2,
	ExportLANDTABLE = 3
};

static void FixMaterialColors(NJS_MODEL_SADX* msadx)
{
	if (!msadx || msadx->nbMat == 0)
		return;
	for (int ms = 0; ms < msadx->nbMeshset; ms++)
	{
		if (msadx->meshsets[ms].vertcolor != NULL)
			continue;
		msadx->mats[msadx->meshsets[ms].type_matId & ~0xC000].diffuse.color |= 0xFFFFFF;
	}
}

static void FixMaterialColors(NJS_OBJECT* obj)
{
	if (!obj)
		return;
	if (obj->basicdxmodel)
		FixMaterialColors(obj->basicdxmodel);
	if (obj->child)
		FixMaterialColors(obj->child);
	if (obj->sibling)
		FixMaterialColors(obj->sibling);
}

static void FixMaterialColorsDll(LPCWSTR dllname, const char* exportname, int arraysize, DLLExportType DLLExportType)
{
	void** exp = (void**)GetProcAddress(GetModuleHandle(dllname), exportname);
	for (int i = 0; i < arraysize; i++)
	{
		if (!exp[i])
			continue;
		switch (DLLExportType)
		{
		case ExportMODEL:
			FixMaterialColors(((NJS_MODEL_SADX**)exp)[i]);
			break;
		case ExportOBJECT:
			FixMaterialColors(((NJS_OBJECT**)exp)[i]);
			break;
		case ExportACTION:
			FixMaterialColors(((NJS_ACTION**)exp)[i]->object);
			break;
		case ExportLANDTABLE:
			for (int c = 0; c < ((LandTable**)exp)[i]->COLCount; c++)
				FixMaterialColors(((LandTable**)exp)[i]->Col[c].Model);
			break;
		default:
			break;
		}
	}
}

void MaterialColorFixes_Init()
{
	for (unsigned int m = 0; m < LengthOfArray(EXEModelPointers); m++)
		FixMaterialColors((NJS_MODEL_SADX*)EXEModelPointers[m]);
	FixMaterialColorsDll(L"ADV00MODELS", "___MODEL_SS_PEOPLE_OBJECTS", 26, ExportOBJECT);
	FixMaterialColorsDll(L"ADV00MODELS", "___LANDTABLESS", 6, ExportLANDTABLE);
	FixMaterialColorsDll(L"ADV01MODELS", "___LANDTABLEEC", 6, ExportLANDTABLE);
	FixMaterialColorsDll(L"ADV01MODELS", "___ADV01_ACTIONS", 25, ExportACTION);
	FixMaterialColorsDll(L"ADV01MODELS", "___ADV01_OBJECTS", 75, ExportOBJECT);
	FixMaterialColorsDll(L"ADV01MODELS", "___ADV01_MODELS", 15, ExportMODEL);
	FixMaterialColorsDll(L"ADV01CMODELS", "___LANDTABLEEC", 6, ExportLANDTABLE);
	FixMaterialColorsDll(L"ADV01CMODELS", "___ADV01C_ACTIONS", 23, ExportACTION);
	FixMaterialColorsDll(L"ADV01CMODELS", "___ADV01C_OBJECTS", 50, ExportOBJECT);
	FixMaterialColorsDll(L"ADV01CMODELS", "___ADV01C_MODELS", 34, ExportMODEL);
	FixMaterialColorsDll(L"ADV02MODELS", "___LANDTABLEMR", 4, ExportLANDTABLE);
	FixMaterialColorsDll(L"ADV02MODELS", "___ADV02_ACTIONS", 35, ExportACTION);
	FixMaterialColorsDll(L"ADV02MODELS", "___ADV02_OBJECTS", 128, ExportOBJECT);
	FixMaterialColorsDll(L"ADV02MODELS", "___ADV02_MODELS", 23, ExportMODEL);
	FixMaterialColorsDll(L"ADV03MODELS", "___LANDTABLEPAST", 3, ExportLANDTABLE);
	FixMaterialColorsDll(L"ADV03MODELS", "___ADV03_ACTIONS", 13, ExportACTION);
	FixMaterialColorsDll(L"ADV03MODELS", "___ADV03_OBJECTS", 40, ExportOBJECT);
	FixMaterialColorsDll(L"BOSSCHAOS0MODELS", "___LANDTABLEBOSSCHAOS0", 1, ExportLANDTABLE);
	FixMaterialColorsDll(L"BOSSCHAOS0MODELS", "___BOSSCHAOS0_ACTIONS", 19, ExportACTION);
	FixMaterialColorsDll(L"BOSSCHAOS0MODELS", "___BOSSCHAOS0LAND_OBJECTS", 73, ExportOBJECT);
	FixMaterialColorsDll(L"BOSSCHAOS0MODELS", "___BOSSCHAOS0_OBJECTS", 40, ExportOBJECT);
	FixMaterialColorsDll(L"CHRMODELS_orig", "___SONIC_OBJECTS", 79, ExportOBJECT);
	FixMaterialColorsDll(L"CHRMODELS_orig", "___SONIC_ACTIONS", 149, ExportACTION);
	FixMaterialColorsDll(L"CHRMODELS_orig", "___SONIC_MODELS", 11, ExportMODEL);
	FixMaterialColorsDll(L"CHRMODELS_orig", "___MILES_OBJECTS", 72, ExportOBJECT);
	FixMaterialColorsDll(L"CHRMODELS_orig", "___MILES_ACTIONS", 114, ExportACTION);
	FixMaterialColorsDll(L"CHRMODELS_orig", "___MILES_MODELS", 15, ExportMODEL);
	FixMaterialColorsDll(L"CHRMODELS_orig", "___KNUCKLES_OBJECTS", 74, ExportOBJECT);
	FixMaterialColorsDll(L"CHRMODELS_orig", "___KNUCKLES_ACTIONS", 90, ExportACTION);
	FixMaterialColorsDll(L"CHRMODELS_orig", "___KNUCKLES_MODELS", 24, ExportMODEL);
	FixMaterialColorsDll(L"CHRMODELS_orig", "___AMY_OBJECTS", 39, ExportOBJECT);
	FixMaterialColorsDll(L"CHRMODELS_orig", "___AMY_ACTIONS", 80, ExportACTION);
	FixMaterialColorsDll(L"CHRMODELS_orig", "___AMY_MODELS", 5, ExportMODEL);
	FixMaterialColorsDll(L"CHRMODELS_orig", "___E102_OBJECTS", 28, ExportOBJECT);
	FixMaterialColorsDll(L"CHRMODELS_orig", "___E102_ACTIONS", 78, ExportACTION);
	FixMaterialColorsDll(L"CHRMODELS_orig", "___BIG_OBJECTS", 44, ExportOBJECT);
	FixMaterialColorsDll(L"CHRMODELS_orig", "___BIG_ACTIONS", 90, ExportACTION);
	FixMaterialColorsDll(L"CHRMODELS_orig", "___BIG_MODELS", 4, ExportMODEL);
}