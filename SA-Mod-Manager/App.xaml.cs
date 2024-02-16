using SAModManager.Languages;
using SAModManager.Themes;
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
using SAModManager.Updater;
using SAModManager.Ini;
using System.Reflection;
using SAModManager.Configuration;
using System.Diagnostics;
using System.Security.Cryptography;
using SAModManager.Properties;
using System.Data;
using System.Net.Http;
using System.Net;
using SAModManager.UI;
using SAModManager.Controls.SADX;

namespace SAModManager
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    /// 

    public partial class App : Application
    {
        private const string pipeName = "sa-mod-manager";
        public static Version Version = Assembly.GetExecutingAssembly().GetName().Version;
        public static string VersionString = $"{Version.Major}.{Version.Minor}.{Version.Revision}";
        public static string StartDirectory = AppDomain.CurrentDomain.BaseDirectory;
        public static readonly string ConfigFolder = Directory.Exists(Path.Combine(StartDirectory, "SAManager")) ? Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "SAManager") : Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "SAManager");
        public static readonly string extLibPath = Path.Combine(ConfigFolder, "extlib");
        public static readonly string tempFolder = Path.Combine(StartDirectory, ".SATemp");
        public static readonly string crashFolder = Path.Combine(ConfigFolder, "CrashDump");
        public static bool isVanillaTransition = false; //used when installing the manager from an update
        public static bool isFirstBoot = false; //used when installing the new manager manually
        public static bool isLinux = false;

        public static string ManagerConfigFile = Path.Combine(ConfigFolder, "Manager.json");
        public static ManagerSettings ManagerSettings { get; set; }

        private static readonly Mutex mutex = new(true, pipeName);
        public static Updater.UriQueue UriQueue;
        public static string RepoCommit = SAModManager.Properties.Resources.Version.Trim();

        public static LangEntry CurrentLang { get; set; }
        public static LanguageList LangList { get; set; }

        public static ThemeEntry CurrentTheme { get; set; }
        public static bool IsLightTheme = false;
        public static ThemeList ThemeList { get; set; }
        public static Game CurrentGame = new();
        public static List<Game> GamesList = new();

        [STAThread]
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        /// 
        protected override async void OnStartup(StartupEventArgs e)
        {
            Application.Current.ShutdownMode = ShutdownMode.OnExplicitShutdown;

            string[] args = Environment.GetCommandLineArgs();

            if (CheckinstallURLHandler(args)) //we check if the program has been launched just to enable One Click Install
            {
                Application.Current.Shutdown(); //we don't want to continue if so
                return;
            }

            bool alreadyRunning;
            try { alreadyRunning = !mutex.WaitOne(0, true); }
            catch (AbandonedMutexException) { alreadyRunning = false; }

            if (await DoUpdate(args, alreadyRunning))
            {
                return;
            }

            UpdateHelper.InitHttpClient();
            Util.CheckLinux();
            HandleVanillaTransition(args);
            Steam.Init();
            SetupLanguages();
            SetupThemes();

            ManagerSettings = LoadManagerConfig();
            await ExecuteDependenciesCheck();

            await InitUriAsync(args, alreadyRunning);

            if (alreadyRunning)
            {
                Current.Shutdown();
                return;
            }

#if !DEBUG
            AppDomain.CurrentDomain.UnhandledException += (sender, e) =>
            {
                ExceptionHandler.UnhandledExceptionEventHandler(e.ExceptionObject as Exception);
            };
#endif

            ShutdownMode = ShutdownMode.OnMainWindowClose;

            MainWindow = new MainWindow();
            base.OnStartup(e);
            MainWindow.Show();

        }

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

        public static void SetLightBool()
        {
            switch (CurrentTheme.FileName)
            {
                case "LightTheme":
                    IsLightTheme = true;
                    break;
                default:
                    IsLightTheme = false;
                    break;
            }
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

            SetLightBool();
        }

        public static void SetupThemes()
        {
            var resource = Current.TryFindResource("Themes");

            if (resource is ThemeList themes)
                ThemeList = themes;
        }

        private static bool CheckinstallURLHandler(string[] args)
        {
            int index = 0;

            foreach (var arg in args)
            {
                if (arg == "urlhandler")
                {
                    if (index + 1 < args.Length)
                    {
                        using var hkcu = Microsoft.Win32.Registry.CurrentUser;

                        using var key = hkcu.CreateSubKey("Software\\Classes\\" + args[index + 1]);
                        key.SetValue(null, "URL:SA Mod Manager Protocol");
                        key.SetValue("URL Protocol", string.Empty);
                        using (var k2 = key.CreateSubKey("DefaultIcon"))
                            k2.SetValue(null, Environment.ProcessPath + ",1");
                        using var k3 = key.CreateSubKey("shell");
                        using var k4 = k3.CreateSubKey("open");
                        using var k5 = k4.CreateSubKey("command");
                        k5.SetValue(null, $"\"{Environment.ProcessPath}\" \"%1\"");
                        key.Close();
                        return true;
                    }
                }
                index++;
            }

            return false;
        }

        private async Task InitUriAsync(string[] args, bool alreadyRunning)
        {
            if (!alreadyRunning)
            {
                UriQueue = new UriQueue(pipeName);
            }

            foreach (var game in GamesInstall.GetSupportedGames())
            {

                List<string> uris = args
                .Where(x => x.Length > game?.oneClickName.Length && x.StartsWith(game?.oneClickName + ":", StringComparison.Ordinal))
                .ToList();

                if (uris.Count > 0)
                {
                    if (game != App.CurrentGame)
                    {
                        App.CurrentGame = game;
                    }

                    using (var pipe = new NamedPipeClientStream(".", pipeName, PipeDirection.Out))
                    {
                        try
                        {
                            await pipe.ConnectAsync();

                            using (var writer = new StreamWriter(pipe))
                            {
                                foreach (string s in uris)
                                {
                                    await writer.WriteLineAsync(s);
                                }
                                await writer.FlushAsync();
                            }
                        }
                        catch (Exception ex)
                        {
                            // Handle connection or writing errors here
                            Console.WriteLine($"Error sending URIs: {ex.Message}");
                        }
                    }
                }
            }
        }

        //left over from previous method to update, maybe will be re used for a dev purpose.
        /* private async static void GetArtifact()
         {
             GitHubAction latestAction = await GitHub.GetLatestAction();
             GitHubArtifact info = null;

             if (latestAction != null)
             {
                 List<GitHubArtifact> artifacts = await GitHub.GetArtifactsForAction(latestAction.Id);

                 if (artifacts != null)
                 {
                     bool is64BitSystem = Environment.Is64BitOperatingSystem;
                     string targetArchitecture = is64BitSystem ? "x64" : "x86";

                     info = artifacts.FirstOrDefault(t => t.Expired == false && t.Name.Contains("Release-" + targetArchitecture));

                     // If there's no specific architecture match, try to get a generic "Release" artifact
                     info ??= artifacts.FirstOrDefault(t => t.Expired == false && t.Name.Contains("Release"));
                 }
             }
         }*/

        public static async Task<bool> PerformUpdateManagerCheck()
        {
            var mainWindow = ((MainWindow)Application.Current.MainWindow);

            mainWindow.UpdateManagerStatusText(Lang.GetString("UpdateStatus.ChkManagerUpdate"));

            try
            {
                var update = await GitHub.GetLatestManagerRelease();

                if (update.Item1 == false) //no update found
                {
                    return false;
                }

                string changelog = await GitHub.GetGitChangeLog(update.Item2);

                if (string.IsNullOrEmpty(changelog)) //update found but no changelog (?)
                {
                    return false;
                }

                var manager = new InfoManagerUpdate(changelog);
                manager.ShowDialog();

                if (manager.DialogResult != true)
                    return false;

                // string dlLink = string.Format(SAModManager.Properties.Resources.URL_SAMM_UPDATE, update.Item2.CheckSuiteID, update.Item3.Id);
                string dlLink = update.Item3.DownloadUrl;
                string fileName = update.Item3.Name;
                string destFolder = App.tempFolder;
                Directory.CreateDirectory(destFolder);

                var dl = new ManagerUpdate(dlLink, destFolder, fileName)
                {
                    DownloadCompleted = async () => await ManagerUpdate.DownloadManagerCompleted(destFolder, fileName)
                };

                dl.StartManagerDL();
                return true;
            }
            catch
            {
                mainWindow.UpdateManagerStatusText(Lang.GetString("UpdateStatus.ChkManagerUpdateFail"));
                return false;
            }
        }

        private static async Task<(bool, string)> CheckLoaderUpdate()
        {
            var lastCommit = await GitHub.GetLoaderHashCommit();

            if (lastCommit is null)
                return (false, null);

            string loaderVersion = string.Empty;
            string loaderversionPath = App.CurrentGame.loader.loaderVersionpath;
            if (File.Exists(loaderversionPath))
            {
                loaderVersion = File.ReadAllText(loaderversionPath);
            }

            return (loaderVersion != lastCommit, lastCommit);
        }

        public static async Task<bool> PerformUpdateLoaderCheck()
        {
            try
            {
                var mainWindow = ((MainWindow)Application.Current.MainWindow);
                mainWindow.UpdateManagerStatusText(Lang.GetString("UpdateStatus.ChkLoaderUpdate"));
                var update = await CheckLoaderUpdate();

                if (update.Item1 == false) //no update found
                {
                    return false;
                }

                string changelog = await GitHub.GetGitLoaderChangeLog(update.Item2); //item2 is commit hash

                if (string.IsNullOrEmpty(changelog)) //if string is null, we got error(s) so the DL can't continue
                {
                    return false;
                }

                var manager = new InfoManagerUpdate(changelog, App.CurrentGame.loader.name);
                manager.ShowDialog();

                if (manager.DialogResult != true)
                    return false;

                if (await GamesInstall.UpdateLoader(App.CurrentGame))
                {
                    File.WriteAllText(App.CurrentGame.loader.loaderVersionpath, update.Item2);
                    await GamesInstall.UpdateDependencies(App.CurrentGame);
                    return true;
                }
            }
            catch
            { }

            return false;
        }

        public static async Task<bool> PerformUpdateCodesCheck()
        {
            try
            {
                ((MainWindow)Application.Current.MainWindow).UpdateManagerStatusText(Lang.GetString("UpdateStatus.ChkCodesUpdates"));

                var codesPath = Path.Combine(App.CurrentGame.modDirectory, "Codes.lst");

                bool CodeExist = File.Exists(codesPath);

                if (CodeExist)
                {
                    string localCodes = File.ReadAllText(codesPath);
                    var httpClient = UpdateHelper.HttpClient;

                    string repoCodes = await httpClient.GetStringAsync(App.CurrentGame.codeURL + $"?t={DateTime.Now:yyyyMMddHHmmss}");

                    if (localCodes == repoCodes)
                    {
                        return false;
                    }
                }

                await GamesInstall.UpdateCodes(App.CurrentGame); //update codes
                return true;
            }
            catch
            {

                ((MainWindow)Application.Current.MainWindow).UpdateManagerStatusText(Lang.GetString("UpdateStatus.FailedUpdateCodes"));

            }

            return false;
        }

        public static async Task<bool> PerformUpdatePatchesCheck()
        {
            try
            {
                ((MainWindow)Application.Current.MainWindow).UpdateManagerStatusText(Lang.GetString("UpdateStatus.ChkPatchesUpdates"));

                var jsonPath = Path.Combine(App.CurrentGame.modDirectory, "Patches.json");

                if (File.Exists(jsonPath))
                {
                    string localCodes = File.ReadAllText(jsonPath);
                    var httpClient = UpdateHelper.HttpClient;

                    string repoCodes = await httpClient.GetStringAsync(App.CurrentGame.patchURL + $"?t={DateTime.Now:yyyyMMddHHmmss}");

                    if (localCodes == repoCodes)
                    {
                        return false;
                    }
                }

                await GamesInstall.UpdatePatches(App.CurrentGame); //update patch
                return true;
            }
            catch
            {

                ((MainWindow)Application.Current.MainWindow).UpdateManagerStatusText(Lang.GetString("UpdateStatus.FailedUpdatePatches"));

            }

            return false;
        }

        public static async Task<bool> PerformUpdateAppLauncherCheck()
        {
            try
            {

                var path = Path.Combine(App.CurrentGame.gameDirectory, "AppLauncher.crc");

                if (File.Exists(path))
                {
                    string local = File.ReadAllText(path);
                    var httpClient = UpdateHelper.HttpClient;

                    string repo = await httpClient.GetStringAsync("https://dcmods.unreliable.network/owncloud/data/PiKeyAr/files/AppLauncher/AppLauncher.crc");

                    if (local == repo)
                    {
                        return false;
                    }
                }

                await GameConfig.UpdateAppLauncher();
                return true;
            }
            catch
            {


            }

            return false;
        }

        private static async Task<bool> DoUpdate(string[] args, bool alreadyRunning)
        {
            foreach (var arg in args)
            {
                if (arg == "doupdate")
                {
                    if (alreadyRunning)
                        try { mutex.WaitOne(); }
                        catch (AbandonedMutexException) { }


                    var dialog = new InstallManagerUpdate(args[2], args[3]);
                    await dialog.InstallUpdate();
                    Util.ClearTempFolder();
                    Application.Current.Shutdown();

                    return true;
                }

            }

            return false;
        }

        public static void CreateConfigFolder()
        {
            try
            {
                if (!Directory.Exists(App.ConfigFolder))
                    Directory.CreateDirectory(App.ConfigFolder);
            }
            catch
            {
                new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), string.Format(Lang.GetString("MessageWindow.Errors.CreateConfigFolder"), "'" + App.ConfigFolder + "'"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error).ShowDialog();
            }
        }

        private static void HandleVanillaTransition(string[] args)
        {

            int index = 0;
            foreach (var arg in args)
            {
                if (arg == "vanillaUpdate")
                {
                    isVanillaTransition = true;
                    isFirstBoot = true;
                    Util.DoVanillaFilesCleanup(args, index);
                }

                index++;
            }
        }
        public static Uri GetResourceUri(string resourceName)
        {
            // Get the assembly where the resource is located
            Assembly assembly = Assembly.GetExecutingAssembly();

            // Construct the full resource name
            string fullResourceName = assembly.GetName().Name + ".Resources." + resourceName;

            // Load the resource stream from the assembly
            using (Stream stream = assembly.GetManifestResourceStream(fullResourceName))
            {
                // Check if the resource stream is found
                if (stream == null)
                {
                    Console.WriteLine($"Resource not found: {fullResourceName}");
                    return null;
                }

                // Copy the resource stream to a temporary file
                string tempFilePath = Path.GetTempFileName();
                using (FileStream fileStream = File.Create(tempFilePath))
                {
                    stream.CopyTo(fileStream);
                }

                // Return the URI to the temporary file
                return new Uri($"file://{tempFilePath}");
            }
        }

        public static async Task<bool> ExecuteDependenciesCheck()
        {
            return await SAModManager.Startup.StartupCheck();
        }

        private ManagerSettings LoadManagerConfig()
        {
            ManagerSettings settings = ManagerSettings.Deserialize(Path.Combine(ConfigFolder, ManagerConfigFile));

            switch (settings.CurrentSetGame)
            {
                default:
                case (int)SetGame.SADX:
                    CurrentGame = GamesInstall.SonicAdventure;
                    break;
                case (int)SetGame.SA2:
                    CurrentGame = GamesInstall.SonicAdventure2;
                    break;
            }

            return settings;
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
}

