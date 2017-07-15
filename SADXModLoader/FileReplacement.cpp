#include "stdafx.h"
#include "FileReplacement.h"

// File replacement map.
// NOTE: Do NOT mark this as static.
// MediaFns.cpp needs to access the FileMap.
FileMap sadx_fileMap;

/**
* CreateFileA() wrapper using _ReplaceFile().
* @param lpFileName
* @param dwDesiredAccess
* @param dwShareMode
* @param lpSecurityAttributes
* @param dwCreationDisposition
* @param dwFlagsAndAttributes
* @param hTemplateFile
* @return
*/
HANDLE WINAPI MyCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	return CreateFileA(sadx_fileMap.replaceFile(lpFileName), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

/**
* C wrapper to call sadx_fileMap.replaceFile() from asm.
* @param lpFileName Filename.
* @return Replaced filename, or original filename if not replaced by a mod.
*/
const char *_ReplaceFile(const char *lpFileName)
{
	return sadx_fileMap.replaceFile(lpFileName);
}
