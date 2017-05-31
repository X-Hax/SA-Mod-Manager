using System.Collections.Generic;
using System.ComponentModel;
using IniFile;

namespace SADXModManager
{
	enum UpdateUnit
	{
		Always,
		Hours,
		Days,
		Weeks,
	}

	class LoaderInfo
	{
		public bool DebugConsole { get; set; }
		public bool DebugScreen { get; set; }
		public bool DebugFile { get; set; }
		public bool? ShowConsole { get { return null; } set { if (value.HasValue) DebugConsole = value.Value; } }
		public bool DisableCDCheck { get; set; }
		[DefaultValue(640)]
		public int HorizontalResolution { get; set; } = 640;
		[DefaultValue(480)]
		public int VerticalResolution { get; set; } = 480;
		public bool ForceAspectRatio { get; set; }
		[DefaultValue(false)]
		public bool ScaleHud { get; set; }
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
		public bool CustomWindowSize { get; set; }
		[DefaultValue(640)]
		public int WindowWidth { get; set; } = 640;
		[DefaultValue(480)]
		public int WindowHeight { get; set; } = 480;
		public bool MaintainWindowAspectRatio { get; set; }
		public bool ResizableWindow { get; set; }

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
}
