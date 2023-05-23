using ModManagerWPF.Languages;
using ModManagerWPF.Themes;
using System;
using System.Linq;
using System.Reflection;
using System.Drawing;
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
using MessageBox = Xceed.Wpf.Toolkit.MessageBox;

namespace ModManagerWPF
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		#region Variables
		private const string pipeName = "sadx-mod-manager";
		private static readonly Mutex mutex = new Mutex(true, pipeName);
		public readonly string titleName = "SADX Mod Manager";
		const string updatePath = "mods/.updates";
		const string datadllpath = "system/CHRMODELS.dll";
		const string datadllorigpath = "system/CHRMODELS_orig.dll";
		const string loaderinipath = "mods/SADXModLoader.ini";
		const string loaderdllpath = "mods/SADXModLoader.dll";
		SADXLoaderInfo loaderini;
		Dictionary<string, SADXModInfo>? mods = null;
		const string codelstpath = "mods/Codes.lst";
		const string codexmlpath = "mods/Codes.xml";
		const string codedatpath = "mods/Codes.dat";
		const string patchdatpath = "mods/Patches.dat";
		CodeList? mainCodes = null;
		List<Code>? codes = null;
		bool installed = false;
		bool suppressEvent = false;

		public static LangEntry CurrentLang = new();
		public static LanguageList LangList = new();
		public static ThemeEntry CurrentTheme = new();
		public static ThemeList ThemeList = new();
		public GameGraphics graphics;
		#endregion

	
		public MainWindow()
		{
			loaderini = File.Exists(loaderinipath) ? IniSerializer.Deserialize<SADXLoaderInfo>(loaderinipath) : new SADXLoaderInfo();
			InitializeComponent();
			AddLanguagesToList();
			AddThemesToList();
			graphics = new GameGraphics(comboScreen);
			LoadSettings();
			InitCodes();
		}

		#region Main

		private void Save()
		{
			loaderini.Mods.Clear();

			/*foreach (ListViewItem item in listMods.isch.CheckedItems)
			{
				loaderini.Mods.Add((string)item.Tag);
			}*/

			loaderini.HorizontalResolution = (int)txtResX.Value;
			loaderini.VerticalResolution = (int)txtResY.Value;
			loaderini.ForceAspectRatio = (bool)chkRatio.IsChecked;
			//loaderini.ScaleHud = chkhud.Checked;
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
			loaderini.ResizableWindow = (bool)chkResizableWin.IsChecked;
			loaderini.UpdateCheck = (bool)chkUpdatesML.IsChecked;
			loaderini.ModUpdateCheck = (bool)chkUpdatesMods.IsChecked;

			IniSerializer.Serialize(loaderini, loaderinipath);
		}

		private void SaveButton_Click(object sender, RoutedEventArgs e)
		{
			Save();
		}

		private void SaveAndPlayButton_Click(object sender, RoutedEventArgs e)
		{
			Save();

		}
		private void LoadModList()
		{
			//modTopButton.Enabled = modUpButton.Enabled = modDownButton.Enabled = modBottomButton.Enabled = configureModButton.Enabled = false;
			//modDescription.Text = "Description: No mod selected.";
			listMods.Items.Clear();
			mods = new Dictionary<string, SADXModInfo>();
			string modDir = Path.Combine(Environment.CurrentDirectory, "mods");

			if (!Directory.Exists(modDir)) 
			{ 
				Directory.CreateDirectory(modDir);
			}

			if (File.Exists(Path.Combine(modDir, "mod.ini")))
			{
				/*MessageBox.Show(this, "There is a mod.ini in the mods folder."
							+ "\n\nEach mod must be placed in a subfolder in the mods folder. Do not extract mods directly to the mods folder." +
							"\n\nMove or delete mod.ini in the mods folder and run the Mod Manager again.", "SADX Mod Manager Error", MessageBoxButtons.OK, MessageBoxIcon.Error);*/
				Close();
				return;
			}


			foreach (string filename in SADXModInfo.GetModFiles(new DirectoryInfo(modDir)))
			{
				SADXModInfo mod = IniSerializer.Deserialize<SADXModInfo>(filename);
				mods.Add((Path.GetDirectoryName(filename) ?? string.Empty).Substring(modDir.Length + 1), mod);
			}

			foreach (string mod in new List<string>(loaderini.Mods))
			{
				if (mods.ContainsKey(mod))
				{
					SADXModInfo inf = mods[mod];
					suppressEvent = true;
					//listMods.Items.Add(new ListViewItem(new[] { inf.Name, inf.Author, inf.Version, inf.Category }) { IsChecked = true, Tag = mod });
					suppressEvent = false;
				}
				else
				{
					//MessageBox.Show(this, "Mod \"" + mod + "\" could not be found.\n\nThis mod will be removed from the list.);
					loaderini.Mods.Remove(mod);
				}
			}

			foreach (KeyValuePair<string, SADXModInfo> inf in mods.OrderBy(x => x.Value.Name))
			{
				/*if (!loaderini.Mods.Contains(inf.Key))
					listMods.Items.Add(new ListViewItem(new[] { inf.Value.Name, inf.Value.Author, inf.Value.Version, inf.Value.Category }) { Tag = inf.Key });*/
			}
	
		}

		private void LoadSettings()
		{
			LoadModList();

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

			chkBorderless.IsChecked = loaderini.Borderless;
			checkMipmapping.IsChecked = loaderini.AutoMipmap;
			chkScaleScreen.IsChecked = loaderini.StretchFullscreen;

			int screenNum = graphics.GetScreenNum(loaderini.ScreenNum);

			comboScreen.SelectedIndex = screenNum;
			chkBorderless.IsChecked = txtCustomResY.IsEnabled = chkMaintainRatio.IsEnabled = loaderini.CustomWindowSize;
			txtCustomResX.IsEnabled = loaderini.CustomWindowSize && !loaderini.MaintainWindowAspectRatio;
			Rectangle rect = graphics.GetRectangleStruct();

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
				MessageBox.Show(this, $"Error loading code list: {ex.Message}");
				mainCodes = new CodeList();
			}
		}

		private void LoadCodes()
		{
			codes = new List<Code>(mainCodes.Codes);

			foreach (Code item in codes)
			{
				CodeListView.Items.Add(item.Name);
			}

			loaderini.EnabledCodes = new List<string>(loaderini.EnabledCodes.Where(a => codes.Any(c => c.Name == a)));
		}

		private void OpenAboutCodeWindow(Code code)
		{
			new AboutCode(code).ShowDialog();
		}

		private Code GetCodeFromView()
		{
			return codes[CodeListView.SelectedIndex];
		}
		private void CodesView_Item_MouseDoubleClick(object sender, MouseButtonEventArgs e)
		{
			var code = GetCodeFromView();

			if (code == null)
				return;

			OpenAboutCodeWindow(code);
		}

		private void CodesView_Item_MouseEnter(object sender, MouseEventArgs e)
		{

		}

		private void CodesView_Item_MouseLeave(object sender, MouseEventArgs e)
		{

		}

		private void CodesView_Item_Selected(object sender, RoutedEventArgs e)
		{

		}
		#endregion

		#region Languages
		private void comboLanguage_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			SwitchLanguage(comboLanguage.SelectedIndex);
		}


		private void SwitchLanguage(int index)
		{
			if (LangList is null)
				return;

			CurrentLang = LangList[index];

			string name = "Languages/" + CurrentLang.FileName + ".xaml";
			ResourceDictionary dictionary = new()
			{
				Source = new Uri(name, UriKind.Relative)
			};

			Resources.MergedDictionaries.Add(dictionary);
		}

		private void AddLanguagesToList()
		{
			var mergedDict = Application.Current.Resources.MergedDictionaries.Where(md => md.Source.OriginalString.Equals("Languages/languages.xaml")).FirstOrDefault();
			LangList = (LanguageList)mergedDict["Languages"];

			if (LangList is not null)
			{
				foreach (var lang in LangList)
				{
					comboLanguage.Items.Add(lang.Name);
				}
			}
		}
		#endregion

		#region Themes
		private void SwitchTheme(int index)
		{
			if (ThemeList is null)
				return;

			CurrentTheme = ThemeList[index];

			string name = "Themes/" + CurrentTheme.FileName + ".xaml";
			ResourceDictionary dictionary = new()
			{
				Source = new Uri(name, UriKind.Relative)
			};

			Resources.MergedDictionaries.Add(dictionary);
		}

		private void AddThemesToList()
		{
			var mergedDict = Application.Current.Resources.MergedDictionaries.Where(md => md.Source.OriginalString.Equals("Themes/Themes.xaml")).FirstOrDefault();
			ThemeList = (ThemeList)mergedDict["Themes"];

			if (ThemeList is not null)
			{
				foreach (var lang in ThemeList)
				{
					comboThemes.Items.Add(lang.Name);
				}
			}
		}

		private void comboThemes_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			SwitchTheme(comboThemes.SelectedIndex);
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
		#endregion
	}
}
