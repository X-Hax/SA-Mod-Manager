#ifndef LANDTABLEINFO_H
#define LANDTABLEINFO_H

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "SADXModLoader.h"

class LandTableInfo
{
public:
	struct Metadata { uint32_t size; const uint8_t *data; };

	explicit LandTableInfo(const char *filename);
	explicit LandTableInfo(const std::string &filename);
#ifdef _MSC_VER
	explicit LandTableInfo(const wchar_t *filename);
	explicit LandTableInfo(const std::wstring &filename);
#endif /* _MSC_VER */
	explicit LandTableInfo(std::istream &stream);

	LandTable *getlandtable();
	const std::string &getauthor();
	const std::string &gettool();
	const std::string &getdescription();
	const uint8_t *getmetadata(uint32_t identifier, uint32_t &size);
	const std::string &getlabel(void *data);
	void *getdata(const std::string &label);
	const std::unordered_map<std::string, void *> *getlabels();

private:
	static const uint64_t SA1LVL = 0x4C564C314153ULL;
	static const uint64_t FormatMask = 0xFFFFFFFFFFFFULL;
	static const uint8_t CurrentVersion = 3;
	static const int headersize = 0x10;

	LandTable *landtable;
	std::string author, tool, description;
	std::unordered_map<uint32_t, Metadata> metadata;
	std::unordered_map<void *, std::string> labels1;
	std::unordered_map<std::string, void *> labels2;
	std::vector<std::shared_ptr<void> > allocatedmem;
	std::unordered_set<void *> fixedpointers;
	std::unordered_map<void *, void *> reallocateddata;

	enum ChunkTypes : uint32_t
	{
		ChunkTypes_Label = 0x4C42414C,
		ChunkTypes_Author = 0x48545541,
		ChunkTypes_Tool = 0x4C4F4F54,
		ChunkTypes_Description = 0x43534544,
		ChunkTypes_End = 0x444E45
	};

	void fixmodelpointers(NJS_MODEL_SADX *model, intptr_t base);
	void fixobjectpointers(NJS_OBJECT *object, intptr_t base);
	void fixmotionpointers(NJS_MOTION *motion, intptr_t base, int count);
	void fixactionpointers(NJS_ACTION *action, intptr_t base);
	void fixlandtablepointers(LandTable *landtable, intptr_t base);
	void init(std::istream &stream);
};

#endif /* LANDTABLEINFO_H */
