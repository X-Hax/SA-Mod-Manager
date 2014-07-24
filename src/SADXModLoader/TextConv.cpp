/**
 * SADX Mod Loader
 * Text conversion functions.
 */

#include "stdafx.h"
#include "TextConv.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/**
 * Convert multibyte text to UTF-16.
 * @param mbs Multibyte text, null-terminated.
 * @param cp Code page.
 * @return UTF-16 text (allocated via new[]), or nullptr on error.
 */
wchar_t *MBStoUTF16(const char *mbs, unsigned int cp)
{
	int cchWcs = MultiByteToWideChar(cp, 0, mbs, -1, NULL, 0);
	if (cchWcs <= 0)
		return nullptr;
	wchar_t *wcs = new wchar_t[cchWcs];
	MultiByteToWideChar(cp, 0, mbs, -1, wcs, cchWcs);
	return wcs;
}

/**
 * Convert UTF-16 text to multibyte.
 * @param wcs UTF-16 text, null-terminated.
 * @param cp Code page.
 * @return Multibyte text (allocated via new[]), or nullptr on error.
 */
char *UTF16toMBS(const wchar_t *wcs, unsigned int cp)
{
	int cbMbs = WideCharToMultiByte(cp, 0, wcs, -1, NULL, 0, NULL, NULL);
	if (cbMbs <= 0)
		return nullptr;
	char *mbs = new char[cbMbs];
	WideCharToMultiByte(CP_UTF8, 0, wcs, -1, mbs, cbMbs, NULL, NULL);
	return mbs;
}

/** Convenience functions. **/

/**
 * Convert Shift-JIS text to UTF-8.
 * @param sjis Shift-JIS text, null-terminated.
 * @return UTF-8 text (allocated via new[]), or nullptr on error.
 */
char *SJIStoUTF8(const char *sjis)
{
	// Convert from Shift-JIS to UTF-16.
	wchar_t *wcs = MBStoUTF16(sjis, 932);
	if (!wcs)
		return nullptr;

	// Convert from UTF-16 to UTF-8.
	char *mbs = UTF16toMBS(wcs, CP_UTF8);
	delete[] wcs;
	if (!mbs)
		return nullptr;

	return mbs;
}
