using System.Collections.Generic;
using System.ComponentModel;
using SAModManager.Ini;
using SAModManager.Configuration;
using System.IO;
using System.Text.Json;

// TODO: Build SA2 Game Settings
namespace SAModManager.Configuration.SA2
{
	public class GraphicsSettings
	{

	}

	public class ControllerSettings
	{

	}

	public class SoundSettings
	{

	}

	public class TestSpawnSettings
	{

	}

	public class GamePatches
	{

	}

	public class GameSettings
	{
		/// <summary>
		/// Versioning.
		/// </summary>
		public enum SA2SettingsVersions
		{
			v0 = 0,
			v1 = 1
		}

		/// <summary>
		/// Versioning for the SADX Settings file.
		/// </summary>
		[DefaultValue((int)SA2SettingsVersions.v1)]
		public int SettingsVersion { get; set; } = (int)SA2SettingsVersions.v1;

		/// <summary>
		/// Graphics Settings for SADX.
		/// </summary>
		public GraphicsSettings Graphics { get; set; } = new();

		/// <summary>
		/// Controller Settings for SADX.
		/// </summary>
		public ControllerSettings Controller { get; set; } = new();

		/// <summary>
		/// Sound Settings for SADX.
		/// </summary>
		public SoundSettings Sound { get; set; } = new();

		/// <summary>
		/// TestSpawn Settings for SADX.
		/// </summary>
		public TestSpawnSettings TestSpawn { get; set; } = new();

		/// <summary>
		/// Patches for SADX.
		/// </summary>
		public GamePatches Patches { get; set; } = new();

		/// <summary>
		/// Debug Settings.
		/// </summary>
		public DebugSettings DebugSettings { get; set; } = new();

		/// <summary>
		/// Path to the game install saved with this configuration.
		/// </summary>
		public string GamePath { get; set; } = string.Empty;

		/// <summary>
		/// Enabled Mods for SADX.
		/// </summary>
		[IniName("Mod")]
		[IniCollection(IniCollectionMode.NoSquareBrackets, StartIndex = 1)]
		public List<string> EnabledMods { get; set; }       // SADXLoaderInfo.Mods

		/// <summary>
		/// Enabled Codes for SADX.
		/// </summary>
		[IniName("Code")]
		[IniCollection(IniCollectionMode.NoSquareBrackets, StartIndex = 1)]
		public List<string> EnabledCodes { get; set; }      // SADXLoaderInfo.EnabledCodes

		/// <summary>
		/// Converts from original settings file.
		/// </summary>
		/// <param name="oldSettings"></param>
		public void ConvertFromV0(SADXLoaderInfo oldSettings)
		{

		}

		/// <summary>
		/// Converts the current GameSettings info back to the Loader ini's required format.
		/// </summary>
		/// <param name="managerSettings"></param>
		/// <returns></returns>
		private void ConvertToLoaderInfo(ManagerSettings managerSettings)
		{
			// TODO: Make this return SA2LoaderInfo.
		}

		/// <summary>
		/// Writes to the Loader's necessary ini file. Path is to the Mod's Directory.
		/// </summary>
		/// <param name="path"></param>
		public void WriteToLoaderInfo(string path, ManagerSettings managerSettings)
		{
			
		}

		public static GameSettings Deserialize(string path)
		{
			try
			{
				if (File.Exists(path))
				{
					string jsonContent = File.ReadAllText(path);

					return JsonSerializer.Deserialize<GameSettings>(jsonContent);
				}
				else
					return new();
			}
			catch
			{
                return new();
            }
		}

        /// <summary>
        /// Serializes an SADX GameSettings JSON File.
        /// </summary>
        /// <param name="path"></param>
        public void Serialize(string path, string profileName)
        {

            try
            {
                if (Directory.Exists(App.CurrentGame.ProfilesDirectory))
                {
                    string jsonContent = JsonSerializer.Serialize(this, new JsonSerializerOptions { WriteIndented = true });
                    File.WriteAllText(path, jsonContent);
                }
                else
                {
                    App.CurrentGame.ProfilesDirectory = Path.Combine(App.ConfigFolder, App.CurrentGame.gameAbbreviation);
                    Directory.CreateDirectory(App.CurrentGame.ProfilesDirectory);
                    if (Directory.Exists(App.CurrentGame.ProfilesDirectory))
                    {
                        path = Path.Combine(App.CurrentGame.ProfilesDirectory, profileName);
                        string jsonContent = JsonSerializer.Serialize(this, new JsonSerializerOptions { WriteIndented = true });
                        File.WriteAllText(path, jsonContent);
                    }
                }
            }
            catch
            {

            }
        }
    }
}
