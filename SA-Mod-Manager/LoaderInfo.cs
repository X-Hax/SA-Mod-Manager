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
        
		[DefaultValue(true)]
        public bool WindowedFullscreen { get; set; } = true;

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

		[IniAlwaysInclude]
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

	#region Manager Settings
		public int Theme { get; set; } = 0;
		[DefaultValue(0)]
		public int Language { get; set; } = 0;
		[DefaultValue(true)]
		public bool InputModEnabled { get; set; }
		[DefaultValue(true)]
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
		public bool NodeLimit { get; set; }
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
        [DefaultValue(true)]
        public bool ExtendedSaveSupport { get; set; }

        #endregion

        public SADXLoaderInfo()
		{
			Mods = new List<string>();
			EnabledCodes = new List<string>();
		}

	}


	public class SA2LoaderInfo : LoaderInfo
    {
        public bool DebugConsole { get; set; }
        public bool DebugScreen { get; set; }
        public bool DebugFile { get; set; }
        [DefaultValue(true)]
        public bool DebugCrashLog { get; set; } = true;
        public bool? ShowConsole { get { return null; } set { if (value.HasValue) DebugConsole = value.Value; } }
        [DefaultValue(true)]
        public bool PauseWhenInactive { get; set; }

        [DefaultValue(1)]
        public int ScreenNum { get; set; } = 1;
        [DefaultValue(true)]
        public bool WindowedFullscreen { get; set; } = true; //previously borderless window

        [DefaultValue(false)]
        public bool FullScreen { get; set; }
        public bool SkipIntro { get; set; }

        [DefaultValue(640)]
        public int HorizontalResolution { get; set; } = 640;
        [DefaultValue(480)]
        public int VerticalResolution { get; set; } = 480;
        [DefaultValue(1)]
        public int VoiceLanguage { get; set; } = 1;
        [DefaultValue(0)]
        public int TextLanguage { get; set; } = 0;
        public bool CustomWindowSize { get; set; }
        [DefaultValue(640)]
        public int WindowWidth { get; set; } = 640;
        [DefaultValue(480)]
        public int WindowHeight { get; set; } = 480;
        [DefaultValue(false)]
        public bool ResizableWindow { get; set; }
        [DefaultValue(false)]
        public bool MaintainAspectRatio { get; set; }

        #region Manager Settings
        public int Theme { get; set; } = 0;
        [DefaultValue(0)]
        public int Language { get; set; } = 0;
        [DefaultValue(true)]
        public bool InputModEnabled { get; set; }
        [DefaultValue(true)]
        public bool managerOpen { get; set; }
        [DefaultValue(false)]
        public bool devMode { get; set; }

        #endregion

        #region testSpawn
        [DefaultValue(-1)]
        public int TestSpawnLevel { get; set; } = -1;
        [DefaultValue(-1)]
        public int TestSpawnCharacter { get; set; } = -1;
        [DefaultValue(0)]
        public int TestSpawnPlayer2 { get; set; } = -1;
        [DefaultValue(0)]
        public int TestSpawnX { get; set; } = 0;
        [DefaultValue(0)]
        public int TestSpawnY { get; set; } = 0;
        [DefaultValue(0)]
        public int TestSpawnZ { get; set; } = 0;
        [DefaultValue(false)]
        public bool TestSpawnPositionEnabled { get; set; } = false;

        [DefaultValue(-1)]
        public int TestSpawnEvent { get; set; } = -1;
        [DefaultValue(-1)]
        public int TestSpawnSaveID { get; set; } = -1;
		#endregion


		#region Patches
		[DefaultValue(true)]
		public bool FramerateLimiter { get; set; } = true;
		[DefaultValue(true)]
		public bool DisableExitPrompt { get; set; } = true;
        [DefaultValue(true)]
        public bool SyncLoad { get; set; } = true;
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

        #endregion

        public SA2LoaderInfo()
        {
            Mods = new List<string>();
            EnabledCodes = new List<string>();
        }
    }

}


