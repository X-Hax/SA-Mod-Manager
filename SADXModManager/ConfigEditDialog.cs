using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using IniSerializer;

namespace SADXModManager
{
    public partial class ConfigEditDialog : Form
    {
        public ConfigEditDialog()
        {
            InitializeComponent();
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            Close();
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
