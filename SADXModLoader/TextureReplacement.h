#pragma once
#include <ninja.h>
#include <string>
#include <vector>

struct CustomTextureEntry
{
	uint32_t globalIndex;
	std::string name;
};

Sint32 njLoadTexture_Wrapper_r(NJS_TEXLIST* texlist);
Sint32 njLoadTexture_Hook(NJS_TEXLIST* texlist);
void __cdecl LoadPVM_C_r(const char* filename, NJS_TEXLIST* texlist);

namespace texpack
{
	/// <summary>
	/// List of texture pack directories with trailing slash (e.g mods\MyCoolMod\textures\)
	/// </summary>
	extern std::vector<std::wstring> Paths;
	/// <summary>
	/// Initializes function hooks for texture replacement.
	/// </summary>
	void Init();
}
