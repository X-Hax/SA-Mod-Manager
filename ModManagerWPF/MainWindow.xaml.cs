using ModManagerWPF.Languages;
using ModManagerWPF.Themes;
using System;
using System.Linq;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using System.Xml.Linq;

namespace ModManagerWPF
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		public static LangEntry CurrentLang = new();
		public static LanguageList LangList = new();
		public static ThemeEntry CurrentTheme = new();
		public static ThemeList ThemeList = new();

		public MainWindow()
		{
			InitializeComponent();
			AddLanguagesToList();
			AddThemesToList();

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
	}
}
