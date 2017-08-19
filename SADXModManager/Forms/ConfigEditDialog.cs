using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using IniFile;
using SADXModManager.Controls;
using SharpDX.DirectInput;

/*
 * To-Do:
 *	Error checking
*/

namespace SADXModManager.Forms
{
	public partial class ConfigEditDialog : Form
	{
		private ConfigFile configFile;
		private const string sadxIni = "sonicDX.ini";
		DirectInput directInput;
		Joystick inputDevice;
		readonly List<ButtonControl> buttonControls = new List<ButtonControl>();
		readonly List<ControllerConfigInternal> controllerConfig = new List<ControllerConfigInternal>();

		static readonly string[] actionNames =
		{
			"Rotate camera right",
			"Rotate camera left",
			"Start/Set",
			"Jump",
			"Cancel/Attack",
			"Action",
			"Flute",
			"Z Button"
		};

		static readonly int[] buttonIDs = { 16, 17, 3, 2, 1, 10, 9, 8 };

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
			directInput = new DirectInput();
			var list = directInput.GetDevices(DeviceClass.GameControl, DeviceEnumerationFlags.AttachedOnly);
			if (list.Count > 0)
			{
				inputDevice = new Joystick(directInput, list.First().InstanceGuid);
				tableLayoutPanel1.RowCount = actionNames.Length;
				for (int i = 0; i < actionNames.Length; i++)
				{
					tableLayoutPanel1.Controls.Add(new Label
					{
						AutoSize = true,
						Anchor = AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Top,
						Text = actionNames[i],
						TextAlign = ContentAlignment.MiddleLeft
					}, 0, i);
					ButtonControl buttonControl = new ButtonControl { Enabled = false };
					tableLayoutPanel1.Controls.Add(buttonControl, 1, i);
					buttonControls.Add(buttonControl);
					buttonControl.SetButtonClicked += buttonControl_SetButtonClicked;
					buttonControl.ClearButtonClicked += buttonControl_ClearButtonClicked;
					if (i == tableLayoutPanel1.RowStyles.Count)
						tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.AutoSize));
				}
			}
			else
			{
				tabControl1.TabPages.Remove(tabPage_Controller);
			}
			// Load the config INI upon window load
			LoadConfigIni();
		}

		private void LoadConfigIni()
		{
			configFile = File.Exists(sadxIni) ? IniSerializer.Deserialize<ConfigFile>(sadxIni) : new ConfigFile();
			if (configFile.GameConfig == null)
			{
				configFile.GameConfig = new GameConfig
				{
					FrameRate = (int)FrameRate.High,
					Sound3D = 1,
					SEVoice = 1,
					BGM = 1,
					BGMVolume = 100,
					VoiceVolume = 100
				};
			}
			if (configFile.Controllers == null)
				configFile.Controllers = new Dictionary<string, ControllerConfig>();

			// Video
			// Display mode
			if (configFile.GameConfig.FullScreen == 1)
				radioFullscreen.Checked = true;
			else
				radioWindowMode.Checked = true;

			// Framerate
			if (configFile.GameConfig.FrameRate == (int)FrameRate.Invalid || configFile.GameConfig.FrameRate > (int)FrameRate.Low)
			{
				MessageBox.Show("Invalid framerate setting detected.\nDefaulting to \"High\".", "Invalid setting", MessageBoxButtons.OK, MessageBoxIcon.Error);
				comboFramerate.SelectedIndex = (int)FrameRate.High - 1;
			}
			else
			{
				comboFramerate.SelectedIndex = configFile.GameConfig.FrameRate - 1;
			}

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

			foreach (KeyValuePair<string, ControllerConfig> item in configFile.Controllers)
			{
				int[] cfg = Enumerable.Repeat(-1, buttonIDs.Length).ToArray();
				for (int i = 0; i < item.Value.ButtonSettings.Length; i++)
				{
					if (Array.IndexOf(buttonIDs, item.Value.ButtonSettings[i]) != -1)
					{
						cfg[Array.IndexOf(buttonIDs, item.Value.ButtonSettings[i])] = i;
					}
				}
				controllerConfig.Add(new ControllerConfigInternal { Name = item.Key, Buttons = cfg });
				controllerConfigSelect.Items.Add(item.Key);
			}

			if (inputDevice == null)
			{
				return;
			}

			for (int i = 0; i < controllerConfig.Count; i++)
			{
				if (controllerConfig[i].Name == configFile.GameConfig.PadConfig)
				{
					controllerConfigSelect.SelectedIndex = i;
					break;
				}
			}
		}

		private void SaveConfigIni()
		{
			configFile.GameConfig.FullScreen = radioFullscreen.Checked ? 1 : 0;

			configFile.GameConfig.FrameRate = comboFramerate.SelectedIndex + 1;
			configFile.GameConfig.ClipLevel = comboClip.SelectedIndex;
			configFile.GameConfig.FogEmulation = comboFog.SelectedIndex;

			configFile.GameConfig.Sound3D = check3DSound.Checked ? 1 : 0;
			configFile.GameConfig.SEVoice = checkSound.Checked ? 1 : 0;
			configFile.GameConfig.BGM = checkMusic.Checked ? 1 : 0;

			configFile.GameConfig.VoiceVolume = (int)numericSoundVol.Value;
			configFile.GameConfig.BGMVolume = (int)numericBGMVol.Value;

			configFile.GameConfig.MouseMode = radioMouseModeHold.Checked ? 0 : 1;

			if (inputDevice != null)
			{
				configFile.GameConfig.PadConfig = controllerConfigSelect.SelectedIndex == -1 ? null : controllerConfig[controllerConfigSelect.SelectedIndex].Name;

				configFile.Controllers.Clear();
				foreach (ControllerConfigInternal item in controllerConfig)
				{
					ControllerConfig config = new ControllerConfig { ButtonCount = item.Buttons.Max() + 1 };
					config.ButtonSettings = Enumerable.Repeat(-1, config.ButtonCount).ToArray();
					for (int i = 0; i < buttonIDs.Length; i++)
					{
						if (item.Buttons[i] != -1)
						{
							config.ButtonSettings[item.Buttons[i]] = buttonIDs[i];
						}
					}
					configFile.Controllers.Add(item.Name, config);
				}
			}

			IniSerializer.Serialize(configFile, sadxIni);
		}

		private void comboMouseActions_SelectedIndexChanged(object sender, EventArgs e)
		{
			/*
			 * Here, we take the selected Action index, get the Button
			 * assignment, and select it in comboMouseButtons.
			*/
			ushort selected;

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
			}
		}

		private static ushort MouseToInt(ushort n)
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

		private static ushort IntToMouse(ushort n)
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

		int currentAction;
		System.Threading.Thread controllerThread;

		void buttonControl_SetButtonClicked(object sender, EventArgs e)
		{
			Enabled = false;
			currentAction = buttonControls.IndexOf((ButtonControl)sender);
			controllerThread = new System.Threading.Thread(GetControllerInput);
			controllerThread.Start();
			((ButtonControl)sender).Text = "Waiting...";
		}

		private void GetControllerInput()
		{
			System.Threading.AutoResetEvent inputevent = new System.Threading.AutoResetEvent(false);
			inputDevice.SetNotification(inputevent);
			inputDevice.Acquire();
			int pressed = -1;
			while (pressed == -1)
			{
				if (!inputevent.WaitOne(20000))
					break;
				bool[] buttons = inputDevice.GetCurrentState().Buttons;
				for (int i = 0; i < buttons.Length; i++)
				{
					if (buttons[i])
					{
						pressed = i;
						break;
					}
				}
			}
			inputDevice.Unacquire();
			inputDevice.SetNotification(null);
			Invoke(new Action<int>(ButtonPressed), pressed);
		}

		void ButtonPressed(int button)
		{
			ControllerConfigInternal config = controllerConfig[controllerConfigSelect.SelectedIndex];
			if (button != -1)
			{
				if (Array.IndexOf(config.Buttons, button) != -1)
				{
					int i = Array.IndexOf(config.Buttons, button);
					buttonControls[i].Text = "Unassigned";
					config.Buttons[i] = -1;
				}
				buttonControls[currentAction].Text = "Button " + (button + 1);
				config.Buttons[currentAction] = button;
			}
			else
			{
				buttonControls[currentAction].Text = config.Buttons[currentAction] == -1
					? "Unassigned"
					: "Button " + (config.Buttons[currentAction] + 1);
			}
			Enabled = true;
		}

		void buttonControl_ClearButtonClicked(object sender, EventArgs e)
		{
			int i = buttonControls.IndexOf((ButtonControl)sender);
			buttonControls[i].Text = "Unassigned";
			controllerConfig[controllerConfigSelect.SelectedIndex].Buttons[i] = -1;
		}

		private void ConfigEditDialog_KeyDown(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Escape && controllerThread != null && controllerThread.IsAlive)
			{
				controllerThread.Abort();
				inputDevice.Unacquire();
				inputDevice.SetNotification(null);
				ControllerConfigInternal config = controllerConfig[controllerConfigSelect.SelectedIndex];
				buttonControls[currentAction].Text = config.Buttons[currentAction] == -1
					? "Unassigned"
					: "Button " + (config.Buttons[currentAction] + 1);
				Enabled = true;
			}
		}

		private void controllerConfigSelect_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (controllerConfigSelect.SelectedIndex == -1)
			{
				controllerConfigRemove.Enabled = controllerConfigName.Enabled = false;
				foreach (ButtonControl control in buttonControls)
					control.Enabled = false;
			}
			else
			{
				controllerConfigRemove.Enabled = controllerConfigName.Enabled = true;
				ControllerConfigInternal config = controllerConfig[controllerConfigSelect.SelectedIndex];
				controllerConfigName.Text = config.Name;
				for (int i = 0; i < buttonControls.Count; i++)
				{
					buttonControls[i].Enabled = true;
					buttonControls[i].Text = config.Buttons[i] == -1 ? "Unassigned" : "Button " + (config.Buttons[i] + 1);
				}
			}
		}

		private void controllerConfigAdd_Click(object sender, EventArgs e)
		{
			controllerConfig.Add(new ControllerConfigInternal
			{
				Name = "*NEW*",
				Buttons = Enumerable.Repeat(-1, buttonIDs.Length).ToArray()
			});
			controllerConfigSelect.Items.Add("*NEW*");
			controllerConfigSelect.SelectedIndex = controllerConfigSelect.Items.Count - 1;
		}

		private void controllerConfigRemove_Click(object sender, EventArgs e)
		{
			controllerConfig.RemoveAt(controllerConfigSelect.SelectedIndex);
			controllerConfigSelect.Items.RemoveAt(controllerConfigSelect.SelectedIndex);
		}

		private void controllerConfigName_TextChanged(object sender, EventArgs e)
		{
			for (int i = 0; i < controllerConfig.Count; i++)
			{
				if (i != controllerConfigSelect.SelectedIndex && controllerConfig[i].Name == controllerConfigName.Text)
				{
					controllerConfigName.BackColor = Color.Red;
					return;
				}
			}

			controllerConfigName.BackColor = SystemColors.Window;
			controllerConfig[controllerConfigSelect.SelectedIndex].Name = controllerConfigName.Text;
			controllerConfigSelect.Items[controllerConfigSelect.SelectedIndex] = controllerConfigName.Text;
		}

		private void ConfigEditDialog_FormClosing(object sender, FormClosingEventArgs e)
		{
			directInput?.Dispose();
			directInput = null;

			inputDevice?.Dispose();
			inputDevice = null;
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
		[IniAlwaysInclude]
		[IniName("fogemulation")]
		public int FogEmulation { get; set; }
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
		public int[] ButtonSettings { get; set; }
	}

	class ControllerConfigInternal
	{
		public string Name { get; set; }
		public int[] Buttons { get; set; }
	}
}
