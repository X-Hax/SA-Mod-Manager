using System;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using ModManagerCommon;
using System.Collections.Generic;
using System.IO;
using System.Windows.Input;
using System.Text.RegularExpressions;
using IniFile;
using System.Windows.Threading;
using System.Diagnostics;
using SAModManager.Properties;
using System.Windows.Media;
using System.Threading.Tasks;
using SAModManager.Common;
using System.ComponentModel;
using System.Windows.Data;
using System.Security.Cryptography;
using System.Threading;
using System.Collections.ObjectModel;
using Keyboard = System.Windows.Input.Keyboard;
using Key = System.Windows.Input.Key;
using SAModManager.UI;
using SAModManager.Updater;
using SAModManager.Elements;
using Newtonsoft.Json.Linq;
using SevenZipExtractor;

namespace SAModManager
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>

	public partial class MainWindow : Window
	{
		#region Variables

		public readonly string titleName = "SADX Mod Manager";
		private const string V = "1.0.0";
		private static string Version = V;
		private static string GitVersion = string.Empty;
		public static string modDirectory = string.Empty;
		private static string updatePath = "mods/.updates";
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
		private string d3d8to9InstalledDLLName = "d3d8.dll";
		private string d3d8to9StoredDLLName = "d3d8m.dll";
		CodeList mainCodes = null;
		List<Code> codes = null;
		public List<CodeData> codesSearch { get; set; }
		bool installed = false;
		bool suppressEvent = false;
		BackgroundWorker updateChecker;
		private bool manualModUpdate;
		readonly Updater.ModUpdater modUpdater = new();
		private bool checkedForUpdates = false;

		private readonly double LowOpacityIcon = 0.3;
		private readonly double LowOpacityBtn = 0.7;


		private Game.GameConfigFile gameConfigFile;

		public Game.Graphics graphics;
		public GitHub git;
		static private uint count = 0;
		MenuItem ModContextDev { get; set; }
		TestSpawn TS { get; set; }
		private bool displayedManifestWarning;

		public MainWindowViewModel ViewModel = new();

		#endregion

		public MainWindow(string[] args = null)
		{
			git = new(this);
			InitializeComponent();
			git.GetRecentCommit();

			graphics = new Game.Graphics(ref comboScreen);
			SetGamePath(Settings.Default.GamePath);
			UpdatePathsStringsInfo();
			LoadSettings();
			InitCodes();
			LoadModList();

			LoadAllProfiles();
			UpdateDLLData();
			UpdatePatches();
			setupTestSpawn();
			InitMouseList();
			SetUp_UpdateD3D9();
			UpdateAppLauncherBtn();
			SetOneClickBtnState();

			if (args is not null)
				CleanUpdate(args);
		}

		private void UpdateDLLData()
		{
			if (File.Exists("SADXModLoader.dll"))
			{
				File.Copy("SADXModLoader.dll", loaderdllpath, true);
			}

			if (File.Exists(loaderdllpath) && File.Exists(datadllorigpath))
			{
				File.Copy(loaderdllpath, datadllpath, true);
			}
		}

		private async void SetGamePath(string path)
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
				else
				{
					//if none of the conditions are respected, try to look for sadx folder
					var fullPath = await Util.GetSADXGamePath();
					if (fullPath is not null)
					{
						gamePath = fullPath;
					}
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
			new OneClickInstall(updatePath, modDirectory);
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

				d3d8to9InstalledDLLName = Path.Combine(gamePath, "d3d8.dll");
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
			loaderini.HorizontalResolution = txtResX.GetInt();
			loaderini.VerticalResolution = txtResY.GetInt();
			loaderini.ForceAspectRatio = (bool)chkRatio.IsChecked;
			loaderini.PauseWhenInactive = (bool)chkPause.IsChecked;

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
			loaderini.WindowWidth = txtCustomResX.GetInt();
			loaderini.WindowHeight = txtCustomResY.GetInt();
			loaderini.MaintainWindowAspectRatio = (bool)chkMaintainRatio.IsChecked;
			loaderini.EnableDynamicBuffer = (bool)chkDynamicBuffers.IsChecked;
			loaderini.ResizableWindow = (bool)chkResizableWin.IsChecked;
			loaderini.UpdateCheck = (bool)chkUpdatesML.IsChecked;
			loaderini.ModUpdateCheck = (bool)chkUpdatesMods.IsChecked;
			loaderini.Language = comboLanguage.SelectedIndex;
			loaderini.Theme = comboThemes.SelectedIndex;
			loaderini.managerOpen = (bool)checkManagerOpen.IsChecked;
			loaderini.devMode = (bool)checkDevEnabled.IsChecked;
			loaderini.InputModEnabled = (bool)radBetterInput.IsChecked;
			loaderini.SEVolume = (int)sliderSFX.Value;

			loaderini.EnableBassMusic = (bool)checkBassMusic.IsChecked;
			loaderini.EnableBassSFX = (bool)checkBassSFX.IsChecked;

			//test spawn stuff
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

			txtResX.Value = loaderini.HorizontalResolution;
			txtResY.Value = loaderini.VerticalResolution;
			chkUpdatesML.IsChecked = loaderini.UpdateCheck;
			chkUpdatesMods.IsChecked = loaderini.ModUpdateCheck;

			chkRatio.IsChecked = loaderini.ForceAspectRatio;
			checkUIScale.IsChecked = loaderini.ScaleHud;

			comboBGFill.SelectedIndex = loaderini.BackgroundFillMode;
			comboFMVFill.SelectedIndex = loaderini.FmvFillMode;

			chkDynamicBuffers.IsChecked = loaderini.EnableDynamicBuffer;
			chkBorderless.IsChecked = loaderini.Borderless;
			checkMipmapping.IsChecked = loaderini.AutoMipmap;
			chkScaleScreen.IsChecked = loaderini.StretchFullscreen;

			int screenNum = graphics.GetScreenNum(loaderini.ScreenNum);

			comboScreen.SelectedIndex = screenNum;
			chkBorderless.IsChecked = txtCustomResY.IsEnabled = chkMaintainRatio.IsEnabled = loaderini.CustomWindowSize;
			chkCustomWinSize.IsChecked = loaderini.CustomWindowSize;
			chkMaintainRatio.IsChecked = loaderini.MaintainWindowAspectRatio;
			txtCustomResX.IsEnabled = loaderini.CustomWindowSize && !loaderini.MaintainWindowAspectRatio;
			System.Drawing.Rectangle rect = graphics.GetRectangleStruct();

			int CustresXMax = (int)txtCustomResX.MaxValue;
			int CustresXMin = (int)txtCustomResX.MinValue;
			int CustresYMin = (int)txtCustomResY.MinValue;
			int CustresYMax = (int)txtCustomResY.MaxValue;
			txtCustomResX.MaxValue = rect.Width;
			txtCustomResX.Value = loaderini.WindowWidth;
			txtCustomResY.Value = loaderini.WindowHeight;
			txtCustomResY.MaxValue = rect.Height;

			suppressEvent = true;
			chkBorderless.IsChecked = loaderini.MaintainWindowAspectRatio;
			suppressEvent = false;

			chkResizableWin.IsChecked = loaderini.ResizableWindow;
			chkPause.IsChecked = loaderini.PauseWhenInactive;

			radBetterInput.IsChecked = loaderini.InputModEnabled;
			radVanillaInput.IsChecked = !radBetterInput.IsChecked;
			checkManagerOpen.IsChecked = loaderini.managerOpen;
			checkDevEnabled.IsChecked = loaderini.devMode;

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

			if ((bool)!checkDevEnabled.IsChecked)
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
				new MessageWindow(Lang.GetString("MessageWindow.Errors.GamePathNotFound.Title"), Lang.GetString("MessageWindow.Errors.GamePathNotFound"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
				return;
			}

			string executablePath = loaderini.Mods.Select(item => mods[item].EXEFile).FirstOrDefault(item => !string.IsNullOrEmpty(item)) ?? Path.Combine(gamePath, "sonic.exe");

			string folderPath = Path.GetDirectoryName(executablePath);
			Process.Start(new ProcessStartInfo(executablePath)
			{
				WorkingDirectory = folderPath
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
			ViewModel.Modsdata.Clear();
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
				new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), Lang.GetString("MessageWindow.Errors.ModWithoutFolder0") + Lang.GetString("MessageWindow.Errors.ModWithoutFolder1") +
							Lang.GetString("MessageWindow.Errors.ModWithoutFolder2"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error,
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

					ViewModel.Modsdata.Add(item);

					//if a mod has a code, add it to the list
					if (!string.IsNullOrEmpty(inf.Codes))
					{
						var t = CodeList.Load(Path.Combine(Path.Combine(modDirectory, mod), inf.Codes));
						codes.AddRange(t.Codes);

						foreach (var code in t.Codes)
						{
							CodeData extraItem = new()
							{
								codes = code,
								IsChecked = loaderini.EnabledCodes.Contains(item.Name),
							};

							extraItem.codes.Category = "Codes From " + inf.Name;
							codesSearch.Add(extraItem);
							CodeListView.Items.Add(extraItem);
						}
					}

					suppressEvent = false;
				}
				else
				{
					new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle"), "Mod \"" + mod + "\"" + Lang.GetString("MessageWindow.Errors.ModNotFound"), MessageWindow.WindowType.Message, MessageWindow.Icons.Information, MessageWindow.Buttons.OK).ShowDialog();
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

					ViewModel.Modsdata.Add(item);
				}
			}


			DataContext = ViewModel;
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

			textModsDescription.Text = Lang.GetString("CommonStrings.Description") + " " + mods[mod.Tag].Description;
		}

		private void ModList_MouseLeave(object sender, MouseEventArgs e)
		{
			var item = GetModFromView(sender);
			textModsDescription.Text = (item is not null) ? $"{Lang.GetString("CommonStrings.Description")} {item.Description}" : Lang.GetString("Manager.Tabs.Mods.Text.NoModSelected");
		}

		#region ModContext
		private void ModContextOpenFolder_Click(object sender, RoutedEventArgs e)
		{
			var selectedMods = listMods.SelectedItems.OfType<ModData>();

			foreach (var mod in selectedMods.Where(mod => !string.IsNullOrEmpty(mod.Tag)))
			{
				string fullPath = Path.Combine(modDirectory, mod.Tag);
				if (Directory.Exists(fullPath))
					Process.Start(new ProcessStartInfo { FileName = fullPath, UseShellExecute = true });
			}
		}

		private void ModContextChkUpdate_Click(object sender, RoutedEventArgs e)
		{
			UpdateSelectedMods();
		}

		private void ModContextVerifyIntegrity_Click(object sender, RoutedEventArgs e)
		{

			List<Tuple<string, ModInfo>> items = listMods.SelectedItems.Cast<ModData>()
				.Select(x => (string)x.Tag)
				.Where(x => File.Exists(Path.Combine(modDirectory, x, "mod.manifest")))
				.Select(x => new Tuple<string, ModInfo>(x, mods[x]))
				.ToList();

			if (items.Count < 1)
			{
				new MessageWindow(Lang.GetString("MessageWindow.Errors.MissingManifest.Title"), Lang.GetString("MessageWindow.Errors.MissingManifest"),
			MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Information).ShowDialog();
				return;
			}

			var progress = new Updater.VerifyModDialog(items, modDirectory);

			progress.ShowDialog();

			bool? res = progress.DialogResult;

			if (res == false)
			{
				return;
			}

			List<Tuple<string, ModInfo, List<Updater.ModManifestDiff>>> failed = progress.Failed;

			if (failed.Count < 1)
			{
				new MessageWindow(Lang.GetString("MessageWindow.Information.ModPassedVerif.Title"), Lang.GetString("MessageWindow.Information.ModPassedVerif"),
					MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Information).ShowDialog();
			}
			else
			{

				var error0 = Lang.GetString("MessageWindow.Errors.ModFailedVerif")
	+ string.Join("\n", failed.Select(x => $"{x.Item2.Name}: {x.Item3.Count(y => y.State != Updater.ModManifestState.Unchanged)}" + Lang.GetString("MessageWindow.Errors.ModFailedVerif1"))
	+ Lang.GetString("MessageWindow.Errors.ModFailedVerif2"));


				var result = new MessageWindow(Lang.GetString("MessageWindow.Errors.ModFailedVerif.Title"), error0, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Information, MessageWindow.Buttons.YesNo);
				result.ShowDialog();

				if (result.isYes != true)
				{
					return;
				}


				InitializeWorker();

				updateChecker.DoWork -= UpdateChecker_DoWork;
				updateChecker.DoWork += UpdateChecker_DoWorkForced;

				updateChecker.RunWorkerAsync(failed);

				modUpdater.ForceUpdate = true;
				btnCheckUpdates.IsEnabled = false;
			}

		}

		private void ForceModUpdate_Click(object sender, RoutedEventArgs e)
		{
			var result = new MessageWindow(Lang.GetString("MessageWindow.Warnings.ForceModUpdateTitle"), Lang.GetString("MessageWindow.Warnings.ForceModUpdate"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Caution, MessageWindow.Buttons.YesNo);
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
			Edit.ShowDialog();
			Edit.Closed += EditMod_FormClosing;
		}

		private void ModContextDeleteMod_Click(object sender, RoutedEventArgs e)
		{
			var selectedItems = listMods.SelectedItems;
			var count = selectedItems.Count > 0;

			if (count)
			{
				var confirmMessage = Lang.GetString("MessageWindow.Warnings.DeleteMod");
				var deleteConfirmation = new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle"), confirmMessage, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);

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
		#endregion

		private void InitModConfig()
		{
			var mod = (ModData)listMods.SelectedItem;

			if (mod is not null)
			{
				SADXModInfo modInfo = mods[mod.Tag];
				string fullPath = Path.Combine(modDirectory, mod.Tag);
				Common.ModConfig config = new(modInfo.Name, fullPath);
				config.ShowDialog();
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
					Edit.ShowDialog();
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
			if (ConfigureModBtn.IsEnabled)
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
				gameConfigFile.GameConfig = new()
				{
					FrameRate = (int)Game.FrameRate.High,
					Sound3D = 1,
					SEVoice = 1,
					BGM = 1,
					BGMVolume = 100,
					VoiceVolume = 100
				};
			}

			// Video
			// Display mode
			if (gameConfigFile.GameConfig.FullScreen == 1)
				radFullscreen.IsChecked = true;
			else
				radWindowed.IsChecked = true;

			// Framerate
			if (gameConfigFile.GameConfig.FrameRate == (int)Game.FrameRate.Invalid || gameConfigFile.GameConfig.FrameRate > (int)Game.FrameRate.Low)
			{
				new MessageWindow(Lang.GetString("MessageWindow.Errors.InvalidFramerate.Title"), Lang.GetString("MessageWindow.Errors.InvalidFramerate.Title"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
				comboFramerate.SelectedIndex = (int)Game.FrameRate.High - 1;
			}
			else
			{
				comboFramerate.SelectedIndex = gameConfigFile.GameConfig.FrameRate - 1;
			}

			// Clip level
			comboDetail.SelectedIndex = (int)gameConfigFile.GameConfig.ClipLevel;

			// Fog mode
			comboFog.SelectedIndex = gameConfigFile.GameConfig.Foglation;


			// Sound
			// Toggles
			checkEnableMusic.IsChecked = gameConfigFile.GameConfig.BGM > 0;
			checkEnableSounds.IsChecked = gameConfigFile.GameConfig.SEVoice > 0;
			checkEnable3DSound.IsChecked = gameConfigFile.GameConfig.Sound3D > 0;

			//volume
			sliderMusic.Value = gameConfigFile.GameConfig.BGMVolume;
			sliderVoice.Value = gameConfigFile.GameConfig.VoiceVolume;


			if (gameConfigFile.GameConfig.MouseMode == 0)
				inputMouseDragHold.IsChecked = true;
			else
				inputMouseDragAccel.IsChecked = true;

		}

		private void SaveGameConfigIni()
		{
			if (!File.Exists(sadxIni))
				return;

			gameConfigFile.GameConfig.FullScreen = (bool)radFullscreen.IsChecked ? 1 : 0;

			gameConfigFile.GameConfig.FrameRate = comboFramerate.SelectedIndex + 1;
			gameConfigFile.GameConfig.ClipLevel = comboDetail.SelectedIndex;
			gameConfigFile.GameConfig.Foglation = comboFog.SelectedIndex;

			gameConfigFile.GameConfig.Sound3D = (bool)checkEnable3DSound.IsChecked ? 1 : 0;
			gameConfigFile.GameConfig.SEVoice = (bool)checkEnableSounds.IsChecked ? 1 : 0;
			gameConfigFile.GameConfig.BGM = (bool)checkEnableMusic.IsChecked ? 1 : 0;

			gameConfigFile.GameConfig.VoiceVolume = (int)sliderVoice.Value;
			gameConfigFile.GameConfig.BGMVolume = (int)sliderMusic.Value;

			gameConfigFile.GameConfig.MouseMode = (bool)inputMouseDragHold.IsChecked ? 0 : 1;

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
				new MessageWindow(Lang.GetString("MessageWindow.Errors.CodesListFailed.Title"), Lang.GetString("MessageWindow.Errors.CodesListFailed") + msg, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
				mainCodes = new CodeList();
			}
		}

		private void LoadCodes()
		{
			codes = new List<Code>(mainCodes.Codes);
			codesSearch = new();

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

				codesSearch.Add(extraItem);
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

		private CodeData GetCodeFromView(object sender)
		{
			if (sender is ListViewItem lvItem)
				return lvItem.Content as CodeData;
			else if (sender is ListView lv)
				return lv.SelectedItem as CodeData;


			return CodeListView.Items[CodeListView.SelectedIndex] as CodeData;
		}
		private void CodesView_Item_MouseDoubleClick(object sender, MouseButtonEventArgs e)
		{
			var code = GetCodeFromView(sender);

			if (code == null)
				return;

			OpenAboutCodeWindow(code.codes);
		}

		private void CodesView_Item_MouseEnter(object sender, MouseEventArgs e)
		{
			var codes = GetCodeFromView(sender);

			if (codes == null)
				return;

			var code = codes.codes;

			CodeAuthorGrid.Text += " " + code.Author;
			CodeDescGrid.Text += " " + code.Description;
			CodeCategoryGrid.Text += " " + code.Category;
		}

		private void CodesView_Item_MouseLeave(object sender, MouseEventArgs e)
		{
			CodeAuthorGrid.Text = Lang.GetString("CommonStrings.Author");
			CodeDescGrid.Text = Lang.GetString("CommonStrings.Description");
			CodeCategoryGrid.Text = Lang.GetString("CommonStrings.Category");
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

		private void btnAddCode_Click(object sender, RoutedEventArgs e)
		{
			Common.NewCode newcodewindow = new Common.NewCode();
			newcodewindow.ShowDialog();
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

		private void comboScreen_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			graphics?.screenNumBox_SelectChanged(ref comboScreen, ref comboDisplay);
		}

		private void chkCustomWinSize_Checked(object sender, RoutedEventArgs e)
		{
			chkMaintainRatio.IsEnabled = chkCustomWinSize.IsChecked.GetValueOrDefault();
			chkResizableWin.IsEnabled = !chkCustomWinSize.IsChecked.GetValueOrDefault();

			txtCustomResX.IsEnabled = chkCustomWinSize.IsChecked.GetValueOrDefault() && !chkMaintainRatio.IsChecked.GetValueOrDefault();
			txtCustomResY.IsEnabled = chkCustomWinSize.IsChecked.GetValueOrDefault();
		}

		private void chkRatio_Click(object sender, RoutedEventArgs e)
		{
			if (chkRatio.IsChecked == true)
			{
				txtResX.IsEnabled = false;
				decimal resYDecimal = (decimal)txtResY.Value;
				decimal roundedValue = Math.Round(resYDecimal * Game.Graphics.ratio);
				txtResX.Value = (double)roundedValue;
			}
			else if (!suppressEvent)
			{
				txtResX.IsEnabled = true;
			}
		}

		private void comboResolutionPreset_SelectedIndexChanged(object sender, SelectionChangedEventArgs e)
		{
			if (comboDisplay.SelectedIndex == -1)
				return;

			int index = comboDisplay.SelectedIndex;

			suppressEvent = true;
			txtResY.Value = graphics.resolutionPresets[index].Height;

			if (chkRatio.IsChecked == false)
				txtResX.Value = graphics.resolutionPresets[index].Width;

			suppressEvent = false;
		}

		private void chkMaintainRatio_Click(object sender, RoutedEventArgs e)
		{
			if (chkMaintainRatio.IsChecked == true)
			{
				txtCustomResX.IsEnabled = false;
				double ratio = txtResX.Value / txtResY.Value;
				txtCustomResX.Value = Math.Ceiling(txtCustomResY.Value * ratio);
			}
			else if (!suppressEvent)
			{
				txtCustomResX.IsEnabled = true;
			}
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

		#region Test Spawn Settings
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
			if (string.IsNullOrEmpty(gamePath))
			{
				new MessageWindow(Lang.GetString("MessageWindow.Errors.GamePathNotFound.Title"), Lang.GetString("MessageWindow.Errors.GamePathNotFound"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
				return;
			}

			string executablePath = loaderini.Mods.Select(item => mods[item].EXEFile).FirstOrDefault(item => !string.IsNullOrEmpty(item)) ?? Path.Combine(gamePath, "sonic.exe");

			string commandLine = GetTestSpawnCommandLine();

			ProcessStartInfo startInfo = new ProcessStartInfo(executablePath)
			{
				WorkingDirectory = gamePath,
				Arguments = commandLine
			};

			Process.Start(startInfo);
		}

		private void tsComboLevel_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			ComboBox t = sender as ComboBox;

			tsComboAct.BeginInit();

			tsComboAct.ItemsSource = TestSpawn.GetNewAct(t.SelectedIndex);

			tsComboAct.EndInit();
			tsComboAct.SelectedIndex = 0;
		}

		private void tsCheckLevel_Checked(object sender, RoutedEventArgs e)
		{
			if (tsComboAct.SelectedIndex < 0)
				tsComboAct.SelectedIndex = 0;

			if (tsComboLevel.SelectedIndex < 0)
				tsComboLevel.SelectedIndex = 0;

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

					tsComboSave.ItemsSource = list;

					tsComboSave.EndInit();
				}
			}
		}

		private void setupTestSpawn()
		{
			TS = new TestSpawn(ref tsComboLevel, ref tsComboGameMode, ref tsComboEvent, ref tsComboCharacter);

			TS.InitCutsceneList();
			TS.InitGameModeList();
			TS.InitCharactersList();
			TS.InitLevels();

			int index = loaderini?.TestSpawnLevel != -1 ? loaderini.TestSpawnLevel : 0;
			tsComboAct.ItemsSource = TestSpawn.GetNewAct(index);
			if (tsCheckLevel.IsChecked == true)
				tsComboAct.SelectedIndex = loaderini?.TestSpawnAct != -1 ? loaderini.TestSpawnAct : 0;
			tsComboTime.ItemsSource = TestSpawn.TimeDay;

			TS_GetSave();
		}

		private void tsCheckEvent_Checked(object sender, RoutedEventArgs e)
		{
			tsCheckCharacter.IsChecked = true;
			tsCheckLevel.IsChecked = true;

			if (tsComboCharacter.SelectedIndex < 0 && tsCheckManual.IsChecked == false)
				tsComboCharacter.SelectedIndex = 0;

			if (tsComboLevel.SelectedIndex < 0 && tsCheckManual.IsChecked == false)
				tsComboLevel.SelectedIndex = 0;

			if (tsComboEvent.SelectedIndex < 0 && tsCheckManual.IsChecked == false)
				tsComboEvent.SelectedIndex = 0;

			TestSpawnGrid.RowDefinitions[3].Height = new GridLength(1, GridUnitType.Auto);
		}

		private void tsCheckEvent_Unchecked(object sender, RoutedEventArgs e)
		{
			TestSpawnGrid.RowDefinitions[3].Height = new GridLength(0);
		}

		private void tsCheckCharacter_Click(object sender, RoutedEventArgs e)
		{
			if ((bool)tsCheckCharacter.IsChecked)
			{
				tsCheckLevel.IsChecked = true;

				if (tsComboCharacter.SelectedIndex < 0 && tsCheckManual.IsChecked == false)
					tsComboCharacter.SelectedIndex = 0;
			}

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
				tsNumLevel.Value = 0;
				tsNumAct.Value = tsComboAct.SelectedIndex;
			}
			else
			{
				tsCheckCharacter.IsEnabled = true;
				tsCheckLevel.IsEnabled = true;
				Binding bindCharacter = new Binding
				{
					Path = new PropertyPath("IsChecked"),
					Source = tsCheckCharacter
				};
				Binding bindLevel = new Binding
				{
					Path = new PropertyPath("IsChecked"),
					Source = tsCheckLevel
				};

				BindingOperations.SetBinding(tsComboCharacter, IsEnabledProperty, bindCharacter);
				BindingOperations.SetBinding(tsComboLevel, IsEnabledProperty, bindLevel);
				BindingOperations.SetBinding(tsComboAct, IsEnabledProperty, bindLevel);
				BindingOperations.SetBinding(tsComboTime, IsEnabledProperty, bindLevel);
			}
		}

		private string GetTestSpawnCommandLine()
		{
			List<string> cmdline = new List<string>();

			bool advanced = tsCheckManual.IsChecked == true;

			if (tsCheckLevel.IsChecked.GetValueOrDefault() && tsComboLevel.SelectedIndex > -1)
				cmdline.Add("-l " + (advanced ? tsNumLevel.Value.ToString() : tsComboLevel.SelectedIndex.ToString()) + " -a " + (advanced ? tsNumAct.Value.ToString() : tsComboAct.SelectedIndex.ToString()));

			if (tsCheckCharacter.IsChecked == true && tsComboCharacter.SelectedIndex > -1)
				cmdline.Add("-c " + (advanced ? tsNumCharacter.Value.ToString() : tsComboCharacter.SelectedIndex.ToString()));

			if (tsCheckPosition.IsChecked == true)
				cmdline.Add("-p " + tsNumPosX.Value.ToString() + " " +
					tsNumPosY.Value.ToString() + " " +
					tsNumPosZ.Value.ToString() + " -r " +
					tsNumAngle.Value.ToString());

			if (tsCheckEvent.IsChecked == true && tsComboEvent.SelectedIndex > -1)
			{
				int ev = 0;
				int ev_result = 0;

				foreach (var item in TS.GetCutsceneList())
				{
					if (ev == tsComboEvent.SelectedIndex)
					{
						ev_result = item.Key;
						break;
					}
					ev++;
				}
				cmdline.Add("-e " + ev_result.ToString());
			}
			if (tsComboTime.SelectedIndex > 0)
				cmdline.Add("-t " + (tsComboTime.SelectedIndex - 1).ToString());

			if (tsCheckGameMode.IsChecked == true && tsComboGameMode.SelectedIndex > -1)
			{
				uint gm = 0;
				uint gm_result = 0;
				foreach (var item in TS.GetTestSpawnGameModeList())
				{
					if (gm == tsComboGameMode.SelectedIndex)
					{
						gm_result = item.Key;
						break;
					}
					gm++;
				}
				cmdline.Add("-g " + gm_result.ToString());
			}

			if (tsCheckSave.IsChecked == true)
			{
				string save = tsComboSave.SelectedValue.ToString();
				save = Util.GetSaveNumber(save);
				cmdline.Add("-s " + save);
			}

			return string.Join(" ", cmdline);
		}



		#endregion

		#region Inputs

		private void InitMouseList()
		{
			List<string> mouseActionList = new()
			{
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Start"),
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Cancel"),
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Jump"),
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Action"),
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Flute"),

			};

			mouseAction.ItemsSource = mouseActionList;

			List<string> mouseBtnAssignList = new()
			{
				Lang.GetString("CommonStrings.None"),
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Group.MouseKeyboard.LeftMouseBtn"),
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Group.MouseKeyboard.RightMouseBtn"),
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Group.MouseKeyboard.MiddleMouseBtn"),
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Group.MouseKeyboard.OtherMouseBtn"),
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Group.MouseKeyboard.LeftRightMouseBtn"),
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Group.MouseKeyboard.RightLeftMouseBtn"),
			};

			mouseBtnAssign.ItemsSource = mouseBtnAssignList;
		}


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
				var item = ViewModel.Modsdata[index];
				ViewModel.Modsdata.Remove(item);
				ViewModel.Modsdata.Insert(0, item);
			}
		}

		private void btnMoveUp_Click(object sender, RoutedEventArgs e)
		{
			if (listMods.SelectedItems.Count < 1)
				return;

			int index = listMods.SelectedIndex;

			if (index > 0)
			{
				var item = ViewModel.Modsdata[index];
				ViewModel.Modsdata.Remove(item);
				ViewModel.Modsdata.Insert(index - 1, item);
			}
		}

		private void btnMoveBottom_Click(object sender, RoutedEventArgs e)
		{
			if (listMods.SelectedItems.Count < 1)
				return;

			int index = listMods.SelectedIndex;

			if (index != listMods.Items.Count - 1)
			{
				var item = ViewModel.Modsdata[index];
				ViewModel.Modsdata.Remove(item);
				ViewModel.Modsdata.Insert(listMods.Items.Count, item);
			}
		}

		private void btnMoveDown_Click(object sender, RoutedEventArgs e)
		{
			if (listMods.SelectedItems.Count < 1)
				return;

			int index = listMods.SelectedIndex;

			if (index < listMods.Items.Count)
			{
				var item = ViewModel.Modsdata[index];
				ViewModel.Modsdata.Remove(item);
				ViewModel.Modsdata.Insert(index + 1, item);
			}
		}

		public void Refresh()
		{
			InitCodes();
			LoadModList();
			RefreshPatchesList();
			if (ModsFind.Visibility == Visibility.Visible)
			{
				FilterMods(TextBox_ModsSearch.Text.ToLowerInvariant());
			}
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
					new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), Lang.GetString("MessageWindow.Errors.MissingLoaderDLL"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
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
					InstallLoader(true);
				}
				else
				{
					new MessageWindow(Lang.GetString("MessageWindow.Errors.GamePathFailed.Title"), Lang.GetString("MessageWindow.Errors.GamePathFailed"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
				}
			}
		}

		private void UpdateBtnInstallLoader_State()
		{
			if (btnInstallLoader is null)
				return;

			//Update Text Button of Mod Loader Installer
			string textKey = installed ? "Manager.Tabs.ManagerConfig.Group.Options.Buttons.UninstallLoader" : "Manager.Tabs.ManagerConfig.Group.Options.Buttons.InstallLoader";
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

		private void InstallLoader(bool force = false)
		{
			if (installed && force)
			{
				File.Delete(datadllpath);
				Util.MoveFile(datadllorigpath, datadllpath);

				installed = false;
			}

			if (installed)
			{
				File.Delete(datadllpath);
				Util.MoveFile(datadllorigpath, datadllpath);
			}
			else
			{
				File.Move(datadllpath, datadllorigpath);

				if (File.Exists(loaderdllpath))
					File.Copy(loaderdllpath, datadllpath);
			}

			installed = !installed;
			UpdateBtnInstallLoader_State();
			Update_PlayButtonsState();
		}

		private async void btnInstallLoader_Click(object sender, RoutedEventArgs e)
		{
			if (string.IsNullOrEmpty(gamePath) || !File.Exists(Path.Combine(gamePath, exeName)))
			{
				new MessageWindow(Lang.GetString("MessageWindow.Errors.GamePathNotFound.Title"), Lang.GetString("MessageWindow.Errors.GamePathNotFound"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
				return;
			}

			InstallLoader();
			btnBrowseGameDir.IsEnabled = false;
			Save();
			await Task.Delay(2000);
			btnBrowseGameDir.IsEnabled = true;
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
			bool isEnabled = ConfigureModBtn.IsEnabled;

			if (ModContextMenu is not null)
			{
				var item = ModContextMenu.Items.OfType<MenuItem>().FirstOrDefault(item => item.Name == "ModContextConfigureMod");

				if (item is not null)
				{
					item.IsEnabled = isEnabled;
					item.Opacity = IsEnabled ? 1 : LowOpacityBtn;
					Image iconConfig = FindName("menuIconConfig") as Image;
					iconConfig?.SetValue(Image.OpacityProperty, isEnabled ? 1 : LowOpacityIcon);
				}

				if (checkDevEnabled.IsChecked == true)
				{
					if (ModContextDev is null)
					{
						/*
						ModContextDev = new();
						MenuItem manifest = new();
						ModContextDev.Name = "menuDev";
						ModContextDev.Header = Lang.GetString("ModsUIDev");
						manifest.Name = "menuManif";
						manifest.Header = Lang.GetString("ModsUISubDevManifest");
						ModContextDev.Items.Add(manifest);
						ModContextMenu.Items.Add(ModContextDev);
						*/
					}
				}
				else
				{
					if (ModContextDev is not null)
					{
						var modDev = ModContextMenu.Items.OfType<MenuItem>().FirstOrDefault(item => item.Name == "menuDev");

						if (modDev is not null)
							ModContextMenu.Items.Remove(modDev);
					}

				}

			}
		}

		private void ModsContextDeveloperManifest_Click(object sender, RoutedEventArgs e)
		{
			if (!displayedManifestWarning)
			{
				var result = new MessageWindow(Lang.GetString("Warning"), Lang.GetString("MessageWindow.Warnings.GenerateManifest"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);
				result.ShowDialog();
				if (result.isYes != true)
				{
					return;
				}

				displayedManifestWarning = true;
			}

			foreach (ModData item in listMods.SelectedItems)
			{

				var modPath = Path.Combine(modDirectory, (string)item.Tag);
				var manifestPath = Path.Combine(modPath, "mod.manifest");

				List<Updater.ModManifestEntry> manifest;
				List<Updater.ModManifestDiff> diff;

				var progress = new Updater.ManifestDialog(modPath, $"Generating manifest: {(string)item.Tag}", true);
				progress.ShowDialog();

				bool? ManifDialog = progress.DialogResult;

				//progress.SetTask("Generating file index...");
				if (ManifDialog != true)
				{
					continue;
				}

				diff = progress.Diff;

				if (diff == null)
				{
					continue;
				}

				if (diff.Count(x => x.State != Updater.ModManifestState.Unchanged) <= 0)
				{
					continue;
				}

				var dialog = new ManifestChanges(diff);
				dialog.ShowDialog();
				bool? resultManifChange = dialog.DialogResult;

				if (resultManifChange != true)
				{
					continue;
				}

				manifest = dialog.MakeNewManifest();

				Updater.ModManifest.ToFile(manifest, manifestPath);
			}
		}


		#endregion

		#region Mod Profiles

		private void btnProfileSettings_Click(object sender, RoutedEventArgs e)
		{
			if (!installed)
				return;

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


			PatchesData patch = (PatchesData)listPatches.Items[14];
			loaderini.DisableCDCheck = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[13];
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
				new PatchesData() { Name = Lang.GetString("GamePatches.3DSound"),
					Description = Lang.GetString("GamePatches.3DSoundDesc"), IsChecked = loaderini.HRTFSound },
				new PatchesData() { Name = Lang.GetString("GamePatches.CamCode"),
					Description = Lang.GetString("GamePatches.CamCodeDesc"),IsChecked = loaderini.CCEF },
				new PatchesData() { Name = Lang.GetString("GamePatches.VertexColor"),
					Description = Lang.GetString("GamePatches.VertexColorDesc"),IsChecked = loaderini.PolyBuff },
				new PatchesData() { Name = Lang.GetString("GamePatches.MaterialColor"),
					Description = Lang.GetString("GamePatches.MaterialColorDesc"),IsChecked = loaderini.MaterialColorFix},
				new PatchesData() { Name = Lang.GetString("GamePatches.Interpolation"),
					Description = Lang.GetString("GamePatches.InterpolationDesc"),IsChecked = loaderini.InterpolationFix},
				new PatchesData() { Name = Lang.GetString("GamePatches.FixFOV"),
					Description = Lang.GetString("GamePatches.FixFOVDesc"),IsChecked = loaderini.FovFix },
				new PatchesData() { Name = Lang.GetString("GamePatches.Skychase"),
					Description = Lang.GetString("GamePatches.SkychaseDesc"),IsChecked = loaderini.SCFix },
				new PatchesData() { Name = Lang.GetString("GamePatches.Chaos2"),
					Description = Lang.GetString("GamePatches.Chaos2Desc"),IsChecked = loaderini.Chaos2CrashFix },
				new PatchesData() { Name = Lang.GetString("GamePatches.ChunkRendering"),
					Description = Lang.GetString("GamePatches.ChunkRenderingDesc"),IsChecked = loaderini.ChunkSpecFix},
				new PatchesData() { Name = Lang.GetString("GamePatches.E102Lamp"),
					Description = Lang.GetString("GamePatches.E102LampDesc"),IsChecked = loaderini.E102PolyFix},
				new PatchesData() { Name = Lang.GetString("GamePatches.ChaoStats"), IsChecked = loaderini.ChaoPanelFix},
				new PatchesData() { Name = Lang.GetString("GamePatches.PixelOffset"),
					Description = Lang.GetString("GamePatches.PixelOffsetDesc"),IsChecked = loaderini.PixelOffSetFix},
				new PatchesData() { Name = Lang.GetString("GamePatches.Lights"),
					Description = Lang.GetString("GamePatches.LightsDesc"),IsChecked = loaderini.LightFix},
				new PatchesData() { Name = Lang.GetString("GamePatches.DisableGBIX"),
					Description = Lang.GetString("GamePatches.DisableGBIXDesc"),IsChecked = loaderini.KillGbix},
				new PatchesData() { Name = Lang.GetString("GamePatches.DisableCDCheck"), IsChecked = loaderini.DisableCDCheck},
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

			List<KeyValuePair<string, ModInfo>> list = listMods.SelectedItems
			.OfType<ModData>()
			.Select(mod => new KeyValuePair<string, ModInfo>(mod.Tag, mods[mod.Tag]))
			.ToList();

			updateChecker?.RunWorkerAsync(list);
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
			if (sender is not BackgroundWorker worker)
			{
				throw new Exception("what");
			}

			Dispatcher.Invoke(() =>
			{
				btnCheckUpdates.IsEnabled = false;
				ModContextChkUpdate.IsEnabled = false;
				ModContextDeleteMod.IsEnabled = false;
				if (ModContextDev is not null)
					ModContextDev.IsEnabled = false;
				ModContextForceModUpdate.IsEnabled = false;
				ModContextVerifyIntegrity.IsEnabled = false;
			});

			var updatableMods = e.Argument as List<KeyValuePair<string, ModInfo>>;
			List<ModDownloadWPF> updates = null;
			List<string> errors = null;

			var tokenSource = new CancellationTokenSource();
			CancellationToken token = tokenSource.Token;

			var task = Task.Run(() => modUpdater.GetModUpdates(modDirectory, updatableMods, out updates, out errors, token), token);

			while (!task.IsCompleted && !task.IsCanceled)
			{
				// Process pending UI events
				Application.Current.Dispatcher.Invoke(DispatcherPriority.Background, new Action(delegate { }));

				if (worker.CancellationPending)
				{
					tokenSource.Cancel();
				}
			}

			task.Wait(token);

			e.Result = new Tuple<List<ModDownloadWPF>, List<string>>(updates, errors);
		}

		private void UpdateChecker_DoWorkForced(object sender, DoWorkEventArgs e)
		{
			if (sender is not BackgroundWorker worker)
			{
				throw new Exception("what");
			}

			if (!(e.Argument is List<Tuple<string, ModInfo, List<Updater.ModManifestDiff>>> updatableMods) || updatableMods.Count == 0)
			{
				return;
			}

			var updates = new List<ModDownloadWPF>();
			var errors = new List<string>();

			using (var client = new UpdaterWebClient())
			{
				foreach (Tuple<string, ModInfo, List<Updater.ModManifestDiff>> info in updatableMods)
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

						ModDownloadWPF d = modUpdater.GetGitHubReleases(mod, modDirectory, info.Item1, client, errors);
						if (d != null)
						{
							updates.Add(d);
						}
					}
					else if (!string.IsNullOrEmpty(mod.GameBananaItemType) && mod.GameBananaItemId.HasValue)
					{
						ModDownloadWPF d = modUpdater.GetGameBananaReleases(mod, modDirectory, info.Item1, errors);
						if (d != null)
						{
							updates.Add(d);
						}
					}
					else if (!string.IsNullOrEmpty(mod.UpdateUrl))
					{
						List<Updater.ModManifestEntry> localManifest = info.Item3
							.Where(x => x.State == Updater.ModManifestState.Unchanged)
							.Select(x => x.Current)
							.ToList();

						ModDownloadWPF d = modUpdater.CheckModularVersion(mod, modDirectory, info.Item1, localManifest, client, errors);
						if (d != null)
						{
							updates.Add(d);
						}
					}
				}
			}

			e.Result = new Tuple<List<ModDownloadWPF>, List<string>>(updates, errors);
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

			if (e.Result is not Tuple<List<ModDownloadWPF>, List<string>> data)
			{
				return;
			}

			List<string> Errors = data.Item2;
			if (Errors.Count != 0)
			{
				string msgError = Lang.GetString("MessageWindow.Errors.CheckUpdateError");
				string title = Lang.GetString("MessageWindow.Errors.CheckUpdateError.Title");

				if (Errors.Contains("403"))
				{
					title = "GitHub Rate Limit Exceeded";
				}

				foreach (var error in Errors) 
				{
					msgError += "\n" + error;
				}
				
				new MessageWindow(title, msgError, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
			}

			bool manual = manualModUpdate;
			manualModUpdate = false;

			List<ModDownloadWPF> updates = data.Item1;
			if (updates.Count == 0)
			{
				if (manual)
				{
					new MessageWindow(Lang.GetString("MessageWindow.Information.NoAvailableUpdates.Title"), Lang.GetString("MessageWindow.Information.NoAvailableUpdates"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Information, MessageWindow.Buttons.OK).ShowDialog();
				}
				return;
			}

			var dialog = new ModChangelog(updates);
			dialog.ShowDialog();

			bool? res = dialog.DialogResult;
			if (res != true)
			{
				return;
			}

			updates = dialog.SelectedMods;


			if (updates.Count == 0)
			{
				return;
			}

			try
			{
				if (!Directory.Exists(updatePath))
				{
					Directory.CreateDirectory(updatePath);
				}
			}
			catch (Exception ex)
			{

			}


			new Updater.ModDownloadDialogWPF(updates, updatePath).ShowDialog();


			Refresh();
		}

		private void UpdateChecker_EnableControls(object sender, RunWorkerCompletedEventArgs runWorkerCompletedEventArgs)
		{
			btnCheckUpdates.IsEnabled = true;
			btnCheckUpdates.IsEnabled = true;
			ModContextChkUpdate.IsEnabled = true;
			if (ModContextDev is not null)
			{
				ModContextDev.IsEnabled = true;
			}

			ModContextDeleteMod.IsEnabled = true;
			ModContextForceModUpdate.IsEnabled = true;
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

		#region Direct3D wrapper

		private void SetUp_UpdateD3D9()
		{
			btnUpdateD3D9.IsEnabled = CheckD3D8to9Update();
			checkD3D9.IsEnabled = File.Exists(d3d8to9StoredDLLName);
			checkD3D9.IsChecked = File.Exists(d3d8to9InstalledDLLName);
		}

		private void CopyD3D9Dll()
		{
			try
			{
				File.Copy(d3d8to9StoredDLLName, d3d8to9InstalledDLLName, true);
			}
			catch (Exception ex)
			{
				string error = Lang.GetString("MessageWindow.Errors.D3D8Update") + "\n" + ex.Message;
				new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle"), error, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
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
				string error = Lang.GetString("MessageWindow.Errors.D3D8UpdateCheck") + "\n" + ex.Message;
				new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle"), error, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error).ShowDialog();
				return false;
			}
		}

		private void btnUpdateD3D9_Click(object sender, RoutedEventArgs e)
		{
			string info = Lang.GetString("MessageWindow.Information.D3D8Update");
			var msg = new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle"), Lang.GetString(info), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Information, MessageWindow.Buttons.YesNo);
			msg.ShowDialog();

			if (msg.isYes)
			{
				CopyD3D9Dll();
				btnUpdateD3D9.IsEnabled = CheckD3D8to9Update();
			}
		}

		private void checkD3D9_Click(object sender, RoutedEventArgs e)
		{
			if (checkD3D9.IsChecked == true)
			{
				CopyD3D9Dll();
			}
			else if (checkD3D9.IsChecked == false && File.Exists(d3d8to9InstalledDLLName))
				File.Delete(d3d8to9InstalledDLLName);

		}

		#endregion

		public void FilterMods(string text)
		{
			ViewModel.ModsSearch.Clear();

			foreach (var mod in ViewModel.Modsdata)
			{
				if (mod.Name.ToLowerInvariant().Contains(text) || mod.Author is not null && mod.Author.ToLowerInvariant().Contains(text))
				{
					ViewModel.ModsSearch.Add(mod); // Add filtered items to the ModsSearch collection.
				}
			}

			string path = BindingOperations.GetBinding(listMods, ListView.ItemsSourceProperty).Path.Path;
			string newPath = text.Length == 0 ? "Modsdata" : "ModsSearch";

			if (path != newPath)
			{
				Binding binding = new()
				{
					Path = new PropertyPath(newPath)
				};

				listMods.SetBinding(ListView.ItemsSourceProperty, binding);
				listMods.SetValue(GongSolutions.Wpf.DragDrop.DragDrop.IsDragSourceProperty, text.Length == 0 ? true : false);
				listMods.SetValue(GongSolutions.Wpf.DragDrop.DragDrop.IsDropTargetProperty, text.Length == 0 ? true : false);
			}
		}

		#region Keyboard Shortcut

		private void ModsList_OnPreviewKeyDown(object sender, KeyEventArgs e)
		{
			if (listMods == null)
				return;

			ModData mod = (ModData)listMods.SelectedItem;

			if (mod == null)
				return;

			var ctrlKey = Keyboard.IsKeyDown(Key.LeftCtrl) || Keyboard.IsKeyDown(Key.RightCtrl);

			if (Keyboard.IsKeyDown(Key.Space))
			{
				listMods.BeginInit();
				mod.IsChecked = !mod.IsChecked;
				listMods.EndInit();
			}
			else if (ctrlKey)
			{

				if (Keyboard.IsKeyDown(Key.E))
					ModContextEditMod_Click(null, null);
				else if (Keyboard.IsKeyDown(Key.O))
					ModContextOpenFolder_Click(null, null);
				else if (Keyboard.IsKeyDown(Key.C))
					ModContextConfigureMod_Click(null, null);
				else if (Keyboard.IsKeyDown(Key.U))
					ModContextChkUpdate_Click(null, null);
				else if (Keyboard.IsKeyDown(Key.Enter))
					AboutBtn_Click(null, null);

				e.Handled = true;
			}
			else if (Keyboard.IsKeyDown(Key.Enter))
			{
				OpenAboutModWindow(mod);
				e.Handled = true;
			}


			if (Keyboard.IsKeyDown(Key.Delete))
			{
				ModContextDeleteMod_Click(null, null);
				e.Handled = true;
			}

		}

		private void CodesList_OnPreviewKeyDown(object sender, KeyEventArgs e)
		{
			var code = GetCodeFromView(sender);

			if (code == null)
				return;

			if (Keyboard.IsKeyDown(Key.Space))
			{
				CodeListView.BeginInit();
				code.IsChecked = !code.IsChecked;
				CodeListView.EndInit();
			}


			if (Keyboard.IsKeyDown(Key.Enter))
				OpenAboutCodeWindow(code.codes);
		}

		private void Window_PreviewKeyDown(object sender, KeyEventArgs e)
		{

			var ctrlkey = Keyboard.IsKeyDown(Key.LeftCtrl) || Keyboard.IsKeyDown(Key.RightCtrl);

			if (Keyboard.IsKeyDown(Key.F5))
			{
				Refresh();
			}
			else if (ctrlkey)
			{
				if (Keyboard.IsKeyDown(Key.F)) //enable or disable search feature
				{
					if (tcMain.SelectedItem == tabMain)
					{
						if (ModsFind.Visibility == Visibility.Visible)
						{
							ModsFind.Visibility = Visibility.Collapsed;
							FilterMods("");
						}
						else
						{
							ModsFind.Visibility = Visibility.Visible;
							FilterMods(TextBox_ModsSearch.Text.ToLowerInvariant());
							TextBox_ModsSearch.Focus();
						}

					}
					else if (tcMain.SelectedItem == tbCodes)
					{
						if (CodesFind.Visibility == Visibility.Visible)
						{
							CodesFind.Visibility = Visibility.Collapsed;
							FilterCodes("");
						}
						else
						{

							CodesFind.Visibility = Visibility.Visible;
							FilterCodes(TextBox_CodesSearch.Text.ToLowerInvariant());
							TextBox_CodesSearch.Focus();
						}
					}
				}
			}
			if (Keyboard.IsKeyDown(Key.Escape))
			{
				if (tcMain.SelectedItem == tabMain)
				{
					ModsFind.Visibility = Visibility.Collapsed;
					FilterMods("");
				}
				else if (tcMain.SelectedItem == tbCodes)
				{
					CodesFind.Visibility = Visibility.Collapsed;
					FilterCodes("");
				}
			}

		}

		#endregion

		#region SearchBar

		//more info in the keyboard shortcut functions above.
		private void TextBox_ModsSearch_LostFocus(object sender, RoutedEventArgs e)
		{
			// Close if focus is lost with no text
			if (TextBox_ModsSearch.Text.Length == 0)
				ModsFind.Visibility = Visibility.Collapsed;
		}


		private void TextBox_ModsSearch_TextChanged(object sender, TextChangedEventArgs e)
		{
			FilterMods(TextBox_ModsSearch.Text.ToLowerInvariant());
		}

		#endregion

		public void FilterCodes(string text)
		{
			CodeListView.Items.Clear();

			int enabledIndex = 0;

			foreach (CodeData item in codesSearch)
			{
				if (item.codes.Name.ToLowerInvariant().Contains(text) || (item.codes.Author != null && item.codes.Author.ToLowerInvariant().Contains(text)))
				{
					if (item.IsChecked)
						CodeListView.Items.Insert(enabledIndex++, item);
					else
						CodeListView.Items.Add(item);
				}
			}
		}

		private void TextBox_CodesSearch_TextChanged(object sender, TextChangedEventArgs e)
		{
			FilterCodes(TextBox_CodesSearch.Text.ToLowerInvariant());
		}

		private void TextBox_CodesSearch_LostFocus(object sender, RoutedEventArgs e)
		{
			if (TextBox_CodesSearch.Text.Length == 0)
				CodesFind.Visibility = Visibility.Collapsed;
		}


		private PatchesData GetPatchFromView(object sender)
		{
			if (sender is ListViewItem lvItem)
				return lvItem.Content as PatchesData;
			else if (sender is ListView lv)
				return lv.SelectedItem as PatchesData;


			return listPatches.Items[listPatches.SelectedIndex] as PatchesData;
		}

		private void PatchViewItem_MouseEnter(object sender, MouseEventArgs e)
		{

			var patch = GetPatchFromView(sender);

			if (patch is null)
				return;

			PatchDescription.Text += " " + patch.Description;
		}

		private void PatchViewItem_MouseLeave(object sender, MouseEventArgs e)
		{
			PatchDescription.Text = Lang.GetString("CommonStrings.Description");
		}

		private void modChecked_Click(object sender, RoutedEventArgs e)
		{
			if (suppressEvent)
				return;

			codes = new List<Code>(mainCodes.Codes);
			codesSearch = new();
			string modDir = Path.Combine(gamePath, "mods");
			List<string> modlistCopy = new();

			//backup mod list here
			foreach (ModData mod in listMods.Items)
			{
				if (mod?.IsChecked == true)
				{
					modlistCopy.Add(mod.Tag);
				}
			}

			if (sender is CheckBox check)
			{
				var curItem = check.DataContext as ModData;
				var index = listMods.Items.IndexOf(curItem);

				var curModCopy = listMods.Items[index] as ModData;
				if (check.IsChecked == false)
				{
					modlistCopy.Remove((string)curModCopy.Tag);
				}
			}

			foreach (string mod in modlistCopy)
			{
				if (mods.TryGetValue(mod, out SADXModInfo value))
				{
					SADXModInfo inf = value;
					if (!string.IsNullOrEmpty(inf.Codes))
						codes.AddRange(CodeList.Load(Path.Combine(Path.Combine(modDir, mod), inf.Codes)).Codes);
				}
			}


			loaderini.EnabledCodes = new List<string>(loaderini.EnabledCodes.Where(a => codes.Any(c => c.Name == a)));

			foreach (Code item in codes.Where(a => a.Required && !loaderini.EnabledCodes.Contains(a.Name)))
				loaderini.EnabledCodes.Add(item.Name);

			CodeListView.BeginInit();
			CodeListView.Items.Clear();

			foreach (Code item in codes)
			{
				CodeData extraItem = new()
				{
					codes = item,
					IsChecked = loaderini.EnabledCodes.Contains(item.Name),
				};

				CodeListView.Items.Add(extraItem);
				codesSearch.Add(extraItem);
			}

			CodeListView.EndInit();
		}

		private void txtCustomResY_TextChanged(object sender, TextChangedEventArgs e)
		{
			NumericUpDown textBox = (NumericUpDown)sender;

			if ((bool)chkMaintainRatio.IsChecked)
			{
				double ratio = txtResX.Value / txtResY.Value;
				txtCustomResX.Value = Math.Ceiling(txtCustomResY.Value * ratio);
			}

			e.Handled = true;
		}

		#region App Launcher
		private void btnGetAppLauncher_Click(object sender, RoutedEventArgs e)
		{
			Uri uri = new("https://dcmods.unreliable.network/owncloud/data/PiKeyAr/files/Setup/data/AppLauncher.7z" + "\r\n");
			var DL = new GenericDownloadDialog(uri, "App Launcher", "AppLauncher.7z", true);
			DL.StartDL();
			DL.ShowDialog();

			if (DL.DialogResult == true)
			{
				try
				{
					using ArchiveFile archiveFile = new("AppLauncher.7z");
					archiveFile.Extract(Environment.CurrentDirectory);
					btnOpenAppLauncher.IsEnabled = true;
					btnOpenAppLauncher.Opacity = 1;
					btnGetAppLauncher.Opacity = LowOpacityBtn;
					btnGetAppLauncher.IsEnabled = false;
				}
				catch
				{
					throw new Exception("Failed to extract one mod.");
				}
			}

		}

		private void btnOpenAppLauncher_Click(object sender, RoutedEventArgs e)
		{
			if (File.Exists("AppLauncher.exe"))
			{
				Process.Start(new ProcessStartInfo { FileName = "AppLauncher.exe", UseShellExecute = true });
			}
		}

		private void UpdateAppLauncherBtn()
		{
			if (File.Exists("AppLauncher.7z") && File.Exists("AppLauncher.exe"))
			{
				try
				{
					File.Delete("AppLauncher.7z");
				}
				catch
				{ }
			}

			if (File.Exists("AppLauncher.exe"))
			{
				btnGetAppLauncher.IsEnabled = false;
				btnGetAppLauncher.Opacity = LowOpacityBtn;
			}
			else
			{
				btnOpenAppLauncher.IsEnabled = false;
				btnOpenAppLauncher.Opacity = LowOpacityBtn;
			}
		}

		#endregion

		#region One Click Install

		private void SetOneClickBtnState()
		{
			try
			{
				using var hkcr = Microsoft.Win32.Registry.ClassesRoot;
				var sammKey = hkcr.OpenSubKey("sadxmm");

				if (sammKey != null)
				{
					var pathManagerKey = sammKey.OpenSubKey("DefaultIcon");
					if (pathManagerKey != null)
					{
						var managerPath = pathManagerKey.GetValue("").ToString();
						
						if (managerPath.ToLower().Contains(Environment.ProcessPath.ToLower()))
						{
							btnOneClick.IsEnabled = false;
							Image iconConfig = FindName("GB") as Image;
							iconConfig?.SetValue(Image.OpacityProperty, LowOpacityIcon);
							btnOneClick.Opacity = LowOpacityBtn;
						}

						pathManagerKey.Close();
						sammKey.Close();
					}
				}
			}
			catch { }
		}

		private async void btnOneClick_Click(object sender, RoutedEventArgs e)
		{
			try
			{
				string execPath = Environment.ProcessPath;

				await Process.Start(new ProcessStartInfo(execPath, "urlhandler")
				{
					UseShellExecute = true,
					Verb = "runas"
				}).WaitForExitAsync();

				btnOneClick.IsEnabled = false;
				Image iconConfig = FindName("GB") as Image;
				iconConfig?.SetValue(Image.OpacityProperty, LowOpacityIcon);
				btnOneClick.Opacity = LowOpacityBtn;

			}
			catch { }
		}

		#endregion
	}

}


