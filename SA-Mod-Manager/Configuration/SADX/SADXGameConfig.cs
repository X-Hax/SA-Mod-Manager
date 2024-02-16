using SAModManager.Ini;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAModManager.Configuration.SADX
{
	enum SADXFrameRate
	{
		Invalid,
		High,
		Normal,
		Low
	}

	public class SADXConfig
	{
		[IniName("framerate")]
		public int FrameRate { get; set; } = 1;
		[IniAlwaysInclude]
		[IniName("fogemulation")]
		public int Foglation { get; set; } = 0;
		[IniAlwaysInclude]
		[IniName("sound3d")]
		[DefaultValue(1)]
		public int Sound3D { get; set; } = 1;
		[IniAlwaysInclude]
		[IniName("screensize")]
		public int ScreenSize { get; set; } = 2;
		[IniAlwaysInclude]
		[IniName("cliplevel")]
		public int ClipLevel { get; set; } = 0;
		[IniAlwaysInclude]
		[IniName("sevoice")]
		[DefaultValue(1)]
		public int SEVoice { get; set; } = 1;
		[IniAlwaysInclude]
		[IniName("bgm")]
		[DefaultValue(1)]
		public int BGM { get; set; } = 1;
		[IniAlwaysInclude]
		[IniName("screen")]
		public int FullScreen { get; set; } = 0;
		[IniAlwaysInclude]
		[IniName("mousemode")]
		public int MouseMode { get; set; }
		[IniAlwaysInclude]
		[IniName("bgmv")]
		[DefaultValue(100)]
		public int BGMVolume { get; set; } = 100;
		[IniAlwaysInclude]
		[IniName("voicev")]
		[DefaultValue(100)]
		public int VoiceVolume { get; set; } = 100;
		[IniName("language")]
		public int Language { get; set; }
		[IniName("padconfig")]
		public string PadConfig { get; set; }

		[IniName("cmd0")]
		public ushort MouseStart { get; set; }
		[IniName("cmd1")]
		public ushort MouseAttack { get; set; }
		[IniName("cmd2")]
		public ushort MouseJump { get; set; }
		[IniName("cmd3")]
		public ushort MouseAction { get; set; }
		[IniName("cmd4")]
		public ushort MouseFlute { get; set; }
	}

	public class SADXConfigFile
	{
		[IniName("sonicDX")]
		public SADXConfig GameConfig { get; set; } = new();
	}
}
