#pragma once

#include <wtypes.h>

// TODO: Better documentation
// TODO: Clearer member names
// TODO: Code restoration

class Trampoline
{
private:
	void* target;
	void* detour;
	LPVOID codeData;
	size_t originalSize;
	size_t codeSize;
	const bool revert;

public:
	/// <summary>
	/// Initializes a new <see cref="Trampoline" />, allowing you to replace functions and still call the original code.
	/// </summary>
	/// <param name="start">Start offset (address of function).</param>
	/// <param name="end">End offset.</param>
	/// <param name="func">Your detour function.</param>
	/// <param name="destructRevert">If <c>true</c>, code changes will be reverted when this instance is destroyed.</param>
	/// <remarks>If there's a relative jump or call within the range of <see cref="start" /> and <see cref="end" />,
	/// they need to be replaced with absolute offsets before instantiating a <see cref="Trampoline" />.</remarks>
	Trampoline(size_t start, size_t end, void* func, bool destructRevert = true);
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
		return codeSize;
	}
};
