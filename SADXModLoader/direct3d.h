#pragma once

#include <cstdint>
#include <d3d8.h>
#include <d3dx8tex.h>
#include "MemAccess.h"
#include "SADXStructs.h"

#if defined(_MSC_VER) && _MSC_VER >= 1900
// MSVC 2015 inlines printf() and scanf() functions.
// As a result, MSVCRT no longer has definitions for them, resulting
// in a linker error because d3dx8.lib depends on sprintf().
// Link in legacy_stdio_definitions.lib to fix this.
// Reference: https://msdn.microsoft.com/en-us/library/Bb531344.aspx
#pragma comment(lib, "legacy_stdio_definitions.lib")
#endif

struct MeshSetBuffer
{
	NJS_MESHSET_SADX* Meshset;
	void* field_4;
	int FVF;
	IDirect3DVertexBuffer8* VertexBuffer;
	int Size;
	IDirect3DIndexBuffer8* IndexBuffer;
	D3DPRIMITIVETYPE PrimitiveType;
	int MinIndex;
	int NumVertecies;
	int StartIndex;
	int PrimitiveCount;
};

#pragma pack(push, 1)
struct __declspec(align(2)) PolyBuff_RenderArgs
{
	Uint32 StartVertex;
	Uint32 PrimitiveCount;
	Uint32 CullMode;
	Uint32 d;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct PolyBuff
{
	IDirect3DVertexBuffer8* pStreamData;
	Uint32 TotalSize;
	Uint32 CurrentSize;
	Uint32 Stride;
	Uint32 FVF;
	PolyBuff_RenderArgs* RenderArgs;
	Uint32 LockCount;
	const char* name;
	int i;
};
#pragma pack(pop)

DataPointer(IDirect3DDevice8*, Direct3D_Device, 0x03D128B0);
DataPointer(IDirect3D8*, Direct3D_Object, 0x03D11F60);

FastcallFunctionPointer(void, PolyBuff_Init, (PolyBuff *polyBuff, int count, int size, Uint32 FVF, const char *name), 0x794540);
FastcallFunctionPointer(void, PolyBuff_Free, (PolyBuff *_this), 0x7945C0);
FastcallFunctionPointer(void, PolyBuff_SetCurrent, (PolyBuff *_this), 0x794600);
FastcallFunctionPointer(BYTE*, PolyBuff_LockTriangleStrip, (PolyBuff *__this, int primitives, int cullmode), 0x794630);
FastcallFunctionPointer(BYTE*, PolyBuff_LockTriangleList, (PolyBuff *__this, unsigned int primitives, int cullmode), 0x7946C0);
FastcallFunctionPointer(int, PolyBuff_Unlock, (PolyBuff *_this), 0x794750);
FastcallFunctionPointer(void, PolyBuff_DrawTriangleStrip, (PolyBuff *_this), 0x794760);
FastcallFunctionPointer(void, PolyBuff_DrawTriangleList, (PolyBuff *_this), 0x7947B0);
FastcallFunctionPointer(void, PolyBuff_Discard, (PolyBuff *_this), 0x794800);

// BYTE* __usercall@<eax>(MeshSetBuffer* mesh@<edi>, int count)
static const void* const MeshSetBuffer_CreateVertexBufferPtr = reinterpret_cast<void*>(0x7853D0);

static inline BYTE* MeshSetBuffer_CreateVertexBuffer(MeshSetBuffer* mesh, int count)
{
	BYTE* result;
	__asm
	{
		push  [count]
		mov   edi, [mesh]
		call  MeshSetBuffer_CreateVertexBufferPtr
		add   esp, 4
		mov   result, eax
	}
	return result;
}

namespace direct3d
{
	void init();
	void set_vsync(bool value);
	void reset_device();
	void change_resolution(uint32_t width, uint32_t height);
	void change_resolution(uint32_t width, uint32_t height, bool windowed);
	bool is_windowed();
}
