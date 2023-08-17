using SAModManager.Ini;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace SAModManager.IniSettings
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
		public bool EnableManagerBootCheck { get; set; } = true;	// LoaderInfo.UpdateCheck

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
		[DefaultValue (false)]
		public bool EnableDebugFile { get; set; }         // SADXLoaderInfo.DebugFile

		/// <summary>
		/// Enables crash log mini dump creation.
		/// </summary>
		[DefaultValue(true)]
		public bool EnableDebugCrashLog { get; set; } = true;     // SADXLoaderInfo.DebugCrashLog

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

	public class GameManagement
	{
		/// <summary>
		/// Deprecated, not removing until able to.
		/// </summary>
		[DefaultValue((int)SetGame.SADX)]
		public int CurrentSetGame { get; set; } = (int)SetGame.SADX;

		/// <summary>
		/// Deprecated, not removing until able to.
		/// </summary>
		[DefaultValue("")]
		public string SADXProfile { get; set; } = string.Empty;

		/// <summary>
		/// Deprecated, not removing until able to.
		/// </summary>
		[DefaultValue("")]
		public string SA2Profile { get; set; } = string.Empty;

		/// <summary>
		/// The Name/Key of the last loaded profile.
		/// </summary>
		[DefaultValue("")]
		public string LoadedProfile { get; set; } = string.Empty;

		/// <summary>
		/// List of Profile options.
		/// </summary>
		public Dictionary<string, string> Profiles { get; set; } = new();
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
		[DefaultValue(false)]
		public bool KeepManagerOpen { get; set; }       // SADXLoaderInfo.managerOpen

		/// <summary>
		/// Update Settings for the Manager.
		/// </summary>
		[IniAlwaysInclude]
		public UpdateSettings UpdateSettings { get; set; } = new();

		/// <summary>
		/// Debug Settings for the Manager, shared between profiles.
		/// </summary>
		public DebugSettings DebugSettings { get; set; } = new();

		/// <summary>
		/// Game Management so the loader can always use last loaded settings.
		/// </summary>
		[IniAlwaysInclude]
		public GameManagement GameManagement { get; set; } = new();

		public void ConvertFromV0(SADXLoaderInfo oldSettings)
		{
			SettingsVersion = (int)ManagerSettingsVersions.v1;
			UpdateSettings.ConvertFromV0(oldSettings);
			EnableDeveloperMode = oldSettings.devMode;
			KeepManagerOpen = oldSettings.managerOpen;
			GameManagement = new();
			Theme = oldSettings.Theme;
			Language = oldSettings.Language;
		}

		public ManagerSettings Deserialize(string path)
		{
			if (File.Exists(path))
			{
				string jsonContent = File.ReadAllText(path);

				return JsonSerializer.Deserialize<ManagerSettings>(jsonContent);
			}
			else
				return new();
		}

		public void Serialize(string path)
		{
			string jsonContent = JsonSerializer.Serialize(this, new JsonSerializerOptions { WriteIndented = true });

			File.WriteAllText(path, jsonContent);
		}
	}
}
