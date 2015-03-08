/**
 * SADX Mod Loader
 * INI file parser.
 */

#include "stdafx.h"
#include "IniFile.hpp"
#include "TextConv.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
using std::transform;
using std::string;
using std::unordered_map;
using std::wstring;

/** IniGroup **/

/**
 * Check if the INI group has the specified key.
 * @param key Key.
 * @return True if the key exists; false if not.
 */
bool IniGroup::hasKey(const string &key) const
{
	return (m_data.find(key) != m_data.end());
}

/**
 * Check if the INI group has the specified key with a non-empty value.
 * @param key Key.
 * @return True if the key exists; false if not or value is empty.
 */
bool IniGroup::hasKeyNonEmpty(const string &key) const
{
	auto iter = m_data.find(key);
	if (iter == m_data.end())
		return false;

	return !iter->second.empty();
}

const unordered_map<string, string> *IniGroup::data(void) const
{
	return &m_data;
}

/**
 * Get a string value from the INI group.
 * @param key Key.
 * @param def Default value.
 * @return String value.
 */
string IniGroup::getString(const string &key, const string &def) const
{
	auto iter = m_data.find(key);
	return (iter != m_data.end() ? iter->second : def);
}

/**
 * Get a wide string value from the INI group.
 * INI strings are converted from UTF-8 to UTF-16.
 * @param key Key.
 * @param def Default value.
 * @return Wide string value.
 */
wstring IniGroup::getWString(const string &key, const wstring &def) const
{
	auto iter = m_data.find(key);
	if (iter == m_data.end())
		return def;

	// Convert the string from UTF-8 to UTF-16.
	return MBStoUTF16(iter->second, CP_UTF8);
}

/**
 * Get a boolean value from the INI group.
 * @param key Key.
 * @param def Default value.
 * @return Boolean value.
 */
bool IniGroup::getBool(const string &key, bool def) const
{
	auto iter = m_data.find(key);
	if (iter == m_data.end())
		return def;

	string value = iter->second;
	transform(value.begin(), value.end(), value.begin(), ::tolower);
	// TODO: Support integer values.
	return (value == "true");
}

/**
 * Get an integer value from the INI group.
 * @param key Key.
 * @param radix Radix.
 * @param def Default value.
 * @return Integer value.
 */
int IniGroup::getIntRadix(const string &key, int radix, int def) const
{
	auto iter = m_data.find(key);
	if (iter == m_data.end())
		return def;

	string value = iter->second;
	return (int)strtol(value.c_str(), nullptr, radix);
}

/**
 * Get an integer value from the INI group.
 * @param key Key.
 * @param def Default value.
 * @return Integer value.
 */
int IniGroup::getInt(const string &key, int def) const
{
	return getIntRadix(key, 10, def);
}

/**
 * Get an floating-point value from the INI group.
 * @param key Key.
 * @param def Default value.
 * @return Floating-point value.
 */
float IniGroup::getFloat(const string &key, float def) const
{
	auto iter = m_data.find(key);
	if (iter == m_data.end())
		return def;

	string value = iter->second;
	return (float)strtod(value.c_str(), nullptr);
}

std::unordered_map<string, string>::iterator IniGroup::begin()
{
	return m_data.begin();
}

std::unordered_map<string, string>::const_iterator IniGroup::cbegin() const
{
	return m_data.cbegin();
}

std::unordered_map<string, string>::reverse_iterator IniGroup::rbegin()
{
	return m_data.rbegin();
}

std::unordered_map<string, string>::const_reverse_iterator IniGroup::crbegin() const
{
	return m_data.crbegin();
}

std::unordered_map<string, string>::iterator IniGroup::end()
{
	return m_data.end();
}

std::unordered_map<string, string>::const_iterator IniGroup::cend() const
{
	return m_data.cend();
}

std::unordered_map<string, string>::reverse_iterator IniGroup::rend()
{
	return m_data.rend();
}

std::unordered_map<string, string>::const_reverse_iterator IniGroup::crend() const
{
	return m_data.crend();
}

/** IniFile **/

IniFile::IniFile(const string &filename)
{
	FILE *f = fopen(filename.c_str(), "r");
	if (!f)
		return;

	load(f);
	fclose(f);
}

IniFile::IniFile(const wstring &filename)
{
	FILE *f = _wfopen(filename.c_str(), L"r");
	if (!f)
		return;

	load(f);
	fclose(f);
}

IniFile::IniFile(FILE *f)
{
	load(f);
}

IniFile::~IniFile()
{
	clear();
}

/**
 * Get an INI group.
 * @param section Section.
 * @return INI group, or nullptr if not found.
 */
const IniGroup *IniFile::getGroup(const string &section) const
{
	auto iter = m_groups.find(section);
	return (iter != m_groups.end() ? iter->second : nullptr);
}

/**
 * Check if the INI file has the specified group.
 * @param section Section.
 * @return True if the section exists; false if not.
 */
bool IniFile::hasGroup(const string &section) const
{
	return (m_groups.find(section) != m_groups.end());
}

/**
 * Check if the INI file has the specified key.
 * @param section Section.
 * @param key Key.
 * @return True if the key exists; false if not.
 */
bool IniFile::hasKey(const string &section, const string &key) const
{
	auto iter = m_groups.find(section);
	if (iter == m_groups.end())
		return false;

	return iter->second->hasKey(key);
}

/**
 * Check if the INI file has the specified key with a non-empty value.
 * @param section Section.
 * @param key Key.
 * @return True if the key exists; false if not or value is empty.
 */
bool IniFile::hasKeyNonEmpty(const string &section, const string &key) const
{
	auto iter = m_groups.find(section);
	if (iter == m_groups.end())
		return false;

	return iter->second->hasKeyNonEmpty(key);
}

/**
 * Get a string value from the INI file.
 * @param section Section.
 * @param key Key.
 * @param def Default value.
 * @return String value.
 */
string IniFile::getString(const string &section, const string &key, const string &def) const
{
	const IniGroup *group = getGroup(section);
	if (!group)
		return def;
	return group->getString(key, def);
}

/**
 * Get a wide string value from the INI group.
 * INI strings are converted from UTF-8 to UTF-16.
 * @param section Section.
 * @param key Key.
 * @param def Default value.
 * @return Wide string value.
 */
wstring IniFile::getWString(const string &section, const string &key, const wstring &def) const
{
	const IniGroup *group = getGroup(section);
	if (!group)
		return def;
	return group->getWString(key, def);
}

/**
 * Get a boolean value from the INI file.
 * @param section Section.
 * @param key Key.
 * @param def Default value.
 * @return Boolean value.
 */
bool IniFile::getBool(const string &section, const string &key, bool def) const
{
	const IniGroup *group = getGroup(section);
	if (!group)
		return def;
	return group->getBool(key, def);
}

/**
 * Get an integer value from the INI file.
 * @param section Section.
 * @param key Key.
 * @param radix Radix.
 * @param def Default value.
 * @return Integer value.
 */
int IniFile::getIntRadix(const string &section, const string &key, int radix, int def) const
{
	const IniGroup *group = getGroup(section);
	if (!group)
		return def;
	return group->getIntRadix(key, radix, def);
}

/**
 * Get an integer value from the INI file.
 * @param section Section.
 * @param key Key.
 * @param def Default value.
 * @return Integer value.
 */
int IniFile::getInt(const string &section, const string &key, int def) const
{
	const IniGroup *group = getGroup(section);
	if (!group)
		return def;
	return group->getInt(key, def);
}

/**
 * Get a floating-point value from the INI file.
 * @param section Section.
 * @param key Key.
 * @param def Default value.
 * @return Floating-point value.
 */
float IniFile::getFloat(const string &section, const string &key, float def) const
{
	const IniGroup *group = getGroup(section);
	if (!group)
		return def;
	return group->getFloat(key, def);
}

std::unordered_map<string, IniGroup*>::iterator IniFile::begin()
{
	return m_groups.begin();
}

std::unordered_map<string, IniGroup*>::const_iterator IniFile::cbegin() const
{
	return m_groups.cbegin();
}

std::unordered_map<string, IniGroup*>::reverse_iterator IniFile::rbegin()
{
	return m_groups.rbegin();
}

std::unordered_map<string, IniGroup*>::const_reverse_iterator IniFile::crbegin() const
{
	return m_groups.crbegin();
}

std::unordered_map<string, IniGroup*>::iterator IniFile::end()
{
	return m_groups.end();
}

std::unordered_map<string, IniGroup*>::const_iterator IniFile::cend() const
{
	return m_groups.cend();
}

std::unordered_map<string, IniGroup*>::reverse_iterator IniFile::rend()
{
	return m_groups.rend();
}

std::unordered_map<string, IniGroup*>::const_reverse_iterator IniFile::crend() const
{
	return m_groups.crend();
}

/**
 * Load an INI file.
 * Internal function; called from the constructor.
 * @param f FILE pointer. (File is not closed after processing.)
 */
void IniFile::load(FILE *f)
{
	clear();
	fseek(f, 0, SEEK_SET);

	// Create an empty group for default settings.
	IniGroup *curGroup = new IniGroup();
	m_groups[""] = curGroup;

	// Process the INI file.
	while (!feof(f))
	{
		char line[1024];
		char *ret = fgets(line, sizeof(line), f);
		if (!ret)
			break;
		const int line_len = (int)strnlen(line, sizeof(line));
		if (line_len == 0)
			continue;

		bool startswithbracket = false;
		int firstequals = -1;
		int endbracket = -1;

		// String can contain escape characters, so
		// we need a string buffer.
		string sb;
		sb.reserve(line_len);

		// Process the line.
		for (int c = 0; c < line_len; c++)
		{
			switch (line[c])
			{
				case '\\': // escape character
					if (c + 1 >= line_len)
					{
						// Backslash at the end of the line.
						goto appendchar;
					}
					c++;
					switch (line[c])
					{
						case 'n': // line feed
							sb += '\n';
							break;
						case 'r': // carriage return
							sb += '\r';
							break;
						default: // literal character
							goto appendchar;
					}
					break;

				case '=':
					if (firstequals == -1)
						firstequals = sb.length();
					goto appendchar;

				case '[':
					if (c == 0)
						startswithbracket = true;
					goto appendchar;

				case ']':
					endbracket = sb.length();
					goto appendchar;

				case ';':	// comment character
				case '\r':	// trailing newline (CRLF)
				case '\n':	// trailing newline (LF)
					// Stop processing this line.
					c = line_len;
					break;

				default:
appendchar:
					// Normal character. Append to the string buffer.
					sb += line[c];
					break;
			}
		}

		// Check the string buffer.
		if (startswithbracket && endbracket != -1)
		{
			// New section.
			string section = sb.substr(1, endbracket - 1);
			auto iter = m_groups.find(section);
			if (iter != m_groups.end())
			{
				// Section already exists.
				// Use the existing section.
				curGroup = iter->second;
			}
			else
			{
				// New section.
				curGroup = new IniGroup();
				m_groups[section] = curGroup;
			}
		}
		else if (!sb.empty())
		{
			// Key/value.
			string key;
			string value;
			if (firstequals > -1)
			{
				key = sb.substr(0, firstequals);
				value = sb.substr(firstequals + 1);
			}
			else
			{
				key = line;
			}

			// Store the value in the current group.
			curGroup->m_data[key] = value;
		}
	}
}

/**
 * Clear the loaded INI file.
 */
void IniFile::clear(void)
{
	for (auto iter = m_groups.begin(); iter != m_groups.end(); ++iter)
	{
		delete iter->second;
	}

	m_groups.clear();
}
