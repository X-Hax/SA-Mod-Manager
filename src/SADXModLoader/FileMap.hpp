/**
 * SADX Mod Loader
 * File remapper.
 */

#ifndef FILEMAP_HPP
#define FILEMAP_HPP

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
		 * @param destFile Replacement filename.
		 */
		void addReplaceFile(const std::string &origFile, const std::string &destFile);

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

	public:
		/**
		 * Get a filename from the file replacement map.
		 * @param lpFileName Filename.
		 * @return Replaced filename, or original filename if not replaced by a mod.
		 */
		const char *replaceFile(const char *lpFileName) const;

		/**
		 * Clear the file replacement map.
		 */
		void clear(void);

	protected:
		/**
		 * File replacement map.
		 * - Key: Original filename.
		 * - Value: New filename. (allocated via new[])
		 */
		std::unordered_map<std::string, const char *> m_fileMap;
};

#endif /* FILEMAP_HPP */
