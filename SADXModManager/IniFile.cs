using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using IniDictionary = System.Collections.Generic.Dictionary<string, System.Collections.Generic.Dictionary<string, string>>;
using IniGroup = System.Collections.Generic.Dictionary<string, string>;
using IniNameGroup = System.Collections.Generic.KeyValuePair<string, System.Collections.Generic.Dictionary<string, string>>;
using IniNameValue = System.Collections.Generic.KeyValuePair<string, string>;

namespace IniFile
{
	public static class IniFile
	{
		public static IniDictionary Load(params string[] data)
		{
			IniDictionary result = new IniDictionary();
			IniGroup curent = new IniGroup();
			result.Add(string.Empty, curent);
			string curgroup = string.Empty;
			for (int i = 0; i < data.Length; i++)
			{
				string line = data[i];
				StringBuilder sb = new StringBuilder(line.Length);
				bool startswithbracket = false;
				int firstequals = -1;
				int endbracket = -1;
				for (int c = 0; c < line.Length; c++)
					switch (line[c])
					{
						case '\\': // escape character
							if (c + 1 == line.Length) goto default;
							c++;
							switch (line[c])
							{
								case 'n': // line feed
									sb.Append('\n');
									break;
								case 'r': // carriage return
									sb.Append('\r');
									break;
								default: // literal character
									sb.Append(line[c]);
									break;
							}
							break;
						case '=':
							if (firstequals == -1)
								firstequals = sb.Length;
							goto default;
						case '[':
							if (c == 0)
								startswithbracket = true;
							goto default;
						case ']':
							endbracket = sb.Length;
							goto default;
						case ';': // comment character, stop processing this line
							c = line.Length;
							break;
						default:
							sb.Append(line[c]);
							break;
					}
				line = sb.ToString();
				if (startswithbracket & endbracket != -1)
				{
					curgroup = line.Substring(1, endbracket - 1);
					curent = new IniGroup();
					try
					{
						result.Add(curgroup, curent);
					}
					catch (ArgumentException ex)
					{
						throw new Exception("INI File error: Group \"" + curgroup + "\" already exists.\nline " + (i + 1), ex);
					}
				}
				else if (!IsNullOrWhiteSpace(line))
				{
					string key;
					string value = string.Empty;
					if (firstequals > -1)
					{
						key = line.Substring(0, firstequals);
						value = line.Substring(firstequals + 1);
					}
					else
						key = line;
					try
					{
						curent.Add(key, value);
					}
					catch (ArgumentException ex)
					{
						throw new Exception("INI File error: Value \"" + key + "\" already exists in group \"" + curgroup + "\".\nline " + (i + 1), ex);
					}
				}
			}
			return result;
		}

		public static IniDictionary Load(string filename) { return Load(File.ReadAllLines(filename)); }

		public static IniDictionary Load(IEnumerable<string> data) { return Load(data.ToArray()); }

		public static IniDictionary Load(Stream stream)
		{
			List<string> data = new List<string>();
			StreamReader reader = new StreamReader(stream);
			string line;
			while ((line = reader.ReadLine()) != null)
				data.Add(line);
			return Load(data.ToArray());
		}

		public static string[] Save(IniDictionary INI)
		{
			List<string> result = new List<string>();
			foreach (IniNameGroup group in INI)
			{
				if (!string.IsNullOrEmpty(group.Key))
					result.Add("[" + group.Key.Replace(@"\", @"\\").Replace("\n", @"\n").Replace("\r", @"\r").Replace(";", @"\;") + "]");
				foreach (IniNameValue value in group.Value)
				{
					string escapedkey = value.Key.Replace(@"\", @"\\").Replace("=", @"\=").Replace("\n", @"\n").Replace("\r", @"\r").Replace(";", @"\;");
					if (escapedkey.StartsWith("["))
						escapedkey = escapedkey.Insert(0, @"\");
					result.Add(escapedkey + "=" + value.Value.Replace(@"\", @"\\").Replace("\n", @"\n").Replace("\r", @"\r").Replace(";", @"\;"));
				}
			}
			return result.ToArray();
		}

		public static void Save(IniDictionary INI, string filename) { File.WriteAllLines(filename, Save(INI)); }

		public static void Save(IniDictionary INI, Stream stream)
		{
			StreamWriter writer = new StreamWriter(stream);
			foreach (string line in Save(INI))
				writer.WriteLine(line);
		}

		public static IniDictionary Combine(IniDictionary dictA, IniDictionary dictB)
		{
			IniDictionary result = new IniDictionary();
			foreach (IniNameGroup group in dictA)
				result.Add(group.Key, new IniGroup(group.Value));
			foreach (IniNameGroup group in dictB)
				if (result.ContainsKey(group.Key))
					foreach (IniNameValue item in group.Value)
						result[group.Key][item.Key] = item.Value;
				else
					result.Add(group.Key, new IniGroup(group.Value));
			return result;
		}

		internal static bool IsNullOrWhiteSpace(string value)
		{
			if (string.IsNullOrEmpty(value))
				return true;
			for (int i = 0; i < value.Length; i++)
				if (!char.IsWhiteSpace(value[i]))
					return false;
			return true;
		}
	}
}