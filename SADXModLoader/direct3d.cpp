#include "stdafx.h"

#include "direct3d.h"

#include "include/d3d8types.h"
#include "FixFOV.h"
#include "UiScale.h"

DataPointer(D3DVIEWPORT8, Direct3D_ViewPort, 0x3D12780);

static bool vsync = false;

// oh god
static const D3DRENDERSTATETYPE D3DRENDERSTATE_TYPES[] = {
	D3DRS_ZENABLE,
	D3DRS_FILLMODE,
	D3DRS_SHADEMODE,
	D3DRS_LINEPATTERN,
	D3DRS_ZWRITEENABLE,
	D3DRS_ALPHATESTENABLE,
	D3DRS_LASTPIXEL,
	D3DRS_SRCBLEND,
	D3DRS_DESTBLEND,
	D3DRS_CULLMODE,
	D3DRS_ZFUNC,
	D3DRS_ALPHAREF,
	D3DRS_ALPHAFUNC,
	D3DRS_DITHERENABLE,
	D3DRS_ALPHABLENDENABLE,
	D3DRS_FOGENABLE,
	D3DRS_SPECULARENABLE,
	D3DRS_ZVISIBLE,
	D3DRS_FOGCOLOR,
	D3DRS_FOGTABLEMODE,
	D3DRS_FOGSTART,
	D3DRS_FOGEND,
	D3DRS_FOGDENSITY,
	D3DRS_EDGEANTIALIAS,
	D3DRS_ZBIAS,
	D3DRS_RANGEFOGENABLE,
	D3DRS_STENCILENABLE,
	D3DRS_STENCILFAIL,
	D3DRS_STENCILZFAIL,
	D3DRS_STENCILPASS,
	D3DRS_STENCILFUNC,
	D3DRS_STENCILREF,
	D3DRS_STENCILMASK,
	D3DRS_STENCILWRITEMASK,
	D3DRS_TEXTUREFACTOR,
	D3DRS_WRAP0,
	D3DRS_WRAP1,
	D3DRS_WRAP2,
	D3DRS_WRAP3,
	D3DRS_WRAP4,
	D3DRS_WRAP5,
	D3DRS_WRAP6,
	D3DRS_WRAP7,
	D3DRS_CLIPPING,
	D3DRS_LIGHTING,
	D3DRS_AMBIENT,
	D3DRS_FOGVERTEXMODE,
	D3DRS_COLORVERTEX,
	D3DRS_LOCALVIEWER,
	D3DRS_NORMALIZENORMALS,
	D3DRS_DIFFUSEMATERIALSOURCE,
	D3DRS_SPECULARMATERIALSOURCE,
	D3DRS_AMBIENTMATERIALSOURCE,
	D3DRS_EMISSIVEMATERIALSOURCE,
	D3DRS_VERTEXBLEND,
	D3DRS_CLIPPLANEENABLE,
	D3DRS_SOFTWAREVERTEXPROCESSING,
	D3DRS_POINTSIZE,
	D3DRS_POINTSIZE_MIN,
	D3DRS_POINTSPRITEENABLE,
	D3DRS_POINTSCALEENABLE,
	D3DRS_POINTSCALE_A,
	D3DRS_POINTSCALE_B,
	D3DRS_POINTSCALE_C,
	D3DRS_MULTISAMPLEANTIALIAS,
	D3DRS_MULTISAMPLEMASK,
	D3DRS_PATCHEDGESTYLE,
	D3DRS_PATCHSEGMENTS,
	D3DRS_DEBUGMONITORTOKEN,
	D3DRS_POINTSIZE_MAX,
	D3DRS_INDEXEDVERTEXBLENDENABLE,
	D3DRS_COLORWRITEENABLE,
	D3DRS_TWEENFACTOR,
	D3DRS_BLENDOP,
	D3DRS_POSITIONORDER,
	D3DRS_NORMALORDER
};

static HRESULT reset_parameters()
{
	// Grab the current FOV before making any changes.
	auto fov = fov::get_fov();

	const size_t renderstates_length = LengthOfArray(D3DRENDERSTATE_TYPES);

	std::vector<DWORD> renderstate_values(renderstates_length);
	std::vector<DWORD> renderstate_results(renderstates_length);

	// Store all the current render states we can grab.
	for (size_t i = 0; i < renderstates_length; i++)
	{
		renderstate_results[i] = Direct3D_Device->GetRenderState(D3DRENDERSTATE_TYPES[i], &renderstate_values[i]);
	}

	HRESULT result = Direct3D_Device->Reset(&PresentParameters);

	if (FAILED(result))
	{
		return result;
	}

	// Restore all the render states we stored.
	for (size_t i = 0; i < renderstates_length; i++)
	{
		if (SUCCEEDED(renderstate_results[i]))
		{
			Direct3D_Device->SetRenderState(D3DRENDERSTATE_TYPES[i], renderstate_values[i]);
		}
	}

	Direct3D_Device->GetViewport(&Direct3D_ViewPort);

	ViewPortWidth        = static_cast<float>(Direct3D_ViewPort.Width);
	HorizontalResolution = Direct3D_ViewPort.Width;
	ViewPortWidth_Half   = ViewPortWidth / 2.0f;
	ViewPortHeight       = static_cast<float>(Direct3D_ViewPort.Height);
	VerticalResolution   = Direct3D_ViewPort.Height;
	ViewPortHeight_Half  = ViewPortHeight / 2.0f;
	HorizontalStretch    = static_cast<float>(HorizontalResolution) / 640.0f;
	VerticalStretch      = static_cast<float>(VerticalResolution) / 480.0f;

	fov::check_aspect_ratio();
	uiscale::update_parameters();

	NJS_POINT2* points = GlobalPoint2Col.p;

	points[0].x = 0.0f;
	points[0].y = 0.0f;
	points[1].x = ViewPortWidth;
	points[1].y = 0.0f;
	points[2].x = ViewPortWidth;
	points[2].y = ViewPortHeight;
	points[3].x = 0.0f;
	points[3].y = ViewPortHeight;

	screen_setup.w  = HorizontalStretch * 640.0f;
	screen_setup.h  = VerticalStretch * 480.0f;
	screen_setup.cx = screen_setup.w / 2.0f;
	screen_setup.cy = screen_setup.h / 2.0f;
	SetupScreen(&screen_setup);

	// Restores previously set FOV in case the game doesn't on its own.
	njSetPerspective(fov);

	if (Camera_Data1 != nullptr)
	{
		Camera_Display_(Camera_Data1);
	}

	TransformAndViewportInvalid = 1;
	Direct3D_SetViewportAndTransform();

	RaiseEvents(modRenderDeviceReset);

	return result;
}

inline void setup_vsync()
{
	auto& p = PresentParameters;

	if (vsync)
	{
		p.SwapEffect = D3DSWAPEFFECT_COPY_VSYNC;
		p.FullScreen_PresentationInterval = IsWindowed ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_ONE;
	}
	else
	{
		p.SwapEffect = D3DSWAPEFFECT_DISCARD;
		p.FullScreen_PresentationInterval = IsWindowed ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;
	}
}

void __fastcall CreateDirect3DDevice_r(void*, int behavior, D3DDEVTYPE type)
{
	if (Direct3D_Device != nullptr)
	{
		return;
	}

	setup_vsync();

	auto result = Direct3D_Object->CreateDevice(DisplayAdapter, type, PresentParameters.hDeviceWindow,
	                                            behavior, &PresentParameters, &Direct3D_Device);

	if (FAILED(result))
	{
		Direct3D_Device = nullptr;
	}
}

void __cdecl Direct3D_Present_r()
{
	if (Direct3D_Device->Present(nullptr, nullptr, nullptr, nullptr) == D3DERR_DEVICELOST)
	{
		direct3d::reset_device();
	}
}

void direct3d::init()
{
	WriteJump(Direct3D_Present, Direct3D_Present_r);
	WriteJump(reinterpret_cast<void*>(0x00794000), CreateDirect3DDevice_r);
}

void direct3d::set_vsync(bool value)
{
	vsync = value;
}

void direct3d::reset_device()
{
	const auto retry_count = 5;

	DWORD  reset = D3D_OK;
	Uint32 tries = 0;

	setup_vsync();

	auto level = D3DERR_DEVICENOTRESET;
	RaiseEvents(modRenderDeviceLost);

	while (tries < retry_count)
	{
		if (level == D3DERR_DEVICENOTRESET)
		{
			if (SUCCEEDED(reset = reset_parameters()))
			{
				return;
			}

			if (++tries >= retry_count)
			{
				const wchar_t* error_str;

				switch (reset)
				{
					default:
					case static_cast<DWORD>(D3DERR_INVALIDCALL):
						error_str = L"D3DERR_INVALIDCALL";
						break;
					case static_cast<DWORD>(D3DERR_OUTOFVIDEOMEMORY):
						error_str = L"D3DERR_OUTOFVIDEOMEMORY";
						break;
					case static_cast<DWORD>(E_OUTOFMEMORY):
						error_str = L"E_OUTOFMEMORY";
						break;
					case static_cast<DWORD>(D3DERR_DEVICELOST):
						error_str = L"D3DERR_DEVICELOST";
						break;
					case static_cast<DWORD>(D3DERR_DRIVERINTERNALERROR):
						error_str = L"D3DERR_DRIVERINTERNALERROR";
						break;
				}

				wchar_t wbuf[256];
				swprintf(wbuf, LengthOfArray(wbuf),
				         L"The following error occurred while trying to reset DirectX:\n\n%s\n\n"
				         L"Press Cancel to exit, or press Retry to try again.", error_str);

				DWORD mb_result = MessageBox(WindowHandle, wbuf, L"Direct3D Reset failed", MB_RETRYCANCEL | MB_ICONERROR);

				if (mb_result == IDRETRY)
				{
					tries = 0;
					continue;
				}
			}
		}

		Sleep(1000);
		level = Direct3D_Device->TestCooperativeLevel();
	}

	exit(reset);
}

void direct3d::change_resolution(uint32_t width, uint32_t height)
{
	change_resolution(width, height, PresentParameters.Windowed);
}

void direct3d::change_resolution(uint32_t width, uint32_t height, bool windowed)
{
	if (width == PresentParameters.BackBufferWidth
	    && height == PresentParameters.BackBufferHeight
	    && windowed == !!PresentParameters.Windowed)
	{
		return;
	}

#ifdef _DEBUG
	const auto old_width    = PresentParameters.BackBufferWidth;
	const auto old_height   = PresentParameters.BackBufferHeight;
	const bool was_windowed = PresentParameters.Windowed;

	PrintDebug("Changing resolution from %ux%u (%s) to %ux%u (%s)\n",
	           old_width, old_height, was_windowed ? "windowed" : "fullscreen",
	           width, height, windowed ? "windowed" : "fullscreen");
#endif

	PresentParameters.BackBufferWidth  = width;
	PresentParameters.BackBufferHeight = height;
	PresentParameters.Windowed         = windowed;

	reset_device();
}

bool direct3d::is_windowed()
{
	return !!PresentParameters.Windowed;
}
