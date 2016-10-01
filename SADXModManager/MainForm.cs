using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Windows.Forms;
using System.Xml.Serialization;
using IniSerializer;

namespace SADXModManager
{
	public partial class MainForm : Form
	{
		public MainForm()
		{
			InitializeComponent();
		}

		const string datadllpath = "system/CHRMODELS.dll";
		const string datadllorigpath = "system/CHRMODELS_orig.dll";
		const string loaderinipath = "mods/SADXModLoader.ini";
		const string loaderdllpath = "mods/SADXModLoader.dll";
		LoaderInfo loaderini;
		Dictionary<string, ModInfo> mods;
		const string codexmlpath = "mods/Codes.xml";
		const string codedatpath = "mods/Codes.dat";
		const string patchdatpath = "mods/Patches.dat";
		CodeList mainCodes;
		List<Code> codes;
		bool installed;
		bool suppressEvent;

		private void MainForm_Load(object sender, EventArgs e)
		{
			if (File.Exists(loaderinipath))
				loaderini = IniFile.Deserialize<LoaderInfo>(loaderinipath);
			else
				loaderini = new LoaderInfo();

			try { mainCodes = CodeList.Load(codexmlpath); }
			catch { mainCodes = new CodeList() { Codes = new List<Code>() }; }

			LoadModList();

			for (int i = 0; i < Screen.AllScreens.Length; i++)
				screenNumComboBox.Items.Add((i + 1).ToString() + " " + Screen.AllScreens[i].DeviceName + " (" + Screen.AllScreens[i].Bounds.Location.X + "," + Screen.AllScreens[i].Bounds.Y + ") " + Screen.AllScreens[i].Bounds.Width + "x" + Screen.AllScreens[i].Bounds.Height + " " + Screen.AllScreens[i].BitsPerPixel + "bpp" + (Screen.AllScreens[i].Primary ? " Primary" : ""));

			consoleCheckBox.Checked = loaderini.DebugConsole;
			screenCheckBox.Checked = loaderini.DebugScreen;
			fileCheckBox.Checked = loaderini.DebugFile;
			disableCDCheckCheckBox.Checked = loaderini.DisableCDCheck;
			useCustomResolutionCheckBox.Checked = verticalResolution.Enabled = forceAspectRatioCheckBox.Enabled = nativeResolutionButton.Enabled = loaderini.UseCustomResolution;
			checkVsync.Checked = loaderini.EnableVsync;
			horizontalResolution.Enabled = loaderini.UseCustomResolution && !loaderini.ForceAspectRatio;
			horizontalResolution.Value = Math.Max(horizontalResolution.Minimum, Math.Min(horizontalResolution.Maximum, loaderini.HorizontalResolution));
			verticalResolution.Value = Math.Max(verticalResolution.Minimum, Math.Min(verticalResolution.Maximum, loaderini.VerticalResolution));
			suppressEvent = true;
			forceAspectRatioCheckBox.Checked = loaderini.ForceAspectRatio;
			checkScaleHud.Checked = loaderini.ScaleHud;
			suppressEvent = false;
			windowedFullscreenCheckBox.Checked = loaderini.WindowedFullscreen;
			forceMipmappingCheckBox.Checked = loaderini.AutoMipmap;
			forceTextureFilterCheckBox.Checked = loaderini.TextureFilter;
			pauseWhenInactiveCheckBox.Checked = loaderini.PauseWhenInactive;
			stretchFullscreenCheckBox.Checked = loaderini.StretchFullscreen;
			int scrn = loaderini.ScreenNum;
			if (scrn > Screen.AllScreens.Length)
				scrn = 1;
			screenNumComboBox.SelectedIndex = scrn;
			customWindowSizeCheckBox.Checked = windowHeight.Enabled = maintainWindowAspectRatioCheckBox.Enabled = loaderini.CustomWindowSize;
			windowWidth.Enabled = loaderini.CustomWindowSize && !loaderini.MaintainWindowAspectRatio;
			System.Drawing.Rectangle rect = Screen.PrimaryScreen.Bounds;
			foreach (Screen screen in Screen.AllScreens)
				rect = System.Drawing.Rectangle.Union(rect, screen.Bounds);
			windowWidth.Maximum = rect.Width;
			windowWidth.Value = Math.Max(windowWidth.Minimum, Math.Min(rect.Width, loaderini.WindowWidth));
			windowHeight.Maximum = rect.Height;
			windowHeight.Value = Math.Max(windowHeight.Minimum, Math.Min(rect.Height, loaderini.WindowHeight));
			suppressEvent = true;
			maintainWindowAspectRatioCheckBox.Checked = loaderini.MaintainWindowAspectRatio;
			suppressEvent = false;
			if (!File.Exists(datadllpath))
			{
				MessageBox.Show(this, "CHRMODELS.dll could not be found.\n\nCannot determine state of installation.", Text, MessageBoxButtons.OK, MessageBoxIcon.Warning);
				installButton.Hide();
			}
			else if (File.Exists(datadllorigpath))
			{
				installed = true;
				installButton.Text = "Uninstall loader";
				MD5 md5 = MD5.Create();
				byte[] hash1 = md5.ComputeHash(File.ReadAllBytes(loaderdllpath));
				byte[] hash2 = md5.ComputeHash(File.ReadAllBytes(datadllpath));
				if (!hash1.SequenceEqual(hash2))
					if (MessageBox.Show(this, "Installed loader DLL differs from copy in mods folder.\n\nDo you want to overwrite the installed copy?", Text, MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.Yes)
						File.Copy(loaderdllpath, datadllpath, true);
			}
		}

		private void LoadModList()
		{
			modListView.Items.Clear();
			mods = new Dictionary<string, ModInfo>();
			codes = new List<Code>(mainCodes.Codes);
			string modDir = Path.Combine(Environment.CurrentDirectory, "mods");
			foreach (string filename in GetModFiles(new DirectoryInfo(modDir)))
				mods.Add(Path.GetDirectoryName(filename).Substring(modDir.Length + 1), IniFile.Deserialize<ModInfo>(filename));
			modListView.BeginUpdate();
			foreach (string mod in new List<string>(loaderini.Mods))
			{
				if (mods.ContainsKey(mod))
				{
					ModInfo inf = mods[mod];
					suppressEvent = true;
					modListView.Items.Add(new ListViewItem(new[] { inf.Name, inf.Author, inf.Version }) { Checked = true, Tag = mod });
					suppressEvent = false;
					if (!string.IsNullOrEmpty(inf.Codes))
						codes.AddRange(CodeList.Load(Path.Combine(Path.Combine(modDir, mod), inf.Codes)).Codes);
				}
				else
				{
					MessageBox.Show(this, "Mod \"" + mod + "\" could not be found.\n\nThis mod will be removed from the list.", Text, MessageBoxButtons.OK, MessageBoxIcon.Warning);
					loaderini.Mods.Remove(mod);
				}
			}
			foreach (KeyValuePair<string, ModInfo> inf in mods)
				if (!loaderini.Mods.Contains(inf.Key))
					modListView.Items.Add(new ListViewItem(new[] { inf.Value.Name, inf.Value.Author, inf.Value.Version }) { Tag = inf.Key });
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

		private static IEnumerable<string> GetModFiles(DirectoryInfo directoryInfo)
		{
			string modini = Path.Combine(directoryInfo.FullName, "mod.ini");
			if (File.Exists(modini))
			{
				yield return modini;
				yield break;
			}

			foreach (DirectoryInfo item in directoryInfo.GetDirectories())
			{
				if (!item.Name.Equals("system", StringComparison.OrdinalIgnoreCase))
				{
					foreach (string filename in GetModFiles(item))
						yield return filename;
				}
			}
		}

		private void modListView_SelectedIndexChanged(object sender, EventArgs e)
		{
			int count = modListView.SelectedIndices.Count;
			if (count == 0)
			{
				modUpButton.Enabled = modDownButton.Enabled = false;
				modDescription.Text = "Description: No mod selected.";
			}
			else if (count == 1)
			{
				modDescription.Text = "Description: " + mods[(string)modListView.SelectedItems[0].Tag].Description;
				modUpButton.Enabled = modListView.SelectedIndices[0] > 0;
				modDownButton.Enabled = modListView.SelectedIndices[0] < modListView.Items.Count - 1;
			}
			else if (count > 1)
			{
				modDescription.Text = "Description: Multiple mods selected.";
				modUpButton.Enabled = modDownButton.Enabled = true;
			}
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

		private void Save()
		{
			loaderini.Mods.Clear();
			foreach (ListViewItem item in modListView.CheckedItems)
				loaderini.Mods.Add((string)item.Tag);
			loaderini.DebugConsole = consoleCheckBox.Checked;
			loaderini.DebugScreen = screenCheckBox.Checked;
			loaderini.DebugFile = fileCheckBox.Checked;
			loaderini.DisableCDCheck = disableCDCheckCheckBox.Checked;
			loaderini.UseCustomResolution = useCustomResolutionCheckBox.Checked;
			loaderini.HorizontalResolution = (int)horizontalResolution.Value;
			loaderini.VerticalResolution = (int)verticalResolution.Value;
			loaderini.ForceAspectRatio = forceAspectRatioCheckBox.Checked;
			loaderini.ScaleHud = checkScaleHud.Checked;
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
			IniFile.Serialize(loaderini, loaderinipath);
			List<Code> codes = new List<Code>();
			List<Code> patches = new List<Code>();
			foreach (Code item in codesCheckedListBox.CheckedIndices.OfType<int>().Select(a => this.codes[a]))
				if (item.Patch)
					patches.Add(item);
				else
					codes.Add(item);
			using (FileStream fs = File.Create(patchdatpath))
			using (BinaryWriter bw = new BinaryWriter(fs, System.Text.Encoding.ASCII))
			{
				bw.Write(new[] { 'c', 'o', 'd', 'e', 'v', '5' });
				bw.Write(patches.Count);
				foreach (Code item in patches)
				{
					if (item.IsReg)
						bw.Write((byte)CodeType.newregs);
					WriteCodes(item.Lines, bw);
				}
				bw.Write(byte.MaxValue);
			}
			using (FileStream fs = File.Create(codedatpath))
			using (BinaryWriter bw = new BinaryWriter(fs, System.Text.Encoding.ASCII))
			{
				bw.Write(new[] { 'c', 'o', 'd', 'e', 'v', '5' });
				bw.Write(codes.Count);
				foreach (Code item in codes)
				{
					if (item.IsReg)
						bw.Write((byte)CodeType.newregs);
					WriteCodes(item.Lines, bw);
				}
				bw.Write(byte.MaxValue);
			}
		}

		private void WriteCodes(IEnumerable<CodeLine> codeList, BinaryWriter writer)
		{
			foreach (CodeLine line in codeList)
			{
				writer.Write((byte)line.Type);
				uint address;
				if (line.Address.StartsWith("r"))
					address = uint.Parse(line.Address.Substring(1), System.Globalization.NumberStyles.None, System.Globalization.NumberFormatInfo.InvariantInfo);
				else
					address = uint.Parse(line.Address, System.Globalization.NumberStyles.HexNumber);
				if (line.Pointer)
					address |= 0x80000000u;
				writer.Write(address);
				if (line.Pointer)
					if (line.Offsets != null)
					{
						writer.Write((byte)line.Offsets.Count);
						foreach (int off in line.Offsets)
							writer.Write(off);
					}
					else
						writer.Write((byte)0);
				if (line.Type == CodeType.ifkbkey)
					writer.Write((int)(Keys)Enum.Parse(typeof(Keys), line.Value));
				else
					switch (line.ValueType)
					{
						case ValueType.@decimal:
							switch (line.Type)
							{
								case CodeType.writefloat:
								case CodeType.addfloat:
								case CodeType.subfloat:
								case CodeType.mulfloat:
								case CodeType.divfloat:
								case CodeType.ifeqfloat:
								case CodeType.ifnefloat:
								case CodeType.ifltfloat:
								case CodeType.iflteqfloat:
								case CodeType.ifgtfloat:
								case CodeType.ifgteqfloat:
									writer.Write(float.Parse(line.Value, System.Globalization.NumberStyles.Float, System.Globalization.NumberFormatInfo.InvariantInfo));
									break;
								default:
									writer.Write(unchecked((int)long.Parse(line.Value, System.Globalization.NumberStyles.Integer, System.Globalization.NumberFormatInfo.InvariantInfo)));
									break;
							}
							break;
						case ValueType.hex:
							writer.Write(uint.Parse(line.Value, System.Globalization.NumberStyles.HexNumber, System.Globalization.NumberFormatInfo.InvariantInfo));
							break;
					}
				writer.Write(line.RepeatCount ?? 1);
				if (line.IsIf)
				{
					WriteCodes(line.TrueLines, writer);
					if (line.FalseLines.Count > 0)
					{
						writer.Write((byte)CodeType.@else);
						WriteCodes(line.FalseLines, writer);
					}
					writer.Write((byte)CodeType.endif);
				}
			}
		}

		private void saveAndPlayButton_Click(object sender, EventArgs e)
		{
			Save();
			System.Diagnostics.Process.Start(loaderini.Mods.Select((item) => mods[item].EXEFile)
				.FirstOrDefault((item) => !string.IsNullOrEmpty(item)) ?? "sonic.exe").WaitForInputIdle(10000);
			Close();
		}

		private void saveButton_Click(object sender, EventArgs e)
		{
			Save();
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

		private void useCustomResolutionCheckBox_CheckedChanged(object sender, EventArgs e)
		{
			if (nativeResolutionButton.Enabled = forceAspectRatioCheckBox.Enabled =
				verticalResolution.Enabled = useCustomResolutionCheckBox.Checked)
				horizontalResolution.Enabled = !forceAspectRatioCheckBox.Checked;
			else
				horizontalResolution.Enabled = false;
		}

		const decimal ratio = 4 / 3m;
		private void forceAspectRatioCheckBox_CheckedChanged(object sender, EventArgs e)
		{
			if (forceAspectRatioCheckBox.Checked)
			{
				horizontalResolution.Enabled = false;
				horizontalResolution.Value = Math.Round(verticalResolution.Value * ratio);
			}
			else if (!suppressEvent)
				horizontalResolution.Enabled = true;
		}

		private void verticalResolution_ValueChanged(object sender, EventArgs e)
		{
			if (forceAspectRatioCheckBox.Checked)
				horizontalResolution.Value = Math.Round(verticalResolution.Value * ratio);
		}

		private void nativeResolutionButton_Click(object sender, EventArgs e)
		{
			System.Drawing.Rectangle rect = Screen.PrimaryScreen.Bounds;
			if (screenNumComboBox.SelectedIndex > 0)
				rect = Screen.AllScreens[screenNumComboBox.SelectedIndex - 1].Bounds;
			else
				foreach (Screen screen in Screen.AllScreens)
					rect = System.Drawing.Rectangle.Union(rect, screen.Bounds);
			verticalResolution.Value = rect.Height;
			if (!forceAspectRatioCheckBox.Checked)
				horizontalResolution.Value = rect.Width;
		}

		private void configEditorButton_Click(object sender, EventArgs e)
		{
			using (ConfigEditDialog dlg = new ConfigEditDialog())
				dlg.ShowDialog(this);
		}

		private void buttonRefreshModList_Click(object sender, EventArgs e)
		{
			LoadModList();
		}

		private void buttonModsFolder_Click(object sender, EventArgs e)
		{
			System.Diagnostics.Process.Start(@"mods");
		}

		private void buttonNewMod_Click(object sender, EventArgs e)
		{
			using (var ModDialog = new NewModDialog())
			{
				if (ModDialog.ShowDialog() == DialogResult.OK)
					LoadModList();
			}
		}

		private void customWindowSizeCheckBox_CheckedChanged(object sender, EventArgs e)
		{
			if (maintainWindowAspectRatioCheckBox.Enabled = windowHeight.Enabled = customWindowSizeCheckBox.Checked)
				windowWidth.Enabled = !maintainWindowAspectRatioCheckBox.Checked;
			else
				windowWidth.Enabled = false;

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
					ModInfo inf = mods[mod];
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
	}

	class LoaderInfo
	{
		public bool DebugConsole { get; set; }
		public bool DebugScreen { get; set; }
		public bool DebugFile { get; set; }
		public bool? ShowConsole { get { return null; } set { if (value.HasValue) DebugConsole = value.Value; } }
		public bool DisableCDCheck { get; set; }
		public bool UseCustomResolution { get; set; }
		[DefaultValue(640)]
		public int HorizontalResolution { get; set; }
		[DefaultValue(480)]
		public int VerticalResolution { get; set; }
		public bool ForceAspectRatio { get; set; }
		[DefaultValue(false)]
		public bool ScaleHud { get; set; }
		public bool WindowedFullscreen { get; set; }
		[DefaultValue(true)]
		public bool EnableVsync { get; set; }
		[DefaultValue(true)]
		public bool AutoMipmap { get; set; }
		[DefaultValue(true)]
		public bool TextureFilter { get; set; }
		[DefaultValue(true)]
		public bool PauseWhenInactive { get; set; }
		[DefaultValue(true)]
		public bool StretchFullscreen { get; set; }
		[DefaultValue(1)]
		public int ScreenNum { get; set; }
		public bool CustomWindowSize { get; set; }
		[DefaultValue(640)]
		public int WindowWidth { get; set; }
		[DefaultValue(480)]
		public int WindowHeight { get; set; }
		public bool MaintainWindowAspectRatio { get; set; }
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

	class ModInfo
	{
		public string Name { get; set; }
		public string Author { get; set; }
		public string Version { get; set; }
		public string Description { get; set; }
		public string EXEFile { get; set; }
		public string DLLFile { get; set; }
		public bool RedirectMainSave { get; set; }
		public bool RedirectChaoSave { get; set; }
		public string Codes { get; set; }
	}

	[XmlRoot(Namespace = "http://www.sonicretro.org")]
	public class CodeList
	{
		static readonly XmlSerializer serializer = new XmlSerializer(typeof(CodeList));

		public static CodeList Load(string filename)
		{
			using (FileStream fs = File.OpenRead(filename))
				return (CodeList)serializer.Deserialize(fs);
		}

		public void Save(string filename)
		{
			using (FileStream fs = File.Create(filename))
				serializer.Serialize(fs, this);
		}

		[XmlElement("Code")]
		public List<Code> Codes { get; set; }
	}

	public class Code
	{
		[XmlAttribute("name")]
		public string Name { get; set; }
		[XmlAttribute("required")]
		public bool Required { get; set; }
		[XmlAttribute("patch")]
		public bool Patch { get; set; }
		[XmlElement("CodeLine")]
		public List<CodeLine> Lines { get; set; }

		[XmlIgnore]
		public bool IsReg { get { return Lines.Any((line) => line.IsReg); } }
	}

	public class CodeLine
	{
		public CodeType Type { get; set; }
		[XmlElement(IsNullable = false)]
		public string Address { get; set; }
		public bool Pointer { get; set; }
		[XmlIgnore]
		public bool PointerSpecified { get { return Pointer; } set { } }
		[XmlIgnore]
		public List<int> Offsets { get; set; }
		[XmlArray("Offsets")]
		[XmlArrayItem("Offset")]
		public string[] OffsetStrings
		{
			get { return Offsets == null ? null : Offsets.Select((a) => a.ToString("X")).ToArray(); }
			set { Offsets = value.Select((a) => int.Parse(a, System.Globalization.NumberStyles.HexNumber)).ToList(); }
		}
		[XmlIgnore]
		public bool OffsetStringsSpecified { get { return Offsets != null && Offsets.Count > 0; } set { } }
		[XmlElement(IsNullable = false)]
		public string Value { get; set; }
		public ValueType ValueType { get; set; }
		public uint? RepeatCount { get; set; }
		[XmlIgnore]
		public bool RepeatCountSpecified { get { return RepeatCount.HasValue; } set { } }
		[XmlArray]
		public List<CodeLine> TrueLines { get; set; }
		[XmlIgnore]
		public bool TrueLinesSpecified { get { return TrueLines.Count > 0 && IsIf; } set { } }
		[XmlArray]
		public List<CodeLine> FalseLines { get; set; }
		[XmlIgnore]
		public bool FalseLinesSpecified { get { return FalseLines.Count > 0 && IsIf; } set { } }

		[XmlIgnore]
		public bool IsIf
		{
			get
			{
				return (Type >= CodeType.ifeq8 && Type <= CodeType.ifkbkey)
					|| (Type >= CodeType.ifeqreg8 && Type <= CodeType.ifmaskreg32);
			}
		}

		[XmlIgnore]
		public bool IsReg
		{
			get
			{
				if (IsIf)
				{
					if (TrueLines.Any((line) => line.IsReg))
						return true;
					if (FalseLines.Any((line) => line.IsReg))
						return true;
				}
				if (Address.StartsWith("r"))
					return true;
				if (Type >= CodeType.readreg8 && Type <= CodeType.ifmaskreg32)
					return true;
				return false;
			}
		}
	}

	public enum CodeType
	{
		write8, write16, write32, writefloat,
		add8, add16, add32, addfloat,
		sub8, sub16, sub32, subfloat,
		mulu8, mulu16, mulu32, mulfloat,
		muls8, muls16, muls32,
		divu8, divu16, divu32, divfloat,
		divs8, divs16, divs32,
		modu8, modu16, modu32,
		mods8, mods16, mods32,
		shl8, shl16, shl32,
		shru8, shru16, shru32,
		shrs8, shrs16, shrs32,
		rol8, rol16, rol32,
		ror8, ror16, ror32,
		and8, and16, and32,
		or8, or16, or32,
		xor8, xor16, xor32,
		writenop,
		ifeq8, ifeq16, ifeq32, ifeqfloat,
		ifne8, ifne16, ifne32, ifnefloat,
		ifltu8, ifltu16, ifltu32, ifltfloat,
		iflts8, iflts16, iflts32,
		ifltequ8, ifltequ16, ifltequ32, iflteqfloat,
		iflteqs8, iflteqs16, iflteqs32,
		ifgtu8, ifgtu16, ifgtu32, ifgtfloat,
		ifgts8, ifgts16, ifgts32,
		ifgtequ8, ifgtequ16, ifgtequ32, ifgteqfloat,
		ifgteqs8, ifgteqs16, ifgteqs32,
		ifmask8, ifmask16, ifmask32,
		ifkbkey,
		readreg8, readreg16, readreg32,
		writereg8, writereg16, writereg32,
		addreg8, addreg16, addreg32, addregfloat,
		subreg8, subreg16, subreg32, subregfloat,
		mulregu8, mulregu16, mulregu32, mulregfloat,
		mulregs8, mulregs16, mulregs32,
		divregu8, divregu16, divregu32, divregfloat,
		divregs8, divregs16, divregs32,
		modregu8, modregu16, modregu32,
		modregs8, modregs16, modregs32,
		shlreg8, shlreg16, shlreg32,
		shrregu8, shrregu16, shrregu32,
		shrregs8, shrregs16, shrregs32,
		rolreg8, rolreg16, rolreg32,
		rorreg8, rorreg16, rorreg32,
		andreg8, andreg16, andreg32,
		orreg8, orreg16, orreg32,
		xorreg8, xorreg16, xorreg32,
		writenopreg,
		ifeqreg8, ifeqreg16, ifeqreg32, ifeqregfloat,
		ifnereg8, ifnereg16, ifnereg32, ifneregfloat,
		ifltregu8, ifltregu16, ifltregu32, ifltregfloat,
		ifltregs8, ifltregs16, ifltregs32,
		iflteqregu8, iflteqregu16, iflteqregu32, iflteqregfloat,
		iflteqregs8, iflteqregs16, iflteqregs32,
		ifgtregu8, ifgtregu16, ifgtregu32, ifgtregfloat,
		ifgtregs8, ifgtregs16, ifgtregs32,
		ifgteqregu8, ifgteqregu16, ifgteqregu32, ifgteqregfloat,
		ifgteqregs8, ifgteqregs16, ifgteqregs32,
		ifmaskreg8, ifmaskreg16, ifmaskreg32,
		s8tos32, s16tos32, s32tofloat, u32tofloat, floattos32, floattou32,
		@else,
		endif,
		newregs
	}

	public enum ValueType
	{
		@decimal,
		hex
	}
}