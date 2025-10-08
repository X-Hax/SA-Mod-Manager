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
		/// <summary>
		/// Information from the mod's ini file. See <see cref="ModInfo"/>.
		/// </summary>
		public ModInfo? ModInfo { get; set; }

		/// <summary>
		/// Path to the mod directory.
		/// </summary>
		public string Directory { get; private set; } = string.Empty;

		/// <summary>
		/// Folder name of the mod. Used by the game config.
		/// </summary>
		public string Key { get { return Path.GetFileName(Directory); } }

		/// <summary>
		/// If the mod is enabled, this will return true.
		/// </summary>
		public bool IsEnabled { get; set; } = false;

		/// <summary>
		/// If the mod has a <see cref="ConfigSchema"/> file, will return true.
		/// </summary>
		public bool HasConfig { get; set; } = false;

		/// <summary>
		/// Path to the ConfigSchema file for a mod. If <see cref="HasConfig"/> is false, returns an empty string.
		/// </summary>
		public string ConfigSchemaPath
		{
			get
			{
				if (HasConfig)
					return Path.Combine(Directory, "configschema.xml");
				else
					return string.Empty;
			}
		}

		/// <summary>
		/// Path to the Config File. If <see cref="HasConfig"/> is false, returns an empty string.
		/// 
		/// It will not verify the file exists, that must be done by whatever process is loading/saving the file.
		/// </summary>
		public string ConfigFilePath
		{
			get
			{
				if (HasConfig)
					return Path.Combine(Directory, "config.ini");
				else
					return string.Empty;
			}
		}

		#endregion

		#region Constructors
		public ModEntry() { }

		public ModEntry(string modPath)
		{
			if (Path.Exists(Path.GetFullPath(modPath)))
			{
				if (File.Exists(Path.Combine(modPath)))
				{
					ModInfo = IniSerializer.Deserialize<ModInfo>(modPath);
					if (ModInfo == null)
						return;

					Directory = Path.GetDirectoryName(modPath);
					HasConfig = File.Exists(Path.Combine(Directory, "configschema.xml"));
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
						yield return new ModEntry(path);
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
