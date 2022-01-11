#include "stdafx.h"
#include "ArchiveX.h"
#include <iostream>
#include <sstream>

ArchiveX::ArchiveX(const char* filename)
{
	stream.open(filename, std::ios::binary);
	open();
}

ArchiveX::ArchiveX(const std::string& filename)
{
	stream.open(filename, std::ios::binary);
	open();
}

#ifdef _MSC_VER
ArchiveX::ArchiveX(const wchar_t* filename)
{
	stream.open(filename, std::ios::binary);
	open();
}

ArchiveX::ArchiveX(const std::wstring& filename)
{
	stream.open(filename, std::ios::binary);
	open();
}
#endif /* _MSC_VER */

ArchiveX::~ArchiveX()
{
	stream.close();
}

ArchiveX::ArcFile* ArchiveX::FindFile(const std::string& path)
{
	for (auto& i : files)
	{
		if (i.folder.size() > 0)
		{
			if (!path.compare(i.folder + "\\" + i.name))
			{
				return &i;
			}
		}
		else
		{
			if (!path.compare(i.name))
			{
				return &i;
			}
		}
	}

	return nullptr;
}

void ArchiveX::GetFile(const std::string& path, std::stringstream& str)
{
	ArcFile* file = FindFile(path);

	if (file)
	{
		stream.seekg(file->offset);
		auto size = file->size;
		char* buf = new char[size];
		stream.read(buf, size);
		str.write(buf, size);
	}
}

void ArchiveX::GetFile(const char* path, std::stringstream& str)
{
	return GetFile(static_cast<std::string>(path), str);
}

ModelInfo* ArchiveX::GetModel(const std::string& path)
{
	ArcFile* file = FindFile(path);

	if (file)
	{
		stream.seekg(file->offset);
		return new ModelInfo(stream);
	}
	else
	{
		return nullptr;
	}
}

ModelInfo* ArchiveX::GetModel(const char* path)
{
	return GetModel(static_cast<std::string>(path));
}

AnimationFile* ArchiveX::GetAnimation(const std::string& path)
{
	ArcFile* file = FindFile(path);

	if (file)
	{
		stream.seekg(file->offset);
		return new AnimationFile(stream);
	}
	else
	{
		return nullptr;
	}
}

AnimationFile* ArchiveX::GetAnimation(const char* path)
{
	return GetAnimation(static_cast<std::string>(path));
}

LandTableInfo* ArchiveX::GetLandTable(const std::string& path)
{
	ArcFile* file = FindFile(path);

	if (file)
	{
		stream.seekg(file->offset);
		return new LandTableInfo(stream);
	}
	else
	{
		return nullptr;
	}
}

LandTableInfo* ArchiveX::GetLandTable(const char* path)
{
	return GetLandTable(static_cast<std::string>(path));
}

template <typename T>
auto& read_t(std::ifstream& file, T& data)
{
	return file.read(reinterpret_cast<char*>(&data), sizeof(T));
}

void read_string(std::ifstream& file, std::string& out)
{
	std::stringstream buffer;

	while (true)
	{
		char c;
		read_t(file, c);

		if (c == '\0')
		{
			break;
		}

		buffer.put(c);
	}

	out = buffer.str();
}

void ArchiveX::open()
{
	uint32_t magic;
	read_t(stream, magic);

	if (magic != ARCX)
	{
		return;
	}

	uint8_t version;
	read_t(stream, version);

	if (version > CurrentVersion)
	{
		return;
	}

	ArcField field = ArcField::None;

	for (read_t(stream, field); field != ArcField::None; read_t(stream, field))
	{
		ArcFile file = {};

		while (field != ArcField::None)
		{
			switch (field)
			{
			case ArcField::Folder:
				read_string(stream, file.folder);
				break;
			case ArcField::Name:
				read_string(stream, file.name);
				break;
			default:
				break;
			}

			read_t(stream, field);
		}

		read_t(stream, file.offset);
		read_t(stream, file.size);

		files.push_back(file);
	}
}