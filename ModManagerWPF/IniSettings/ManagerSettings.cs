using SAModManager.Ini;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
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
		/// Currently Set game, defaults to None. Should only Reset to None if no GameConfigs can be found.
		/// </summary>
		[DefaultValue((int)SetGame.SADX)]
		public int CurrentSetGame { get; set; } = (int)SetGame.SADX;

		/// <summary>
		/// Most recently loaded SADX Game Profile.
		/// </summary>
		[DefaultValue("")]
		public string SADXProfile { get; set; } = string.Empty;

		/// <summary>
		/// Most recently loaded SA2 Game Profile.
		/// </summary>
		[DefaultValue("")]
		public string SA2Profile { get; set; } = string.Empty;
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
        /// Update Settings for the Manager.
        /// </summary>
        [IniAlwaysInclude]
		public UpdateSettings UpdateSettings { get; set; } = new();

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
		/// Game Management so the loader can always use last loaded settings.
		/// </summary>
		[IniAlwaysInclude]
		public GameManagement GameManagement { get; set; } = new();

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
	}
}
