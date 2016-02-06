/**
 * SADX Mod Loader
 * INI file parser.
 */

#ifndef INIFILE_H
#define INIFILE_H

#include <cstdio>
#include <string>
#include <unordered_map>

class IniFile;

/**
 * Individual INI group.
 */
class IniGroup
{
	public:
		bool hasKey(const std::string &key) const;
		bool hasKeyNonEmpty(const std::string &key) const;

		const std::unordered_map<std::string, std::string> *data(void) const;

		std::string getString(const std::string &key, const std::string &def = "") const;
		std::wstring getWString(const std::string &key, const std::wstring &def = L"") const;
		bool getBool(const std::string &key, bool def = false) const;
		int getIntRadix(const std::string &key, int radix, int def = 0) const;
		int getInt(const std::string &key, int def = 0) const;
		float getFloat(const std::string &key, float def = 0) const;

		void setString(const std::string &key, const std::string &val);
		void setWString(const std::string &key, const std::wstring &val);
		void setBool(const std::string &key, bool val);
		void setIntRadix(const std::string &key, int radix, int val);
		void setInt(const std::string &key, int val);
		void setFloat(const std::string &key, float val);

		bool removeKey(const std::string &key);

		std::unordered_map<std::string, std::string>::iterator begin();
		std::unordered_map<std::string, std::string>::const_iterator cbegin() const;
		std::unordered_map<std::string, std::string>::iterator end();
		std::unordered_map<std::string, std::string>::const_iterator cend() const;

	protected:
		friend class IniFile;

		/**
		 * INI section data.
		 * - Key: Key name. (UTF-8)
		 * - Value: Value. (UTF-8)
		 */
		std::unordered_map<std::string, std::string> m_data;
};

/**
 * INI file.
 * Contains multiple INI groups.
 */
class IniFile
{
	public:
		IniFile(const std::string &filename);
		IniFile(const std::wstring &filename);
		IniFile(FILE *f);
		~IniFile();

		IniGroup *getGroup(const std::string &section);
		const IniGroup *getGroup(const std::string &section) const;
		IniGroup *createGroup(const std::string &section);

		bool hasGroup(const std::string &section) const;
		bool hasKey(const std::string &section, const std::string &key) const;
		bool hasKeyNonEmpty(const std::string &section, const std::string &key) const;

		std::string getString(const std::string &section, const std::string &key, const std::string &def = "") const;
		std::wstring getWString(const std::string &section, const std::string &key, const std::wstring &def = L"") const;
		bool getBool(const std::string &section, const std::string &key, bool def = false) const;
		int getIntRadix(const std::string &section, const std::string &key, int radix, int def = 0) const;
		int getInt(const std::string &section, const std::string &key, int def = 0) const;
		float getFloat(const std::string &section, const std::string &key, float def = 0) const;

		void setString(const std::string &section, const std::string &key, const std::string &val);
		void setWString(const std::string &section, const std::string &key, const std::wstring &val);
		void setBool(const std::string &section, const std::string &key, bool val);
		void setIntRadix(const std::string &section, const std::string &key, int radix, int val);
		void setInt(const std::string &section, const std::string &key, int val);
		void setFloat(const std::string &section, const std::string &key, float val);

		bool removeGroup(const std::string &group);
		bool removeKey(const std::string &section, const std::string &key);

		void save(const std::string &filename) const;
		void save(const std::wstring &filename) const;
		void save(FILE *f) const;

		std::unordered_map<std::string, IniGroup*>::iterator begin();
		std::unordered_map<std::string, IniGroup*>::const_iterator cbegin() const;
		std::unordered_map<std::string, IniGroup*>::iterator end();
		std::unordered_map<std::string, IniGroup*>::const_iterator cend() const;

	protected:
		void load(FILE *f);
		void clear(void);
		std::string escape(const std::string &str, bool sec, bool key) const;

		/**
		 * INI groups.
		 * - Key: Section name. (UTF-8)
		 * - Value: IniGroup.
		 */
		std::unordered_map<std::string, IniGroup*> m_groups;
};

#endif /* INIFILE_H */
