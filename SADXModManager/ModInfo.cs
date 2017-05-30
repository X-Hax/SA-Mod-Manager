using System;
using System.Collections.Generic;
using System.IO;

namespace SADXModManager
{
	public class ModInfo
	{
		public string Name { get; set; }
		public string Author { get; set; }
		public string Version { get; set; }
		public string Description { get; set; }
		public string EXEFile { get; set; }
		public string DLLFile { get; set; }
		public bool RedirectMainSave { get; set; }
		public bool RedirectChaoSave { get; set; }
		public string Codes { get; set; }
		public string GitHubRepo { get; set; }
		public string GitHubAsset { get; set; }
		public string UpdateUrl { get; set; }
		public string ChangelogUrl { get; set; }

		public static IEnumerable<string> GetModFiles(DirectoryInfo directoryInfo)
		{
			string modini = Path.Combine(directoryInfo.FullName, "mod.ini");
			if (File.Exists(modini))
			{
				yield return modini;
				yield break;
			}

			foreach (DirectoryInfo item in directoryInfo.GetDirectories())
			{
				if (item.Name.Equals("system", StringComparison.OrdinalIgnoreCase) || item.Name[0] == '.')
				{
					continue;
				}

				foreach (string filename in GetModFiles(item))
					yield return filename;
			}
		}
	}
}
