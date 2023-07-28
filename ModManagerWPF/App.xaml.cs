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
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Navigation;
using System.Runtime.InteropServices.ComTypes;
using System.Text.RegularExpressions;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Input;
using System.Globalization;

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

		public async Task<bool> ExecuteDependenciesCheck()
		{
			Startup startup = new();
			return await startup.StartupCheck();
		}

		protected override async void OnStartup(StartupEventArgs e)
		{
			
			Application.Current.ShutdownMode = ShutdownMode.OnExplicitShutdown;
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
		
			if (await ExecuteDependenciesCheck() == false)
			{
				return;
			}

			checkUrlhandlerArg(args);
			InitUri(args, alreadyRunning);

			if (alreadyRunning)
			{
				Current.Shutdown();
				return;
			}

			ShutdownMode = ShutdownMode.OnMainWindowClose;
			MainWindow = new MainWindow(args is not null ? args : null);
			MainWindow.Show();
			base.OnStartup(e);
			UriQueue.Close();
		}

		private void MinimizeWindow(object sender, RoutedEventArgs e)
		{
			Window window = Window.GetWindow((DependencyObject)sender);
			window.WindowState = WindowState.Minimized;
		}

		private void MaximizeWindow(object sender, RoutedEventArgs e)
		{
			Window window = Window.GetWindow((DependencyObject)sender);
			window.WindowState = window.WindowState == WindowState.Maximized ? WindowState.Normal : WindowState.Maximized;
		}

		private void CloseWindow(object sender, RoutedEventArgs e)
		{
			Window window = Window.GetWindow((DependencyObject)sender);
			window.Close();
		}

		
	}

	public partial class ImageButton : Button
	{
		public enum DrawType
		{
			Path = 0,
			Fill = 1,
		}

		public Geometry Icon
		{
			get
			{
				return GetValue(IconProperty) as Geometry;
			}
			set
			{
				SetValue(IconProperty, value);
			}
		}

		public static readonly DependencyProperty IconProperty =
			DependencyProperty.Register("Icon", typeof(Geometry), typeof(ImageButton));

		public Brush ImageBrush
		{
			get
			{
				return GetValue(ImageBrushProperty) as Brush;
			}
			set
			{
				SetValue(ImageBrushProperty, value);
			}
		}

		public static readonly DependencyProperty ImageBrushProperty = 
			DependencyProperty.Register("ImageBrush", typeof(Brush), typeof(ImageButton));

		public Brush ImageBrushDisabled
		{
			get
			{
				return GetValue(ImageBrushDisabledProperty) as Brush;
			}
			set
			{
				SetValue(ImageBrushDisabledProperty, value);
			}
		}

		public static readonly DependencyProperty ImageBrushDisabledProperty = 
			DependencyProperty.Register("ImageBrushDisabled", typeof(Brush), typeof(ImageButton));

		public Brush ImageBrushHover
		{
			get
			{
				return GetValue(ImageBrushHoverProperty) as Brush;
			}
			set
			{
				SetValue(ImageBrushHoverProperty, value);
			}
		}

		public static readonly DependencyProperty ImageBrushHoverProperty = 
			DependencyProperty.Register("ImageBrushHover", typeof(Brush), typeof(ImageButton));

		public double IconThickness
		{
			get { return (double)GetValue(IconThicknessProperty); }
			set { SetValue(IconThicknessProperty, value); }
		}

		public static readonly DependencyProperty IconThicknessProperty =
			DependencyProperty.Register("IconThickness", typeof(double), typeof(ImageButton));

		public bool IconFill
		{
			get { return (bool)GetValue(IconFillProperty); }
			set { SetValue(IconFillProperty, value); }
		}

		public static readonly DependencyProperty IconFillProperty = 
			DependencyProperty.Register("IconFill", typeof(bool), typeof(ImageButton));
	}
}

