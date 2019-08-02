#include "stdafx.h"
#include "SADXModLoader.h"
#include "polybuff.h"

/*
 * Despite having designated polybuff drawing functions
 * specifically for meshes with vertex color, vanilla SADX
 * deliberately ignores the mesh-provided vertex color,
 * and instead uses a global vertex color.
 *
 * It uses that global vertex color EVEN AFTER IT CHECKS
 * WHETHER OR NOT IT SHOULD. As in, the code in the "if"
 * and the "else" ARE 100% IDENTICAL IN FUNCTIONALITY.
 *
 * The code below re-implements the functions to handle the
 * secondary case CORRECTLY and use the mesh-provided vcolors.
 */

DataPointer(PolyBuff, stru_3D0FEB4, 0x3D0FEB4);
DataPointer(PolyBuff, stru_3D0FED8, 0x3D0FED8);
DataPointer(PolyBuff, stru_3D0FF20, 0x3D0FF20);

void __cdecl polybuff_vcolor_strip_r(NJS_MESHSET_SADX* meshset, NJS_POINT3* points)
{
	NJS_COLOR v6; // edi

	Sint16* meshes = meshset->meshes;
	int v5 = 0;
	v6.color = PolyBuffVertexColor.color;
	Sint16* v7 = meshset->meshes;

	if (meshset->nbMesh)
	{
		int v8 = meshset->nbMesh;
		do
		{
			--v8;
			int v9 = *v7 & 0x3FFF;
			v5 += v9 + 2;
			v7 += v9 + 1;
		} while (v8);
	}

	PolyBuff_SetCurrent(&stru_3D0FEB4);
	auto buffer = (FVFStruct_F*)PolyBuff_LockTriangleStrip(&stru_3D0FEB4, v5, Direct3D_CurrentCullMode);

	if (_RenderFlags & RenderFlags_OffsetMaterial || !meshset->vertcolor)
	{
		if (meshset->nbMesh)
		{
			NJS_POINT3* v21 = points;
			int v32 = meshset->nbMesh;
			do
			{
				int v22 = meshes[1];
				Sint16 v23 = *meshes;
				++meshes;
				float* v24 = &v21[v22].x;
				NJS_VECTOR* v25 = &buffer->position;
				buffer->position.x = *v24;
				buffer->position.y = v24[1];
				float v26 = v24[2];
				unsigned __int16 v27 = v23 & 0x3FFF;
				buffer->diffuse = v6.color;
				FVFStruct_F* v28 = buffer + 1;
				v25->z = v26;
				v21 = points;
				if (v27)
				{
					int a3b = v27;
					do
					{
						NJS_POINT3* v29 = &points[*meshes];
						v28->position.x = v29->x;
						v28->position.y = v29->y;
						v28->position.z = v29->z;
						v28->diffuse = v6.color;
						++v28;
						++meshes;
						--a3b;
					} while (a3b);
				}
				NJS_POINT3* v30 = &points[*(meshes - 1)];
				v28->position.x = v30->x;
				v28->position.y = v30->y;
				v28->position.z = v30->z;
				v28->diffuse = v6.color;
				buffer = v28 + 1;
				--v32;
			} while (v32);
		}
	}
	else if (meshset->nbMesh && meshset->vertcolor)
	{
		size_t index = 0;
		size_t c = 0;

		for (int i = 0; i < meshset->nbMesh; i++)
		{
			const auto n = static_cast<Sint16>(meshes[index++] & 0x3FFF);
			auto head = buffer++;

			for (int j = 0; j < n; j++)
			{
				const short vert_index = meshes[index++];

				buffer->position = points[vert_index];

				const auto vertcolor = meshset->vertcolor[c].color;
				++c;

				buffer->diffuse = vertcolor;
				++buffer;
			}

			*head = *(head + 1);

			*buffer = *(buffer - 1);
			++buffer;
		}
	}

	PolyBuff_Unlock(&stru_3D0FEB4);
	PolyBuff_DrawTriangleStrip(&stru_3D0FEB4);
}

void __cdecl polybuff_vcolor_tri_r(NJS_MESHSET_SADX* meshset, NJS_POINT3* points)
{
	NJS_COLOR v5; // edi
	FVFStruct_F* v9; // edx
	float v10; // ecx
	FVFStruct_F* v12; // ecx
	float v13; // edx

	Sint16* v4 = meshset->meshes;
	v5.color = PolyBuffVertexColor.color;
	unsigned int v6 = 3 * meshset->nbMesh;

	PolyBuff_SetCurrent(&stru_3D0FEB4);
	auto buffer = (FVFStruct_F*)PolyBuff_LockTriangleList(&stru_3D0FEB4, v6, Direct3D_CurrentCullMode);

	if (buffer)
	{
		if (_RenderFlags & RenderFlags_OffsetMaterial || !meshset->vertcolor)
		{
			for (; v6; v12->position.z = v13)
			{
				NJS_POINT3* v11 = &points[*v4];
				v12 = buffer;
				buffer->position.x = v11->x;
				buffer->position.y = v11->y;
				v13 = v11->z;
				buffer->diffuse = v5.color;
				++buffer;
				++v4;
				--v6;
			}
		}
		else if (meshset->vertcolor)
		{
			size_t i = 0;

			for (; v6; v9->position.z = v10)
			{
				NJS_POINT3* v8 = &points[*v4];
				v9 = buffer;
				buffer->position.x = v8->x;
				buffer->position.y = v8->y;
				v10 = v8->z;
				buffer->diffuse = meshset->vertcolor[i++].color;
				++buffer;
				++v4;
				--v6;
			}
		}
	}
	PolyBuff_Unlock(&stru_3D0FEB4);
	PolyBuff_DrawTriangleList(&stru_3D0FEB4);
}

void __cdecl polybuff_normal_vcolor_strip_r(NJS_MESHSET_SADX* meshset, NJS_POINT3* points, NJS_VECTOR* normals)
{
	NJS_COLOR v6; // edi

	Sint16* meshes = meshset->meshes;
	int v5 = 0;
	v6.color = PolyBuffVertexColor.color;
	char v33 = _RenderFlags;
	Sint16* v7 = meshset->meshes;

	if (meshset->nbMesh)
	{
		int v8 = meshset->nbMesh;
		do
		{
			--v8;
			int v9 = *v7 & 0x3FFF;
			v5 += v9 + 2;
			v7 += v9 + 1;
		} while (v8);
	}

	PolyBuff_SetCurrent(&stru_3D0FED8);
	auto buffer = (FVFStruct_G*)PolyBuff_LockTriangleStrip(&stru_3D0FED8, v5, Direct3D_CurrentCullMode);

	if (v33 & RenderFlags_OffsetMaterial || !meshset->vertcolor)
	{
		if (meshset->nbMesh)
		{
			int v35 = meshset->nbMesh;
			do
			{
				Sint16 v22 = *meshes;
				++meshes;
				int a3c = v22 & 0x3FFF;
				int v23 = *meshes;
				buffer->position.x = points[v23].x;
				buffer->position.y = points[v23].y;
				buffer->position.z = points[v23].z;
				NJS_VECTOR* v24 = &normals[v23];
				NJS_VECTOR* v25 = &buffer->normal;
				v25->x = v24->x;
				v25->y = v24->y;
				v25->z = v24->z;
				buffer->diffuse = v6.color;
				FVFStruct_G* v26 = buffer + 1;
				if ((WORD)a3c)
				{
					int a3d = (unsigned __int16)a3c;
					do
					{
						int v27 = *meshes;
						v26->position.x = points[v27].x;
						v26->position.y = points[v27].y;
						v26->position.z = points[v27].z;
						NJS_VECTOR* v28 = &normals[v27];
						NJS_VECTOR* v29 = &v26->normal;
						v29->x = v28->x;
						v29->y = v28->y;
						v29->z = v28->z;
						v26->diffuse = v6.color;
						++v26;
						++meshes;
						--a3d;
					} while (a3d);
				}
				int v30 = *(meshes - 1);
				v26->position.x = points[v30].x;
				v26->position.y = points[v30].y;
				v26->position.z = points[v30].z;
				NJS_VECTOR* v31 = &normals[v30];
				NJS_VECTOR* v32 = &v26->normal;
				v32->x = v31->x;
				v32->y = v31->y;
				v32->z = v31->z;
				v26->diffuse = v6.color;
				buffer = v26 + 1;
				--v35;
			} while (v35);
		}
	}
	else if (meshset->nbMesh && meshset->vertcolor)
	{
		size_t index = 0;
		size_t c = 0;

		for (int i = 0; i < meshset->nbMesh; i++)
		{
			const auto n = static_cast<Sint16>(meshes[index++] & 0x3FFF);
			auto head = buffer++;

			for (int j = 0; j < n; j++)
			{
				const short vert_index = meshes[index++];

				buffer->position = points[vert_index];
				buffer->normal = normals[vert_index];

				const auto vertcolor = meshset->vertcolor[c].color;
				++c;

				buffer->diffuse = vertcolor;
				++buffer;
			}

			*head = *(head + 1);

			*buffer = *(buffer - 1);
			++buffer;
		}
	}

	PolyBuff_Unlock(&stru_3D0FED8);
	PolyBuff_DrawTriangleStrip(&stru_3D0FED8);
}

void __cdecl polybuff_normal_vcolor_tri_r(NJS_MESHSET_SADX* meshset, NJS_POINT3* points, NJS_VECTOR* normals)
{
	NJS_COLOR v6; // edi

	Sint16* meshes = meshset->meshes;
	int nbMesh = meshset->nbMesh;
	RenderFlags meshseta = _RenderFlags;
	v6.color = PolyBuffVertexColor.color;
	int vertex_count = 3 * nbMesh;

	PolyBuff_SetCurrent(&stru_3D0FED8);
	auto buffer = (FVFStruct_G*)PolyBuff_LockTriangleList(&stru_3D0FED8, vertex_count, Direct3D_CurrentCullMode);

	if (buffer)
	{
		if (meshseta & RenderFlags_OffsetMaterial || !meshset->vertcolor)
		{
			if (vertex_count)
			{
				int meshsetc = vertex_count;
				do
				{
					int v12 = *meshes;
					buffer->position.x = points[v12].x;
					buffer->position.y = points[v12].y;
					buffer->position.z = points[v12].z;
					NJS_VECTOR* v13 = &normals[v12];
					NJS_VECTOR* v14 = &buffer->normal;
					v14->x = v13->x;
					v14->y = v13->y;
					v14->z = v13->z;
					buffer->diffuse = v6.color;
					++buffer;
					++meshes;
					meshsetc = meshsetc - 1;
				} while (meshsetc);
			}
		}
		else if (vertex_count && meshset->vertcolor)
		{
			int meshsetb = vertex_count;
			auto vertcolor = meshset->vertcolor;
			do
			{
				int v9 = *meshes;
				buffer->position.x = points[v9].x;
				buffer->position.y = points[v9].y;
				buffer->position.z = points[v9].z;
				NJS_VECTOR* v10 = &normals[v9];
				NJS_VECTOR* v11 = &buffer->normal;
				v11->x = v10->x;
				v11->y = v10->y;
				v11->z = v10->z;
				buffer->diffuse = vertcolor->color;
				++buffer;
				++vertcolor;
				++meshes;
				meshsetb = meshsetb - 1;
			} while (meshsetb);
		}
	}

	PolyBuff_Unlock(&stru_3D0FED8);
	PolyBuff_DrawTriangleList(&stru_3D0FED8);
}

void __cdecl polybuff_normal_vcolor_uv_strip_r(NJS_MESHSET_SADX* meshset, NJS_POINT3* points, NJS_VECTOR* normals)
{
	NJS_COLOR v3; // ebp
	NJS_TEX* uv; // esi
	Sint16* meshes; // edi
	int primitiveCount; // ebx
	Sint16* _meshes; // ecx
	_BOOL1 done; // zf
	Sint16 _n; // cx
	int v26; // ecx
	NJS_POINT3* v27; // ecx
	NJS_VECTOR* normal; // ecx
	FVFStruct_I* v29; // eax
	int v30; // ecx
	NJS_VECTOR* v31; // ecx
	NJS_VECTOR* v32; // edx
	signed int v33; // ST1C_4
	int v34; // ecx
	NJS_POINT3* v35; // ecx
	NJS_VECTOR* v36; // ecx
	NJS_TEX* v37; // esi
	NJS_COLOR v41; // [esp+14h] [ebp-8h]
	char v42; // [esp+18h] [ebp-4h]
	unsigned __int16 n_masked; // [esp+18h] [ebp-4h]
	int a1f; // [esp+20h] [ebp+4h]
	int a1b; // [esp+20h] [ebp+4h]
	int a1g; // [esp+20h] [ebp+4h]
	int a1h; // [esp+20h] [ebp+4h]

	v3.color = PolyBuffVertexColor.color;
	uv = meshset->vertuv;
	meshes = meshset->meshes;
	primitiveCount = 0;
	v41.color = PolyBuffVertexColor.color;
	v42 = _RenderFlags;
	_meshes = meshset->meshes;

	if (meshset->nbMesh)
	{
		int i = meshset->nbMesh;
		do
		{
			--i;
			int n = *_meshes & 0x3FFF;
			primitiveCount += n + 2;
			_meshes += n + 1;
		} while (i);
	}

	PolyBuff_SetCurrent(&stru_3D0FF20);
	auto buffer = (FVFStruct_I*)PolyBuff_LockTriangleStrip(&stru_3D0FF20, primitiveCount, Direct3D_CurrentCullMode);

	if (v42 & RenderFlags_OffsetMaterial || !meshset->vertcolor)
	{
		if (meshset->nbMesh)
		{
			int j = meshset->nbMesh;
			do
			{
				_n = *meshes;
				++meshes;
				n_masked = _n & 0x3FFF;
				v26 = *meshes;
				a1f = v26 * 12;
				v27 = &points[v26];
				buffer->position.x = v27->x;
				buffer->position.y = v27->y;
				buffer->position.z = v27->z;
				normal = &buffer->normal;
				normal->x = *(float*)((char*)&normals->x + a1f);
				normal->y = *(float*)((char*)&normals->y + a1f);
				normal->z = *(float*)((char*)&normals->z + a1f);
				buffer->diffuse = v3.color;
				v29 = buffer + 1;
				v29[-1].u = (float)uv->u * 0.0039215689f;
				v29[-1].v = (float)uv->v * 0.0039215689f;
				if (n_masked)
				{
					a1b = n_masked;
					do
					{
						v30 = *meshes;
						v29->position.x = points[v30].x;
						v29->position.y = points[v30].y;
						v3.color = v41.color;
						v29->position.z = points[v30].z;
						v31 = &normals[v30];
						v32 = &v29->normal;
						v32->x = v31->x;
						v32->y = v31->y;
						v32->z = v31->z;
						v29->diffuse = v41.color;
						v33 = uv->u;
						++v29;
						++meshes;
						++uv;
						v29[-1].u = (float)v33 * 0.0039215689f;
						done = a1b-- == 1;
						v29[-1].v = (float)uv[-1].v * 0.0039215689f;
					} while (!done);
				}
				v34 = *(meshes - 1);
				a1g = v34 * 12;
				v35 = &points[v34];
				v29->position.x = v35->x;
				v29->position.y = v35->y;
				v29->position.z = v35->z;
				v36 = &v29->normal;
				v36->x = *(float*)((char*)&normals->x + a1g);
				v36->y = *(float*)((char*)&normals->y + a1g);
				v37 = uv - 1;
				v36->z = *(float*)((char*)&normals->z + a1g);
				v29->diffuse = v3.color;
				a1h = v37->u;
				buffer = v29 + 1;
				uv = v37 + 1;
				done = j-- == 1;
				buffer[-1].u = (float)a1h * 0.0039215689f;
				buffer[-1].v = (float)uv[-1].v * 0.0039215689f;
			} while (!done);
		}
	}
	else if (meshset->nbMesh && meshset->vertcolor)
	{
		size_t index = 0;
		size_t c = 0;

		for (int i = 0; i < meshset->nbMesh; i++)
		{
			const auto n = static_cast<Sint16>(meshes[index++] & 0x3FFF);
			auto head = buffer++;

			for (int j = 0; j < n; j++)
			{
				const short vert_index = meshes[index++];

				buffer->position = points[vert_index];
				buffer->normal = normals[vert_index];

				const auto vertuv = meshset->vertuv[c];
				const auto vertcolor = meshset->vertcolor[c].color;
				++c;

				buffer->diffuse = vertcolor;
				buffer->u = static_cast<float>(vertuv.u) / 255.0f;
				buffer->v = static_cast<float>(vertuv.v) / 255.0f;
				++buffer;
			}

			*head = *(head + 1);

			*buffer = *(buffer - 1);
			++buffer;
		}
	}

	PolyBuff_Unlock(&stru_3D0FF20);
	PolyBuff_DrawTriangleStrip(&stru_3D0FF20);
}

void __cdecl polybuff_normal_vcolor_uv_tri_r(NJS_MESHSET* a1, NJS_POINT3* points, NJS_VECTOR* normals)
{
	NJS_COLOR v18; // [esp+10h] [ebp-8h]

	int v3 = Direct3D_CurrentCullMode;
	v18.color = PolyBuffVertexColor.color;
	NJS_TEX* uv = a1->vertuv;
	Sint16* meshes = a1->meshes;
	const int nbMesh = a1->nbMesh;
	int a1a = _RenderFlags;
	int v7 = 3 * nbMesh;

	PolyBuff_SetCurrent(&stru_3D0FF20);
	auto buffer = (FVFStruct_I*)PolyBuff_LockTriangleList(&stru_3D0FF20, v7, v3);

	if (buffer)
	{
		if (a1a & RenderFlags_OffsetMaterial && a1->vertcolor)
		{
			if (v7)
			{
				int a1c = v7;
				do
				{
					int v14 = *meshes;
					buffer->position.x = points[v14].x;
					buffer->position.y = points[v14].y;
					buffer->position.z = points[v14].z;
					NJS_POINT3* v15 = &normals[v14];
					NJS_VECTOR* v16 = &buffer->normal;
					v16->x = v15->x;
					v16->y = v15->y;
					v16->z = v15->z;
					buffer->diffuse = v18.color;
					signed int v17 = uv->u;
					++buffer;
					++meshes;
					++uv;
					buffer[-1].u = (float)v17 * 0.0039215689f;
					buffer[-1].v = (float)uv[-1].v * 0.0039215689f;
				} while (a1c-- != 1);
			}
		}
		else if (v7)
		{
			auto vertcolor = a1->vertcolor;
			int a1b = v7;
			do
			{
				int v9 = *meshes;
				buffer->position.x = points[v9].x;
				buffer->position.y = points[v9].y;
				buffer->position.z = points[v9].z;
				NJS_POINT3* v10 = &normals[v9];
				NJS_VECTOR* v11 = &buffer->normal;
				v11->x = v10->x;
				v11->y = v10->y;
				v11->z = v10->z;
				buffer->diffuse = vertcolor->color;
				const int v12 = uv->u;
				++buffer;
				++meshes;
				++vertcolor;
				++uv;
				buffer[-1].u = (float)v12 * 0.0039215689f;
				buffer[-1].v = (float)uv[-1].v * 0.0039215689f;
			} while (a1b-- != 1);
		}
	}

	PolyBuff_Unlock(&stru_3D0FF20);
	PolyBuff_DrawTriangleList(&stru_3D0FF20);
}

void polybuff::rewrite_init()
{
	WriteJump(polybuff_vcolor_strip, polybuff_vcolor_strip_r);
	WriteJump(polybuff_vcolor_tri, polybuff_vcolor_tri_r);
	WriteJump(polybuff_normal_vcolor_strip, polybuff_normal_vcolor_strip_r);
	WriteJump(polybuff_normal_vcolor_tri, polybuff_normal_vcolor_tri_r);
	WriteJump(polybuff_normal_vcolor_uv_strip, polybuff_normal_vcolor_uv_strip_r);
	WriteJump(polybuff_normal_vcolor_uv_tri, polybuff_normal_vcolor_uv_tri_r);
}
