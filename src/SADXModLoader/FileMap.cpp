/**
 * SADX Mod Loader
 * File remapper.
 */

#include "stdafx.h"
#include "FileMap.hpp"

#include <cctype>
#include <cstring>
#include <algorithm>
using std::list;
using std::string;
using std::transform;
using std::unordered_map;

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlwapi.h>

FileMap::FileMap()
{
}

FileMap::~FileMap()
{
	clear();
}

/**
 * Replace slash characters with backslashes.
 * @param c Character.
 * @return If c == '/', '\\'; otherwise, c.
 */
static inline int backslashes(int c)
{
	if (c == '/')
		return '\\';
	else
		return c;
}

/**
 * Normalize a filename for the file replacement map.
 * @param filename Filename.
 * @return Normalized filename.
 */
string FileMap::normalizePath(const string &filename)
{
	return normalizePath(filename.c_str());
}

/**
 * Normalize a filename for the file replacement map.
 * @param filename Filename.
 * @return Normalized filename.
 */
string FileMap::normalizePath(const char *filename)
{
	string path = filename;
	transform(path.begin(), path.end(), path.begin(), backslashes);
	if (path.length() > 2 && (path[0] == '.' && path[1] == '\\'))
		path = path.substr(2, path.length() - 2);
	transform(path.begin(), path.end(), path.begin(), ::tolower);
	return path;
}

/**
 * Ignore a file.
 * @param ignoreFile File to ignore.
 */
void FileMap::addIgnoreFile(const string &ignoreFile)
{
	string path = normalizePath(ignoreFile);

	static const char nullfile[] = "nullfile";
	char *buf = new char[sizeof(nullfile)];
	strncpy(buf, nullfile, sizeof(buf));
	m_fileMap[path] = buf;
}

/**
 * Add a file replacement.
 * @param origFile Original filename.
 * @param destFile Replacement filename.
 */
void FileMap::addReplaceFile(const std::string &origFile, const std::string &destFile)
{
	string origFile_norm = normalizePath(origFile);
	string destFile_norm = normalizePath(destFile);

	// Check if the destination file is being replaced.
	auto iter = m_fileMap.find(destFile_norm);
	if (iter != m_fileMap.end())
	{
		// Destination file is being replaced.
		// Use the replacement for the original file.
		auto xiter = m_fileMap.find(origFile_norm);
		if (xiter != m_fileMap.end())
		{
			// Original file is already in the map.
			delete[] xiter->second;
		}
		else
		{
			// Create an entry in the map.
			xiter = m_fileMap.insert(std::make_pair(origFile_norm, nullptr)).first;
		}

		// Copy the new mod filename into the map.
		size_t len = strlen(iter->second) + 1;
		char *buf = new char[len];
		strncpy(buf, iter->second, len);
		xiter->second = buf;

		PrintDebug("Replaced file: \"%s\" = \"%s\"\n", origFile_norm.c_str(), buf);
	}
	else
	{
		// Destination file is not already in the map.
		auto xiter = m_fileMap.find(origFile);
		if (xiter != m_fileMap.end())
		{
			// Original file is already in the map.
			delete[] xiter->second;
		}
		else
		{
			// Create an entry in the map.
			xiter = m_fileMap.insert(std::make_pair(origFile_norm, nullptr)).first;
		}

		// Copy the new mod filename into the map.
		size_t len = destFile_norm.size() + 1;
		char *buf = new char[len];
		strncpy(buf, destFile_norm.c_str(), len);
		xiter->second = buf;

		PrintDebug("Replaced file: \"%s\" = \"%s\"\n", origFile_norm.c_str(), buf);
	}
}

/**
 * Recursively scan a directory and add all files to the replacement map.
 * Destination is always relative to system/.
 * @param srcPath Path to scan.
 */
void FileMap::scanFolder(const string &srcPath)
{
	scanFolder_int(srcPath, srcPath.length() + 1);
}

/**
 * Recursively scan a directory and add all files to the replacement map.
 * Destination is always relative to system/.
 * (Internal recursive function)
 * @param srcPath Path to scan.
 * @param srcLen Length of original srcPath. (used for recursion)
 */
void FileMap::scanFolder_int(const std::string &srcPath, int srcLen)
{
	WIN32_FIND_DATAA data;
	string srcPathSearch = srcPath + "\\*";
	HANDLE hFind = FindFirstFileA(srcPathSearch.c_str(), &data);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		// No files found.
		return;
	}

	do
	{
		// NOTE: This will hide *all* files starting with '.'.
		// SADX doesn't use any files starting with '.',
		// so this won't cause any problems.
		if (data.cFileName[0] == '.')
		{
			continue;
		}
		else if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// Recursively scan this directory.
			string newSrcPath = srcPath + "\\" + string(data.cFileName);
			scanFolder_int(newSrcPath, srcLen);
		}
		else
		{
			// Create the mod filename and original filename.
			string modFile = srcPath + "\\" + string(data.cFileName);
			transform(modFile.begin(), modFile.end(), modFile.begin(), ::tolower);
			string fileBase = modFile.substr(srcLen);
			string origFile = "system\\" + fileBase;

			// Check if this file is already in the map.
			auto iter = m_fileMap.find(origFile);
			if (iter != m_fileMap.end())
			{
				// File is already in the map.
				// Delete the existing string.
				delete[] iter->second;
			}
			else
			{
				// Create an entry in the map.
				iter = m_fileMap.insert(std::make_pair(origFile, nullptr)).first;
			}

			// Copy the new mod filename into the map.
			size_t len = modFile.length() + 1;
			char *buf = new char[len];
			strncpy(buf, modFile.c_str(), len);
			iter->second = buf;

			PrintDebug("Replaced file: \"%s\" = \"%s\"\n", origFile.c_str(), buf);
		}
	}
	while (FindNextFileA(hFind, &data) != 0);
	FindClose(hFind);
}

/**
 * Get a filename from the file replacement map.
 * @param lpFileName Filename.
 * @return Replaced filename, or original filename if not replaced by a mod.
 */
const char *FileMap::replaceFile(const char *lpFileName) const
{
	// Check if the normalized filename is in the file replacement map.
	string path = normalizePath(lpFileName);
	unordered_map<string, const char *>::const_iterator iter = m_fileMap.find(path);
	if (iter != m_fileMap.cend())
		return iter->second;

	// File was not replaced by a mod.
	// Return the filename as-is.
	return lpFileName;
}

/**
 * Find filenames that matches the specified filename, ignoring the extension.
 * This is used for BASS vgmstream.
 * @param lpFileName Filename.
 * @return List of matching filenames, or empty list if not fonud.
 */
list<const char *> FileMap::findFileNoExt(const char *lpFileName)
{
	// FIXME: Optimize this so it isn't O(n).
	// FIXME: We have to return all of the filenames because
	// we can't call BASS_VGMSTREAM_StreamCreate() here.
	list<const char *> lstFilenames;

	// Remove the file extension from the specified filename.
	char pathnoext[MAX_PATH];
	strncpy(pathnoext, lpFileName, sizeof(pathnoext));
	PathRemoveExtensionA(pathnoext);
	string path = normalizePath(pathnoext);

	// FIXME: Add a list with filenames, sans extension.
	// MSVC apparently maintains ordering; STL does not.
#ifdef _MSC_VER
	for (auto iter = m_fileMap.crbegin(); iter != m_fileMap.crend(); iter++)
#else
	for (auto iter = m_fileMap.cbegin(); iter != m_fileMap.cend(); iter++)
#endif
	{
		// Remove the extension from this filename.
		strncpy(pathnoext, iter->first.c_str(), sizeof(pathnoext));
		PathRemoveExtensionA(pathnoext);
		if (!path.compare(pathnoext))
		{
			// Found a matching filename.
			lstFilenames.push_back(iter->second);
		}
	}

	return lstFilenames;
}

/**
 * Clear the file replacement map.
 */
void FileMap::clear(void)
{
	for (auto iter = m_fileMap.begin(); iter != m_fileMap.end(); ++iter)
	{
		// Allocated using new[];
		delete[] iter->second;
	}
	m_fileMap.clear();
}
