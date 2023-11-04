using System.Collections.Generic;
using System.ComponentModel;
using SAModManager.Ini;
using SAModManager.Configuration;

namespace SAModManager
{
	public enum GameType
	{
		SADX,
		SA2
	}

	public enum UpdateUnit
	{
		Always,
		Hours,
		Days,
		Weeks,
	}

	public class LoaderInfo
	{
		[DefaultValue(true)]
		public bool UpdateCheck { get; set; } = true;
		[DefaultValue(true)]
		public bool ModUpdateCheck { get; set; } = true;

		[DefaultValue(UpdateUnit.Weeks)]
		public UpdateUnit UpdateUnit { get; set; } = UpdateUnit.Weeks;
		[DefaultValue(1)]
		public int UpdateFrequency { get; set; } = 1;

		[DefaultValue(0)] public long UpdateTime { get; set; }
		[DefaultValue(0)] public long ModUpdateTime { get; set; }

		[DefaultValue(true)]
		public bool AutoClose { get; set; } = true;

		[IniName("Mod")]
		[IniCollection(IniCollectionMode.NoSquareBrackets, StartIndex = 1)]
		public List<string> Mods { get; set; }
		[IniName("Code")]
		[IniCollection(IniCollectionMode.NoSquareBrackets, StartIndex = 1)]
		public List<string> EnabledCodes { get; set; }

		public LoaderInfo()
		{
			Mods = new List<string>();
			EnabledCodes = new List<string>();
		}
	}

	public class SADXLoaderInfo : LoaderInfo
	{
		enum FillMode
		{
			Stretch = 0,
			Fit = 1,
			Fill = 2
		}

		public bool DebugConsole { get; set; }

		public bool DebugScreen { get; set; }

		public bool DebugFile { get; set; }

		[DefaultValue(true)]
		public bool DebugCrashLog { get; set; } = true;

		public bool? ShowConsole { get { return null; } set { if (value.HasValue) DebugConsole = value.Value; } }


		[DefaultValue(640)]
		public int HorizontalResolution { get; set; } = 640;

		[DefaultValue(480)]
		public int VerticalResolution { get; set; } = 480;

		public bool ForceAspectRatio { get; set; }

		public bool Borderless { get; set; }

		[DefaultValue(true)]
		public bool EnableVsync { get; set; } = true;

		[DefaultValue(true)]
		public bool AutoMipmap { get; set; } = true;

		[DefaultValue(true)]
		public bool TextureFilter { get; set; } = true;

		[DefaultValue(true)]
		public bool PauseWhenInactive { get; set; } = true;

		[DefaultValue(true)]
		public bool StretchFullscreen { get; set; } = true;

		[DefaultValue(1)]
		public int ScreenNum { get; set; } = 1;

		[DefaultValue(1)]
		public int VoiceLanguage { get; set; } = 1;

		[DefaultValue(1)]
		public int TextLanguage { get; set; } = 1;

		public bool CustomWindowSize { get; set; }

		[DefaultValue(640)]
		public int WindowWidth { get; set; } = 640;

		[DefaultValue(480)]
		public int WindowHeight { get; set; } = 480;

		public bool MaintainWindowAspectRatio { get; set; }

		public bool ResizableWindow { get; set; }

		[DefaultValue(true)]
		public bool ScaleHud { get; set; } = true;

		[DefaultValue((int)FillMode.Fill)]
		public int BackgroundFillMode { get; set; } = (int)FillMode.Fill;

		[DefaultValue((int)FillMode.Fit)]
		public int FmvFillMode { get; set; } = (int)FillMode.Fit;

		[DefaultValue(true)]
		public bool EnableBassMusic { get; set; }

		[DefaultValue(false)]
		public bool EnableBassSFX { get; set; }

		[DefaultValue(100)]
		public int SEVolume { get; set; } = 100;
		[DefaultValue(0)]

	#region Manager Settings
		public int Theme { get; set; } = 0;
		[DefaultValue(0)]
		public int Language { get; set; } = 0;
		[DefaultValue(true)]
		public bool InputModEnabled { get; set; }
		[DefaultValue(false)]
		public bool managerOpen { get; set; }
		[DefaultValue(false)]
		public bool devMode { get; set; }

		#endregion

		#region TestSpawn

		[DefaultValue(-1)]
		public int TestSpawnLevel { get; set; } = -1;

		[DefaultValue(0)]
		public int TestSpawnAct { get; set; } = 0;

		[DefaultValue(-1)]
		public int TestSpawnCharacter { get; set; } = -1;

		[DefaultValue(false)]
		public bool TestSpawnPositionEnabled { get; set; } = false;

		[DefaultValue(false)]
		public bool TestSpawnRotationHex { get; set; } = false;

		[DefaultValue(0)]
		public int TestSpawnX { get; set; } = 0;

		[DefaultValue(0)]
		public int TestSpawnY { get; set; } = 0;

		[DefaultValue(0)]
		public int TestSpawnZ { get; set; } = 0;

		[DefaultValue(0)]
		public int TestSpawnRotation { get; set; } = 0;

		[DefaultValue(-1)]
		public int TestSpawnEvent { get; set; } = -1;
		[DefaultValue(-1)]
		public int TestSpawnGameMode { get; set; } = -1;

		[DefaultValue(-1)]
		public int TestSpawnSaveID { get; set; } = -1;
	#endregion

	#region Patches
		[DefaultValue(true)]
		public bool HRTFSound { get; set; }
		[DefaultValue(true)]
		public bool CCEF { get; set; }
		[DefaultValue(true)]
		public bool PolyBuff { get; set; } //vertex color 
		[DefaultValue(true)]
		public bool MaterialColorFix { get; set; }
		[DefaultValue(true)]
		public bool InterpolationFix { get; set; }
		[DefaultValue(true)]
		public bool FovFix { get; set; }
		[DefaultValue(true)]
		public bool SCFix { get; set; }
		[DefaultValue(true)]
		public bool Chaos2CrashFix { get; set; }
		[DefaultValue(true)]
		public bool ChunkSpecFix { get; set; }
		[DefaultValue(true)]
		public bool E102PolyFix { get; set; }
		[DefaultValue(true)]
		public bool ChaoPanelFix { get; set; }
		[DefaultValue(true)]
		public bool PixelOffSetFix { get; set; }
		[DefaultValue(true)]
		public bool LightFix { get; set; }
		[DefaultValue(true)]
		public bool KillGbix { get; set; }
		public bool DisableCDCheck { get; set; }

		#endregion

		public SADXLoaderInfo()
		{
			Mods = new List<string>();
			EnabledCodes = new List<string>();
		}

		/// <summary>
		/// Converts Manager and Profile settings to this old format, here for debugging purposes.
		/// </summary>
		/// <param name="managerSettings"></param>
		/// <param name="gameSettings"></param>
		public void ConvertFromV1(ManagerSettings managerSettings, Configuration.SADX.GameSettings gameSettings)
		{
			// Manager Settings
			devMode = managerSettings.EnableDeveloperMode;
			managerOpen = managerSettings.KeepManagerOpen;
			Theme = managerSettings.Theme;
			Language = managerSettings.Language;
			UpdateCheck = managerSettings.UpdateSettings.EnableManagerBootCheck;
			ModUpdateCheck = managerSettings.UpdateSettings.EnableModsBootCheck;

			// Graphics
			ScreenNum = gameSettings.Graphics.SelectedScreen;
			HorizontalResolution = gameSettings.Graphics.HorizontalResolution;
			VerticalResolution = gameSettings.Graphics.VerticalResolution;
			ForceAspectRatio = gameSettings.Graphics.Enable43ResolutionRatio;
			EnableVsync = gameSettings.Graphics.EnableVsync;
			PauseWhenInactive = gameSettings.Graphics.EnablePauseOnInactive;
			Borderless = gameSettings.Graphics.EnableBorderless;
			StretchFullscreen = gameSettings.Graphics.EnableScreenScaling;
			CustomWindowSize = gameSettings.Graphics.EnableCustomWindow;
			WindowWidth = gameSettings.Graphics.CustomWindowWidth; 
			WindowHeight = gameSettings.Graphics.CustomWindowHeight;
			MaintainWindowAspectRatio = gameSettings.Graphics.EnableKeepResolutionRatio;
			ResizableWindow = gameSettings.Graphics.EnableResizableWindow;
			BackgroundFillMode = gameSettings.Graphics.FillModeBackground;
			FmvFillMode = gameSettings.Graphics.FillModeFMV;
			ScaleHud = gameSettings.Graphics.EnableUIScaling;
			AutoMipmap = gameSettings.Graphics.EnableForcedMipmapping;
            TextureFilter = gameSettings.Graphics.EnableForcedTextureFilter;

			// Input
			InputModEnabled = gameSettings.Controller.EnabledInputMod;

			// Sound
			EnableBassMusic = gameSettings.Sound.EnableBassMusic;
			EnableBassSFX = gameSettings.Sound.EnableBassSFX;
			SEVolume = gameSettings.Sound.SEVolume;

			// Patches
			HRTFSound = gameSettings.Patches.HRTFSound;
			CCEF = gameSettings.Patches.KeepCamSettings;
			PolyBuff = gameSettings.Patches.FixVertexColorRendering;
			MaterialColorFix = gameSettings.Patches.MaterialColorFix;
			InterpolationFix = gameSettings.Patches.InterpolationFix;
			FovFix = gameSettings.Patches.FOVFix;
			SCFix = gameSettings.Patches.SkyChaseResolutionFix;
			Chaos2CrashFix = gameSettings.Patches.Chaos2CrashFix;
			ChunkSpecFix = gameSettings.Patches.ChunkSpecularFix;
			E102PolyFix = gameSettings.Patches.E102NGonFix;
			ChaoPanelFix = gameSettings.Patches.ChaoPanelFix;
			PixelOffSetFix = gameSettings.Patches.PixelOffSetFix;
			LightFix = gameSettings.Patches.LightFix;
			KillGbix = gameSettings.Patches.KillGBIX;
			DisableCDCheck = gameSettings.Patches.DisableCDCheck;

			// Debug
			DebugConsole = gameSettings.DebugSettings.EnableDebugConsole;
			DebugScreen = gameSettings.DebugSettings.EnableDebugScreen;
			DebugFile = gameSettings.DebugSettings.EnableDebugFile;
			DebugCrashLog = gameSettings.DebugSettings.EnableDebugCrashLog;

			// Test Spawn
			TestSpawnLevel = gameSettings.TestSpawn.LevelIndex;
			TestSpawnAct = gameSettings.TestSpawn.ActIndex;
			TestSpawnCharacter = gameSettings.TestSpawn.CharacterIndex;
			TestSpawnEvent = gameSettings.TestSpawn.EventIndex;
			TestSpawnGameMode = gameSettings.TestSpawn.GameModeIndex;
			TestSpawnSaveID = gameSettings.TestSpawn.SaveIndex;
			TestSpawnPositionEnabled = gameSettings.TestSpawn.UsePosition;
			TestSpawnX = (int)gameSettings.TestSpawn.XPosition;
			TestSpawnY = (int)gameSettings.TestSpawn.YPosition;
			TestSpawnZ = (int)gameSettings.TestSpawn.ZPosition;
			TestSpawnRotation = gameSettings.TestSpawn.Rotation;
			TestSpawnRotationHex = (TestSpawnRotation > 0) ? true : false;
		}
	}

}


