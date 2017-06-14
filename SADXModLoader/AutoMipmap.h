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

	class mip_guard
	{
		bool current = false;
		bool last = false;

	public:
		explicit mip_guard(bool skip);
		~mip_guard();

		bool is_blacklisted() const
		{
			return current;
		}
	};
}