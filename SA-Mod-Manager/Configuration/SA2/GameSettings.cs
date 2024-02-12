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
        public enum GameLanguage
        {
            Japanese = 0,
            English = 1,
            French = 2,
            Spanish = 3,
            German = 4
        }

        /// <summary>
        /// Enables Character options when launching.
        /// </summary>
        [DefaultValue(false)]
        public bool UseCharacter { get; set; } = false;

        /// <summary>
        /// Enables Player 2 options when launching.
        /// </summary>
        [DefaultValue(false)]
        public bool UsePlayer2 { get; set; } = false;

        /// <summary>
        /// Enables the Level, Act, and Time of Day options when launching.
        /// </summary>
        [DefaultValue(false)]
        public bool UseLevel { get; set; } = false;

        /// <summary>
        /// Enables the Event options when launching.
        /// </summary>
        [DefaultValue(false)]
        public bool UseEvent { get; set; } = false;

        /// <summary>
        /// Enables the Save options when launching.
        /// </summary>
        [DefaultValue(false)]
        public bool UseSave { get; set; } = false;

        /// <summary>
        /// Selected index for the level used by test spawn.
        /// </summary>
        [DefaultValue(-1)]
        public int LevelIndex { get; set; } = -1;       // SADXLoaderInfo.TestSpawnLevel

        /// <summary>
        /// Selected index for the act used by test spawn.
        /// </summary>
        [DefaultValue(0)]
        public int MissionIndex { get; set; } = 0;          // SA2LoaderInfo.TestSpawnAct

        /// <summary>
        /// Selected index for the character used by test spawn.
        /// </summary>
        [DefaultValue(-1)]
        public int CharacterIndex { get; set; } = -1;   
        
        // SA2LoaderInfo.TestSpawnPlayer2
        /// Selected index for the player 2 used by test spawn.
        /// </summary>
        [DefaultValue(-1)]
        public int Player2Index { get; set; } = -1;   // SA2LoaderInfo.TestSpawnPlayer2

        /// <summary>
        /// Selected index of an event used by test spawn.
        /// </summary>
        [DefaultValue(-1)]
        public int EventIndex { get; set; } = -1;       // SADXLoaderInfo.TestSpawnEvent


        /// <summary>
        /// Selected save file index used by test spawn.
        /// </summary>
        [DefaultValue(-1)]
        public int SaveIndex { get; set; } = -1;      // SADXLoaderInfo.TestSpawnSaveID

        /// <summary>
        /// Sets the game's Text Language.
        /// </summary>
        [DefaultValue((int)GameLanguage.English)]
        public int GameTextLanguage { get; set; } = 1;      // SADXLoaderInfo.TextLanguage

        /// <summary>
        /// Sets the game's Voice Language.
        /// </summary>
        [DefaultValue((int)GameLanguage.English)]
        public int GameVoiceLanguage { get; set; } = 1;     // SADXLoaderInfo.VoiceLanguage

        /// <summary>
        /// Enables the Manual settings for Character, Level, and Act.
        /// </summary>
        [DefaultValue(false)]
        public bool UseManual { get; set; } = false;

        /// <summary>
        /// Enables manually modifying the start position when using test spawn.
        /// </summary>
        [DefaultValue(false)]
        public bool UsePosition { get; set; } = false; // SADXLoaderInfo.TestSpawnPositionEnabled

        /// <summary>
        /// X Position where the player will spawn using test spawn.
        /// </summary>
        [DefaultValue(0f)]
        public float XPosition { get; set; } = 0f;            // SADXLoaderInfo.TestSpawnX

        /// <summary>
        /// Y Position where the player will spawn using test spawn.
        /// </summary>
        [DefaultValue(0f)]
        public float YPosition { get; set; } = 0f;            // SADXLoaderInfo.TestSpawnY

        /// <summary>
        /// Z Position where the player will spawn using test spawn.
        /// </summary>
        [DefaultValue(0f)]
        public float ZPosition { get; set; } = 0f;            // SADXLoaderInfo.TestSpawnZ

        /// <summary>
        /// Initial Y Rotation for the player when using test spawn.
        /// </summary>
        [DefaultValue(0)]
        public int Rotation { get; set; } = 0;     // SADXLoaderInfo.TestSpawnRotation

        /// <summary>
        /// Converts from original settings file.
        /// </summary>
        /// <param name="oldSettings"></param>
        public void ConvertFromV0(SADXLoaderInfo oldSettings)
        {
            LevelIndex = oldSettings.TestSpawnLevel;
            MissionIndex = oldSettings.TestSpawnAct;
            CharacterIndex = oldSettings.TestSpawnCharacter;
            EventIndex = oldSettings.TestSpawnEvent;
            SaveIndex = oldSettings.TestSpawnEvent;

            GameTextLanguage = oldSettings.TextLanguage;
            GameVoiceLanguage = oldSettings.VoiceLanguage;

            UsePosition = oldSettings.TestSpawnPositionEnabled;

            XPosition = oldSettings.TestSpawnX;
            YPosition = oldSettings.TestSpawnY;
            ZPosition = oldSettings.TestSpawnZ;
            Rotation = oldSettings.TestSpawnRotation;
        }
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
		/// Versioning for the SA2 Settings file.
		/// </summary>
		[DefaultValue((int)SA2SettingsVersions.v1)]
		public int SettingsVersion { get; set; } = (int)SA2SettingsVersions.v1;

		/// <summary>
		/// Graphics Settings for SA2.
		/// </summary>
		public GraphicsSettings Graphics { get; set; } = new();

		/// <summary>
		/// Controller Settings for SA2.
		/// </summary>
		public ControllerSettings Controller { get; set; } = new();

		/// <summary>
		/// Sound Settings for SA2.
		/// </summary>
		public SoundSettings Sound { get; set; } = new();

		/// <summary>
		/// TestSpawn Settings for SA2.
		/// </summary>
		public TestSpawnSettings TestSpawn { get; set; } = new();

		/// <summary>
		/// Patches for SA2.
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
		/// Enabled Mods for SA2.
		/// </summary>
		[IniName("Mod")]
		[IniCollection(IniCollectionMode.NoSquareBrackets, StartIndex = 1)]
		public List<string> EnabledMods { get; set; }       // SA2LoaderInfo.Mods

		/// <summary>
		/// Enabled Codes for SA2.
		/// </summary>
		[IniName("Code")]
		[IniCollection(IniCollectionMode.NoSquareBrackets, StartIndex = 1)]
		public List<string> EnabledCodes { get; set; }      // SA2LoaderInfo.EnabledCodes

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
        /// Serializes an SA2 GameSettings JSON File.
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
