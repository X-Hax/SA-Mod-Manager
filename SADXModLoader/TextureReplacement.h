#pragma once
#include <ninja.h>
#include <string>
#include <vector>

struct CustomTextureEntry
{
	uint32_t globalIndex;
	std::string name;
};

void InitTextureReplacement();

NJS_TEXMEMLIST* LoadTexture(const std::wstring& _path, CustomTextureEntry& entry);
NJS_TEXMEMLIST* LoadTexture(const std::wstring& _path, uint32_t globalIndex, const std::string& name);
bool ParseIndex(const std::wstring& path, std::vector<CustomTextureEntry>& entries);
std::vector<CustomTextureEntry> ParseIndex(const std::wstring& path);

signed int LoadPVR_wrapper(NJS_TEXLIST* texlist);
signed int LoadPVRFile(NJS_TEXLIST* texlist);
void __cdecl LoadPVM_C(const char* filename, NJS_TEXLIST* texlist);

extern std::vector<std::wstring> TexturePackPaths;
extern bool forceMipmaps;
