using SAMM.Utilities.INI;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace SAMM.Configuration.Mods
{
	/// <summary>
	/// Stores info on Mods that are found by the Manager
	/// </summary>
	public class ModEntry
	{
		#region Variables
		public ModInfo? ModInfo { get; set; }

		public string Directory { get; private set; } = string.Empty;

		public bool IsEnabled { get; set; } = false;

		public bool HasConfig { get; set; } = false;

		#endregion

		#region Constructors
		public ModEntry() { }

		public ModEntry(string gameDirectory, string modPath)
		{
			if (Path.Exists(Path.GetFullPath(modPath)))
			{
				if (File.Exists(Path.Combine(modPath)))
				{
					ModInfo = IniSerializer.Deserialize<ModInfo>(modPath);
					if (ModInfo == null)
						return;

					Directory = Path.GetRelativePath(gameDirectory, modPath);
					HasConfig = File.Exists(Path.Combine(Path.GetDirectoryName(modPath), "configschema.xml"));
				}
			}
		}

		#endregion

		#region Functions
		public static IEnumerable<ModEntry> GetModEntries(string gameDirectory)
		{
			string modsDirectory = Path.Combine(gameDirectory, "mods");
			if (Path.Exists(modsDirectory))
			{
				if (File.Exists(Path.Combine(modsDirectory, "mods.ini")))
				{
					throw new Exception("Invalid file! mod.ini located in mods directory.");
				}

				foreach (var subdir in System.IO.Directory.EnumerateDirectories(modsDirectory))
				{
					string path = Path.Combine(subdir, "mod.ini");
					if (File.Exists(path))
					{
						yield return new ModEntry(gameDirectory, path);
						continue;
					}
				}
			}
			else
				yield break;
		}

		#endregion
	}
}
