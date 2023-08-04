using System.Collections.Generic;
using System.ComponentModel;
using SAModManager.Ini;

namespace SAModManager.ManagerSettings.SADX
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

		[DefaultValue(false)]
		public bool Enable43ResolutionRatio { get; set; }              // SADXLoaderInfo.ForseAspectRatio

		[DefaultValue(true)]
		public bool EnableVsync { get; set; } = true;           // SADXLoaderInfo.EnableVSync

		[DefaultValue(true)]
		public bool EnablePauseOnInactive { get; set; } = true;     // SADXLoaderInfo.PauseWhenInactive

		[DefaultValue(false)]
		public bool EnableDynamicBuffer { get; set; }           // SADXLoaderInfo.EnableDynamicBuffer

		[DefaultValue(false)]
		public bool EnableBorderless { get; set; }                    // SADXLoaderInfo.Borderless

		[DefaultValue(true)]
		public bool EnableScreenScaling { get; set; } = true;     // SADXLoaderInfo.StretchFullscreen

		[DefaultValue(false)]
		public bool EnableCustomWindow { get; set; }              // SADXLoaderInfo.CustomWindowSize

		[DefaultValue(640)]
		public int CustomWindowWidth { get; set; } = 640;             // SADXLoaderInfo.WindowWidth

		[DefaultValue(480)]
		public int CustomWindowHeight { get; set; } = 480;            // SADXLoaderInfo.WindowHeight

		[DefaultValue(false)]
		public bool EnableKeepResolutionRatio { get; set; }     // SADXLoaderInfo.MaintainWindowAspectRatio

		[DefaultValue(false)]
		public bool EnableResizableWindow { get; set; }               // SADXLoaderInfo.ResizableWindow

		[DefaultValue((int)FillMode.Fill)]
		public int FillModeBackground { get; set; } = (int)FillMode.Fill;   // SADXLoaderInfo.BackgroundFillMode

		[DefaultValue((int)FillMode.Fit)]
		public int FillModeFMV { get; set; } = (int)FillMode.Fit;           // SADXLoaderInfo.FmvFillMode

		[DefaultValue(TextureFilter.temp)]
		public TextureFilter ModeTextureFiltering { get; set; }

		[DefaultValue(TextureFilter.temp)]
		public TextureFilter ModeUIFiltering { get; set; }

		[DefaultValue(true)]
		public bool EnableUIScaling { get; set; } = true;              // SADXLoaderInfo.ScaleHud

		[DefaultValue(true)]
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
		[DefaultValue(true)]
		public bool EnabledInputMod { get; set; }   // SADXLoaderInfo.InputModEnabled

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
		[DefaultValue(true)]
		public bool EnableBassMusic { get; set; }               // SADXLoaderInfo.EnableBassMusic

		[DefaultValue(false)]
		public bool EnableBassSFX { get; set; }                 // SADXLoaderInfo.EnableBassSFX

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
	}

	public class TestSpawnSettings
	{
		[DefaultValue(-1)]
		public int LevelIndex { get; set; } = -1;       // SADXLoaderInfo.TestSpawnLevel

		[DefaultValue(0)]
		public int ActIndex { get; set; } = 0;          // SADXLoaderInfo.TestSpawnAct

		[DefaultValue(-1)]
		public int CharacterIndex { get; set; } = -1;   // SADXLoaderInfo.TestSpawnCharacter

		[DefaultValue(-1)]
		public int EventIndex { get; set; } = -1;       // SADXLoaderInfo.TestSpawnEvent

		[DefaultValue(-1)]
		public int GameModeIndex { get; set; } = -1;    // SADXLoaderInfo.TestSpawnGameMode

		[DefaultValue(-1)]
		public int SaveIndex { get; set; } = -1;      // SADXLoaderInfo.TestSpawnSaveID

		[DefaultValue(false)]
		public bool UsePosition { get; set; } = false; // SADXLoaderInfo.TestSpawnPositionEnabled

		[DefaultValue(false)]
		public bool UseRotation { get; set; } = false;     // SADXLoaderInfo.TestSpawnRotationHex

		[DefaultValue(0)]
		public float XPosition { get; set; } = 0;            // SADXLoaderInfo.TestSpawnX

		[DefaultValue(0)]
		public float YPosition { get; set; } = 0;            // SADXLoaderInfo.TestSpawnY

		[DefaultValue(0)]
		public float ZPosition { get; set; } = 0;            // SADXLoaderInfo.TestSpawnZ

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

			UsePosition = oldSettings.TestSpawnPositionEnabled;
			UseRotation = oldSettings.TestSpawnRotationHex;

			XPosition = oldSettings.TestSpawnX;
			YPosition = oldSettings.TestSpawnY;
			ZPosition = oldSettings.TestSpawnZ;
			Rotation = oldSettings.TestSpawnRotation;
		}
	}

	public class GamePatches
	{
		[DefaultValue(true)]
		public bool HRTFSound { get; set; }         // SADXLoaderInfo.HRTFSound
		[DefaultValue(true)]
		public bool CCEF { get; set; }              // SADXLoaderInfo.CCEF
		[DefaultValue(true)]
		public bool PolyBuff { get; set; } //vertex color	// SADXLoaderInfo.PolyBuff
		[DefaultValue(true)]
		public bool MaterialColorFix { get; set; }      // SADXLoaderInfo.MaterialColorFix
		[DefaultValue(true)]
		public bool InterpolationFix { get; set; }      // SADXLoaderInfo.InterpolationFix
		[DefaultValue(true)]
		public bool FovFix { get; set; }                // SADXLoaderInfo.FovFix
		[DefaultValue(true)]
		public bool SCFix { get; set; }                 // SADXLoaderInfo.SCFix
		[DefaultValue(true)]
		public bool Chaos2CrashFix { get; set; }        // SADXLoaderInfo.Chaos2CrashFix
		[DefaultValue(true)]
		public bool ChunkSpecFix { get; set; }          // SADXLoaderInfo.ChunkSpecFix
		[DefaultValue(true)]
		public bool E102PolyFix { get; set; }           // SADXLoaderInfo.E102PolyFix
		[DefaultValue(true)]
		public bool ChaoPanelFix { get; set; }          // SADXLoaderInfo.ChaoPanelFix
		[DefaultValue(true)]
		public bool PixelOffSetFix { get; set; }        // SADXLoaderInfo.PixelOffsetFix
		[DefaultValue(true)]
		public bool LightFix { get; set; }              // SADXLoaderInfo.LightFix
		[DefaultValue(true)]
		public bool KillGbix { get; set; }              // SADXLoaderInfo.KillGbix
		[DefaultValue(true)]
		public bool DisableCDCheck { get; set; }        // SADXLoaderInfo.DisableCDCheck

		/// <summary>
		/// Converts from original settings file.
		/// </summary>
		/// <param name="oldSettings"></param>
		public void ConvertFromV0(SADXLoaderInfo oldSettings)
		{

		}
	}

	public class GameSettings
	{
		public enum SADXSettingsVersion
		{
			v0 = 0,
			v1 = 1
		}

		[IniAlwaysInclude]
		[DefaultValue(SADXSettingsVersion.v1)]
		public SADXSettingsVersion SettingsVersion { get; set; }

		/// <summary>
		/// Graphics Settings for SADX.
		/// </summary>
		[IniAlwaysInclude]
		public GraphicsSettings Graphics { get; set; }

		/// <summary>
		/// Controller Settings for SADX.
		/// </summary>
		[IniAlwaysInclude]
		public ControllerSettings Controller { get; set; }

		/// <summary>
		/// Sound Settings for SADX.
		/// </summary>
		[IniAlwaysInclude]
		public SoundSettings Sound { get; set; }

		/// <summary>
		/// TestSpawn Settings for SADX.
		/// </summary>
		public TestSpawnSettings TestSpawn { get; set; }

		/// <summary>
		/// Patches for SADX.
		/// </summary>
		[IniAlwaysInclude]
		public GamePatches Patches { get; set; }

		/// <summary>
		/// Enabled Mods for SADX.
		/// </summary>
		[IniName("Mod")]
		[IniCollection(IniCollectionMode.NoSquareBrackets, StartIndex = 1)]
		public List<string> EnabledMods { get; set; }		// 
		/// <summary>
		/// Enabled Codes for SADX.
		/// </summary>
		[IniName("Code")]
		[IniCollection(IniCollectionMode.NoSquareBrackets, StartIndex = 1)]
		public List<string> EnabledCodes { get; set; }

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

			SettingsVersion = SADXSettingsVersion.v1;
			EnabledMods = oldSettings.Mods;
			EnabledCodes = oldSettings.EnabledCodes;
		}
	}
}
