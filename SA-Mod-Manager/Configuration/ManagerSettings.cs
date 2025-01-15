using SAModManager.Ini;
using System;
using System.ComponentModel;
using System.IO;
using System.Text.Json;
using SAModManager.UI;
using SAModManager.Configuration.SA2;
using SAModManager.Configuration.SADX;
using System.Collections.Generic;
using System.Security.RightsManagement;
using System.Text;

namespace SAModManager.Configuration
{
    public enum ManagerSettingsVersions
    {
        v0 = 0,	// V0: Pre-Release
        v1 = 1,	// V1: Initial Release
		v2 = 2,	// V2: Migrated to GameEntry system for managing game selection setup.

		MAX,
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

        public void ConvertFromV0(SA2LoaderInfo oldSettings)
        {
            EnableDebugConsole = oldSettings.DebugConsole;
            EnableDebugScreen = oldSettings.DebugScreen;
            EnableDebugFile = oldSettings.DebugFile;
            EnableDebugCrashLog = oldSettings.DebugCrashLog;
            EnableShowConsole = oldSettings.ShowConsole;
        }
    }

	public class AdvancedSettings
	{
		

		public AdvancedSettings() { }
	}

	public class GameEntry
	{
		public enum GameType
		{
			Unsupported = 0,
			SADX = 1,
			SA2 = 2,
		}

		/// <summary>
		/// Name of the <see cref="GameEntry"/>.
		/// </summary>
		public string Name { get; set; }

		/// <summary>
		/// Installation directory of the <see cref="GameEntry"/>.
		/// </summary>
		public string Directory { get; set; }

		/// <summary>
		/// Executable name for the <see cref="GameEntry"/>
		/// </summary>
		public string Executable { get; set; }

		/// <summary>
		/// <see cref="GameType"/> for the <see cref="GameEntry"/>.
		/// </summary>
		public GameType Type { get; set; }

		public GameEntry() { }

		public GameEntry(Game game)
		{
			Name = game.gameName;
			Directory = game.gameDirectory;
			Executable = game.exeName;
			Type = game.id;
		}

		public void Create(string executablePath)
		{
			Executable = Path.GetFileName(executablePath);
			Directory = Path.GetDirectoryName(executablePath);

			switch (Executable)
			{
				default:
					Name = Path.GetFileNameWithoutExtension(Executable);
					Type = GameType.Unsupported;
					break;
				case "sonic.exe":
					Name = "Sonic Adventure DX";
					Type = GameType.SADX;
					break;
				case "sonic2app.exe":
				case "sonic2app_decrypted.exe":
					Name = "Sonic Adventure 2";
					Type = GameType.SA2;
					break;
			}
		}
	}

    public class ManagerSettings
    {
        /// <summary>
        /// Versioning for the Manager Settings file.
        /// </summary>
        [DefaultValue((int)ManagerSettingsVersions.MAX-1)]
        public int SettingsVersion { get; set; } = (int)(ManagerSettingsVersions.MAX-1);

        /// <summary>
        /// Last Loaded Game
        /// </summary>
        [DefaultValue(-1)]
        public int CurrentSetGame { get; set; } = -1;

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
		/// Advanced Settings for the Manager.
		/// </summary>
		//public AdvancedSettings AdvancedSettings { get; set; } = new();

		/// <summary>
		/// List of installed games.
		/// </summary>
		public List<GameEntry> GameEntries { get; set; } = new();

        public double managerWidth { get; set; }
        public double managerHeight { get; set; }
        [DefaultValue(false)]
        public bool KeepModOrder { get; set; } = false;

        /// <summary>
        /// Deserializes a Manager Settings CFG (JSON) file and returns a populated class.
        /// </summary>
        /// <param name="path"></param>
        /// <returns></returns>
        public static ManagerSettings Deserialize(string path)
        {
            try
            {
                if (File.Exists(path))
                {
                    string jsonContent = File.ReadAllText(path);

					ManagerSettings settings = JsonSerializer.Deserialize<ManagerSettings>(jsonContent);

					// Switch case to bump settings version. Allows for manual adjustment if needed for any version bumps.
					switch (settings.SettingsVersion)
					{
						default:
							if (settings.SettingsVersion < (int)(ManagerSettingsVersions.MAX - 1))
								settings.SettingsVersion = (int)(ManagerSettingsVersions.MAX - 1);
							break;
					}

                    return settings;
                }
                else
                {
                    return new();
                }

            }
            catch (Exception ex)
            {
                new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), Lang.GetString("MessageWindow.Errors.ProfileLoad") + "\n\n" + ex.Message, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error).ShowDialog();

            }

            return new();
        }

        /// <summary>
        /// Serializes a Manager Setting CFG (JSON) file.
        /// </summary>
        /// <param name="path"></param>
        public void Serialize(string path)
        {
            try
            {
                App.CreateConfigFolder();
                string jsonContent = JsonSerializer.Serialize(this, new JsonSerializerOptions { WriteIndented = true });
                File.WriteAllText(path, jsonContent);
            }
            catch
            {

            }
        }
    }
}

