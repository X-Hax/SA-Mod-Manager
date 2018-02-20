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

		return version == PVMX_VERSION;
	}

	bool is_pvmx(std::ifstream& file)
	{
		if (!file.is_open())
		{
			return false;
		}

		const auto pos = file.tellg();
		const auto result = check_header(file);
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
		const auto pos = file.tellg();

		if (!check_header(file))
		{
			file.seekg(pos);
			return false;
		}

		uint8_t type = 0;

		for (read_t(file, type); type != dictionary_field::none; read_t(file, type))
		{
			DictionaryEntry entry = {};

			while (type != dictionary_field::none)
			{
				switch (type)
				{
					case dictionary_field::none:
						break;

					case dictionary_field::global_index:
						read_t(file, entry.global_index);
						break;

					case dictionary_field::name:
						read_cstr(file, entry.name);
						break;

					case dictionary_field::dimensions:
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

		const auto pos = static_cast<uint64_t>(file.tellg());
		file.read(reinterpret_cast<char*>(out.data()), out.size());

		const auto delta = static_cast<uint64_t>(file.tellg()) - pos;

		if (delta != entry.size)
		{
			out.clear();
			return false;
		}

		return true;
	}
}
