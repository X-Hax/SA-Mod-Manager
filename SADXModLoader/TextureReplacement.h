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

NJS_TEXMEMLIST* LoadTexture(const std::wstring& _path, CustomTextureEntry& entry, bool mipmap);
NJS_TEXMEMLIST* LoadTexture(const std::wstring& _path, uint32_t globalIndex, const std::string& name, bool mipmap);
bool ParseIndex(const std::wstring& path, std::vector<CustomTextureEntry>& entries);
std::vector<CustomTextureEntry> ParseIndex(const std::wstring& path);

Sint32 njLoadTexture_Wrapper_r(NJS_TEXLIST* texlist);
Sint32 njLoadTexture_Hook(NJS_TEXLIST* texlist);
void __cdecl LoadPVM_C_r(const char* filename, NJS_TEXLIST* texlist);

extern std::vector<std::wstring> TexturePackPaths;
