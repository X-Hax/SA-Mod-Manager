#pragma once
#include <ninja.h>
#include <string>
#include <vector>

struct CustomTextureEntry
{
	uint32_t globalIndex;
	std::string name;
};

NJS_TEXMEMLIST* LoadTexture(const std::wstring& _path, CustomTextureEntry& entry);
NJS_TEXMEMLIST* LoadTexture(const std::wstring& _path, uint32_t globalIndex, const std::string& name);
bool ParseIndex(const std::wstring & path, std::vector<CustomTextureEntry>& entries);
std::vector<CustomTextureEntry> ParseIndex(const std::wstring & path);

signed int LoadPVRFile(NJS_TEXLIST* texlist);
signed int __cdecl LoadPVM_D(const char *pvmName, NJS_TEXLIST *texList);
void __cdecl LoadPVM_C(const char* pvmName, NJS_TEXLIST* texList);

extern std::vector<std::wstring> TexturePackPaths;
