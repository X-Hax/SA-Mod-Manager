using System.Collections.Generic;
using System.ComponentModel;
using SAModManager.Ini;
using SAModManager.Configuration;
using System.IO;
using System.Text.Json;
using SAModManager.UI;
using System.Text.Json.Serialization;
using static SAModManager.Configuration.SADX.GameSettings;

// TODO: Build SA2 Game Settings
namespace SAModManager.Configuration.SA2
{
	public class GraphicsSettings
	{
        public enum DisplayMode
        {
            Windowed,
            Fullscreen,
            Borderless,
            CustomWindow
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
        public int HorizontalResolution { get; set; } = 640;    // SA2XLoaderInfo.HorizontalResolution

        /// <summary>
        /// Rendering Vertical Resolution.
        /// </summary>
        [DefaultValue(480)]
        public int VerticalResolution { get; set; } = 480;      // SADXLoaderInfo.VerticalResolution

        /// <summary>
		/// Enables the window to be paused when not focused.
		/// </summary>
		[DefaultValue(true)]
        public bool EnablePauseOnInactive { get; set; } = true;     // SADXLoaderInfo.PauseWhenInactive

        /// <summary>
        /// Sets the Width of the Custom Window Size.
        /// </summary>
        [DefaultValue(640)]
        public int CustomWindowWidth { get; set; } = 640;             // SADXLoaderInfo.WindowWidth

        /// <summary>
        /// Sets the Height of the Custom Window Size.
        /// </summary>
        [DefaultValue(480)]
        public int CustomWindowHeight { get; set; } = 480;            // SADXLoaderInfo.WindowHeight

        /// <summary>
        /// Keeps the Resolution's ratio for the Custom Window size.
        /// </summary>
        [DefaultValue(false)]
        public bool EnableKeepResolutionRatio { get; set; }     // SADXLoaderInfo.MaintainWindowAspectRatio

        /// <summary>
        /// Enables resizing of the game window.
        /// </summary>
        [DefaultValue(false)]
        public bool EnableResizableWindow { get; set; }               // SADXLoaderInfo.ResizableWindow

        /// <summary>
		/// Sets the Screen Mode (Windowed, Fullscreen, Borderless, or Custom Window)
		/// </summary>
		[DefaultValue(0)]
        public int ScreenMode { get; set; }

        #region Deprecated
        /// <summary>
        /// Deprecated, see <see cref="ScreenMode"/>
        /// </summary>
        [DefaultValue(false)]
        public bool EnableCustomWindow { get; set; }              // SA2LoaderInfo.CustomWindowSize

        /// <summary>
        /// Deprecated, see <see cref="ScreenMode"/>
        /// </summary>
        [DefaultValue(true)]
        public bool EnableBorderless { get; set; } = true;          // SA2LoaderInfo.Borderless
        #endregion
        public void ConvertFromV0(SA2LoaderInfo oldSettings)
        {
            SelectedScreen = oldSettings.ScreenNum;

            HorizontalResolution = oldSettings.HorizontalResolution;
            VerticalResolution = oldSettings.VerticalResolution;

            EnablePauseOnInactive = oldSettings.PauseWhenInactive;

            EnableBorderless = oldSettings.WindowedFullscreen;


            EnableCustomWindow = oldSettings.CustomWindowSize;
            CustomWindowWidth = oldSettings.WindowWidth;
            CustomWindowHeight = oldSettings.WindowHeight;
            EnableKeepResolutionRatio = oldSettings.MaintainAspectRatio;
            EnableResizableWindow = oldSettings.ResizableWindow;
        }

        public void LoadLoaderInfo(ref SA2LoaderInfo info)
        {
            SelectedScreen = info.ScreenNum;
            HorizontalResolution = info.HorizontalResolution;
            VerticalResolution = info.VerticalResolution;

            switch ((DisplayMode)ScreenMode)
            {
                case DisplayMode.Borderless:
                    info.WindowedFullscreen = true;
                    break;
                case DisplayMode.CustomWindow:
                    info.CustomWindowSize = true;
                    break;
            }


            EnableKeepResolutionRatio = info.MaintainAspectRatio;
            CustomWindowWidth = info.WindowWidth;
            CustomWindowHeight = info.WindowHeight;


            EnablePauseOnInactive = info.PauseWhenInactive;
            EnableResizableWindow = info.ResizableWindow;
        }

        public void LoadGameConfig(ref SA2ConfigFile config)
        {


            switch ((DisplayMode)ScreenMode)
            {
                case DisplayMode.Fullscreen:
                    config.GameConfig.FullScreen = 1;
                    break;
                default:
                    config.GameConfig.FullScreen = 0;
                    break;
            }
        }

        public void LoadConfigs(ref SA2LoaderInfo info, ref SA2ConfigFile config)
        {
            LoadLoaderInfo(ref info);
            LoadGameConfig(ref config);
        }

        public void ToLoaderInfo(ref SA2LoaderInfo info)
        {
            info.ScreenNum = SelectedScreen;
            info.HorizontalResolution = HorizontalResolution;
            info.VerticalResolution = VerticalResolution;

            switch ((DisplayMode)ScreenMode)
            {
                case DisplayMode.Borderless:
                    info.WindowedFullscreen = true;
                    break;
                case DisplayMode.CustomWindow:
                    info.CustomWindowSize = true;
                    break;
            }

  
            info.MaintainAspectRatio = EnableKeepResolutionRatio;
            info.WindowWidth = CustomWindowWidth;
            info.WindowHeight = CustomWindowHeight;

            info.PauseWhenInactive = EnablePauseOnInactive;
            info.ResizableWindow = EnableResizableWindow;
        }

        public void ToGameConfig(ref SA2ConfigFile config)
        {

            switch ((DisplayMode)ScreenMode)
            {
                case DisplayMode.Fullscreen:
                case DisplayMode.Borderless:
                    config.GameConfig.FullScreen = 1;
                    break;
                default:
                    config.GameConfig.FullScreen = 0;
                    break;
            }
        }

        /// <summary>
        /// Sets values for referenced SADXLoaderInfo and SADXConfigFile.
        /// </summary>
        /// <param name="info"></param>
        /// <param name="config"></param>
        public void ToConfigs(ref SA2LoaderInfo info, ref SA2ConfigFile config)
        {
            ToLoaderInfo(ref info);
            ToGameConfig(ref config);
        }
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

        public void LoadLoaderInfo(ref SA2LoaderInfo info)
        {
            LevelIndex = info.TestSpawnLevel;
            CharacterIndex = info.TestSpawnCharacter;
            Player2Index = info.TestSpawnPlayer2;
            EventIndex = info.TestSpawnEvent;
            SaveIndex = info.TestSpawnSaveID;
            GameTextLanguage = info.TextLanguage;
            GameVoiceLanguage = info.VoiceLanguage;
            UsePosition = info.TestSpawnPositionEnabled;
            XPosition = info.TestSpawnX;
            YPosition = info.TestSpawnY;
            ZPosition = info.TestSpawnZ;
  
        }

        public void LoadConfigs(ref SA2LoaderInfo info)
        {
            LoadLoaderInfo(ref info);
        }

        public void ToLoaderInfo(ref SA2LoaderInfo info)
        {
            info.TestSpawnLevel = LevelIndex;
            info.TestSpawnCharacter = CharacterIndex;
            info.TestSpawnPlayer2 = Player2Index;
            info.TestSpawnEvent = EventIndex;
    
            info.TestSpawnSaveID = SaveIndex;
            info.TextLanguage = GameTextLanguage;
            info.VoiceLanguage = GameVoiceLanguage;
            info.TestSpawnPositionEnabled = UsePosition;
            info.TestSpawnX = (int)XPosition;
            info.TestSpawnY = (int)YPosition;
            info.TestSpawnZ = (int)ZPosition;

        }

        /// <summary>
        /// Sets values for referenced SA2LoaderInfo.
        /// </summary>
        /// <param name="info"></param>
        public void ToConfigs(ref SA2LoaderInfo info)
        {
            ToLoaderInfo(ref info);
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


        public void LoadLoaderInfo(ref SA2LoaderInfo info)
        {
            FramerateLimiter = info.FramerateLimiter;
            DisableExitPrompt = info.DisableExitPrompt;
            SyncLoad = info.SyncLoad;
            ExtendVertexBuffer = info.ExtendVertexBuffer;
            EnvMapFix = info.EnvMapFix;
            ScreenFadeFix = info.ScreenFadeFix;
            CECarFix = info. CECarFix;
            ParticlesFix = info.ParticlesFix;
        }

        public void LoadConfigs(ref SA2LoaderInfo info)
        {
            LoadLoaderInfo(ref info);
        }

        public void ToLoaderInfo(ref SA2LoaderInfo info)
        {
            info.FramerateLimiter = FramerateLimiter;
            info.DisableExitPrompt = DisableExitPrompt;
            info.SyncLoad = SyncLoad;
            info.ExtendVertexBuffer = ExtendVertexBuffer;
            info.EnvMapFix = EnvMapFix;
            info.ScreenFadeFix = ScreenFadeFix;
            info. CECarFix = CECarFix;
            info. ParticlesFix = ParticlesFix;
        }

        public void ToConfigs(ref SA2LoaderInfo info)
        {
            ToLoaderInfo(ref info);
        }

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

            GamePath = App.CurrentGame.gameDirectory;
            EnabledMods = oldSettings.Mods;
            EnabledCodes = oldSettings.EnabledCodes;
        }

		/// <summary>
		/// Converts the current GameSettings info back to the Loader ini's required format.
		/// </summary>
		/// <param name="managerSettings"></param>
		/// <returns></returns>
		private SA2LoaderInfo ConvertToLoaderInfo(ManagerSettings managerSettings)
		{
            SA2LoaderInfo loaderInfo = new();

            // Manager Settings
            loaderInfo.devMode = managerSettings.EnableDeveloperMode;
            loaderInfo.managerOpen = managerSettings.KeepManagerOpen;
            loaderInfo.Theme = managerSettings.Theme;
            loaderInfo.Language = managerSettings.Language;
            loaderInfo.UpdateCheck = managerSettings.UpdateSettings.EnableManagerBootCheck;
            loaderInfo.ModUpdateCheck = managerSettings.UpdateSettings.EnableModsBootCheck;

            // Mods & Codes
            loaderInfo.Mods = EnabledMods;
            loaderInfo.EnabledCodes = EnabledCodes;

            // Graphics
            loaderInfo.ScreenNum = Graphics.SelectedScreen;
            loaderInfo.HorizontalResolution = Graphics.HorizontalResolution;
            loaderInfo.VerticalResolution = Graphics.VerticalResolution;
            loaderInfo.PauseWhenInactive = Graphics.EnablePauseOnInactive;

            loaderInfo.CustomWindowSize = Graphics.EnableCustomWindow;
            loaderInfo.WindowWidth = Graphics.CustomWindowWidth;
            loaderInfo.WindowHeight = Graphics.CustomWindowHeight;
            loaderInfo.MaintainAspectRatio = Graphics.EnableKeepResolutionRatio;
            loaderInfo.ResizableWindow = Graphics.EnableResizableWindow;


            // Patches
            loaderInfo.FramerateLimiter = Patches.FramerateLimiter;
            loaderInfo.DisableExitPrompt = Patches.DisableExitPrompt;
            loaderInfo.SyncLoad = Patches.SyncLoad;
            loaderInfo.ExtendVertexBuffer = Patches.ExtendVertexBuffer;
            loaderInfo.EnvMapFix = Patches.EnvMapFix;
            loaderInfo.ScreenFadeFix = Patches.ScreenFadeFix;
            loaderInfo.CECarFix = Patches.CECarFix;
            loaderInfo.ParticlesFix = Patches.ParticlesFix;


            // Debug
            loaderInfo.DebugConsole = DebugSettings.EnableDebugConsole;
            loaderInfo.DebugScreen = DebugSettings.EnableDebugScreen;
            loaderInfo.DebugFile = DebugSettings.EnableDebugFile;
            loaderInfo.DebugCrashLog = DebugSettings.EnableDebugCrashLog;

            // Test Spawn
            loaderInfo.TestSpawnLevel = TestSpawn.LevelIndex;
            loaderInfo.TestSpawnCharacter = TestSpawn.CharacterIndex;
            loaderInfo.TestSpawnEvent = TestSpawn.EventIndex;
            loaderInfo.TestSpawnPlayer2 = TestSpawn.Player2Index;
            loaderInfo.TestSpawnSaveID = TestSpawn.SaveIndex;
            loaderInfo.TextLanguage = TestSpawn.GameTextLanguage;
            loaderInfo.VoiceLanguage = TestSpawn.GameVoiceLanguage;
            loaderInfo.TestSpawnPositionEnabled = TestSpawn.UsePosition;
            loaderInfo.TestSpawnX = (int)TestSpawn.XPosition;
            loaderInfo.TestSpawnY = (int)TestSpawn.YPosition;
            loaderInfo.TestSpawnZ = (int)TestSpawn.ZPosition;


            return loaderInfo;
        }

		/// <summary>
		/// Writes to the Loader's necessary ini file. Path is to the Mod's Directory.
		/// </summary>
		/// <param name="path"></param>
		public void WriteToLoaderInfo(string path, ManagerSettings managerSettings)
		{
            if (Directory.Exists(path))
            {
                SA2LoaderInfo loaderInfo = ConvertToLoaderInfo(managerSettings);
                string loaderInfoPath = Path.Combine(path, "SA2ModLoader.ini");

                IniSerializer.Serialize(loaderInfo, loaderInfoPath);
            }
            else
            {
                MessageWindow message = new MessageWindow(Lang.GetString("MessageWindow.Errors.LoaderFailedToSave.Title"), Lang.GetString("MessageWindow.Errors.LoaderFailedToSave"),
                    icon: MessageWindow.Icons.Error);
                message.ShowDialog();
            }
        }

        public void LoadConfigs(string iniSource)
        {
            SA2LoaderInfo info = new();
            SA2ConfigFile config = new();
            var JsonSerializerSettings = new JsonSerializerOptions();
            JsonSerializerSettings.DefaultIgnoreCondition = JsonIgnoreCondition.WhenWritingNull;

            string loaderInfoPath = iniSource;

            if (File.Exists(loaderInfoPath))
                info = IniSerializer.Deserialize<SA2LoaderInfo>(loaderInfoPath);

            string configPath = Path.Combine(App.CurrentGame.gameDirectory, App.CurrentGame.GameConfigFile[1]);
            if (File.Exists(configPath))
                config = IniSerializer.Deserialize<SA2ConfigFile>(configPath);

            Graphics.LoadConfigs(ref info, ref config);
            TestSpawn.LoadConfigs(ref info);
            Patches.LoadConfigs(ref info);

            DebugSettings.EnableDebugConsole = info.DebugConsole;
            DebugSettings.EnableDebugScreen = info.DebugScreen;
            DebugSettings.EnableDebugFile = info.DebugFile;
            DebugSettings.EnableDebugCrashLog = info.DebugCrashLog;
        }

        /// <summary>
        /// Writes LoaderInfo and SADXConfig files.
        /// </summary>
        public void WriteConfigs()
        {
            SA2LoaderInfo info = new SA2LoaderInfo();
            SA2ConfigFile config = new SA2ConfigFile();

            info.Mods = EnabledMods;
            info.EnabledCodes = EnabledCodes;

            Graphics.ToConfigs(ref info, ref config);
            TestSpawn.ToConfigs(ref info);
            Patches.ToConfigs(ref info);

            // Debug Settings
            info.DebugConsole = DebugSettings.EnableDebugConsole;
            info.DebugScreen = DebugSettings.EnableDebugScreen;
            info.DebugFile = DebugSettings.EnableDebugFile;
            info.DebugCrashLog = DebugSettings.EnableDebugCrashLog;

            if (Directory.Exists(GamePath))
            {
                string loaderInfoPath = Path.GetFullPath(Path.Combine(App.CurrentGame.modDirectory, "SA2ModLoader.ini"));
                IniSerializer.Serialize(info, loaderInfoPath);

                string configPath = Path.Combine(GamePath, App.CurrentGame.GameConfigFile[0]);
                IniSerializer.Serialize(config, configPath);
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
