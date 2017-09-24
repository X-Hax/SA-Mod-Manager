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

		public SADXLoaderInfo()
		{
			Mods = new List<string>();
			EnabledCodes = new List<string>();
		}
	}
}
