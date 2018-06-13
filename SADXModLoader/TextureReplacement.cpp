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
#include "direct3d.h"
#include "DDS.h"
#include "AutoMipmap.h"

// This
#include "TextureReplacement.h"

#define TOMAPSTRING(a) { a, #a }

using namespace std;

static const unordered_map<HRESULT, const char*> D3D_ERRORS = {
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
static Sint32 __cdecl LoadPvmMEM2_r(const char* filename, NJS_TEXLIST* texlist);
static Sint32 __cdecl njLoadTexturePvmFile_r(const char *filename, NJS_TEXLIST *texList);

void texpack::init()
{
	WriteJump(static_cast<void*>(LoadSystemPVM), LoadSystemPVM_r);
	WriteJump(static_cast<void*>(njLoadTexture), njLoadTexture_r);
	WriteJump(static_cast<void*>(njLoadTexture_Wrapper), njLoadTexture_Wrapper_r);
	WriteJump(static_cast<void*>(LoadPVM), LoadPVM_r);
	WriteJump(static_cast<void*>(LoadPVM), LoadPVM_r);
	WriteJump(static_cast<void*>(LoadPvmMEM2), LoadPvmMEM2_r);
	WriteJump(static_cast<void*>(njLoadTexturePvmFile), njLoadTexturePvmFile_r);
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

static std::vector<uint8_t> get_prs_data(const string& path)
{
	std::ifstream file(path, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		return {};
	}

	const auto file_size = static_cast<size_t>(file.tellg());
	file.seekg(0);

	std::vector<uint8_t> in_buf(file_size);

	file.read(reinterpret_cast<char*>(in_buf.data()), file_size);
	file.close();

	const size_t decompress_size = prs_decompress_size(in_buf.data());

	if (!decompress_size)
	{
		return {};
	}

	std::vector<uint8_t> out_buf(decompress_size);
	out_buf.shrink_to_fit();

	size_t bytes_decompressed = prs_decompress(in_buf.data(), out_buf.data());

	if (bytes_decompressed != decompress_size)
	{
		return {};
	}

	return out_buf;
}

#pragma region Index Parsing

/**
* \brief Parses custom texture index.
* \param path A valid path to a texture pack directory containing index.txt
* \param out A vector to populate.
* \return \c true on success.
*/
bool texpack::parse_index(const string& path, vector<TexPackEntry>& out)
{
	PrintDebug("Loading texture pack: %s\n", path.c_str());

	const auto it = raw_cache.find(path);

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

	ifstream index_file(index);

	if (!index_file.is_open())
	{
		PrintDebug("Unable to open index file: %s\n", index.c_str());
		return false;
	}

	bool result = true;
	string line;
	uint32_t line_number = 0;

	// This reads the custom texture list from disk.
	// The format is: gbix,filename
	// e.g: 1000,MyCoolTexture.dds
	// TODO: Add GBIX bypass so texture packs can optionally ignore GBIX on load.

	while (!index_file.eof())
	{
		try
		{
			++line_number;
			getline(index_file, line);

			if (line.length() == 0 || line[0] == '#')
			{
				continue;
			}

			size_t comma = line.find(',');

			if (comma < 1)
			{
				PrintDebug("Invalid texture index entry on line %u (missing comma?)\n", line_number);
				result = false;
				break;
			}

			uint32_t width = 0;
			uint32_t height = 0;

			uint32_t gbix = stoul(line.substr(0, comma));
			auto name = line.substr(comma + 1);

			comma = name.find(',');

			// check for an additional texture dimensions field
			if (comma != string::npos && comma > 0)
			{
				auto tmp = name;
				name = tmp.substr(0, comma);

				auto dimensions = tmp.substr(++comma);
				size_t separator = dimensions.find('x');

				// If no 'x' separator is found, try capital
				if (!separator || separator == string::npos)
				{
					separator = dimensions.find('X');
				}

				if (!separator || separator == string::npos)
				{
					PrintDebug("Invalid format for texture dimensions on line %u: %s\n",
						line_number, dimensions.c_str());
					break;
				}

				width = stoul(dimensions.substr(0, separator));
				height = stoul(dimensions.substr(++separator));
			}

			auto texture_path = path;
			texture_path.append("\\");
			texture_path.append(name);

			if (!FileExists(texture_path))
			{
				PrintDebug("Texture entry on line %u does not exist: %s\n",
				           line_number, texture_path.c_str());
				result = false;
				break;
			}

			out.push_back({ gbix, name, width, height });
		}
		catch (exception& exception)
		{
			PrintDebug("An exception occurred while parsing texture index on line %u: %s\n",
			           line_number, exception.what());
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

bool get_archive_index(const string& path, ifstream& file, vector<pvmx::DictionaryEntry>& out)
{
	PrintDebug("Loading texture pack: %s\n", path.c_str());

	const auto it = archive_cache.find(path);

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

void check_cache()
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

bool get_dds_header(ifstream& file, DDS_HEADER& header)
{
	if (!file.is_open())
	{
		return false;
	}

	const auto pos = file.tellg();
	uint32_t magic = 0;
	file.read(reinterpret_cast<char*>(&magic), sizeof(uint32_t));

	if (magic != DDS_MAGIC)
	{
		file.seekg(pos);
		return false;
	}

	uint32_t size = 0;
	file.read(reinterpret_cast<char*>(&size), sizeof(uint32_t));

	if (size != sizeof(DDS_HEADER))
	{
		file.seekg(pos);
		return false;
	}

	file.seekg(-static_cast<int>(sizeof(uint32_t)), ios_base::cur);
	file.read(reinterpret_cast<char*>(&header), sizeof(DDS_HEADER));
	file.seekg(pos);
	return true;
}

/**
 * \brief Reads the header from a DDS file.
 * \param path Path to the texture file.
 * \param header Output structure.
 * \return \c true if this file is a valid DDS file and \p header has been populated.
 */
bool get_dds_header(const string& path, DDS_HEADER& header)
{
	ifstream file(path, ios::binary);
	return get_dds_header(file, header);
}

/**
 * \brief Loads a texture from the provided file stream.
 * \param file The file stream containing the texture data.
 * \param offset Offset at which the texture data starts.
 * \param size The size of the texture data.
 * \param path The path containing the texture defined by \p name.
 * \param global_index Global texture index for cache lookup.
 * \param name The name of the texture.
 * \param mipmap If \c true, automatically generate mipmaps.
 * \param width If non-zero, overrides texture width info in \c NJS_TEXMEMLIST.
 * \param height If non-zero, overrides texture height info in \c NJS_TEXMEMLIST.
 * \return A pointer to the texture, or \c nullptr on failure.
 */
NJS_TEXMEMLIST* load_texture_stream(ifstream& file, uint64_t offset, uint64_t size,
	const string& path, uint32_t global_index, const string& name, bool mipmap, uint32_t width, uint32_t height)
{
	// TODO: Implement custom texture queue to replace the global texture array
	auto texture = GetCachedTexture(global_index);

	// GetCachedTexture will only return null if over 2048 unique textures have been loaded.
	if (texture == nullptr)
	{
		PrintDebug("Failed to allocate global texture for gbix %u (likely exceeded max global texture limit)\n",
		           global_index);
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
		PrintDebug("Using cached texture for GBIX %u (ref count: %u)\n",
			       global_index, texture->count);
#endif
	}
	else
	{
		if (!mipmap)
		{
			DDS_HEADER header {};

			if (get_dds_header(file, header))
			{
				mip_levels = header.mipMapCount + 1;
			}
		}

		vector<uint8_t> buffer;
		buffer.resize(static_cast<size_t>(size));

		file.seekg(static_cast<size_t>(offset));
		file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

		IDirect3DTexture8* d3d_texture = nullptr;
		HRESULT result = D3DXCreateTextureFromFileInMemoryEx(Direct3D_Device, buffer.data(), buffer.size(), D3DX_DEFAULT, D3DX_DEFAULT, mip_levels,
			0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, nullptr, nullptr, &d3d_texture);

		if (result != D3D_OK)
		{
			PrintDebug("Failed to load texture %s: %s (%u)\n", name.c_str(), D3D_ERRORS.at(result), result);
			return nullptr;
		}

		D3DSURFACE_DESC info;
		d3d_texture->GetLevelDesc(0, &info);

		auto w = !width ? info.Width : width;
		auto h = !height ? info.Height : height;

		// Now we assign some basic metadata from the texture entry and D3D texture, as well as the pointer to the texture itself.
		// A few things I know are missing for sure are:
		// NJS_TEXSURFACE::Type, Depth, Format, Flags. Virtual and Physical are pretty much always null.
		texture->count                          = 1; // Texture reference count.
		texture->globalIndex                    = global_index;
		texture->texinfo.texsurface.nWidth      = w;
		texture->texinfo.texsurface.nHeight     = h;
		texture->texinfo.texsurface.TextureSize = info.Size;
		texture->texinfo.texsurface.pSurface    = reinterpret_cast<Uint32*>(d3d_texture);
	}

	for (auto& event : modCustomTextureLoadEvents)
	{
		event(texture, texture_path.c_str(), mip_levels);
	}

	return texture;
}

/**
 * \brief 
 * \param path The texture pack directory containing the texture.
 * \param global_index Global texture index for cache lookup.
 * \param name The name of the texture.
 * \param mipmap If \c true, automatically generate mipmaps.
 * \param width If non-zero, overrides texture width info in \c NJS_TEXMEMLIST.
 * \param height If non-zero, overrides texture height info in \c NJS_TEXMEMLIST.
 * \return A pointer to the texture, or \c nullptr on failure.
 */
NJS_TEXMEMLIST* load_texture(const string& path, uint32_t global_index, const string& name,
	bool mipmap, uint32_t width, uint32_t height)
{
	// TODO: Implement custom texture queue to replace the global texture array
	auto texture = GetCachedTexture(global_index);

	// GetCachedTexture will only return null if over 2048 unique textures have been loaded.
	if (texture == nullptr)
	{
		PrintDebug("Failed to allocate global texture for gbix %u (likely exceeded max global texture limit)\n",
		           global_index);
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
		PrintDebug("Using cached texture for GBIX %u (ref count: %u)\n", global_index, texture->count);
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
		return load_texture_stream(file, 0, size, path, global_index, name, mipmap, width, height);
	}

	for (auto& event : modCustomTextureLoadEvents)
	{
		event(texture, texture_path.c_str(), mip_levels);
	}

	return texture;
}

/**
 * \brief Loads the specified texture from disk, or uses a cached texture if available.
 * \param path A valid path to a texture pack directory containing index.txt.
 * \param entry The entry containing the global index and filename.
 * \param mipmap If \c true, automatically generate mipmaps.
 * \return A pointer to the texture, or \c nullptr on failure.
 */
NJS_TEXMEMLIST* load_texture(const string& path, const TexPackEntry& entry, bool mipmap)
{
	return load_texture(path, entry.global_index, entry.name, mipmap, entry.width, entry.height);
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

/**
 * \brief Replaces the specified PVM with a texture pack virtual PVM.
 * \param path A valid path to a texture pack directory containing index.txt.
 * \param texlist The associated texlist.
 * \return \c true on success
 */
static bool replace_pvm(const string& path, NJS_TEXLIST* texlist)
{
	if (texlist == nullptr)
	{
		return false;
	}

	vector<TexPackEntry> index;
	if (!texpack::parse_index(path, index))
	{
		return false;
	}

	auto pvm_name = GetBaseName(path);
	StripExtension(pvm_name);

	bool mipmap = mipmap::auto_mipmaps_enabled() && !mipmap::is_blacklisted_pvm(pvm_name.c_str());

	dynamic_expand(texlist, index.size());

	for (uint32_t i = 0; i < texlist->nbTexture; i++)
	{
		auto texture = load_texture(path, index[i], mipmap);

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

/**
 * \brief Replaces the specified PVM with a texture pack PVMX archive.
 * \param path The path to the PVMX archive. Used for caching and error handling.
 * \param file An opened file stream for the PVMX archive.
 * \param texlist The associated texlist.
 * \return \c true on success.
 */
static bool replace_pvmx(const string& path, ifstream& file, NJS_TEXLIST* texlist)
{
	if (texlist == nullptr)
	{
		return false;
	}

	using namespace pvmx;
	vector<DictionaryEntry> index;

	if (!get_archive_index(path, file, index))
	{
		return false;
	}

	auto pvm_name = GetBaseName(path);
	StripExtension(pvm_name);

	bool mipmap = mipmap::auto_mipmaps_enabled() && !mipmap::is_blacklisted_pvm(pvm_name.c_str());

	dynamic_expand(texlist, index.size());

	for (size_t i = 0; i < index.size(); i++)
	{
		auto& entry = index[i];

		auto texture = load_texture_stream(file, entry.offset, entry.size,
			path, entry.global_index, entry.name, mipmap, entry.width, entry.height);

		if (texture == nullptr)
		{
			njReleaseTexture(texlist);
			return false;
		}

		texlist->textures[i].texaddr = reinterpret_cast<Uint32>(texture);
	}

	return true;
}

static std::string get_replaced_path(const string& filename, const char* extension)
{
	// Since the filename can be passed in with or without an extension, first
	// we try getting a replacement with the filename as-is (with SYSTEM\ prepended).
	const string system_path = "SYSTEM\\" + filename;
	std::string replaced = sadx_fileMap.replaceFile(system_path.c_str());

	// But if that failed, we can assume that it was given without an extension
	// (which is the intended use) and append one before trying again.
	if (!Exists(replaced))
	{
		const string system_path_ext = system_path + extension;
		replaced = sadx_fileMap.replaceFile(system_path_ext.c_str());
	}

	return replaced;
}

static bool try_texture_pack(const char* filename, NJS_TEXLIST* texlist)
{
	string filename_str(filename);
	StripExtension(filename_str);
	mipmap::MipGuard _guard(mipmap::is_blacklisted_pvm(filename_str.c_str()));

	check_cache();
	LoadingFile = true;

	const string replaced = get_replaced_path(filename_str, ".PVM");

	// If we can ensure this path exists, we can determine how to load it.
	if (!Exists(replaced))
	{
		return false;
	}

	// If the replaced path is a file, we should check if it's a PVMX archive.
	if (IsFile(replaced))
	{
		ifstream pvmx(replaced, ios::in | ios::binary);
		return pvmx::is_pvmx(pvmx) && replace_pvmx(replaced, pvmx, texlist);
	}

	// Otherwise it's probably a directory, so try loading it as a texture pack.
	return replace_pvm(replaced, texlist);
}

static int __cdecl LoadSystemPVM_r(const char* filename, NJS_TEXLIST* texlist)
{
	mipmap::MipGuard _guard(mipmap::is_blacklisted_pvm(filename));

	auto temp = *texlist;

	if (try_texture_pack(filename, &temp))
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
	char texture_names[28 * TexNameBuffer_Length] = {};

	njSetPvmTextureList(&temp, TexNameBuffer, texture_names, TexNameBuffer_Length);

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

static Sint32 LoadPvmMEM2_r(const char* filename, NJS_TEXLIST* texlist)
{
	PrintDebug("LoadPvmMEM2 - %s\n", filename);

	auto temp = *texlist;

	// set LoadingFile to 1 to attempt cache lookups
	const auto last = LoadingFile;
	LoadingFile = 1;

	if (try_texture_pack(filename, &temp))
	{
		LoadingFile = last;
		*texlist = temp;
		return 1;
	}

	LoadingFile = last;
	return njLoadTexturePvmFile(filename, texlist);
}

static Sint32 njLoadTexturePvmFile_r(const char* filename, NJS_TEXLIST* texList)
{
	if (filename == nullptr || texList == nullptr)
	{
		return -1;
	}

	const std::string replaced = get_replaced_path(filename, ".PVM");
	const std::string reaplced_extension = GetExtension(replaced);

	if (!_stricmp(reaplced_extension.c_str(), "prs"))
	{
		//PrintDebug("Loading PRS'd PVM: %s\n", filename);

		auto out_buf = get_prs_data(replaced);

		if (out_buf.empty())
		{
			return -1;
		}

		return njLoadTexturePvmMemory(out_buf.data(), texList);
	}

	std::string name = filename;
	const std::string extension = GetExtension(name);

	if (extension.empty())
	{
		name += ".PVM";
	}

	Uint8* data = LoadPVx(name.c_str());
	Sint32 result = njLoadTexturePvmMemory(data, texList);
	j__HeapFree_0(data);
	return result;
}

#pragma endregion

#pragma region PVR

/**
 * \brief Loads texture pack replacement for specified PVR.
 * \param filename The PVR filename without extension.
 * \param tex The output \c NJS_TEXMEMLIST.
 * \return \c true on success.
 */
static bool replace_pvr(const string& filename, NJS_TEXMEMLIST** tex)
{
	// NOTE: This cannot be `static const string` due to
	// TLS issues with MSVC 2017 on Windows XP.
	static const char index_file[] = "index.txt";

	// tl;dr compare the base name of the pvr with the base name of each texpack
	// entry until a mach is found; otherwise return false.

	string filename_str = filename;
	transform(filename_str.begin(), filename_str.end(), filename_str.begin(), tolower);

	string file_path = "system\\" + filename_str + ".pvr";
	string index_path = sadx_fileMap.replaceFile(file_path.c_str());

	if (index_path == file_path)
	{
		return false;
	}

	auto offset = index_path.length() - (sizeof(index_file)-1);
	const auto end = index_path.substr(offset);
	const auto path = index_path.substr(0, --offset);

	if (end != index_file)
	{
		return false;
	}

	vector<TexPackEntry> entries;
	if (!texpack::parse_index(path, entries))
	{
		return false;
	}

	for (const auto& i : entries)
	{
		const auto &name = i.name;

		const auto dot = name.find_last_of('.');
		if (dot == string::npos)
		{
			continue;
		}

		// Get the filename portion of the path.
		auto slash = name.find_last_of("/\\");
		slash = (slash == string::npos ? 0 : (slash + 1));
		if (slash > dot)
		{
			// Should not happen, but this usually means the
			// dot is part of some other path component, not
			// the filename.
			continue;
		}

		string texture_name = name.substr(slash, dot - slash);
		transform(texture_name.begin(), texture_name.end(), texture_name.begin(), tolower);

		if (filename_str != texture_name)
		{
			continue;
		}

		*tex = load_texture(path, i.global_index, name,
			!mipmap::is_blacklisted_pvr(filename.c_str()), i.width, i.height);

		return *tex != nullptr;
	}

	return false;
}

static Sint32 __cdecl njLoadTexture_Wrapper_r(NJS_TEXLIST* texlist)
{
	check_cache();
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
				memlist = TexMemList_PixelFormat(static_cast<NJS_TEXINFO*>(entries->filename), gbix);
			}
			else
			{
				memlist = LoadPVR(*static_cast<void**>(entries->filename), gbix);
			}
		}
		else
		{
			string filename(static_cast<const char*>(entries->filename));
			mipmap::MipGuard _guard(mipmap::is_blacklisted_pvr(filename.c_str()));

			if (replace_pvr(filename, reinterpret_cast<NJS_TEXMEMLIST**>(&entries->texaddr)))
			{
				continue;
			}

			const string replaced = get_replaced_path(filename, ".PVR");

			if (!_stricmp(GetExtension(replaced).c_str(), "prs"))
			{
				//PrintDebug("Loading PRS'd PVR: %s\n", filename.c_str());

				auto out_buf = get_prs_data(replaced);

				if (out_buf.empty())
				{
					return -1;
				}

				memlist = LoadPVR(out_buf.data(), gbix);
			}
			else
			{
				filename += ".pvr";
				void* data = LoadPVx(filename.c_str());
				memlist = LoadPVR(data, gbix);
				j__HeapFree_0(data);
			}

			if (_guard.is_blacklisted() && memlist != nullptr)
			{
				mipmap::blacklist_gbix(memlist->globalIndex);
			}
		}

		entries->texaddr = reinterpret_cast<Uint32>(memlist);
	}

	return 1;
}

#pragma endregion
