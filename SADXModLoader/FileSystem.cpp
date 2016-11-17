#include "stdafx.h"
#include <Windows.h>
#include <string>

bool DirectoryExists(const std::wstring& path)
{
	DWORD dwAttrib = GetFileAttributesW(path.c_str());

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
bool DirectoryExists(const std::string& path)
{
	return DirectoryExists(std::wstring(path.begin(), path.end()));
}

bool FileExists(const std::wstring& path)
{
	DWORD dwAttrib = GetFileAttributesW(path.c_str());

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
bool FileExists(const std::string& path)
{
	return FileExists(std::wstring(path.begin(), path.end()));
}

