using IniFile;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ModManagerWPF.Game
{
	//System.Threading.Thread controllerThread;
	public class ControllerConfig
	{
		[IniName("buttons")]
		public int ButtonCount { get; set; }
		[IniName("pad")]
		[IniCollection(IniCollectionMode.NoSquareBrackets)]
		public int[] ButtonSettings { get; set; }
	}
	enum FrameRate
	{
		Invalid,
		High,
		Normal,
		Low
	}

	public class GameConfigFile
	{
		[IniName("sonicDX")]
		public GameConfig GameConfig { get; set; }
		[IniCollection(IniCollectionMode.IndexOnly)]
		public Dictionary<string, ControllerConfig> Controllers { get; set; }
	}

	public class GameConfig
	{
		[IniName("framerate")]
		public int FrameRate { get; set; }
		[IniAlwaysInclude]
		[IniName("fogemulation")]
		public int Foglation { get; set; }
		[IniAlwaysInclude]
		[IniName("sound3d")]
		public int Sound3D { get; set; }
		[IniAlwaysInclude]
		[IniName("screensize")]
		public int ScreenSize { get; set; }
		[IniAlwaysInclude]
		[IniName("cliplevel")]
		public int ClipLevel { get; set; }
		[IniAlwaysInclude]
		[IniName("sevoice")]
		public int SEVoice { get; set; }
		[IniAlwaysInclude]
		[IniName("bgm")]
		public int BGM { get; set; }
		[IniAlwaysInclude]
		[IniName("screen")]
		public int FullScreen { get; set; }
		[IniAlwaysInclude]
		[IniName("mousemode")]
		public int MouseMode { get; set; }
		[IniAlwaysInclude]
		[IniName("bgmv")]
		public int BGMVolume { get; set; }
		[IniAlwaysInclude]
		[IniName("voicev")]
		public int VoiceVolume { get; set; }
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
}
