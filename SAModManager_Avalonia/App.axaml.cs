using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Data.Core;
using Avalonia.Data.Core.Plugins;
using Avalonia.Markup.Xaml;
using SAModManager_Avalonia.ViewModels;
using SAModManager_Avalonia.UI.Windows;
using System.IO;
using System.Reflection;
using System;

namespace SAModManager_Avalonia
{
    public partial class App : Application
    {
        private const string pipeName = "sa-mod-manager";
        public static Version Version = Assembly.GetExecutingAssembly().GetName().Version;
        public static string VersionString = $"{Version.Major}.{Version.Minor}.{Version.Revision}";
        public static readonly string StartDirectory = AppDomain.CurrentDomain.BaseDirectory;
        public static string ConfigFolder = Directory.Exists(Path.Combine(StartDirectory, "SAManager")) ? Path.Combine(StartDirectory, "SAManager") : Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "SAManager");
        public static string extLibPath = Path.Combine(ConfigFolder, "extlib");
        public static readonly string tempFolder = Path.Combine(StartDirectory, "SATemp");
        public static string crashFolder = Path.Combine(ConfigFolder, "CrashDump");
        public static bool isLinux = false;
        public static bool isDebug = false;

        public override void Initialize()
        {
            AvaloniaXamlLoader.Load(this);
            Startup.StartupCheck();
        }

        public override void OnFrameworkInitializationCompleted()
        {
            if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
            {
                // Line below is needed to remove Avalonia data validation.
                // Without this line you will get duplicate validations from both Avalonia and CT
                BindingPlugins.DataValidators.RemoveAt(0);
                desktop.MainWindow = new MainWindow
                {
                    DataContext = new VMMainWindow(),
                };
            }

            base.OnFrameworkInitializationCompleted();
        }
    }
}