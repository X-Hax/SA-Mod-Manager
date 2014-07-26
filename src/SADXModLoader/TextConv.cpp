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

/** C++ wrappers. **/

#include <string>
using std::string;
using std::wstring;

/**
 * Convert multibyte text to UTF-16.
 * @param mbs Multibyte text.
 * @param cp Code page.
 * @return UTF-16 text, or empty string on error.
 */
wstring MBStoUTF16(const string &mbs, unsigned int cp)
{
	wchar_t *wcs = MBStoUTF16(mbs.c_str(), cp);
	if (!wcs)
		return wstring();

	wstring wstr(wcs);
	delete[] wcs;
	return wstr;
}

/**
 * Convert UTF-16 text to multibyte.
 * @param wcs UTF-16 text.
 * @param cp Code page.
 * @return Multibyte text, or empty string on error.
 */
string UTF16toMBS(const wstring &wcs, unsigned int cp)
{
	char *mbs = UTF16toMBS(wcs.c_str(), cp);
	if (!mbs)
		return string();

	string mstr(mbs);
	delete[] mbs;
	return mstr;
}

/** Convenience functions. **/

/**
 * Convert Shift-JIS text to UTF-8.
 * @param sjis Shift-JIS text.
 * @return UTF-8 text, or empty string on error.
 */
string SJIStoUTF8(const string &sjis)
{
	char *utf8 = SJIStoUTF8(sjis.c_str());
	if (!utf8)
		return string();

	string ustr(utf8);
	delete[] utf8;
	return ustr;
}
