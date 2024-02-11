using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Reflection;
using System.Text.Json;
using PropertyChanged;
using SAModManager.Common;
using SAModManager.Configuration;
using SAModManager.Ini;

namespace SAModManager
{
	public class ModInfo
	{
		public string Name { get; set; }
		public string Category { get; set; }
		public string Author { get; set; }
		public string AuthorURL { get; set; }
		public string Version { get; set; }
		public string Description { get; set; }
		public string DLLFile { get; set; }
		public string SourceCode { get; set; }
		public string Codes { get; set; }
		public string GitHubRepo { get; set; }
		public string GitHubAsset { get; set; }
		public string UpdateUrl { get; set; }
		public string ChangelogUrl { get; set; }
		public string GameBananaItemType { get; set; }
		public long? GameBananaItemId { get; set; }
		public string ModID { get; set; }
		[IniName("Dependency")]
		[IniCollection(IniCollectionMode.NoSquareBrackets, StartIndex = 1)]
		public List<string> Dependencies { get; set; }
		public bool DisableUpdate { get; set; }

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
				if (item.Name[0] == '.')
				{
					continue;
				}

				foreach (string filename in GetModFiles(item))
					yield return filename;
			}
		}
	}

	public class SAModInfo : ModInfo
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
			"Translation",
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

                if (item.Name.Equals("gd_pc", StringComparison.OrdinalIgnoreCase))
                {
                    continue;
                }

                foreach (string filename in GetModFiles(item))
					yield return filename;
			}
		}

		public static string[] GetAllVariablesName()
		{
			PropertyInfo[] properties = typeof(SAModInfo).GetProperties();
			List<String> AllVariables = new();

			foreach (PropertyInfo property in properties)
			{
				AllVariables.Add(property.Name);
			}

			return AllVariables.ToArray();
		}
	}

    public class ModData : INotifyPropertyChanged
	{
		public string Name { get; set; }
		public string Author { get; set; }
		public string AuthorURL { get; set; }
		public string Version { get; set; }
		public string Category { get; set; }
		public string Description { get; set; }
		public string SourceCode { get; set; }

        public bool IsChecked { get; set; }
		public string Tag { get; set; }

        public event PropertyChangedEventHandler PropertyChanged;
    }

    public class CodeData : INotifyPropertyChanged
	{
		public Code codes { get; set; }
		public bool IsChecked { get; set; }
		public bool IsEnabled { get; set; }

        public event PropertyChangedEventHandler PropertyChanged;
    }

	public class PatchesData
	{
		public string Name { get; set; }
		public string Author { get; set; }
		public string Category { get; set; }
		public string Description { get; set; }
		public bool IsChecked { get; set; }
		public string InternalName { get; set; }
	}

	public class PatchesList
	{
		public List<PatchesData> Patches { get; set; } = new();
        public static PatchesList Deserialize(string path)
        {
            if (File.Exists(path))
            {
                string jsonContent = File.ReadAllText(path);
                return JsonSerializer.Deserialize<PatchesList>(jsonContent);
            }

			return null;
        }
    }
}
