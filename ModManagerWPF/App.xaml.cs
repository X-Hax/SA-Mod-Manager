using Microsoft.Win32;
using ModManagerCommon;
using ModManagerCommon.Forms;
using ModManagerWPF.Languages;
using ModManagerWPF.Themes;
using System;
using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Threading;
using System.Threading.Tasks;
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

			//Remove current resource from the merged dictionaries
			Current.Resources.MergedDictionaries.RemoveAt(3);
			Current.Resources.MergedDictionaries.Add(dictionary);
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

