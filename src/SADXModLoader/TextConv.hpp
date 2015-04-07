/**
 * SADX Mod Loader
 * Text conversion functions.
 */

#ifndef TEXTCONV_HPP
#define TEXTCONV_HPP

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Convert multibyte text to UTF-16.
 * @param mbs Multibyte text, null-terminated.
 * @param cp Code page.
 * @return UTF-16 text (allocated via new[]), or nullptr on error.
 */
wchar_t *MBStoUTF16(const char *mbs, unsigned int cp);

/**
 * Convert UTF-16 text to multibyte.
 * @param wcs UTF-16 text, null-terminated.
 * @param cp Code page.
 * @return Multibyte text (allocated via new[]), or nullptr on error.
 */
char *UTF16toMBS(const wchar_t *wcs, unsigned int cp);

/** Convenience functions. **/

/**
 * Convert Shift-JIS text to UTF-8.
 * @param sjis Shift-JIS text, null-terminated.
 * @return UTF-8 text (allocated via new[]), or nullptr on error.
 */
char *SJIStoUTF8(const char *sjis);

/**
 * Convert UTF-8 text to Shift-JIS.
 * @param utf8 UTF-8 text, null-terminated.
 * @return Shift-JIS text (allocated via new[]), or nullptr on error.
 */
char *UTF8toSJIS(const char *utf8);

/**
 * Convert UTF-8 text to Windows-1252.
 * @param utf8 UTF-8 text, null-terminated.
 * @return Windows-1252 text (allocated via new[]), or nullptr on error.
 */
char *UTF8to1252(const char *utf8);

#ifdef __cplusplus
}

// C++ wrappers.
#include <string>

/**
 * Convert multibyte text to UTF-16.
 * @param mbs Multibyte text.
 * @param cp Code page.
 * @return UTF-16 text, or empty string on error.
 */
std::wstring MBStoUTF16(const std::string &mbs, unsigned int cp);

/**
 * Convert UTF-16 text to multibyte.
 * @param wcs UTF-16 text.
 * @param cp Code page.
 * @return Multibyte text, or empty string on error.
 */
std::string UTF16toMBS(const std::wstring &wcs, unsigned int cp);

/** Convenience functions. **/

/**
 * Convert Shift-JIS text to UTF-8.
 * @param sjis Shift-JIS text.
 * @return UTF-8 text, or empty string on error.
 */
std::string SJIStoUTF8(const std::string &sjis);

/**
 * Convert UTF-8 text to Shift-JIS.
 * @param utf8 UTF-8 text.
 * @return Shift-JIS text, or empty string on error.
 */
std::string UTF8toSJIS(const std::string &utf8);

/**
 * Convert UTF-8 text to Windows-1252.
 * @param utf8 UTF-8 text.
 * @return Windows-1252 text, or empty string on error.
 */
std::string UTF8to1252(const std::string &utf8);

#endif /* __cplusplus */

#endif /* TEXTCONV_H */
