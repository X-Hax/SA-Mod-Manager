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

namespace ModManagerWPF
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
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
		bool installed = false;

		public static LangEntry CurrentLang = new();
		public static LanguageList LangList = new();
		public static ThemeEntry CurrentTheme = new();
		public static ThemeList ThemeList = new();

		public MainWindow()
		{
			InitializeComponent();
			AddLanguagesToList();
			AddThemesToList();
			InitCodes();
		}

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
				MessageBox.Show(this, $"Error loading code list: {ex.Message}", "SADX Mod Loader");
				mainCodes = new CodeList();
			}

		}

		private void LoadCodes()
		{
			codes = new List<Code>(mainCodes.Codes);

			foreach (Code item in codes)
				CodeListView.Items.Add(item.Name);
		}

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

				comboLanguage.SelectedIndex = 0;
			}
		}

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

				comboThemes.SelectedIndex = 0;
			}
		}

		private void comboThemes_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			SwitchTheme(comboThemes.SelectedIndex);
		}

		private void OpenAboutCodeWindow(Code code)
		{
			new AboutCode(code).ShowDialog();
		}

		private Code GetCodeFromView(object sender)
		{
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
			
		}

		private void CodesView_Item_MouseLeave(object sender, MouseEventArgs e)
		{

		}

		private void CodesView_Item_Selected(object sender, RoutedEventArgs e)
		{
			
		}
	}
}
