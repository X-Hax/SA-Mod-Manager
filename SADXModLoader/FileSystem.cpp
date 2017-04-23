#include "stdafx.h"
#include <Windows.h>
#include <string>
#include <shlwapi.h>

bool Exists(const std::wstring& path)
{
	return PathFileExistsW(path.c_str()) != 0;
}
bool Exists(const std::string& path)
{
	return PathFileExistsA(path.c_str()) != 0;
}

bool IsDirectory(const std::wstring& path)
{
	return PathIsDirectoryW(path.c_str()) != 0;
}
bool IsDirectory(const std::string& path)
{
	return PathIsDirectoryA(path.c_str()) != 0;
}

bool IsFile(const std::wstring& path)
{
	return !IsDirectory(path);
}
bool IsFile(const std::string& path)
{
	return !IsDirectory(path);
}

std::string GetDirectory(const std::string& path)
{
	std::string result;
	auto npos = path.npos;

	auto slash = path.find_last_of('\\');
	if (slash == npos)
	{
		slash = path.find_last_of('/');

		if (slash == npos)
		{
			result = "";
			return result;
		}
	}

	if (slash != path.size() - 1)
	{
		result = path.substr(0, slash);
		return result;
	}

	auto last = slash;

	slash = path.find_last_of('\\', last);
	if (slash == npos)
	{
		slash = path.find_last_of('/', last);
	}

	if (slash == npos)
	{
		result = "";
		return result;
	}

	result = path.substr(last);
	return result;
}

std::string GetBaseName(const std::string& path)
{
	std::string result;
	auto npos = path.npos;

	auto slash = path.find_last_of('\\');
	if (slash == npos)
	{
		slash = path.find_last_of('/');
		if (slash == npos)
		{
			result = path;
			return result;
		}
	}

	if (slash != path.size() - 1)
	{
		result = path.substr(++slash);
		return result;
	}

	auto last = slash - 1;

	slash = path.find_last_of('\\', last);
	if (slash == npos)
	{
		slash = path.find_last_of('/', last);
	}

	result = (!slash || slash == npos) ? "" : path.substr(slash + 1, last - slash);
	return result;
}

void StripExtension(std::string& path)
{
	auto dot = path.find('.');
	if (dot == path.npos)
	{
		return;
	}

	path = path.substr(0, dot);
}

std::string GetExtension(const std::string& path, bool includeDot)
{
	auto dot = path.find('.');
	if (dot == path.npos)
	{
		return "";
	}

	if (!includeDot)
	{
		++dot;
	}

	return path.substr(dot);
}
