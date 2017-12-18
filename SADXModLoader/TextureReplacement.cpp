#include "stdafx.h"

// Windows
#include <Windows.h>

// Standard
#include <string>
#include <algorithm> // for std::transform
#include <fstream>
#include <vector>

// Global
#include <SADXModLoader.h>

// Local
#include "FileReplacement.h"
#include "FileSystem.h"
#include "D3DCommon.h"
#include "DDS.h"
#include "AutoMipmap.h"

// This
#include "TextureReplacement.h"

#define TOMAPSTRING(a) { a, #a }

using namespace std;

static const unordered_map<HRESULT, const char*> d3d_errors = {
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

static unordered_map<string, vector<TexPackEntry>> raw_cache;
static unordered_map<string, vector<pvmx::DictionaryEntry>> archive_cache;
static bool was_loading = false;

DataArray(NJS_TEXPALETTE*, unk_3CFC000, 0x3CFC000, 0);

static Sint32 njLoadTexture_Wrapper_r(NJS_TEXLIST* texlist);
static Sint32 njLoadTexture_r(NJS_TEXLIST* texlist);
static int __cdecl LoadSystemPVM_r(const char* filename, NJS_TEXLIST* texlist);
static void __cdecl LoadPVM_r(const char* filename, NJS_TEXLIST* texlist);

void texpack::Init()
{
	WriteJump((void*)LoadSystemPVM, LoadSystemPVM_r);
	WriteJump((void*)njLoadTexture, njLoadTexture_r);
	WriteJump((void*)njLoadTexture_Wrapper, njLoadTexture_Wrapper_r);
	WriteJump((void*)LoadPVM, LoadPVM_r);
}

inline void check_loading()
{
	bool loading = LoadingFile != 0;

#ifdef _DEBUG
	if (!was_loading && was_loading != loading)
	{
		PrintDebug("\tBuilding cache...\n");

	}
#endif

	was_loading = loading;
}

#pragma region Index Parsing

/// <summary>
/// Parses custom texture index.
/// </summary>
/// <param name="path">A valid path to a texture pack directory containing index.txt</param>
/// <param name="out">A vector to populate.</param>
/// <returns><c>true</c> on success.</returns>
bool texpack::ParseIndex(const string& path, vector<TexPackEntry>& out)
{
	PrintDebug("Loading texture pack: %s\n", path.c_str());

	auto it = raw_cache.find(path);
	if (it != raw_cache.end())
	{
		out = it->second;
		return true;
	}

	auto index = path + "\\index.txt";

	if (!FileExists(index))
	{
		return false;
	}

	check_loading();

	ifstream indexFile(index);

	if (!indexFile.is_open())
	{
		PrintDebug("Unable to open index file: %s\n", index.c_str());
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
			{
				continue;
			}

			size_t comma = line.find(',');

			if (comma < 1 && comma != line.npos)
			{
				PrintDebug("Invalid texture index entry on line %u (missing comma?)\n", lineNumber);
				result = false;
				break;
			}

			uint32_t width = 0;
			uint32_t height = 0;

			uint32_t gbix = stoul(line.substr(0, comma));
			auto name = line.substr(comma + 1);

			comma = name.find(',');

			// check for an additional texture dimensions field
			if (comma != name.npos && comma > 0)
			{
				auto tmp = name;
				name = tmp.substr(0, comma);

				auto dimensions = tmp.substr(++comma);
				size_t separator = dimensions.find('x');

				// If no 'x' separator is found, try capital
				if (!separator || separator == dimensions.npos)
				{
					separator = dimensions.find('X');
				}

				if (!separator || separator == dimensions.npos)
				{
					PrintDebug("Invalid format for texture dimensions on line %u: %s\n",
						lineNumber, dimensions.c_str());
					break;
				}

				width = stoul(dimensions.substr(0, separator));
				height = stoul(dimensions.substr(++separator));
			}

			auto texturePath = path + "\\" + name;

			if (!FileExists(texturePath))
			{
				PrintDebug("Texture entry on line %u does not exist: %s\n", lineNumber, texturePath.c_str());
				result = false;
				break;
			}

			out.push_back({ gbix, name, width, height });
		}
		catch (exception& exception)
		{
			PrintDebug("An exception occurred while parsing texture index on line %u: %s\n", lineNumber, exception.what());
			out.clear();
			return false;
		}
	}

	if (!result)
	{
		out.clear();
	}

	raw_cache[path] = out;
	return result;
}

bool GetArchiveIndex(const string& path, ifstream& file, vector<pvmx::DictionaryEntry>& out)
{
	PrintDebug("Loading texture pack: %s\n", path.c_str());

	auto it = archive_cache.find(path);
	if (it != archive_cache.end())
	{
		out = it->second;
		return true;
	}

	if (!FileExists(path))
	{
		return false;
	}

	check_loading();

	if (!file.is_open())
	{
		PrintDebug("Unable to open archive file: %s\n", path.c_str());
		return false;
	}

	if (pvmx::read_index(file, out))
	{
		archive_cache[path] = out;
		return true;
	}

	return false;
}

void CheckCache()
{
	if (LoadingFile)
	{
		return;
	}

	if (was_loading)
	{
#ifdef _DEBUG
		PrintDebug("\tClearing cache...\n");
#endif
		raw_cache.clear();
		archive_cache.clear();
	}

	was_loading = false;
}

#pragma endregion

#pragma region Texture Loading

bool GetDDSHeader(ifstream& file, DDS_HEADER& header)
{
	if (!file.is_open())
	{
		return false;
	}

	auto pos = file.tellg();
	uint32_t magic = 0;
	file.read((char*)&magic, sizeof(uint32_t));

	if (magic != DDS_MAGIC)
	{
		file.seekg(pos);
		return false;
	}

	uint32_t size = 0;
	file.read((char*)&size, sizeof(uint32_t));

	if (size != sizeof(DDS_HEADER))
	{
		file.seekg(pos);
		return false;
	}

	file.seekg(-(int)sizeof(uint32_t), ios_base::cur);
	file.read((char*)&header, sizeof(DDS_HEADER));
	file.seekg(pos);
	return true;
}

/// <summary>
/// Reads the header from a DDS file.
/// </summary>
/// <param name="path">Path to the texture file.</param>
/// <param name="header">Output structure.</param>
/// <returns>Returns <c>true</c> if this file is a valid DDS file and <paramref name="header"/> has been populated.</returns>
bool GetDDSHeader(const string& path, DDS_HEADER& header)
{
	ifstream file(path, ios::binary);
	return GetDDSHeader(file, header);
}

/// <summary>
/// Loads a texture from the provided file stream.
/// </summary>
/// <param name="file">The file stream containing the texture data.</param>
/// <param name="offset">Offset at which the texture data starts.</param>
/// <param name="size">The size of the texture data.</param>
/// <param name="globalIndex">Global texture index for cache lookup.</param>
/// <param name="name">The name of the texture.</param>
/// <param name="mipmap">If <c>true</c>, automatically generate mipmaps.</param>
/// <param name="width">If non-zero, overrides texture width info in <see cref="NJS_TEXMEMLIST"/>.</param>
/// <param name="height">If non-zero, overrides texture height info in <see cref="NJS_TEXMEMLIST"/>.</param>
/// <returns>A pointer to the texture, or <c>nullptr</c> on failure.</returns>
NJS_TEXMEMLIST* LoadTextureStream(ifstream& file, uint64_t offset, uint64_t size,
	const string& path, uint32_t globalIndex, const string& name, bool mipmap, uint32_t width, uint32_t height)
{
	// TODO: Implement custom texture queue to replace the global texture array
	auto texture = GetCachedTexture(globalIndex);

	// GetCachedTexture will only return null if over 2048 unique textures have been loaded.
	if (texture == nullptr)
	{
		PrintDebug("Failed to allocate global texture for gbix %u (likely exceeded max global texture limit)\n", globalIndex);
		return nullptr;
	}

	uint32_t mip_levels = mipmap ? D3DX_DEFAULT : 1;
	auto texture_path = path + "\\" + name;

	// A texture count of 0 indicates that this is an empty texture slot.
	if (texture->count != 0)
	{
		// Increment the internal reference count to avoid the texture getting freed erroneously.
		++texture->count;

#ifdef _DEBUG
		PrintDebug("Using cached texture for GBIX %u (ref count: %u)\n", globalIndex, texture->count);
#endif
	}
	else
	{
		if (!mipmap)
		{
			DDS_HEADER header;
			if (GetDDSHeader(file, header))
			{
				mip_levels = header.mipMapCount + 1;
			}
		}

		vector<uint8_t> buffer;
		buffer.resize(static_cast<size_t>(size));

		file.seekg(static_cast<size_t>(offset));
		file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

		IDirect3DTexture8* d3dtexture = nullptr;
		HRESULT result = D3DXCreateTextureFromFileInMemoryEx(Direct3D_Device, buffer.data(), buffer.size(), D3DX_DEFAULT, D3DX_DEFAULT, mip_levels,
			0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr, &d3dtexture);

		if (result != D3D_OK)
		{
			PrintDebug("Failed to load texture %s: %s (%u)\n", name.c_str(), d3d_errors.at(result), result);
			return nullptr;
		}

		D3DSURFACE_DESC info;
		d3dtexture->GetLevelDesc(0, &info);

		auto w = !width ? info.Width : width;
		auto h = !height ? info.Height : height;

		// Now we assign some basic metadata from the texture entry and D3D texture, as well as the pointer to the texture itself.
		// A few things I know are missing for sure are:
		// NJS_TEXSURFACE::Type, Depth, Format, Flags. Virtual and Physical are pretty much always null.
		texture->count                          = 1; // Texture reference count.
		texture->globalIndex                    = globalIndex;
		texture->texinfo.texsurface.nWidth      = w;
		texture->texinfo.texsurface.nHeight     = h;
		texture->texinfo.texsurface.TextureSize = info.Size;
		texture->texinfo.texsurface.pSurface    = (Uint32*)d3dtexture;
	}

	for (auto& event : modCustomTextureLoadEvents)
	{
		event(texture, texture_path.c_str(), mip_levels);
	}

	return texture;
}


/// <summary>
/// Loads the specified texture from disk, or uses a cached texture if available.
/// </summary>
/// <param name="path">The texture pack directory containing the texture.</param>
/// <param name="globalIndex">Global texture index for cache lookup.</param>
/// <param name="name">The name of the texture.</param>
/// <param name="mipmap">If <c>true</c>, automatically generate mipmaps.</param>
/// <param name="width">If non-zero, overrides texture width info in <see cref="NJS_TEXMEMLIST"/>.</param>
/// <param name="height">If non-zero, overrides texture height info in <see cref="NJS_TEXMEMLIST"/>.</param>
/// <returns>A pointer to the texture, or <c>nullptr</c> on failure.</returns>
NJS_TEXMEMLIST* LoadTexture(const string& path, uint32_t globalIndex, const string& name,
	bool mipmap, uint32_t width, uint32_t height)
{
	// TODO: Implement custom texture queue to replace the global texture array
	auto texture = GetCachedTexture(globalIndex);

	// GetCachedTexture will only return null if over 2048 unique textures have been loaded.
	if (texture == nullptr)
	{
		PrintDebug("Failed to allocate global texture for gbix %u (likely exceeded max global texture limit)\n", globalIndex);
		return nullptr;
	}

	uint32_t mip_levels = mipmap ? D3DX_DEFAULT : 1;
	auto texture_path = path + "\\" + name;

	// A texture count of 0 indicates that this is an empty texture slot.
	if (texture->count != 0)
	{
		// Increment the internal reference count to avoid the texture getting freed erroneously.
		++texture->count;

#ifdef _DEBUG
		PrintDebug("Using cached texture for GBIX %u (ref count: %u)\n", globalIndex, texture->count);
#endif
	}
	else
	{
		if (!FileExists(texture_path))
		{
			PrintDebug("Texture does not exist: %s\n", texture_path.c_str());
			return nullptr;
		}

		ifstream file(texture_path, ios::ate | ios::binary | ios::in);
		auto size = static_cast<uint64_t>(file.tellg());
		file.seekg(0);
		return LoadTextureStream(file, 0, size, path, globalIndex, name, mipmap, width, height);
	}

	for (auto& event : modCustomTextureLoadEvents)
	{
		event(texture, texture_path.c_str(), mip_levels);
	}

	return texture;
}

/// <summary>
/// Loads the specified texture from disk, or uses a cached texture if available.
/// </summary>
/// <param name="path">A valid path to a texture pack directory containing index.txt.</param>
/// <param name="entry">The entry containing the global index and filename.</param>
/// <param name="mipmap">If <c>true</c>, automatically generate mipmaps.</param>
/// <returns>A pointer to the texture, or <c>nullptr</c> on failure.</returns>
NJS_TEXMEMLIST* LoadTexture(const string& path, const TexPackEntry& entry, bool mipmap)
{
	return LoadTexture(path, entry.globalIndex, entry.name, mipmap, entry.width, entry.height);
}

#pragma endregion

#pragma region PVM

static vector<NJS_TEXNAME> texname_overflow;

inline void dynamic_expand(NJS_TEXLIST* texlist, size_t count)
{
	if (count > TexNameBuffer_Length)
	{
		static const NJS_TEXNAME dummy = {};

		texname_overflow.resize(count);
		fill(texname_overflow.begin(), texname_overflow.end(), dummy);

		texlist->textures = texname_overflow.data();
	}
	else if (!texname_overflow.empty())
	{
		texname_overflow.clear();
	}

	texlist->nbTexture = count;
}

/// <summary>
/// Replaces the specified PVM with a texture pack virtual PVM.
/// </summary>
/// <param name="path">A valid path to a texture pack directory containing index.txt</param>
/// <param name="texlist">The associated texlist.</param>
/// <returns><c>true</c> on success.</returns>
static bool ReplacePVM(const string& path, NJS_TEXLIST* texlist)
{
	if (texlist == nullptr)
	{
		return false;
	}

	vector<TexPackEntry> index;
	if (!texpack::ParseIndex(path, index))
	{
		return false;
	}

	auto pvmName = GetBaseName(path);
	StripExtension(pvmName);

	bool mipmap = mipmap::AutoMipmapsEnabled() && !mipmap::IsBlacklistedPVM(pvmName.c_str());

	dynamic_expand(texlist, index.size());

	for (uint32_t i = 0; i < texlist->nbTexture; i++)
	{
		auto texture = LoadTexture(path, index[i], mipmap);

		if (texture == nullptr)
		{
			auto nbTexture = texlist->nbTexture;
			texlist->nbTexture = i;
			njReleaseTexture(texlist);
			texlist->nbTexture = nbTexture;
			return false;
		}

		texlist->textures[i].texaddr = reinterpret_cast<Uint32>(texture);
	}

	return true;
}

/// <summary>
/// Replaces the specified PVM with a texture pack PVMX archive.
/// </summary>
/// <param name="path">The path to the PVMX archive. Used for caching and error handling.</param>
/// <param name="file">An opened file stream for the PVMX archive.</param>
/// <param name="texlist">The associated texlist.</param>
/// <returns><c>true</c> on success.</returns>
static bool ReplacePVMX(const string& path, ifstream& file, NJS_TEXLIST* texlist)
{
	if (texlist == nullptr)
	{
		return false;
	}

	using namespace pvmx;
	vector<DictionaryEntry> index;

	if (!GetArchiveIndex(path, file, index))
	{
		return false;
	}

	auto pvmName = GetBaseName(path);
	StripExtension(pvmName);

	bool mipmap = mipmap::AutoMipmapsEnabled() && !mipmap::IsBlacklistedPVM(pvmName.c_str());

	dynamic_expand(texlist, index.size());

	for (size_t i = 0; i < index.size(); i++)
	{
		auto& entry = index[i];

		auto texture = LoadTextureStream(file, entry.offset, entry.size,
			path, entry.globalIndex, entry.name, mipmap, entry.width, entry.height);

		if (texture == nullptr)
		{
			njReleaseTexture(texlist);
			return false;
		}

		texlist->textures[i].texaddr = reinterpret_cast<Uint32>(texture);
	}

	return true;
}

static bool TryTexturePack(const char* filename, NJS_TEXLIST* texlist)
{
	mipmap::mip_guard _guard(mipmap::IsBlacklistedPVM(filename));

	string filename_str(filename);
	CheckCache();
	LoadingFile = true;

	// Since the filename can be passed in with or without an extension, first
	// we try getting a replacement with the filename as-is (with SYSTEM\ prepended).
	const string system_path = "SYSTEM\\" + filename_str;
	string replaced = sadx_fileMap.replaceFile(system_path.c_str());

	// But if that failed, we can assume that it was given without an extension
	// (which is the intended use) and append one before trying again.
	if (!Exists(replaced))
	{
		const string system_path_ext = system_path + ".PVM";
		replaced = sadx_fileMap.replaceFile(system_path_ext.c_str());
	}

	// If we can ensure this path exists, we can determine how to load it.
	if (!Exists(replaced))
	{
		return false;
	}

	// If the replaced path is a file, we should check if it's a PVMX archive.
	if (IsFile(replaced))
	{
		ifstream pvmx(replaced, ios::in | ios::binary);
		return pvmx::is_pvmx(pvmx) && ReplacePVMX(replaced, pvmx, texlist);
	}

	// Otherwise it's probably a directory, so try loading it as a texture pack.
	return ReplacePVM(replaced, texlist);
}

static int __cdecl LoadSystemPVM_r(const char* filename, NJS_TEXLIST* texlist)
{
	mipmap::mip_guard _guard(mipmap::IsBlacklistedPVM(filename));

	auto temp = *texlist;

	if (TryTexturePack(filename, &temp))
	{
		*texlist = temp;
		return 1;
	}

	return njLoadTexturePvmFile(filename, texlist);
}

static void __cdecl LoadPVM_r(const char* filename, NJS_TEXLIST* texlist)
{
	// Some fun stuff from the original code:

	if (!lstrcmpA(filename, "SS_MIZUGI"))
	{
		PrintDebug("SS_MIZUGI\n");
	}

	if (!texlist)
	{
		PrintDebug("%s %d : ptlo == 0\n", "\\SADXPC\\sonic\\src\\texture.c", 2675);
		return;
	}

	if (!texlist->nbTexture)
	{
		return;
	}

	NJS_TEXLIST temp = {};
	char textureNames[28 * TexNameBuffer_Length] = {};

	njSetPvmTextureList(&temp, TexNameBuffer, textureNames, TexNameBuffer_Length);

	if (LoadSystemPVM_r(filename, &temp) == -1)
	{
		return;
	}

	// Expand the static texlist with a dynamically allocated NJS_TEXNAME array.
	// This could become a memory leak for dynamically allocated NJS_TEXLISTs.
	if (temp.nbTexture > texlist->nbTexture)
	{
		auto textures = new NJS_TEXNAME[temp.nbTexture] {};

		memcpy(textures, texlist->textures, texlist->nbTexture * sizeof(NJS_TEXNAME));

		texlist->textures = textures;
		texlist->nbTexture = temp.nbTexture;
	}

	// Copy over the texture attributes and addresses.
	for (uint32_t i = 0; i < texlist->nbTexture; i++)
	{
		texlist->textures[i].attr = temp.textures[i].attr;
		texlist->textures[i].texaddr = temp.textures[i].texaddr;
	}
}

#pragma endregion

#pragma region PVR

/// <summary>
/// Loads texture pack replacement for specified PVR.
/// </summary>
/// <param name="filename">The PVR filename without extension.</param>
/// <param name="tex">The output TEXMEMLIST</param>
/// <returns><c>true</c> on success.</returns>
static bool ReplacePVR(const string& filename, NJS_TEXMEMLIST** tex)
{
	// NOTE: This cannot be `static const string` due to
	// TLS issues with MSVC 2017 on Windows XP.
	static const char index_file[] = "index.txt";

	// tl;dr compare the base name of the pvr with the base name of each texpack
	// entry until a mach is found; otherwise return false.

	string _filename = filename;
	transform(_filename.begin(), _filename.end(), _filename.begin(), tolower);

	string file_path = "system\\" + _filename + ".pvr";
	string index_path = sadx_fileMap.replaceFile(file_path.c_str());

	if (index_path == file_path)
	{
		return false;
	}

	auto offset = index_path.length() - (sizeof(index_file)-1);
	auto end = index_path.substr(offset);
	auto path = index_path.substr(0, --offset);

	if (end != index_file)
	{
		return false;
	}

	vector<TexPackEntry> entries;
	if (!texpack::ParseIndex(path, entries))
	{
		return false;
	}

	for (const auto& i : entries)
	{
		const auto &name = i.name;

		auto dot = name.find_last_of('.');
		if (dot == string::npos)
		{
			continue;
		}

		// Get the filename portion of the path.
		auto slash = name.find_last_of("/\\");
		slash = (slash == string::npos ? 0 : (slash+1));
		if (slash > dot)
		{
			// Should not happen, but this usually means the
			// dot is part of some other path component, not
			// the filename.
			continue;
		}

		string texture_name = name.substr(slash, dot - slash);
		transform(texture_name.begin(), texture_name.end(), texture_name.begin(), tolower);

		if (_filename != texture_name)
		{
			continue;
		}

		*tex = LoadTexture(path, i.globalIndex, name,
			!mipmap::IsBlacklistedPVR(filename.c_str()), i.width, i.height);

		return *tex != nullptr;
	}

	return false;
}

static Sint32 __cdecl njLoadTexture_Wrapper_r(NJS_TEXLIST* texlist)
{
	CheckCache();
	LoadingFile = true;
	return njLoadTexture_r(texlist);
}

static Sint32 __cdecl njLoadTexture_r(NJS_TEXLIST* texlist)
{
	NJS_TEXMEMLIST* memlist; // edi@7

	if (texlist == nullptr)
	{
		return -1;
	}

	for (Uint32 i = 0; i != texlist->nbTexture; i++)
	{
		NJS_TEXNAME* entries = &texlist->textures[i];
		Uint32 gbix = 0xFFFFFFEF;

		if (entries->attr & NJD_TEXATTR_GLOBALINDEX)
		{
			gbix = entries->texaddr;
		}

		DoSomethingWithPalette(unk_3CFC000[i]);
		Uint32 attr = entries->attr;

		// If already loaded, grab from memory. Otherwise, load from disk.
		// If type is memory, ->filename is NJS_TEXINFO or data, otherwise a string.
		if (attr & NJD_TEXATTR_TYPE_MEMORY)
		{
			if (attr & NJD_TEXATTR_GLOBALINDEX)
			{
				memlist = TexMemList_PixelFormat((NJS_TEXINFO*)entries->filename, gbix);
			}
			else
			{
				memlist = LoadPVR(*(void**)entries->filename, gbix);
			}
		}
		else
		{
			string filename((const char*)entries->filename);
			mipmap::mip_guard _guard(mipmap::IsBlacklistedPVR(filename.c_str()));

			if (ReplacePVR(filename, (NJS_TEXMEMLIST**)&entries->texaddr))
			{
				continue;
			}

			filename += ".pvr";
			void* data = LoadPVx(filename.c_str());
			memlist = LoadPVR(data, gbix);
			j__HeapFree_0(data);

			if (_guard.is_blacklisted() && memlist != nullptr)
			{
				mipmap::BlacklistGBIX(memlist->globalIndex);
			}
		}

		entries->texaddr = (Uint32)memlist;
	}

	return 1;
}

#pragma endregion
