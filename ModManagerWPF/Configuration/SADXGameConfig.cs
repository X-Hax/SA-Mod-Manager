using SAModManager.Ini;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAModManager.Configuration
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

	public class SADXConfigMouseControl
	{
		public enum MouseButtons
		{
			Unset = 0,
			LeftMouse = 1,
			RightMouse = 2,
			MiddleMouse = 3,
			OtherMouse = 4,
			LeftAndRightMouse = 5,
			RightAndLeftMouse = 6,
		}

		public string DisplayName { get; set; }
		public MouseButtons Index { get; set; }

		public SADXConfigMouseControl(string displayName, MouseButtons index)
		{
			DisplayName = displayName;
			Index = index;
		}

		public static List<SADXConfigMouseControl> MouseControls()
		{
			return new List<SADXConfigMouseControl>()
			{
				new("Unset", MouseButtons.Unset),
				new("Left Mouse Button", MouseButtons.LeftMouse),
				new("Right Mouse Button", MouseButtons.RightMouse),
				new("Middle Mouse Button", MouseButtons.MiddleMouse),
				new("Other Mouse Button", MouseButtons.OtherMouse),
				new("Left + Right Mouse Button", MouseButtons.LeftAndRightMouse),
				new("Right + Left Mouse Button", MouseButtons.RightAndLeftMouse),
			};
		}

		public static List<string> MouseActionsList()
		{
			return new List<string>()
			{
				"Start",
				"Attack",
				"Jump",
				"Action",
				"Flute"
			};
		}
	}
}
