#pragma once

#include <ninja.h>

namespace mipmap
{
	void EnableAutoMipmaps();
	bool IsBlacklistedPVR(const char* name);
	bool IsBlacklistedGBIX(Uint32 gbix);
	bool IsBlacklistedPVM(const char* name);
	void BlacklistGBIX(Uint32 gbix);
	void SkipMipmap(bool value);
	bool AutoMipmapsEnabled();
}