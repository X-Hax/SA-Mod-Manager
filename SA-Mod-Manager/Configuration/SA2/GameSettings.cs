using System.Collections.Generic;
using System.ComponentModel;
using SAModManager.Ini;
using System.IO;
using System.Text.Json;
using SAModManager.UI;
using System.Text.Json.Serialization;

// TODO: Build SA2 Game Settings
namespace SAModManager.Configuration.SA2
{
    public enum SA2Lang
    {
        English = 0,
        Deutsch = 1,
        Espanol = 2,
        Francais = 3,
        Japanese = 4,
    }

    public class GraphicsSettings
    {
        public enum DisplayMode
        {
            Windowed,
            Borderless,
            CustomWindow,
            Fullscreen
        }

        /// <summary>
        /// Index for the screen the game will boot on.
        /// </summary>
        [DefaultValue(1)]
        public int SelectedScreen { get; set; } = 1;       // SA2LoaderInfo.ScreenNum

        /// <summary>
        /// Rendering Horizontal Resolution.
        /// </summary>
        [DefaultValue(640)]
        public int HorizontalResolution { get; set; } = 640;    // SA2LoaderInfo.HorizontalResolution

        /// <summary>
        /// Rendering Vertical Resolution.
        /// </summary>
        [DefaultValue(480)]
        public int VerticalResolution { get; set; } = 480;      // SA2LoaderInfo.VerticalResolution

        /// <summary>
        /// Enables the window to be paused when not focused.
        /// </summary>
        [DefaultValue(true)]
        public bool EnablePauseOnInactive { get; set; } = true;     // SA2LoaderInfo.PauseWhenInactive

        /// <summary>
        /// Sets the Width of the Custom Window Size.
        /// </summary>
        [DefaultValue(640)]
        public int CustomWindowWidth { get; set; } = 640;             // SA2LoaderInfo.WindowWidth

        /// <summary>
        /// Sets the Height of the Custom Window Size.
        /// </summary>
        [DefaultValue(480)]
        public int CustomWindowHeight { get; set; } = 480;            // SA2LoaderInfo.WindowHeight

        /// <summary>
        /// Keeps the Resolution's ratio for the Custom Window size.
        /// </summary>
        [DefaultValue(false)]
        public bool EnableKeepResolutionRatio { get; set; }     // SA2LoaderInfo.MaintainWindowAspectRatio

        /// <summary>
        /// Enables resizing of the game window.
        /// </summary>
        [DefaultValue(false)]
        public bool EnableResizableWindow { get; set; }               // SA2LoaderInfo.ResizableWindow

        /// <summary>
        /// Sets the Screen Mode (Windowed, Fullscreen, Borderless, or Custom Window)
        /// </summary>
        [DefaultValue(DisplayMode.Borderless)]
        public int ScreenMode { get; set; } = (int)DisplayMode.Borderless;

		/// <summary>
		/// Stretches the inner window (game render) to the outer window's size.
		/// </summary>
		[DefaultValue(false)]
		public bool StretchToWindow { get; set; } = false;

        /// <summary>
        /// Placed here to provide parity with the launcher as it also allows a language selection. 
        /// This will be saved and used on each boot.
        /// </summary>
        [DefaultValue((int)SA2Lang.English)]
        public int GameTextLanguage { get; set; } = (int)SA2Lang.English;

        /// <summary>
        /// Allows skipping the intro to the game.
        /// </summary>
        [DefaultValue(false)]
        public bool SkipIntro { get; set; } = false;

        /// <summary>
        /// Sets the monitor's refresh rate when in exclusive full screen mode.
        /// </summary>
        [DefaultValue(60)]
        public int RefreshRate { get; set; } = 60;

		/// <summary>
		/// Disables the Border Image from being loaded and rendered.
		/// </summary>
		[DefaultValue(false)]
		public bool DisableBorderImage { get; set; } = false;

        /// <summary>
        /// Converts from the original settings file.
        /// </summary>
        /// <param name="oldSettings"></param>
        public void ConvertFromV0(SA2LoaderInfo oldSettings)
        {
            SelectedScreen = oldSettings.ScreenNum;

            HorizontalResolution = oldSettings.HorizontalResolution;
            VerticalResolution = oldSettings.VerticalResolution;

            EnablePauseOnInactive = oldSettings.PauseWhenInactive;

            CustomWindowWidth = oldSettings.WindowWidth;
            CustomWindowHeight = oldSettings.WindowHeight;
            EnableKeepResolutionRatio = oldSettings.MaintainAspectRatio;
            EnableResizableWindow = oldSettings.ResizableWindow;
            GameTextLanguage = oldSettings.TextLanguage;
            SkipIntro = oldSettings.SkipIntro;
        }

        public void LoadGameConfig(ref SA2GameConfig config)
        {
            SelectedScreen = config.Display;
            HorizontalResolution = config.Width;
            VerticalResolution = config.Height;
            GameTextLanguage = config.Language;
            RefreshRate = config.RefreshRate;
        }

        public void LoadConfigs(ref SA2GameConfig config)
        {
            LoadGameConfig(ref config);
        }

        public void ToGameConfig(ref SA2GameConfig config)
        {
            switch ((DisplayMode)ScreenMode)
            {
                case DisplayMode.Fullscreen:
                    config.FullScreen = 1;
                    break;
                default:
                    config.FullScreen = 0;
                    break;
            }

            int screenID = SelectedScreen;
            if (screenID > 0) //adjust offset because the official launcher doesn't have "all screen support" which means first monitor is index 0.
                screenID -= 1;

            config.Display = screenID;

            config.Width = HorizontalResolution;
            config.Height = VerticalResolution;
            config.Language = GameTextLanguage;
            config.RefreshRate = RefreshRate;
        }

        public void ToConfigs(ref SA2GameConfig config)
        {
            ToGameConfig(ref config);
        }
    }

    public class TestSpawnSettings
    {
        public enum GameLanguage
        {
            English = 0,
            German,
            Spanish,
            French,
            Italian,
            Japanese
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
        public int LevelIndex { get; set; } = -1;       // SA2LoaderInfo.TestSpawnLevel

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
        public int EventIndex { get; set; } = -1;       // SA2LoaderInfo.TestSpawnEvent


        /// <summary>
        /// Selected save file index used by test spawn.
        /// </summary>
        [DefaultValue(-1)]
        public int SaveIndex { get; set; } = -1;      // SA2LoaderInfo.TestSpawnSaveID

        /// <summary>
        /// Sets the game's Text Language.
        /// </summary>
        [DefaultValue((int)GameLanguage.English)]
        public int GameTextLanguage { get; set; } = 0;      // SA2LoaderInfo.TextLanguage

        /// <summary>
        /// Sets the game's Voice Language.
        /// </summary>
        [DefaultValue(1)]
        public int GameVoiceLanguage { get; set; } = 1;     // SA2LoaderInfo.VoiceLanguage

        /// <summary>
        /// Enables the Manual settings for Character, Level, and Act.
        /// </summary>
        [DefaultValue(false)]
        public bool UseManual { get; set; } = false;

        /// <summary>
        /// Enables manually modifying the start position when using test spawn.
        /// </summary>
        [DefaultValue(false)]
        public bool UsePosition { get; set; } = false; // SA2LoaderInfo.TestSpawnPositionEnabled

        /// <summary>
        /// X Position where the player will spawn using test spawn.
        /// </summary>
        [DefaultValue(0f)]
        public float XPosition { get; set; } = 0f;            // SA2LoaderInfo.TestSpawnX

        /// <summary>
        /// Y Position where the player will spawn using test spawn.
        /// </summary>
        [DefaultValue(0f)]
        public float YPosition { get; set; } = 0f;            // SA2LoaderInfo.TestSpawnY

        /// <summary>
        /// Z Position where the player will spawn using test spawn.
        /// </summary>
        [DefaultValue(0f)]
        public float ZPosition { get; set; } = 0f;            // SA2LoaderInfo.TestSpawnZ

        /// <summary>
        /// Initial Y Rotation for the player when using test spawn.
        /// </summary>
        [DefaultValue(0)]
        public int Rotation { get; set; } = 0;     // SA2LoaderInfo.TestSpawnRotation

        /// <summary>
        /// Converts from original settings file.
        /// </summary>
        /// <param name="oldSettings"></param>
        public void ConvertFromV0(SA2LoaderInfo oldSettings)
        {
            LevelIndex = oldSettings.TestSpawnLevel;
            CharacterIndex = oldSettings.TestSpawnCharacter;
            Player2Index = oldSettings.TestSpawnPlayer2;
            EventIndex = oldSettings.TestSpawnEvent;
            SaveIndex = oldSettings.TestSpawnEvent;

            GameTextLanguage = oldSettings.TextLanguage;
            GameVoiceLanguage = oldSettings.VoiceLanguage;

            UsePosition = oldSettings.TestSpawnPositionEnabled;

            XPosition = oldSettings.TestSpawnX;
            YPosition = oldSettings.TestSpawnY;
            ZPosition = oldSettings.TestSpawnZ;
        }
    }

    public class GamePatches
    {
        [DefaultValue(true)]
        public bool FramerateLimiter { get; set; } = true;
        [DefaultValue(true)]
        public bool DisableExitPrompt { get; set; } = true;
        [DefaultValue(true)]
        public bool SyncLoad { get; set; } = true; //disable Omochao loading animation (reduce crash on startup probability)
        [DefaultValue(true)]
        public bool ExtendVertexBuffer { get; set; } = true; //incrase the vertex limit per mesh to 32k
        [DefaultValue(true)]
        public bool EnvMapFix { get; set; } = true;
        [DefaultValue(true)]
        public bool ScreenFadeFix { get; set; } = true;
        [DefaultValue(true)]
        public bool CECarFix { get; set; } = true; //intel GPU issue
        [DefaultValue(true)]
        public bool ParticlesFix { get; set; } = true; //intel GPU issue
    }

    public class GameSettings
    {
        /// <summary>
        /// Versioning.
        /// </summary>
        public enum SA2SettingsVersions
        {
            v0 = 0,		// Version 0: Original LoaderInfo Version
            v1 = 1,		// Version 1: Launch Version, functional parity with SA2GameSettings.
			v2 = 2,		// Version 2: Removed KeepAspectOnResize option, added StretchToWindow and DisableBorderImage
        }

        /// <summary>
        /// Versioning for the SA2 Settings file.
        /// </summary>
        [DefaultValue((int)SA2SettingsVersions.v2)]
        public int SettingsVersion { get; set; } = (int)SA2SettingsVersions.v2;

        /// <summary>
        /// Graphics Settings for SA2.
        /// </summary>
        public GraphicsSettings Graphics { get; set; } = new();

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
        public void ConvertFromV0(SA2LoaderInfo oldSettings)
        {
            Graphics.ConvertFromV0(oldSettings);
            TestSpawn.ConvertFromV0(oldSettings);
            DebugSettings.ConvertFromV0(oldSettings);

            SettingsVersion = (int)SA2SettingsVersions.v1;
            GamePath = App.CurrentGame.gameDirectory;
            EnabledMods = oldSettings.Mods;
            EnabledCodes = oldSettings.EnabledCodes;
        }


        /// <summary>
        /// Writes LoaderInfo and SA2Config files.
        /// </summary>
        public void WriteConfigs()
        {
            SA2GameConfig config = new SA2GameConfig();

            Graphics.ToConfigs(ref config);

            if (Directory.Exists(GamePath))
            {
                string configPath = Path.Combine(GamePath, App.CurrentGame.GameConfigFile[1]);
                config.Serialize(configPath);
            }
            else
            {
                MessageWindow message = new MessageWindow(Lang.GetString("MessageWindow.Errors.LoaderFailedToSave.Title"), Lang.GetString("MessageWindow.Errors.LoaderFailedToSave"),
                    icon: MessageWindow.Icons.Error);
                message.ShowDialog();
            }
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
        public void Serialize(string profileName)
        {
			if (!profileName.Contains(".json"))
				profileName += ".json";

			// TODO: Fix this function.
			string path = Path.Combine(App.CurrentGame.ProfilesDirectory, profileName);
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