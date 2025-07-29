using SAMM.Utilities.INI;
using System.Reflection;

namespace SAMM.Configuration.Mods
{
	/// <summary>
	/// The information found in mod.ini.
	/// </summary>
	public class ModInfo
	{
		#region Variables
		/// <summary>
		/// Name of the mod.
		/// </summary>
		public string Name { get; set; } = string.Empty;

		/// <summary>
		/// Category of the mod.
		/// </summary>
		public string Category { get; set; } = ModCategory[6];

		/// <summary>
		/// Author's name.
		/// </summary>
		public string Author { get; set; } = string.Empty;

		/// <summary>
		/// Author's website/social link.
		/// </summary>
		public string AuthorURL { get; set; } = string.Empty;
		
		/// <summary>
		/// Version number of the mod.
		/// </summary>
		public string Version { get; set; } = string.Empty;
		
		/// <summary>
		/// Description of the mod.
		/// </summary>
		public string Description { get; set; } = string.Empty;
		
		/// <summary>
		/// Custom executable that will be used over the game's original executable.
		/// </summary>
		public string EXEFile { get; set; } = string.Empty;

		/// <summary>
		/// Code file belonging to the mod to be loaded.
		/// </summary>
		public string DLLFile { get; set; } = string.Empty;

		/// <summary>
		/// Redirects the main save file to point to a save file in the mod's folder.
		/// </summary>
		public bool RedirectMainSave { get; set; } = false;

		/// <summary>
		/// Redirects the Chao save file to point to a save file in the mod's folder.
		/// </summary>
		public bool RedirectChaoSave { get; set; } = false;
		
		/// <summary>
		/// Link to the Source code of the mod if applicable.
		/// </summary>
		public string SourceCode { get; set; } = string.Empty;
		
		/// <summary>
		/// Codes file.
		/// </summary>
		public string Codes { get; set; } = string.Empty;
		
		/// <summary>
		/// Link to the Github Repo for the mod. This is used for updates.
		/// </summary>
		public string GitHubRepo { get; set; } = string.Empty;
		
		/// <summary>
		/// Name of the archive that's downloaded from the Github repository when updating.
		/// </summary>
		public string GitHubAsset { get; set; } = string.Empty;
		
		/// <summary>
		/// Link to a direct hosting of the mod's content, used for updating the mod.
		/// </summary>
		public string UpdateUrl { get; set; } = string.Empty;
		
		/// <summary>
		/// The url to the changelog to be displayed when using self hosted updates.
		/// </summary>
		public string ChangelogUrl { get; set; } = string.Empty;
		
		/// <summary>
		/// The Item Type from Gamebanana, used for updates.
		/// </summary>
		public string GameBananaItemType { get; set; } = string.Empty;

		/// <summary>
		/// Item ID taken from Gamebanana, used for updates.
		/// </summary>
		public long? GameBananaItemId { get; set; }
		
		/// <summary>
		/// The custom ID for the mod that's used for dependency checks.
		/// </summary>
		public string ModID { get; set; } = string.Empty;

		/// <summary>
		/// List of mods that are required by the current mod.
		/// </summary>
		[IniName("Dependency")]
		[IniCollection(IniCollectionMode.SingleLine, Format = "|", StartIndex = 1)]
		public List<ModDependency> Dependencies { get; set; } = new List<ModDependency>();

		/// <summary>
		/// Disables updates for the specific mod.
		/// </summary>
		public bool DisableUpdate { get; set; } = false;

		/// <summary>
		/// Selected include directory.
		/// </summary>
		[IniName("Config", "IncludeDir")]
		[IniCollection(IniCollectionMode.NoSquareBrackets, StartIndex = 0)]
		public List<string> IncludeDirs { get; set; } = new List<string>();

		/// <summary>
		/// Number of include directories.
		/// </summary>
		[IniName("Config", "IncludeDirCount")]
		public string IncludeDirCount { get; set; } = "0";

		/// <summary>
		/// List of premade Mod Categories.
		/// </summary>
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

		#endregion

		#region Functions
		/// <summary>
		/// Scans through the mods directory for all valid mods.
		/// </summary>
		/// <param name="directoryInfo"></param>
		/// <returns></returns>
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

		/// <summary>
		/// Collects all properties within a mod.ini file and returns them as an array of strings.
		/// </summary>
		/// <returns></returns>
		public static string[] GetAllVariablesName()
		{
			PropertyInfo[] properties = typeof(ModInfo).GetProperties();
			List<String> AllVariables = new();

			foreach (PropertyInfo property in properties)
			{
				AllVariables.Add(property.Name);
			}

			return AllVariables.ToArray();
		}

		#endregion
	}
}
