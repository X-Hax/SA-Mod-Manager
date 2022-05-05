using System.Collections.Generic;
using System.ComponentModel;
using IniFile;
using ModManagerCommon;

namespace SADXModManager
{
	enum FillMode
	{
		Stretch = 0,
		Fit = 1,
		Fill = 2
	}

	class SADXLoaderInfo : LoaderInfo
	{
		public bool DebugConsole { get; set; }

		public bool DebugScreen { get; set; }

		public bool DebugFile { get; set; }

		[DefaultValue(true)]
		public bool DebugCrashLog { get; set; }

		public bool? ShowConsole { get { return null; } set { if (value.HasValue) DebugConsole = value.Value; } }

		public bool DisableCDCheck { get; set; }

		[DefaultValue(640)]
		public int HorizontalResolution { get; set; } = 640;

		[DefaultValue(480)]
		public int VerticalResolution { get; set; } = 480;

		public bool ForceAspectRatio { get; set; }

		public bool WindowedFullscreen { get; set; }

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

		[DefaultValue(false)]
		public bool ScaleHud { get; set; }

		[DefaultValue((int)FillMode.Fill)]
		public int BackgroundFillMode { get; set; } = (int)FillMode.Fill;

		[DefaultValue((int)FillMode.Fit)]
		public int FmvFillMode { get; set; } = (int)FillMode.Fit;

		[DefaultValue(false)]
		public bool DisablePolyBuff { get; set; }

		[DefaultValue(false)]
		public bool DisableMaterialColorFix { get; set; }

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
		public int TestSpawnSaveID { get; set; } = -1;

		public SADXLoaderInfo()
		{
			Mods = new List<string>();
			EnabledCodes = new List<string>();
		}
	}
}
