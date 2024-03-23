using SAModManager.Configuration.SA2;
using SAModManager.Configuration.SADX;
using SAModManager.Ini;
using SAModManager.UI;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAModManager.Configuration
{
	/// <summary>
	/// Static Class to manage the App Profiles.
	/// </summary>
	static public class ProfileManager
	{
		private static string GetProfilePath()
		{
			return Path.Combine(App.CurrentGame.ProfilesDirectory, "Profiles.json");
		}

		/// <summary>
		/// Checks to ensure an index is within the bounds of the App's Profile List.
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
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
			sourceFile = Path.Combine(App.CurrentGame.gameDirectory, "mods", sourceFile);

			try
			{
				switch (App.CurrentGame.id)
				{
					case SetGame.SADX:
						SADX.GameSettings sadxSettings = new();
						SADXLoaderInfo sadxLoader = IniSerializer.Deserialize<SADXLoaderInfo>(sourceFile);
						sadxSettings.ConvertFromV0(sadxLoader);
						sadxSettings.Serialize(App.CurrentGame.ProfilesDirectory, destFile);
						break;
					case SetGame.SA2:
						SA2.GameSettings sa2Settings = new();
						SA2LoaderInfo sa2Loader = IniSerializer.Deserialize<SA2LoaderInfo>(sourceFile);
						sa2Settings.ConvertFromV0(sa2Loader);
						sa2Settings.Serialize(App.CurrentGame.ProfilesDirectory, destFile);
						break;
				}
			}
			catch (Exception ex)
			{
				//new ExceptionHandler(ex);
				return;
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
		/// Remove a profile from the App's Profile List by name.
		/// </summary>
		/// <param name="name"></param>
		public static void RemoveProfile(string name)
		{
			foreach (ProfileEntry entry in App.Profiles.ProfilesList)
			{
				if (entry.Name == name)
					App.Profiles.ProfilesList.Remove(entry);
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
		/// Automatically migrates any profiles found in the Game Install>mods directory to the new GameSettings format.
		/// Set deletesource to true if you want to delete the source file when conversion is completed.
		/// </summary>
		public static void MigrateProfiles(bool deletesource = false)
		{
			if (App.CurrentGame.gameDirectory != "")
			{
				if (Directory.Exists(App.CurrentGame.gameDirectory))
				{
					if (!Directory.Exists(App.CurrentGame.ProfilesDirectory))
						Directory.CreateDirectory(App.CurrentGame.ProfilesDirectory);

					foreach (var item in Directory.EnumerateFiles(Path.Combine(App.CurrentGame.gameDirectory, "mods"), "*.ini"))
					{
						string sourceFile = Path.GetFileName(item);
						string destFile = 
							Path.GetFileNameWithoutExtension(sourceFile) == App.CurrentGame.loader.name ? "Default.json" : Path.GetFileNameWithoutExtension(sourceFile) + ".json";

						File.Copy(Path.GetFullPath(item), Path.Combine(App.CurrentGame.ProfilesDirectory, destFile), true);
						ConvertProfile(sourceFile, destFile);

						if (deletesource)
							File.Delete(Path.GetFullPath(sourceFile));
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

			Profiles profiles = new Profiles();

			foreach (var item in Directory.EnumerateFiles(App.CurrentGame.ProfilesDirectory, "*.json"))
			{
				if (Path.GetFileName(item) != "Profiles.json")
					profiles.ProfilesList.Add(new ProfileEntry(Path.GetFileNameWithoutExtension(item), Path.GetFileName(item)));
			}

			profiles.Serialize(GetProfilePath());

			App.Profiles = profiles;
		}

		/// <summary>
		/// Gets currently selected Profile as a ProfileEntry.
		/// </summary>
		/// <returns></returns>
		public static ProfileEntry GetCurrentProfile()
		{
			return App.Profiles.ProfilesList[App.Profiles.ProfileIndex];
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

		public static void ValidateProfileIndex()
		{
			if (App.Profiles.ProfileIndex < 0 || App.Profiles.ProfileIndex > App.Profiles.ProfilesList.Count)
				App.Profiles.ProfileIndex = 0;
		}

		public static void ValidateProfiles()
		{
			ValidateProfileIndex();
			App.Profiles.ValidateProfiles();
		}
	}
}
