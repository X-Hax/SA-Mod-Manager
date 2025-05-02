using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SAModManager.Configuration;
using System.IO;
using SAModManager;
using System.Windows;
using System.Runtime.CompilerServices;

namespace SAModManager.Management
{
	/// <summary>
	/// Static Management class for the <see cref="ManagerSettings"/> used in the application.
	/// </summary>
    public static class SettingsManager
    {
		/// <summary>
		/// Path to the SAManager folder where the <see cref="ManagerSettings"/> are stored.
		/// </summary>
		private static string SettingsFolder 
		{ 
			get
			{
				if (PortableModeEnabled)
				{
					return Path.Combine(App.StartDirectory, "SAManager");
				}
				else
				{
					return  Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "SAManager");
				}
			}
		}

		/// <summary>
		/// Path to the Settings File, the serialized file for storing <see cref="ManagerSettings"/>.
		/// </summary>
		private static string SettingsFile
		{
			get { return Path.Combine(SettingsFolder, "Manager.json"); }
		}

		/// <summary>
		/// Bool for determining if Portable Mode is enabled.
		/// 
		/// If enabled, there is an "SAManager" folder in the same folder as the Mod Manager. 
		/// </summary>
		private static bool PortableModeEnabled
		{
			get { return Directory.Exists(Path.Combine(App.StartDirectory, "SAManager")) ? true : false; }
		}

		/// <summary>
		/// Initializes the <see cref="SettingsManager"/> and loads the <see cref="ManagerSettings"/> file.
		/// </summary>
		public static void InitializeSettingsManager()
		{
			App.ManagerSettings = ManagerSettings.Deserialize(SettingsFile);
		}

		/// <summary>
		/// Saves the <see cref="ManagerSettings"/> to the correct location based.
		/// </summary>
		public static void SaveSettings()
		{
			App.ManagerSettings.GameEntries.Clear();
			foreach (var game in App.GamesList)
			{
				if (game.id != GameEntry.GameType.Unsupported)
					App.ManagerSettings.GameEntries.Add(new GameEntry(game));
			}

			foreach (var game in App.ManagerSettings.GameEntries)
			{
				if (game.Type == App.CurrentGame.id)
				{
                    App.ManagerSettings.CurrentSetGame = App.ManagerSettings.GameEntries.IndexOf(game);
                }
			}

            App.ManagerSettings.Serialize(SettingsFile);
		}

		public static GameEntry GetCurrentGame()
		{
			int index = App.ManagerSettings.CurrentSetGame;
			if (index < 0 || index >= App.ManagerSettings.GameEntries.Count)
                App.ManagerSettings.CurrentSetGame = 0;

            return App.ManagerSettings.GameEntries[App.ManagerSettings.CurrentSetGame];
		}

		/// <summary>
		/// Gets the Settings Folder directory.
		/// </summary>
		public static string GetSettingsFolder() { return SettingsFolder; }

		/// <summary>
		/// Gets the path to the Settings File ("Manager.json")
		/// </summary>
		public static string GetSettingsFile() { return SettingsFile; }

		/// <summary>
		/// Returns the current state of the Portable Mode bool.
		/// </summary>
		public static bool IsPortableMode() { return PortableModeEnabled; }

		/// <summary>
		/// Deletes all files within the <see cref="SettingsFolder"/> along with the folder itself. 
		/// 
		/// Requires a program reboot when run.
		/// </summary>
		public static void DeleteSettings()
		{
			Directory.Delete(SettingsFolder, true);
		}

		/// <summary>
		/// Copies the data from the global or local folder based on if the application was already in Portable mode or not.
		/// 
		/// Use whenever switching between Global and Portable mode to ensure settings are properly handled.
		/// </summary>
		public static void ProcessPortableMode()
		{
			string globalSettingsFolder = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "SAManager");
			string localSettingsFolder = Path.Combine(App.StartDirectory, "SAManager");

			if (!IsPortableMode())
				Util.CopyAllFiles(globalSettingsFolder, localSettingsFolder);
			else
			{
				Util.CopyAllFiles(localSettingsFolder, globalSettingsFolder);
				DeleteSettings();
			}
        }

		/// <summary>
		/// Packages the setting related json files from the <see cref="SettingsFolder"/> into a 7z that can be imported.
		/// </summary>
		/// <param name="destination">Output path for the 7z archive.</param>
		public static void PackageSettings(string destination)
		{
			// TODO: Implement this.
		}

		/// <summary>
		/// Imports a 7z archive containing the json setting files for the <see cref="SettingsFolder"/>.
		/// 
		/// Requires a reboot for the imported settings to take effect.
		/// </summary>
		/// <param name="source">Path to the 7z to be loaded.</param>
		public static void ImportSettings(string source)
		{
			// TODO: Implement this.
		}

		/// <summary>
		/// Saves the Manager's directory to a text file in the current game's .modloader folder.
		/// </summary>
		public static void SaveAppDirectoryInfo()
		{
			if (App.CurrentGame.id != GameEntry.GameType.Unsupported)
			{
				if (!Directory.Exists(App.CurrentGame.modLoaderDirectory))
					return;

				string destFile = Path.Combine(App.CurrentGame.modLoaderDirectory, "samanager.txt");

				File.WriteAllText(destFile, App.StartDirectory);
			}
		}
    }
}
