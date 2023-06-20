using ModManagerCommon;
using ModManagerWPF.Languages;
using ModManagerWPF.Themes;
using System;
using System.Collections.Generic;
using System.IO.Pipes;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Linq;
using System.Windows.Threading;

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

		[STAThread]
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

		public async Task ExecuteDependenciesCheck()
		{
			Startup startup = new Startup();
			await startup.StartupCheck();
		}

		private void checkUrlhandlerArg(string[] args)
		{
			if (args.Length > 0 && args[0] == "urlhandler")
			{
				using (var hkcr = Microsoft.Win32.Registry.ClassesRoot)
				using (var key = hkcr.CreateSubKey("sadxmm"))
				{
					key.SetValue(null, "URL:SADX Mod Manager Protocol");
					key.SetValue("URL Protocol", string.Empty);
					using (var k2 = key.CreateSubKey("DefaultIcon"))
						k2.SetValue(null, System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName + ",1");
					using (var k3 = key.CreateSubKey("shell"))
					using (var k4 = k3.CreateSubKey("open"))
					using (var k5 = k4.CreateSubKey("command"))
						k5.SetValue(null, $"\"{System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName}\" \"%1\"");
				}
				return;
			}
		}

		private void InitUri(string[] args, bool alreadyRunning)
		{
			if (!alreadyRunning)
			{
				UriQueue = new UriQueue(pipeName);
			}	
		
			List<string> uris = args.Where(x => x.Length > protocol.Length && x.StartsWith(protocol, StringComparison.Ordinal)).ToList();

			if (uris.Count > 0)
			{
				using (var pipe = new NamedPipeClientStream(".", pipeName, PipeDirection.Out))
				{
					pipe.Connect();

					var writer = new StreamWriter(pipe);
					foreach (string s in uris)
					{
						writer.WriteLine(s);
					}
					writer.Flush();
				}
			}
		}

		public static void DoEvents()
		{
			Current.Dispatcher.Invoke(DispatcherPriority.Background, new Action(delegate { }));
		}

		private bool DoUpdate(string[] args, bool alreadyRunning)
		{
			if (args.Length > 1 && args[0] == "doupdate")
			{
				if (alreadyRunning)
					try { mutex.WaitOne(); }
					catch (AbandonedMutexException) { }


				return true;
			}

			return false;
		}


		protected override void OnStartup(StartupEventArgs e)
		{

			bool alreadyRunning;
			try { alreadyRunning = !mutex.WaitOne(0, true); }
			catch (AbandonedMutexException) { alreadyRunning = false; }

			string[] args = Environment.GetCommandLineArgs();

			if (DoUpdate(args, alreadyRunning))
			{
				return;
			}

			if (args.Length > 1 && args[0] == "cleanupdate")
			{
				if (alreadyRunning)
					try { mutex.WaitOne(); }
					catch (AbandonedMutexException) { }

				alreadyRunning = false;
			}

			SetupLanguages();
			SetupThemes();
			ShutdownMode = ShutdownMode.OnMainWindowClose;
			ExecuteDependenciesCheck();

			checkUrlhandlerArg(args);
			InitUri(args, alreadyRunning);

			if (alreadyRunning)
			{
				Current.Shutdown();
				return;
			}

			MainWindow = new MainWindow(args is not null ? args : null);
			MainWindow.Show();
			base.OnStartup(e);
			UriQueue.Close();
		}
	}
}

