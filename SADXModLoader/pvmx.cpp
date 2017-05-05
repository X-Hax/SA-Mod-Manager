#include "stdafx.h"

#include <cstdint>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "pvmx.h"

#pragma region convenience

template <typename T>
auto& write_t(std::ofstream& file, const T& data)
{
	return file.write(reinterpret_cast<const char*>(&data), sizeof(T));
}

template <typename T>
auto& read_t(std::ifstream& file, T& data)
{
	return file.read(reinterpret_cast<char*>(&data), sizeof(T));
}

void read_cstr(std::ifstream& file, std::string& out)
{
	std::stringstream buffer;

	while (true)
	{
		auto c = '\0';
		read_t(file, c);

		if (c == '\0')
		{
			break;
		}

		buffer.put(c);
	}

	out = buffer.str();
}

#pragma endregion 

namespace pvmx
{
	bool check_header(std::ifstream& file)
	{
		if (!file.is_open())
		{
			return false;
		}

		int fourcc;
		read_t(file, fourcc);

		if (fourcc != PVMX_FOURCC)
		{
			return false;
		}

		uint8_t version;
		read_t(file, version);

		if (version != PVMX_VERSION)
		{
			return false;
		}

		return true;
	}

	bool is_pvmx(std::ifstream& file)
	{
		if (!file.is_open())
		{
			return false;
		}

		auto pos = file.tellg();
		auto result = check_header(file);
		file.seekg(pos);

		return result;
	}

	bool is_pvmx(const char* path)
	{
		std::ifstream file(path, std::ios::binary | std::ios::in);
		return check_header(file);
	}

	bool is_pvmx(const std::string& path)
	{
		return is_pvmx(path.c_str());
	}

	bool read_index(std::ifstream& file, std::vector<DictionaryEntry>& out)
	{
		auto pos = file.tellg();
		if (!check_header(file))
		{
			file.seekg(pos);
			return false;
		}

		uint8_t type = 0;

		for (read_t(file, type); type != DictionaryField::None; read_t(file, type))
		{
			DictionaryEntry entry = {};

			while (type != DictionaryField::None)
			{
				switch (type)
				{
					case DictionaryField::None:
						break;

					case DictionaryField::GlobalIndex:
						read_t(file, entry.globalIndex);
						break;

					case DictionaryField::Name:
						read_cstr(file, entry.name);
						break;

					case DictionaryField::Dimensions:
						read_t(file, entry.width);
						read_t(file, entry.height);
						break;

					// Unknown field type.
					default:
						return false;
				}

				read_t(file, type);
			}

			read_t(file, entry.offset);
			read_t(file, entry.size);

			out.push_back(entry);
		}

		return true;
	}

	bool get_entry(std::ifstream& file, const DictionaryEntry& entry, std::vector<uint8_t>& out)
	{
		if (!file.is_open())
		{
			return false;
		}

		out.resize(static_cast<size_t>(entry.size));

		auto pos = static_cast<uint64_t>(file.tellg());
		file.read(reinterpret_cast<char*>(out.data()), out.size());
		auto delta = static_cast<uint64_t>(file.tellg()) - pos;


		if (delta != entry.size)
		{
			out.clear();
			return false;
		}

		return true;
	}
}
