using Newtonsoft.Json.Bson;
using SAModManager.Common;
using SAModManager.Ini;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.Json;

namespace SAModManager.Configuration
{
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

		public void ValidateProfiles()
		{
			if (!string.IsNullOrEmpty(App.CurrentGame.ProfilesDirectory))
			{
				List<ProfileEntry> list = new();

				foreach (ProfileEntry entry in ProfilesList)
				{
					if (!File.Exists(Path.Combine(App.CurrentGame.ProfilesDirectory, entry.Filename)))
					{
						list.Add(entry);
					}
				}

				if (list.Count > 0)
				{
					StringBuilder error = new();
					error.AppendLine(Lang.GetString("MessageWindow.Warnings.ProfilesDeleted.Message1"));
					for (int i = 0; i < list.Count; i++)
						error.AppendLine(list[i].Name);
					error.AppendLine(Lang.GetString("MessageWindow.Warnings.ProfilesDeleted.Message2"));

					MessageWindow message = new(Lang.GetString("MessageWindow.Warnings.ProfilesDeleted.Title"), 
						error.ToString(), icon: MessageWindow.Icons.Warning);
					message.ShowDialog();
					if (message.isClosed)
					{
						foreach (ProfileEntry entry in list)
							ProfilesList.Remove(entry);

						ProfileIndex = 0;
					}
				}
			}
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
