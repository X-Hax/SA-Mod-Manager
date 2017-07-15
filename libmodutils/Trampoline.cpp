#include "stdafx.h"
#include "Trampoline.h"

#include <windows.h>
#include <exception>
#include <vector>
#include <ninja.h>		// for typedefs
#include <MemAccess.h>

Trampoline::Trampoline(intptr_t start, intptr_t end, void* func, bool destructRevert)
	: target(reinterpret_cast<void*>(start))
	, detour(func)
	, codeData(nullptr)
	, originalSize(0)
	, codeSize(0)
	, revert(destructRevert)
{
	if (start > end)
		throw std::exception("Start address cannot exceed end address.");
	if (end - start < 5)
		throw std::exception("Length cannot be less than 5 bytes.");

	originalSize = end - start;
	codeSize = originalSize + 5;	// TODO: Probably not needed...

	// Copy original instructions
	codeData = VirtualAlloc(nullptr, codeSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (codeData == nullptr)
		throw std::exception("VirtualAlloc failure.");

	// memcpy() can be used instead of ReadProcessMemory
	// because we're not reading memory from another process.
	memcpy(codeData, target, originalSize);

	uint8_t *const ptr = static_cast<uint8_t*>(codeData);

	// If an existing (hopefully) trampoline has been applied to this address,
	// correct the jump offset for it.
	if (ptr[0] == 0xE9)
	{
		intptr_t addr = start + 5 + *(reinterpret_cast<intptr_t*>(&ptr[1]));
		WriteJump(ptr, (void*)addr);
	}

	// Append jump (terribly)
	WriteJump(&ptr[originalSize], (void*)end);

	// NOP the original code.
	// NOTE: This is in .text, so we have to use WriteData().
	// Using memset() will crash.
	std::vector<uint8_t> nop(originalSize, 0x90);
	WriteData(target, nop.data(), nop.size());

	// Write a Jump to the new target function.
	WriteJump(target, func);
}

Trampoline::~Trampoline()
{
	if (codeData)
	{
		if (revert)
			WriteData(target, codeData, originalSize);

		// FIXME: MEM_RELEASE caused problems.
		// Using MEM_DECOMMIT instead of MEM_RELEASE frees
		// physical memory, but leaks address space.
		VirtualFree(codeData, codeSize, MEM_DECOMMIT);
	}
}
