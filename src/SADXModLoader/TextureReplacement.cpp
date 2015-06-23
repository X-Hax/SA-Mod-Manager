// Windows
#include <d3dx8tex.h>
#include <Windows.h>

// Standard
#include <string>
#include <fstream>
#include <vector>

// Global
#include <SADXModLoader.h>

// This
#include "TextureReplacement.h"

DataPointer(IDirect3DDevice8*, Direct3D_Device, 0x03D128B0);
DataPointer(bool, LoadingFile, 0x3ABDF68);

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

struct CustomTextureEntry
{
	uint32_t globalIndex;
	std::string name;
};

using namespace std;

// List of texture pack directories with trailing slash
// (e.g mods\MyCoolMod\textures\)
std::vector<std::wstring> TexturePackPaths;

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

	ifstream indexFile(pvmName + L"\\index.txt");
	vector<CustomTextureEntry> customEntries;
	string line;
	uint32_t lineNumber = 0;

	// This reads the custom texture list from disk.
	// The format is: gbix,filename
	// e.g: 1000,MyCoolTexture.dds

	while (!indexFile.eof())
	{
		++lineNumber;
		getline(indexFile, line);
		
		if (line.length() == 0)
			continue;

		size_t comma = line.find(',');

		if (comma < 1 && comma != line.npos)
		{
			PrintDebug("\aInvalid texture index entry on line %d (missing comma?)\n", lineNumber);
			indexFile.close();
			return false;
		}

		customEntries.push_back({ stoi(line.substr(0, comma)), line.substr(comma + 1) });
	}

	indexFile.close();

#if 0
	PrintDebug("NJS_TEXLIST Textures:		0x%08X\n", texList->textures);
	PrintDebug("NJS_TEXLIST Count:			%d\n", texList->nbTexture);
	PrintDebug("NJS_TEXLIST New Count:		%d\n\n", customEntries.size());
#endif

	texList->nbTexture = customEntries.size();

	for (uint32_t i = 0; i < texList->nbTexture; i++)
	{
#if 0
		PrintDebug("NJS_TEXNAME Filename:		0x%08X\n",		texList->textures[i].filename);
		PrintDebug("NJS_TEXNAME Attributes:		%d\n",			texList->textures[i].attr);
		PrintDebug("NJS_TEXNAME texaddr:		0x%08X\n\n",	texList->textures[i].texaddr);

		// This is the CURRENTLY loaded texture list, if any.
		// We're displaying all the details in order to determine what needs to be assigned
		// by the custom texture loader.
		NJS_TEXMEMLIST* _texture = (NJS_TEXMEMLIST*)texList->textures[i].texaddr;

		PrintDebug("NJS_TEXMEMLIST GBIX:		%d\n",			_texture->globalIndex);
		PrintDebug("NJS_TEXMEMLIST Bank:		%d\n",			_texture->bank);
		PrintDebug("NJS_TEXMEMLIST TSPP:		0x%08X\n",		_texture->tspparambuffer);
		PrintDebug("NJS_TEXMEMLIST TPARAM:		0x%08X\n",		_texture->texparambuffer);
		PrintDebug("NJS_TEXMEMLIST texaddr:		0x%08X\n",		_texture->texaddr);
		PrintDebug("NJS_TEXMEMLIST TexInfo:		0x%08X\n",		_texture->texinfo);
		PrintDebug("NJS_TEXMEMLIST Count:		%d\n\n",		_texture->count);

		PrintDebug("NJS_TEXINFO texaddr:		0x%08X\n",		_texture->texinfo.texaddr);
		PrintDebug("NJS_TEXINFO Surface:		0x%08X\n\n",	_texture->texinfo.texsurface);

		// The surface is the texture itself. It contains a pointer (pSurface) to an IDirect3DTexture8
		PrintDebug("NJS_TEXSURFACE Type:		0x%08X\n",		_texture->texinfo.texsurface.Type);
		PrintDebug("NJS_TEXSURFACE Depth:		%d\n",			_texture->texinfo.texsurface.BitDepth);
		PrintDebug("NJS_TEXSURFACE Format:		%d\n",			_texture->texinfo.texsurface.PixelFormat);
		PrintDebug("NJS_TEXSURFACE Width:		%d\n",			_texture->texinfo.texsurface.nWidth);
		PrintDebug("NJS_TEXSURFACE Height:		%d\n",			_texture->texinfo.texsurface.nHeight);
		PrintDebug("NJS_TEXSURFACE Size:		%d\n",			_texture->texinfo.texsurface.TextureSize);
		PrintDebug("NJS_TEXSURFACE Flags:		0x%08X\n",		_texture->texinfo.texsurface.fSurfaceFlags);
		PrintDebug("NJS_TEXSURFACE texaddr:		0x%08X\n",		_texture->texinfo.texsurface.pSurface);
		PrintDebug("NJS_TEXSURFACE Virtual:		0x%08X\n",		_texture->texinfo.texsurface.pVirtual);
		PrintDebug("NJS_TEXSURFACE Physical:	0x%08X\n\n\n",	_texture->texinfo.texsurface.pPhysical);
#endif

		wstring path = pvmName + L"\\" + wstring(customEntries[i].name.begin(), customEntries[i].name.end());

		// This loads the DDS/PNG/etc texture from disk.
		IDirect3DTexture8* d3dtexture;
		D3DXCreateTextureFromFile(Direct3D_Device, path.c_str(), &d3dtexture);
		
		D3DSURFACE_DESC info;
		d3dtexture->GetLevelDesc(0, &info);

		// Now we assign some basic metadata from the texture entry and D3D texture, as well as the pointer to the texture itself.
		// A few things I know are missing for sure are:
		// NJS_TEXSURFACE::Type, Depth, Format, Flags. Virtual and Physical are pretty much always null.
		NJS_TEXMEMLIST* texture = new NJS_TEXMEMLIST;
		*texture = {};

		texture->count							= 1;	// Required for the game to release the texture.
		texture->globalIndex					= customEntries[i].globalIndex;
		texture->texinfo.texsurface.nWidth		= info.Width;
		texture->texinfo.texsurface.nHeight		= info.Height;
		texture->texinfo.texsurface.TextureSize	= info.Size;
		texture->texinfo.texsurface.pSurface	= (Uint32*)d3dtexture;

		texList->textures[i].texaddr = (Uint32)texture;

		// Realistically those two integers should be void*, but I'm just working with what I've got :V
	}

	return true;
}

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
	if (FileExists("SYSTEM\\" + filename + ".PVM") || FileExists("SYSTEM\\" + filename))
	{
		LoadPVM_D_original(pvmName, texList);
		return;
	}

	PrintDebug("Unable to locate PVM: %s\n", pvmName);
	return;
}

// This is for individual texture replacement and is currently unused.
// TODO: Re-enable this but add proper metadata population.
#if 0
FunctionPointer(void, sub_792A70, (LPVOID), 0x792A70);
FunctionPointer(signed int, LoadPVM_HeaderStuff_SetsRAMAddr, (void*, NJS_TEXLIST*), 0x0077FD80);
FunctionPointer(void*, LoadTextureFromFile, (char*, char*), 0x7929D0);
signed int __cdecl LoadPVM_D(const char* pvmName, NJS_TEXLIST* texList)
{
	signed int result; // eax@2

	if (pvmName != nullptr && texList != nullptr)
	{
		string name(pvmName);
		name += ".pvm";

		void* v3 = LoadTextureFromFile((char*)name.c_str(), (char*)name.c_str());
		result = LoadPVM_HeaderStuff_SetsRAMAddr(v3, texList);
		sub_792A70(v3);

		string path(pvmName);
		path = "Textures\\" + path;

		if (DirectoryExists(path.c_str()))
		{
			PrintDebug("Loading PVM: [%08X] %s\n", *(int*)texList, pvmName);

			for (uint32_t i = 0; i < texList->nbTexture; i++)
			{
				NJS_TEXMEMLIST* texture = (NJS_TEXMEMLIST*)texList->textures[i].texaddr;

				string file(path);
				file.append("\\" + to_string(texture->globalIndex) + ".dds");

				if (FileExists(file))
				{
					PrintDebug("\tLoading custom texture: %s\n", file.c_str());
					IDirect3DTexture8* d3dtexture = (IDirect3DTexture8*)texture->texinfo.texsurface.pSurface;
					d3dtexture->Release();
					D3DXCreateTextureFromFileA(Direct3D_Device, file.c_str(), &d3dtexture);
					texture->texinfo.texsurface.pSurface = (Uint32*)d3dtexture;
				}
			}
		}
	}
	else
	{
		result = -1;
	}
	return result;
}
#endif