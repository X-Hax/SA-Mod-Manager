#pragma once

#include <ninja.h>

namespace mipmap
{
	void enable_auto_mipmaps();
	bool is_blacklisted_pvm(const char* name);
	bool is_blacklisted_pvr(const char* name);
	bool is_blacklisted_gbix(Uint32 gbix);
	void blacklist_gbix(Uint32 gbix);
	void skip_mipmap(bool value);
	bool auto_mipmaps_enabled();

	class MipGuard
	{
		bool current = false;
		bool last = false;

	public:
		explicit MipGuard(bool skip);
		~MipGuard();

		bool is_blacklisted() const
		{
			return current;
		}
	};
}