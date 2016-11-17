#pragma once
#include <string>

bool DirectoryExists(const std::wstring& path);
bool DirectoryExists(const std::string& path);
bool FileExists(const std::wstring& path);
bool FileExists(const std::string& path);
