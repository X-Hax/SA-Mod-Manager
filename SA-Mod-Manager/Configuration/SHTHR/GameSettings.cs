using SAModManager.Configuration.SADX;
using SAModManager.Ini;
using SAModManager.Management;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Text.Json;


namespace SAModManager.Configuration.SHTHR
{

    public class TestSpawnSettings // to do maybe add language, android character, 2p, position and rotation.
    {

        /// <summary>
        /// Enables Level option when launching.
        /// </summary>
        [DefaultValue(false)]
        public bool UseLevel { get; set; } = false;

        /// <summary>
        /// Enables the Event options when launching.
        /// </summary>
        [DefaultValue(false)]
        public bool UseEvent { get; set; } = false;


        /// <summary>
        /// Selected index for the level used by test spawn.
        /// </summary>
        [DefaultValue(-1)]
        public int LevelIndex { get; set; } = -1;       // SHTHRLoaderInfo.TestSpawnLevel

        
        /// <summary>
        /// Selected index of an event used by test spawn.
        /// </summary>
        [DefaultValue(-1)]
        public int EventIndex { get; set; } = -1;       // SHTHRLoaderInfo.TestSpawnEvent

    }

    public class GameSettings
    {
        /// <summary>
        /// Versioning.
        /// </summary>
        public enum SHTHRSettingsVersions
        {
            v0,     // Version 0: Original LoaderInfo Version
            v1, // Version 1: Add Test Spawn
            MAX,    // Do Not Modify, new versions are placed above this.
        }

        /// <summary>
        /// Versioning for the SHTHR Settings file.
        /// </summary>
        [DefaultValue((int)(SHTHRSettingsVersions.MAX - 1))]
        public int SettingsVersion { get; set; } = (int)(SHTHRSettingsVersions.MAX - 1);


        /// <summary>
        /// TestSpawn Settings for SHTHR.
        /// </summary>
        public TestSpawnSettings TestSpawn { get; set; } = new();

        /// <summary>
        /// Path to the game install saved with this configuration.
        /// </summary>
        public string GamePath { get; set; } = string.Empty;

        /// <summary>
        /// Enabled Mods for SHTHR.
        /// </summary>
        [IniName("Mod")]
        [IniCollection(IniCollectionMode.NoSquareBrackets, StartIndex = 1)]
        public List<string> EnabledMods { get; set; }       // SHTHRLoaderInfo.Mods

        /// <summary>
        /// Enabled Codes for SHTHR.
        /// </summary>
        [IniName("Code")]
        [IniCollection(IniCollectionMode.NoSquareBrackets, StartIndex = 1)]
        public List<string> EnabledCodes { get; set; }      // SHTHRLoaderInfo.EnabledCodes

        public List<string> ModsList { get; set; } = new();   // used for consistent mod order option



        /// <summary>
        /// Deserializes an SHTHR GameSettings JSON File and returns a populated class.
        /// </summary>
        /// <param name="path"></param>
        /// <returns></returns>
        public static GameSettings Deserialize(string path)
        {
            try
            {
                if (File.Exists(path))
                {
                    string jsonContent = File.ReadAllText(path);

                    GameSettings settings = JsonSerializer.Deserialize<GameSettings>(jsonContent);

                    // This is where the settings versions get bumped on load.
                    // Set in a switch case in the event we need to make changes on a specific version.
                    switch (settings.SettingsVersion)
                    {
                        default:
                            if (settings.SettingsVersion < (int)(SHTHRSettingsVersions.MAX - 1))
                                settings.SettingsVersion = (int)(SHTHRSettingsVersions.MAX - 1);
                            break;
                    }

                    return settings;
                }
                else
                    return new();
            }
            catch
            {
                return new();
            }
        }

        // Codes.json is from the Manager so we can rewrite it whenever it differs from the embedded one.
        public static void ExtractCodesFile(string codesPath)
        {
            try
            {
                if (Util.IsStringValid(codesPath) == false || !Directory.Exists(App.CurrentGame.gameDirectory))
                    return;

                Util.CreateSafeDirectory(Path.GetDirectoryName(codesPath));

                byte[] codes = Properties.Resources.SHTHR_Codes;

                if (File.Exists(codesPath) && File.ReadAllBytes(codesPath).AsSpan().SequenceEqual(codes))
                    return;

                File.WriteAllBytes(codesPath, codes);
            }
            catch
            {

            }
        }

        /// <summary>
        /// Serializes an SHTHR GameSettings JSON File.
        /// </summary>
        /// <param name="path"></param>
        public void Serialize(string profileName)
        {
            if (!profileName.Contains(".json"))
                profileName += ".json";


            string path = Path.Combine(ProfileManager.GetProfilesDirectory(), profileName);
            try
            {
                if (Directory.Exists(ProfileManager.GetProfilesDirectory()))
                {
                    string jsonContent = JsonSerializer.Serialize(this, new JsonSerializerOptions { WriteIndented = true });
                    File.WriteAllText(path, jsonContent);
                }
            }
            catch
            {

            }
        }
    }
}