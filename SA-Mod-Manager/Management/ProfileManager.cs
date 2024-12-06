using System;
using System.IO;
using System.Linq;
using System.Text;
using SAModManager.Configuration;
using SAModManager.Configuration.SA2;
using SAModManager.Configuration.SADX;
using SAModManager.Ini;
using SAModManager.UI;
using SAModManager.Profile;

namespace SAModManager.Management
{
    /// <summary>
    /// Static Class to manage the App Profiles.
    /// </summary>
    static public class ProfileManager
    {
		private static string ProfilesDirectory { 
			get 
			{
				if (App.CurrentGame.modDirectory == null)
					return ".profiles";
				else
					return Path.Combine(App.CurrentGame.modDirectory, ".profiles"); 
			}
		} 

		/// <summary>
		/// Checks if the Game Profile directory exists. Tries to create it if it doesn't. 
		/// </summary>
		/// <returns>Returns True when directory exists or has been created otherwise returns False.</returns>
		private static bool CheckProfileDirectory()
		{
			if (!Directory.Exists(ProfilesDirectory))
			{
				try
				{
					Util.CreateSafeDirectory(ProfilesDirectory);
					return true;
				}
				catch (Exception ex)
				{
					ExceptionHandler exception = new (ex);
					exception.ShowDialog();
					return false;
				}
			}
			return true;
		}

		/// <summary>
		/// Gets the profile path. Will create the directory if it doesn't exist.
		/// </summary>
		/// <returns>Full path to currently selected game's Profiles.json file.</returns>
        private static string GetProfilePath()
        {
			if (string.IsNullOrEmpty(ProfilesDirectory) && !string.Equals(".profiles", ProfilesDirectory))
                return null;

            return Path.Combine(ProfilesDirectory, "Profiles.json");
        }

        /// <summary>
        /// Checks to ensure an index is within the bounds of the App's Profile List.
        /// </summary>
        /// <param name="index"></param>
        /// <returns>Returns True if index is within the profile count, False if not.</returns>
        private static bool CheckIndexBounds(int index)
        {
            if (!(index > App.Profiles.ProfilesList.Count) && !(index < 0))
                return true;
            else
                return false;
        }

        /// <summary>
        /// Converts Old Loader File to new GameSettings format.
        /// </summary>
        /// <param name="sourceFile"></param>
        /// <param name="destFile"></param>
        private static void ConvertProfile(string sourceFile, string destFile)
        {
            sourceFile = Path.GetFullPath(sourceFile);

			switch (App.CurrentGame.id)
			{
				case SetGame.SADX:
					Configuration.SADX.GameSettings sadxSettings = new();
					SADXLoaderInfo sadxLoader = IniSerializer.Deserialize<SADXLoaderInfo>(sourceFile);
                    sadxSettings.ConvertFromV0(sadxLoader);
					sadxSettings.Serialize(destFile);
					break;
				case SetGame.SA2:
					Configuration.SA2.GameSettings sa2Settings = new();
					SA2LoaderInfo sa2Loader = IniSerializer.Deserialize<SA2LoaderInfo>(sourceFile);
					sa2Settings.ConvertFromV0(sa2Loader);
					sa2Settings.Serialize(destFile);
					break;
			}
		}

        /// <summary>
        /// Saves the App Profile's file.
        /// </summary>
        public static void SaveProfiles()
        {
            App.Profiles.Serialize(GetProfilePath());
        }

        /// <summary>
        /// Add a profile to the App's Profiles List.
        /// </summary>
        /// <param name="name"></param>
        public static void AddProfile(string name)
        {
            App.Profiles.ProfilesList.Add(new ProfileEntry(name, name + ".json"));
        }

		/// <summary>
		/// Adds a new profile and writes the settings file.
		/// </summary>
		/// <param name="name"></param>
		/// <param name="gameSettings"></param>
		public static void AddNewProfile(string name, object gameSettings)
		{
			switch (App.CurrentGame.id)
			{
				case SetGame.SADX:
					Configuration.SADX.GameSettings sadxSettings = gameSettings as Configuration.SADX.GameSettings;
					sadxSettings.Serialize(name);
					break;
				case SetGame.SA2:
					Configuration.SA2.GameSettings sa2Settings = gameSettings as Configuration.SA2.GameSettings;
					sa2Settings.Serialize(name);
					break;
			}

			AddProfile(name);
		}

		/// <summary>
		/// Renames a profile in the Profiles List.
		/// </summary>
		/// <param name="name"></param>
		public static void RenameProfile(string oldname, string newname)
		{
			foreach (ProfileEntry entry in App.Profiles.ProfilesList.ToList<ProfileEntry>())
			{
				if (entry.Name == oldname)
				{
					string oldFilePath = Path.Combine(App.CurrentGame.ProfilesDirectory, oldname + ".json");
					string newFilePath = Path.Combine(App.CurrentGame.ProfilesDirectory, newname + ".json");

					entry.Name = newname;
					entry.Filename = newname + ".json";

					File.Move(oldFilePath, newFilePath);
				}
			}
		}

        /// <summary>
        /// Remove a profile from the App's Profile List by name and delete the source file.
        /// </summary>
        /// <param name="name"></param>
        public static void RemoveProfile(string name)
        {
            foreach (ProfileEntry entry in App.Profiles.ProfilesList.ToList<ProfileEntry>())
            {
                if (entry.Name == name)
				{
					string filePath = Path.Combine(App.CurrentGame.ProfilesDirectory, entry.Filename);
                    if (File.Exists(filePath))
                    {
						try
						{
							File.Delete(filePath);
						}
						catch
						{
							throw new Exception("Failed to delete profile.");
						}
                    }

                    App.Profiles.ProfilesList.Remove(entry);
				}
            }
        }

        /// <summary>
        /// Remove a profile from the App's Profile List by index.
        /// </summary>
        /// <param name="index"></param>
        public static void RemoveProfile(int index)
        {
            if (CheckIndexBounds(index))
                App.Profiles.ProfilesList.RemoveAt(index);
        }

        /// <summary>
        /// Sets the App's Profiles.
        /// </summary>
        public static void SetProfile()
        {
            App.Profiles = Profiles.Deserialize(GetProfilePath());
        }

		/// <summary>
		/// Migrates a single profile from the supplied path.
		/// </summary>
		/// <param name="path">Path to the profile.</param>
		/// <returns>True when successful, otherwise false.</returns>
		public static bool MigrateProfile(string path)
		{
            if (File.Exists(path))
            {
                string sourceFile = path;
                string destFile = Path.GetFileNameWithoutExtension(sourceFile) == App.CurrentGame.loader?.name ? "Default.json" : Path.GetFileNameWithoutExtension(sourceFile) + ".json";

                File.Copy(Path.GetFullPath(path), Path.Combine(App.CurrentGame.ProfilesDirectory, destFile), true);
                ConvertProfile(sourceFile, destFile);
                return true;
            }
            else
            {
                return false;
            }
		}

		/// <summary>
		/// Automatically migrates any profiles found in the Game Install>mods directory to the new GameSettings format.
		/// Set deletesource to true if you want to delete the source file when conversion is completed.
		/// </summary>
		public static void MigrateProfiles(bool deletesource = false)
        {
            if (string.IsNullOrEmpty(App.CurrentGame.gameDirectory) == false)
            {
                string modPath = Path.Combine(App.CurrentGame.gameDirectory, "mods");
                Util.CreateSafeDirectory(modPath);

                if (CheckProfileDirectory() && Directory.Exists(modPath))
                {
                    foreach (var item in Directory.EnumerateFiles(modPath, "*.ini"))
                    {
						string itempath = Path.GetFullPath(item);
                        
						if (!MigrateProfile(itempath))
						{
							StringBuilder sb = new StringBuilder();
							sb.AppendLine(Lang.GetString("MessageWindow.Warnings.FirstMigrationFail.Message1"));
							sb.AppendLine();
							sb.AppendLine(Lang.GetString("MessageWindow.Warnings.FirstMigrationFail.Message2"));
							MessageWindow msg = new MessageWindow(
								windowName: Lang.GetString("MessageWindow.Warnings.FirstMigrationFail.Title"),
								ErrorText: sb.ToString());

							msg.ShowDialog();

							if (msg.isClosed)
							{
								string destFile = Path.GetFileNameWithoutExtension(itempath) == App.CurrentGame.loader?.name ? "Default.json" : Path.GetFileNameWithoutExtension(item) + ".json";

								switch (App.CurrentGame.id)
								{
									case SetGame.SADX:
										Configuration.SADX.GameSettings sadxSettings = new();
										sadxSettings.Serialize(destFile);
										break;
									case SetGame.SA2:
										Configuration.SA2.GameSettings sa2Settings = new();
										sa2Settings.Serialize(destFile);
										break;
								}
							}
						}
						if (deletesource)
							File.Delete(itempath);
					}
                }
            }
        }

        /// <summary>
        /// This should only ever be run a single time on boot/initial install for a game.
        /// The MigrateProfiles function is public and should be used in any other instance of mass migrating.
        /// </summary>
        public static void CreateProfiles()
        {
            MigrateProfiles(false);

            Profiles profiles = new();

            if (App.CurrentGame.ProfilesDirectory is not null)
            {
                foreach (var item in Directory.EnumerateFiles(App.CurrentGame.ProfilesDirectory, "*.json"))
                {
                    if (Path.GetFileName(item) != "Profiles.json")
                        profiles.ProfilesList.Add(new ProfileEntry(Path.GetFileNameWithoutExtension(item), Path.GetFileName(item)));
                }

                if (profiles.ProfilesList.Count <= 0) 
                {
                    profiles = Profiles.MakeDefaultProfileFile();
                }

                profiles.Serialize(GetProfilePath());
               
            }

            App.Profiles = profiles;
        }

        /// <summary>
        /// Gets currently selected Profile as a ProfileEntry.
        /// </summary>
        /// <returns></returns>
        public static ProfileEntry GetCurrentProfile()
        {
            if (App.Profiles.ProfileIndex < App.Profiles.ProfilesList.Count)
                return App.Profiles.ProfilesList[App.Profiles.ProfileIndex];
            else
                return App.Profiles.ProfilesList[0];
        }

        /// <summary>
        /// Gets a profile from a specific index, returns null if index is out of bounds.
        /// </summary>
        /// <param name="index"></param>
        /// <returns></returns>
        public static ProfileEntry GetProfile(int index)
        {
            if (CheckIndexBounds(index))
                return App.Profiles.ProfilesList[index];
            else
                return null;
        }

		/// <summary>
		/// Validates that the Profile index is within the boundaries of the list. Resets it to 0 if it is not in bounds.
		/// </summary>
        public static void ValidateProfileIndex()
        {
            if (App.Profiles.ProfileIndex < 0 || App.Profiles.ProfileIndex > App.Profiles.ProfilesList.Count)
                App.Profiles.ProfileIndex = 0;
        }

		/// <summary>
		/// Validates all profiles for the currently selected game.
		/// </summary>
        public static void ValidateProfiles()
        {
            ValidateProfileIndex();
            App.Profiles.ValidateProfiles();
        }
    }
}
