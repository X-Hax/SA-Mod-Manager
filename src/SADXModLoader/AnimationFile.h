#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include "SADXModLoader.h"

class AnimationFile
{
public:
	AnimationFile(const char *filename);
#ifdef _MSC_VER
	AnimationFile(const wchar_t *filename);
#endif /* _MSC_VER */
	AnimationFile(const std::string &filename);
#ifdef _MSC_VER
	AnimationFile(const std::wstring &filename);
#endif /* _MSC_VER */
	AnimationFile(std::istream &stream);

	NJS_MOTION *getmotion();
	int getmodelcount();
	const std::string &getlabel();

private:
	static const uint64_t SAANIM = 0x4D494E414153ULL;
	static const uint64_t FormatMask = 0xFFFFFFFFFFFFULL;
	static const uint8_t CurrentVersion = 1;
	static const int headersize = 0x14;

	NJS_MOTION *motion;
	int modelcount;
	std::string label;
	std::vector<std::shared_ptr<void>> allocatedmem;
	std::unordered_set<void *> fixedpointers;

	void init(std::istream &stream);
};
