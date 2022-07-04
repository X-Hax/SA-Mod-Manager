using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Reflection;
using System.Security.Cryptography;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using IniFile;
using ModManagerCommon;
using ModManagerCommon.Forms;
using SADXModManager.Forms;
using SADXModManager.Properties;
using SADXModManager.Controls;
using SharpDX.DirectInput;

namespace SADXModManager
{
	public partial class MainForm : Form
	{
		#region Variables and classes
		private GameConfigFile configFile;
		DirectInput directInput;
		Joystick inputDevice;
		readonly List<ButtonControl> buttonControls = new List<ButtonControl>();
		readonly List<ControllerConfigInternal> controllerConfig = new List<ControllerConfigInternal>();
		private const string sadxIni = "sonicDX.ini";
		private const string d3d8to9InstalledDLLName = "d3d8.dll";
		private const string d3d8to9StoredDLLName = "d3d8m.dll";
		private bool checkedForUpdates;

		const string updatePath = "mods/.updates";
		const string datadllpath = "system/CHRMODELS.dll";
		const string datadllorigpath = "system/CHRMODELS_orig.dll";
		const string loaderinipath = "mods/SADXModLoader.ini";
		const string loaderdllpath = "mods/SADXModLoader.dll";
		SADXLoaderInfo loaderini;
		Dictionary<string, SADXModInfo> mods;
		const string codelstpath = "mods/Codes.lst";
		const string codexmlpath = "mods/Codes.xml";
		const string codedatpath = "mods/Codes.dat";
		const string patchdatpath = "mods/Patches.dat";
		CodeList mainCodes;
		List<Code> codes;
		bool installed;
		bool suppressEvent;

		readonly ModUpdater modUpdater = new ModUpdater();
		BackgroundWorker updateChecker;
		private bool manualModUpdate;

		private Dictionary<int, string> TestSpawnCutsceneList;
		private Dictionary<ushort, string> TestSpawnGameModeList;

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

		static readonly string moddropname = "Mod" + Process.GetCurrentProcess().Id;

		const decimal ratio = 4 / 3m;

		static readonly Size[] resolutionPresets =
	{
			new Size(640, 480), // 640x480
			new Size(800, 600), // 800x600
			new Size(1024, 768), // 1024x768
			new Size(1152, 864), // 1152x864
			new Size(1280, 960), // 1280x960
			new Size(1280, 1024), // 1280x1024
			new Size(), // Native
			new Size(), // 1/2x Native
			new Size(), // 2x Native
			new Size(1280, 720), // 720p
			new Size(1920, 1080), // 1080p
			new Size(3840, 2160), // 4K
		};

		private bool displayedManifestWarning;

		int currentAction;
		System.Threading.Thread controllerThread;

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

		enum FrameRate
		{
			Invalid,
			High,
			Normal,
			Low
		}

		class GameConfigFile
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
		#endregion

		#region Startup
		public MainForm()
		{
			this.Font = SystemFonts.MessageBoxFont;
			InitializeComponent();

			// WORKAROUND: Windows 7's system fonts don't have
			// U+2912 or U+2913. Use Cambria instead.
			// TODO: Check the actual font to see if it has the glyphs.
			Font boldFont = null;
			OperatingSystem os = Environment.OSVersion;
			if ((os.Platform == PlatformID.Win32NT || os.Platform == PlatformID.Win32Windows) &&
				(os.Version.Major < 6 || (os.Version.Major == 6 && os.Version.Minor < 2)))
			{
				// Windows 7 or earlier.
				// TODO: Make sure this font exists.
				// NOTE: U+2912 and U+2913 are missing in Bold, so use Regular.
				boldFont = new Font("Cambria", this.Font.Size * 1.25f, FontStyle.Regular);
			}
			else
			{
				// Newer than Windows 7, or not Windows.
				// Use the default font.
				boldFont = new Font(this.Font.FontFamily, this.Font.Size * 1.25f, FontStyle.Bold);
			}

			modTopButton.Font = boldFont;
			modUpButton.Font = boldFont;
			modDownButton.Font = boldFont;
			modBottomButton.Font = boldFont;
		}

		private static void SetDoubleBuffered(Control control, bool enable)
		{
			PropertyInfo doubleBufferPropertyInfo = control.GetType().GetProperty("DoubleBuffered", BindingFlags.Instance | BindingFlags.NonPublic);
			doubleBufferPropertyInfo?.SetValue(control, enable, null);
		}

		private void MainForm_Load(object sender, EventArgs e)
		{
			// Try to use TLS 1.2
			try { ServicePointManager.SecurityProtocol = (SecurityProtocolType)3072; } catch { }
			if (!Debugger.IsAttached)
				Environment.CurrentDirectory = Application.StartupPath;
			SetDoubleBuffered(modListView, true);
			loaderini = File.Exists(loaderinipath) ? IniSerializer.Deserialize<SADXLoaderInfo>(loaderinipath) : new SADXLoaderInfo();

			try
			{
				if (File.Exists(codelstpath))
					mainCodes = CodeList.Load(codelstpath);
				else if (File.Exists(codexmlpath))
					mainCodes = CodeList.Load(codexmlpath);
				else
					mainCodes = new CodeList();
			}
			catch (Exception ex)
			{
				MessageBox.Show(this, $"Error loading code list: {ex.Message}", "SADX Mod Loader", MessageBoxButtons.OK, MessageBoxIcon.Warning);
				mainCodes = new CodeList();
			}

			for (int i = 0; i < Screen.AllScreens.Length; i++)
			{
				Screen s = Screen.AllScreens[i];
				screenNumComboBox.Items.Add($"{ i + 1 } { s.DeviceName } ({ s.Bounds.Location.X },{ s.Bounds.Y })"
					+ $" { s.Bounds.Width }x{ s.Bounds.Height } { s.BitsPerPixel } bpp { (s.Primary ? "Primary" : "") }");
			}

			comboBoxTestSpawnTime.SelectedIndex = 0;
			InitTestSpawnCutsceneList();
			InitTestSpawnGameModeList();
			LoadSettings();
			modListView.AutoResizeColumn(0, ColumnHeaderAutoResizeStyle.ColumnContent); // Mod name
			modListView.AutoResizeColumn(1, ColumnHeaderAutoResizeStyle.None); // Author
			modListView.AutoResizeColumn(2, ColumnHeaderAutoResizeStyle.HeaderSize); // Version
			modListView.Columns[3].Width = -2; // Category
			buttonUpdateD3D8to9.Visible = CheckD3D8to9Update();
			checkBoxEnableD3D9.Enabled = File.Exists(d3d8to9StoredDLLName);
			checkBoxEnableD3D9.Checked = File.Exists(d3d8to9InstalledDLLName);
			profileNameBox.BeginUpdate();
			foreach (var item in Directory.EnumerateFiles("mods", "*.ini"))
				if (!item.EndsWith("SADXModLoader.ini", StringComparison.OrdinalIgnoreCase) && !item.EndsWith("desktop.ini", StringComparison.OrdinalIgnoreCase))
					profileNameBox.Items.Add(Path.GetFileNameWithoutExtension(item));
			profileNameBox.EndUpdate();
			// Load controller settings
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
				groupBoxController.Visible = false;
			}
			// Load the config INI upon window load
			LoadConfigIni();
		}

		private void LoadSettings()
		{
			LoadModList();

			consoleCheckBox.Checked = loaderini.DebugConsole;
			screenCheckBox.Checked = loaderini.DebugScreen;
			fileCheckBox.Checked = loaderini.DebugFile;
			crashLogCheckBox.Checked = loaderini.DebugCrashLog;
			disableCDCheckCheckBox.Checked = loaderini.DisableCDCheck;
			checkVsync.Checked = loaderini.EnableVsync;
			horizontalResolution.Enabled = !loaderini.ForceAspectRatio;
			horizontalResolution.Value = Math.Max(horizontalResolution.Minimum, Math.Min(horizontalResolution.Maximum, loaderini.HorizontalResolution));
			verticalResolution.Value = Math.Max(verticalResolution.Minimum, Math.Min(verticalResolution.Maximum, loaderini.VerticalResolution));
			checkUpdateStartup.Checked = loaderini.UpdateCheck;
			checkUpdateModsStartup.Checked = loaderini.ModUpdateCheck;
			comboUpdateFrequency.SelectedIndex = (int)loaderini.UpdateUnit;
			numericUpdateFrequency.Value = loaderini.UpdateFrequency;
			comboVoiceLanguage.SelectedIndex = (int)loaderini.VoiceLanguage;
			comboTextLanguage.SelectedIndex = (int)loaderini.TextLanguage;

			suppressEvent = true;
			forceAspectRatioCheckBox.Checked = loaderini.ForceAspectRatio;
			checkScaleHud.Checked = loaderini.ScaleHud;
			suppressEvent = false;

			comboBackgroundFill.SelectedIndex = loaderini.BackgroundFillMode;
			comboFmvFill.SelectedIndex = loaderini.FmvFillMode;

			windowedFullscreenCheckBox.Checked = loaderini.WindowedFullscreen;
			forceMipmappingCheckBox.Checked = loaderini.AutoMipmap;
			forceTextureFilterCheckBox.Checked = loaderini.TextureFilter;
			pauseWhenInactiveCheckBox.Checked = loaderini.PauseWhenInactive;
			stretchFullscreenCheckBox.Checked = loaderini.StretchFullscreen;

			int screenNum = Math.Min(Screen.AllScreens.Length, loaderini.ScreenNum);

			screenNumComboBox.SelectedIndex = screenNum;
			customWindowSizeCheckBox.Checked = windowHeight.Enabled = maintainWindowAspectRatioCheckBox.Enabled = loaderini.CustomWindowSize;
			windowWidth.Enabled = loaderini.CustomWindowSize && !loaderini.MaintainWindowAspectRatio;
			Rectangle rect = Screen.PrimaryScreen.Bounds;

			foreach (Screen screen in Screen.AllScreens)
				rect = Rectangle.Union(rect, screen.Bounds);

			windowWidth.Maximum = rect.Width;
			windowWidth.Value = Math.Max(windowWidth.Minimum, Math.Min(rect.Width, loaderini.WindowWidth));
			windowHeight.Maximum = rect.Height;
			windowHeight.Value = Math.Max(windowHeight.Minimum, Math.Min(rect.Height, loaderini.WindowHeight));

			suppressEvent = true;
			maintainWindowAspectRatioCheckBox.Checked = loaderini.MaintainWindowAspectRatio;
			suppressEvent = false;

			checkWindowResize.Checked = loaderini.ResizableWindow;

			checkPolyBuff.Checked = loaderini.DisablePolyBuff;
			checkMaterialColor.Checked = loaderini.DisableMaterialColorFix;

			checkBoxTestSpawnLevel.Checked = loaderini.TestSpawnLevel != -1;
			comboBoxTestSpawnLevel.SelectedIndex = loaderini.TestSpawnLevel;
			numericUpDownTestSpawnAct.Value = loaderini.TestSpawnAct;
			checkBoxTestSpawnCharacter.Checked = loaderini.TestSpawnCharacter != -1;
			comboBoxTestSpawnCharacter.SelectedIndex = loaderini.TestSpawnCharacter;
			checkBoxTestSpawnPosition.Checked = loaderini.TestSpawnPositionEnabled;
			numericUpDownTestSpawnX.Value = loaderini.TestSpawnX;
			numericUpDownTestSpawnY.Value = loaderini.TestSpawnY;
			numericUpDownTestSpawnZ.Value = loaderini.TestSpawnZ;
			numericUpDownTestSpawnAngle.Value = loaderini.TestSpawnRotation;
			checkBoxTestSpawnAngleHex.Checked = loaderini.TestSpawnRotationHex;
			checkBoxTestSpawnEvent.Checked = loaderini.TestSpawnEvent != -1;
			comboBoxTestSpawnEvent.SelectedIndex = loaderini.TestSpawnEvent;
			checkBoxTestSpawnGameMode.Checked = loaderini.TestSpawnGameMode != -1;
			comboBoxTestSpawnGameMode.SelectedIndex = loaderini.TestSpawnGameMode;
			checkBoxTestSpawnSave.Checked = loaderini.TestSpawnSaveID != -1;
			numericUpDownTestSpawnSaveID.Value = Math.Max(1, loaderini.TestSpawnSaveID);
		}

		private void MainForm_Shown(object sender, EventArgs e)
		{
			if (CheckForUpdates())
				return;

			if (!File.Exists(datadllpath))
			{
				MessageBox.Show(this, "CHRMODELS.dll could not be found.\n\n" +
					"Cannot determine state of installation. Make sure you are running the Mod Manager from the game's main folder (where sonic.exe is).\n\n" +
					"If you are using the Steam version of the game, you need to convert it to the 2004 version before you can use the Mod Loader.",
					Text, MessageBoxButtons.OK, MessageBoxIcon.Warning);
				installButton.Hide();
			}
			else if (File.Exists(datadllorigpath))
			{
				installed = true;
				installButton.Text = "Uninstall loader";
				using (MD5 md5 = MD5.Create())
				{
					byte[] hash1 = md5.ComputeHash(File.ReadAllBytes(loaderdllpath));
					byte[] hash2 = md5.ComputeHash(File.ReadAllBytes(datadllpath));

					if (!hash1.SequenceEqual(hash2))
					{

						DialogResult result = MessageBox.Show(this, "Installed loader DLL differs from copy in mods folder."
							+ "\n\nWould you like to update the installed copy?", Text, MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

						if (result == DialogResult.Yes)
							File.Copy(loaderdllpath, datadllpath, true);
					}
				}
			}

			List<string> uris = Program.UriQueue.GetUris();

			foreach (string str in uris)
			{
				HandleUri(str);
			}

			Program.UriQueue.UriEnqueued += UriQueueOnUriEnqueued;

			CheckForModUpdates();

			// If we've checked for updates, save the modified
			// last update times without requiring the user to
			// click the save button.
			if (checkedForUpdates)
			{
				IniSerializer.Serialize(loaderini, loaderinipath);
			}
		}

		private void LoadModList()
		{
			modTopButton.Enabled = modUpButton.Enabled = modDownButton.Enabled = modBottomButton.Enabled = configureModButton.Enabled = false;
			modDescription.Text = "Description: No mod selected.";
			modListView.Items.Clear();
			mods = new Dictionary<string, SADXModInfo>();
			codes = new List<Code>(mainCodes.Codes);
			string modDir = Path.Combine(Environment.CurrentDirectory, "mods");

			if (File.Exists(Path.Combine(modDir, "mod.ini")))
			{
				MessageBox.Show(this, "There is a mod.ini in the mods folder."
							+ "\n\nEach mod must be placed in a subfolder in the mods folder. Do not extract mods directly to the mods folder." +
							"\n\nMove or delete mod.ini in the mods folder and run the Mod Manager again.", "SADX Mod Manager Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				Close();
				return;
			}

			foreach (string filename in SADXModInfo.GetModFiles(new DirectoryInfo(modDir)))
			{
				mods.Add((Path.GetDirectoryName(filename) ?? string.Empty).Substring(modDir.Length + 1), IniSerializer.Deserialize<SADXModInfo>(filename));
			}

			modListView.BeginUpdate();

			foreach (string mod in new List<string>(loaderini.Mods))
			{
				if (mods.ContainsKey(mod))
				{
					SADXModInfo inf = mods[mod];
					suppressEvent = true;
					modListView.Items.Add(new ListViewItem(new[] { inf.Name, inf.Author, inf.Version, inf.Category }) { Checked = true, Tag = mod });
					suppressEvent = false;
					if (!string.IsNullOrEmpty(inf.Codes))
						codes.AddRange(CodeList.Load(Path.Combine(Path.Combine(modDir, mod), inf.Codes)).Codes);
				}
				else
				{
					MessageBox.Show(this, "Mod \"" + mod + "\" could not be found.\n\nThis mod will be removed from the list.",
						Text, MessageBoxButtons.OK, MessageBoxIcon.Warning);
					loaderini.Mods.Remove(mod);
				}
			}

			foreach (KeyValuePair<string, SADXModInfo> inf in mods.OrderBy(x => x.Value.Name))
			{
				if (!loaderini.Mods.Contains(inf.Key))
					modListView.Items.Add(new ListViewItem(new[] { inf.Value.Name, inf.Value.Author, inf.Value.Version, inf.Value.Category }) { Tag = inf.Key });
			}

			modListView.EndUpdate();

			loaderini.EnabledCodes = new List<string>(loaderini.EnabledCodes.Where(a => codes.Any(c => c.Name == a)));
			foreach (Code item in codes.Where(a => a.Required && !loaderini.EnabledCodes.Contains(a.Name)))
				loaderini.EnabledCodes.Add(item.Name);

			codesCheckedListBox.BeginUpdate();
			codesCheckedListBox.Items.Clear();

			foreach (Code item in codes)
				codesCheckedListBox.Items.Add(item.Name, loaderini.EnabledCodes.Contains(item.Name));

			codesCheckedListBox.EndUpdate();
		}
		#endregion

		#region Mod management and configuration
		private void modListView_SelectedIndexChanged(object sender, EventArgs e)
		{
			int count = modListView.SelectedIndices.Count;
			if (count == 0)
			{
				modTopButton.Enabled = modUpButton.Enabled = modDownButton.Enabled = modBottomButton.Enabled = configureModButton.Enabled = false;
				modDescription.Text = "Description: No mod selected.";
			}
			else if (count == 1)
			{
				modDescription.Text = "Description: " + mods[(string)modListView.SelectedItems[0].Tag].Description;
				modTopButton.Enabled = modListView.SelectedIndices[0] != 0;
				modUpButton.Enabled = modListView.SelectedIndices[0] > 0;
				modDownButton.Enabled = modListView.SelectedIndices[0] < modListView.Items.Count - 1;
				modBottomButton.Enabled = modListView.SelectedIndices[0] != modListView.Items.Count - 1;
				configureModButton.Enabled = File.Exists(Path.Combine("mods", (string)modListView.SelectedItems[0].Tag, "configschema.xml"));
			}
			else if (count > 1)
			{
				modDescription.Text = "Description: Multiple mods selected.";
				modTopButton.Enabled = modUpButton.Enabled = modDownButton.Enabled = modBottomButton.Enabled = true;
				configureModButton.Enabled = false;
			}
		}

		private void modTopButton_Click(object sender, EventArgs e)
		{
			if (modListView.SelectedItems.Count < 1)
				return;

			modListView.BeginUpdate();

			for (int i = 0; i < modListView.SelectedItems.Count; i++)
			{
				int index = modListView.SelectedItems[i].Index;

				if (index > 0)
				{
					ListViewItem item = modListView.SelectedItems[i];
					modListView.Items.Remove(item);
					modListView.Items.Insert(i, item);
				}
			}

			modListView.SelectedItems[0].EnsureVisible();
			modListView.EndUpdate();
		}

		private void modUpButton_Click(object sender, EventArgs e)
		{
			if (modListView.SelectedItems.Count < 1)
				return;

			modListView.BeginUpdate();

			for (int i = 0; i < modListView.SelectedItems.Count; i++)
			{
				int index = modListView.SelectedItems[i].Index;

				if (index-- > 0 && !modListView.Items[index].Selected)
				{
					ListViewItem item = modListView.SelectedItems[i];
					modListView.Items.Remove(item);
					modListView.Items.Insert(index, item);
				}
			}

			modListView.SelectedItems[0].EnsureVisible();
			modListView.EndUpdate();
		}

		private void modDownButton_Click(object sender, EventArgs e)
		{
			if (modListView.SelectedItems.Count < 1)
				return;

			modListView.BeginUpdate();

			for (int i = modListView.SelectedItems.Count - 1; i >= 0; i--)
			{
				int index = modListView.SelectedItems[i].Index + 1;

				if (index != modListView.Items.Count && !modListView.Items[index].Selected)
				{
					ListViewItem item = modListView.SelectedItems[i];
					modListView.Items.Remove(item);
					modListView.Items.Insert(index, item);
				}
			}

			modListView.SelectedItems[modListView.SelectedItems.Count - 1].EnsureVisible();
			modListView.EndUpdate();
		}

		private void modBottomButton_Click(object sender, EventArgs e)
		{
			if (modListView.SelectedItems.Count < 1)
				return;

			modListView.BeginUpdate();

			for (int i = modListView.SelectedItems.Count - 1; i >= 0; i--)
			{
				int index = modListView.SelectedItems[i].Index;

				if (index != modListView.Items.Count - 1)
				{
					ListViewItem item = modListView.SelectedItems[i];
					modListView.Items.Remove(item);
					modListView.Items.Insert(modListView.Items.Count, item);
				}
			}

			modListView.SelectedItems[modListView.SelectedItems.Count - 1].EnsureVisible();
			modListView.EndUpdate();
		}

		private void buttonRefreshModList_Click(object sender, EventArgs e)
		{
			LoadModList();
		}

		private void buttonNewMod_Click(object sender, EventArgs e)
		{
			using (var ModDialog = new NewModDialog())
			{
				if (ModDialog.ShowDialog() == DialogResult.OK)
					LoadModList();
			}
		}

		private void codesCheckedListBox_ItemCheck(object sender, ItemCheckEventArgs e)
		{
			Code code = codes[e.Index];
			if (code.Required)
				e.NewValue = CheckState.Checked;
			if (e.NewValue == CheckState.Unchecked)
			{
				if (loaderini.EnabledCodes.Contains(code.Name))
					loaderini.EnabledCodes.Remove(code.Name);
			}
			else
			{
				if (!loaderini.EnabledCodes.Contains(code.Name))
					loaderini.EnabledCodes.Add(code.Name);
			}
		}

		private void modListView_ItemCheck(object sender, ItemCheckEventArgs e)
		{
			if (suppressEvent) return;
			codes = new List<Code>(mainCodes.Codes);
			string modDir = Path.Combine(Environment.CurrentDirectory, "mods");
			List<string> modlist = new List<string>();
			foreach (ListViewItem item in modListView.CheckedItems)
				modlist.Add((string)item.Tag);
			if (e.NewValue == CheckState.Unchecked)
				modlist.Remove((string)modListView.Items[e.Index].Tag);
			else
				modlist.Add((string)modListView.Items[e.Index].Tag);
			foreach (string mod in modlist)
				if (mods.ContainsKey(mod))
				{
					SADXModInfo inf = mods[mod];
					if (!string.IsNullOrEmpty(inf.Codes))
						codes.AddRange(CodeList.Load(Path.Combine(Path.Combine(modDir, mod), inf.Codes)).Codes);
				}
			loaderini.EnabledCodes = new List<string>(loaderini.EnabledCodes.Where(a => codes.Any(c => c.Name == a)));
			foreach (Code item in codes.Where(a => a.Required && !loaderini.EnabledCodes.Contains(a.Name)))
				loaderini.EnabledCodes.Add(item.Name);
			codesCheckedListBox.BeginUpdate();
			codesCheckedListBox.Items.Clear();
			foreach (Code item in codes)
				codesCheckedListBox.Items.Add(item.Name, loaderini.EnabledCodes.Contains(item.Name));
			codesCheckedListBox.EndUpdate();
		}

		private void modListView_MouseClick(object sender, MouseEventArgs e)
		{
			if (e.Button != MouseButtons.Right)
			{
				return;
			}

			if (modListView.FocusedItem.Bounds.Contains(e.Location))
			{
				modContextMenu.Show(Cursor.Position);
			}
		}

		private void openFolderToolStripMenuItem_Click(object sender, EventArgs e)
		{
			foreach (ListViewItem item in modListView.SelectedItems)
			{
				Process.Start(Path.Combine("mods", (string)item.Tag));
			}
		}

		private void configureModButton_Click(object sender, EventArgs e)
		{
			using (ModConfigDialog dlg = new ModConfigDialog(Path.Combine("mods", (string)modListView.SelectedItems[0].Tag), modListView.SelectedItems[0].Text))
				dlg.ShowDialog(this);
		}
		#endregion

		#region Drag and drop
		private void modListView_ItemDrag(object sender, ItemDragEventArgs e)
		{
			modListView.DoDragDrop(new DataObject(moddropname, modListView.SelectedItems.Cast<ListViewItem>().ToArray()), DragDropEffects.Move | DragDropEffects.Scroll);
		}

		private void modListView_DragEnter(object sender, DragEventArgs e)
		{
			if (e.Data.GetDataPresent(moddropname))
				e.Effect = DragDropEffects.Move | DragDropEffects.Scroll;
			else
				e.Effect = DragDropEffects.None;
		}

		private void modListView_DragOver(object sender, DragEventArgs e)
		{
			if (e.Data.GetDataPresent(moddropname))
				e.Effect = DragDropEffects.Move | DragDropEffects.Scroll;
			else
				e.Effect = DragDropEffects.None;
		}

		private void modListView_DragDrop(object sender, DragEventArgs e)
		{
			if (e.Data.GetDataPresent(moddropname))
			{
				Point clientPoint = modListView.PointToClient(new Point(e.X, e.Y));
				ListViewItem[] items = (ListViewItem[])e.Data.GetData(moddropname);
				int ind = modListView.GetItemAt(clientPoint.X, clientPoint.Y).Index;
				foreach (ListViewItem item in items)
					if (ind > item.Index)
						ind++;
				modListView.BeginUpdate();
				foreach (ListViewItem item in items)
					modListView.Items.Insert(ind++, (ListViewItem)item.Clone());
				foreach (ListViewItem item in items)
					modListView.Items.Remove(item);
				modListView.EndUpdate();
			}
		}
		#endregion

		#region Saving and Mod Loader installation
		private void Save()
		{
			loaderini.Mods.Clear();

			foreach (ListViewItem item in modListView.CheckedItems)
			{
				loaderini.Mods.Add((string)item.Tag);
			}

			loaderini.DebugConsole = consoleCheckBox.Checked;
			loaderini.DebugScreen = screenCheckBox.Checked;
			loaderini.DebugFile = fileCheckBox.Checked;
			loaderini.DebugCrashLog = crashLogCheckBox.Checked;
			loaderini.DisableCDCheck = disableCDCheckCheckBox.Checked;
			loaderini.HorizontalResolution = (int)horizontalResolution.Value;
			loaderini.VerticalResolution = (int)verticalResolution.Value;
			loaderini.ForceAspectRatio = forceAspectRatioCheckBox.Checked;
			loaderini.ScaleHud = checkScaleHud.Checked;
			loaderini.BackgroundFillMode = comboBackgroundFill.SelectedIndex;
			loaderini.FmvFillMode = comboFmvFill.SelectedIndex;
			loaderini.EnableVsync = checkVsync.Checked;
			loaderini.WindowedFullscreen = windowedFullscreenCheckBox.Checked;
			loaderini.AutoMipmap = forceMipmappingCheckBox.Checked;
			loaderini.TextureFilter = forceTextureFilterCheckBox.Checked;
			loaderini.PauseWhenInactive = pauseWhenInactiveCheckBox.Checked;
			loaderini.StretchFullscreen = stretchFullscreenCheckBox.Checked;
			loaderini.ScreenNum = screenNumComboBox.SelectedIndex;
			loaderini.CustomWindowSize = customWindowSizeCheckBox.Checked;
			loaderini.WindowWidth = (int)windowWidth.Value;
			loaderini.WindowHeight = (int)windowHeight.Value;
			loaderini.MaintainWindowAspectRatio = maintainWindowAspectRatioCheckBox.Checked;
			loaderini.ResizableWindow = checkWindowResize.Checked;
			loaderini.UpdateCheck = checkUpdateStartup.Checked;
			loaderini.ModUpdateCheck = checkUpdateModsStartup.Checked;
			loaderini.UpdateUnit = (UpdateUnit)comboUpdateFrequency.SelectedIndex;
			loaderini.UpdateFrequency = (int)numericUpdateFrequency.Value;
			loaderini.VoiceLanguage = (int)comboVoiceLanguage.SelectedIndex;
			loaderini.TextLanguage = (int)comboTextLanguage.SelectedIndex;
			loaderini.DisablePolyBuff = checkPolyBuff.Checked;
			loaderini.DisableMaterialColorFix = checkMaterialColor.Checked;

			loaderini.TestSpawnLevel = checkBoxTestSpawnLevel.Checked ? comboBoxTestSpawnLevel.SelectedIndex : -1;
			loaderini.TestSpawnAct = (int)numericUpDownTestSpawnAct.Value;
			loaderini.TestSpawnCharacter = checkBoxTestSpawnCharacter.Checked ? comboBoxTestSpawnCharacter.SelectedIndex : -1;
			loaderini.TestSpawnPositionEnabled = checkBoxTestSpawnPosition.Checked;
			loaderini.TestSpawnX = (int)numericUpDownTestSpawnX.Value;
			loaderini.TestSpawnY = (int)numericUpDownTestSpawnY.Value;
			loaderini.TestSpawnZ = (int)numericUpDownTestSpawnZ.Value;
			loaderini.TestSpawnRotation = (int)numericUpDownTestSpawnAngle.Value;
			loaderini.TestSpawnRotationHex = checkBoxTestSpawnAngleHex.Checked;
			loaderini.TestSpawnEvent = checkBoxTestSpawnEvent.Checked ? comboBoxTestSpawnEvent.SelectedIndex : -1;
			loaderini.TestSpawnGameMode = checkBoxTestSpawnGameMode.Checked ? comboBoxTestSpawnGameMode.SelectedIndex : -1;
			loaderini.TestSpawnSaveID = checkBoxTestSpawnSave.Checked ? (int)numericUpDownTestSpawnSaveID.Value : -1;
			IniSerializer.Serialize(loaderini, loaderinipath);

			List<Code> selectedCodes = new List<Code>();
			List<Code> selectedPatches = new List<Code>();

			foreach (Code item in codesCheckedListBox.CheckedIndices.OfType<int>().Select(a => codes[a]))
			{
				if (item.Patch)
					selectedPatches.Add(item);
				else
					selectedCodes.Add(item);
			}

			CodeList.WriteDatFile(patchdatpath, selectedPatches);
			CodeList.WriteDatFile(codedatpath, selectedCodes);
		}

		private void saveAndPlayButton_Click(object sender, EventArgs e)
		{
			if (updateChecker?.IsBusy == true)
			{
				var result = MessageBox.Show(this, "Mods are still being checked for updates. Continue anyway?",
					"Busy", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

				if (result == DialogResult.No)
				{
					return;
				}

				Enabled = false;

				updateChecker.CancelAsync();
				while (updateChecker.IsBusy)
				{
					Application.DoEvents();
				}

				Enabled = true;
			}

			Save();
			SaveConfigIni();
			if (!installed)
				switch (MessageBox.Show(this, "Looks like you're starting the game without the mod loader installed. Without the mod loader, the mods and codes you've selected won't be used, and some settings may not work.\n\nDo you want to install the mod loader now?", "SADX Mod Manager", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button1))
				{
					case DialogResult.Cancel:
						return;
					case DialogResult.Yes:
						File.Move(datadllpath, datadllorigpath);
						File.Copy(loaderdllpath, datadllpath);
						break;
				}
			Process process = Process.Start(loaderini.Mods.Select((item) => mods[item].EXEFile)
												.FirstOrDefault((item) => !string.IsNullOrEmpty(item)) ?? "sonic.exe");
			process?.WaitForInputIdle(10000);
			Close();
		}

		private void saveButton_Click(object sender, EventArgs e)
		{
			Save();
			SaveConfigIni();
			LoadModList();
		}

		private void installButton_Click(object sender, EventArgs e)
		{
			if (installed)
			{
				File.Delete(datadllpath);
				File.Move(datadllorigpath, datadllpath);
				installButton.Text = "Install loader";
			}
			else
			{
				File.Move(datadllpath, datadllorigpath);
				File.Copy(loaderdllpath, datadllpath);
				installButton.Text = "Uninstall loader";
			}
			installed = !installed;
		}
		#endregion

		#region Mod update related
		private static bool UpdateTimeElapsed(UpdateUnit unit, int amount, DateTime start)
		{
			if (unit == UpdateUnit.Always)
			{
				return true;
			}

			TimeSpan span = DateTime.UtcNow - start;

			switch (unit)
			{
				case UpdateUnit.Hours:
					return span.TotalHours >= amount;

				case UpdateUnit.Days:
					return span.TotalDays >= amount;

				case UpdateUnit.Weeks:
					return span.TotalDays / 7.0 >= amount;

				default:
					throw new ArgumentOutOfRangeException(nameof(unit), unit, null);
			}
		}

		private void HandleUri(string uri)
		{
			if (WindowState == FormWindowState.Minimized)
			{
				WindowState = FormWindowState.Normal;
			}

			Activate();

			var fields = uri.Substring("sadxmm:".Length).Split(',');

			// TODO: lib-ify
			string itemType = fields.FirstOrDefault(x => x.StartsWith("gb_itemtype", StringComparison.InvariantCultureIgnoreCase));
			itemType = itemType.Substring(itemType.IndexOf(":") + 1);

			string itemId = fields.FirstOrDefault(x => x.StartsWith("gb_itemid", StringComparison.InvariantCultureIgnoreCase));
			itemId = itemId.Substring(itemId.IndexOf(":") + 1);

			var dummyInfo = new ModInfo();

			GameBananaItem gbi = GameBananaItem.Load(itemType, long.Parse(itemId));

			dummyInfo.Name = gbi.Name;

			dummyInfo.Author = gbi.OwnerName;

			DialogResult result = MessageBox.Show(this, $"Do you want to install mod \"{dummyInfo.Name}\" by {dummyInfo.Author} from {new Uri(fields[0]).DnsSafeHost}?", "Mod Download", MessageBoxButtons.YesNo, MessageBoxIcon.Question);

			if (result != DialogResult.Yes)
			{
				return;
			}

			#region create update folder
			do
			{
				try
				{
					result = DialogResult.Cancel;
					if (!Directory.Exists(updatePath))
					{
						Directory.CreateDirectory(updatePath);
					}
				}
				catch (Exception ex)
				{
					result = MessageBox.Show(this, "Failed to create temporary update directory:\n" + ex.Message
												   + "\n\nWould you like to retry?", "Directory Creation Failed", MessageBoxButtons.RetryCancel);
				}
			} while (result == DialogResult.Retry);
			#endregion

			string dummyPath = dummyInfo.Name;

			foreach (char c in Path.GetInvalidFileNameChars())
			{
				dummyPath = dummyPath.Replace(c, '_');
			}

			dummyPath = Path.Combine("mods", dummyPath);

			var updates = new List<ModDownload>
			{
				new ModDownload(dummyInfo, dummyPath, fields[0], null, 0)
			};

			using (var progress = new ModDownloadDialog(updates, updatePath))
			{
				progress.ShowDialog(this);
			}

			do
			{
				try
				{
					result = DialogResult.Cancel;
					Directory.Delete(updatePath, true);
				}
				catch (Exception ex)
				{
					result = MessageBox.Show(this, "Failed to remove temporary update directory:\n" + ex.Message
												   + "\n\nWould you like to retry? You can remove the directory manually later.",
						"Directory Deletion Failed", MessageBoxButtons.RetryCancel);
				}
			} while (result == DialogResult.Retry);

			LoadModList();
		}

		private void UriQueueOnUriEnqueued(object sender, OnUriEnqueuedArgs args)
		{
			args.Handled = true;

			if (InvokeRequired)
			{
				Invoke((Action<object, OnUriEnqueuedArgs>)UriQueueOnUriEnqueued, sender, args);
				return;
			}

			HandleUri(args.Uri);
		}

		private bool CheckForUpdates(bool force = false)
		{
			if (!force && !loaderini.UpdateCheck)
			{
				return false;
			}

			if (!force && !UpdateTimeElapsed(loaderini.UpdateUnit, loaderini.UpdateFrequency, DateTime.FromFileTimeUtc(loaderini.UpdateTime)))
			{
				return false;
			}

			checkedForUpdates = true;
			loaderini.UpdateTime = DateTime.UtcNow.ToFileTimeUtc();

			if (!File.Exists("sadxmlver.txt"))
			{
				return false;
			}

			using (var wc = new WebClient())
			{
				try
				{
					string msg = wc.DownloadString("http://mm.reimuhakurei.net/toolchangelog.php?tool=sadxml&rev=" + File.ReadAllText("sadxmlver.txt"));

					if (msg.Length > 0)
					{
						using (var dlg = new UpdateMessageDialog("SADX", msg.Replace("\n", "\r\n")))
						{
							if (dlg.ShowDialog(this) == DialogResult.Yes)
							{
								DialogResult result = DialogResult.OK;
								do
								{
									try
									{
										if (!Directory.Exists(updatePath))
										{
											Directory.CreateDirectory(updatePath);
										}
									}
									catch (Exception ex)
									{
										result = MessageBox.Show(this, "Failed to create temporary update directory:\n" + ex.Message
																	   + "\n\nWould you like to retry?", "Directory Creation Failed", MessageBoxButtons.RetryCancel);
										if (result == DialogResult.Cancel) return false;
									}
								} while (result == DialogResult.Retry);

								using (var dlg2 = new LoaderDownloadDialog("http://mm.reimuhakurei.net/sadxmods/SADXModLoader.7z", updatePath))
									if (dlg2.ShowDialog(this) == DialogResult.OK)
									{
										Close();
										return true;
									}
							}
						}
					}
				}
				catch
				{
					MessageBox.Show(this, "Unable to retrieve update information.", "SADX Mod Manager");
				}
			}

			return false;
		}

		private void InitializeWorker()
		{
			if (updateChecker != null)
			{
				return;
			}

			updateChecker = new BackgroundWorker { WorkerSupportsCancellation = true };
			updateChecker.DoWork += UpdateChecker_DoWork;
			updateChecker.RunWorkerCompleted += UpdateChecker_RunWorkerCompleted;
			updateChecker.RunWorkerCompleted += UpdateChecker_EnableControls;
		}

		private void UpdateChecker_EnableControls(object sender, RunWorkerCompletedEventArgs runWorkerCompletedEventArgs)
		{
			buttonCheckForUpdates.Enabled = true;
			checkForUpdatesToolStripMenuItem.Enabled = true;
			verifyToolStripMenuItem.Enabled = true;
			forceUpdateToolStripMenuItem.Enabled = true;
			uninstallToolStripMenuItem.Enabled = true;
			developerToolStripMenuItem.Enabled = true;
		}

		private void CheckForModUpdates(bool force = false)
		{
			if (!force && !loaderini.ModUpdateCheck)
			{
				return;
			}

			InitializeWorker();

			if (!force && !UpdateTimeElapsed(loaderini.UpdateUnit, loaderini.UpdateFrequency, DateTime.FromFileTimeUtc(loaderini.ModUpdateTime)))
			{
				return;
			}

			checkedForUpdates = true;
			loaderini.ModUpdateTime = DateTime.UtcNow.ToFileTimeUtc();
			updateChecker.RunWorkerAsync(mods.Select(x => new KeyValuePair<string, ModInfo>(x.Key, x.Value)).ToList());
			buttonCheckForUpdates.Enabled = false;
		}

		private void UpdateChecker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
		{
			if (modUpdater.ForceUpdate)
			{
				updateChecker?.Dispose();
				updateChecker = null;
				modUpdater.ForceUpdate = false;
				modUpdater.Clear();
			}

			if (e.Cancelled)
			{
				return;
			}

			if (!(e.Result is Tuple<List<ModDownload>, List<string>> data))
			{
				return;
			}

			List<string> errors = data.Item2;
			if (errors.Count != 0)
			{
				MessageBox.Show(this, "The following errors occurred while checking for updates:\n\n" + string.Join("\n", errors),
					"Update Errors", MessageBoxButtons.OK, MessageBoxIcon.Warning);
			}

			bool manual = manualModUpdate;
			manualModUpdate = false;

			List<ModDownload> updates = data.Item1;
			if (updates.Count == 0)
			{
				if (manual)
				{
					MessageBox.Show(this, "Mods are up to date.",
						"No Updates", MessageBoxButtons.OK, MessageBoxIcon.Information);
				}
				return;
			}

			using (var dialog = new ModUpdatesDialog(updates))
			{
				if (dialog.ShowDialog(this) != DialogResult.OK)
				{
					return;
				}

				updates = dialog.SelectedMods;
			}

			if (updates.Count == 0)
			{
				return;
			}

			DialogResult result;

			do
			{
				try
				{
					result = DialogResult.Cancel;
					if (!Directory.Exists(updatePath))
					{
						Directory.CreateDirectory(updatePath);
					}
				}
				catch (Exception ex)
				{
					result = MessageBox.Show(this, "Failed to create temporary update directory:\n" + ex.Message
						+ "\n\nWould you like to retry?", "Directory Creation Failed", MessageBoxButtons.RetryCancel);
				}
			} while (result == DialogResult.Retry);

			using (var progress = new ModDownloadDialog(updates, updatePath))
			{
				progress.ShowDialog(this);
			}

			do
			{
				try
				{
					result = DialogResult.Cancel;
					Directory.Delete(updatePath, true);
				}
				catch (Exception ex)
				{
					result = MessageBox.Show(this, "Failed to remove temporary update directory:\n" + ex.Message
						+ "\n\nWould you like to retry? You can remove the directory manually later.",
						"Directory Deletion Failed", MessageBoxButtons.RetryCancel);
				}
			} while (result == DialogResult.Retry);

			LoadModList();
		}

		private void UpdateChecker_DoWork(object sender, DoWorkEventArgs e)
		{
			if (!(sender is BackgroundWorker worker))
			{
				throw new Exception("what");
			}

			Invoke(new Action(() =>
			{
				buttonCheckForUpdates.Enabled = false;
				checkForUpdatesToolStripMenuItem.Enabled = false;
				verifyToolStripMenuItem.Enabled = false;
				forceUpdateToolStripMenuItem.Enabled = false;
				uninstallToolStripMenuItem.Enabled = false;
				developerToolStripMenuItem.Enabled = false;
			}));

			var updatableMods = e.Argument as List<KeyValuePair<string, ModInfo>>;
			List<ModDownload> updates = null;
			List<string> errors = null;

			var tokenSource = new CancellationTokenSource();
			CancellationToken token = tokenSource.Token;

			using (var task = new Task(() => modUpdater.GetModUpdates(updatableMods, out updates, out errors, token), token))
			{
				task.Start();

				while (!task.IsCompleted && !task.IsCanceled)
				{
					Application.DoEvents();

					if (worker.CancellationPending)
					{
						tokenSource.Cancel();
					}
				}

				task.Wait(token);
			}

			e.Result = new Tuple<List<ModDownload>, List<string>>(updates, errors);
		}

		// TODO: merge with ^
		private void UpdateChecker_DoWorkForced(object sender, DoWorkEventArgs e)
		{
			if (!(sender is BackgroundWorker worker))
			{
				throw new Exception("what");
			}

			if (!(e.Argument is List<Tuple<string, ModInfo, List<ModManifestDiff>>> updatableMods) || updatableMods.Count == 0)
			{
				return;
			}

			var updates = new List<ModDownload>();
			var errors = new List<string>();

			using (var client = new UpdaterWebClient())
			{
				foreach (Tuple<string, ModInfo, List<ModManifestDiff>> info in updatableMods)
				{
					if (worker.CancellationPending)
					{
						e.Cancel = true;
						break;
					}

					ModInfo mod = info.Item2;
					if (!string.IsNullOrEmpty(mod.GitHubRepo))
					{
						if (string.IsNullOrEmpty(mod.GitHubAsset))
						{
							errors.Add($"[{ mod.Name }] GitHubRepo specified, but GitHubAsset is missing.");
							continue;
						}

						ModDownload d = modUpdater.GetGitHubReleases(mod, info.Item1, client, errors);
						if (d != null)
						{
							updates.Add(d);
						}
					}
					else if (!string.IsNullOrEmpty(mod.GameBananaItemType) && mod.GameBananaItemId.HasValue)
					{
						ModDownload d = modUpdater.GetGameBananaReleases(mod, info.Item1, errors);
						if (d != null)
						{
							updates.Add(d);
						}
					}
					else if (!string.IsNullOrEmpty(mod.UpdateUrl))
					{
						List<ModManifestEntry> localManifest = info.Item3
							.Where(x => x.State == ModManifestState.Unchanged)
							.Select(x => x.Current).ToList();

						ModDownload d = modUpdater.CheckModularVersion(mod, info.Item1, localManifest, client, errors);
						if (d != null)
						{
							updates.Add(d);
						}
					}
				}
			}

			e.Result = new Tuple<List<ModDownload>, List<string>>(updates, errors);
		}

		private void uninstallToolStripMenuItem_Click(object sender, EventArgs e)
		{
			DialogResult result = MessageBox.Show(this, "This will uninstall all selected mods."
				+ "\n\nAre you sure you wish to continue?", "Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

			if (result != DialogResult.Yes)
			{
				return;
			}

			result = MessageBox.Show(this, "Would you like to keep mod user data where possible? (Save files, config files, etc)",
				"User Data", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);

			if (result == DialogResult.Cancel)
			{
				return;
			}

			foreach (ListViewItem item in modListView.SelectedItems)
			{
				var dir = (string)item.Tag;
				var modDir = Path.Combine("mods", dir);
				var manpath = Path.Combine(modDir, "mod.manifest");

				try
				{
					if (result == DialogResult.Yes && File.Exists(manpath))
					{
						List<ModManifestEntry> manifest = ModManifest.FromFile(manpath);
						foreach (var entry in manifest)
						{
							var path = Path.Combine(modDir, entry.FilePath);
							if (File.Exists(path))
							{
								File.Delete(path);
							}
						}

						File.Delete(manpath);
						var version = Path.Combine(modDir, "mod.version");
						if (File.Exists(version))
						{
							File.Delete(version);
						}
					}
					else
					{
						if (result == DialogResult.Yes)
						{
							var retain = MessageBox.Show(this, $"The mod \"{ mods[dir].Name }\" (\"mods\\{ dir }\") does not have a manifest, so mod user data cannot be retained."
								+ " Do you want to uninstall it anyway?", "Cannot Retain User Data", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

							if (retain == DialogResult.No)
							{
								continue;
							}
						}

						Directory.Delete(modDir, true);
					}
				}
				catch (Exception ex)
				{
					MessageBox.Show(this, $"Failed to uninstall mod \"{ mods[dir].Name }\" from \"{ dir }\": { ex.Message }", "Failed",
						MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
			}

			LoadModList();
		}

		private void generateManifestToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (!displayedManifestWarning)
			{
				DialogResult result = MessageBox.Show(this, Resources.GenerateManifestWarning,
					"Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

				if (result != DialogResult.Yes)
				{
					return;
				}

				displayedManifestWarning = true;
			}

			foreach (ListViewItem item in modListView.SelectedItems)
			{
				var modPath = Path.Combine("mods", (string)item.Tag);
				var manifestPath = Path.Combine(modPath, "mod.manifest");

				List<ModManifestEntry> manifest;
				List<ModManifestDiff> diff;

				using (var progress = new ManifestDialog(modPath, $"Generating manifest: {(string)item.Tag}", true))
				{
					progress.SetTask("Generating file index...");
					if (progress.ShowDialog(this) == DialogResult.Cancel)
					{
						continue;
					}

					diff = progress.Diff;
				}

				if (diff == null)
				{
					continue;
				}

				if (diff.Count(x => x.State != ModManifestState.Unchanged) <= 0)
				{
					continue;
				}

				using (var dialog = new ManifestDiffDialog(diff))
				{
					if (dialog.ShowDialog(this) == DialogResult.Cancel)
					{
						continue;
					}

					manifest = dialog.MakeNewManifest();
				}

				ModManifest.ToFile(manifest, manifestPath);
			}
		}

		private void UpdateSelectedMods()
		{
			InitializeWorker();
			manualModUpdate = true;
			updateChecker?.RunWorkerAsync(modListView.SelectedItems.Cast<ListViewItem>()
				.Select(x => (string)x.Tag)
				.Select(x => new KeyValuePair<string, ModInfo>(x, mods[x]))
				.ToList());
		}

		private void checkForUpdatesToolStripMenuItem_Click(object sender, EventArgs e)
		{
			UpdateSelectedMods();
		}

		private void forceUpdateToolStripMenuItem_Click(object sender, EventArgs e)
		{
			var result = MessageBox.Show(this, "This will force all selected mods to be completely re-downloaded."
				+ " Are you sure you want to continue?",
				"Force Update", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

			if (result != DialogResult.Yes)
			{
				return;
			}

			modUpdater.ForceUpdate = true;
			UpdateSelectedMods();
		}

		private void verifyToolStripMenuItem_Click(object sender, EventArgs e)
		{
			List<Tuple<string, ModInfo>> items = modListView.SelectedItems.Cast<ListViewItem>()
				.Select(x => (string)x.Tag)
				.Where(x => File.Exists(Path.Combine("mods", x, "mod.manifest")))
				.Select(x => new Tuple<string, ModInfo>(x, mods[x]))
				.ToList();

			if (items.Count < 1)
			{
				MessageBox.Show(this, "None of the selected mods have manifests, so they cannot be verified.",
					"Missing mod.manifest", MessageBoxButtons.OK, MessageBoxIcon.Warning);
				return;
			}

			using (var progress = new VerifyModDialog(items))
			{
				var result = progress.ShowDialog(this);
				if (result == DialogResult.Cancel)
				{
					return;
				}

				List<Tuple<string, ModInfo, List<ModManifestDiff>>> failed = progress.Failed;
				if (failed.Count < 1)
				{
					MessageBox.Show(this, "All selected mods passed verification.", "Integrity Pass",
						MessageBoxButtons.OK, MessageBoxIcon.Information);
				}
				else
				{
					result = MessageBox.Show(this, "The following mods failed verification:\n"
						+ string.Join("\n", failed.Select(x => $"{x.Item2.Name}: {x.Item3.Count(y => y.State != ModManifestState.Unchanged)} file(s)"))
						+ "\n\nWould you like to attempt repairs?",
						"Integrity Fail", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

					if (result != DialogResult.Yes)
					{
						return;
					}

					InitializeWorker();

					updateChecker.DoWork -= UpdateChecker_DoWork;
					updateChecker.DoWork += UpdateChecker_DoWorkForced;

					updateChecker.RunWorkerAsync(failed);

					modUpdater.ForceUpdate = true;
					buttonCheckForUpdates.Enabled = false;
				}
			}
		}

		private void comboUpdateFrequency_SelectedIndexChanged(object sender, EventArgs e)
		{
			numericUpdateFrequency.Enabled = comboUpdateFrequency.SelectedIndex > 0;
		}

		private void buttonCheckForUpdates_Click(object sender, EventArgs e)
		{
			buttonCheckForUpdates.Enabled = false;

			if (CheckForUpdates(true))
			{
				return;
			}

			manualModUpdate = true;
			CheckForModUpdates(true);
		}

		private void installURLHandlerButton_Click(object sender, EventArgs e)
		{
			Process.Start(new ProcessStartInfo(Application.ExecutablePath, "urlhandler") { UseShellExecute = true, Verb = "runas" }).WaitForExit();
			MessageBox.Show(this, "1-Click installed!", Text);
		}
		#endregion

		#region Game settings
		private void LoadConfigIni()
		{
			configFile = File.Exists(sadxIni) ? IniSerializer.Deserialize<GameConfigFile>(sadxIni) : new GameConfigFile();
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
			trackBarVoiceVol.Value = configFile.GameConfig.VoiceVolume;
			trackBarMusicVol.Value = configFile.GameConfig.BGMVolume;
			labelVoiceVol.Text = trackBarVoiceVol.Value.ToString();
			labelMusicVol.Text = trackBarMusicVol.Value.ToString();

			// Input
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

			configFile.GameConfig.VoiceVolume = (int)trackBarVoiceVol.Value;
			configFile.GameConfig.BGMVolume = (int)trackBarMusicVol.Value;

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

		private void screenNumComboBox_SelectedIndexChanged(object sender, EventArgs e)
		{
			Size oldsize = resolutionPresets[6];
			Rectangle rect = Screen.PrimaryScreen.Bounds;
			if (screenNumComboBox.SelectedIndex > 0)
				rect = Screen.AllScreens[screenNumComboBox.SelectedIndex - 1].Bounds;
			else
				foreach (Screen screen in Screen.AllScreens)
					rect = Rectangle.Union(rect, screen.Bounds);
			resolutionPresets[6] = rect.Size;
			resolutionPresets[7] = new Size(rect.Width / 2, rect.Height / 2);
			resolutionPresets[8] = new Size(rect.Width * 2, rect.Height * 2);
			if (comboResolutionPreset.SelectedIndex > 4 && comboResolutionPreset.SelectedIndex < 8 && rect.Size != oldsize)
				comboResolutionPreset.SelectedIndex = -1;
		}

		private void forceAspectRatioCheckBox_CheckedChanged(object sender, EventArgs e)
		{
			if (forceAspectRatioCheckBox.Checked)
			{
				horizontalResolution.Enabled = false;
				horizontalResolution.Value = Math.Round(verticalResolution.Value * ratio);
				comboResolutionPreset.SelectedIndex = -1;
			}
			else if (!suppressEvent)
				horizontalResolution.Enabled = true;
		}

		private void horizontalResolution_ValueChanged(object sender, EventArgs e)
		{
			if (!suppressEvent)
				comboResolutionPreset.SelectedIndex = -1;
		}

		private void verticalResolution_ValueChanged(object sender, EventArgs e)
		{
			if (forceAspectRatioCheckBox.Checked)
				horizontalResolution.Value = Math.Round(verticalResolution.Value * ratio);
			if (!suppressEvent)
				comboResolutionPreset.SelectedIndex = -1;
		}

		private void comboResolutionPreset_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (comboResolutionPreset.SelectedIndex == -1) return;
			suppressEvent = true;
			verticalResolution.Value = resolutionPresets[comboResolutionPreset.SelectedIndex].Height;
			if (!forceAspectRatioCheckBox.Checked)
				horizontalResolution.Value = resolutionPresets[comboResolutionPreset.SelectedIndex].Width;
			suppressEvent = false;
		}

		private void customWindowSizeCheckBox_CheckedChanged(object sender, EventArgs e)
		{
			maintainWindowAspectRatioCheckBox.Enabled = customWindowSizeCheckBox.Checked;
			windowHeight.Enabled = customWindowSizeCheckBox.Checked;
			checkWindowResize.Enabled = !customWindowSizeCheckBox.Checked;
			windowWidth.Enabled = customWindowSizeCheckBox.Checked && !maintainWindowAspectRatioCheckBox.Checked;
		}

		private void maintainWindowAspectRatioCheckBox_CheckedChanged(object sender, EventArgs e)
		{
			if (maintainWindowAspectRatioCheckBox.Checked)
			{
				windowWidth.Enabled = false;
				windowWidth.Value = Math.Round(windowHeight.Value * (horizontalResolution.Value / verticalResolution.Value));
			}
			else if (!suppressEvent)
				windowWidth.Enabled = true;
		}

		private void windowHeight_ValueChanged(object sender, EventArgs e)
		{
			if (maintainWindowAspectRatioCheckBox.Checked)
				windowWidth.Value = Math.Round(windowHeight.Value * (horizontalResolution.Value / verticalResolution.Value));
		}

		private void trackBarVoiceVol_ValueChanged(object sender, EventArgs e)
		{
			labelVoiceVol.Text = trackBarVoiceVol.Value.ToString();
		}

		private void trackBarMusicVol_ValueChanged(object sender, EventArgs e)
		{
			labelMusicVol.Text = trackBarMusicVol.Value.ToString();
		}
		#endregion

		#region TestSpawn
		private void checkBoxTestSpawnPosition_CheckedChanged(object sender, EventArgs e)
		{
			labelTestSpawnX.Enabled = labelTestSpawnY.Enabled = labelTestSpawnZ.Enabled = labelTestSpawnAngle.Enabled =
				numericUpDownTestSpawnX.Enabled = numericUpDownTestSpawnY.Enabled = numericUpDownTestSpawnZ.Enabled =
				numericUpDownTestSpawnAngle.Enabled = checkBoxTestSpawnAngleHex.Enabled = checkBoxTestSpawnPosition.Checked;
		}

		private void checkBoxTestSpawnLevel_CheckedChanged(object sender, EventArgs e)
		{
			comboBoxTestSpawnLevel.Enabled = numericUpDownTestSpawnAct.Enabled = labelTestSpawnAct.Enabled = checkBoxTestSpawnLevel.Checked;
			if (comboBoxTestSpawnLevel.SelectedIndex == -1)
				comboBoxTestSpawnLevel.SelectedIndex = 0;
			ShowOrHideTestSpawnEventWarning();
		}

		private void checkBoxTestSpawnCharacter_CheckedChanged(object sender, EventArgs e)
		{
			comboBoxTestSpawnCharacter.Enabled = checkBoxTestSpawnCharacter.Checked;
			if (comboBoxTestSpawnCharacter.SelectedIndex == -1)
				comboBoxTestSpawnCharacter.SelectedIndex = 0;
			ShowOrHideTestSpawnEventWarning();
		}

		private void checkBoxTestSpawnEvent_CheckedChanged(object sender, EventArgs e)
		{
			comboBoxTestSpawnEvent.Enabled = checkBoxTestSpawnEvent.Checked;
			if (comboBoxTestSpawnEvent.SelectedIndex == -1)
				comboBoxTestSpawnEvent.SelectedIndex = 0;
			ShowOrHideTestSpawnEventWarning();
		}
		private void checkBoxTestSpawnGameMode_CheckedChanged(object sender, EventArgs e)
		{
			comboBoxTestSpawnGameMode.Enabled = checkBoxTestSpawnGameMode.Checked;

			if (comboBoxTestSpawnGameMode.SelectedIndex == -1)
				comboBoxTestSpawnGameMode.SelectedIndex = 0;

		}

		private void checkBoxTestSpawnSave_CheckStateChanged(object sender, EventArgs e)
		{
			numericUpDownTestSpawnSaveID.Enabled = checkBoxTestSpawnSave.Checked;
		}

		private string GetTestSpawnCommandLine()
		{
			List<string> cmdline = new List<string>();
			if (checkBoxTestSpawnLevel.Checked)
				cmdline.Add("-l " + comboBoxTestSpawnLevel.SelectedIndex.ToString() + " -a " + numericUpDownTestSpawnAct.Value.ToString());
			if (checkBoxTestSpawnCharacter.Checked)
				cmdline.Add("-c " + comboBoxTestSpawnCharacter.SelectedIndex.ToString());
			if (checkBoxTestSpawnPosition.Checked)
				cmdline.Add("-p " + numericUpDownTestSpawnX.Value.ToString() + " " +
					numericUpDownTestSpawnY.Value.ToString() + " " +
					numericUpDownTestSpawnZ.Value.ToString() + " -r " +
					numericUpDownTestSpawnAngle.Value.ToString());
			if (checkBoxTestSpawnEvent.Checked)
			{
				int ev = 0;
				int ev_result = 0;
				foreach (var item in TestSpawnCutsceneList)
				{
					if (ev == comboBoxTestSpawnEvent.SelectedIndex)
					{
						ev_result = item.Key;
						break;
					}
					ev++;
				}
				cmdline.Add("-e " + ev_result.ToString());
			}
			if (comboBoxTestSpawnTime.SelectedIndex > 0)
				cmdline.Add("-t " + (comboBoxTestSpawnTime.SelectedIndex - 1).ToString());

			if (checkBoxTestSpawnGameMode.Checked)
			{
					uint gm = 0;
					uint gm_result = 0;
					foreach (var item in TestSpawnGameModeList)
					{
						if (gm == comboBoxTestSpawnGameMode.SelectedIndex)
						{
							gm_result = item.Key;
							break;
						}
						gm++;
					}
					cmdline.Add("-g " + gm_result.ToString());			
			}
			if (checkBoxTestSpawnSave.Checked)
				cmdline.Add("-s " + numericUpDownTestSpawnSaveID.Value.ToString());
			return string.Join(" ", cmdline);
		}

		private void buttonTestSpawnPlay_Click(object sender, EventArgs e)
		{
			Process process = Process.Start(loaderini.Mods.Select((item) => mods[item].EXEFile)
												.FirstOrDefault((item) => !string.IsNullOrEmpty(item)) ?? "sonic.exe", GetTestSpawnCommandLine());
		}

		private void InitTestSpawnCutsceneList()
		{
			TestSpawnCutsceneList = new Dictionary<int, string>();
			TestSpawnCutsceneList.Add(1, "Sonic intro");
			TestSpawnCutsceneList.Add(2, "Sonic defeats Chaos 0");
			TestSpawnCutsceneList.Add(3, "Sonic sees Tails crash");
			TestSpawnCutsceneList.Add(6, "Sonic and Tails poolside");
			TestSpawnCutsceneList.Add(7, "Sonic faces off with the Egg Hornet");
			TestSpawnCutsceneList.Add(8, "Sonic sees Chaos 1 emerge");
			TestSpawnCutsceneList.Add(9, "Sonic and Tails are gassed");
			TestSpawnCutsceneList.Add(11, "Sonic sees Chaos 4 transformation");
			TestSpawnCutsceneList.Add(12, "Sonic and Tails part ways with Knuckles");
			TestSpawnCutsceneList.Add(13, "Sonic and Tails take off on the Tornado 1");
			TestSpawnCutsceneList.Add(17, "Sonic falling into Station Square");
			TestSpawnCutsceneList.Add(18, "Sonic is found by Amy");
			TestSpawnCutsceneList.Add(19, "Sonic and Amy go to Twinkle Park");
			TestSpawnCutsceneList.Add(20, "Sonic goes looking for Amy");
			TestSpawnCutsceneList.Add(21, "Sonic sees Zero and Amy at the station");
			TestSpawnCutsceneList.Add(22, "Sonic sees Zero transported to the Egg Carrier");
			TestSpawnCutsceneList.Add(23, "Sonic catches up with Tails on the Tornado 2");
			TestSpawnCutsceneList.Add(26, "Sonic sees Eggman take Birdie's Emerald");
			TestSpawnCutsceneList.Add(27, "Sonic defeats Gamma");
			TestSpawnCutsceneList.Add(28, "Sonic finds Chaos 6");
			TestSpawnCutsceneList.Add(29, "Sonic jumps from the Egg Carrier into the jungle");
			TestSpawnCutsceneList.Add(30, "Sonic sees the temple come out of the ground");
			TestSpawnCutsceneList.Add(32, "Sonic looks at the Perfect Chaos mural");
			TestSpawnCutsceneList.Add(33, "Sonic enters the Past");
			TestSpawnCutsceneList.Add(34, "Sonic listens to Tikal in the Past");
			TestSpawnCutsceneList.Add(35, "Sonic sees Eggman heading to his base");
			TestSpawnCutsceneList.Add(36, "Sonic faces the Egg Viper");
			TestSpawnCutsceneList.Add(38, "Sonic's outro");
			TestSpawnCutsceneList.Add(40, "Sonic vs. Knuckles");
			TestSpawnCutsceneList.Add(41, "Sonic and Tails land on the Egg Carrier");
			TestSpawnCutsceneList.Add(42, "Sonic and Tails awaken after being gassed");
			TestSpawnCutsceneList.Add(43, "Sonic meets Chaos 0");
			TestSpawnCutsceneList.Add(48, "Tails intro");
			TestSpawnCutsceneList.Add(49, "Tails is rescued by Sonic");
			TestSpawnCutsceneList.Add(50, "Tails and Sonic poolside");
			TestSpawnCutsceneList.Add(51, "Tails faces off with the Egg Hornet");
			TestSpawnCutsceneList.Add(52, "Tails sees Chaos 1 emerge");
			TestSpawnCutsceneList.Add(53, "Tails and Sonic are gassed at Casinopolis");
			TestSpawnCutsceneList.Add(56, "Tails vs. Knuckles");
			TestSpawnCutsceneList.Add(57, "Tails sees Chaos 4 emerge");
			TestSpawnCutsceneList.Add(58, "Tails and Sonic part ways with Knuckles");
			TestSpawnCutsceneList.Add(59, "Tails and Sonic depart on the Tornado 1");
			TestSpawnCutsceneList.Add(62, "Tails' flashback");
			TestSpawnCutsceneList.Add(64, "Tails wakes up from his dream");
			TestSpawnCutsceneList.Add(66, "Tails chases Froggy");
			TestSpawnCutsceneList.Add(68, "Tails enters the Past");
			TestSpawnCutsceneList.Add(69, "Tails talks to Tikal");
			TestSpawnCutsceneList.Add(70, "Tails meets Big and lets go of Froggy");
			TestSpawnCutsceneList.Add(71, "Tails takes off on the Tornado 2");
			TestSpawnCutsceneList.Add(72, "Tails finds Sonic in Red Mountain");
			TestSpawnCutsceneList.Add(75, "Tails faces off with Gamma");
			TestSpawnCutsceneList.Add(76, "Tails and Amy escape from the Egg Carrier");
			TestSpawnCutsceneList.Add(77, "Tails sees Eggman launching his missile attack");
			TestSpawnCutsceneList.Add(78, "Tails follows Eggman after the missile");
			TestSpawnCutsceneList.Add(80, "Tails takes on the Egg Walker");
			TestSpawnCutsceneList.Add(81, "Tails defeated the Egg Walker");
			TestSpawnCutsceneList.Add(82, "Tails outro");
			TestSpawnCutsceneList.Add(83, "Error");
			TestSpawnCutsceneList.Add(84, "Tails and Sonic landing on the Egg Carrier");
			TestSpawnCutsceneList.Add(85, "Tails and Froggy go to the Past");
			TestSpawnCutsceneList.Add(86, "Tails and Sonic awake after being gassed");
			TestSpawnCutsceneList.Add(88, "Unused");
			TestSpawnCutsceneList.Add(96, "Amy intro");
			TestSpawnCutsceneList.Add(97, "Amy meets Birdie");
			TestSpawnCutsceneList.Add(98, "Amy meets up with Sonic");
			TestSpawnCutsceneList.Add(99, "Amy and Sonic visit Twinkle Park");
			TestSpawnCutsceneList.Add(100, "Amy is kidnapped by Zero");
			TestSpawnCutsceneList.Add(101, "Amy is released by Gamma");
			TestSpawnCutsceneList.Add(102, "Amy escapes Hot Shelter");
			TestSpawnCutsceneList.Add(103, "Amy finds herself in the Past");
			TestSpawnCutsceneList.Add(104, "Amy meets Tikal");
			TestSpawnCutsceneList.Add(105, "Amy sees Eggman take Birdie's Emerald");
			TestSpawnCutsceneList.Add(106, "Amy and Tails escape the Egg Carrier");
			TestSpawnCutsceneList.Add(107, "Error");
			TestSpawnCutsceneList.Add(108, "Amy returns to the present");
			TestSpawnCutsceneList.Add(109, "Amy decides to help find Birdie's family");
			TestSpawnCutsceneList.Add(110, "Amy discovers the Final Egg Base");
			TestSpawnCutsceneList.Add(111, "Amy chased by Zero in Final Egg");
			TestSpawnCutsceneList.Add(112, "Amy and Birdie head back to the Egg Carrier");
			TestSpawnCutsceneList.Add(113, "Amy is confronted by Zero");
			TestSpawnCutsceneList.Add(114, "Amy outro");
			TestSpawnCutsceneList.Add(117, "Amy is kidnapped to the Mystic Ruins");
			TestSpawnCutsceneList.Add(128, "Knuckles intro");
			TestSpawnCutsceneList.Add(130, "Knuckles goes hunting for the Master Emerald");
			TestSpawnCutsceneList.Add(131, "Knuckles taken to the Past from Casinopolis");
			TestSpawnCutsceneList.Add(132, "Knuckles finds himself in the Past");
			TestSpawnCutsceneList.Add(133, "Knuckles sees Tikal talk to her father");
			TestSpawnCutsceneList.Add(134, "Knuckles returns from the Past to Station Square");
			TestSpawnCutsceneList.Add(135, "Knuckles and Chaos 2 face off");
			TestSpawnCutsceneList.Add(136, "Knuckles is tricked by Eggman");
			TestSpawnCutsceneList.Add(137, "Knuckles goes after Sonic");
			TestSpawnCutsceneList.Add(138, "Knuckles vs. Sonic");
			TestSpawnCutsceneList.Add(139, "Knuckles sees Chaos 4 emerge");
			TestSpawnCutsceneList.Add(140, "Knuckles parts ways with Sonic and Tails");
			TestSpawnCutsceneList.Add(141, "Knuckles goes to the Past from the temple");
			TestSpawnCutsceneList.Add(142, "Knuckles at the Master Emerald Altar");
			TestSpawnCutsceneList.Add(143, "Knuckles sees Tikal talking to Chaos");
			TestSpawnCutsceneList.Add(145, "Knuckles restores most of the Master Emerald");
			TestSpawnCutsceneList.Add(146, "Knuckles follows Gamma");
			TestSpawnCutsceneList.Add(148, "Knuckles arrives on the Egg Carrier");
			TestSpawnCutsceneList.Add(149, "Knuckles finds the last missing piece");
			TestSpawnCutsceneList.Add(150, "Knuckles sees the Master Emerald Altar on fire");
			TestSpawnCutsceneList.Add(151, "Knuckles witnesses the aftermath of Tikal's plight");
			TestSpawnCutsceneList.Add(152, "Knuckles is back to the Egg Carrier");
			TestSpawnCutsceneList.Add(153, "Knuckles fights Chaos 6");
			TestSpawnCutsceneList.Add(154, "Knuckles has collected the final shards");
			TestSpawnCutsceneList.Add(155, "Knuckles defeats Chaos 6");
			TestSpawnCutsceneList.Add(156, "Error");
			TestSpawnCutsceneList.Add(157, "Knuckles restores the Master Emerald");
			TestSpawnCutsceneList.Add(159, "Knuckles outro");
			TestSpawnCutsceneList.Add(160, "Knuckles follows Eggman in Station Square hotel");
			TestSpawnCutsceneList.Add(176, "Gamma intro");
			TestSpawnCutsceneList.Add(177, "Gamma enters Final Egg");
			TestSpawnCutsceneList.Add(178, "Gamma completes his first objective at Final Egg");
			TestSpawnCutsceneList.Add(179, "Gamma is told that he is a useless machine");
			TestSpawnCutsceneList.Add(180, "Gamma's first fight with Beta");
			TestSpawnCutsceneList.Add(181, "Gamma defeats Beta");
			TestSpawnCutsceneList.Add(183, "Gamma and other E Series briefing");
			TestSpawnCutsceneList.Add(184, "Gamma and Froggy go to the Past");
			TestSpawnCutsceneList.Add(185, "Gamma cannot determine location");
			TestSpawnCutsceneList.Add(186, "Gamma meets Tikal");
			TestSpawnCutsceneList.Add(187, "Gamma brings Froggy to Eggman");
			TestSpawnCutsceneList.Add(188, "Gamma goes to the wrong room");
			TestSpawnCutsceneList.Add(189, "Gamma witnesses Beta being rebuilt");
			TestSpawnCutsceneList.Add(190, "Gamma leaves Beta's room");
			TestSpawnCutsceneList.Add(191, "Gamma meets and releases Amy");
			TestSpawnCutsceneList.Add(192, "Gamma heading to the rear of the ship");
			TestSpawnCutsceneList.Add(193, "Gamma emerges to fight Sonic");
			TestSpawnCutsceneList.Add(194, "Gamma leaves the Egg Carrier");
			TestSpawnCutsceneList.Add(195, "Gamma's objectives changed");
			TestSpawnCutsceneList.Add(197, "Gamma sets out to rescue Zeta and Beta");
			TestSpawnCutsceneList.Add(199, "Gamma outro");
			TestSpawnCutsceneList.Add(208, "Big intro");
			TestSpawnCutsceneList.Add(209, "Big goes searching for Froggy");
			TestSpawnCutsceneList.Add(210, "Big sees Froggy heading into the sewers");
			TestSpawnCutsceneList.Add(211, "Big finds Froggy and Tails");
			TestSpawnCutsceneList.Add(212, "Big loses Froggy to Gamma");
			TestSpawnCutsceneList.Add(216, "Big enters Hot Shelter");
			TestSpawnCutsceneList.Add(217, "Big spots Froggy inside the tanks");
			TestSpawnCutsceneList.Add(218, "Big saves Froggy in Hot Shelter");
			TestSpawnCutsceneList.Add(219, "Big finds himself in the Past");
			TestSpawnCutsceneList.Add(220, "Big sees Tikal and the Master Emerald");
			TestSpawnCutsceneList.Add(221, "Big returns to the Egg Carrier");
			TestSpawnCutsceneList.Add(222, "Big sees Froggy taken by Chaos 6");
			TestSpawnCutsceneList.Add(223, "Big thanks Sonic for saving Froggy");
			TestSpawnCutsceneList.Add(224, "Big and Froggy escape on the Tornado 2");
			TestSpawnCutsceneList.Add(225, "Error");
			TestSpawnCutsceneList.Add(226, "Big outro");
			TestSpawnCutsceneList.Add(227, "Big sees Froggy heading to the beach");
			TestSpawnCutsceneList.Add(240, "Crashed Tornado 2 in the jungle");
			TestSpawnCutsceneList.Add(242, "Eggman heading to the Mystic Ruins base");
			TestSpawnCutsceneList.Add(243, "Knuckles at the Master Emerald");
			TestSpawnCutsceneList.Add(244, "Tails tells Sonic about Angel Island falling");
			TestSpawnCutsceneList.Add(245, "Sonic and Tails find Eggman and Knuckles");
			TestSpawnCutsceneList.Add(246, "Sonic travels to the Master Emerald Altar");
			TestSpawnCutsceneList.Add(247, "Tikal pleads with her father");
			TestSpawnCutsceneList.Add(248, "Tikal seals Chaos");
			TestSpawnCutsceneList.Add(249, "Sonic returns to the present");
			TestSpawnCutsceneList.Add(250, "Sonic and Tails find the Tornado 2");
			TestSpawnCutsceneList.Add(251, "Sonic checks on Tikal in the past");
			TestSpawnCutsceneList.Add(253, "Perfect Chaos reveals himself");
			TestSpawnCutsceneList.Add(254, "Last Story outro");
			TestSpawnCutsceneList.Add(255, "Everyone brings Sonic the emeralds");
			TestSpawnCutsceneList.Add(256, "Sonic and Tails after landing on the Egg Carrier");
			TestSpawnCutsceneList.Add(257, "Don't get too many ideas you fools! (Sonic)");
			TestSpawnCutsceneList.Add(258, "The Egg Carrier has changed chape (Sonic)");
			TestSpawnCutsceneList.Add(259, "Sonic at the Sky Deck entrance");
			TestSpawnCutsceneList.Add(260, "Sonic got into the Egg Carrier (Is that it?)");
			TestSpawnCutsceneList.Add(262, "Sonic heading to transform the Egg Carrier");
			TestSpawnCutsceneList.Add(263, "Emergency alert cancelled (Sonic)");
			TestSpawnCutsceneList.Add(272, "Tails and Sonic after landing on the Egg Carrier");
			TestSpawnCutsceneList.Add(273, "Don't get too many ideas (Tails)");
			TestSpawnCutsceneList.Add(274, "The Egg Carrier has changed shape (Tails)");
			TestSpawnCutsceneList.Add(275, "Tails at the Sky Deck entrance");
			TestSpawnCutsceneList.Add(276, "Tails got into the Egg Carrier (Is that it?)");
			TestSpawnCutsceneList.Add(288, "The Egg Carrier changes shape (Knuckles)");
			TestSpawnCutsceneList.Add(289, "The Egg Carrier transforms again (Knuckles)");
			TestSpawnCutsceneList.Add(290, "Knuckles sensing the emeralds on the Egg Carrier");
			TestSpawnCutsceneList.Add(304, "Amy in the Hedgehog Hammer room");
			TestSpawnCutsceneList.Add(305, "Amy wins Hedgehog Hammer");
			TestSpawnCutsceneList.Add(320, "Gamma is told to find the Jet Booster");
			TestSpawnCutsceneList.Add(321, "Gamma heads to Hot Shelter");
			TestSpawnCutsceneList.Add(322, "Gamma spots E-101 R");
			TestSpawnCutsceneList.Add(336, "Emergency alert cancelled (Big)");
			TestSpawnCutsceneList.Add(352, "The Echidna tribe faces Chaos");
			TestSpawnCutsceneList.Add(357, "Sonic gets the Crystal Ring");
			TestSpawnCutsceneList.Add(358, "Sonic gets the Light Speed Shoes");
			TestSpawnCutsceneList.Add(359, "Sonic gets the Ancient Light");
			TestSpawnCutsceneList.Add(360, "Tails gets the Jet Anklet");
			TestSpawnCutsceneList.Add(361, "Tails gets the Rhythm Badge");
			TestSpawnCutsceneList.Add(362, "Knuckles gets the Fighting Gloves");
			TestSpawnCutsceneList.Add(363, "Knuckles gets the Shovel Claw");
			TestSpawnCutsceneList.Add(364, "Amy gets the Long Hammer");
			TestSpawnCutsceneList.Add(365, "Amy gets the Warrior Feather");
			TestSpawnCutsceneList.Add(366, "Gamma gets the Laser Blaster");
			TestSpawnCutsceneList.Add(367, "Gamma gets the Jet Booster");
			TestSpawnCutsceneList.Add(368, "Big gets the Power Rod");
			TestSpawnCutsceneList.Add(369, "Big gets the Life Belt");
			TestSpawnCutsceneList.Add(374, "Ice Stone appears (Sonic)");
			TestSpawnCutsceneList.Add(375, "Ice Stone appears (Tails)");
			TestSpawnCutsceneList.Add(376, "Ice Stone appears (Big)");
			TestSpawnCutsceneList.Add(377, "Employee Card appears (Sonic)");
			TestSpawnCutsceneList.Add(378, "Passage to Angel Island opens (Sonic)");
			TestSpawnCutsceneList.Add(379, "Passage to Angel Island opens (Tails)");
			TestSpawnCutsceneList.Add(380, "Passage to Angel Island opens (Gamma)");
			TestSpawnCutsceneList.Add(384, "Sonic sees the Egg Carrier in Red Mountain");
			comboBoxTestSpawnEvent.Items.Clear();
			foreach (var item in TestSpawnCutsceneList)
			{
				comboBoxTestSpawnEvent.Items.Add("EV" + item.Key.ToString("X4") + ": " + item.Value);
			}
		}
		private void InitTestSpawnGameModeList()
		{
			TestSpawnGameModeList = new Dictionary<ushort, string>
			{
				{ 4, "Action Stage" },
				{ 5, "Adventure" },
				{ 9, "Trial" },
				{ 10, "Mission" }
			};
			comboBoxTestSpawnGameMode.Items.Clear();
			foreach (var item in TestSpawnGameModeList)
			{
				comboBoxTestSpawnGameMode.Items.Add(item.Value);
			}
		}

		private void checkBoxTestSpawnAngleHex_CheckedChanged(object sender, EventArgs e)
		{
			numericUpDownTestSpawnAngle.Hexadecimal = checkBoxTestSpawnAngleHex.Checked;
		}

		private void ShowOrHideTestSpawnEventWarning()
		{
			if (!checkBoxTestSpawnEvent.Checked)
				labelTestSpawnWarning.Visible = false;
			else
				labelTestSpawnWarning.Visible = checkBoxTestSpawnCharacter.Checked || checkBoxTestSpawnLevel.Checked;
		}
		#endregion

		#region Direct3D wrapper
		private void CopyD3D9Dll()
		{
			try
			{
				File.Copy(d3d8to9StoredDLLName, d3d8to9InstalledDLLName, true);
			}
			catch (Exception ex)
			{
				MessageBox.Show(this, "Unable to update d3d8.dll:\n" + ex.Message, "SADX Mod Manager", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
		}

		private void checkBoxEnableD3D9_Click(object sender, EventArgs e)
		{
			if (checkBoxEnableD3D9.Checked)
			{
				CopyD3D9Dll();
			}
			else if (!checkBoxEnableD3D9.Checked && File.Exists(d3d8to9InstalledDLLName))
				File.Delete(d3d8to9InstalledDLLName);
		}

		private void buttonUpdateD3D8to9_Click(object sender, EventArgs e)
		{
			DialogResult update = MessageBox.Show(this,
												  "The version of d3d8.dll in SADX folder differs from the one included with the Mod Loader." +
												  " Would you like to update the installed copy?",
												  "SADX Mod Manager",
												  MessageBoxButtons.YesNo,
												  MessageBoxIcon.Question);
			if (update == DialogResult.Yes)
			{
				CopyD3D9Dll();
				buttonUpdateD3D8to9.Visible = CheckD3D8to9Update();
			}
		}

		private bool CheckD3D8to9Update()
		{
			if (!File.Exists(d3d8to9StoredDLLName) || !File.Exists(d3d8to9InstalledDLLName))
				return false;
			try
			{
				long length1 = new FileInfo(d3d8to9InstalledDLLName).Length;
				long length2 = new FileInfo(d3d8to9StoredDLLName).Length;
				if (length1 != length2)
					return true;
				else
				{
					byte[] file1 = File.ReadAllBytes(d3d8to9InstalledDLLName);
					byte[] file2 = File.ReadAllBytes(d3d8to9StoredDLLName);
					for (int i = 0; i < file1.Length; i++)
					{
						if (file1[i] != file2[i])
							return true;
					}
					return false;
				}
			}
			catch (Exception ex)
			{
				MessageBox.Show(this, "Unable to check d3d8to9 version:\n" + ex.Message, "SADX Mod Manager", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return false;
			}
		}
		#endregion

		#region Mod profiles
		private void profileNameBox_TextChanged(object sender, EventArgs e)
		{
			if (string.IsNullOrEmpty(profileNameBox.Text) || profileNameBox.Text.Equals("SADXModLoader", StringComparison.OrdinalIgnoreCase)
				|| profileNameBox.Text.Equals("mod", StringComparison.OrdinalIgnoreCase) || profileNameBox.Text.Equals("desktop", StringComparison.OrdinalIgnoreCase)
				|| profileNameBox.Text.IndexOfAny(Path.GetInvalidFileNameChars()) != -1)
			{
				buttonSaveProfile.Enabled = false;
				buttonLoadProfile.Enabled = false;
			}
			else
			{
				buttonSaveProfile.Enabled = true;
				buttonLoadProfile.Enabled = File.Exists($"mods/{profileNameBox.Text}.ini");
			}
		}

		private void buttonLoadProfile_Click(object sender, EventArgs e)
		{
			loaderini = IniSerializer.Deserialize<SADXLoaderInfo>($"mods/{profileNameBox.Text}.ini");
			LoadSettings();
		}

		private void buttonSaveProfile_Click(object sender, EventArgs e)
		{
			Save();
			File.Copy(loaderinipath, $"mods/{profileNameBox.Text}.ini", true);
			if (profileNameBox.FindStringExact(profileNameBox.Text) == -1)
				profileNameBox.Items.Add(profileNameBox.Text);
			buttonLoadProfile.Enabled = true;
		}
		#endregion

		#region Input settings
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
					buttonControls[i].Text = config.Buttons[i] == -1 ? "Unassigned" : "Button " + (config.Buttons[i] + 1).ToString("D2");
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

		private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
		{
			directInput?.Dispose();
			directInput = null;

			inputDevice?.Dispose();
			inputDevice = null;
		}

		private void MainForm_KeyDown(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Escape && controllerThread != null && controllerThread.IsAlive)
			{
				controllerThread.Abort();
				inputDevice.Unacquire();
				inputDevice.SetNotification(null);
				ControllerConfigInternal config = controllerConfig[controllerConfigSelect.SelectedIndex];
				buttonControls[currentAction].Text = config.Buttons[currentAction] == -1
					? "Unassigned"
					: "Button " + (config.Buttons[currentAction] + 1).ToString("D2");
				Enabled = true;
			}
		}
		#endregion


	}
}