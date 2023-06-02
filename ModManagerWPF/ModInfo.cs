using ModManagerCommon;
using System;
using System.Collections.Generic;
using System.IO;

namespace ModManagerWPF
{
	public class SADXModInfo : ModInfo
	{

		public string EXEFile { get; set; }
		public bool RedirectMainSave { get; set; }
		public bool RedirectChaoSave { get; set; }

		public static List<string> ModCategory { get; set; } = new()
		{
			"Animations",
			"Chao",
			"Custom Level",
			"Cutscene",
			"Game Overhaul",
			"Gameplay",
			"Misc",
			"Music",
			"Patch",
			"Skin",
			"Sound",
			"Textures",
			"UI"
		};

		public static new IEnumerable<string> GetModFiles(DirectoryInfo directoryInfo)
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
