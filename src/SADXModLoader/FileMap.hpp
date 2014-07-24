/**
 * SADX Mod Loader
 * File remapper.
 */

#ifndef FILEMAP_HPP
#define FILEMAP_HPP

#include <forward_list>
#include <string>
#include <unordered_map>

class FileMap
{
	public:
		FileMap();
		~FileMap();

	private:
		// Disable the copy and assign constructors.
		FileMap(const FileMap &);
		FileMap &operator=(const FileMap &);

	public:
		/**
		 * Normalize a filename for the file replacement map.
		 * @param filename Filename.
		 * @return Normalized filename.
		 */
		static std::string normalizePath(const std::string &filename);

		/**
		 * Normalize a filename for the file replacement map.
		 * @param filename Filename.
		 * @return Normalized filename.
		 */
		static std::string normalizePath(const char *filename);

		/**
		 * Ignore a file.
		 * @param ignoreFile File to ignore.
		 */
		void addIgnoreFile(const std::string &ignoreFile);

		/**
		 * Add a file replacement.
		 * @param origFile Original filename.
		 * @param modFile Mod filename.
		 */
		void addReplaceFile(const std::string &origFile, const std::string &modFile);

		/**
		 * Recursively scan a directory and add all files to the replacement map.
		 * Destination is always relative to system/.
		 * @param srcPath Path to scan.
		 */
		void scanFolder(const std::string &srcPath);

	protected:
		/**
		 * Recursively scan a directory and add all files to the replacement map.
		 * Destination is always relative to system/.
		 * (Internal recursive function)
		 * @param srcPath Path to scan.
		 * @param srcLen Length of original srcPath. (used for recursion)
		 */
		void scanFolder_int(const std::string &srcPath, int srcLen);

		/**
		 * Set a replacement file in the map.
		 * Filenames must already be normalized!
		 * (Internal function; handles memory allocation)
		 * @param origFile Original file.
		 * @param modFile Mod filename.
		 */
		void setReplaceFile(const std::string &origFile, const std::string &modFile);

	public:
		/**
		 * Get a filename from the file replacement map.
		 * @param lpFileName Filename.
		 * @return Replaced filename, or original filename if not replaced by a mod.
		 */
		const char *replaceFile(const char *lpFileName) const;

		/**
		 * Find filenames that matches the specified filename, ignoring the extension.
		 * This is used for BASS vgmstream.
		 * @param lpFileName Filename.
		 * @return List of matching filenames, or nullptr if not fonud.
		 */
		const std::forward_list<std::string>* findFileNoExt(const char *lpFileName) const;

		/**
		 * Clear the file replacement map.
		 */
		void clear(void);

	protected:
		/**
		 * File replacement map.
		 * - Key: Original filename.
		 * - Value: New filename.
		 */
		std::unordered_map<std::string, std::string> m_fileMap;

		/**
		 * File replacement map, sans extensions.
		 * Used for BASS vgmstream.
		 * - Key: Normalized original filename, sans extension.
		 * - Value: forward_list<string>
		 *   - Contains mod filenames.
		 * NOTE: Strings in forward_list are *copied* from m_fileMap.
		 * They will need to be deleted manually.
		 */
		std::unordered_map<std::string, std::forward_list<std::string>* > m_fileNoExtMap;
};

#endif /* FILEMAP_HPP */
