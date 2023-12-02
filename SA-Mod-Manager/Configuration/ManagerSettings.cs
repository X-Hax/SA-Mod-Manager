using SAModManager.Ini;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace SAModManager.Configuration
{
	public enum ManagerSettingsVersions
	{
		v0 = 0,
		v1 = 1,
	}

	public enum SetGame
	{
		None = 0,
		SADX = 1,
		SA2 = 2,
	}

	public class UpdateSettings
	{
		/// <summary>
		/// Checks for Manager Updates on Boot.
		/// </summary>
		[DefaultValue(true)]
		public bool EnableManagerBootCheck { get; set; } = true;    // LoaderInfo.UpdateCheck

		/// <summary>
		/// Checks for Mod Updates on Boot.
		/// </summary>
		[DefaultValue(true)]
		public bool EnableModsBootCheck { get; set; } = true;       // LoaderInfo.ModUpdateCheck

		/// <summary>
		/// Checks for Loader Updates on Boot.
		/// </summary>
		[DefaultValue(true)]
		public bool EnableLoaderBootCheck { get; set; } = true;
		public long UpdateTimeOutCD { get; set; } = 0;
		public int UpdateCheckCount { get; set; } = 0;


		public void ConvertFromV0(SADXLoaderInfo oldSettings)
		{
			EnableManagerBootCheck = oldSettings.UpdateCheck;
			EnableModsBootCheck = oldSettings.ModUpdateCheck;
			EnableLoaderBootCheck = true;
		}
	}

	public class DebugSettings
	{
		/// <summary>
		/// Enables debug printing to the console window.
		/// </summary>
		[DefaultValue(false)]
		public bool EnableDebugConsole { get; set; }      // SADXLoaderInfo.DebugConsole

		/// <summary>
		/// Enables debug printing in the game window.
		/// </summary>
		[DefaultValue(false)]
		public bool EnableDebugScreen { get; set; }       // SADXLoaderInfo.DebugScreen

		/// <summary>
		/// Enables debug printing to a file.
		/// </summary>
		[DefaultValue(false)]
		public bool EnableDebugFile { get; set; }         // SADXLoaderInfo.DebugFile

		/// <summary>
		/// Enables crash log mini dump creation.
		/// </summary>
		[DefaultValue(true)]
		public bool EnableDebugCrashLog { get; set; } = true;    // SADXLoaderInfo.DebugCrashLog

		/// <summary>
		/// Enables the console window to render. Print
		/// </summary>
		[DefaultValue(false)]
		public bool? EnableShowConsole { get { return null; } set { if (value.HasValue) EnableDebugConsole = value.Value; } }   // SADXLoaderInfo.ShowConsole

		public void ConvertFromV0(SADXLoaderInfo oldSettings)
		{
			EnableDebugConsole = oldSettings.DebugConsole;
			EnableDebugScreen = oldSettings.DebugScreen;
			EnableDebugFile = oldSettings.DebugFile;
			EnableDebugCrashLog = oldSettings.DebugCrashLog;
			EnableShowConsole = oldSettings.ShowConsole;
		}
	}

	public class ManagerSettings
	{
		/// <summary>
		/// Versioning for the Manager Settings file.
		/// </summary>
		[IniAlwaysInclude]
		[DefaultValue((int)ManagerSettingsVersions.v1)]
		public int SettingsVersion { get; set; } = (int)ManagerSettingsVersions.v1;

		/// <summary>
		/// Last Loaded Game
		/// </summary>
		[DefaultValue((int)SetGame.None)]
		public int CurrentSetGame { get; set; } = (int)SetGame.None;

		/// <summary>
		/// The set Theme for the Manager.
		/// </summary>
		[DefaultValue(0)]
		public int Theme { get; set; } = 0;         // SADXLoaderInfo.Theme

		/// <summary>
		/// The set Language for the Manager.
		/// </summary>
		[DefaultValue(0)]
		public int Language { get; set; } = 0;      // SADXLoaderInfo.Language

		/// <summary>
		/// Defaulted to empty, but is updated when a mod is initially created to the last used Author name when creating a new mod.
		/// </summary>
		public string ModAuthor { get; set; } = string.Empty; //From mod creation: Save the Mod Author from user input, so they don't have to re type it.

		/// <summary>
		/// Enables Developer Options in the Manager.
		/// </summary>
		[DefaultValue(false)]
		public bool EnableDeveloperMode { get; set; }           // SADXLoaderInfo.devMode

		/// <summary>
		/// Keeps the Manager open when launching the game.
		/// </summary>
		[DefaultValue(true)]
		public bool KeepManagerOpen { get; set; } = true;   // SADXLoaderInfo.managerOpen

		/// <summary>
		/// Update Settings for the Manager.
		/// </summary>
		[IniAlwaysInclude]
		public UpdateSettings UpdateSettings { get; set; } = new();

		/// <summary>
		/// Deserializes a Manager Settings CFG (JSON) file and returns a populated class.
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		public static ManagerSettings Deserialize(string path)
		{
			if (File.Exists(path))
			{
				string jsonContent = File.ReadAllText(path);

				return JsonSerializer.Deserialize<ManagerSettings>(jsonContent);
			}
			else
				return new();
		}

		/// <summary>
		/// Serializes a Manager Setting CFG (JSON) file.
		/// </summary>
		/// <param name="path"></param>
		public void Serialize(string path)
		{
			string jsonContent = JsonSerializer.Serialize(this, new JsonSerializerOptions { WriteIndented = true });

			File.WriteAllText(path, jsonContent);
		}
	}

	public class ProfileEntry
	{
		public string Name { get; set; } = string.Empty;

		public string Filename { get; set; } = string.Empty;

		public ProfileEntry(string name, string filename)
		{
			Name = name;
			Filename = filename;
		}
	}

	public class Profiles
	{
		/// <summary>
		/// Index of the current/last selected profile.
		/// </summary>
		public int ProfileIndex { get; set; }

		/// <summary>
		/// List of Profile options.
		/// </summary>
		public List<ProfileEntry> ProfilesList { get; set; } = new();

		/// <summary>
		/// Returns the Profile to load using the Manager's settings and the Profiles.json file for the specified game.
		/// </summary>
		/// <param name="profiles"></param>
		/// <returns></returns>
		public string GetProfileFilename()
		{
			if (ProfilesList.Count > 0)
				return ProfilesList.ElementAt(ProfileIndex).Filename;
			else
				return string.Empty;
		}

		/// <summary>
		/// Deserializes a file and returns a populated Profiles class, returns new if file doesn't exist.
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		public static Profiles Deserialize(string path)
		{
			if (File.Exists(path))
			{
				string jsonContent = File.ReadAllText(path);

				return JsonSerializer.Deserialize<Profiles>(jsonContent);
			}
			else
				return new()
				{
					ProfilesList = new List<ProfileEntry> { new ProfileEntry("Default", "Default.json") }
				};
		}

		/// <summary>
		/// Serializes Profiles to JSON.
		/// </summary>
		/// <param name="path"></param>
		public void Serialize(string path)
		{
			string jsonContent = JsonSerializer.Serialize(this, new JsonSerializerOptions { WriteIndented = true });

			File.WriteAllText(path, jsonContent);
		}

		public static Profiles MakeDefaultProfileFile()
		{
			return new()
			{
				ProfileIndex = 0,
				ProfilesList = new List<ProfileEntry> { new ProfileEntry("Default", "Default.json") }
			};
		}
	}
}

