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
using SAModManager.Updater;
using System.Reflection;
using SAModManager.Configuration;
using System.Diagnostics;
using System.Data;
using SAModManager.UI;
using SAModManager.Controls.SADX;
using SAModManager.Profile;
using Newtonsoft.Json;
using static TheArtOfDev.HtmlRenderer.Adapters.RGraphicsPath;
using System.Net.Http;
using System.Text.RegularExpressions;
using System.Text;
using System.Collections.ObjectModel;
using SAModManager.Management;

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
		public static readonly string StartDirectory = AppDomain.CurrentDomain.BaseDirectory;
		public static string ConfigFolder = Directory.Exists(Path.Combine(StartDirectory, "SAManager")) ? Path.Combine(StartDirectory, "SAManager") : Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "SAManager");
		public static string oldExtLibPath = Path.Combine(ConfigFolder, "extlib"); //for migration
		public static string extLibPath = oldExtLibPath; //will be updated if a game is set as new path use mods folder
		public static readonly string tempFolder = Path.Combine(StartDirectory, "SATemp");
		public static string crashFolder = Path.Combine(ConfigFolder, "CrashDump");
		public static bool isVanillaTransition = false; //used when installing the manager from an update
		public static bool isFirstBoot = false; //used when installing the new manager manually
		public static bool isLinux = false;
		public static bool CancelUpdate = false;
		public static bool isDebug = false;

		public static string ManagerConfigFile = Path.Combine(ConfigFolder, "Manager.json");
		public static ManagerSettings ManagerSettings { get; set; }
		public static Profiles Profiles { get; set; } = new Profiles();

		private static readonly Mutex mutex = new(true, pipeName);
		public static Updater.UriQueue UriQueue;
		public static string RepoCommit = SAModManager.Properties.Resources.Version.Trim();

		public static LangEntry CurrentLang { get; set; }
		public static LanguageList LangList { get; set; }

		public static ThemeEntry CurrentTheme { get; set; }
		public static bool IsLightTheme = false;
		public static ThemeList ThemeList { get; set; }
		public static List<string> UpdateChannels { get; set; } = ["Release", "Development"];
		public static string CurrentChannel { get; set; } = string.IsNullOrEmpty(RepoCommit) ? UpdateChannels[0] : UpdateChannels[1];

		public static Game CurrentGame = new();
		public static ObservableCollection<Game> GamesList = new();

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

            foreach (var arg in args)
            {
                if (arg == "doupdate")
                {
                    if (alreadyRunning)
                        try { mutex.WaitOne(); }
                        catch (AbandonedMutexException) { }

                    Logger.Log("DoUpdate command started");
                    string pID = args.Length > 4 ? args[4] : null;
                    InstallUpdate(args[2], args[3], pID);
                    return;
                }

            }

            UpdateHelper.InitHttpClient();
            Util.CheckLinux();
            SetExeCommands(args);
            Steam.Init();
            SetupLanguages();
            SetupThemes();
            LoadManagerConfig();
            await SAModManager.Startup.StartupCheck();

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

        //todo delete on next update
        public static void RemoveExeFromPath()
        {
            if (Util.IsStringValid(App.CurrentGame?.gameDirectory))
            {
                if (App.CurrentGame.gameDirectory.Contains(".exe"))
                {
                    App.CurrentGame.gameDirectory = Path.GetDirectoryName(App.CurrentGame.gameDirectory);
                }
            }
        }
        public static void UpdateDependenciesLocation()
        {
            if (Directory.Exists(App.extLibPath) == false && Directory.Exists(App.oldExtLibPath))
            {
                try
                {
                    Util.CreateSafeDirectory(App.extLibPath);

                    foreach (string dirPath in Directory.GetDirectories(App.oldExtLibPath, "*", SearchOption.AllDirectories))
                    {
                        Util.CreateSafeDirectory(dirPath.Replace(App.oldExtLibPath, App.extLibPath));
                    }

                    foreach (string newPath in Directory.GetFiles(App.oldExtLibPath, "*.*", SearchOption.AllDirectories))
                    {
                        File.Copy(newPath, newPath.Replace(App.oldExtLibPath, App.extLibPath), true);
                    }
                }
                catch { }
            }
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

            try
            {
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
            catch (Exception ex)
            {
                var msg = new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), string.Format(Lang.GetString("MessageWindow.Errors.LanguageSwitch"), CurrentLang.FileName, ex.Message), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK);
                msg.ShowDialog();
                var mainWindow = ((MainWindow)Application.Current.MainWindow);
                if (mainWindow != null)
                {
                    mainWindow.comboLanguage.SelectedIndex = 0;
                }

            }

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


        public static async Task<(bool, WorkflowRunInfo, GitHubArtifact)> GetArtifact()
        {
     
            var workflowRun = await GitHub.GetLatestWorkflowRun();

            if (workflowRun is null)
                return (false, null, null);


            bool hasUpdate = RepoCommit != workflowRun.HeadSHA;
            
            if (hasUpdate == false)
                return (false, null, null);


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

            return (hasUpdate, workflowRun, info);
        }

        public static async Task<bool> PerformDevUpdateManagerCheck()
        {

            var update = await App.GetArtifact();
            if (update.Item2 is not null)
            {
                string changelog = await GitHub.GetGitChangeLog(update.Item2.HeadSHA);
                var manager = new InfoManagerUpdate(changelog, ".Dev");
                manager.ShowDialog();

                if (manager.DialogResult != true)
                    return false;

                Logger.Log("Now Installing Latest Dev Build Update ...");

                string dlLink = string.Format(SAModManager.Properties.Resources.URL_SAMM_UPDATE, update.Item2.CheckSuiteID, update.Item3.Id);
                string fileName = update.Item3.Name;
                string version = update.Item2.HeadSHA[..7];
                string destFolder = App.tempFolder;
                Util.CreateSafeDirectory(destFolder);

                var dl = new ManagerUpdate(dlLink, destFolder, fileName, version)
                {
                    DownloadCompleted = async () => await ManagerUpdate.DownloadManagerCompleted(destFolder, fileName)
                };

                dl.StartManagerDL();
                return true;
            }

            return false;
        }

        public static async Task<bool> PerformUpdateManagerCheck()
        {
            MainWindow mainWindow = null;
            try
            {
                mainWindow = ((MainWindow)Application.Current.MainWindow);
                mainWindow?.UpdateManagerStatusText(Lang.GetString("UpdateStatus.ChkManagerUpdate"));

                var update = await GitHub.GetLatestManagerRelease();

                if (update.Item1 == false) //no update found
                    return false;

                string changelog = await GitHub.GetGitChangeLog(update.Item2);

                if (!Util.IsStringValid(changelog)) //update found but no changelog (?)
                    return false;

                var manager = new InfoManagerUpdate(changelog, update.Item4);
                manager.ShowDialog();

                if (manager.DialogResult != true)
                    return false;

                Logger.Log("Now Installing New Manager Update...");
                // string dlLink = string.Format(SAModManager.Properties.Resources.URL_SAMM_UPDATE, update.Item2.CheckSuiteID, update.Item3.Id);
                string dlLink = update.Item3.DownloadUrl;
                string fileName = update.Item3.Name;
                string version = update.Item4;
                string destFolder = App.tempFolder;
                Util.CreateSafeDirectory(destFolder);

                var dl = new ManagerUpdate(dlLink, destFolder, fileName, version)
                {
                    DownloadCompleted = async () => await ManagerUpdate.DownloadManagerCompleted(destFolder, fileName)
                };

                dl.StartManagerDL();
                return true;
            }
            catch
            {
                mainWindow?.UpdateManagerStatusText(Lang.GetString("UpdateStatus.ChkManagerUpdateFail"));
                return false;
            }
        }

        private static async Task<(bool, string)> CheckLoaderUpdateOld()
        {
            var lastCommit = await GitHub.GetLoaderHashCommit();

            if (lastCommit is null)
                return (false, null);

            string loaderVersion = string.Empty;
            string loaderversionPath = App.CurrentGame.loader.mlverPath;
            if (File.Exists(loaderversionPath))
            {
                loaderVersion = File.ReadAllText(loaderversionPath);
            }

            return (loaderVersion != lastCommit, lastCommit);
        }

        private static async Task<(bool, GitHubAsset, string, StringBuilder)> CheckLoaderUpdate()
        {
            bool hasUpdate = false;
            StringBuilder changelog = new StringBuilder();
            try
            {
                string url_releases = App.CurrentGame.loader.URL;
                string text_releases = string.Empty;
                string assetName = App.CurrentGame.loader.name + ".7z";
                string mlverfile = App.CurrentGame.loader.mlverPath;
                string currentTagName = File.Exists(mlverfile) ? File.ReadAllText(mlverfile) : App.CurrentGame.loader.defaultReleaseID;

                if (!uint.TryParse(currentTagName, out uint currentID))
                    currentID = uint.Parse(App.CurrentGame.loader.defaultReleaseID);

                var httpClient = UpdateHelper.HttpClient;
                string apiUrl = App.CurrentGame.loader.URL;

                HttpResponseMessage response = await httpClient.GetAsync(apiUrl);

                if (response.IsSuccessStatusCode)
                {
                    string responseBody = await response.Content.ReadAsStringAsync();
                    var release = JsonConvert.DeserializeObject<GitHubRelease>(responseBody);
                    if (release != null && release.Assets != null)
                    {
                        var targetAsset = release.Assets.FirstOrDefault(asset => asset.Name.Equals(assetName, StringComparison.OrdinalIgnoreCase));
                        if (targetAsset != null)
                        {
                            if (uint.TryParse(release.TagName, out uint releaseID))
                            {
                                if (releaseID > currentID)
                                {
                                    hasUpdate = true;
                                    changelog.AppendLine(string.Format("Revision {0}\n{1}\n", release.TagName, release.Body));
                                }
                            }


                            return (hasUpdate, targetAsset, release.TagName, changelog);

                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error fetching latest release: " + ex.Message);
            }


            return (false, null, null, null);
        }

        //also check for code, patches, lib  etc.
        public static async Task PerformUpdateLoaderCheck()
        {
            try
            {
                var mainWindow = ((MainWindow)Application.Current.MainWindow);
                mainWindow.UpdateManagerStatusText(Lang.GetString("UpdateStatus.ChkLoaderUpdate"));

                var update = await CheckLoaderUpdate();

                if (update.Item1 == false) //no update found
                    return;

                string changeLog = update.Item4.ToString();

                if (!Util.IsStringValid(changeLog) || App.CancelUpdate) //if string is null, we got error(s) so the DL can't continue
                    return;


                var manager = new InfoManagerUpdate(changeLog, update.Item3, App.CurrentGame.loader.name);
                manager.ShowDialog();

                if (manager.DialogResult != true || App.CancelUpdate)
                    return;

                await GamesInstall.UpdateLoader(App.CurrentGame, update.Item2.DownloadUrl);
            }
            catch
            { }
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

                    await GameConfig.UpdateAppLauncher();
                }

                return true;
            }
            catch
            {


            }

            return false;
        }

        public static void InstallUpdate(string updatePath, string managerPath, string pID)
        {
            string executablePath = Environment.ProcessPath;
            Logger.Log("Now Replacing old exe...");

            try
            {

                //check if old Manager process is still enabled, if so wait that it's killed properly
                if (string.IsNullOrEmpty(pID) == false)
                {
                    int.TryParse(pID, out int pid);
                    var process = Process.GetProcessById(pid);
                    process.WaitForExit();
                }

            }
            catch { }

            //Finally, Copy and run the new Manager version and replace the old one
            File.Copy(Path.Combine(updatePath, executablePath), managerPath, true);
            Process.Start(new ProcessStartInfo { FileName = managerPath, UseShellExecute = true });
            Environment.Exit(0);
        }


        public static void CreateConfigFolder()
        {
            try
            {
                Util.CreateSafeDirectory(App.ConfigFolder);
            }
            catch
            {
                new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), string.Format(Lang.GetString("MessageWindow.Errors.CreateConfigFolder"), "'" + App.ConfigFolder + "'"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error).ShowDialog();
            }
        }

        private static void SetExeCommands(string[] args)
        {
            foreach (var arg in args)
            {
                if (arg == "vanillaUpdate")
                {
                    isVanillaTransition = true;
                    isFirstBoot = true;
                    Util.DoVanillaFilesCleanup(args);
                }
                else if (arg == "clearLegacy")
                {
                    Util.DoVanillaFilesCleanup(args);
                }
                else if (arg == "debug")
                {
                    App.isDebug = true;
                    Logger.Log("debug mode enabled");
                }
                else if (arg == "reset")
                {
                    if (Directory.Exists(ConfigFolder))
                    {
                        try
                        {
                            Directory.Delete(ConfigFolder, true);
                            App.isFirstBoot = true;
                        }
                        catch { }
                    }
                }
            }
        }

        public static void ExtractResource(string resourceName, string outputPath)
        {

            Assembly assembly = Assembly.GetExecutingAssembly();
            string fullResourceName = assembly.GetName().Name + ".Resources." + resourceName;
            using Stream stream = assembly.GetManifestResourceStream(fullResourceName);
            if (stream == null)
            {
                Console.WriteLine($"Resource not found: {fullResourceName}");
                return;
            }

            using FileStream fileStream = File.Create(outputPath);
            stream.CopyTo(fileStream);
        }

        public static Uri GetResourceUri(string resourceName)
        {
            // Get the assembly where the resource is located
            Assembly assembly = Assembly.GetExecutingAssembly();

            // Construct the full resource name
            string fullResourceName = assembly.GetName().Name + ".Resources." + resourceName;

            // Load the resource stream from the assembly
            using Stream stream = assembly.GetManifestResourceStream(fullResourceName);
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

        private static void LoadManagerConfig()
        {
			SettingsManager.InitializeSettingsManager();

            if (ManagerSettings is not null)
            {
				if (ManagerSettings.GameEntries.Count() > 0)
				{
					switch (SettingsManager.GetCurrentGame().Type)
					{
						case GameEntry.GameType.Unsupported:
							CurrentGame = GamesInstall.Unknown;
							break;
						case GameEntry.GameType.SADX:
							CurrentGame = GamesInstall.SonicAdventure;
							CurrentGame.gameDirectory = SettingsManager.GetCurrentGame().Directory;
							break;
						case GameEntry.GameType.SA2:
							CurrentGame = GamesInstall.SonicAdventure2;
							CurrentGame.gameDirectory = SettingsManager.GetCurrentGame().Directory;
							break;
					}
				}
				else
					CurrentGame = GamesInstall.Unknown;
            }
        }

        public static async Task EnableOneClickInstall()
        {
            try
            {
                if (Util.IsStringValid(App.CurrentGame.oneClickName))
                {
                    string execPath = Environment.ProcessPath;
                    string clickName = App.CurrentGame.oneClickName;
                    await Process.Start(new ProcessStartInfo(execPath, $"urlhandler \"{clickName}\"") { UseShellExecute = true }).WaitForExitAsync();
                }
            }
            catch
            { }
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

