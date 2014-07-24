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
char *UTF16toMBS(const wchar_t *mbs, unsigned int cp);

/** Convenience functions. **/

/**
 * Convert Shift-JIS text to UTF-8.
 * @param sjis Shift-JIS text, null-terminated.
 * @return UTF-8 text (allocated via new[]), or nullptr on error.
 */
char *SJIStoUTF8(const char *sjis);

#ifdef __cplusplus
}
#endif

#endif /* TEXTCONV_H */
