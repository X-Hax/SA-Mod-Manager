using System.Collections.Generic;
using System.ComponentModel;
using SAModManager.Ini;
using SAModManager.Configuration;
using System.IO;
using SAModManager.Common;
using System.Text.Json;
using System.CodeDom.Compiler;
using System;
using System.Text.Json.Serialization;

namespace SAModManager.Configuration.SADX
{
	public class GraphicsSettings
	{
		enum FillMode
		{
			Stretch = 0,
			Fit = 1,
			Fill = 2
		}

		public enum TextureFilter
		{
			temp = 0,
		}

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
		public int SelectedScreen { get; set; } = 1;                 // SADXLoaderInfo.ScreenNum

		/// <summary>
		/// Rendering Horizontal Resolution.
		/// </summary>
		[DefaultValue(640)]
		public int HorizontalResolution { get; set; } = 640;    // SADXLoaderInfo.HorizontalResolution

		/// <summary>
		/// Rendering Vertical Resolution.
		/// </summary>
		[DefaultValue(480)]
		public int VerticalResolution { get; set; } = 480;      // SADXLoaderInfo.VerticalResolution

		/// <summary>
		/// Locks the Resolution to 4:3 Ratio
		/// </summary>
		[DefaultValue(false)]
		public bool Enable43ResolutionRatio { get; set; }              // SADXLoaderInfo.ForseAspectRatio

		/// <summary>
		/// V-Sync.
		/// </summary>
		[DefaultValue(true)]
		public bool EnableVsync { get; set; } = true;           // SADXLoaderInfo.EnableVSync

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
		/// Method in which the BG's fill the game window.
		/// </summary>
		[DefaultValue((int)FillMode.Fill)]
		public int FillModeBackground { get; set; } = (int)FillMode.Fill;   // SADXLoaderInfo.BackgroundFillMode

		/// <summary>
		/// Method in which FMV's fill the game window.
		/// </summary>
		[DefaultValue((int)FillMode.Fit)]
		public int FillModeFMV { get; set; } = (int)FillMode.Fit;           // SADXLoaderInfo.FmvFillMode

		/// <summary>
		/// Texture filtering for non-UI textures.
		/// </summary>
		[DefaultValue(TextureFilter.temp)]
		public TextureFilter ModeTextureFiltering { get; set; }

		/// <summary>
		/// Texture filtering for UI textures only.
		/// </summary>
		[DefaultValue(TextureFilter.temp)]
		public TextureFilter ModeUIFiltering { get; set; }

		/// <summary>
		/// Enables UI Scaling to match the window properly.
		/// </summary>
		[DefaultValue(true)]
		public bool EnableUIScaling { get; set; } = true;              // SADXLoaderInfo.ScaleHud

		/// <summary>
		/// Enables mipmapping on all textures being rendered.
		/// </summary>
		[DefaultValue(true)]
		public bool EnableForcedMipmapping { get; set; } = true;            // SADXLoaderInfo.AutoMipmap

		//To DO change with ComboBox for different settings
        [DefaultValue(true)]
        public bool EnableForcedTextureFilter { get; set; } = true; // SADXLoaderInfo.TextureFilter

		/// <summary>
		/// Sets the Screen Mode (Windowed, Fullscreen, Borderless, or Custom Window)
		/// </summary>
		[DefaultValue(0)]
		public int ScreenMode { get; set; }

		/// <summary>
		/// Sets the Game's Framerate
		/// </summary>
		[DefaultValue(1)]
		public int GameFrameRate { get; set; }  // SADXGameConfig.FrameRate

		/// <summary>
		/// Sets the game's method of FogEmulation
		/// </summary>
		[DefaultValue(0)]
		public int GameFogMode { get; set; }    // SADXGameConfig.Foglation

		/// <summary>
		/// Sets the game's clipping level.
		/// </summary>
		[DefaultValue(0)]
		public int GameClipLevel { get; set; }  // SADXGameConfig.ClipLevel

		/// <summary>
		/// Allows the cursor to show in the game window when in either Fullscreen mode.
		/// </summary>
		[DefaultValue(false)]
		public bool ShowMouseInFullscreen { get; set; }

		#region Deprecated
		/// <summary>
		/// Deprecated, see <see cref="ScreenMode"/>
		/// </summary>
		[DefaultValue(false)]
		public bool EnableCustomWindow { get; set; }              // SADXLoaderInfo.CustomWindowSize

		/// <summary>
		/// Deprecated, see <see cref="ScreenMode"/>
		/// </summary>
		[DefaultValue(true)]
		public bool EnableBorderless { get; set; } = true;          // SADXLoaderInfo.Borderless

		/// <summary>
		/// Deprecated, always set to true in the mod loader now.
		/// </summary>
		[DefaultValue(true)]
		public bool EnableScreenScaling { get; set; } = true;     // SADXLoaderInfo.StretchFullscreen
		#endregion

		/// <summary>
		/// Converts from original settings file.
		/// </summary>
		/// <param name="oldSettings"></param>
		public void ConvertFromV0(SADXLoaderInfo oldSettings)
		{
			SelectedScreen = oldSettings.ScreenNum;

			HorizontalResolution = oldSettings.HorizontalResolution;
			VerticalResolution = oldSettings.VerticalResolution;

			Enable43ResolutionRatio = oldSettings.ForceAspectRatio;
			EnableVsync = oldSettings.EnableVsync;
			EnablePauseOnInactive = oldSettings.PauseWhenInactive;

			EnableBorderless = oldSettings.WindowedFullscreen;
			EnableScreenScaling = oldSettings.StretchFullscreen;

			EnableCustomWindow = oldSettings.CustomWindowSize;
			CustomWindowWidth = oldSettings.WindowWidth;
			CustomWindowHeight = oldSettings.WindowHeight;
			EnableKeepResolutionRatio = oldSettings.MaintainWindowAspectRatio;
			EnableResizableWindow = oldSettings.ResizableWindow;

			FillModeBackground = oldSettings.BackgroundFillMode;
			FillModeFMV = oldSettings.FmvFillMode;
			EnableUIScaling = oldSettings.ScaleHud;
			EnableForcedMipmapping = oldSettings.AutoMipmap;
			EnableForcedTextureFilter = oldSettings.TextureFilter;
		}

		public void LoadLoaderInfo(ref SADXLoaderInfo info)
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

			Enable43ResolutionRatio = info.ForceAspectRatio;
			EnableKeepResolutionRatio = info.MaintainWindowAspectRatio;
			CustomWindowWidth = info.WindowWidth;
			CustomWindowHeight = info.WindowHeight;

			EnableVsync = info.EnableVsync;
			EnablePauseOnInactive = info.PauseWhenInactive;
			EnableScreenScaling = info.StretchFullscreen;
			EnableResizableWindow = info.ResizableWindow;

			FillModeBackground = info.BackgroundFillMode;
			FillModeFMV = info.FmvFillMode;
			EnableUIScaling = info.ScaleHud;
			EnableForcedMipmapping = info.AutoMipmap;
			EnableForcedTextureFilter = info.TextureFilter;
		}

		public void LoadGameConfig(ref SADXConfigFile config)
		{
			config.GameConfig.FrameRate = GameFrameRate + 1;
			config.GameConfig.Foglation = GameFogMode;
			config.GameConfig.ClipLevel = GameClipLevel;

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

		public void LoadConfigs(ref SADXLoaderInfo info, ref SADXConfigFile config)
		{
			LoadLoaderInfo(ref info);
			LoadGameConfig(ref config);
		}

		public void ToLoaderInfo(ref SADXLoaderInfo info)
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

			info.ForceAspectRatio = Enable43ResolutionRatio;
			info.MaintainWindowAspectRatio = EnableKeepResolutionRatio;
			info.WindowWidth = CustomWindowWidth;
			info.WindowHeight = CustomWindowHeight;

			info.EnableVsync = EnableVsync;
			info.PauseWhenInactive = EnablePauseOnInactive;
			info.StretchFullscreen = EnableScreenScaling;
			info.ResizableWindow = EnableResizableWindow;

			info.BackgroundFillMode = FillModeBackground;
			info.FmvFillMode = FillModeFMV;
			info.ScaleHud = EnableUIScaling;
			info.AutoMipmap = EnableForcedMipmapping;
			info.TextureFilter = EnableForcedTextureFilter;
		}

		public void ToGameConfig(ref SADXConfigFile config)
		{
			config.GameConfig.FrameRate = GameFrameRate + 1;
			config.GameConfig.Foglation = GameFogMode;
			config.GameConfig.ClipLevel = GameClipLevel;

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
		public void ToConfigs(ref SADXLoaderInfo info, ref SADXConfigFile config)
		{
			ToLoaderInfo(ref info);
			ToGameConfig(ref config);
		}
	}

	public class ControllerSettings
	{
		/// <summary>
		/// Enable SDL2 Input
		/// </summary>
		[DefaultValue(true)]
		public bool EnabledInputMod { get; set; } = true;   // SADXLoaderInfo.InputModEnabled

		[DefaultValue(false)]
		public bool VanillaMouseUseDrag { get; set; } = false;	// SADXGameConfig.MouseMode

		public int VanillaMouseStart { get; set; }

		public int VanillaMouseAttack { get; set; }

		public int VanillaMouseJump { get; set; }

		public int VanillaMouseAction { get; set; }

		public int VanillaMouseFlute { get; set; }

		/// <summary>
		/// Converts from original settings file.
		/// </summary>
		/// <param name="oldSettings"></param>
		public void ConvertFromV0(SADXLoaderInfo oldSettings)
		{
			EnabledInputMod = oldSettings.InputModEnabled;
		}

		public void LoadLoaderInfo(ref SADXLoaderInfo info)
		{
			EnabledInputMod = info.InputModEnabled;
		}

		public void LoadGameConfig(ref SADXConfigFile config)
		{
			VanillaMouseUseDrag = (config.GameConfig.MouseMode == 1) ? true : false;
			VanillaMouseStart = config.GameConfig.MouseStart;
			VanillaMouseAction = config.GameConfig.MouseAction;
			VanillaMouseJump = config.GameConfig.MouseJump;
			VanillaMouseAttack = config.GameConfig.MouseAttack;
			VanillaMouseFlute = config.GameConfig.MouseFlute;
		}

		public void LoadConfigs(ref SADXLoaderInfo info, ref SADXConfigFile config)
		{
			LoadLoaderInfo(ref info);
			LoadGameConfig(ref config);
		}

		public void ToLoaderInfo(ref SADXLoaderInfo info)
		{
			info.InputModEnabled = EnabledInputMod;
		}

		public void ToGameConfig(ref SADXConfigFile config)
		{
			config.GameConfig.MouseMode = VanillaMouseUseDrag ? 1 : 0;
			config.GameConfig.MouseStart = (ushort)VanillaMouseStart;
			config.GameConfig.MouseAction = (ushort)VanillaMouseAction;
			config.GameConfig.MouseJump = (ushort)VanillaMouseJump;
			config.GameConfig.MouseAttack = (ushort)VanillaMouseAttack;
			config.GameConfig.MouseFlute = (ushort)VanillaMouseFlute;
		}

		/// <summary>
		/// Sets values for referenced SADXLoaderInfo and SADXConfigFile.
		/// </summary>
		/// <param name="info"></param>
		/// <param name="config"></param>
		public void ToConfigs(ref SADXLoaderInfo info, ref SADXConfigFile config)
		{
			ToLoaderInfo(ref info);
			ToGameConfig(ref config);
		}
	}

	public class SoundSettings
	{
		/// <summary>
		/// Enables Game Music
		/// </summary>
		[DefaultValue(true)]
		public bool EnableGameMusic { get; set; } = true;   // SADXGameConfig.BGM

		/// <summary>
		/// Enables Game Sounds and Voices
		/// </summary>
		[DefaultValue(true)]
		public bool EnableGameSound { get; set; } = true;   // SADXGameConfig.SEVoice

		/// <summary>
		/// Enables 3D Sound In-Game
		/// </summary>
		[DefaultValue(true)]
		public bool EnableGameSound3D { get; set; } = true;	// SADXGameConfig.Sound3D

		/// <summary>
		/// Enables BASS as the music backend.
		/// </summary>
		[DefaultValue(true)]
		public bool EnableBassMusic { get; set; } = true;             // SADXLoaderInfo.EnableBassMusic

		/// <summary>
		/// Enables the use of BASS for Sound Effects in game.
		/// </summary>
		[DefaultValue(false)]
		public bool EnableBassSFX { get; set; } = false;              // SADXLoaderInfo.EnableBassSFX

		/// <summary>
		/// Game Music Volume
		/// </summary>
		[DefaultValue(100)]
		public int GameMusicVolume { get; set; } = 100;         // SADXGameConfig.BGMVolume

		/// <summary>
		/// Game Sound and Voice Volume
		/// </summary>
		[DefaultValue(100)]
		public int GameSoundVolume { get; set; } = 100;		// SADXGameConfig.SEVolume

		/// <summary>
		/// Sound Effect Volume when using BASS for Sound Effects.
		/// </summary>
		[DefaultValue(100)]
		public int SEVolume { get; set; } = 100;                // SADXLoaderInfo.SEVolume

		/// <summary>
		/// Converts from original settings file.
		/// </summary>
		/// <param name="oldSettings"></param>
		public void ConvertFromV0(SADXLoaderInfo oldSettings)
		{
			EnableBassMusic = oldSettings.EnableBassMusic;
			EnableBassSFX = oldSettings.EnableBassSFX;
			SEVolume = oldSettings.SEVolume;
		}

		public void LoadLoaderInfo(ref SADXLoaderInfo info)
		{
			EnableBassMusic = info.EnableBassMusic;
			EnableBassSFX = info.EnableBassSFX;
			SEVolume = info.SEVolume;
		}

		public void LoadGameConfig(ref SADXConfigFile config)
		{
			EnableGameMusic = (config.GameConfig.BGM == 1) ? true : false;
			EnableGameSound = (config.GameConfig.SEVoice == 1) ? true : false;
			EnableGameSound3D = (config.GameConfig.Sound3D == 1) ? true : false;
			GameMusicVolume = config.GameConfig.BGMVolume;
			GameSoundVolume = config.GameConfig.VoiceVolume;
		}

		/// <summary>
		/// Loads data from LoaderInfo and SADXConfigFile.
		/// </summary>
		/// <param name="info"></param>
		/// <param name="config"></param>
		public void LoadConfigs(ref SADXLoaderInfo info, ref SADXConfigFile config)
		{
			LoadLoaderInfo(ref info);
			LoadGameConfig(ref config);
		}

		public void ToLoaderInfo(ref SADXLoaderInfo info)
		{
			info.EnableBassMusic = EnableBassMusic;
			info.EnableBassSFX = EnableBassSFX;
			info.SEVolume = SEVolume;
		}

		public void ToGameConfig(ref SADXConfigFile config)
		{
			config.GameConfig.BGM = EnableGameMusic ? 1 : 0;
			config.GameConfig.SEVoice = EnableGameSound ? 1 : 0;
			config.GameConfig.Sound3D = EnableGameSound3D ? 1 : 0;
			config.GameConfig.BGMVolume = GameMusicVolume;
			config.GameConfig.VoiceVolume = GameSoundVolume;
		}

		/// <summary>
		/// Sets values for referenced SADXLoaderInfo and SADXConfigFile.
		/// </summary>
		/// <param name="info"></param>
		/// <param name="config"></param>
		public void ToConfigs(ref SADXLoaderInfo info, ref SADXConfigFile config)
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
		/// Enables the GameMode options when launching.
		/// </summary>
		[DefaultValue(false)]
		public bool UseGameMode { get; set; } = false;

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
		public int ActIndex { get; set; } = 0;          // SADXLoaderInfo.TestSpawnAct

		/// <summary>
		/// Selected index for the character used by test spawn.
		/// </summary>
		[DefaultValue(-1)]
		public int CharacterIndex { get; set; } = -1;   // SADXLoaderInfo.TestSpawnCharacter

		/// <summary>
		/// Selected index of an event used by test spawn.
		/// </summary>
		[DefaultValue(-1)]
		public int EventIndex { get; set; } = -1;       // SADXLoaderInfo.TestSpawnEvent

		/// <summary>
		/// Selected index for the GameMode used by test spawn.
		/// </summary>
		[DefaultValue(-1)]
		public int GameModeIndex { get; set; } = -1;    // SADXLoaderInfo.TestSpawnGameMode

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
			ActIndex = oldSettings.TestSpawnAct;
			CharacterIndex = oldSettings.TestSpawnCharacter;
			EventIndex = oldSettings.TestSpawnEvent;
			GameModeIndex = oldSettings.TestSpawnGameMode;
			SaveIndex = oldSettings.TestSpawnEvent;

			GameTextLanguage = oldSettings.TextLanguage;
			GameVoiceLanguage = oldSettings.VoiceLanguage;

			UsePosition = oldSettings.TestSpawnPositionEnabled;

			XPosition = oldSettings.TestSpawnX;
			YPosition = oldSettings.TestSpawnY;
			ZPosition = oldSettings.TestSpawnZ;
			Rotation = oldSettings.TestSpawnRotation;
		}

		public void LoadLoaderInfo(ref SADXLoaderInfo info)
		{
			LevelIndex = info.TestSpawnLevel;
			ActIndex = info.TestSpawnAct;
			CharacterIndex = info.TestSpawnCharacter;
			EventIndex = info.TestSpawnEvent;
			GameModeIndex = info.TestSpawnGameMode;
			SaveIndex = info.TestSpawnSaveID;
			GameTextLanguage = info.TextLanguage;
			GameVoiceLanguage = info.VoiceLanguage;
			UsePosition = info.TestSpawnPositionEnabled;
			XPosition = info.TestSpawnX;
			YPosition = info.TestSpawnY;
			ZPosition = info.TestSpawnZ;
			Rotation = info.TestSpawnRotation;
		}

		public void LoadConfigs(ref SADXLoaderInfo info)
		{
			LoadLoaderInfo(ref info);
		}

		public void ToLoaderInfo(ref SADXLoaderInfo info)
		{
			info.TestSpawnLevel = LevelIndex;
			info.TestSpawnAct = ActIndex;
			info.TestSpawnCharacter = CharacterIndex;
			info.TestSpawnEvent = EventIndex;
			info.TestSpawnGameMode = GameModeIndex;
			info.TestSpawnSaveID = SaveIndex;
			info.TextLanguage = GameTextLanguage;
			info.VoiceLanguage = GameVoiceLanguage;
			info.TestSpawnPositionEnabled = UsePosition;
			info.TestSpawnX = (int)XPosition;
			info.TestSpawnY = (int)YPosition;
			info.TestSpawnZ = (int)ZPosition;
			info.TestSpawnRotation = Rotation;
			info.TestSpawnRotationHex = (info.TestSpawnRotation > 0) ? true : false;
		}

		/// <summary>
		/// Sets values for referenced SADXLoaderInfo.
		/// </summary>
		/// <param name="info"></param>
		public void ToConfigs(ref SADXLoaderInfo info)
		{
			ToLoaderInfo(ref info);
		}
	}

	public class GamePatches
	{
		/// <summary>
		/// Modifies the sound system. Only SF94 knows why.
		/// </summary>
		[DefaultValue(true)]
		public bool HRTFSound { get; set; } = true;        // SADXLoaderInfo.HRTFSound

		/// <summary>
		/// Fixes the game turning off free cam when dying if it was previously set.
		/// </summary>
		[DefaultValue(true)]
		public bool KeepCamSettings { get; set; } = true;           // SADXLoaderInfo.CCEF

		/// <summary>
		/// Fixes the game's rendering to properly allow mesh's with vertex colors to be rendered with those vertex colors.
		/// </summary>
		[DefaultValue(true)]
		public bool FixVertexColorRendering { get; set; } = true; //vertex color	// SADXLoaderInfo.PolyBuff

		/// <summary>
		/// Fixes the material colors so they render properly.
		/// </summary>
		[DefaultValue(true)]
		public bool MaterialColorFix { get; set; } = true;      // SADXLoaderInfo.MaterialColorFix

		/// <summary>
		/// Increases nodes limits to 254 (originally 111 for characters)
		/// </summary>
		[DefaultValue(true)]
		public bool NodeLimit { get; set; } = true;      // New Feature, doesn't originally exist

		/// <summary>
		/// Fixes the game's FOV.
		/// </summary>
		[DefaultValue(true)]
		public bool FOVFix { get; set; } = true;     // SADXLoaderInfo.FovFix

		/// <summary>
		/// Fixes Skychase to properly render.
		/// </summary>
		[DefaultValue(true)]
		public bool SkyChaseResolutionFix { get; set; } = true; // SADXLoaderInfo.SCFix

		/// <summary>
		/// Fixes a bug that will cause the game to crash when fighting Chaos 2.
		/// </summary>
		[DefaultValue(true)]
		public bool Chaos2CrashFix { get; set; } = true;        // SADXLoaderInfo.Chaos2CrashFix

		/// <summary>
		/// Fixes specular rendering on Chunk models.
		/// </summary>
		[DefaultValue(true)]
		public bool ChunkSpecularFix { get; set; } = true;         // SADXLoaderInfo.ChunkSpecFix

		/// <summary>
		/// Fixes rendering on E-102's gun which uses an N-Gon.
		/// </summary>
		[DefaultValue(true)]
		public bool E102NGonFix { get; set; } = true;           // SADXLoaderInfo.E102PolyFix

		/// <summary>
		/// Fixes Chao Panel rendering.
		/// </summary>
		[DefaultValue(true)]
		public bool ChaoPanelFix { get; set; } = true;          // SADXLoaderInfo.ChaoPanelFix

		/// <summary>
		/// Fixes a slight pixel offset in the window.
		/// </summary>
		[DefaultValue(true)]
		public bool PixelOffSetFix { get; set; } = true;        // SADXLoaderInfo.PixelOffsetFix

		/// <summary>
		/// Fixes lights
		/// </summary>
		[DefaultValue(true)]
		public bool LightFix { get; set; } = true;           // SADXLoaderInfo.LightFix

		/// <summary>
		/// Removes GBIX processing for most textures. UI is excluded.
		/// </summary>
		[DefaultValue(true)]
		public bool KillGBIX { get; set; } = true;            // SADXLoaderInfo.KillGbix

		/// <summary>
		/// Disables the built in CD Check.
		/// </summary>
		[DefaultValue(false)]
		public bool DisableCDCheck { get; set; } = false;       // SADXLoaderInfo.DisableCDCheck
        [DefaultValue(true)]
        public bool ExtendedSaveSupport { get; set; } = true;

        /// <summary>
        /// Converts from original settings file.
        /// </summary>
        /// <param name="oldSettings"></param>
        public void ConvertFromV0(SADXLoaderInfo oldSettings)
		{
			HRTFSound = oldSettings.HRTFSound;
			KeepCamSettings = oldSettings.CCEF;
			FixVertexColorRendering = oldSettings.PolyBuff;
			MaterialColorFix = oldSettings.MaterialColorFix;
			NodeLimit = oldSettings.NodeLimit;
			FOVFix = oldSettings.FovFix;
			SkyChaseResolutionFix = oldSettings.SCFix;
			Chaos2CrashFix = oldSettings.Chaos2CrashFix;
			ChunkSpecularFix = oldSettings.ChunkSpecFix;
			E102NGonFix = oldSettings.E102PolyFix;
			ChaoPanelFix = oldSettings.ChaoPanelFix;
			PixelOffSetFix = oldSettings.PixelOffSetFix;
			LightFix = oldSettings.LightFix;
			KillGBIX = oldSettings.KillGbix;
			DisableCDCheck = oldSettings.DisableCDCheck;
			ExtendedSaveSupport = oldSettings.ExtendedSaveSupport;
		}

		public void LoadLoaderInfo(ref SADXLoaderInfo info)
		{
			HRTFSound = info.HRTFSound;
			KeepCamSettings = info.CCEF;
			FixVertexColorRendering = info.PolyBuff;
			MaterialColorFix = info.MaterialColorFix;
			NodeLimit = info.NodeLimit;
			FOVFix = info.FovFix;
			SkyChaseResolutionFix = info.SCFix;
			Chaos2CrashFix = info.Chaos2CrashFix;
			ChunkSpecularFix = info.ChunkSpecFix;
			E102NGonFix = info.E102PolyFix;
			ChaoPanelFix = info.ChaoPanelFix;
			PixelOffSetFix = info.PixelOffSetFix;
			LightFix = info.LightFix;
			KillGBIX = info.KillGbix;
			DisableCDCheck = info.DisableCDCheck;
		}

		public void LoadConfigs(ref SADXLoaderInfo info)
		{
			LoadLoaderInfo(ref info);
		}

		public void ToLoaderInfo(ref SADXLoaderInfo info)
		{
			info.HRTFSound = HRTFSound;
			info.CCEF = KeepCamSettings;
			info.PolyBuff = FixVertexColorRendering;
			info.MaterialColorFix = MaterialColorFix;
			info.NodeLimit = NodeLimit;
			info.FovFix = FOVFix;
			info.SCFix = SkyChaseResolutionFix;
			info.Chaos2CrashFix = Chaos2CrashFix;
			info.ChunkSpecFix = ChunkSpecularFix;
			info.E102PolyFix = E102NGonFix;
			info.ChaoPanelFix = ChaoPanelFix;
			info.PixelOffSetFix = PixelOffSetFix;
			info.LightFix = LightFix;
			info.KillGbix = KillGBIX;
			info.DisableCDCheck = DisableCDCheck;
		}

		public void ToConfigs(ref SADXLoaderInfo info)
		{
			ToLoaderInfo(ref info);
		}
	}

	public class GameSettings
	{
		/// <summary>
		/// Versioning. Please comment a brief on the changes to the version.
		/// </summary>
		public enum SADXSettingsVersions
		{
			v0 = 0,	// Version 0: Original LoaderInfo version
			v1 = 1,	// Version 1: Initial version at launch
			v2 = 2,	// Version 2: Updated to include all settings, intended to be used as the only loaded file, now writes SADXLoaderInfo and SADXConfigFile.
		}

		/// <summary>
		/// Versioning for the SADX Settings file.
		/// </summary>
		[DefaultValue((int)SADXSettingsVersions.v2)]
		public int SettingsVersion { get; set; } = (int)SADXSettingsVersions.v2;

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
		public List<string> EnabledMods { get; set; } = new();       // SADXLoaderInfo.Mods

		/// <summary>
		/// Enabled Codes for SADX.
		/// </summary>
		[IniName("Code")]
		[IniCollection(IniCollectionMode.NoSquareBrackets, StartIndex = 1)]
		public List<string> EnabledCodes { get; set; } = new();      // SADXLoaderInfo.EnabledCodes

		/// <summary>
		/// Used for Profiles Migration, for initial boot, see <see cref="LoadConfigs"/>
		/// </summary>
		/// <param name="oldSettings"></param>
		public void ConvertFromV0(SADXLoaderInfo oldSettings)
		{
			Graphics.ConvertFromV0(oldSettings);
			Controller.ConvertFromV0(oldSettings);
			Sound.ConvertFromV0(oldSettings);
			TestSpawn.ConvertFromV0(oldSettings);
			Patches.ConvertFromV0(oldSettings);
			DebugSettings.ConvertFromV0(oldSettings);

			SettingsVersion = (int)SADXSettingsVersions.v1;
			GamePath = App.CurrentGame.gameDirectory;
			EnabledMods = oldSettings.Mods;
			EnabledCodes = oldSettings.EnabledCodes;
		}

		/// <summary>
		/// Deprecated in v2, see <see cref="WriteConfigs"/>
		/// </summary>
		/// <param name="managerSettings"></param>
		/// <returns></returns>
		private SADXLoaderInfo ConvertToLoaderInfo(ManagerSettings managerSettings)
		{
			SADXLoaderInfo loaderInfo = new();

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
			loaderInfo.ForceAspectRatio = Graphics.Enable43ResolutionRatio;
			loaderInfo.EnableVsync = Graphics.EnableVsync;
			loaderInfo.PauseWhenInactive = Graphics.EnablePauseOnInactive;
			loaderInfo.WindowedFullscreen = Graphics.EnableBorderless;
			loaderInfo.StretchFullscreen = Graphics.EnableScreenScaling;
			loaderInfo.CustomWindowSize = Graphics.EnableCustomWindow;
			loaderInfo.WindowWidth = Graphics.CustomWindowWidth;
			loaderInfo.WindowHeight = Graphics.CustomWindowHeight;
			loaderInfo.MaintainWindowAspectRatio = Graphics.EnableKeepResolutionRatio;
			loaderInfo.ResizableWindow = Graphics.EnableResizableWindow;
			loaderInfo.BackgroundFillMode = Graphics.FillModeBackground;
			loaderInfo.FmvFillMode = Graphics.FillModeFMV;
			loaderInfo.ScaleHud = Graphics.EnableUIScaling;
			loaderInfo.AutoMipmap = Graphics.EnableForcedMipmapping;
			loaderInfo.TextureFilter = Graphics.EnableForcedTextureFilter;

            // Input
            loaderInfo.InputModEnabled = Controller.EnabledInputMod;

			// Sound
			loaderInfo.EnableBassMusic = Sound.EnableBassMusic;
			loaderInfo.EnableBassSFX = Sound.EnableBassSFX;
			loaderInfo.SEVolume = Sound.SEVolume;

			// Patches
			loaderInfo.HRTFSound = Patches.HRTFSound;
			loaderInfo.CCEF = Patches.KeepCamSettings;
			loaderInfo.PolyBuff = Patches.FixVertexColorRendering;
			loaderInfo.MaterialColorFix = Patches.MaterialColorFix;
			loaderInfo.NodeLimit = Patches.NodeLimit;
			loaderInfo.FovFix = Patches.FOVFix;
			loaderInfo.SCFix = Patches.SkyChaseResolutionFix;
			loaderInfo.Chaos2CrashFix = Patches.Chaos2CrashFix;
			loaderInfo.ChunkSpecFix = Patches.ChunkSpecularFix;
			loaderInfo.E102PolyFix = Patches.E102NGonFix;
			loaderInfo.ChaoPanelFix = Patches.ChaoPanelFix;
			loaderInfo.PixelOffSetFix = Patches.PixelOffSetFix;
			loaderInfo.LightFix = Patches.LightFix;
			loaderInfo.KillGbix = Patches.KillGBIX;
			loaderInfo.DisableCDCheck = Patches.DisableCDCheck;
			loaderInfo.ExtendedSaveSupport = Patches.ExtendedSaveSupport;


            // Debug
            loaderInfo.DebugConsole = DebugSettings.EnableDebugConsole;
			loaderInfo.DebugScreen = DebugSettings.EnableDebugScreen;
			loaderInfo.DebugFile = DebugSettings.EnableDebugFile;
			loaderInfo.DebugCrashLog = DebugSettings.EnableDebugCrashLog;

			// Test Spawn
			loaderInfo.TestSpawnLevel = TestSpawn.LevelIndex;
			loaderInfo.TestSpawnAct = TestSpawn.ActIndex;
			loaderInfo.TestSpawnCharacter = TestSpawn.CharacterIndex;
			loaderInfo.TestSpawnEvent = TestSpawn.EventIndex;
			loaderInfo.TestSpawnGameMode = TestSpawn.GameModeIndex;
			loaderInfo.TestSpawnSaveID = TestSpawn.SaveIndex;
			loaderInfo.TextLanguage = TestSpawn.GameTextLanguage;
			loaderInfo.VoiceLanguage = TestSpawn.GameVoiceLanguage;
			loaderInfo.TestSpawnPositionEnabled = TestSpawn.UsePosition;
			loaderInfo.TestSpawnX = (int)TestSpawn.XPosition;
			loaderInfo.TestSpawnY = (int)TestSpawn.YPosition;
			loaderInfo.TestSpawnZ = (int)TestSpawn.ZPosition;
			loaderInfo.TestSpawnRotation = TestSpawn.Rotation;
			loaderInfo.TestSpawnRotationHex = (loaderInfo.TestSpawnRotation > 0) ? true : false;

			return loaderInfo;
		}

		/// <summary>
		/// Deprecated in v2, use <see cref="WriteConfigs"/>
		/// </summary>
		/// <param name="path"></param>
		public void WriteToLoaderInfo(string path, ManagerSettings managerSettings)
		{
			if (Directory.Exists(path))
			{
				SADXLoaderInfo loaderInfo = ConvertToLoaderInfo(managerSettings);
				string loaderInfoPath = Path.Combine(path, "SADXModLoader.ini");

				IniSerializer.Serialize(loaderInfo, loaderInfoPath);
			}
			else
			{
				MessageWindow message = new MessageWindow(Lang.GetString("MessageWindow.Errors.LoaderFailedToSave.Title"), Lang.GetString("MessageWindow.Errors.LoaderFailedToSave"),
					icon: MessageWindow.Icons.Error);
				message.ShowDialog();
			}
		}

		/// <summary>
		/// Loads LoaderInfo and SADXConfigFile into GameSettings. This is intended to only be used for a first boot.
		/// </summary>
		public void LoadConfigs()
		{
			SADXLoaderInfo info = new();
			SADXConfigFile config = new();
			var JsonSerializerSettings = new JsonSerializerOptions();
			JsonSerializerSettings.DefaultIgnoreCondition = JsonIgnoreCondition.WhenWritingNull;

			string loaderInfoPath = Path.Combine(App.CurrentGame.gameDirectory, "mods/SADXModLoader.ini");
			if (File.Exists(loaderInfoPath))
				info = IniSerializer.Deserialize<SADXLoaderInfo>(loaderInfoPath);

            string configPath = Path.Combine(App.CurrentGame.gameDirectory, App.CurrentGame.GameConfigFile[0]);
			if (File.Exists(configPath))
				config = IniSerializer.Deserialize<SADXConfigFile>(configPath);

			Graphics.LoadConfigs(ref info, ref config);
			Controller.LoadConfigs(ref info, ref config);
			Sound.LoadConfigs(ref info, ref config);
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
			SADXLoaderInfo info = new SADXLoaderInfo();
			SADXConfigFile config = new SADXConfigFile();

			info.Mods = EnabledMods;
			info.EnabledCodes = EnabledCodes;

			Graphics.ToConfigs(ref info, ref config);
			Controller.ToConfigs(ref info, ref config);
			Sound.ToConfigs(ref info, ref config);
			TestSpawn.ToConfigs(ref info);
			Patches.ToConfigs(ref info);

			// Debug Settings
			info.DebugConsole = DebugSettings.EnableDebugConsole;
			info.DebugScreen = DebugSettings.EnableDebugScreen;
			info.DebugFile = DebugSettings.EnableDebugFile;
			info.DebugCrashLog = DebugSettings.EnableDebugCrashLog;

			if (Directory.Exists(GamePath))
			{
				string loaderInfoPath = Path.Combine(GamePath, "mods/SADXModLoader.ini");
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

		/// <summary>
		/// Deserializes an SADX GameSettings JSON File and returns a populated class.
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

					// Version update changes go here.
					switch ((SADXSettingsVersions)settings.SettingsVersion)
					{
						case SADXSettingsVersions.v1:
							// Update to Version 2
							settings.SettingsVersion = (int)SADXSettingsVersions.v2;
							SADXConfigFile config = new();
							string configPath = Path.Combine(settings.GamePath, App.CurrentGame.GameConfigFile[0]);
							if (File.Exists(configPath))
								config = IniSerializer.Deserialize<SADXConfigFile>(configPath);

							settings.Graphics.LoadGameConfig(ref config);
							settings.Controller.LoadGameConfig(ref config);
							settings.Sound.LoadGameConfig(ref config);
							break;
					}

					return settings;
				}
				else
					return new();
			}
			catch (Exception ex)
			{
				new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), Lang.GetString("MessageWindow.Errors.ProfileLoad") + "\n\n" + ex.Message, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error).ShowDialog();
			}

			return new();
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
