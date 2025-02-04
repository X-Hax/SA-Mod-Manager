using Newtonsoft.Json.Bson;
using SAModManager.Ini;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using SAModManager.UI;
using System.Collections.ObjectModel;
using SAModManager.Management;
using System.Text.Json.Serialization;

namespace SAModManager.Profile
{
    public class ProfileEntry
    {
        public string Name { get; set; } = string.Empty;

        public string Filename { get; set; } = string.Empty;

		[JsonIgnore]
		public string Filepath { get { return Path.Combine(ProfileManager.GetProfilesDirectory(), Filename); } }

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
        public ObservableCollection<ProfileEntry> ProfilesList { get; set; } = new();

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
            try
            {
                if (File.Exists(path))
                {
                    string jsonContent = File.ReadAllText(path);

                    return JsonSerializer.Deserialize<Profiles>(jsonContent);
                }
                else
                {
                    return new()
                    {
                        ProfilesList = new ObservableCollection<ProfileEntry> { new("Default", "Default.json") }
                    };
                }
            }
            catch (Exception ex)
            {
                new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), Lang.GetString("MessageWindow.Errors.ProfileLoad") + "\n\n" + ex.Message, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error).ShowDialog();

            }

            return new() { ProfilesList = new ObservableCollection<ProfileEntry> { new("Default", "Default.json") } };
        }

        /// <summary>
        /// Serializes Profiles to JSON.
        /// </summary>
        /// <param name="path"></param>
        public void Serialize(string path)
        {
            try
            {
                if (Directory.Exists(ProfileManager.GetProfilesDirectory()))
                {
                    string jsonContent = JsonSerializer.Serialize(this, new JsonSerializerOptions { WriteIndented = true });
                    File.WriteAllText(path, jsonContent);
                }
                else
                {
                    Util.CreateSafeDirectory(ProfileManager.GetProfilesDirectory());
                    if (Directory.Exists(ProfileManager.GetProfilesDirectory()))
                    {

                        string jsonContent = JsonSerializer.Serialize(this, new JsonSerializerOptions { WriteIndented = true });
                        File.WriteAllText(path, jsonContent);
                    }
                }
            }
            catch
            {
                throw new Exception("Failed to create Profile Directory.");
            }
        }

        /// <summary>
        /// Runs a validation check on profiles to ensure that no files have been manually deleted or manually renamed. Removes any missing/renamed entries.
        /// </summary>
        public bool ValidateProfiles()
        {
            try
            {
                if (!string.IsNullOrEmpty(ProfileManager.GetProfilesDirectory()) && !string.IsNullOrWhiteSpace(ProfileManager.GetProfilesDirectory()))
                {
                    List<ProfileEntry> list = [];
                    int count = 0;
                    foreach (ProfileEntry entry in ProfilesList)
                    {
                        if (entry is not null)
                        {
                            if (!File.Exists(Path.Combine(ProfileManager.GetProfilesDirectory(), entry.Filename)))
                            {
                                list.Add(entry);
                            }
                        }
                        else
                        {
                            ProfilesList.Remove(entry);
                        }
                        count++;
                    }

                    if (list.Count == 1 && count == 1) //no profile at all
                    {
                        ProfileManager.MigrateProfiles(false);
                        return false;
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

                        return false;

                    }

                }
            }
            catch (Exception ex)
            {

                new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), Lang.GetString("MessageWindow.Errors.ProfileLoad") + "\n\n" + ex.Message, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error).ShowDialog();
                return false;
            }

            return true;
        }

        /// <summary>
        /// Creates a Default Profile file.
        /// </summary>
        /// <returns></returns>
        public static Profiles MakeDefaultProfileFile()
        {
            return new()
            {
                ProfileIndex = 0,
                ProfilesList = new ObservableCollection<ProfileEntry> { new("Default", "Default.json") }
            };
        }
    }
}
