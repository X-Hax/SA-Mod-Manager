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
