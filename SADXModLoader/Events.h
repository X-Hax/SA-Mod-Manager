#pragma once

#include "SADXModLoader.h"
#include <vector>

extern std::vector<ModEvent> modFrameEvents;
extern std::vector<ModEvent> modInputEvents;
extern std::vector<ModEvent> modExitEvents;

/**
* Calls all registered events in the specified event list.
* @param eventList The list of events to trigger.
*/
inline void RaiseEvents(const std::vector<ModEvent>& eventList)
{
	for (auto &i : eventList)
		i();
}

/**
* Registers an event to the specified event list.
* @param eventList The event list to add to.
* @param module The module for the mod DLL.
* @param name The name of the exported function from the module (i.e OnFrame)
*/
void RegisterEvent(std::vector<ModEvent>& eventList, HMODULE module, const char * name);

void __cdecl OnInput();
void __cdecl OnInput_MidJump();
void __cdecl OnExit(UINT uExitCode, int a1, int a2);
