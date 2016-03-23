#include "stdafx.h"
#include "Trampoline.h"

#include <Windows.h>
#include <exception>
#include <vector>
#include <ninja.h>		// for typedefs
#include <ModLoader/MemAccess.h>

/// <summary>
/// Initializes a new <see cref="Trampoline" />, allowing you to replace functions and still call the original code.
/// </summary>
/// <param name="start">Start offset (address of function).</param>
/// <param name="end">End offset.</param>
/// <param name="func">Your detour function.</param>
Trampoline::Trampoline(size_t start, size_t end, DetourFunction func) :
	target(nullptr), detour(nullptr), codeData(nullptr), originalSize(0), codeSize(0)
{
	if (start > end)
		throw std::exception("Start address cannot exceed end address.");

	if (end - start < 5)
		throw std::exception("Length cannot be less than 5 bytes.");

	target = (void*)start;
	detour = func;
	originalSize = end - start;
	codeSize = originalSize + 5;

	// Copy original instructions
	codeData = VirtualAlloc(nullptr, codeSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (codeData == nullptr)
		throw std::exception("VirtualAlloc failure");

	// ReadProcessMemory maybe?
	memcpy(codeData, target, originalSize);

	// Append jump (terribly)
	WriteJump(&((Uint8*)codeData)[originalSize], (void*)end);

	// NOP
	std::vector<Uint8> nop(originalSize, 0x90);
	WriteData(target, nop.data(), nop.size());

	// Jump
	WriteJump(target, func);
}

Trampoline::~Trampoline()
{
	if (codeData)
		VirtualFree(codeData, codeSize, MEM_DECOMMIT);
}
