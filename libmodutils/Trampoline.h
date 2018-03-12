#pragma once

#include <WTypes.h>

// TODO: Better documentation
// TODO: Clearer member names

class Trampoline
{
private:
	void* target;
	void* detour;
	LPVOID codeData;
	size_t originalSize;
	const bool revert;

public:
	/// <summary>
	/// Initializes a new <see cref="Trampoline" />, allowing you to replace functions and still call the original code.
	/// </summary>
	/// <param name="start">Start offset (address of function).</param>
	/// <param name="end">End offset.</param>
	/// <param name="func">Your detour function.</param>
	/// <param name="destructRevert">If <c>true</c>, code changes will be reverted when this instance is destroyed.</param>
	/// <remarks>
	/// If the start address begins with a jump or call instruction, the relative address will be automatically repaired.
	/// Any subsequent jumps or calls caught in the range of <paramref name="start"/> and <paramref name="end"/> will need
	/// to be repaired manually.
	/// </remarks>
	Trampoline(intptr_t start, intptr_t end, void* func, bool destructRevert = true);
	~Trampoline();

	// Pointer to original code.
	LPVOID Target() const
	{
		return codeData;
	}
	// Pointer to your detour.
	void* Detour() const
	{
		return detour;
	}
	// Original data size.
	size_t OriginalSize() const
	{
		return originalSize;
	}
	// Size of Target including appended jump to remaining original code.
	size_t CodeSize() const
	{
		return originalSize + 5;
	}
};
