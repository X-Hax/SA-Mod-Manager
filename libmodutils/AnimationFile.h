#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include "SADXModLoader.h"

class AnimationFile
{
public:
	AnimationFile(const char* filename);
	AnimationFile(const wchar_t* filename);
	AnimationFile(const std::string& filename);
	AnimationFile(const std::wstring& filename);
	AnimationFile(std::istream& stream);

	NJS_MOTION* getmotion() const;
	int getmodelcount() const;
	bool isshortrot() const;
	const std::string& getlabel();
	const std::string& getlabel(void* data);
	void* getdata(const std::string& label);
	const std::unordered_map<std::string, void*>* getlabels() const;

private:
	static const uint64_t SAANIM = 0x4D494E414153u;
	static const uint64_t FormatMask = 0xFFFFFFFFFFFFu;
	static const uint8_t CurrentVersion = 2;
	static const int headersize = 0x14;

	NJS_MOTION* motion;
	int modelcount;
	bool shortrot;
	std::unordered_map<void*, std::string> labels1;
	std::unordered_map<std::string, void*> labels2;
	std::vector<std::shared_ptr<void>> allocatedmem;
	std::unordered_set<void*> fixedpointers;

	enum ChunkTypes : uint32_t
	{
		ChunkTypes_Label = 0x4C42414C,
		ChunkTypes_End = 0x444E45
	};

	void init(std::istream& stream);
};