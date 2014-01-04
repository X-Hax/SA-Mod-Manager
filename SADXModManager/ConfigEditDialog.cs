using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using IniSerializer;

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

				/*
				//	First let's handle the video settings
				 */

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

				/*
				//	Now for sound...
				 */

				// Toggles
				check3DSound.Checked = (configFile.GameConfig.Sound3D != 0);
				checkSound.Checked = (configFile.GameConfig.SEVoice != 0);
				checkMusic.Checked = (configFile.GameConfig.BGM != 0);

				// Volume
				numericSoundVol.Value = configFile.GameConfig.VoiceVolume;
				numericBGMVol.Value = configFile.GameConfig.BGMVolume;

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
				if (radioFullscreen.Checked)
					configFile.GameConfig.FullScreen = 1;
				else
					configFile.GameConfig.FullScreen = 0;

				configFile.GameConfig.ScreenSize = comboResolutionPreset.SelectedIndex;
				configFile.GameConfig.FrameRate = comboFramerate.SelectedIndex + 1;
				configFile.GameConfig.ClipLevel = comboClip.SelectedIndex;
				configFile.GameConfig.FogEmulation = comboFog.SelectedIndex;

				configFile.GameConfig.Sound3D = (check3DSound.Checked == true) ? 1 : 0;
				configFile.GameConfig.SEVoice = (checkSound.Checked == true) ? 1 : 0;
				configFile.GameConfig.BGM = (checkMusic.Checked == true) ? 1 : 0;

				configFile.GameConfig.VoiceVolume = (int)numericSoundVol.Value;
				configFile.GameConfig.BGMVolume = (int)numericBGMVol.Value;
				
				IniFile.Serialize(configFile, sadxIni);
			}
			else
				return;
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
        [IniName("cmd")]
        [IniCollection(IniCollectionMode.NoSquareBrackets)]
        public List<int> MouseSettings { get; set; }
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
