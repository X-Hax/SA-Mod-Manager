using SAModManager.Ini;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAModManager.ManagerSettings
{
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
	}

	public class AdvancedSettings
	{
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
	}

	public class ManagerSettings
	{
		public enum ManagerSettingsVersions
		{
			v0 = 0,
			v1 = 1
		}

		/// <summary>
		/// Versioning for the Manager Settings file.
		/// </summary>
		[IniAlwaysInclude]
		[DefaultValue((int)ManagerSettingsVersions.v1)]
		public int SettingsVersion { get; set; }

		/// <summary>
		/// Update Settings for the Manager.
		/// </summary>
		[IniAlwaysInclude]
		public UpdateSettings UpdateSettings { get; set; } = new();

		/// <summary>
		/// Advanced Settings for the Manager.
		/// </summary>
		[IniAlwaysInclude]
		public AdvancedSettings AdvancedSettings { get; set; } = new();

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
	}
}
