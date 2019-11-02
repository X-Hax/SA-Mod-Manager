#pragma once
#include "MemAccess.h"

enum registers
{
	rEAX,
	rEBX,
	rECX,
	rEDX,
	rESI,
	rEDI,
	rEBP,
	rAX,
	rBX,
	rCX,
	rDX,
	rSI,
	rDI,
	rBP,
	rAL,
	rBL,
	rCL,
	rDL,
	rAH,
	rBH,
	rCH,
	rDH,
	stack1,
	stack2,
	stack4,
	rst0,
	noret
};

inline void writebytes(char* dst, int& dstoff, char a1, char a2)
{
	dst[dstoff++] = a1;
	dst[dstoff++] = a2;
}

inline void writebytes(char* dst, int& dstoff, char a1, char a2, char a3)
{
	dst[dstoff++] = a1;
	dst[dstoff++] = a2;
	dst[dstoff++] = a3;
}

inline void writebytes(char* dst, int& dstoff, char a1, char a2, char a3, char a4)
{
	dst[dstoff++] = a1;
	dst[dstoff++] = a2;
	dst[dstoff++] = a3;
	dst[dstoff++] = a4;
}

inline void writebytes(char* dst, int& dstoff, char a1, char a2, char a3, char a4, char a5)
{
	dst[dstoff++] = a1;
	dst[dstoff++] = a2;
	dst[dstoff++] = a3;
	dst[dstoff++] = a4;
	dst[dstoff++] = a5;
}

inline void writebytes(char* dst, int& dstoff, char a1, char a2, char a3, char a4, char a5, char a6)
{
	dst[dstoff++] = a1;
	dst[dstoff++] = a2;
	dst[dstoff++] = a3;
	dst[dstoff++] = a4;
	dst[dstoff++] = a5;
	dst[dstoff++] = a6;
}

inline void writebytes(char* dst, int& dstoff, char a1, char a2, char a3, char a4, char a5, char a6, char a7, char a8)
{
	dst[dstoff++] = a1;
	dst[dstoff++] = a2;
	dst[dstoff++] = a3;
	dst[dstoff++] = a4;
	dst[dstoff++] = a5;
	dst[dstoff++] = a6;
	dst[dstoff++] = a7;
	dst[dstoff++] = a8;
}

template<typename T, typename... TArgs>
constexpr T const GenerateUsercallCallWrapper(int ret, intptr_t address, TArgs... args)
{
	const size_t argc = sizeof...(TArgs);
	int argarray[] = { args... };
	int stackcnt = 0;
	int memsz = 0;
	for (size_t i = 0; i < argc; ++i)
	{
		switch (argarray[i])
		{
		case rEAX:
		case rEBX:
		case rECX:
		case rEDX:
		case rESI:
		case rEDI:
		case rEBP:
			memsz += 4;
			break;
		case rAX:
		case rBX:
		case rCX:
		case rDX:
		case rSI:
		case rDI:
		case rBP:
			memsz += 5;
			break;
		case rAL:
		case rBL:
		case rCL:
		case rDL:
		case rAH:
		case rBH:
		case rCH:
		case rDH:
			memsz += 4;
			break;
		case stack1:
		case stack2:
			memsz += 6;
			++stackcnt;
			break;
		case stack4:
			memsz += 4;
			++stackcnt;
			break;
		}
	}
	memsz += 5; // call
	if (stackcnt > 0)
		memsz += 3;
	switch (ret)
	{
	case rEBX:
	case rECX:
	case rEDX:
	case rESI:
	case rEDI:
	case rEBP:
		memsz += 2;
		break;
	case rBX:
	case rCX:
	case rDX:
	case rSI:
	case rDI:
	case rBP:
	case rBL:
	case rCL:
	case rDL:
	case rAH:
	case rBH:
	case rCH:
	case rDH:
		memsz += 3;
		break;
	case rst0:
		memsz += 8;
		break;
	}
	++memsz; // retn
	auto codeData = (char*)VirtualAlloc(nullptr, memsz, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	int cdoff = 0;
	char stackoff = 4;
	for (size_t i = 0; i < argc; ++i)
	{
		switch (argarray[i])
		{
		case rEAX:
			writebytes(codeData, cdoff, 0x8B, 0x44, 0x24, stackoff);
			stackoff += 4;
			break;
		case rEBX:
			writebytes(codeData, cdoff, 0x8B, 0x5C, 0x24, stackoff);
			stackoff += 4;
			break;
		case rECX:
			writebytes(codeData, cdoff, 0x8B, 0x4C, 0x24, stackoff);
			stackoff += 4;
			break;
		case rEDX:
			writebytes(codeData, cdoff, 0x8B, 0x54, 0x24, stackoff);
			stackoff += 4;
			break;
		case rESI:
			writebytes(codeData, cdoff, 0x8B, 0x74, 0x24, stackoff);
			stackoff += 4;
			break;
		case rEDI:
			writebytes(codeData, cdoff, 0x8B, 0x7C, 0x24, stackoff);
			stackoff += 4;
			break;
		case rEBP:
			writebytes(codeData, cdoff, 0x8B, 0x6C, 0x24, stackoff);
			stackoff += 4;
			break;
		case rAX:
			writebytes(codeData, cdoff, 0x66, 0x8B, 0x44, 0x24, stackoff);
			stackoff += 4;
			break;
		case rBX:
			writebytes(codeData, cdoff, 0x66, 0x8B, 0x5C, 0x24, stackoff);
			stackoff += 4;
			break;
		case rCX:
			writebytes(codeData, cdoff, 0x66, 0x8B, 0x4C, 0x24, stackoff);
			stackoff += 4;
			break;
		case rDX:
			writebytes(codeData, cdoff, 0x66, 0x8B, 0x54, 0x24, stackoff);
			stackoff += 4;
			break;
		case rSI:
			writebytes(codeData, cdoff, 0x66, 0x8B, 0x74, 0x24, stackoff);
			stackoff += 4;
			break;
		case rDI:
			writebytes(codeData, cdoff, 0x66, 0x8B, 0x7C, 0x24, stackoff);
			stackoff += 4;
			break;
		case rBP:
			writebytes(codeData, cdoff, 0x66, 0x8B, 0x6C, 0x24, stackoff);
			stackoff += 4;
			break;
		case rAL:
			writebytes(codeData, cdoff, 0x8A, 0x44, 0x24, stackoff);
			stackoff += 4;
			break;
		case rBL:
			writebytes(codeData, cdoff, 0x8A, 0x5C, 0x24, stackoff);
			stackoff += 4;
			break;
		case rCL:
			writebytes(codeData, cdoff, 0x8A, 0x4C, 0x24, stackoff);
			stackoff += 4;
			break;
		case rDL:
			writebytes(codeData, cdoff, 0x8A, 0x54, 0x24, stackoff);
			stackoff += 4;
			break;
		case rAH:
			writebytes(codeData, cdoff, 0x8A, 0x64, 0x24, stackoff);
			stackoff += 4;
			break;
		case rBH:
			writebytes(codeData, cdoff, 0x8A, 0x7C, 0x24, stackoff);
			stackoff += 4;
			break;
		case rCH:
			writebytes(codeData, cdoff, 0x8A, 0x6C, 0x24, stackoff);
			stackoff += 4;
			break;
		case rDH:
			writebytes(codeData, cdoff, 0x8A, 0x74, 0x24, stackoff);
			stackoff += 4;
			break;
		case stack1:
			writebytes(codeData, cdoff, 0x0F, 0xBE, 0x44, 0x24, stackoff, 0x50);
			break;
		case stack2:
			writebytes(codeData, cdoff, 0x0F, 0xBF, 0x44, 0x24, stackoff, 0x50);
			break;
		case stack4:
			writebytes(codeData, cdoff, 0xFF, 0x74, 0x24, stackoff);
			break;
		}
	}
	WriteCall(&codeData[cdoff], address);
	cdoff += 5;
	if (stackcnt > 0)
		writebytes(codeData, cdoff, 0x83, 0xC4, (char)(stackcnt * 4));
	switch (ret)
	{
	case rEBX:
		writebytes(codeData, cdoff, 0x89, 0xD8);
		break;
	case rECX:
		writebytes(codeData, cdoff, 0x89, 0xC8);
		break;
	case rEDX:
		writebytes(codeData, cdoff, 0x89, 0xD0);
		break;
	case rESI:
		writebytes(codeData, cdoff, 0x89, 0xF0);
		break;
	case rEDI:
		writebytes(codeData, cdoff, 0x89, 0xF8);
		break;
	case rEBP:
		writebytes(codeData, cdoff, 0x89, 0xE8);
		break;
	case rAX:
		writebytes(codeData, cdoff, 0x0F, 0xBF, 0xC0);
		break;
	case rBX:
		writebytes(codeData, cdoff, 0x0F, 0xBF, 0xC3);
		break;
	case rCX:
		writebytes(codeData, cdoff, 0x0F, 0xBF, 0xC1);
		break;
	case rDX:
		writebytes(codeData, cdoff, 0x0F, 0xBF, 0xC2);
		break;
	case rSI:
		writebytes(codeData, cdoff, 0x0F, 0xBF, 0xC6);
		break;
	case rDI:
		writebytes(codeData, cdoff, 0x0F, 0xBF, 0xC7);
		break;
	case rBP:
		writebytes(codeData, cdoff, 0x0F, 0xBF, 0xC5);
		break;
	case rAL:
		writebytes(codeData, cdoff, 0x0F, 0xBE, 0xC0);
		break;
	case rBL:
		writebytes(codeData, cdoff, 0x0F, 0xBE, 0xC3);
		break;
	case rCL:
		writebytes(codeData, cdoff, 0x0F, 0xBE, 0xC1);
		break;
	case rDL:
		writebytes(codeData, cdoff, 0x0F, 0xBE, 0xC2);
		break;
	case rAH:
		writebytes(codeData, cdoff, 0x0F, 0xBE, 0xC4);
		break;
	case rBH:
		writebytes(codeData, cdoff, 0x0F, 0xBE, 0xC7);
		break;
	case rCH:
		writebytes(codeData, cdoff, 0x0F, 0xBE, 0xC5);
		break;
	case rDH:
		writebytes(codeData, cdoff, 0x0F, 0xBE, 0xC6);
		break;
	case rst0:
		writebytes(codeData, cdoff, 0xD9, 0x5C, 0x24, 0xFC, 0x8B, 0x44, 0x24, 0xFC);
		break;
	}
	codeData[cdoff++] = 0xC3;
	static_assert(cdoff == memsz);
	return (T)codeData;
}
