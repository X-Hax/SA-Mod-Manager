Sonic Adventure DX PC Mod Loader
================================
Code File Format
----------------


Table of Contents
-----------------

1. What is the Code File?
2. Opcodes
3. XML format
4. Binary format


1. What is the Code File?
-------------------------

The code file contains a set of patch codes that are applied to the game in
memory. The functionality is similar to the Wii Ocarina system as well as
later Game Shark and Action Replay devices in that it supports comparisons
and branching as well as assorted arithmetic and logic operations.


2. Opcodes
----------

Each code instruction has an 8-bit opcode and parameters. The opcodes
are detailed below.

For instructions with multiple formats, the formats are as follows:
  * 8, u8: unsigned 8-bit integer
  * 16, u16: unsigned 16-bit integer
  * 32, u32: unsigned 32-bit integer
  * float: 32-bit floating-point number
  * s8: signed 8-bit integer
  * s16: signed 16-bit integer
  * s32: signed 32-bit integer
  * kbkey: Win32 virtual key code

`write8`, `write16`, `write32`, `writefloat`: `*address = value;`

    0x00: write8
    0x01: write16
    0x02: write32
    0x03: writefloat

`add8`, `add16`, `add32`, `addfloat`: `*address += value;`

    0x04: add8
    0x05: add16
    0x06: add32
    0x07: addfloat

`sub8`, `sub16`, `sub32`, `subfloat`: `*address -= value;`

    0x08: sub8
    0x09: sub16
    0x0A: sub32
    0x0B: subfloat

`mulu8`, `mulu16`, `mulu32`, `mulfloat`, `muls8`, `muls16`, `muls32`: `*address *= value;`

    0x0C: mulu8
    0x0D: mulu16
    0x0E: mulu32
    0x0F: mulfloat
    0x10: muls8
    0x11: muls16
    0x12: muls32

`divu8`, `divu16`, `divu32`, `divfloat`, `divs8`, `divs16`, `divs32`: `*address /= value;`

    0x13: divu8
    0x14: divu16
    0x15: divu32
    0x16: divfloat
    0x17: divs8
    0x18: divs16
    0x19: divs32

`modu8`, `modu16`, `modu32`, `mods8`, `mods16`, `mods32`: `*address %= value;`

    0x1A: modu8
    0x1B: modu16
    0x1C: modu32
    0x1D: mods8
    0x1E: mods16
    0x1F: mods32

`shl8`, `shl16`, `shl32`: `*address <<= value;`

    0x20: shl8
    0x21: shl16
    0x22: shl32

`shru8`, `shru16`, `shru32`: Logical shift right.

    0x23: shru8
    0x24: shru16
    0x25: shru32

`shrs8`, `shrs16`, `shrs32`: Arithmetic shift right.

    0x26: shrs8
    0x27: shrs16
    0x28: shrs32

`rol8`, `rol16`, `rol32`: Rotate left.

    0x29: rol8
    0x2A: rol16
    0x2B: rol32

`ror8`, `ror16`, `ror32`: Rotate right.

    0x2C: ror8
    0x2D: ror16
    0x2E: ror32

`and8`, `and16`, `and32`: `*address &= value;`

    0x2F: and8
    0x30: and16
    0x31: and32

`or8`, `or16`, `or32`: `*address |= value;`

    0x32: or8
    0x33: or16
    0x34: or32

`xor8`, `xor16`, `xor32`: `*address ^= value;`

    0x35: xor8
    0x36: xor16
    0x37: xor32
    
`writenop`: Replaces `value` bytes starting at `address` with `0x90` (x86 opcode for NOP)
    
    0x38: writenop
    
`ifeq8`, `ifeq16`, `ifeq32`, `ifeqfloat`: `if (*address == value)`

    0x39: ifeq8
    0x3A: ifeq16
    0x3B: ifeq32
    0x3C: ifeqfloat

`ifne8`, `ifne16`, `ifne32`, `ifnefloat`: `if (*address != value)`

    0x3D: ifne8
    0x3E: ifne16
    0x3F: ifne32
    0x40: ifnefloat

`ifltu8`, `ifltu16`, `ifltu32`, `ifltfloat`, `iflts8`, `iflts16`, `iflts32`: `if (*address < value)`

    0x41: ifltu8
    0x42: ifltu16
    0x43: ifltu32
    0x44: ifltfloat
    0x45: iflts8
    0x46: iflts16
    0x47: iflts32

`ifltequ8`, `ifltequ16`, `ifltequ32`, `iflteqfloat`, `iflteqs8`, `iflteqs16`, `iflteqs32`: `if (*address <= value)`

    0x48: ifltequ8
    0x49: ifltequ16
    0x4A: ifltequ32
    0x4B: iflteqfloat
    0x4C: iflteqs8
    0x4D: iflteqs16
    0x4E: iflteqs32

`ifgtu8`, `ifgtu16`, `ifgtu32`, `ifgtfloat`, `ifgts8`, `ifgts16`, `ifgts32`: `if (*address > value)`

    0x4F: ifgtu8
    0x50: ifgtu16
    0x51: ifgtu32
    0x52: ifgtfloat
    0x53: ifgts8
    0x54: ifgts16
    0x55: ifgts32

`ifgtequ8`, `ifgtequ16`, `ifgtequ32`, `ifgteqfloat`, `ifgteqs8`, `ifgteqs16`, `ifgteqs32`: `if (*address >= value)`

    0x56: ifgtequ8
    0x57: ifgtequ16
    0x58: ifgtequ32
    0x59: ifgteqfloat
    0x5A: ifgteqs8
    0x5B: ifgteqs16
    0x5C: ifgteqs32

`ifmask8`, `ifmask16`, `ifmask32`: `if (*address & value == value)`

    0x5D: ifmask8
    0x5E: ifmask16
    0x5F: ifmask32

`ifkbkey`: If keyboard key is pressed

    0x60: ifkbkey

`readreg8`, `readreg16`, `readreg32`: `reg[value] = *address;`

    0x61: readreg8
    0x62: readreg16
    0x63: readreg32

`writereg8`, `writereg16`, `writereg32`: `*address = reg[value];`

    0x64: writereg8
    0x65: writereg16
    0x66: writereg32

`addreg8`, `addreg16`, `addreg32`, `addregfloat`: `*address += reg[value];`

    0x67: addreg8
    0x68: addreg16
    0x69: addreg32
    0x6A: addregfloat

`subreg8`, `subreg16`, `subreg32`, `subregfloat`: `*address -= reg[value];`

    0x6B: subreg8
    0x6C: subreg16
    0x6D: subreg32
    0x6E: subregfloat

`mulregu8`, `mulregu16`, `mulregu32`, `mulregfloat`, `mulregs8`, `mulregs16`, `mulregs32`: `*address *= reg[value];`

    0x6F: mulregu8
    0x70: mulregu16
    0x71: mulregu32
    0x72: mulregfloat
    0x73: mulregs8
    0x74: mulregs16
    0x75: mulregs32

`divregu8`, `divregu16`, `divregu32`, `divregfloat`, `divregs8`, `divregs16`, `divregs32`: `*address /= reg[value];`

    0x76: divregu8
    0x77: divregu16
    0x78: divregu32
    0x79: divregfloat
    0x7A: divregs8
    0x7B: divregs16
    0x7C: divregs32

`modregu8`, `modregu16`, `modregu32`, `modregs8`, `modregs16`, `modregs32`: `*address %= reg[value];`

    0x7D: modregu8
    0x7E: modregu16
    0x7F: modregu32
    0x80: modregs8
    0x81: modregs16
    0x82: modregs32

`shlreg8`, `shlreg16`, `shlreg32`: `*address <<= reg[value];`

    0x83: shlreg8
    0x84: shlreg16
    0x85: shlreg32

`shrregu8`, `shrregu16`, `shrregu32`: Logical shift right `*address` by `reg[value]` bits.

    0x86: shrregu8
    0x87: shrregu16
    0x88: shrregu32

`shrregs8`, `shrregs16`, `shrregs32`: Arithmetic shift right `*address` by `reg[value]` bits.

    0x89: shrregs8
    0x8A: shrregs16
    0x8B: shrregs32

`rolreg8`, `rolreg16`, `rolreg32`: Rotate left `*address` by `reg[value]` bits.

    0x8C: rolreg8
    0x8D: rolreg16
    0x8E: rolreg32

`rorreg8`, `rorreg16`, `rorreg32`: Rotate right `*address` by `reg[value]` bits.

    0x8F: rorreg8
    0x90: rorreg16
    0x91: rorreg32

`andreg8`, `andreg16`, `andreg32`: `*address &= reg[value];`

    0x92: andreg8
    0x93: andreg16
    0x94: andreg32

`orreg8`, `orreg16`, `orreg32`: `*address |= reg[value];`

    0x95: orreg8
    0x96: orreg16
    0x97: orreg32

`xorreg8`, `xorreg16`, `xorreg32`: `*address ^= reg[value];`

    0x98: xorreg8
    0x99: xorreg16
    0x9A: xorreg32
    
`writenopreg`: Replaces `reg[value]` bytes starting at `address` with `0x90` (x86 opcode for NOP)
    
    0x9B: writenopreg
    
`ifeqreg8`, `ifeqreg16`, `ifeqreg32`, `ifeqregfloat`: `if (*address == reg[value])`

    0x9C: ifeqreg8
    0x9D: ifeqreg16
    0x9E: ifeqreg32
    0x9F: ifeqregfloat

`ifnereg8`, `ifnereg16`, `ifnereg32`, `ifneregfloat`: `if (*address != reg[value])`

    0xA0: ifnereg8
    0xA1: ifnereg16
    0xA2: ifnereg32
    0xA3: ifneregfloat

`ifltregu8`, `ifltregu16`, `ifltregu32`, `ifltregfloat`, `ifltregs8`, `ifltregs16`, `ifltregs32`: `if (*address < reg[value])`

    0xA4: ifltregu8
    0xA5: ifltregu16
    0xA6: ifltregu32
    0xA7: ifltregfloat
    0xA8: ifltregs8
    0xA9: ifltregs16
    0xAA: ifltregs32

`iflteqregu8`, `iflteqregu16`, `iflteqregu32`, `iflteqregfloat`, `iflteqregs8`, `iflteqregs16`, `iflteqregs32`: `if (*address <= reg[value])`

    0xAB: iflteqregu8
    0xAC: iflteqregu16
    0xAD: iflteqregu32
    0xAE: iflteqregfloat
    0xAF: iflteqregs8
    0xB0: iflteqregs16
    0xB1: iflteqregs32

`ifgtregu8`, `ifgtregu16`, `ifgtregu32`, `ifgtregfloat`, `ifgtregs8`, `ifgtregs16`, `ifgtregs32`: `if (*address > reg[value])`

    0xB2: ifgtregu8
    0xB3: ifgtregu16
    0xB4: ifgtregu32
    0xB5: ifgtregfloat
    0xB6: ifgtregs8
    0xB7: ifgtregs16
    0xB8: ifgtregs32

`ifgteqregu8`, `ifgteqregu16`, `ifgteqregu32`, `ifgteqregfloat`, `ifgteqregs8`, `ifgteqregs16`, `ifgteqregs32`: `if (*address >= reg[value])`

    0xB9: ifgteqregu8
    0xBA: ifgteqregu16
    0xBB: ifgteqregu32
    0xBC: ifgteqregfloat
    0xBD: ifgteqregs8
    0xBE: ifgteqregs16
    0xBF: ifgteqregs32

`ifmaskreg8`, `ifmaskreg16`, `ifmaskreg32`: `if (*address & reg[value] == reg[value])`

    0xC0: ifmaskreg8
    0xC1: ifmaskreg16
    0xC2: ifmaskreg32
    
`s8tos32`, `s16tos32`, `s32tofloat`, `u32tofloat`, `floattos32`, `floattou32`: `*address = (dsttype)*address`

    0xC3: s8tos32
    0xC4: s16tos32
    0xC5: s32tofloat
    0xC6: u32tofloat
    0xC7: floattos32
    0xC8: floattou32
    
`_else`: 'else' condition block. Use after an 'if' opcode.

    0xC9: else

`endif`: 'endif'. Use after 'if' or 'else' opcodes.

    0xCA: endif

`newregs`: Create a new register set.

    0xCB: newregs

`codeeof`: End of file.

    0xFF: codeeof


3. XML format
-------------

See the [XML schema](CodeList.xsd)


4. Binary format
----------------

Code files start with the following struct:

    #pragma pack(1)
    struct CodeFileHeader {
        char magic[6];	// "codev5"
        int codes;	// Number of codes in the file.
    };
    #pragma pack()

"codes" indicates the number of codes present in the file.

Immediately following the CodeFileHeader is a list of codes. Note that
codes are variable-length depending on what's implemented.

Each code has the following format. The code list is terminated with
an opcode codeeof (0xFF); anything following the 0xFF is ignored.

Offset 0: opcode (uint8_t)

Opcode is an 8-bit value defined in section 2 above. This indicates
what function the code has.

Offset 1: address (uint32_t)

Address is the 32-bit address that the code should operate on.
If bit 31 (0x80000000) is set, this address is the base of an
offset table, which immediately follows the address. The offset
table starts with an int32_t indicating how many offsets are
present, followed by that many int32_t values.

Offset 5(+offsettable): value (uint32_t)

Value is a 32-bit value that should be written to the specified address.
Depending on the opcode, this may be interpreted as a signed or unsigned
8-bit, 16-bit, or 32-bit integer, or a 32-bit floating-point number.
All formats are little-endian.

Offset 9(+offsettable): repeatcount (uint32_t)

RepeatCount indicates how many times the value should be written to
the address. For example, if the value is (uint32_t)0x12345678,
address is 0x1000, and repeatcount is 2, the value 0x12345678
will be written to 0x1000 and 0x1004.

Offset 13(+offsettable): trueCodes/falseCodes

This section only exists for opcodes starting with 'if'.

For 'if' opcodes, this section contains a set of codes that are executed
if the condition specified in the opcode is true. This block is terminated
with an '_else' or 'endif' opcode.

If the 'if' code block was terminated with an '_else' opcode, then that
opcode is followed by another set of codes that are executed if the condition
specified by the 'if' code block is false. This block must be terminated
with an 'endif' opcode.

NOTE: If the 'if' opcode has a pointer-type address and any of the pointers
in the offset table evaluates to NULL, the condition will be evaluated as
false.
