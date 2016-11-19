#pragma once

#include <ninja.h>

namespace mipmap
{
	void EnableAutoMipmaps();
	bool IsBlacklistedPVM(const char* name);
	bool IsBlacklistedPVR(const char* name);
	bool IsBlacklistedGBIX(Uint32 gbix);
	void BlacklistGBIX(Uint32 gbix);
	void SkipMipmap(bool value);
	bool AutoMipmapsEnabled();
}