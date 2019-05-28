#include "stdafx.h"
#include "SADXModLoader.h"
#include "direct3d.h"
#include "ChunkSpecularFix.h"

void sub_78E3D0(unsigned int ambient, unsigned int diffuse, unsigned int specular, float exponent)
{
	D3DMATERIAL8 v3; // [esp+8h] [ebp-44h]
	NJS_COLOR ambientColor;
	ambientColor.color = ambient;
	NJS_COLOR diffuseColor;
	diffuseColor.color = diffuse;
	NJS_COLOR specularColor;
	specularColor.color = specular;

	//v3.Diffuse.r = (double)(*(unsigned int *)&diffuse >> 24);
	v3.Diffuse.a = (double)diffuseColor.argb.a;
	v3.Diffuse.r = (double)diffuseColor.argb.r;
	v3.Diffuse.g = (double)diffuseColor.argb.g;
	v3.Diffuse.b = (double)diffuseColor.argb.b;

	v3.Ambient.a = (double)ambientColor.argb.a;
	v3.Ambient.r = (double)ambientColor.argb.r;
	v3.Ambient.g = (double)ambientColor.argb.g;
	v3.Ambient.b = (double)ambientColor.argb.b;

	v3.Specular.a = (double)specularColor.argb.a;
	v3.Specular.r = (double)specularColor.argb.r;
	v3.Specular.g = (double)specularColor.argb.g;
	v3.Specular.b = (double)specularColor.argb.b;

	// divide by 255
	v3.Ambient.a = v3.Ambient.a * 0.0039215689;
	v3.Ambient.r = v3.Ambient.r * 0.0039215689;
	v3.Ambient.g = v3.Ambient.g * 0.0039215689;
	v3.Ambient.b = v3.Ambient.b * 0.0039215689;

	v3.Diffuse.a = v3.Diffuse.a * 0.0039215689;
	v3.Diffuse.r = v3.Diffuse.r * 0.0039215689;
	v3.Diffuse.g = v3.Diffuse.g * 0.0039215689;
	v3.Diffuse.b = v3.Diffuse.b * 0.0039215689;

	v3.Specular.a = v3.Specular.a * 0.0039215689;
	v3.Specular.r = v3.Specular.r * 0.0039215689;
	v3.Specular.g = v3.Specular.g * 0.0039215689;
	v3.Specular.b = v3.Specular.b * 0.0039215689;

	v3.Power = exponent;

	v3.Emissive.a = 0.0;
	v3.Emissive.r = 0.0;
	v3.Emissive.g = 0.0;
	v3.Emissive.b = 0.0;
	if (_nj_control_3d_flag_ & NJD_CONTROL_3D_CONSTANT_MATERIAL)
	{
		v3.Diffuse.a = _nj_constant_material_.a;
		v3.Diffuse.r = _nj_constant_material_.r;
		v3.Diffuse.g = _nj_constant_material_.g;
		v3.Diffuse.b = _nj_constant_material_.b;
	}
	Direct3D_Device->SetMaterial(&v3);
	*(int*)0x389D8F4 = (int)diffuseColor.color;
}

void __cdecl sub_7913D0(int* a1) //Diffuse
{
	sub_78E3D0(0xFFFFFFFF, *a1, 0, 0.0f);
}

void __cdecl sub_7913F0(unsigned int* a1) //Ambient
{
	sub_78E3D0(*a1, 0xFFFFFFFF, 0, 0.0f);
}

void __cdecl sub_791410(int* a1) //DiffuseAmbient
{
	sub_78E3D0(a1[1], *a1, 0, 0);
}

void __cdecl sub_791430(int* a1) //Specular
{
	uint16_t* shrt = (uint16_t*)a1;
	sub_78E3D0(0xFFFFFFFF, 0xFFFFFFFF, *a1, shrt[1] >> 8);
}

void __cdecl sub_791450(int* a1) //DiffuseSpecular
{
	uint16_t* shrt = (uint16_t*)& a1[1];
	sub_78E3D0(0xFFFFFFFF, *a1, a1[1], shrt[1] >> 8);
}

void __cdecl sub_791470(int* a1) //AmbientSpecular
{
	uint16_t* shrt = (uint16_t*)& a1[1];
	sub_78E3D0(*a1, 0xFFFFFFFF, a1[1], shrt[1] >> 8);
}

void __cdecl sub_791490(int* a1) //DiffuseAmbientSpecular
{
	uint16_t* shrt = (uint16_t*)& a1[2];
	sub_78E3D0(a1[1], *a1, a1[2], shrt[1] >> 8);
}

void ChunkSpecularFix_Init()
{
	WriteJump((void*)0x7913D0, sub_7913D0);
	WriteJump((void*)0x7913F0, sub_7913F0);
	WriteJump((void*)0x791410, sub_791410);
	WriteJump((void*)0x791430, sub_791430);
	WriteJump((void*)0x791450, sub_791450);
	WriteJump((void*)0x791470, sub_791470);
	WriteJump((void*)0x791490, sub_791490);
}