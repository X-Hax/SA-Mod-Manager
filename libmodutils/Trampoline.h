#pragma once

#include <windef.h>

typedef void(*DetourFunction)(void);

// TODO: Better documentation
// TODO: Clearer member names
// TODO: Code restoration

class Trampoline
{
private:
	void* target;
	DetourFunction detour;
	LPVOID codeData;
	size_t originalSize;
	size_t codeSize;

public:
	/// <summary>
	/// Initializes a new <see cref="Trampoline" />, allowing you to replace functions and still call the original code.
	/// </summary>
	/// <param name="start">Start offset (address of function).</param>
	/// <param name="end">End offset.</param>
	/// <param name="func">Your detour function.</param>
	Trampoline(size_t start, size_t end, DetourFunction func);
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
