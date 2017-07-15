#pragma once
#include <string>

bool Exists(const std::wstring& path);
bool Exists(const std::string& path);

bool IsDirectory(const std::wstring& path);
bool IsDirectory(const std::string& path);

bool IsFile(const std::wstring& path);
bool IsFile(const std::string& path);

inline bool DirectoryExists(const std::wstring& path)
{
	return Exists(path) && IsDirectory(path);
}
inline bool DirectoryExists(const std::string& path)
{
	return Exists(path) && IsDirectory(path);
}

inline bool FileExists(const std::wstring& path)
{
	return Exists(path) && IsFile(path);
}
inline bool FileExists(const std::string& path)
{
	return Exists(path) && IsFile(path);
}

std::string GetDirectory(const std::string& path);
std::string GetBaseName(const std::string& path);
void StripExtension(std::string& path);
std::string GetExtension(const std::string& path, bool includeDot = false);

/**
 * Replace the extension of the specified filename.
 * @param filename	[in/out] Filename.
 * @param ext		[in] New extension, with leading dot.
 */
void ReplaceFileExtension(std::string &filename, const char *ext);
