#pragma once
#include "MemAccess.h"

// Class defining a function that can be hooked and unhooked dynamically.
template<typename TRet, typename... TArgs>
class FunctionHook
{
public:
	typedef TRet(*FuncType)(TArgs...);

	FunctionHook(FuncType address) : origaddr(address) {	}

	FunctionHook(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	FunctionHook(FuncType address, FuncType hook) : FunctionHook(address)
	{
		Hook(hook);
	}

	// Initialize the object and immediately apply a hook.
	FunctionHook(intptr_t address, FuncType hook) : FunctionHook(address)
	{
		Hook(hook);
	}

	FunctionHook() = delete;
	FunctionHook(FunctionHook&) = delete;
	FunctionHook(FunctionHook&&) = delete;

	~FunctionHook()
	{
		if (hookaddr)
			Unhook();
	}

	// Apply a hook to the unhooked function.
	void Hook(FuncType hook)
	{
		if (hookaddr)
			throw new std::exception("Cannot apply hook to already hooked function!");
		memcpy(origbytes, origaddr, 5);
		hookaddr = hook;
		WriteJump(origaddr, hookaddr);
	}

	// Remove a hook from the function.
	void Unhook()
	{
		if (!hookaddr)
			throw new std::exception("Attempted to unhook non-hooked function!");
		WriteData(origaddr, origbytes);
		hookaddr = nullptr;
	}

	// Call the original function, bypassing the hook.
	TRet Original(TArgs... args)
	{
		if (hookaddr)
		{
			uint8_t hookbytes[5];
			memcpy(hookbytes, origaddr, 5);
			WriteData(origaddr, origbytes);
			TRet retval = origaddr(args...);
			WriteData(origaddr, hookbytes);
			return retval;
		}
		else
			return origaddr(args...);
	}

	// Get the address of the function this object is attached to.
	FuncType GetFunctionAddress()
	{
		return origaddr;
	}

	// Get the address of the currently applied hook.
	FuncType GetCurrentHook()
	{
		return hookaddr;
	}

private:
	uint8_t origbytes[5];
	const FuncType origaddr;
	FuncType hookaddr = nullptr;
};

// Class defining a function that can be hooked and unhooked dynamically.
template<typename... TArgs>
class FunctionHook<void, TArgs...>
{
public:
	typedef void(*FuncType)(TArgs...);

	FunctionHook(FuncType address) : origaddr(address) {	}

	FunctionHook(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	FunctionHook(FuncType address, FuncType hook) : FunctionHook(address)
	{
		Hook(hook);
	}

	// Initialize the object and immediately apply a hook.
	FunctionHook(intptr_t address, FuncType hook) : FunctionHook(address)
	{
		Hook(hook);
	}

	FunctionHook() = delete;
	FunctionHook(FunctionHook&) = delete;
	FunctionHook(FunctionHook&&) = delete;

	~FunctionHook()
	{
		if (hookaddr)
			Unhook();
	}

	// Apply a hook to the unhooked function.
	void Hook(FuncType hook)
	{
		if (hookaddr)
			throw new std::exception("Cannot apply hook to already hooked function!");
		memcpy(origbytes, origaddr, 5);
		hookaddr = hook;
		WriteJump(origaddr, hookaddr);
	}

	// Remove a hook from the function.
	void Unhook()
	{
		if (!hookaddr)
			throw new std::exception("Attempted to unhook non-hooked function!");
		WriteData(origaddr, origbytes);
		hookaddr = nullptr;
	}

	// Call the original function, bypassing the hook.
	void Original(TArgs... args)
	{
		if (hookaddr)
		{
			uint8_t hookbytes[5];
			memcpy(hookbytes, origaddr, 5);
			WriteData(origaddr, origbytes);
			origaddr(args...);
			WriteData(origaddr, hookbytes);
		}
		else
			origaddr(args...);
	}

	// Get the address of the function this object is attached to.
	constexpr FuncType GetFunctionAddress()
	{
		return origaddr;
	}

	// Get the address of the currently applied hook.
	FuncType GetCurrentHook()
	{
		return hookaddr;
	}

private:
	uint8_t origbytes[5];
	const FuncType origaddr;
	FuncType hookaddr = nullptr;
};

// Class defining a function that can be hooked and unhooked dynamically.
template<typename TRet, typename... TArgs>
class StdcallFunctionHook
{
public:
	typedef TRet(__stdcall* FuncType)(TArgs...);

	StdcallFunctionHook(FuncType address) : origaddr(address) {	}

	StdcallFunctionHook(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	StdcallFunctionHook(FuncType address, FuncType hook) : StdcallFunctionHook(address)
	{
		Hook(hook);
	}

	// Initialize the object and immediately apply a hook.
	StdcallFunctionHook(intptr_t address, FuncType hook) : StdcallFunctionHook(address)
	{
		Hook(hook);
	}

	StdcallFunctionHook() = delete;
	StdcallFunctionHook(StdcallFunctionHook&) = delete;
	StdcallFunctionHook(StdcallFunctionHook&&) = delete;

	~StdcallFunctionHook()
	{
		if (hookaddr)
			Unhook();
	}

	// Apply a hook to the unhooked function.
	void Hook(FuncType hook)
	{
		if (hookaddr)
			throw new std::exception("Cannot apply hook to already hooked function!");
		memcpy(origbytes, origaddr, 5);
		hookaddr = hook;
		WriteJump(origaddr, hookaddr);
	}

	// Remove a hook from the function.
	void Unhook()
	{
		if (!hookaddr)
			throw new std::exception("Attempted to unhook non-hooked function!");
		WriteData(origaddr, origbytes);
		hookaddr = nullptr;
	}

	// Call the original function, bypassing the hook.
	TRet Original(TArgs... args)
	{
		if (hookaddr)
		{
			uint8_t hookbytes[5];
			memcpy(hookbytes, origaddr, 5);
			WriteData(origaddr, origbytes);
			TRet retval = origaddr(args...);
			WriteData(origaddr, hookbytes);
			return retval;
		}
		else
			return origaddr(args...);
	}

	// Get the address of the function this object is attached to.
	constexpr FuncType GetFunctionAddress()
	{
		return origaddr;
	}

	// Get the address of the currently applied hook.
	FuncType GetCurrentHook()
	{
		return hookaddr;
	}

private:
	uint8_t origbytes[5];
	const FuncType origaddr;
	FuncType hookaddr = nullptr;
};

// Class defining a function that can be hooked and unhooked dynamically.
template<typename... TArgs>
class StdcallFunctionHook<void, TArgs...>
{
public:
	typedef void(__stdcall* FuncType)(TArgs...);

	StdcallFunctionHook(FuncType address) : origaddr(address) {	}

	StdcallFunctionHook(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	StdcallFunctionHook(FuncType address, FuncType hook) : StdcallFunctionHook(address)
	{
		Hook(hook);
	}

	// Initialize the object and immediately apply a hook.
	StdcallFunctionHook(intptr_t address, FuncType hook) : StdcallFunctionHook(address)
	{
		Hook(hook);
	}

	StdcallFunctionHook() = delete;
	StdcallFunctionHook(StdcallFunctionHook&) = delete;
	StdcallFunctionHook(StdcallFunctionHook&&) = delete;

	~StdcallFunctionHook()
	{
		if (hookaddr)
			Unhook();
	}

	// Apply a hook to the unhooked function.
	void Hook(FuncType hook)
	{
		if (hookaddr)
			throw new std::exception("Cannot apply hook to already hooked function!");
		memcpy(origbytes, origaddr, 5);
		hookaddr = hook;
		WriteJump(origaddr, hookaddr);
	}

	// Remove a hook from the function.
	void Unhook()
	{
		if (!hookaddr)
			throw new std::exception("Attempted to unhook non-hooked function!");
		WriteData(origaddr, origbytes);
		hookaddr = nullptr;
	}

	// Call the original function, bypassing the hook.
	void Original(TArgs... args)
	{
		if (hookaddr)
		{
			uint8_t hookbytes[5];
			memcpy(hookbytes, origaddr, 5);
			WriteData(origaddr, origbytes);
			origaddr(args...);
			WriteData(origaddr, hookbytes);
		}
		else
			origaddr(args...);
	}

	// Get the address of the function this object is attached to.
	constexpr FuncType GetFunctionAddress()
	{
		return origaddr;
	}

	// Get the address of the currently applied hook.
	FuncType GetCurrentHook()
	{
		return hookaddr;
	}

private:
	uint8_t origbytes[5];
	const FuncType origaddr;
	FuncType hookaddr = nullptr;
};

// Class defining a function that can be hooked and unhooked dynamically.
template<typename TRet, typename... TArgs>
class FastcallFunctionHook
{
public:
	typedef TRet(__fastcall* FuncType)(TArgs...);

	FastcallFunctionHook(FuncType address) : origaddr(address) {	}

	FastcallFunctionHook(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	FastcallFunctionHook(FuncType address, FuncType hook) : FastcallFunctionHook(address)
	{
		Hook(hook);
	}

	// Initialize the object and immediately apply a hook.
	FastcallFunctionHook(intptr_t address, FuncType hook) : FastcallFunctionHook(address)
	{
		Hook(hook);
	}

	FastcallFunctionHook() = delete;
	FastcallFunctionHook(FastcallFunctionHook&) = delete;
	FastcallFunctionHook(FastcallFunctionHook&&) = delete;

	~FastcallFunctionHook()
	{
		if (hookaddr)
			Unhook();
	}

	// Apply a hook to the unhooked function.
	void Hook(FuncType hook)
	{
		if (hookaddr)
			throw new std::exception("Cannot apply hook to already hooked function!");
		memcpy(origbytes, origaddr, 5);
		hookaddr = hook;
		WriteJump(origaddr, hookaddr);
	}

	// Remove a hook from the function.
	void Unhook()
	{
		if (!hookaddr)
			throw new std::exception("Attempted to unhook non-hooked function!");
		WriteData(origaddr, origbytes);
		hookaddr = nullptr;
	}

	// Call the original function, bypassing the hook.
	TRet Original(TArgs... args)
	{
		if (hookaddr)
		{
			uint8_t hookbytes[5];
			memcpy(hookbytes, origaddr, 5);
			WriteData(origaddr, origbytes);
			TRet retval = origaddr(args...);
			WriteData(origaddr, hookbytes);
			return retval;
		}
		else
			return origaddr(args...);
	}

	// Get the address of the function this object is attached to.
	constexpr FuncType GetFunctionAddress()
	{
		return origaddr;
	}

	// Get the address of the currently applied hook.
	FuncType GetCurrentHook()
	{
		return hookaddr;
	}

private:
	uint8_t origbytes[5];
	const FuncType origaddr;
	FuncType hookaddr = nullptr;
};

// Class defining a function that can be hooked and unhooked dynamically.
template<typename... TArgs>
class FastcallFunctionHook<void, TArgs...>
{
public:
	typedef void(__fastcall* FuncType)(TArgs...);

	FastcallFunctionHook(FuncType address) : origaddr(address) {	}

	FastcallFunctionHook(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	FastcallFunctionHook(FuncType address, FuncType hook) : FastcallFunctionHook(address)
	{
		Hook(hook);
	}

	// Initialize the object and immediately apply a hook.
	FastcallFunctionHook(intptr_t address, FuncType hook) : FastcallFunctionHook(address)
	{
		Hook(hook);
	}

	FastcallFunctionHook() = delete;
	FastcallFunctionHook(FastcallFunctionHook&) = delete;
	FastcallFunctionHook(FastcallFunctionHook&&) = delete;

	~FastcallFunctionHook()
	{
		if (hookaddr)
			Unhook();
	}

	// Apply a hook to the unhooked function.
	void Hook(FuncType hook)
	{
		if (hookaddr)
			throw new std::exception("Cannot apply hook to already hooked function!");
		memcpy(origbytes, origaddr, 5);
		hookaddr = hook;
		WriteJump(origaddr, hookaddr);
	}

	// Remove a hook from the function.
	void Unhook()
	{
		if (!hookaddr)
			throw new std::exception("Attempted to unhook non-hooked function!");
		WriteData(origaddr, origbytes);
		hookaddr = nullptr;
	}

	// Call the original function, bypassing the hook.
	void Original(TArgs... args)
	{
		if (hookaddr)
		{
			uint8_t hookbytes[5];
			memcpy(hookbytes, origaddr, 5);
			WriteData(origaddr, origbytes);
			origaddr(args...);
			WriteData(origaddr, hookbytes);
		}
		else
			origaddr(args...);
	}

	// Get the address of the function this object is attached to.
	constexpr FuncType GetFunctionAddress()
	{
		return origaddr;
	}

	// Get the address of the currently applied hook.
	FuncType GetCurrentHook()
	{
		return hookaddr;
	}

private:
	uint8_t origbytes[5];
	const FuncType origaddr;
	FuncType hookaddr = nullptr;
};

// Class defining a function that can be hooked and unhooked dynamically.
template<typename TRet, typename... TArgs>
class ThiscallFunctionHook
{
public:
	typedef TRet(__thiscall* FuncType)(TArgs...);

	ThiscallFunctionHook(FuncType address) : origaddr(address) {	}

	ThiscallFunctionHook(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	ThiscallFunctionHook(FuncType address, FuncType hook) : ThiscallFunctionHook(address)
	{
		Hook(hook);
	}

	// Initialize the object and immediately apply a hook.
	ThiscallFunctionHook(intptr_t address, FuncType hook) : ThiscallFunctionHook(address)
	{
		Hook(hook);
	}

	ThiscallFunctionHook() = delete;
	ThiscallFunctionHook(ThiscallFunctionHook&) = delete;
	ThiscallFunctionHook(ThiscallFunctionHook&&) = delete;

	~ThiscallFunctionHook()
	{
		if (hookaddr)
			Unhook();
	}

	// Apply a hook to the unhooked function.
	void Hook(FuncType hook)
	{
		if (hookaddr)
			throw new std::exception("Cannot apply hook to already hooked function!");
		memcpy(origbytes, origaddr, 5);
		hookaddr = hook;
		WriteJump(origaddr, hookaddr);
	}

	// Remove a hook from the function.
	void Unhook()
	{
		if (!hookaddr)
			throw new std::exception("Attempted to unhook non-hooked function!");
		WriteData(origaddr, origbytes);
		hookaddr = nullptr;
	}

	// Call the original function, bypassing the hook.
	TRet Original(TArgs... args)
	{
		if (hookaddr)
		{
			uint8_t hookbytes[5];
			memcpy(hookbytes, origaddr, 5);
			WriteData(origaddr, origbytes);
			TRet retval = origaddr(args...);
			WriteData(origaddr, hookbytes);
			return retval;
		}
		else
			return origaddr(args...);
	}

	// Get the address of the function this object is attached to.
	constexpr FuncType GetFunctionAddress()
	{
		return origaddr;
	}

	// Get the address of the currently applied hook.
	FuncType GetCurrentHook()
	{
		return hookaddr;
	}

private:
	uint8_t origbytes[5];
	const FuncType origaddr;
	FuncType hookaddr = nullptr;
};

// Class defining a function that can be hooked and unhooked dynamically.
template<typename... TArgs>
class ThiscallFunctionHook<void, TArgs...>
{
public:
	typedef void(__thiscall* FuncType)(TArgs...);

	ThiscallFunctionHook(FuncType address) : origaddr(address) {	}

	ThiscallFunctionHook(intptr_t address) : origaddr(reinterpret_cast<FuncType>(address)) {	}

	// Initialize the object and immediately apply a hook.
	ThiscallFunctionHook(FuncType address, FuncType hook) : ThiscallFunctionHook(address)
	{
		Hook(hook);
	}

	// Initialize the object and immediately apply a hook.
	ThiscallFunctionHook(intptr_t address, FuncType hook) : ThiscallFunctionHook(address)
	{
		Hook(hook);
	}

	ThiscallFunctionHook() = delete;
	ThiscallFunctionHook(ThiscallFunctionHook&) = delete;
	ThiscallFunctionHook(ThiscallFunctionHook&&) = delete;

	~ThiscallFunctionHook()
	{
		if (hookaddr)
			Unhook();
	}

	// Apply a hook to the unhooked function.
	void Hook(FuncType hook)
	{
		if (hookaddr)
			throw new std::exception("Cannot apply hook to already hooked function!");
		memcpy(origbytes, origaddr, 5);
		hookaddr = hook;
		WriteJump(origaddr, hookaddr);
	}

	// Remove a hook from the function.
	void Unhook()
	{
		if (!hookaddr)
			throw new std::exception("Attempted to unhook non-hooked function!");
		WriteData(origaddr, origbytes);
		hookaddr = nullptr;
	}

	// Call the original function, bypassing the hook.
	void Original(TArgs... args)
	{
		if (hookaddr)
		{
			uint8_t hookbytes[5];
			memcpy(hookbytes, origaddr, 5);
			WriteData(origaddr, origbytes);
			origaddr(args...);
			WriteData(origaddr, hookbytes);
		}
		else
			origaddr(args...);
	}

	// Get the address of the function this object is attached to.
	constexpr FuncType GetFunctionAddress()
	{
		return origaddr;
	}

	// Get the address of the currently applied hook.
	FuncType GetCurrentHook()
	{
		return hookaddr;
	}

private:
	uint8_t origbytes[5];
	const FuncType origaddr;
	FuncType hookaddr = nullptr;
};
