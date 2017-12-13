#pragma once
#include <string>

void ProcessDLLData(const wchar_t *filename, const std::wstring &mod_dir);

void SetChrmodelsDLLHandle(HMODULE handle);
