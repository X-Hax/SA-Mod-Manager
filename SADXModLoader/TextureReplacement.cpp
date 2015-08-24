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
FunctionPointer(void, UnloadTextures, (NJS_TEXNAME** texName_pp), 0x00403290);

#define TOMAPSTRING(a) { a, #a }

static const std::unordered_map<HRESULT, const char*> D3DErrors = {
	TOMAPSTRING(D3DXERR_CANNOTMODIFYINDEXBUFFER),
	TOMAPSTRING(D3DXERR_INVALIDMESH),
	TOMAPSTRING(D3DXERR_CANNOTATTRSORT),
	TOMAPSTRING(D3DXERR_SKINNINGNOTSUPPORTED),
	TOMAPSTRING(D3DXERR_TOOMANYINFLUENCES),
	TOMAPSTRING(D3DXERR_INVALIDDATA),
	TOMAPSTRING(D3DXERR_LOADEDMESHASNODATA),
	TOMAPSTRING(D3DERR_NOTAVAILABLE),
	TOMAPSTRING(D3DERR_OUTOFVIDEOMEMORY),
	TOMAPSTRING(D3DERR_INVALIDCALL),
	TOMAPSTRING(E_OUTOFMEMORY)
};

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
std::unordered_map<std::wstring, vector<CustomTextureEntry>> entryCache;
static bool wasLoading = false;

#pragma region Index Parsing

/// <summary>
/// Parses custom texture index.
/// </summary>
/// <param name="path">The path containing index.txt</param>
/// <returns>A vector containing the entries. Can be empty, particularly on failure.</returns>
std::vector<CustomTextureEntry> ParseIndex(const std::wstring& path)
{
	vector<CustomTextureEntry> result;
	ParseIndex(path, result);
	return result;
}

/// <summary>
/// Parses custom texture index.
/// </summary>
/// <param name="path">The path containing index.txt</param>
/// <param name="out">A vector to populate.</param>
/// <returns><c>true</c> on success.</returns>
bool ParseIndex(const std::wstring& path, std::vector<CustomTextureEntry>& out)
{
	wstring index = path + L"index.txt";
	string path_a = string(path.begin(), path.end());

	if (!FileExists(index))
	{
		//PrintDebug("Texture index missing for: %s\n", path_a.c_str());
		return false;
	}

	PrintDebug("Loading texture pack: %s\n", path_a.c_str());

	auto it = entryCache.find(path);
	if (it != entryCache.end())
	{
		out = it->second;
		return true;
	}

	if (!wasLoading)
		PrintDebug("\tBuilding cache...\n");

	wasLoading = LoadingFile;

	ifstream indexFile(index);

	if (!indexFile.is_open())
	{
		PrintDebug("Unable to open index file: %s\n", string(index.begin(), index.end()).c_str());
		return false;
	}

	bool result = true;
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

			if (line.length() == 0 || line[0] == '#')
				continue;

			size_t comma = line.find(',');

			if (comma < 1 && comma != line.npos)
			{
				PrintDebug("Invalid texture index entry on line %u (missing comma?)\n", lineNumber);
				result = false;
				break;
			}

			uint32_t gbix = stoul(line.substr(0, comma));
			string name = line.substr(comma + 1);
			string texturePath = path_a + name;

			if (!FileExists(texturePath))
			{
				PrintDebug("Texture entry on line %u does not exist: %s\n", lineNumber, texturePath.c_str());
				result = false;
				break;
			}

			out.push_back({ gbix, name });
		}
		catch (std::exception& exception)
		{
			PrintDebug("An exception occurred while parsing texture index on line %u: %s\n", lineNumber, exception.what());
			out.clear();
			return false;
		}
	}

	if (!result)
		out.clear();

	entryCache[path] = out;

	return result;
}

void CheckCache()
{
	if (!LoadingFile)
	{
		if (wasLoading)
		{
			PrintDebug("\tClearing cache...\n");
			entryCache.clear();
		}

		wasLoading = false;
	}
}

#pragma endregion

#pragma region Texture Loading

/// <summary>
/// Loads a texture from disk in the specified path.
/// </summary>
/// <param name="_path">The folder containing the texture.</param>
/// <param name="globalIndex">The global index for this texture.</param>
/// <param name="name">The filename of this texture..</param>
/// <returns><c>true</c> on success.</returns>
NJS_TEXMEMLIST* LoadTexture(const std::wstring& _path, uint32_t globalIndex, const std::string& name)
{
	wstring path = _path + L"\\" + wstring(name.begin(), name.end());
	NJS_TEXMEMLIST* texture = GetGlobalTexture(globalIndex);

	// GetGlobalTexture will only return null if over 2048 unique textures have been loaded.
	// TODO: Add custom texture overflow vector with an access function similar to GetGlobalTexture.
	if (texture == nullptr)
	{
		PrintDebug("Failed to allocate global texture for gbix %u (likely exceeded max global texture limit)\n", globalIndex);
	}
	else
	{
		// A texture count of 0 indicates that this is an empty texture slot.
		if (texture->count != 0)
		{
			PrintDebug("Using cached texture for GBIX %u\n", globalIndex);
		}
		else
		{
			IDirect3DTexture8* d3dtexture;
			// This loads the DDS/PNG/etc texture from disk.
			HRESULT result = D3DXCreateTextureFromFile(Direct3D_Device, path.c_str(), &d3dtexture);

			if (result != D3D_OK)
			{
				PrintDebug("Failed to load texture %s: %s (%u)\n", name.c_str(), D3DErrors.at(result), result);
				return nullptr;
			}

			D3DSURFACE_DESC info;
			d3dtexture->GetLevelDesc(0, &info);

			// Now we assign some basic metadata from the texture entry and D3D texture, as well as the pointer to the texture itself.
			// A few things I know are missing for sure are:
			// NJS_TEXSURFACE::Type, Depth, Format, Flags. Virtual and Physical are pretty much always null.
			texture->count							= 1;	// Required for the game to release the texture.
			texture->globalIndex					= globalIndex;
			texture->texinfo.texsurface.nWidth		= info.Width;
			texture->texinfo.texsurface.nHeight		= info.Height;
			texture->texinfo.texsurface.TextureSize	= info.Size;
			texture->texinfo.texsurface.pSurface	= (Uint32*)d3dtexture;
		}
	}

	return texture;
}

/// <summary>
/// Loads a texture from disk in the specified path.
/// </summary>
/// <param name="_path">The folder containing the texture.</param>
/// <param name="entry">The entry containing the global index and filename.</param>
/// <returns><c>true</c> on success.</returns>
NJS_TEXMEMLIST* LoadTexture(const std::wstring& _path, CustomTextureEntry& entry)
{
	return LoadTexture(_path, entry.globalIndex, entry.name);
}

#pragma endregion

#pragma region PVM

/// <summary>
/// Replaces the specified PVM with a texture pack virtual PVM.
/// </summary>
/// <param name="pvmName">Name of the PVM without extension.</param>
/// <param name="texList">The tex list.</param>
/// <returns><c>true</c> on success.</returns>
bool ReplacePVM(const std::wstring& pvmName, NJS_TEXLIST* texList)
{
	if (texList == nullptr)
		return false;

	string pvmName_a(pvmName.begin(), pvmName.end());

	vector<CustomTextureEntry> customEntries;

	if (!ParseIndex(pvmName + L'\\', customEntries))
		return false;

	texList->nbTexture = customEntries.size();
	bool result = true;

	for (uint32_t i = 0; i < texList->nbTexture; i++)
	{
		NJS_TEXMEMLIST* texture = LoadTexture(pvmName, customEntries[i]);;

		// I would just break here, but unloading the textures after that causes issues.
		// TODO: Investigate that further. This is wasteful.
		if (texture == nullptr && result)
			result = false;

		texList->textures[i].texaddr = (Uint32)texture;
	}

	if (!result)
		UnloadTextures((NJS_TEXNAME**)&texList->textures);

	return result;
}

FunctionPointer(signed int, LoadPVM_D_original, (const char*, NJS_TEXLIST*), 0x0077FEB0);

void __cdecl LoadPVM_C(const char* pvmName, NJS_TEXLIST* texList)
{
	string filename(pvmName);
	CheckCache();
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
		if (ReplacePVM(path, texList))
			return;
	}

	// Default behavior.
	// Loads real PVM archives if no texture packs were found or successfully loaded.
	// TODO: Clean up.
	if (FileExists(string(sadx_fileMap.replaceFile(("SYSTEM\\" + filename + ".PVM").c_str()))) || FileExists(string(sadx_fileMap.replaceFile(("SYSTEM\\" + filename).c_str()))))
	{
		LoadPVM_D_original(pvmName, texList);
		return;
	}

	PrintDebug("Unable to locate PVM: %s\n", pvmName);
	return;
}

#pragma endregion

#pragma region PVR

/// <summary>
/// Loads texture pack replacement for specified PVR.
/// </summary>
/// <param name="filename">The PVR filename without extension.</param>
/// <param name="tex">The output TEXMEMLIST</param>
/// <returns><c>true</c> on success.</returns>
bool ReplacePVR(const std::string& filename, NJS_TEXMEMLIST** tex)
{
	string a = filename;
	transform(a.begin(), a.end(), a.begin(), ::tolower);

	for (size_t i = TexturePackPaths.size(); i-- > 0;)
	{
		vector<CustomTextureEntry> entries;

		if (!ParseIndex(TexturePackPaths[i], entries))
			continue;

		for (CustomTextureEntry& e : entries)
		{
			replace(e.name.begin(), e.name.end(), '/', '\\');
			size_t npos = e.name.npos;

			size_t slash = e.name.find_last_of('\\');
			size_t dot = e.name.find_last_of('.');

			slash = (slash == npos) ? 0 : ++slash;

			if (dot != npos)
			{
				string b = e.name.substr(slash, dot - slash);
				transform(b.begin(), b.end(), b.begin(), ::tolower);

				if (a == b)
					return (*tex = LoadTexture(TexturePackPaths[i], e.globalIndex, e.name)) != nullptr;
			}
		}
	}

	return false;
}

ThiscallFunctionPointer(int, sub_78CF80, (void*), 0x78CF80);
FunctionPointer(NJS_TEXMEMLIST*, TexMemList_PixelFormat, (NJS_TEXINFO* info, Uint32 gbix), 0x0077F7F0);
FunctionPointer(NJS_TEXMEMLIST*, LoadPVR, (void* data, int gbix), 0x0077FBD0);
FunctionPointer(void*, LoadTextureFromFile, (const char*), 0x007929D0);
FunctionPointer(void, j__HeapFree_0, (LPVOID lpMem), 0x00792A70);
DataPointer(char, unk_3CFC000, 0x3CFC000);

signed int __cdecl LoadPVR_wrapper(NJS_TEXLIST* texlist)
{
	CheckCache();
	LoadingFile = true;
	return LoadPVRFile(texlist);
}

signed int __cdecl LoadPVRFile(NJS_TEXLIST* texlist)
{
	NJS_TEXMEMLIST* memlist; // edi@7

	if (texlist == nullptr)
		return -1;

	for (Uint32 i = 0; i != texlist->nbTexture; i++)
	{
		NJS_TEXNAME* entries = &texlist->textures[i];
		Uint32 gbix = 0xFFFFFFEF;

		if (entries->attr & NJD_TEXATTR_GLOBALINDEX)
			gbix = entries->texaddr;

		sub_78CF80(*((void**)&unk_3CFC000 + i));
		Uint32 attr = entries->attr;

		// If already loaded, grab from memory. Otherwise, load from disk.
		if (attr & NJD_TEXATTR_TYPE_MEMORY)
		{
			if (attr & NJD_TEXATTR_GLOBALINDEX)
				memlist = TexMemList_PixelFormat((NJS_TEXINFO*)entries->filename, gbix);
			else
				memlist = LoadPVR(*(void**)entries->filename, gbix);
		}
		else
		{
			string filename((const char*)entries->filename);

			if (ReplacePVR(filename, (NJS_TEXMEMLIST**)&entries->texaddr))
				continue;

			filename += ".pvr";
			void* data = LoadTextureFromFile(filename.c_str());
			memlist = LoadPVR(data, gbix);
			j__HeapFree_0(data);
		}

		entries->texaddr = (Uint32)memlist;
	}

	return 1;
}

#pragma endregion
