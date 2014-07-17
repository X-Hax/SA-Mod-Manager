/**
 * SADX Mod Loader
 * Code file parser.
 */

#ifndef CODEPARSER_HPP
#define CODEPARSER_HPP

#include <cstdint>
#include <iostream>
#include <list>

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
	_else,
	endif,
	newregs
};

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

/**
 * Process a code list.
 * @param codes Code list.
 */
void ProcessCodeList(const std::list<Code> &codes);

/**
 * Read codes from a code file.
 * @param stream Code file.
 * @param list Output list.
 * @return ???
 */
unsigned char ReadCodes(std::istream &stream, std::list<Code> &list);

#endif /* CODEPARSER_HPP */
