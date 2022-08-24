#pragma once

#include <memory>
#include <string>
#include <cstdint>
#include <fstream>
#include "SADXModLoader.h"
#include "ModelInfo.h"
#include "AnimationFile.h"
#include "LandTableInfo.h"

class ArchiveX
{
public:
	ArchiveX(const char* filename);
	ArchiveX(const wchar_t* filename);
	ArchiveX(const std::string& filename);
	ArchiveX(const std::wstring& filename);
	~ArchiveX();

	void GetFile(const std::string& path, std::stringstream& str);
	void GetFile(const char* path, std::stringstream& str);

	ModelInfo* GetModel(const std::string& path);
	ModelInfo* GetModel(const char* path);

	AnimationFile* GetAnimation(const std::string& path);
	AnimationFile* GetAnimation(const char* path);

	LandTableInfo* GetLandTable(const std::string& path);
	LandTableInfo* GetLandTable(const char* path);

private:
	struct ArcFile
	{
		std::string folder, name;
		uint64_t offset;
		uint64_t size;
	};

	enum class ArcField : uint8_t
	{
		None,
		Folder,
		Name
	};

	static constexpr uint32_t ARCX = 0x58435241u;
	static constexpr uint8_t CurrentVersion = 0;

	std::ifstream stream;
	std::vector<ArcFile> files;

	void open();
	ArcFile* FindFile(const std::string& path);
};