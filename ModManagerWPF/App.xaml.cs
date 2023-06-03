using ModManagerCommon;
using ModManagerWPF.Languages;
using ModManagerWPF.Themes;
using System;
using System.Threading;
using System.Windows;

namespace ModManagerWPF
{
	/// <summary>
	/// Interaction logic for App.xaml
	/// </summary>
	/// 

	public partial class App : Application
	{
		private const string pipeName = "sadx-mod-manager";
		private const string protocol = "sadxmm:";

		private static readonly Mutex mutex = new Mutex(true, pipeName);
		public static UriQueue UriQueue;

		public static LangEntry CurrentLang { get; set; }
		public static LanguageList LangList { get; set; }

		public static ThemeEntry CurrentTheme { get; set; }
		public static ThemeList ThemeList { get; set; }

		/// <summary>
		/// The main entry point for the application.
		/// </summary>

		public static void SetupLanguages()
		{
			var resource = Current.TryFindResource("Languages");

			if (resource is LanguageList langs)
			{
				LangList = langs;
			}
		}

		public static void SwitchLanguage()
		{
			if (LangList is null)
				return;

			string name = "Languages/" + CurrentLang.FileName + ".xaml";
			ResourceDictionary dictionary = new()
			{
				Source = new Uri(name, UriKind.Relative)
			};

			//if a language different than english is set, remove the previous one.
			if (Current.Resources.MergedDictionaries.Count >= 5)
			{
				Current.Resources.MergedDictionaries.RemoveAt(4); 
			}

			//if we go back to english, give up the process as it's always in the list.
			if (Current.Resources.MergedDictionaries[3].Source.ToString().Contains("en-EN") && name.Contains("en-EN"))
			{
				return;
			}

			//add new language
			Current.Resources.MergedDictionaries.Insert(4, dictionary);		
		}

		public static void SwitchTheme()
		{
			if (ThemeList is null)
				return;

			string name = "Themes/" + CurrentTheme.FileName + ".xaml";
			ResourceDictionary dictionary = new()
			{
				Source = new Uri(name, UriKind.Relative)
			};

			Current.Resources.MergedDictionaries.RemoveAt(1);
			Current.Resources.MergedDictionaries.Insert(1, dictionary);
		}

		public static void SetupThemes()
		{
			var resource = Current.TryFindResource("Themes");

			if (resource is ThemeList themes)
				ThemeList = themes;
		}

		protected override void OnStartup(StartupEventArgs e)
		{
			SetupLanguages();
			SetupThemes();
			ShutdownMode = ShutdownMode.OnMainWindowClose;
			MainWindow = new MainWindow();
			MainWindow.Show();
			base.OnStartup(e);
		}
	}
}

