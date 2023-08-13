using System.Collections.Generic;
using System.ComponentModel;
using SAModManager.Ini;

namespace SAModManager.IniSettings.SADX
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

		/// <summary>
		/// Index for the screen the game will boot on.
		/// </summary>
		[DefaultValue(1)]
		[IniAlwaysInclude]
		public int SelectedScreen { get; set; } = 1;                 // SADXLoaderInfo.ScreenNum

		/// <summary>
		/// Rendering Horizontal Resolution.
		/// </summary>
		[DefaultValue(640)]
		[IniAlwaysInclude]
		public int HorizontalResolution { get; set; } = 640;    // SADXLoaderInfo.HorizontalResolution

		/// <summary>
		/// Rendering Vertical Resolution.
		/// </summary>
		[DefaultValue(480)]
		[IniAlwaysInclude]
		public int VerticalResolution { get; set; } = 480;      // SADXLoaderInfo.VerticalResolution

		/// <summary>
		/// Locks the Resolution to 4:3 Ratio
		/// </summary>
		[DefaultValue(false)]
		[IniAlwaysInclude]
		public bool Enable43ResolutionRatio { get; set; }              // SADXLoaderInfo.ForseAspectRatio

		/// <summary>
		/// V-Sync.
		/// </summary>
		[DefaultValue(true)]
		[IniAlwaysInclude]
		public bool EnableVsync { get; set; } = true;           // SADXLoaderInfo.EnableVSync

		/// <summary>
		/// Enables the window to be paused when not focused.
		/// </summary>
		[DefaultValue(true)]
		[IniAlwaysInclude]
		public bool EnablePauseOnInactive { get; set; } = true;     // SADXLoaderInfo.PauseWhenInactive

		/// <summary>
		/// Changes from static to dynamic buffers for fullscreen. Fixes Alt-Tab but sacrifices some performance.
		/// </summary>
		[DefaultValue(false)]
		[IniAlwaysInclude]
		public bool EnableDynamicBuffer { get; set; }           // SADXLoaderInfo.EnableDynamicBuffer

		/// <summary>
		/// Makes the fullscreen window borderless.
		/// </summary>
		[DefaultValue(false)]
		[IniAlwaysInclude]
		public bool EnableBorderless { get; set; }                    // SADXLoaderInfo.Borderless

		/// <summary>
		/// Scales the screen to window edges in fullscreen.
		/// </summary>
		[DefaultValue(true)]
		[IniAlwaysInclude]
		public bool EnableScreenScaling { get; set; } = true;     // SADXLoaderInfo.StretchFullscreen

		/// <summary>
		/// Enables a custom window size that can be smaller than the resolution is set.
		/// </summary>
		[DefaultValue(false)]
		[IniAlwaysInclude]
		public bool EnableCustomWindow { get; set; }              // SADXLoaderInfo.CustomWindowSize

		/// <summary>
		/// Sets the Width of the Custom Window Size.
		/// </summary>
		[DefaultValue(640)]
		[IniAlwaysInclude]
		public int CustomWindowWidth { get; set; } = 640;             // SADXLoaderInfo.WindowWidth

		/// <summary>
		/// Sets the Height of the Custom Window Size.
		/// </summary>
		[DefaultValue(480)]
		[IniAlwaysInclude]
		public int CustomWindowHeight { get; set; } = 480;            // SADXLoaderInfo.WindowHeight

		/// <summary>
		/// Keeps the Resolution's ratio for the Custom Window size.
		/// </summary>
		[DefaultValue(false)]
		[IniAlwaysInclude]
		public bool EnableKeepResolutionRatio { get; set; }     // SADXLoaderInfo.MaintainWindowAspectRatio

		/// <summary>
		/// Enables resizing of the game window.
		/// </summary>
		[DefaultValue(false)]
		[IniAlwaysInclude]
		public bool EnableResizableWindow { get; set; }               // SADXLoaderInfo.ResizableWindow

		/// <summary>
		/// Method in which the BG's fill the game window.
		/// </summary>
		[DefaultValue((int)FillMode.Fill)]
		[IniAlwaysInclude]
		public int FillModeBackground { get; set; } = (int)FillMode.Fill;   // SADXLoaderInfo.BackgroundFillMode

		/// <summary>
		/// Method in which FMV's fill the game window.
		/// </summary>
		[DefaultValue((int)FillMode.Fit)]
		[IniAlwaysInclude]
		public int FillModeFMV { get; set; } = (int)FillMode.Fit;           // SADXLoaderInfo.FmvFillMode

		/// <summary>
		/// Texture filtering for non-UI textures.
		/// </summary>
		[DefaultValue(TextureFilter.temp)]
		[IniAlwaysInclude]
		public TextureFilter ModeTextureFiltering { get; set; }

		/// <summary>
		/// Texture filtering for UI textures only.
		/// </summary>
		[DefaultValue(TextureFilter.temp)]
		[IniAlwaysInclude]
		public TextureFilter ModeUIFiltering { get; set; }

		/// <summary>
		/// Enables UI Scaling to match the window properly.
		/// </summary>
		[DefaultValue(true)]
		[IniAlwaysInclude]
		public bool EnableUIScaling { get; set; } = true;              // SADXLoaderInfo.ScaleHud

		/// <summary>
		/// Enables mipmapping on all textures being rendered.
		/// </summary>
		[DefaultValue(true)]
		[IniAlwaysInclude]
		public bool EnableForcedMipmapping { get; set; } = true;            // SADXLoaderInfo.AutoMipmap

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
			EnableDynamicBuffer = oldSettings.EnableDynamicBuffer;

			EnableBorderless = oldSettings.Borderless;
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
		}
	}

	public class ControllerSettings
	{
		/// <summary>
		/// SDL2 Input Enabled
		/// </summary>
		[DefaultValue(true)]
		[IniAlwaysInclude]
		public bool EnabledInputMod { get; set; } = true;   // SADXLoaderInfo.InputModEnabled

		/// <summary>
		/// Converts from original settings file.
		/// </summary>
		/// <param name="oldSettings"></param>
		public void ConvertFromV0(SADXLoaderInfo oldSettings)
		{
			EnabledInputMod = oldSettings.InputModEnabled;
		}
	}

	public class SoundSettings
	{
		/// <summary>
		/// Enables BASS as the music backend.
		/// </summary>
		[DefaultValue(true)]
		[IniAlwaysInclude]
		public bool EnableBassMusic { get; set; } = true;             // SADXLoaderInfo.EnableBassMusic

		/// <summary>
		/// Enables the use of BASS for Sound Effects in game.
		/// </summary>
		[DefaultValue(false)]
		[IniAlwaysInclude]
		public bool EnableBassSFX { get; set; } = false;              // SADXLoaderInfo.EnableBassSFX

		/// <summary>
		/// Sound Effect Volume when using BASS for Sound Effects.
		/// </summary>
		[DefaultValue(100)]
		[IniAlwaysInclude]
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
	}

	public class TestSpawnSettings
	{
		/// <summary>
		/// Selected index for the level used by test spawn.
		/// </summary>
		[DefaultValue(-1)]
		[IniAlwaysInclude]
		public int LevelIndex { get; set; } = -1;       // SADXLoaderInfo.TestSpawnLevel

		/// <summary>
		/// Selected index for the act used by test spawn.
		/// </summary>
		[DefaultValue(0)]
		[IniAlwaysInclude]
		public int ActIndex { get; set; } = 0;          // SADXLoaderInfo.TestSpawnAct

		/// <summary>
		/// Selected index for the character used by test spawn.
		/// </summary>
		[DefaultValue(-1)]
		[IniAlwaysInclude]
		public int CharacterIndex { get; set; } = -1;   // SADXLoaderInfo.TestSpawnCharacter

		/// <summary>
		/// Selected index of an event used by test spawn.
		/// </summary>
		[DefaultValue(-1)]
		[IniAlwaysInclude]
		public int EventIndex { get; set; } = -1;       // SADXLoaderInfo.TestSpawnEvent

		/// <summary>
		/// Selected index for the GameMode used by test spawn.
		/// </summary>
		[DefaultValue(-1)]
		[IniAlwaysInclude]
		public int GameModeIndex { get; set; } = -1;    // SADXLoaderInfo.TestSpawnGameMode

		/// <summary>
		/// Selected save file index used by test spawn.
		/// </summary>
		[DefaultValue(-1)]
		[IniAlwaysInclude]
		public int SaveIndex { get; set; } = -1;      // SADXLoaderInfo.TestSpawnSaveID

		/// <summary>
		/// Enables the Manual settings for Character, Level, and Act.
		/// </summary>
		[DefaultValue(false)]
		[IniAlwaysInclude]
		public bool UseManual { get; set; } = false;

		/// <summary>
		/// Enables manually modifying the start position when using test spawn.
		/// </summary>
		[DefaultValue(false)]
		[IniAlwaysInclude]
		public bool UsePosition { get; set; } = false; // SADXLoaderInfo.TestSpawnPositionEnabled

		/// <summary>
		/// X Position where the player will spawn using test spawn.
		/// </summary>
		[DefaultValue(0f)]
		[IniAlwaysInclude]
		public float XPosition { get; set; } = 0f;            // SADXLoaderInfo.TestSpawnX

		/// <summary>
		/// Y Position where the player will spawn using test spawn.
		/// </summary>
		[DefaultValue(0f)]
		[IniAlwaysInclude]
		public float YPosition { get; set; } = 0f;            // SADXLoaderInfo.TestSpawnY

		/// <summary>
		/// Z Position where the player will spawn using test spawn.
		/// </summary>
		[DefaultValue(0f)]
		[IniAlwaysInclude]
		public float ZPosition { get; set; } = 0f;            // SADXLoaderInfo.TestSpawnZ

		/// <summary>
		/// Initial Y Rotation for the player when using test spawn.
		/// </summary>
		[DefaultValue(0)]
		[IniAlwaysInclude]
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

			UsePosition = oldSettings.TestSpawnPositionEnabled;

			XPosition = oldSettings.TestSpawnX;
			YPosition = oldSettings.TestSpawnY;
			ZPosition = oldSettings.TestSpawnZ;
			Rotation = oldSettings.TestSpawnRotation;
		}
	}

	public class GamePatches
	{
		/// <summary>
		/// Modifies the sound system. Only SF94 knows why.
		/// </summary>
		[DefaultValue(true)]
        [IniAlwaysInclude]
        public bool HRTFSound { get; set; } = true;        // SADXLoaderInfo.HRTFSound

		/// <summary>
		/// Fixes the game turning off free cam when dying if it was previously set.
		/// </summary>
		[DefaultValue(true)]
		[IniAlwaysInclude]
		public bool KeepCamSettings { get; set; } = true;           // SADXLoaderInfo.CCEF

		/// <summary>
		/// Fixes the game's rendering to properly allow mesh's with vertex colors to be rendered with those vertex colors.
		/// </summary>
		[DefaultValue(true)]
		[IniAlwaysInclude]
		public bool FixVertexColorRendering { get; set; } = true; //vertex color	// SADXLoaderInfo.PolyBuff

		/// <summary>
		/// Fixes the material colors so they render properly.
		/// </summary>
		[DefaultValue(true)]
		[IniAlwaysInclude]
		public bool MaterialColorFix { get; set; } = true;      // SADXLoaderInfo.MaterialColorFix

		/// <summary>
		/// Fixes an issue where rotations don't always follow the shortest path.
		/// </summary>
		[DefaultValue(true)]
		[IniAlwaysInclude]
		public bool InterpolationFix { get; set; } = true;      // SADXLoaderInfo.InterpolationFix

		/// <summary>
		/// Fixes the game's FOV.
		/// </summary>
		[DefaultValue(true)]
		[IniAlwaysInclude]
		public bool FOVFix { get; set; } = true;     // SADXLoaderInfo.FovFix

		/// <summary>
		/// Fixes Skychase to properly render.
		/// </summary>
		[DefaultValue(true)]
		[IniAlwaysInclude]
		public bool SkyChaseResolutionFix { get; set; } = true; // SADXLoaderInfo.SCFix

		/// <summary>
		/// Fixes a bug that will cause the game to crash when fighting Chaos 2.
		/// </summary>
		[DefaultValue(true)]
		[IniAlwaysInclude]
		public bool Chaos2CrashFix { get; set; } = true;        // SADXLoaderInfo.Chaos2CrashFix

		/// <summary>
		/// Fixes specular rendering on Chunk models.
		/// </summary>
		[DefaultValue(true)]
		[IniAlwaysInclude]
		public bool ChunkSpecularFix { get; set; } = true;         // SADXLoaderInfo.ChunkSpecFix

		/// <summary>
		/// Fixes rendering on E-102's gun which uses an N-Gon.
		/// </summary>
		[DefaultValue(true)]
		[IniAlwaysInclude]
		public bool E102NGonFix { get; set; } = true;           // SADXLoaderInfo.E102PolyFix

		/// <summary>
		/// Fixes Chao Panel rendering.
		/// </summary>
		[DefaultValue(true)]
		[IniAlwaysInclude]
		public bool ChaoPanelFix { get; set; } = true;          // SADXLoaderInfo.ChaoPanelFix

		/// <summary>
		/// Fixes a slight pixel offset in the window.
		/// </summary>
		[DefaultValue(true)]
		[IniAlwaysInclude]
		public bool PixelOffSetFix { get; set; } = true;        // SADXLoaderInfo.PixelOffsetFix

		/// <summary>
		/// Fixes lights
		/// </summary>
		[DefaultValue(true)]
		[IniAlwaysInclude]
		public bool LightFix { get; set; } = true;           // SADXLoaderInfo.LightFix

		/// <summary>
		/// Removes GBIX processing for most textures. UI is excluded.
		/// </summary>
		[DefaultValue(true)]
		[IniAlwaysInclude]
		public bool KillGBIX { get; set; } = true;            // SADXLoaderInfo.KillGbix

		/// <summary>
		/// Disables the built in CD Check.
		/// </summary>
		[DefaultValue(false)]
		[IniAlwaysInclude]
		public bool DisableCDCheck { get; set; } = false;       // SADXLoaderInfo.DisableCDCheck

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
			InterpolationFix = oldSettings.InterpolationFix;
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
		}
	}

	public class GameSettings
	{
		public enum SADXSettingsVersions
		{
			v0 = 0,
			v1 = 1
		}

		/// <summary>
		/// Versioning for the SADX Settings file.
		/// </summary>
		[IniAlwaysInclude]
		[DefaultValue((int)SADXSettingsVersions.v1)]
		public int SettingsVersion { get; set; } = (int)SADXSettingsVersions.v1;

		[IniAlwaysInclude]
		[DefaultValue(0)]
		public int ProfileIndex { get; set; } = 0;

		/// <summary>
		/// Graphics Settings for SADX.
		/// </summary>
		[IniAlwaysInclude]
		public GraphicsSettings Graphics { get; set; } = new();

		/// <summary>
		/// Controller Settings for SADX.
		/// </summary>
		[IniAlwaysInclude]
		public ControllerSettings Controller { get; set; } = new();

		/// <summary>
		/// Sound Settings for SADX.
		/// </summary>
		[IniAlwaysInclude]
		public SoundSettings Sound { get; set; } = new();

		/// <summary>
		/// TestSpawn Settings for SADX.
		/// </summary>
		public TestSpawnSettings TestSpawn { get; set; } = new();

		/// <summary>
		/// Patches for SADX.
		/// </summary>
		[IniAlwaysInclude]
		public GamePatches Patches { get; set; } = new();

		/// <summary>
		/// Debug Settings.
		/// </summary>
		[IniAlwaysInclude]
		public DebugSettings DebugSettings { get; set; } = new();

		/// <summary>
		/// Path to the game install saved with this configuration.
		/// </summary>
		[IniAlwaysInclude]
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
	}
}
