#pragma once
#include <string>
#include <vector>

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
