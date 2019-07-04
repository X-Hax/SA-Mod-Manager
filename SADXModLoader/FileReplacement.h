#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "FileMap.hpp"

extern FileMap sadx_fileMap;

HANDLE __stdcall MyCreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
const char* _ReplaceFile(const char* lpFileName);
