using ModManagerWPF.Languages;
using ModManagerWPF.Themes;
using System;
using System.Linq;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using ModManagerCommon;
using System.Collections.Generic;
using System.IO;
using System.Windows.Input;
using System.Text.RegularExpressions;
using IniFile;
using MessageBox = System.Windows.MessageBox;
using System.Windows.Threading;
using System.Diagnostics;
using ModManagerWPF.Properties;
using System.Windows.Media;
using System.Threading.Tasks;
using ModManagerWPF.Common;
using static ModManagerWPF.MainWindow;
using System.ComponentModel;
using System.Windows.Data;
using System.Security.Cryptography;
using System.Threading;
using ModManagerCommon.Forms;
using System.Net;

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
		public static string updatePath = "mods/.updates";
		public static string loaderinipath = "mods/SADXModLoader.ini";
		private string sadxIni = "sonicDX.ini";
		string datadllorigpath = "system/CHRMODELS_orig.dll";
		string loaderdllpath = "mods/SADXModLoader.dll";
		public string datadllpath = "system/CHRMODELS.dll";
		const string exeName = "sonic.exe";

		string gamePath = string.Empty;
		SADXLoaderInfo loaderini;
		public Dictionary<string, SADXModInfo> mods = null;
		string codelstpath = "mods/Codes.lst";
		string codexmlpath = "mods/Codes.xml";
		string codedatpath = "mods/Codes.dat";
		string patchdatpath = "mods/Patches.dat";
		CodeList mainCodes = null;
		List<Code> codes = null;
		bool installed = false;
		bool suppressEvent = false;
		BackgroundWorker updateChecker;
		private bool manualModUpdate;
		readonly ModUpdater modUpdater = new ModUpdater();
		private bool checkedForUpdates = false;

		private readonly double LowOpacityIcon = 0.3;
		private readonly double LowOpacityBtn = 0.7;


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

		public class CodeData
		{
			public Code codes { get; set; }
			public bool IsChecked { get; set; }
		}

		public class PatchesData
		{
			public string Name { get; set; }
			public bool IsChecked { get; set; }
		}

		#endregion

		public MainWindow(string[] args = null)
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
			LoadAllProfiles();
			UpdateDLLData();
			UpdatePatches();
			setupTestSpawn();

			if (args is not null)
				CleanUpdate(args);
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
		private void CleanUpdate(string[] args)
		{
			try
			{
				File.Delete(args[1] + ".7z");
				Directory.Delete(args[1], true);

				if (File.Exists(datadllorigpath))
				{
					using (MD5 md5 = MD5.Create())
					{
						byte[] hash1 = md5.ComputeHash(File.ReadAllBytes(loaderdllpath));
						byte[] hash2 = md5.ComputeHash(File.ReadAllBytes(datadllpath));

						if (!hash1.SequenceEqual(hash2))
						{
							File.Copy(loaderdllpath, datadllpath, true);
						}
					}
				}
			}
			catch { }
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
			DownloadMod dl = new(updatePath, Path.Combine(gamePath, "mods"));
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
				updatePath = Path.Combine(gamePath, "mods/.updates");
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
			UpdateBtnInstallLoader_State();
			loaderini = File.Exists(loaderinipath) ? IniSerializer.Deserialize<SADXLoaderInfo>(loaderinipath) : new SADXLoaderInfo();
			Update_PlayButtonsState();
		}

		public async void Save()
		{
			if (!Directory.Exists(modDirectory))
				return;

			loaderini.Mods.Clear();

			//save mod list here
			foreach (ModData mod in listMods.Items)
			{
				if (mod?.IsChecked == true)
				{
					loaderini.Mods.Add(mod.Tag);
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

			//test spaw nstuff
			loaderini.TestSpawnCharacter = (bool)tsCheckCharacter.IsChecked ? tsComboCharacter.SelectedIndex : -1;

			loaderini.TestSpawnLevel = (bool)tsCheckLevel.IsChecked ? tsComboLevel.SelectedIndex : -1;
			loaderini.TestSpawnAct = tsComboAct.SelectedIndex;

			loaderini.TestSpawnGameMode = (bool)tsCheckGameMode.IsChecked ? tsComboGameMode.SelectedIndex : -1;

			loaderini.TestSpawnEvent = (bool)tsCheckEvent.IsChecked ? tsComboEvent.SelectedIndex : -1;

			loaderini.TestSpawnPositionEnabled = (bool)tsCheckPosition.IsChecked;
			loaderini.TestSpawnX = (int)tsNumPosX.Value;
			loaderini.TestSpawnY = (int)tsNumPosY.Value;
			loaderini.TestSpawnZ = (int)tsNumPosZ.Value;

			SavePatches();
			SaveCodes();
	
			IniSerializer.Serialize(loaderini, loaderinipath);

			SaveGameConfigIni();
			await Task.Delay(200);

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
			sliderSFX.IsEnabled = checkBassSFX.IsChecked.Value;

			tsCheckCharacter.IsChecked = loaderini.TestSpawnCharacter > -1;
			tsComboCharacter.SelectedIndex = loaderini.TestSpawnCharacter;
			tsCheckLevel.IsChecked = loaderini.TestSpawnLevel > -1;
			tsComboLevel.SelectedIndex = loaderini.TestSpawnLevel;
			
			tsComboAct.SelectedIndex = loaderini.TestSpawnAct;
			tsComboGameMode.SelectedIndex = loaderini.TestSpawnGameMode;
			tsCheckEvent.IsChecked = loaderini.TestSpawnEvent > -1;
			tsComboEvent.SelectedIndex = loaderini.TestSpawnEvent;
			tsCheckPosition.IsChecked = loaderini.TestSpawnPositionEnabled;
			tsNumPosX.Value = loaderini.TestSpawnX;
			tsNumPosY.Value = loaderini.TestSpawnY;
			tsNumPosZ.Value = loaderini.TestSpawnZ;

			if ((bool)!checkEnableTestSpawn.IsChecked)
			{
				tcMain.Items.Remove(tabTestSpawn);
			}

		}

		private void SaveButton_Click(object sender, RoutedEventArgs e)
		{
			Save();
		}

		private void StartGame()
		{
			if (string.IsNullOrEmpty(gamePath))
			{
				new MessageWindow(Lang.GetString("FailedDetectGamePathTitle"), Lang.GetString("FailedDetectGamePath"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
				return;
			}

			Process.Start(new ProcessStartInfo(Path.Combine(gamePath, "sonic.exe"))
			{
				WorkingDirectory = gamePath
			});

			if ((bool)!checkManagerOpen.IsChecked)
				Close();
		}

		private void SaveAndPlayButton_Click(object sender, RoutedEventArgs e)
		{
			Save();
			StartGame();
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
				UpdateMainButtonsState();
				return;
			}
			else if (Directory.Exists(gamePath) && !modFolderExist)
			{
				Directory.CreateDirectory(modDirectory);
			}

			if (File.Exists(Path.Combine(modDirectory, "mod.ini")))
			{
				new MessageWindow(Lang.GetString("SadxManagerError"), Lang.GetString("ModIniError0") + Lang.GetString("ModIniError1") +
							Lang.GetString("ModIniError2"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, 
							MessageWindow.Buttons.OK).ShowDialog();

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
					new MessageWindow(Lang.GetString("SadxManagerTitle"), "Mod \"" + mod + "\"" + Lang.GetString("ModRemovedList"), MessageWindow.WindowType.Message, MessageWindow.Icons.Information, MessageWindow.Buttons.OK).ShowDialog();
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

			ConfigureModBtn_UpdateState();
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
			if (!(GetModFromView(sender) is ModData mod))
				return;

			OpenAboutModWindow(mod);
		}

		private void ModList_MouseEnter(object sender, MouseEventArgs e)
		{
			if (!(GetModFromView(sender) is ModData mod))
				return;

			textModsDescription.Text = Lang.GetString("ModSelectTextDesc") + " " + mods[mod.Tag].Description;
		}

		private void ModList_MouseLeave(object sender, MouseEventArgs e)
		{
			var item = GetModFromView(sender);
			textModsDescription.Text = (item is not null) ? $"{Lang.GetString("ModSelectTextDesc")} {item.Description}" : Lang.GetString("ModTextDesc");
		}

		#region ModContext
		private void ModContextOpenFolder_Click(object sender, RoutedEventArgs e)
		{
			if (listMods.SelectedItem is ModData item)
			{
				string fullPath = Path.Combine(modDirectory, item.Tag);
				Process.Start(new ProcessStartInfo { FileName = fullPath, UseShellExecute = true });
			}
		}

		private void ModContextChkUpdate_Click(object sender, RoutedEventArgs e)
		{
			UpdateSelectedMods();
		}

		private void ModContextVerifyIntegrity_Click(object sender, RoutedEventArgs e)
		{
			List<Tuple<string, ModInfo>> items = listMods.SelectedItems.Cast<ListViewItem>()
				.Select(x => (string)x.Tag)
				.Where(x => File.Exists(Path.Combine("mods", x, "mod.manifest")))
				.Select(x => new Tuple<string, ModInfo>(x, mods[x]))
				.ToList();

			if (items.Count < 1)
			{
			
				return;
			}



		}

		private void ModContextForceUpdate_Click(object sender, RoutedEventArgs e)
		{
			var result = new MessageWindow(Lang.GetString("ForceUpdateTitle"), Lang.GetString("ForceUpdate"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);
			result.ShowDialog();
			if (result.isYes) 
			{
				modUpdater.ForceUpdate = true;
				UpdateSelectedMods();
			}
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
			var selectedItems = listMods.SelectedItems;
			var count = selectedItems.Count > 0;

			if (count)
			{
				var confirmMessage = Lang.GetString("DeleteModWarning");
				var deleteConfirmation = new MessageWindow(Lang.GetString("SadxManagerTitle"), confirmMessage, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);

				deleteConfirmation.ShowDialog();
				if (deleteConfirmation.isYes)
				{
					foreach (var selectedItem in selectedItems)
					{
						var item = (ModData)selectedItem;

						string fullPath = Path.Combine(modDirectory, item.Tag);

						if (Directory.Exists(fullPath))
						{
							Directory.Delete(fullPath, true);
						}
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
				Common.ModConfig config = new(modInfo.Name, fullPath);
				config.Show();
			}
		}
		private void EditMod_FormClosing(object sender, EventArgs e)
		{
			Refresh();
		}

		private void NewModBtn_Click(object sender, RoutedEventArgs e)
		{
			var form = new InstallModOptions();
			var choice = form.Ask();
			
			switch (choice)
			{
				case (int)InstallModOptions.Type.ModArchive:
					var archiveFile = new System.Windows.Forms.OpenFileDialog();
					archiveFile.Multiselect = true;

					archiveFile.Filter = "archive files|*.zip;*.7z;*.rar;*.tar";
					System.Windows.Forms.DialogResult result_ = archiveFile.ShowDialog();

					if (result_ == System.Windows.Forms.DialogResult.OK)
					{
						string[] sFileName = archiveFile.FileNames;
						form.InstallMod(sFileName, modDirectory);
				
					}
					break;
				case (int)InstallModOptions.Type.ModFolder:
					var newModFolder = new System.Windows.Forms.FolderBrowserDialog();

					System.Windows.Forms.DialogResult result = newModFolder.ShowDialog();

					if (result == System.Windows.Forms.DialogResult.OK)
					{
						string[] FileName = { newModFolder.SelectedPath };
	
						form.InstallMod(FileName, modDirectory);
					}
					break;
				case (int)InstallModOptions.Type.NewMod: //create mod
					EditMod Edit = new(null);
					Edit.Show();
					Edit.Closed += EditMod_FormClosing;
					
					break;
			}

		}

		private void ConfigureModBtn_Click(object sender, RoutedEventArgs e)
		{
			InitModConfig();
		}

		private void ModContextConfigureMod_Click(object sender, RoutedEventArgs e)
		{
			InitModConfig();
		}

		private void ConfigureModBtn_UpdateState()
		{
			//get the config icon image, check if it's not null, then change its oppacity depending if the button is enabled or not.
			Image iconConfig = FindName("configIcon") as Image;
			iconConfig?.SetValue(Image.OpacityProperty, ConfigureModBtn.IsEnabled ? 1 : LowOpacityIcon);
			ConfigureModBtn.Opacity = ConfigureModBtn.IsEnabled ? 1 : LowOpacityBtn;
		}

		private void AddModBtn_UpdateState()
		{
			Image iconConfig = FindName("newIcon") as Image;
			iconConfig?.SetValue(Image.OpacityProperty, installed ? 1 : LowOpacityIcon);
			NewModBtn.Opacity = installed ? 1 : LowOpacityBtn;
			NewModBtn.IsEnabled = installed;
		}

		private void Update_PlayButtonsState()
		{
			SaveAndPlayButton.IsEnabled = installed;
			Image iconSavePlay = FindName("savePlayIcon") as Image;
			iconSavePlay?.SetValue(Image.OpacityProperty, SaveAndPlayButton.IsEnabled ? 1 : LowOpacityIcon);
			SaveAndPlayButton.Opacity = installed ? 1 : LowOpacityBtn;
			btnTSLaunch.IsEnabled = installed;
			btnTSLaunch.Opacity = installed ? 1 : LowOpacityBtn;
		}

		private void UpdateMainButtonsState()
		{
			ConfigureModBtn_UpdateState();
			AddModBtn_UpdateState();
		}

		private void modListView_SelectedIndexChanged(object sender, SelectionChangedEventArgs e)
		{
			int count = listMods.SelectedItems.Count;

			if (count == 0)
			{
				btnMoveTop.IsEnabled = btnMoveUp.IsEnabled = btnMoveDown.IsEnabled = btnMoveBottom.IsEnabled = ConfigureModBtn.IsEnabled = false;
			}
			else if (count == 1)
			{
				var mod = GetModFromView(sender);

				btnMoveTop.IsEnabled = listMods.Items.IndexOf(mod) != 0;
				btnMoveUp.IsEnabled = listMods.Items.IndexOf(mod) > 0;
				btnMoveDown.IsEnabled = listMods.Items.IndexOf(mod) < listMods.Items.Count - 1;
				btnMoveBottom.IsEnabled = listMods.Items.IndexOf(mod) != listMods.Items.Count - 1;

				ConfigureModBtn.IsEnabled = File.Exists(Path.Combine(modDirectory, mod.Tag, "configschema.xml"));
				ConfigureModBtn_UpdateState();
			}
			else if (count > 1)
			{
				btnMoveTop.IsEnabled = btnMoveUp.IsEnabled = btnMoveDown.IsEnabled = btnMoveBottom.IsEnabled = true;
				ConfigureModBtn.IsEnabled = false;
			}
		}

		private async void btnSelectAll_Click(object sender, RoutedEventArgs e)
		{
			foreach (ModData mod in listMods.Items)
			{
				mod.IsChecked = true;
			}

			RefreshModList();
			btnSelectAll.IsEnabled = false;
			await Task.Delay(150);
			btnSelectAll.IsEnabled = true;
		}

		private async void btnDeselectAll_Click(object sender, RoutedEventArgs e)
		{
			foreach (ModData mod in listMods.Items)
			{
				mod.IsChecked = false;
			}

			RefreshModList();
			btnDeselectAll.IsEnabled = false;
			await Task.Delay(150);
			btnDeselectAll.IsEnabled = true;
		}

		private void RefreshModList()
		{
			ICollectionView view = CollectionViewSource.GetDefaultView(listMods.Items);
			view.Refresh();
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
				new MessageWindow(Lang.GetString("InvalidSettingFrameTitle"),Lang.GetString("InvalidSettingFrameTitle"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
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
			gameConfigFile.GameConfig.ClipLevel = comboDetail.SelectedIndex;
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
				string msg = " " + ex.Message;
				new MessageWindow(Lang.GetString("ErrorLoadingCodeTitle"), Lang.GetString("ErrorLoadingCode") + msg, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
				mainCodes = new CodeList();
			}
		}

		private void LoadCodes()
		{

			codes = new List<Code>(mainCodes.Codes);

			loaderini.EnabledCodes = new List<string>(loaderini.EnabledCodes.Where(a => codes.Any(c => c.Name == a)));

			CodeListView.BeginInit();
			CodeListView.Items.Clear();

			foreach (Code item in codes.Where(a => a.Required && !loaderini.EnabledCodes.Contains(a.Name)))
				loaderini.EnabledCodes.Add(item.Name);

			foreach (Code item in codes)
			{
				CodeData extraItem = new()
				{
					codes = item,
					IsChecked = loaderini.EnabledCodes.Contains(item.Name),
				};

				CodeListView.Items.Add(extraItem);
			}
			
			CodeListView.EndInit();
		}

		private void SaveCodes()
		{
			List<Code> selectedCodes = new List<Code>();
			List<Code> selectedPatches = new List<Code>();

			foreach (CodeData code in CodeListView.Items)
			{
				var Code = code.codes;

				if (code?.IsChecked == true)
				{
					
					if (Code.Patch)
						selectedPatches.Add(Code);
					else
						selectedCodes.Add(Code);

					if (!loaderini.EnabledCodes.Contains(Code.Name))
						loaderini.EnabledCodes.Add(Code.Name);
				}
				else
				{
					if (loaderini.EnabledCodes.Contains(Code.Name))
						loaderini.EnabledCodes.Remove(Code.Name);
				}
			}

			CodeList.WriteDatFile(patchdatpath, selectedPatches);
			CodeList.WriteDatFile(codedatpath, selectedCodes);

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
			UpdateBtnInstallLoader_State();
			FlowDirectionHelper.UpdateFlowDirection();
			UpdatePatches();
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
			graphics?.screenNumBox_SelectChanged(comboScreen, comboDisplay);
		}

		private void chkCustomWinSize_Checked(object sender, RoutedEventArgs e)
		{
			chkMaintainRatio.IsEnabled = chkCustomWinSize.IsChecked.GetValueOrDefault();
			chkResizableWin.IsEnabled = !chkCustomWinSize.IsChecked.GetValueOrDefault();

			txtCustomResX.IsEnabled = chkCustomWinSize.IsChecked.GetValueOrDefault() && !chkMaintainRatio.IsChecked.GetValueOrDefault();
			txtCustomResY.IsEnabled = chkCustomWinSize.IsChecked.GetValueOrDefault();
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
		private void checkBassSFX_Checked(object sender, RoutedEventArgs e)
		{
			sliderSFX.IsEnabled = true;
		}

		private void checkBassSFX_Unchecked(object sender, RoutedEventArgs e)
		{
			sliderSFX.IsEnabled = false;
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

		private void btnTestSpawnLaunchGame_Click(object sender, RoutedEventArgs e)
		{
			StartGame();
		}

		private void tsComboLevel_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			ComboBox t = sender as ComboBox;

			var list = TestSpawn.GetNewAct(t.SelectedIndex);

			tsComboAct.BeginInit();
			tsComboAct.Items.Clear();

			foreach (var item in list)
			{
				tsComboAct.Items.Add(item);
			}

			tsComboAct.EndInit();
			tsComboAct.SelectedIndex = 0;

		}

		private void TS_GetSave()
		{
			if (installed)
			{
				string fullPath = Path.Combine(gamePath, "SAVEDATA");

				//if savedata exists
				if (Directory.Exists(fullPath))
				{
					string targetExtension = ".snc"; 

					string[] files = Directory.GetFiles(fullPath, "*" + targetExtension, SearchOption.TopDirectoryOnly);

					tsComboSave.BeginInit();
					tsComboSave.Items.Clear();

					List<string> list = new List<string>();
					
					//browse each save file of the user
					foreach (string file in files)
					{
						string name = Path.GetFileNameWithoutExtension(file);
						string nameDup = name.ToLower();

						if (nameDup.Contains("sonicdx")) //skip chao garden save
							list.Add(name);
					}	

					//sort just in case the order is wrong
					list.Sort((x, y) => String.Compare(x, y, StringComparison.Ordinal));

					//finally, add all the saves in the comboBox
					foreach (string file in list)
					{
						tsComboSave.Items.Add(file);
					}

					tsComboSave.EndInit();
				}
			}		
		}

		private void setupTestSpawn()
		{
			var testSpawn = new TestSpawn(ref tsComboLevel, ref tsComboGameMode,  ref tsComboEvent, ref tsComboCharacter);

			testSpawn.InitCutsceneList();
			testSpawn.InitGameModeList();
			testSpawn.InitCharactersList();
			testSpawn.InitLevels();

			tsComboAct.BeginInit();
			tsComboAct.Items.Clear();

			var list = TestSpawn.GetNewAct(0);

			foreach (var item in list)
			{
				tsComboAct.Items.Add(item);
			}

			tsComboAct.EndInit();

			TS_GetSave();

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
			RefreshPatchesList();
		}

		private async void RefreshBtn_Click(object sender, RoutedEventArgs e)
		{
			Refresh();
			RefreshBtn.IsEnabled = false;
			await Task.Delay(150);
			RefreshBtn.IsEnabled = true;
		}

		private void SetLoaderFile()
		{
			if (!File.Exists(loaderdllpath))
			{
				if (File.Exists("SADXModLoader.dll"))
				{
					Util.MoveFile("SADXModLoader.dll", loaderdllpath);
				}
				else
				{
					new MessageWindow(Lang.GetString("SadxManagerError"), Lang.GetString("LoaderDllMissing"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
				}
			}
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
					SetLoaderFile();
				}
				else
				{
					new MessageWindow(Lang.GetString("FaildSetGamePathTitle"), Lang.GetString("FaildSetGamePath"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
				}
			}
		}

		private void UpdateBtnInstallLoader_State()
		{
			if (btnInstallLoader is null)
				return;

			//Update Text Button of Mod Loader Installer
			string textKey = installed ? "ManagerBtnUninstallLoader" : "ManagerBtnInstallLoader";
			TextBlock txt = FindName("txtInstallLoader") as TextBlock;
			txt.Text = Lang.GetString(textKey);

			//update icon image depending if the Mod Loader is installed or not
			string iconName = installed ? "IconUninstall" : "IconInstall";
			var dic = installed ? Icons.Icons.UninstallIcon : Icons.Icons.InstallIcon;

			DrawingImage Icon = dic[iconName] as DrawingImage;

			Image imgInstall = FindName("imgInstall") as Image;

			if (imgInstall is not null)
				imgInstall.Source = Icon;
		}

		private async void btnInstallLoader_Click(object sender, RoutedEventArgs e)
		{
			if (string.IsNullOrEmpty(gamePath) || !File.Exists(Path.Combine(gamePath, exeName)))
			{
				new MessageWindow(Lang.GetString("FailedDetectGamePathTitle"), Lang.GetString("FailedDetectGamePath"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
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
			UpdateBtnInstallLoader_State();
			Button button = (Button)sender;
			button.IsEnabled = false;
			int delayDuration = 2000;
			await Task.Delay(delayDuration);
			button.IsEnabled = true;

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

		#region Mod Profiles

		private void btnProfileSettings_Click(object sender, RoutedEventArgs e)
		{
			new ModProfile(ref comboProfile).ShowDialog();
		}

		private void ModProfile_FormClosing(object sender, EventArgs e)
		{
			Refresh();
		}

		private void LoadAllProfiles()
		{
			comboProfile.Items.Clear();

			if (modDirectory != string.Empty)
			{
				foreach (var item in Directory.EnumerateFiles(modDirectory, "*.ini"))
				{
					if (!item.EndsWith("SADXModLoader.ini", StringComparison.OrdinalIgnoreCase) && !item.EndsWith("desktop.ini", StringComparison.OrdinalIgnoreCase))
					{
						Profile pro = new()
						{
							name = Path.GetFileNameWithoutExtension(item),
							iniPath = item
						};

						comboProfile.Items.Add(pro);
					}
				}
			}
		}

		private void comboProfile_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			var selectedItem = comboProfile.SelectedItem as Profile;

			if (selectedItem != null)
			{
				loaderini = IniSerializer.Deserialize<SADXLoaderInfo>(selectedItem.iniPath);
				LoadSettings();
				Refresh();
			}
		}

		#endregion

		#region Patches

		private void SavePatches()
		{
			if (listPatches is null)
				return;

			PatchesData patch = (PatchesData)listPatches.Items[13];
			loaderini.KillGbix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[12];
			loaderini.LightFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[11];
			loaderini.PixelOffSetFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[10];
			loaderini.ChaoPanelFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[9];
			loaderini.E102PolyFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[8];
			loaderini.ChunkSpecFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[7];
			loaderini.Chaos2CrashFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[6];
			loaderini.SCFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[5];
			loaderini.FovFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[4];
			loaderini.InterpolationFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[3];
			loaderini.MaterialColorFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[2];
			loaderini.PolyBuff = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[1];
			loaderini.CCEF = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[0];
			loaderini.HRTFSound = patch.IsChecked;
		}

		private void UpdatePatches()
		{
			listPatches.Items.Clear();

			if (loaderini is null)
				return;

			List<PatchesData> patches = new List<PatchesData>()
			{
				new PatchesData() { Name = Lang.GetString("Patch3DSound"), IsChecked = loaderini.HRTFSound },
				new PatchesData() { Name = Lang.GetString("PatchCamCode"), IsChecked = loaderini.CCEF },
				new PatchesData() { Name = Lang.GetString("PatchVerColor"), IsChecked = loaderini.PolyBuff },
				new PatchesData() { Name = Lang.GetString("PatchMatColor"), IsChecked = loaderini.MaterialColorFix},
				new PatchesData() { Name = Lang.GetString("PatchInterpolation"), IsChecked = loaderini.InterpolationFix},
				new PatchesData() { Name = Lang.GetString("PatchFov"), IsChecked = loaderini.FovFix },
				new PatchesData() { Name = Lang.GetString("PatchSC"), IsChecked = loaderini.SCFix },
				new PatchesData() { Name = Lang.GetString("PatchChaos2Crash"), IsChecked = loaderini.Chaos2CrashFix },
				new PatchesData() { Name = Lang.GetString("PatchChunkSpec"), IsChecked = loaderini.ChunkSpecFix},
				new PatchesData() { Name = Lang.GetString("PatchE102Poly"), IsChecked = loaderini.E102PolyFix},
				new PatchesData() { Name = Lang.GetString("PatchChaoPanel"), IsChecked = loaderini.ChaoPanelFix},
				new PatchesData() { Name = Lang.GetString("PatchPixelOffset"), IsChecked = loaderini.PixelOffSetFix},
				new PatchesData() { Name = Lang.GetString("PatchLights"), IsChecked = loaderini.LightFix},
				new PatchesData() { Name = Lang.GetString("PatchGbix"), IsChecked = loaderini.KillGbix},
			};

			foreach (var patch in patches)
			{
				listPatches.Items.Add(patch);
			}
		}

		private void btnSelectAllPatch_Click(object sender, RoutedEventArgs e)
		{
			foreach (PatchesData patch in listPatches.Items)
			{
				patch.IsChecked = true;
			}
			RefreshPatchesList();
		}

		private void btnDeselectAllPatch_Click(object sender, RoutedEventArgs e)
		{
			foreach (PatchesData patch in listPatches.Items)
			{
				patch.IsChecked = false;
			}
			RefreshPatchesList();

		}

		private void RefreshPatchesList()
		{
			ICollectionView view = CollectionViewSource.GetDefaultView(listPatches.Items);
			view.Refresh();
		}

		#endregion

		#region Updates

		private void UpdateSelectedMods()
		{
			InitializeWorker();
			manualModUpdate = true;

			if (listMods.SelectedItem is ModData mod)
			{
				var pair = new KeyValuePair<string, ModInfo>(mod.Tag, mods[mod.Tag]);
				List<KeyValuePair<string, ModInfo>> list = new()
				{
					pair
				};


				updateChecker?.RunWorkerAsync(list);
			}
		}

		private bool CheckForUpdates(bool force = false)
		{
			if (!force && !loaderini.UpdateCheck)
			{
				return false;
			}
			if (!force && !Updater.UpdateHelper.UpdateTimeElapsed(loaderini.UpdateUnit, loaderini.UpdateFrequency, DateTime.FromFileTimeUtc(loaderini.UpdateTime)))
			{ 
				return false;
			}

			checkedForUpdates = true;
			loaderini.UpdateTime = DateTime.UtcNow.ToFileTimeUtc();

			if (!File.Exists(Path.Combine(gamePath, "sadxmlver.txt")))
			{
				return false;
			}

			return true;
		}

		private void UpdateChecker_DoWork(object sender, DoWorkEventArgs e)
		{
			if (!(sender is BackgroundWorker worker))
			{
				throw new Exception("what");
			}

			Dispatcher.Invoke(() =>
			{
				btnCheckUpdates.IsEnabled = false;
				ModContextChkUpdate.IsEnabled = false;
				ModContextDeleteMod.IsEnabled = false;
				ModContextDev.IsEnabled = false;
				ModContextForceUpdate.IsEnabled = false;
				ModContextVerifyIntegrity.IsEnabled = false;
			});

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
					App.DoEvents();

					if (worker.CancellationPending)
					{
						tokenSource.Cancel();
					}
				}

				task.Wait(token);
			}

			e.Result = new Tuple<List<ModDownload>, List<string>>(updates, errors);
		}

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
							errors.Add($"[{mod.Name}] GitHubRepo specified, but GitHubAsset is missing.");
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
				new MessageWindow(Lang.GetString("ErrorCheckForUpdateTitle"), Lang.GetString("ErrorCheckForUpdate") + "`\n" + errors, MessageWindow.WindowType.Message, MessageWindow.Icons.Warning, MessageWindow.Buttons.OK).ShowDialog();
			}

			bool manual = manualModUpdate;
			manualModUpdate = false;

			List<ModDownload> updates = data.Item1;
			if (updates.Count == 0)
			{
				if (manual)
				{
					new MessageWindow(Lang.GetString("InfoModUpToDateTitle"), Lang.GetString("InfoModUpToDate"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Information, MessageWindow.Buttons.OK).ShowDialog();
				}
				return;
			}

			var progress = new Updater.ModDownloadDialogWPF(updates, updatePath);	
			progress.Show();
			
			Refresh();
		}

		private void UpdateChecker_EnableControls(object sender, RunWorkerCompletedEventArgs runWorkerCompletedEventArgs)
		{
			btnCheckUpdates.IsEnabled = true;
			btnCheckUpdates.IsEnabled = true;
			ModContextChkUpdate.IsEnabled = true;
			ModContextDeleteMod.IsEnabled = true;
			ModContextDev.IsEnabled = true;
			ModContextForceUpdate.IsEnabled = true;
			ModContextVerifyIntegrity.IsEnabled = true;
		}

		private void CheckForModUpdates(bool force = false)
		{
			if (!force && !loaderini.ModUpdateCheck)
			{
				return;
			}

			InitializeWorker();

			if (!force && !Updater.UpdateHelper.UpdateTimeElapsed(loaderini.UpdateUnit, loaderini.UpdateFrequency, DateTime.FromFileTimeUtc(loaderini.ModUpdateTime)))
			{
				return;
			}

			checkedForUpdates = true;
			loaderini.ModUpdateTime = DateTime.UtcNow.ToFileTimeUtc();
			updateChecker.RunWorkerAsync(mods.Select(x => new KeyValuePair<string, ModInfo>(x.Key, x.Value)).ToList());
			btnCheckUpdates.IsEnabled = false;
		}

		private void btnCheckUpdates_Click(object sender, RoutedEventArgs e)
		{
			btnCheckUpdates.IsEnabled = false;

			if (CheckForUpdates(true))
			{
				Updater.UpdateHelper.DoUpdates(updatePath);
				return;
			}

			manualModUpdate = true;
			CheckForModUpdates(true);
		}

		#endregion

		private void tsCheckEvent_Checked(object sender, RoutedEventArgs e)
		{
			tsCheckCharacter.IsChecked = true;
			tsCheckLevel.IsChecked = true;
			TestSpawnGrid.RowDefinitions[3].Height = new GridLength(1, GridUnitType.Auto);
		}

		private void tsCheckEvent_Unchecked(object sender, RoutedEventArgs e)
		{
			TestSpawnGrid.RowDefinitions[3].Height = new GridLength(0);
		}

		private void btnSelectAllCode_Click(object sender, RoutedEventArgs e)
		{
			CodeListView.BeginInit();
			foreach (CodeData code in CodeListView.Items)
			{
				code.IsChecked = true;
			}
			CodeListView.EndInit();
		}

		private void btnDeselectAllCode_Click(object sender, RoutedEventArgs e)
		{
			CodeListView.BeginInit();
			foreach (CodeData code in CodeListView.Items)
			{
				code.IsChecked = false;
			}
			CodeListView.EndInit();
		}

		private void tsCheckCharacter_Click(object sender, RoutedEventArgs e)
		{
			if ((bool)tsCheckCharacter.IsChecked)
				tsCheckLevel.IsChecked = true;
		}

		private void tsCheckManual_Clicked(object sender, RoutedEventArgs e)
		{
			if ((bool)tsCheckManual.IsChecked)
			{
				tsCheckCharacter.IsEnabled = false;
				tsCheckLevel.IsEnabled = false;
				tsComboCharacter.IsEnabled = false;
				tsComboLevel.IsEnabled = false;
				tsComboAct.IsEnabled = false;
				tsComboTime.IsEnabled = false;

				tsNumCharacter.Value = 0;
				tsNumLevel.Value = tsComboLevel.SelectedIndex;
				tsNumAct.Value = tsComboAct.SelectedIndex;
			}
			else
			{
				tsCheckCharacter.IsEnabled = true;
				tsCheckLevel.IsEnabled = true;
				Binding bindCharacter = new Binding();
				bindCharacter.Path = new PropertyPath("IsChecked");
				bindCharacter.Source = tsCheckCharacter;
				Binding bindLevel = new Binding();
				bindLevel.Path = new PropertyPath("IsChecked");
				bindLevel.Source = tsCheckLevel;

				BindingOperations.SetBinding(tsComboCharacter, IsEnabledProperty, bindCharacter);
				BindingOperations.SetBinding(tsComboLevel, IsEnabledProperty, bindLevel);
				BindingOperations.SetBinding(tsComboAct, IsEnabledProperty, bindLevel);
				BindingOperations.SetBinding(tsComboTime, IsEnabledProperty, bindLevel);
			}
        }
    }
}
