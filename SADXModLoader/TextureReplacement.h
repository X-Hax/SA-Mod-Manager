#pragma once
#include <ninja.h>
#include <string>
#include <vector>

signed int __cdecl LoadPVM_D(const char *pvmName, NJS_TEXLIST *texList);
void __cdecl LoadPVM_C(const char* pvmName, NJS_TEXLIST* texList);

extern std::vector<std::wstring> TexturePackPaths;
