/**
 * SADX Mod Loader
 * Code file parser.
 */

#ifndef CODEPARSER_HPP
#define CODEPARSER_HPP

#include <cstdint>
#include <iostream>
#include <list>
#include <string>
#include <vector>

class CodeParser
{
	public:
		CodeParser(const std::string &filename);
#ifdef _MSC_VER
		CodeParser(const std::wstring &filename);
#endif /* _MSC VER */
		CodeParser(std::istream &f);
		CodeParser();
		~CodeParser();

	private:
		// Disable the copy and assign constructors.
		CodeParser(const CodeParser &);
		CodeParser &operator=(const CodeParser &);

	public:
		/**
		 * Opcodes for the cheat code system.
		 */
		enum CodeType : uint8_t
		{
			write8, write16, write32, writefloat,
			add8, add16, add32, addfloat,
			sub8, sub16, sub32, subfloat,
			mulu8, mulu16, mulu32, mulfloat,
			muls8, muls16, muls32,
			divu8, divu16, divu32, divfloat,
			divs8, divs16, divs32,
			modu8, modu16, modu32,
			mods8, mods16, mods32,
			shl8, shl16, shl32,
			shru8, shru16, shru32,
			shrs8, shrs16, shrs32,
			rol8, rol16, rol32,
			ror8, ror16, ror32,
			and8, and16, and32,
			or8, or16, or32,
			xor8, xor16, xor32,
			ifeq8, ifeq16, ifeq32, ifeqfloat,
			ifne8, ifne16, ifne32, ifnefloat,
			ifltu8, ifltu16, ifltu32, ifltfloat,
			iflts8, iflts16, iflts32,
			ifltequ8, ifltequ16, ifltequ32, iflteqfloat,
			iflteqs8, iflteqs16, iflteqs32,
			ifgtu8, ifgtu16, ifgtu32, ifgtfloat,
			ifgts8, ifgts16, ifgts32,
			ifgtequ8, ifgtequ16, ifgtequ32, ifgteqfloat,
			ifgteqs8, ifgteqs16, ifgteqs32,
			ifmask8, ifmask16, ifmask32,
			ifkbkey,
			readreg8, readreg16, readreg32,
			writereg8, writereg16, writereg32,
			addreg8, addreg16, addreg32, addregfloat,
			subreg8, subreg16, subreg32, subregfloat,
			mulregu8, mulregu16, mulregu32, mulregfloat,
			mulregs8, mulregs16, mulregs32,
			divregu8, divregu16, divregu32, divregfloat,
			divregs8, divregs16, divregs32,
			modregu8, modregu16, modregu32,
			modregs8, modregs16, modregs32,
			shlreg8, shlreg16, shlreg32,
			shrregu8, shrregu16, shrregu32,
			shrregs8, shrregs16, shrregs32,
			rolreg8, rolreg16, rolreg32,
			rorreg8, rorreg16, rorreg32,
			andreg8, andreg16, andreg32,
			orreg8, orreg16, orreg32,
			xorreg8, xorreg16, xorreg32,
			ifeqreg8, ifeqreg16, ifeqreg32, ifeqregfloat,
			ifnereg8, ifnereg16, ifnereg32, ifneregfloat,
			ifltregu8, ifltregu16, ifltregu32, ifltregfloat,
			ifltregs8, ifltregs16, ifltregs32,
			iflteqregu8, iflteqregu16, iflteqregu32, iflteqregfloat,
			iflteqregs8, iflteqregs16, iflteqregs32,
			ifgtregu8, ifgtregu16, ifgtregu32, ifgtregfloat,
			ifgtregs8, ifgtregs16, ifgtregs32,
			ifgteqregu8, ifgteqregu16, ifgteqregu32, ifgteqregfloat,
			ifgteqregs8, ifgteqregs16, ifgteqregs32,
			ifmaskreg8, ifmaskreg16, ifmaskreg32,
			s8tos32, s16tos32, s32tofloat, u32tofloat, floattos32, floattou32,
			_else,
			endif,
			newregs,

			// End of file
			codeeof = 0xFF
		};

		/**
		 * Union of all possible types of values.
		 */
		union valuetype
		{
			uint32_t u32;
			int32_t s32;
			uint16_t u16;
			int16_t s16;
			uint8_t u8;
			int8_t s8;
			float f;
		};

		/**
		 * Code struct.
		 */
		struct Code
		{
			bool newregs;
			CodeType type;
			void *address;
			bool pointer;
			int offsetcount;
			int32_t *offsets;
			valuetype value;
			uint32_t repeatcount;
			std::list<Code> trueCodes;
			std::list<Code> falseCodes;
		};

	protected:
		static void *GetAddress(const Code &code, valuetype *regs);

		/**
		 * Process a code list. (Internal recursive function)
		 * @param codes Code list.
		 * @param regnum Next register number.
		 * @return Last register number used.
		 */
		int processCodeList_int(const std::list<Code> &codes, int regnum);

	public:
		/**
		 * Process the code list.
		 */
		void processCodeList(void);

	public:
		/**
		 * Read codes from a code file.
		 * This will clear all loaded codes before loading new codes.
		 * @param filename Code file.
		 * @return Number of codes read on success; -ENOENT if stream is closed; -EINVAL if file is invalid.
		 */
		int readCodes(const std::string &filename);

#ifdef _MSC_VER
		/**
		 * Read codes from a code file.
		 * This will clear all loaded codes before loading new codes.
		 * @param filename Code file.
		 * @return Number of codes read on success; -ENOENT if stream is closed; -EINVAL if file is invalid.
		 */
		int readCodes(const std::wstring &filename);
#endif /* _MSC_VER */

		/**
		 * Read codes from a code file.
		 * This will clear all loaded codes before loading new codes.
		 * @param stream Code file.
		 * @return Number of codes read on success; -ENOENT if stream is closed; -EINVAL if file is invalid.
		 */
		int readCodes(std::istream &stream);

	protected:
		/**
		 * Read codes from a code file.
		 * Internal recursive function; used for internal code lists.
		 * @param stream	[in] Code file.
		 * @param clist		[out] Code list.
		 * @return 0 when completed; codeeof on EOF; _else or endif while processing.
		 */
		unsigned char readCodes_int(std::istream &stream, std::list<Code> &clist);

		/**
		 * Clear the loaded codes.
		 */
		void clear(void);

	protected:
		/**
		 * Register sets.
		 * Each element is a pointer to 16 valuetypes.
		 */
		std::vector<valuetype*> m_registers;

		/**
		 * All loaded codes.
		 */
		std::list<Code> m_codes;
};

#endif /* CODEPARSER_HPP */
