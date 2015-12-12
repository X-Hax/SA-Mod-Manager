#include "stdafx.h"
#include "Trampoline.h"
#include "Events.h"

std::vector<ModEvent> modFrameEvents;
std::vector<ModEvent> modInputEvents;
std::vector<ModEvent> modExitEvents;

Trampoline exitDetour((size_t)0x0064672F, 0x00646736, (DetourFunction)OnExit);

/**
* Registers an event to the specified event list.
* @param eventList The event list to add to.
* @param module The module for the mod DLL.
* @param name The name of the exported function from the module (i.e OnFrame)
*/
void RegisterEvent(std::vector<ModEvent>& eventList, HMODULE module, const char* name)
{
	const ModEvent modEvent = (const ModEvent)GetProcAddress(module, name);

	if (modEvent != nullptr)
		eventList.push_back(modEvent);
}

DataPointer(short, word_3B2C464, 0x3B2C464);

void OnInput()
{
	RaiseEvents(modInputEvents);
}

void __declspec(naked) OnInput_MidJump()
{
	__asm
	{
		inc word_3B2C464
		pop esi
		jmp OnInput
	}
}

void __cdecl OnExit(UINT uExitCode, int a1, int a2)
{
	RaiseEvents(modExitEvents);
	FunctionPointer(void, original, (UINT, int, int), exitDetour.Target());
	original(uExitCode, a1, a2);
}
