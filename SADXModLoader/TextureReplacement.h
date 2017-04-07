#pragma once
#include <string>
#include <vector>

struct TexPackEntry
{
	uint32_t globalIndex;
	std::string name;
	uint32_t width, height;
};

namespace texpack
{
	/// <summary>
	/// Parses custom texture index.
	/// </summary>
	/// <param name="path">A valid path to a texture pack directory containing index.txt</param>
	/// <param name="out">A vector to populate.</param>
	/// <returns><c>true</c> on success.</returns>
	bool ParseIndex(const std:: string& path, std::vector<TexPackEntry>& out);
	/// <summary>
	/// Initializes function hooks for texture replacement.
	/// </summary>
	void Init();
}
