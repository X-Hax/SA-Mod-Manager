using ModManagerWPF.Languages;
using ModManagerWPF.Themes;
using System;
using System.Linq;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using System.Xml.Linq;
using ModManagerCommon;
using ModManagerCommon.Forms;
using System.Collections.Generic;
using System.IO;
using System.Windows.Input;
using System.Text.RegularExpressions;
using System.Threading;
using IniFile;
using Xceed.Wpf.Toolkit;
using MessageBox = System.Windows.MessageBox;
using System.Threading.Tasks;
using System.Windows.Threading;
using System.Windows.Media.Animation;
using System.Diagnostics;
using ModManagerWPF.Properties;
using Xceed.Wpf.AvalonDock.Controls;
using System.Windows.Media;

namespace ModManagerWPF
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		#region Variables

		private const string pipeName = "sadx-mod-manager";
		public readonly string titleName = "SADX Mod Manager";
		public static string Version = "1.0.0";
		public static string GitVersion = string.Empty;
		public static string modDirectory = string.Empty;
		public static string loaderinipath = "mods/SADXModLoader.ini";
		private string sadxIni = "sonicDX.ini";
		string datadllorigpath = "system/CHRMODELS_orig.dll";
		string loaderdllpath = "mods/SADXModLoader.dll";
		public string datadllpath = "system/CHRMODELS.dll";
		const string exeName = "sonic.exe";

		string gamePath = string.Empty;
		SADXLoaderInfo loaderini;
		Dictionary<string, SADXModInfo> mods = null;
		string codelstpath = "mods/Codes.lst";
		string codexmlpath = "mods/Codes.xml";
		string codedatpath = "mods/Codes.dat";
		string patchdatpath = "mods/Patches.dat";
		CodeList mainCodes = null;
		List<Code> codes = null;
		bool installed = false;
		bool suppressEvent = false;

		private Game.GameConfigFile gameConfigFile;

		public Game.GameGraphics graphics;
		public GitHub git;
		static private uint count = 0;

		public class ModData
		{
			public string Name { get; set; }
			public string Author { get; set; }
			public string AuthorURL { get; set; }
			public string Version { get; set; }
			public string Category { get; set; }
			public string Description { get; set; }
			public string SourceCode { get; set; }
			public bool IsChecked { get; set; }
			public string Tag { get; set; }
		}
		#endregion

		public MainWindow()
		{	
			git = new(this);
			InitializeComponent();
			git.GetRecentCommit();
			
			graphics = new Game.GameGraphics(comboScreen);
			SetGamePath(Settings.Default.GamePath);
			UpdatePathsStringsInfo();
			LoadSettings();
			LoadModList();
			InitCodes();
			UpdateDLLData();
		}

		private void UpdateDLLData()
		{
			if (File.Exists("SADXModLoader.dll"))
			{
				File.Copy("SADXModLoader.dll", loaderdllpath, true);
			}

			if (File.Exists(loaderdllpath))
			{
				File.Copy(loaderdllpath, datadllpath, true);
			}
		}

		private void SetGamePath(string path)
		{
			if (Directory.Exists(path))
			{
				gamePath = path;
			}
			else
			{
				if (File.Exists(exeName)) //if current game path is wrong, check if the Mod Manager didn't get put in the game folder just in case.
				{
					gamePath = Directory.GetCurrentDirectory();
				}
			}
		}

		private void MainWindowManager_ContentRendered(object sender, EventArgs e)
		{
			LoadGameConfigIni();
			this.Resources.MergedDictionaries.Clear(); //this is very important to get Theme and Language swap to work on MainWindow
		}

		private void Save_AppUserSettings()
		{
			Settings.Default.Save();
		}

		#region Main

		private async void SetModManagerVersion(object sender, EventArgs e)
		{
			if (count >= 3 || Title.Length > titleName.Length + 1)
			{
				(sender as DispatcherTimer).Stop();
				return;
			}

			await git.GetRecentCommit();
			count++;
		}

		public void SetModManagerVersion()
		{
			GitVersion = git.LastCommit;
			Title = titleName + " " + "(" + Version + "-" + GitVersion + ")";
			Settings.Default.LastCommit = git.LastCommit;
		}

		private void MainWindowManager_Loaded(object sender, RoutedEventArgs e)
		{
			DispatcherTimer timer = new()
			{
				Interval = TimeSpan.FromMilliseconds(10000)
			};
			timer.Tick += SetModManagerVersion;
			timer.IsEnabled = true;
		}

		private void MainForm_FormClosing(object sender, EventArgs e)
		{
			Save_AppUserSettings();
		}

		private void UpdatePathsStringsInfo()
		{
			if (!string.IsNullOrEmpty(gamePath) && File.Exists(Path.Combine(gamePath, exeName)))
			{
				modDirectory = Path.Combine(gamePath, "mods");
				loaderinipath = Path.Combine(gamePath, "mods/SADXModLoader.ini");
				datadllorigpath = Path.Combine(gamePath, "system/CHRMODELS_orig.dll");
				loaderdllpath = Path.Combine(gamePath, "mods/SADXModLoader.dll");
				datadllpath = Path.Combine(gamePath, "system/CHRMODELS.dll");
				sadxIni = Path.Combine(gamePath, "sonicDX.ini");

				codelstpath = Path.Combine(gamePath, "mods/Codes.lst");
				codexmlpath = Path.Combine(gamePath, "mods/Codes.xml");
				codedatpath = Path.Combine(gamePath, "mods/Codes.dat");
				patchdatpath = Path.Combine(gamePath, "mods/Patches.dat");
			}
			else
			{
				gamePath = string.Empty;
			}

			installed = File.Exists(datadllorigpath);
			UpdateBtnInstallLoader_Text();
			loaderini = File.Exists(loaderinipath) ? IniSerializer.Deserialize<SADXLoaderInfo>(loaderinipath) : new SADXLoaderInfo();
		}

		private void Save()
		{
			if (!Directory.Exists(modDirectory))
				return;

			loaderini.Mods.Clear();

			//save mod list here
			foreach (ModData mod in listMods.Items)
			{
				if (mod is not null)
				{
					if (mod.IsChecked)
					{
						loaderini.Mods.Add(mod.Tag);
					}
				}
			}

			Properties.Settings.Default.GamePath = gamePath;
			loaderini.DebugConsole = (bool)checkEnableLogConsole.IsChecked;
			loaderini.HorizontalResolution = (int)txtResX.Value;
			loaderini.VerticalResolution = (int)txtResY.Value;
			loaderini.ForceAspectRatio = (bool)chkRatio.IsChecked;

			loaderini.BackgroundFillMode = comboBGFill.SelectedIndex;
			loaderini.FmvFillMode = comboFMVFill.SelectedIndex;
			loaderini.EnableVsync = (bool)chkVSync.IsChecked;
			loaderini.ScaleHud = (bool)checkUIScale.IsChecked;
			loaderini.Borderless = (bool)chkBorderless.IsChecked;
			loaderini.AutoMipmap = (bool)checkMipmapping.IsChecked;
			//loaderini.TextureFilter = (bool)comboTextureFilter.IsChecked;

			loaderini.StretchFullscreen = (bool)chkScaleScreen.IsChecked;
			loaderini.ScreenNum = comboScreen.SelectedIndex;
			loaderini.CustomWindowSize = (bool)chkCustomWinSize.IsChecked;
			loaderini.WindowWidth = (int)txtCustomResX.Value;
			loaderini.WindowHeight = (int)txtCustomResY.Value;
			loaderini.MaintainWindowAspectRatio = (bool)chkMaintainRatio.IsChecked;
			loaderini.EnableDynamicBuffer = (bool)chkDynamicBuffers.IsChecked;
			loaderini.ResizableWindow = (bool)chkResizableWin.IsChecked;
			loaderini.UpdateCheck = (bool)chkUpdatesML.IsChecked;
			loaderini.ModUpdateCheck = (bool)chkUpdatesMods.IsChecked;
			loaderini.Language = comboLanguage.SelectedIndex;
			loaderini.Theme = comboThemes.SelectedIndex;
			loaderini.EnableTestSpawnTab = (bool)checkEnableTestSpawn.IsChecked;
			loaderini.InputModEnabled = (bool)radBetterInput.IsChecked;
			loaderini.SEVolume = (int)sliderSFX.Value;
			loaderini.EnableBassMusic = (bool)checkBassMusic.IsChecked;
			loaderini.EnableBassSFX = (bool)checkBassSFX.IsChecked;

			IniSerializer.Serialize(loaderini, loaderinipath);

			Refresh();
		}
		private void LoadSettings()
		{
			comboLanguage.SelectedIndex = loaderini.Language;
			comboThemes.SelectedIndex = loaderini.Theme;
			textGameDir.Text = gamePath;

			checkEnableLogConsole.IsChecked = loaderini.DebugConsole;

			chkVSync.IsChecked = loaderini.EnableVsync;
			txtResX.IsEnabled = !loaderini.ForceAspectRatio;

			int resXMin = (int)txtResX.Minimum;
			int resXMax = (int)txtResX.Maximum;

			int resYMin = (int)txtResY.Minimum;
			int resYMax = (int)txtResY.Maximum;

			txtResX.Value = Math.Max(resXMin, Math.Min(resXMax, loaderini.HorizontalResolution));
			txtResY.Value = Math.Max(resYMin, Math.Min(resYMax, loaderini.VerticalResolution));
			chkUpdatesML.IsChecked = loaderini.UpdateCheck;
			chkUpdatesMods.IsChecked = loaderini.ModUpdateCheck;

			suppressEvent = true;
			chkRatio.IsChecked = loaderini.ForceAspectRatio;
			checkUIScale.IsChecked = loaderini.ScaleHud;
			suppressEvent = false;

			comboBGFill.SelectedIndex = loaderini.BackgroundFillMode;
			comboFMVFill.SelectedIndex = loaderini.FmvFillMode;

			chkDynamicBuffers.IsChecked = loaderini.EnableDynamicBuffer;
			chkBorderless.IsChecked = loaderini.Borderless;
			checkMipmapping.IsChecked = loaderini.AutoMipmap;
			chkScaleScreen.IsChecked = loaderini.StretchFullscreen;

			int screenNum = graphics.GetScreenNum(loaderini.ScreenNum);

			comboScreen.SelectedIndex = screenNum;
			chkBorderless.IsChecked = txtCustomResY.IsEnabled = chkMaintainRatio.IsEnabled = loaderini.CustomWindowSize;
			txtCustomResX.IsEnabled = loaderini.CustomWindowSize && !loaderini.MaintainWindowAspectRatio;
			System.Drawing.Rectangle rect = graphics.GetRectangleStruct();

			int CustresXMax = (int)txtCustomResX.Maximum;
			int CustresXMin = (int)txtCustomResX.Minimum;
			int CustresYMin = (int)txtCustomResY.Minimum;
			int CustresYMax = (int)txtCustomResY.Maximum;
			txtCustomResX.Maximum = rect.Width;
			txtCustomResX.Value = Math.Max(CustresXMin, Math.Min(CustresXMax, loaderini.WindowWidth));
			txtCustomResY.Value = Math.Max(CustresYMin, Math.Min(CustresYMax, loaderini.WindowHeight));
			txtCustomResY.Maximum = rect.Height;

			suppressEvent = true;
			chkBorderless.IsChecked = loaderini.MaintainWindowAspectRatio;
			suppressEvent = false;

			chkResizableWin.IsChecked = loaderini.ResizableWindow;

			checkEnableTestSpawn.IsChecked = loaderini.EnableTestSpawnTab;
			radBetterInput.IsChecked = loaderini.InputModEnabled;
			radVanillaInput.IsChecked = !radBetterInput.IsChecked;

			checkBassMusic.IsChecked = loaderini.EnableBassMusic;
			checkBassSFX.IsChecked = loaderini.EnableBassSFX;
			sliderSFX.Value = loaderini.SEVolume;

			if ((bool)!checkEnableTestSpawn.IsChecked)
			{
				tcMain.Items.Remove(tabTestSpawn);
			}
		}

		private void SaveButton_Click(object sender, RoutedEventArgs e)
		{
			Save();
			SaveGameConfigIni();
		}

		private void SaveAndPlayButton_Click(object sender, RoutedEventArgs e)
		{
			Save();
			SaveGameConfigIni();
			if (string.IsNullOrEmpty(gamePath))
			{
				MessageBox.Show(Lang.GetString("FailedDetectGamePath"), Lang.GetString("FailedDetectGamePathTitle"), MessageBoxButton.OK, MessageBoxImage.Error);
				return;
			}

			Process.Start(new ProcessStartInfo(Path.Combine(gamePath, "sonic.exe"))
			{
				WorkingDirectory = gamePath
			});

			if ((bool)!checkManagerOpen.IsChecked)
				Close();
		}

		private void LoadModList()
		{
			btnMoveTop.IsEnabled = btnMoveUp.IsEnabled = btnMoveDown.IsEnabled = btnMoveBottom.IsEnabled = ConfigureModBtn.IsEnabled = false;
			listMods.Items.Clear();
			mods = new Dictionary<string, SADXModInfo>();

			bool modFolderExist = Directory.Exists(modDirectory);

			//if mod folder doesn't exist and game path hasn't been set, give up the process of loading mods.

			if (!modFolderExist && string.IsNullOrEmpty(gamePath))
			{
				return;
			}
			else if (Directory.Exists(gamePath) && !modFolderExist)
			{
				Directory.CreateDirectory(modDirectory);
			}

			if (File.Exists(Path.Combine(modDirectory, "mod.ini")))
			{
				MessageBox.Show(this, Lang.GetString("ModIniError0")
							+ Lang.GetString("ModIniError1") +
							Lang.GetString("ModIniError2"), Lang.GetString("SadxManagerError"), MessageBoxButton.OK, MessageBoxImage.Error);
				Close();
				return;
			}

			//browse the mods folder and get each mod name by their ini file
			foreach (string filename in SADXModInfo.GetModFiles(new DirectoryInfo(modDirectory)))
			{
				SADXModInfo mod = IniSerializer.Deserialize<SADXModInfo>(filename);
				mods.Add((Path.GetDirectoryName(filename) ?? string.Empty).Substring(modDirectory.Length + 1), mod);
			}

			foreach (string mod in new List<string>(loaderini.Mods))
			{
				if (mods.ContainsKey(mod))
				{
					SADXModInfo inf = mods[mod];
					suppressEvent = true;
					var item = new ModData()
					{
						Name = inf.Name,
						Author = inf.Author,
						AuthorURL = inf.AuthorURL,
						Description = inf.Description,
						Version = inf.Version,
						Category = inf.Category,
						SourceCode = inf.SourceCode,
						IsChecked = true,
						Tag = mod
					};
					listMods.Items.Add(item);
					suppressEvent = false;
				}
				else
				{
					MessageBox.Show(this, "Mod \"" + mod + "\"" + Lang.GetString("ModRemovedList"));
					loaderini.Mods.Remove(mod);
				}
			}

			foreach (KeyValuePair<string, SADXModInfo> inf in mods.OrderBy(x => x.Value.Name))
			{
				if (!loaderini.Mods.Contains(inf.Key))
				{
					var item = new ModData()
					{
						Name = inf.Value.Name,
						Author = inf.Value.Author,
						AuthorURL = inf.Value.AuthorURL,
						Description = inf.Value.Description,
						Version = inf.Value.Version,
						Category = inf.Value.Category,
						SourceCode = inf.Value.SourceCode,
						IsChecked = false,
						Tag = inf.Key,
					};

					listMods.Items.Add(item);
				}
			}

			ConfigureModBtn_UpdateOpacity();
		}

		private void OpenAboutModWindow(ModData mod)
		{
			new AboutMod(mod).ShowDialog();
		}

		private ModData GetModFromView(object sender)
		{
			if (sender is ListViewItem lvItem)
				return lvItem.Content as ModData;
			else if (sender is ListView lv)
				return lv.SelectedItem as ModData;

			return null;
		}

		private void ModsView_Item_MouseDoubleClick(object sender, MouseButtonEventArgs e)
		{
			var mod = GetModFromView(sender);

			if (mod == null)
				return;

			OpenAboutModWindow((ModData)mod);
		}

		private void ModList_MouseEnter(object sender, MouseEventArgs e)
		{
			var mod = GetModFromView(sender);

			if (mod == null)
				return;

			textModsDescription.Text = Lang.GetString("ModSelectTextDesc") + " " + mods[mod.Tag].Description;
		}

		private void ModList_MouseLeave(object sender, MouseEventArgs e)
		{
			var item = GetModFromView(sender);

			if (item is not null)
			{
				textModsDescription.Text = Lang.GetString("ModSelectTextDesc") + " " + item.Description;
				return;
			}
			
			textModsDescription.Text = Lang.GetString("ModTextDesc");
		}

	#region ModContext
		private void ModContextOpenFolder_Click(object sender, RoutedEventArgs e)
		{
			var item = (ModData)listMods.SelectedItem;

			if (item is not null)
			{
				string fullPath = Path.Combine(modDirectory, item.Tag);
				var psi = new ProcessStartInfo() { FileName = fullPath, UseShellExecute = true };
				Process.Start(psi);
			}
		}

		private void ModContextChkUpdate_Click(object sender, RoutedEventArgs e)
		{

		}

		private void ModContextVerifyIntegrity_Click(object sender, RoutedEventArgs e)
		{

		}

		private void ModContextForceUpdate_Click(object sender, RoutedEventArgs e)
		{

		}

		private void ModContextEditMod_Click(object sender, RoutedEventArgs e)
		{
			var mod = (ModData)listMods.SelectedItem;

			if (mod == null)
				return;

			SADXModInfo modInfo = mods[mod.Tag];
			EditMod Edit = new(modInfo);
			Edit.Show();
			Edit.Closed += EditMod_FormClosing;
		}

		private void ModContextDeleteMod_Click(object sender, RoutedEventArgs e)
		{
			var item = (ModData)listMods.SelectedItem;

			if (item is not null)
			{
				var msg = MessageBox.Show(Lang.GetString("DeleteModWarning") + " " + item.Name + "?", Lang.GetString("SadxManagerTitle"), MessageBoxButton.YesNoCancel, MessageBoxImage.Warning);
				if (msg == MessageBoxResult.Yes)
				{
					string fullPath = Path.Combine(modDirectory, item.Tag);

					if (Directory.Exists(fullPath))
					{
						Directory.Delete(fullPath, true);
					}

					LoadModList();
				}
			}
		}

		private void ModContextDev_Click(object sender, RoutedEventArgs e)
		{

		}
		#endregion

		private void InitModConfig()
		{
			var mod = (ModData)listMods.SelectedItem;

			if (mod is not null)
			{
				SADXModInfo modInfo = mods[mod.Tag];
				string fullPath = Path.Combine(modDirectory, mod.Tag);
				Common.ModConfig config = new(modInfo, fullPath);
				config.Show();
			}
		}
		private void EditMod_FormClosing(object sender, EventArgs e)
		{
			Refresh();
		}

		private void NewModBtn_Click(object sender, RoutedEventArgs e)
		{
			EditMod Edit = new(null);
			Edit.Show();
			Edit.Closed += EditMod_FormClosing;
		}

		private void ConfigureModBtn_Click(object sender, RoutedEventArgs e)
		{
			InitModConfig();
		}

		private void ModContextConfigureMod_Click(object sender, RoutedEventArgs e)
		{
			InitModConfig();
		}

		private void ConfigureModBtn_UpdateOpacity()
		{
			//get the config icon image, check if it's not null, then change its oppacity depending if the button is enabled or not.
			Image iconConfig = FindName("configIcon") as Image;
			iconConfig?.SetValue(Image.OpacityProperty, ConfigureModBtn.IsEnabled ? 1 : 0.4);
		}

		private void modListView_SelectedIndexChanged(object sender, SelectionChangedEventArgs e)
		{
			int count = listMods.SelectedItems.Count;

			if (count == 0)
			{
				btnMoveTop.IsEnabled = btnMoveUp.IsEnabled = btnMoveDown.IsEnabled = btnMoveBottom.IsEnabled = ConfigureModBtn.IsEnabled = false;
				//.Text = "Description: No mod selected.";
			}
			else if (count == 1)
			{
				var mod = GetModFromView(sender);

				btnMoveTop.IsEnabled = listMods.Items.IndexOf(mod) != 0;
				btnMoveUp.IsEnabled = listMods.Items.IndexOf(mod) > 0;
				btnMoveDown.IsEnabled = listMods.Items.IndexOf(mod) < listMods.Items.Count - 1;
				btnMoveBottom.IsEnabled = listMods.Items.IndexOf(mod) != listMods.Items.Count - 1;

				ConfigureModBtn.IsEnabled = File.Exists(Path.Combine(modDirectory, mod.Tag, "configschema.xml"));
				ConfigureModBtn_UpdateOpacity();
			}
			else if (count > 1)
			{
				//modDescription.Text = "Description: Multiple mods selected.";
				btnMoveTop.IsEnabled = btnMoveUp.IsEnabled = btnMoveDown.IsEnabled = btnMoveBottom.IsEnabled = true;

				ConfigureModBtn.IsEnabled = false;
			}
		}

		#endregion

		#region Game Settings

		private void LoadGameConfigIni()
		{
			gameConfigFile = File.Exists(sadxIni) ? IniSerializer.Deserialize<Game.GameConfigFile>(sadxIni) : new Game.GameConfigFile();
			
			if (gameConfigFile.GameConfig == null)
			{
				gameConfigFile.GameConfig = new Game.GameConfig
				{
					FrameRate = (int)Game.FrameRate.High,
					Sound3D = 1,
					SEVoice = 1,
					BGM = 1,
					BGMVolume = 100,
					VoiceVolume = 100
				};
			}

			if (gameConfigFile.Controllers == null)
				gameConfigFile.Controllers = new Dictionary<string, Game.ControllerConfig>();

			// Video
			// Display mode
			if (gameConfigFile.GameConfig.FullScreen == 1)
				radFullscreen.IsChecked = true;
			else
				radWindowed.IsChecked = true;

			// Framerate
			if (gameConfigFile.GameConfig.FrameRate == (int)Game.FrameRate.Invalid || gameConfigFile.GameConfig.FrameRate > (int)Game.FrameRate.Low)
			{
				MessageBox.Show("Invalid framerate setting detected.\nDefaulting to \"High\".", "Invalid setting", MessageBoxButton.OK, MessageBoxImage.Error);
				comboFramerate.SelectedIndex = (int)Game.FrameRate.High - 1;
			}
			else
			{
				comboFramerate.SelectedIndex = gameConfigFile.GameConfig.FrameRate - 1;
			}

			// Clip level
			comboDetail.SelectedIndex = (int)gameConfigFile.GameConfig.ClipLevel;

			// Fog mode
			comboFog.SelectedIndex = gameConfigFile.GameConfig.FogEmulation;

			sliderMusic.Value = gameConfigFile.GameConfig.BGMVolume;
			sliderVoice.Value = gameConfigFile.GameConfig.VoiceVolume;

			checkEnableMusic.IsChecked = gameConfigFile.GameConfig.BGM > 0;
			checkEnableSounds.IsChecked = gameConfigFile.GameConfig.SEVoice > 0;
			checkEnable3DSound.IsChecked = gameConfigFile.GameConfig.Sound3D > 0;

			//controller mouse vanilla stuff go here

		}

		private void SaveGameConfigIni()
		{
			if (!File.Exists(sadxIni))
				return;

			gameConfigFile.GameConfig.FullScreen = (bool)radFullscreen.IsChecked ? 1 : 0;

			gameConfigFile.GameConfig.FrameRate = comboFramerate.SelectedIndex + 1;
			//gameConfigFile.GameConfig.ClipLevel = comboClip.SelectedIndex;
			gameConfigFile.GameConfig.FogEmulation = comboFog.SelectedIndex;

			gameConfigFile.GameConfig.Sound3D = (bool)checkEnable3DSound.IsChecked ? 1 : 0;
			gameConfigFile.GameConfig.SEVoice = (bool)checkEnableSounds.IsChecked ? 1 : 0;
			gameConfigFile.GameConfig.BGM = (bool)checkEnableMusic.IsChecked ? 1 : 0;

			gameConfigFile.GameConfig.VoiceVolume = (int)sliderVoice.Value;
			gameConfigFile.GameConfig.BGMVolume = (int)sliderMusic.Value;

			/*gameConfigFile.GameConfig.MouseMode = radioMouseModeHold.Checked ? 0 : 1;

			if (inputDevice != null)
			{
				gameConfigFile.GameConfig.PadConfig = controllerConfigSelect.SelectedIndex == -1 ? null : controllerConfig[controllerConfigSelect.SelectedIndex].Name;

				gameConfigFile.Controllers.Clear();
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
					gameConfigFile.Controllers.Add(item.Name, config);
				}
			}*/

			IniSerializer.Serialize(gameConfigFile, sadxIni);
		}

		#endregion

		#region Cheat Codes
		private void InitCodes()
		{
			try
			{
				if (File.Exists(codelstpath))
					mainCodes = CodeList.Load(codelstpath);
				else if (File.Exists(codexmlpath))
					mainCodes = CodeList.Load(codexmlpath);
				else
					mainCodes = new CodeList();

				LoadCodes();
			}
			catch (Exception ex)
			{
				MessageBox.Show(this, $"Error loading code list: {ex.Message}", "Error Loading Code", MessageBoxButton.OK, MessageBoxImage.Error);

				mainCodes = new CodeList();
			}
		}

		private void LoadCodes()
		{
			codes = new List<Code>(mainCodes.Codes);

			foreach (Code item in codes)
			{
				CodeListView.Items.Add(item);
			}

			loaderini.EnabledCodes = new List<string>(loaderini.EnabledCodes.Where(a => codes.Any(c => c.Name == a)));
		}

		private void OpenAboutCodeWindow(Code code)
		{
			new AboutCode(code).ShowDialog();
		}

		private Code GetCodeFromView(object sender)
		{
			if (sender is ListViewItem lvItem)
				return lvItem.Content as Code;
			else if (sender is ListView lv)
				return lv.SelectedItem as Code;

			return codes[CodeListView.SelectedIndex];
		}
		private void CodesView_Item_MouseDoubleClick(object sender, MouseButtonEventArgs e)
		{
			var code = GetCodeFromView(sender);

			if (code == null)
				return;

			OpenAboutCodeWindow(code);
		}

		private void CodesView_Item_MouseEnter(object sender, MouseEventArgs e)
		{
			var code = GetCodeFromView(sender);

			if (code == null)
				return;

			CodeAuthorGrid.Text += " " + code.Author;
			CodeDescGrid.Text += " " + code.Description;
			CodeCategoryGrid.Text += " " + code.Category;
		}

		private void CodesView_Item_MouseLeave(object sender, MouseEventArgs e)
		{
			CodeAuthorGrid.Text = Lang.GetString("ModsListAuthor");
			CodeDescGrid.Text = Lang.GetString("ModSelectTextDesc");
			CodeCategoryGrid.Text = Lang.GetString("CodesListCategory");
		}

		private void CodesView_Item_Selected(object sender, RoutedEventArgs e)
		{

		}

		#endregion

		#region Languages

		private void comboLanguage_Loaded(object sender, RoutedEventArgs e)
		{
			comboLanguage.GetBindingExpression(ComboBox.ItemsSourceProperty).UpdateTarget();
			comboLanguage.GetBindingExpression(ComboBox.SelectedItemProperty).UpdateTarget();
		}
		private void comboLanguage_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			App.SwitchLanguage();
			UpdateBtnInstallLoader_Text();
		}

		#endregion

		#region Themes

		private void comboThemes_Loaded(object sender, RoutedEventArgs e)
		{
			comboThemes.GetBindingExpression(ComboBox.ItemsSourceProperty).UpdateTarget();
			comboThemes.GetBindingExpression(ComboBox.SelectedItemProperty).UpdateTarget();
		}

		private void comboThemes_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			App.SwitchTheme();
		}

		#endregion

		#region Graphics Settings

		private void comboResolutionPreset_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (comboDisplay.SelectedIndex == -1)
				return;

			suppressEvent = true;

			txtResY.Text = graphics.resolutionPresets[comboDisplay.SelectedIndex].Height.ToString();

			if (chkRatio.IsChecked == false)
				txtResX.Text = graphics.resolutionPresets[comboDisplay.SelectedIndex].Width.ToString();

			suppressEvent = false;
		}

		private void comboScreen_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			if (graphics is null)
				return;

			graphics.screenNumBox_SelectChanged(comboScreen, comboDisplay);
		}

		private void chkCustomWinSize_Checked(object sender, RoutedEventArgs e)
		{
			chkMaintainRatio.IsEnabled = (bool)chkCustomWinSize.IsChecked;
			chkResizableWin.IsEnabled = (bool)!chkCustomWinSize.IsChecked;

			txtCustomResX.IsEnabled = (bool)chkCustomWinSize.IsChecked && (bool)!chkMaintainRatio.IsChecked;
			txtCustomResY.IsEnabled = (bool)chkCustomWinSize.IsChecked;
		}

		private void radFullscreen_Checked(object sender, RoutedEventArgs e)
		{
			
		}

		private void radWindowed_Checked(object sender, RoutedEventArgs e)
		{
			
		}

		#endregion

		#region Audio Settings

		private void sliderMusic_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
		{
			labelMusicLevel?.SetValue(ContentProperty, $"{(int)sliderMusic.Value}");
		}

		private void sliderVoice_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
		{
			labelVoiceLevel?.SetValue(ContentProperty, $"{(int)sliderVoice.Value}");
		}

		private void sliderSFX_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
		{
			labelSFXLevel?.SetValue(ContentProperty, $"{(int)sliderSFX.Value}");
		}

		#endregion

		#region TestSpawn Settings
		private void checkEnableTestSpawn_Checked(object sender, RoutedEventArgs e)
		{
			if (tcMain.Items.Contains(tabTestSpawn))
				return;

			int index = tcMain.Items.IndexOf(tabConfig);
			tcMain.Items.Insert(index, tabTestSpawn);
		}

		private void checkEnableTestSpawn_Unchecked(object sender, RoutedEventArgs e)
		{
			if (!tcMain.Items.Contains(tabTestSpawn))
				return;

			tcMain.Items.Remove(tabTestSpawn);
		}
		#endregion

		#region Inputs

		private void radVanillaInput_Checked(object sender, RoutedEventArgs e)
		{
			if (grpVanillaInput is null)
				return;

			int index = tabInputGrid.Children.IndexOf(grpSDLInput); //Graphic Window setting is a children of the graphic grid 

			tabInputGrid.Children.RemoveAt(index); //we remove it so we can only display the full screen options

			if (!tabInputGrid.Children.Contains(grpVanillaInput)) //if the fullscreen grid doesn't exist, add it back
			{
				tabInputGrid.Children.Add(grpVanillaInput);
			}
		}

		private void radBetterInput_Checked(object sender, RoutedEventArgs e)
		{
			if (grpSDLInput is null)
				return;

			int index = tabInputGrid.Children.IndexOf(grpVanillaInput);
			tabInputGrid.Children.RemoveAt(index);

			if (!tabInputGrid.Children.Contains(grpSDLInput))
			{
				tabInputGrid.Children.Add(grpSDLInput);
			}
		}

		#endregion

		#region Others

		private void NumberValidationTextBox(object sender, TextCompositionEventArgs e)
		{
			Regex regex = new("[^0-9]+");
			e.Handled = regex.IsMatch(e.Text);
		}

		private void AboutBtn_Click(object sender, RoutedEventArgs e)
		{
			new AboutManager().ShowDialog();
		}

		private void btnMoveTop_Click(object sender, RoutedEventArgs e)
		{
			if (listMods.SelectedItems.Count < 1)
				return;

			int index = listMods.SelectedIndex;

			if (index > 0)
			{
				var item = listMods.Items.GetItemAt(index);
				listMods.Items.Remove(item);
				listMods.Items.Insert(0, item);
			}
		}

		private void btnMoveUp_Click(object sender, RoutedEventArgs e)
		{
			if (listMods.SelectedItems.Count < 1)
				return;

			int index = listMods.SelectedIndex;

			if (index > 0)
			{
				var item = listMods.Items.GetItemAt(index);
				listMods.Items.Remove(item);
				listMods.Items.Insert(index - 1, item);
			}
		}

		private void btnMoveBottom_Click(object sender, RoutedEventArgs e)
		{
			if (listMods.SelectedItems.Count < 1)
				return;

			int index = listMods.SelectedIndex;

			if (index != listMods.Items.Count - 1)
			{
				var item = listMods.Items.GetItemAt(index);
				listMods.Items.Remove(item);
				listMods.Items.Insert(listMods.Items.Count, item);
			}
		}

		private void btnMoveDown_Click(object sender, RoutedEventArgs e)
		{
			if (listMods.SelectedItems.Count < 1)
				return;

			int index = listMods.SelectedIndex;

			if (index < listMods.Items.Count)
			{
				var item = listMods.Items.GetItemAt(index);
				listMods.Items.Remove(item);
				listMods.Items.Insert(index + 1, item);
			}
		}

		private void Refresh()
		{
			LoadModList();
			InitCodes();
		}

		private void RefreshBtn_Click(object sender, RoutedEventArgs e)
		{
			Refresh();
		}

		//set new game Path
		private void btnBrowseGameDir_Click(object sender, RoutedEventArgs e)
		{
			var dialog = new System.Windows.Forms.FolderBrowserDialog();

			System.Windows.Forms.DialogResult result = dialog.ShowDialog();

			if (result == System.Windows.Forms.DialogResult.OK)
			{
				string GamePath = dialog.SelectedPath;
				string path = Path.Combine(GamePath, exeName);

				if (File.Exists(path))
				{
					textGameDir.Text = GamePath;
					SetGamePath(GamePath);
					Properties.Settings.Default.GamePath = gamePath;
					UpdatePathsStringsInfo();
					Refresh();
				}
				else
				{
					MessageBox.Show(Lang.GetString("FaildSetGamePath"), Lang.GetString("FaildSetGamePathTitle"), MessageBoxButton.OK, MessageBoxImage.Error);
				}
			}
		}

		private void UpdateBtnInstallLoader_Text()
		{
			if (btnInstallLoader is null)
				return;

			if (installed)
			{
				btnInstallLoader.Content = Lang.GetString("ManagerBtnUninstallLoader");
			}
			else
			{
				btnInstallLoader.Content = Lang.GetString("ManagerBtnInstallLoader");
			}
		}

		private void btnInstallLoader_Click(object sender, RoutedEventArgs e)
		{
			if (string.IsNullOrEmpty(gamePath) || !File.Exists(Path.Combine(gamePath, exeName)))
			{
				MessageBox.Show(Lang.GetString("FailedDetectGamePath"), Lang.GetString("FailedDetectGamePathTitle"), MessageBoxButton.OK, MessageBoxImage.Error);
				return;
			}

			if (installed)
			{
				File.Delete(datadllpath);
				Util.MoveFile(datadllorigpath, datadllpath);
			}
			else
			{
				Util.MoveFile(datadllpath, datadllorigpath);
				if (File.Exists(loaderdllpath))
					File.Copy(loaderdllpath, datadllpath);
			}

			installed = !installed;
			UpdateBtnInstallLoader_Text();
		}

		private void btnSource_Click(object sender, RoutedEventArgs e)
		{
			var ps = new ProcessStartInfo("https://github.com/X-Hax/sadx-mod-loader")
			{
				UseShellExecute = true,
				Verb = "open"
			};
			Process.Start(ps);
		}

		private void btnReport_Click(object sender, RoutedEventArgs e)
		{
			var ps = new ProcessStartInfo("https://github.com/X-Hax/sadx-mod-loader/issues/new")
			{
				UseShellExecute = true,
				Verb = "open"
			};

			Process.Start(ps);	
		}

		private void ModList_ContextMenuOpening(object sender, ContextMenuEventArgs e)
		{
			if (sender is ListViewItem listViewItem)
			{
				var contextMenu = listViewItem.ContextMenu;

				if (contextMenu != null)
				{
					var menuItem = contextMenu.Items
						.OfType<MenuItem>()
						.FirstOrDefault(item => item.Name == "ModContextConfigureMod");

					menuItem?.SetValue(MenuItem.IsEnabledProperty, ConfigureModBtn.IsEnabled);
				}
			}
		}

		#endregion
	}
}
