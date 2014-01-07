using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using IniSerializer;

/*
 * To-Do:
 *	Error checking
 *	Input configuration
*/

namespace SADXModManager
{
    public partial class ConfigEditDialog : Form
    {
		private ConfigFile configFile;
		private const string sadxIni = "sonicDX.ini";

        public ConfigEditDialog()
        {
            InitializeComponent();
        }

        private void okButton_Click(object sender, EventArgs e)
        {
			SaveConfigIni();
            Close();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            Close();
        }

		private void ConfigEditDialog_Load(object sender, EventArgs e)
		{
			// Load the config INI upon window load
			// Failsafe for incomplete (canceled) configuration
			LoadConfigIni();

			/*
			 * I suppose this could be moved to the constructor as well as cancelButton_Click
			 * so that it doesn't load the INI every time the window opens, but then modifying
			 * the file externally would require hitting cancel to reload it.
			*/
		}

		private void LoadConfigIni()
		{
			if (File.Exists(sadxIni))
			{
				configFile = new ConfigFile();
				configFile = IniFile.Deserialize<ConfigFile>(sadxIni);

				// Video
				// Display mode
				if (configFile.GameConfig.FullScreen == 1)
					radioFullscreen.Checked = true;
				else
					radioWindowMode.Checked = true;

				// Resolution preset
				comboResolutionPreset.SelectedIndex = configFile.GameConfig.ScreenSize;

				// Framerate
				if (configFile.GameConfig.FrameRate == (int)FrameRate.Invalid || configFile.GameConfig.FrameRate > (int)FrameRate.Low)
				{

					MessageBox.Show("Invalid framerate setting detected.\nDefaulting to \"High\".", "Invalid setting", MessageBoxButtons.OK, MessageBoxIcon.Error);
					comboFramerate.SelectedIndex = (int)FrameRate.High - 1;
				}
				else
					comboFramerate.SelectedIndex = configFile.GameConfig.FrameRate - 1;

				// Clip level
				comboClip.SelectedIndex = configFile.GameConfig.ClipLevel;
				// Fog mode
				comboFog.SelectedIndex = configFile.GameConfig.FogEmulation;

				// Sound
				// Toggles
				check3DSound.Checked = (configFile.GameConfig.Sound3D != 0);
				checkSound.Checked = (configFile.GameConfig.SEVoice != 0);
				checkMusic.Checked = (configFile.GameConfig.BGM != 0);

				// Volume
				numericSoundVol.Value = configFile.GameConfig.VoiceVolume;
				numericBGMVol.Value = configFile.GameConfig.BGMVolume;

				// Mouse
				// Mouse Mode
				if (configFile.GameConfig.MouseMode == 0)
					radioMouseModeHold.Checked = true;
				else
					radioMouseModeRelease.Checked = true;

				// putting this here because it'll get
				// overwritten if I put it in InitalizeComponent
				comboMouseActions.SelectedIndex = 0;
			}
			else
			{
				MessageBox.Show("Unable to find sonicDX.ini.", "File not found", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
		}

		private void SaveConfigIni()
		{
			if (File.Exists(sadxIni))
			{
				configFile.GameConfig.FullScreen = (radioFullscreen.Checked == true) ? 1 : 0;
				configFile.GameConfig.ScreenSize = comboResolutionPreset.SelectedIndex;

				configFile.GameConfig.FrameRate = comboFramerate.SelectedIndex + 1;
				configFile.GameConfig.ClipLevel = comboClip.SelectedIndex;
				configFile.GameConfig.FogEmulation = comboFog.SelectedIndex;

				configFile.GameConfig.Sound3D = (check3DSound.Checked == true) ? 1 : 0;
				configFile.GameConfig.SEVoice = (checkSound.Checked == true) ? 1 : 0;
				configFile.GameConfig.BGM = (checkMusic.Checked == true) ? 1 : 0;

				configFile.GameConfig.VoiceVolume = (int)numericSoundVol.Value;
				configFile.GameConfig.BGMVolume = (int)numericBGMVol.Value;

				configFile.GameConfig.MouseMode = (radioMouseModeHold.Checked == true) ? 0 : 1;
				
				IniFile.Serialize(configFile, sadxIni);
			}
			else
				return;
		}

		private void comboMouseActions_SelectedIndexChanged(object sender, EventArgs e)
		{
			/*
			 * Here, we take the selected Action index, get the Button
			 * assignment, and select it in comboMouseButtons.
			*/
			ushort selected = 0;

			switch (comboMouseActions.SelectedIndex)
			{
				case 0:
					selected = configFile.GameConfig.MouseStart;
					break;
				case 1:
					selected = configFile.GameConfig.MouseAttack;
					break;
				case 2:
					selected = configFile.GameConfig.MouseJump;
					break;
				case 3:
					selected = configFile.GameConfig.MouseAction;
					break;
				case 4:
					selected = configFile.GameConfig.MouseFlute;
					break;

				default:
					selected = 0;
					break;
			}

			comboMouseButtons.SelectedIndex = MouseToInt(selected);
		}

		private void comboMouseButtons_SelectedIndexChanged(object sender, EventArgs e)
		{
			/*
			 * Here, we take the newly selected Button,
			 * and assign it to the selected Action.
			*/
			ushort selected = (ushort)comboMouseButtons.SelectedIndex;

			switch (comboMouseActions.SelectedIndex)
			{
				case 0:
					configFile.GameConfig.MouseStart = IntToMouse(selected);
					break;
				case 1:
					configFile.GameConfig.MouseAttack = IntToMouse(selected);
					break;
				case 2:
					configFile.GameConfig.MouseJump = IntToMouse(selected);
					break;
				case 3:
					configFile.GameConfig.MouseAction = IntToMouse(selected);
					break;
				case 4:
					configFile.GameConfig.MouseFlute = IntToMouse(selected);
					break;

				default:
					break;
			}
		}

		private ushort MouseToInt(ushort n)
		{
			switch (n)
			{
				case 258:
					return 5;
				case 513:
					return 6;
				default:
					return n;
			}
		}

		private ushort IntToMouse(ushort n)
		{
			switch (n)
			{
				case 5:
					return 258;
				case 6:
					return 513;
				default:
					return n;
			}
		}
    }

    class ConfigFile
    {
        [IniName("sonicDX")]
        public GameConfig GameConfig { get; set; }
        [IniCollection(IniCollectionMode.IndexOnly)]
        public Dictionary<string, ControllerConfig> Controllers { get; set; }
    }

    class GameConfig
    {
        [IniName("framerate")]
        public int FrameRate { get; set; }
        [IniName("fogemulation")]
        public int FogEmulation { get; set; }
        [IniName("sound3d")]
        public int Sound3D { get; set; }
        [IniName("screensize")]
        public int ScreenSize { get; set; }
        [IniName("cliplevel")]
        public int ClipLevel { get; set; }
        [IniName("sevoice")]
        public int SEVoice { get; set; }
        [IniName("bgm")]
        public int BGM { get; set; }
        [IniName("screen")]
        public int FullScreen { get; set; }
        [IniName("mousemode")]
        public int MouseMode { get; set; }
        [IniName("bgmv")]
        public int BGMVolume { get; set; }
        [IniName("voicev")]
        public int VoiceVolume { get; set; }
        [IniName("language")]
        public int Language { get; set; }
        [IniName("padconfig")]
        public string PadConfig { get; set; }
        //[IniName("cmd")]
        //[IniCollection(IniCollectionMode.NoSquareBrackets)]
        //public List<int> MouseSettings { get; set; }
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

    enum FrameRate
    {
        Invalid,
        High,
        Normal,
        Low
    }

    enum FogEmulation
    {
        Auto,
        Emulation
    }

	class ControllerConfig
    {
        [IniName("buttons")]
        public int ButtonCount { get; set; }
        [IniName("pad")]
        [IniCollection(IniCollectionMode.NoSquareBrackets)]
        public List<int> ButtonSettings { get; set; }
    }
}
