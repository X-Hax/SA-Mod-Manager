#include "stdafx.h"

// Windows
#include <d3dx8tex.h>
#include <Windows.h>

// Standard
#include <string>
#include <algorithm>	// for std::transform
#include <fstream>
#include <vector>

// Global
#include <SADXModLoader.h>

// FileMap from dllmain.cpp.
#include "FileMap.hpp"
extern FileMap sadx_fileMap;

// This
#include "TextureReplacement.h"

DataPointer(IDirect3DDevice8*, Direct3D_Device, 0x03D128B0);
DataPointer(bool, LoadingFile, 0x3ABDF68);
FunctionPointer(NJS_TEXMEMLIST*, GetGlobalTexture, (int gbix), 0x0077F5B0);

#pragma region Filesystem stuff

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
#pragma endregion

using namespace std;

// List of texture pack directories with trailing slash
// (e.g mods\MyCoolMod\textures\)
std::vector<std::wstring> TexturePackPaths;

#pragma region Parse

std::vector<CustomTextureEntry> ParseIndex(const std::wstring& path)
{
	vector<CustomTextureEntry> result;
	ParseIndex(path, result);
	return result;
}

bool ParseIndex(const std::wstring& path, std::vector<CustomTextureEntry>& entries)
{
	ifstream indexFile(path);

	if (!indexFile.is_open())
	{
		PrintDebug("Unable to open index file: %s\n", std::string(path.begin(), path.end()).c_str());
		return false;
	}

	string line;
	uint32_t lineNumber = 0;

	// This reads the custom texture list from disk.
	// The format is: gbix,filename
	// e.g: 1000,MyCoolTexture.dds
	// TODO: Add GBIX bypass so texture packs can optionally ignore GBIX on load.

	while (!indexFile.eof())
	{
		try
		{
			++lineNumber;
			getline(indexFile, line);

			if (line.length() == 0)
				continue;

			size_t comma = line.find(',');

			if (comma < 1 && comma != line.npos)
			{
				PrintDebug("Invalid texture index entry on line %u (missing comma?)\n", lineNumber);
				return false;
			}

			entries.push_back({ stoul(line.substr(0, comma)), line.substr(comma + 1) });
		}
		catch (std::exception& exception)
		{
			PrintDebug("An exception occurred while parsing texture index on line %u: %s", lineNumber, exception.what());
			return false;
		}
	}

	return true;
}

#pragma endregion

#pragma region Load

NJS_TEXMEMLIST* LoadTexture(const std::wstring& _path, uint32_t globalIndex, const std::string& name)
{
	wstring path = _path + L"\\" + wstring(name.begin(), name.end());

	if (!FileExists(path))
	{
		PrintDebug("Texture does not exist: %s\n", path.c_str());
		return nullptr;
	}

	NJS_TEXMEMLIST* texture = GetGlobalTexture(globalIndex);

	// A texture count of 0 indicates that this is an empty texture slot.
	if (texture == nullptr || texture->count == 0)
	{
		IDirect3DTexture8* d3dtexture;
		// This loads the DDS/PNG/etc texture from disk.
		HRESULT result = D3DXCreateTextureFromFile(Direct3D_Device, path.c_str(), &d3dtexture);

		if (result != D3D_OK)
		{
			PrintDebug("Texture load failed with error code %u\n", result);
			return nullptr;
		}

		D3DSURFACE_DESC info;
		d3dtexture->GetLevelDesc(0, &info);

		// GetGlobalTexture will only return null if over 2048 unique textures have been loaded.
		// TODO: Add custom texture overflow vector with an access function similar to GetGlobalTexture.
		// As it is now, I believe this is a memory leak.
		if (texture == nullptr)
		{
			texture = new NJS_TEXMEMLIST;
			*texture = {};
		}

		// Now we assign some basic metadata from the texture entry and D3D texture, as well as the pointer to the texture itself.
		// A few things I know are missing for sure are:
		// NJS_TEXSURFACE::Type, Depth, Format, Flags. Virtual and Physical are pretty much always null.
		texture->count = 1;	// Required for the game to release the texture.
		texture->globalIndex = globalIndex;
		texture->texinfo.texsurface.nWidth = info.Width;
		texture->texinfo.texsurface.nHeight = info.Height;
		texture->texinfo.texsurface.TextureSize = info.Size;
		texture->texinfo.texsurface.pSurface = (Uint32*)d3dtexture;
	}
	else
	{
		PrintDebug("Using cached texture for GBIX %u\n", globalIndex);
	}

	return texture;
}

NJS_TEXMEMLIST* LoadTexture(const std::wstring& _path, CustomTextureEntry& entry)
{
	return LoadTexture(_path, entry.globalIndex, entry.name);
}

bool BuildTextureList(const std::wstring& pvmName, NJS_TEXLIST* texList)
{
	if (texList == nullptr)
		return false;

	string pvmName_a(pvmName.begin(), pvmName.end());

	PrintDebug("Loading texture pack: %s\n", pvmName_a.c_str());

	if (!FileExists(pvmName + L"\\index.txt"))
	{
		PrintDebug("\aTexture index missing for %s\n", pvmName_a.c_str());
		return false;
	}

	vector<CustomTextureEntry> customEntries;

	if (!ParseIndex(pvmName + L"\\index.txt", customEntries))
		return false;

	texList->nbTexture = customEntries.size();

	for (uint32_t i = 0; i < texList->nbTexture; i++)
		texList->textures[i].texaddr = (Uint32)LoadTexture(pvmName, customEntries[i].globalIndex, customEntries[i].name);

	return true;
}

#pragma endregion

FunctionPointer(signed int, LoadPVM_D_original, (const char*, NJS_TEXLIST*), 0x0077FEB0);

void __cdecl LoadPVM_C(const char* pvmName, NJS_TEXLIST* texList)
{
	string filename(pvmName);
	LoadingFile = true;

	// Custom PVM Directory.
	// This iterates backwards through the list of texture pack paths to find
	// the last entry containing the PVM name the game is trying to load.
	for (size_t i = TexturePackPaths.size(); i-- > 0;)
	{
		wstring path = TexturePackPaths[i] + wstring(filename.begin(), filename.end());

		if (!DirectoryExists(path))
			continue;

		// On success, the function is exited. Otherwise, the search continues.
		if (BuildTextureList(path, texList))
			return;
	}
	
	// Default behavior.
	// Loads real PVM archives if no texture packs were found or successfully loaded.
	if (FileExists(string(sadx_fileMap.replaceFile(("SYSTEM\\" + filename + ".PVM").c_str()))) || FileExists(string(sadx_fileMap.replaceFile(("SYSTEM\\" + filename).c_str()))))
	{
		LoadPVM_D_original(pvmName, texList);
		return;
	}

	PrintDebug("Unable to locate PVM: %s\n", pvmName);
	return;
}

bool ReplacePVR(const std::string& fileName, NJS_TEXMEMLIST** tex)
{
	string a = fileName;
	transform(a.begin(), a.end(), a.begin(), ::tolower);

	for (size_t i = TexturePackPaths.size(); i-- > 0;)
	{
		wstring path = TexturePackPaths[i] + L"index.txt";

		if (!FileExists(path))
			continue;

		auto entries = ParseIndex(path);

		if (entries.size() == 0)
			continue;

		for (auto& e : entries)
		{
			size_t dot = e.name.find_last_of('.');

			if (dot != e.name.npos)
			{
				string b = e.name.substr(0, dot);
				transform(b.begin(), b.end(), b.begin(), ::tolower);

				if (a == b)
				{
					*tex = LoadTexture(TexturePackPaths[i], e.globalIndex, e.name);
					return *tex != nullptr;
				}
			}
		}
	}

	return false;
}

ThiscallFunctionPointer(int, sub_78CF80, (void*), 0x78CF80);
FunctionPointer(NJS_TEXMEMLIST*, TexMemList_PixelFormat, (NJS_TEXINFO* info, Uint32 gbix), 0x0077F7F0);
FunctionPointer(NJS_TEXMEMLIST* , LoadPVR, (void* data, int gbix), 0x0077FBD0);
FunctionPointer(void*, LoadTextureFromFile, (const char*), 0x007929D0);
FunctionPointer(void, j__HeapFree_0, (LPVOID lpMem), 0x00792A70);
DataPointer(char, unk_3CFC000, 0x3CFC000);

signed int __cdecl LoadPVRFile(NJS_TEXLIST* texlist)
{
	NJS_TEXMEMLIST* memlist; // edi@7
	char* v8 = nullptr; // ecx@9

	if (texlist == nullptr)
		return -1;

	Uint32 i = 0;
	Uint32 count = texlist->nbTexture;

	for (Uint32 i = 0; i != count; i++)
	{
		NJS_TEXNAME* entries = &texlist->textures[i];
		Uint32 gbix = 0xFFFFFFEF;

		if (entries->attr & NJD_TEXATTR_GLOBALINDEX)
			gbix = entries->texaddr;

		sub_78CF80(*((void** )&unk_3CFC000 + i));
		Uint32 attr = entries->attr;

		// If already loaded, grab from memory. Otherwise, load from disk.
		if (attr & NJD_TEXATTR_TYPE_MEMORY)
		{
			if (attr & NJD_TEXATTR_GLOBALINDEX)
				memlist = TexMemList_PixelFormat((NJS_TEXINFO* )entries->filename, gbix);
			else
				memlist = LoadPVR(*(void** )entries->filename, gbix);
		}
		else
		{
			string fileName((const char*)entries->filename);

			if (ReplacePVR(fileName, (NJS_TEXMEMLIST**)&entries->texaddr))
				continue;

			fileName += ".pvr";
			void* data = LoadTextureFromFile(fileName.c_str());
			memlist = LoadPVR(data, gbix);
			j__HeapFree_0(data);
		}

		entries->texaddr = (Uint32)memlist;
	}

	return 1;
}