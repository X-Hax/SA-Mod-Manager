/**
 * SADX Mod Loader
 * File remapper.
 */

#include "stdafx.h"
#include "FileMap.hpp"

#include <cctype>
#include <cstring>
#include <algorithm>
using std::forward_list;
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
	m_fileMap[path] = "nullfile";
}

/**
 * Add a file replacement.
 * @param origFile Original filename.
 * @param modFile Mod filename.
 */
void FileMap::addReplaceFile(const std::string &origFile, const std::string &modFile)
{
	string origFile_norm = normalizePath(origFile);
	string modFile_norm = normalizePath(modFile);

	// Check if the destination file is being replaced.
	auto iter = m_fileMap.find(modFile_norm);
	if (iter != m_fileMap.end())
	{
		// Destination file is being replaced.
		// Use the replacement for the original file.
		setReplaceFile(origFile_norm, iter->second);
	}
	else
	{
		// Destination file is not already in the map.
		setReplaceFile(origFile_norm, modFile_norm);
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
			setReplaceFile(origFile, modFile);
		}
	}
	while (FindNextFileA(hFind, &data) != 0);
	FindClose(hFind);
}

/**
 * Set a replacement file in the map.
 * Filenames must already be normalized!
 * (Internal function; handles memory allocation)
 * @param origFile Original file.
 * @param destFile Replacement filename.
 */
void FileMap::setReplaceFile(const std::string &origFile, const std::string &destFile)
{
	// Update the main map.
	m_fileMap[origFile] = destFile;

	// Remove the file extension from the original filename.
	char pathnoext[MAX_PATH];
	strncpy(pathnoext, origFile.c_str(), sizeof(pathnoext));
	PathRemoveExtensionA(pathnoext);
	string s_pathnoext(pathnoext);

	// Check if m_fileNoExtMap already has a list.
	auto iter_noext = m_fileNoExtMap.find(s_pathnoext);
	if (iter_noext == m_fileNoExtMap.end())
	{
		// No list yet.
		iter_noext = m_fileNoExtMap.insert(std::make_pair(s_pathnoext, new forward_list<string>())).first;
	}
	forward_list<string> *fileList = iter_noext->second;

	// Add the filename to the list.
	fileList->push_front(destFile);

	PrintDebug("Replaced file: \"%s\" = \"%s\"\n", origFile.c_str(), destFile.c_str());
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
	unordered_map<string, string>::const_iterator iter = m_fileMap.find(path);
	if (iter != m_fileMap.cend())
	{
		const string &newFileName = iter->second;
		return newFileName.c_str();
	}

	// File was not replaced by a mod.
	// Return the filename as-is.
	return lpFileName;
}

/**
 * Find filenames that matches the specified filename, ignoring the extension.
 * This is used for BASS vgmstream.
 * @param lpFileName Filename.
 * @return List of matching filenames, or nullptr if not fonud.
 */
const forward_list<string>* FileMap::findFileNoExt(const char *lpFileName) const
{
	// Remove the file extension from the specified filename.
	char pathnoext[MAX_PATH];
	strncpy(pathnoext, lpFileName, sizeof(pathnoext));
	PathRemoveExtensionA(pathnoext);
	string s_pathnoext = normalizePath(pathnoext);

	auto iter = m_fileNoExtMap.find(s_pathnoext);
	return (iter != m_fileNoExtMap.cend()
		? iter->second
		: nullptr);
}

/**
 * Clear the file replacement map.
 */
void FileMap::clear(void)
{
	m_fileNoExtMap.clear();
	m_fileMap.clear();
}
