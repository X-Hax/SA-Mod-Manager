using Microsoft.Win32;
using SAModManager.Configuration;
using SAModManager.Updater;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using SAModManager.UI;
using SAModManager.Profile;
using System.Text;

namespace SAModManager
{
    public class Game
    {
        /// <summary>
        /// The Name of the Game.
        /// </summary>
        public string gameName { get; set; }
        public Uri gameImage { get; set; }

        /// <summary>
        /// Executables list, necessary due to SADX.
        /// </summary>
        public List<string> exeList { get; set; } //only because SADX has multiple exe names due to different versions

        /// <summary>
        /// This is the executable expected for launching the game.
        /// </summary>
        public string exeName { get; set; }

        /// <summary>
        /// Current game's main directory.
        /// </summary>
        public string gameDirectory;

        /// <summary>
        /// Current game's mods directory.
        /// </summary>
        public string modDirectory;

        /// <summary>
        /// Profiles Directory where Manager, Game Profiles, and other settings are stored.
        /// </summary>
        public string ProfilesDirectory { get; set; }

        /// <summary>
        /// List of Dependencies for the game that the manager will get.
        /// </summary>
        public List<Dependencies> Dependencies { get; set; }

        /// <summary>
        /// Information on the Loader for the game.
        /// </summary>
        public Loader loader { get; set; }

        /// <summary>
        /// URL to the Codes.lst file.
        /// </summary>
        public string codeURL { get; set; }
        public string patchURL { get; set; }

        /// <summary>
        /// Default Profile, used?
        /// </summary>
        public string defaultIniProfile;

        /// <summary>
        /// List of the game's expected configuration files. Is a List due to SA2.
        /// </summary>
        public List<string> GameConfigFile { get; set; }
        public SetGame id = SetGame.None;
        public string gameAbbreviation { get; set; }
        public string oneClickName { get; set; }
        public string BorderLoaderLink { get; set; }
    }

    public enum Format
    {
        zip,
        dll
    }

    public class Dependencies
    {
        public string name;
        public byte[] data;
        public Format format;
        public string path;
        public string URL;
    }

    public class defaultLoaderPath
    {
        public string defaultDataDllOriginPath; //ex 'System/CHRMODELS_orig.dll'
        public string defaultDataDllPath; //ex 'System/CHRMODELS.dll'
        public string defaultLoaderinipath; //ex mods/SADXModLoader.ini"
    }

    public class Loader
    {
        public string name;
        public byte[] data;
        public string URL;
        public bool installed = false;
        public string repoName;
        public string dataDllOriginPath;
        public string dataDllPath;
        public string loaderdllpath;
        public string loaderVersionpath; //used to check version
        public string loaderinipath;
        public defaultLoaderPath originPath;
    }

    public static class GamesInstall
    {
        private static bool DependencyInstalled(Dependencies dependency)
        {
            return File.Exists(Path.Combine(dependency.path, dependency.name + ".dll"));
        }

        public static void SetDependencyPath()
        {
            foreach (var game in GetSupportedGames())
            {
                if (game is null || game.Dependencies is null)
                    continue;

                foreach (var dependency in game.Dependencies)
                {
                    dependency.path = Path.Combine(App.extLibPath, dependency.name);
                }
            }
        }

        private static async Task<bool> InstallDependenciesOffline(Dependencies dependency)
        {
            bool success = false;
            switch (dependency.format)
            {
                case Format.zip:
                    success = await Util.ExtractZipFromResource(dependency.data, dependency.path);
                    break;
                case Format.dll:
                    success = await Util.ExtractEmbeddedDLL(dependency.data, dependency.name, dependency.path);
                    break;
            }

            return success;
        }

        public static async Task InstallDLL_Loader(Game game, bool isupdate = false)
        {
            if (game is null || !File.Exists(Path.Combine(game?.gameDirectory, game?.exeName)))
                return;

            if (Directory.Exists(game.modDirectory) == false)
            {
                Directory.CreateDirectory(game.modDirectory);
            }

            string loaderPath = Path.Combine(game.modDirectory, game.loader.name + ".dll");

            try
            {
                Uri uri = new(game.loader.URL + "\r\n");
                var dl = new DownloadDialog(uri, game.loader.name, Path.GetFileName(game.loader.URL), game.modDirectory, DownloadDialog.DLType.Install, true);
                dl.StartDL();

                if (dl.done == false && !isupdate)
                {
                    if (!File.Exists(loaderPath))
                    {
                        var offline = new OfflineInstall(game.loader.name);
                        offline.Show();
                        await Task.Delay(2000);
                        bool success = await Util.ExtractEmbeddedDLL(game.loader.data, game.loader.name, game.modDirectory);
                        offline.CheckSuccess(success);
                        File.WriteAllText(App.CurrentGame.loader.loaderVersionpath, "offlineVersionInstalled");
                        await Task.Delay(1000);
                        offline.Close();
                    }
                    else
                    {
                        dl.DisplayDownloadFailedMSG(null);
                    }
                }
                else
                {
                    var lastCommit = await GitHub.GetLoaderHashCommit();
                    if (lastCommit is not null)
                    {
                        File.WriteAllText(App.CurrentGame.loader.loaderVersionpath, lastCommit);
                    }
                }
            }
            catch (Exception ex)
            {
                DownloadDialog.DisplayGenericDownloadFailedMSG(ex);
            }

            await UpdateCodes(App.CurrentGame); //update codes
            await UpdatePatches(App.CurrentGame); //update patches

            try
            {
                string border = Path.Combine(game.modDirectory, "Border_Default.png");

                if (!File.Exists(border) && !string.IsNullOrEmpty(game.BorderLoaderLink))
                {
                    Uri uri = new(game.BorderLoaderLink);
                    var dl = new DownloadDialog(uri, "Border Loader", "Border_Default.png", game.modDirectory, DownloadDialog.DLType.Download, true);
                    dl.StartDL();
                }
            } catch { }
        }

        public static async Task<bool> UpdateLoader(Game game)
        {
            if (game is null)
                return false;

            try
            {
                Uri uri = new(game.loader.URL + "\r\n");
                var dl = new DownloadDialog(uri, game.loader.name, Path.GetFileName(game.loader.URL), game.modDirectory, DownloadDialog.DLType.Update);

                await Task.Delay(10);
                bool success = false;

                dl.DownloadFailed += (ex) =>
                {
                    dl.DisplayDownloadFailedMSG(ex);
                };

                dl.DownloadCompleted += () =>
                {
                    if (File.Exists(App.CurrentGame.loader.dataDllOriginPath))
                    {
                        bool retry = false;
                        do
                        {
                            try
                            {
                                File.Copy(App.CurrentGame.loader.loaderdllpath, App.CurrentGame.loader.dataDllPath, true);
                                success = true;
                                retry = false;
                            }
                            catch (Exception ex)
                            {
                                var msg = new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), string.Format(Lang.GetString("MessageWindow.Errors.LoaderCopy"), game.loader?.name) + "\n\n" + ex.Message, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.RetryCancel);
                                msg.ShowDialog();
                                retry = msg.isRetry;
                            }

                        } while (retry == true);
                    }
                };

                dl.StartDL();
                return success;
            }
            catch
            {
                Console.WriteLine("Failed to update mod loader\n");
            }

            return false;
        }

        public static async Task<bool> UpdateCodes(Game game)
        {
            if (game is null)
                return false;

            try
            {
                ((MainWindow)App.Current.MainWindow).UpdateManagerStatusText(Lang.GetString("UpdateStatus.UpdateCodes"));
                string codePath = Path.Combine(game.modDirectory, "Codes.lst");
                Uri uri = new(game.codeURL + "\r\n");
                var dl = new DownloadDialog(uri, "Codes", "Codes.lst", game.modDirectory, DownloadDialog.DLType.Update);

                dl.StartDL();

                await Task.Delay(1);
                return dl.done == true;
            }
            catch
            {
                Console.WriteLine("Failed to update code\n");
                ((MainWindow)App.Current.MainWindow).UpdateManagerStatusText(Lang.GetString("UpdateStatus.FailedUpdateCodes"));


            }

            return false;
        }

        private static async Task<bool> PerformOfflineInstall(Dependencies dependency)
        {
            var offline = new OfflineInstall(dependency.name);
            offline.Show();
            await Task.Delay(2000);
            bool success = await InstallDependenciesOffline(dependency);
            offline.CheckSuccess(success);
            await Task.Delay(1000);
            offline.Close();
            return success;
        }

        public static async Task<bool> UpdatePatches(Game game)
        {
            if (game is null)
                return false;

            try
            {
                ((MainWindow)App.Current.MainWindow).UpdateManagerStatusText(Lang.GetString("UpdateStatus.ChkPatchesUpdates"));
                string codePath = Path.Combine(game.modDirectory, "Patches.json");
                Uri uri = new(game.patchURL + "\r\n");
                var dl = new DownloadDialog(uri, "Patches", "Patches.json", game.modDirectory, DownloadDialog.DLType.Update);

                dl.StartDL();

                await Task.Delay(1);
                return dl.done == true;
            }
            catch
            {
                Console.WriteLine("Failed to update patches\n");
                ((MainWindow)App.Current.MainWindow).UpdateManagerStatusText(Lang.GetString("UpdateStatus.FailedUpdatePatches"));
            }

            return false;
        }

        public static async Task<bool> UpdateDependencies(Game game)
        {
            if (game is null)
                return false;

            try
            {
                ((MainWindow)App.Current.MainWindow).UpdateManagerStatusText(Lang.GetString("UpdateStatus.UpdateDependencies"));

                bool success = false;

                foreach (var dependency in game.Dependencies)
                {
                    try
                    {
                        success = false;
                        Uri uri = new(dependency.URL + "\r\n");

                        var dl = new DownloadDialog(uri, dependency.name, Path.GetFileName(dependency.URL), dependency.path, DependencyInstalled(dependency) ? DownloadDialog.DLType.Update : DownloadDialog.DLType.Download);

                        dl.StartDL();

                        if (dl.done == false)
                        {
                            if (!GamesInstall.DependencyInstalled(dependency))
                            {
                                success = await PerformOfflineInstall(dependency);
                            }
                            else
                            {

                                dl.DisplayDownloadFailedMSG(null);
                            }
                        }
                        else
                        {
                            string dest = Path.Combine(dependency.path, dependency.name);
                            string fullPath = dest + ".zip";

                            if (dependency.format == Format.zip)
                            {
                                await Util.Extract(fullPath, dependency.path, true);
                                File.Delete(fullPath);
                            }
                            success = true;
                        }

                    }
                    catch
                    {
                        if (!File.Exists(dependency.path))
                            success = await InstallDependenciesOffline(dependency);

                        return success;
                    }
                }

                return success;
            }
            catch
            {
                Console.WriteLine("Failed to update Dependencies\n");
                ((MainWindow)App.Current.MainWindow).UpdateManagerStatusText(Lang.GetString("UpdateStatus.FailedUpdateDependencies"));
            }

            return false;
        }

        public static async Task CheckAndInstallDependencies(Game game)
        {
            if (game is null)
                return;

            foreach (var dependency in game.Dependencies)
            {
                if (!DependencyInstalled(dependency))
                {
                    ((MainWindow)App.Current.MainWindow).UpdateManagerStatusText(Lang.GetString("UpdateStatus.InstallDependencies"));

                    try
                    {
                        Uri uri = new(dependency.URL + "\r\n");
                        var dl = new DownloadDialog(uri, dependency.name, Path.GetFileName(dependency.URL), dependency.path, DownloadDialog.DLType.Download, true);

                        dl.StartDL();

                        if (dl.done == false)
                        {
                            await PerformOfflineInstall(dependency);
                        }
                        else
                        {
                            string dest = Path.Combine(dependency.path, dependency.name);
                            string fullPath = dest + ".zip";
                            if (dependency.format == Format.zip)
                            {
                                await Util.Extract(fullPath, dependency.path, true);
                                File.Delete(fullPath);
                            }
                        }
                    }
                    catch
                    {
                        await InstallDependenciesOffline(dependency);
                    }
                }
            }
        }


        public static Game SonicAdventure = new()
        {
            gameName = "Sonic Adventure DX",
            gameImage = App.GetResourceUri("Games.sa1.png"),
            exeList = new() { "sonic.exe", "Sonic Adventure DX.exe" },
            exeName = "sonic.exe",
            defaultIniProfile = "SADXModLoader.ini",
            codeURL = Properties.Resources.URL_SADX_CODE,
            patchURL = Properties.Resources.URL_SADX_PATCH,
            id = SetGame.SADX,
            gameAbbreviation = "SADX",
            oneClickName = "sadxmm",
            BorderLoaderLink = Properties.Resources.URL_SADX_BORDER,

            loader = new()
            {
                name = "SADXModLoader",
                data = Properties.Resources.SADXModLoader,
                URL = Properties.Resources.URL_SADX_DL,
                repoName = "sadx-mod-loader",
                loaderVersionpath = Path.Combine(App.ConfigFolder, "SADXLoaderVersion.ini"),

                originPath = new()
                {
                    defaultDataDllOriginPath = "System/CHRMODELS_orig.dll",
                    defaultDataDllPath = "System/CHRMODELS.dll",
                    defaultLoaderinipath = "mods/SADXModLoader.ini"
                }
            },

            Dependencies = new()
            {
                new Dependencies()
                {
                    name = "BASS",
                    data = Properties.Resources.bass,
                    format = Format.zip,
                    URL = Properties.Resources.URL_BASS,
                },

                new Dependencies()
                {
                    name = "SDL2",
                    data = Properties.Resources.SDL2,
                    format = Format.dll,
                    URL = Properties.Resources.URL_SDL

                },

                new Dependencies()
                {
                    name = "D3D8M",
                    data = Properties.Resources.d3d8m,
                    format = Format.dll,
                    URL = Properties.Resources.URL_D3D8M,
                },
            },

            ProfilesDirectory = Path.Combine(App.ConfigFolder, "SADX"),

            GameConfigFile = new()
            {
                "sonicDX.ini"
            },
        };

        public static Game SonicAdventure2 = new()
        {
            gameName = "Sonic Adventure 2",
            gameImage = App.GetResourceUri("Games.sa2.png"),
            exeName = "sonic2app.exe",
            defaultIniProfile = "SA2ModLoader.ini",
            id = SetGame.SA2,
            gameAbbreviation = "SA2",
            oneClickName = "sa2mm",
            BorderLoaderLink = Properties.Resources.URL_SA2_BORDER,

            loader = new()
            {
                name = "SA2ModLoader",
                data = Properties.Resources.SA2ModLoader,
                URL = Properties.Resources.URL_SA2_DL,
                repoName = "sa2-mod-loader",
                loaderVersionpath = Path.Combine(App.ConfigFolder, "SA2LoaderVersion.ini"),
                loaderinipath = "mods/SA2ModLoader.ini",

                originPath = new()
                {
                    defaultDataDllOriginPath = @"resource\gd_PC\DLL\Win32\Data_DLL_orig.dll",
                    defaultDataDllPath = @"resource\gd_PC\DLL\Win32\Data_DLL.dll",
                    defaultLoaderinipath = @"mods\SA2ModLoader.ini"
                }
            },

            Dependencies = new()
            {
                new Dependencies()
                {
                    name = "BASS",
                    data = Properties.Resources.bass,
                    format = Format.zip,
                    URL = Properties.Resources.URL_BASS,
                },

                new Dependencies()
                {
                    name = "SDL2",
                    data = Properties.Resources.SDL2,
                    format = Format.dll,
                    URL = Properties.Resources.URL_SDL

                },
            },

            ProfilesDirectory = Path.Combine(App.ConfigFolder, "SA2"),

            GameConfigFile = new()
            {
                "Config/Keyboard.cfg",
                "Config/UserConfig.cfg"
            },
        };

        public static IEnumerable<Game> GetSupportedGames()
        {
            yield return SonicAdventure;
            yield return SonicAdventure2;
        }

        public static Game GetGamePerID(SetGame gameID)
        {
            foreach (var game in GetSupportedGames())
            {
                if (game.id == gameID)
                    return game;
            }

            return null;
        }

        //will probably end making our own installer ig
        public static async Task GetSADXModInstaller()
        {
            var destFolder = Path.Combine(Environment.CurrentDirectory, App.tempFolder);
            var zipPath = Path.Combine(Environment.CurrentDirectory, App.tempFolder, "sadx_setup_full.zip");

            try
            {
                Uri uri = new("https://dcmods.unreliable.network/owncloud/data/PiKeyAr/files/Setup/offline/sadx_setup_full.zip" + "\r\n");

                var DL = new DownloadDialog(uri, "SADX Mod Installer (Steam to 2004)", "sadx_setup_full.zip");

                DL.StartDL();

                if (DL.done == true)
                {
                    await Util.Extract(zipPath, destFolder);

                    await Process.Start(new ProcessStartInfo(Path.Combine(destFolder, "sadx_setup.exe"), "/install /passive /norestart")
                    {
                        UseShellExecute = true,
                        Verb = "runas"
                    }).WaitForExitAsync();

                    App.Current.Shutdown();
                }
            }
            catch
            {
                throw new Exception("Failed to download or extract SADX Mod Installer.");
            }
        }


        public static async Task<SetGame> SetGameInstallManual(string GamePath)
        {
            foreach (var game in GamesInstall.GetSupportedGames())
            {
                string path = Path.Combine(GamePath, game.exeName);

                if (Steam.isSADXGamePath(GamePath))
                {
                    //To do add installer support
                    await Steam.InstallSADXModInstaller(GamePath);
                }
                else if (File.Exists(path)) //game Path valid 
                {
                    return game.id;
                }
            }

            return SetGame.None;
        }

        public static async Task<SetGame> SetGameInstall(string GamePath, Game game, bool skipMSG = false)
        {

            string path = Path.Combine(GamePath, game.exeName);

            if (Steam.isSADXGamePath(GamePath))
            {
                //To do add installer support
                await Steam.InstallSADXModInstaller(GamePath);
            }
            else if (File.Exists(path)) //game Path valid 
            {
                if (skipMSG)
                {
                    return game.id;
                }

                var msg = new MessageWindow(Lang.GetString("MessageWindow.Information.GameDetected.Title"), string.Format(Lang.GetString("MessageWindow.Information.GameDetected"), game.gameName, path), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Information, MessageWindow.Buttons.YesNo);
                msg.ShowDialog();

                if (msg.isYes)
                {
                    return game.id;
                }
            }


            return SetGame.None;
        }

        public static void AddGamesInstall()
        {

            try
            {
                foreach (var game in GamesInstall.GetSupportedGames())
                {
                    string path = Path.Combine(Environment.CurrentDirectory, game.exeName);

                    if (File.Exists(path))
                    {
                        App.GamesList.Add(game);
                    }
                    else
                    {
                        foreach (var pathValue in Steam.steamAppsPaths)
                        {
                            string gameInstallPath = Path.Combine(pathValue, "steamapps", "common", game.gameName);

                            if (Directory.Exists(gameInstallPath))
                                App.GamesList.Add(game);
                        }
                    }
                }
            }
            catch { }

        }

        public static void HandleGameSwap(Game game)
        {
            string path = Path.Combine(Environment.CurrentDirectory, game.exeName);

            try
            {
                if (File.Exists(path))
                {
                    string gameDir = Path.GetDirectoryName(path);
                    App.ManagerSettings.CurrentSetGame = (int)game.id;
                    if (App.CurrentGame.id != SetGame.None)
                    {
                        ((MainWindow)App.Current.MainWindow).tempPath = gameDir;
                        App.CurrentGame.gameDirectory = gameDir;
                        ((MainWindow)App.Current.MainWindow).textGameDir.Text = gameDir;
                    }
                }
                else
                {
                    foreach (var pathValue in Steam.steamAppsPaths)
                    {
                        path = Path.Combine(pathValue, "steamapps", "common", game.gameName);
                        if (Directory.Exists(path))
                        {
                            string gameDir = Path.GetDirectoryName(path);
                            App.ManagerSettings.CurrentSetGame = (int)game.id;
                            if (App.CurrentGame.id != SetGame.None)
                            {
                                ((MainWindow)App.Current.MainWindow).tempPath = gameDir;
                                App.CurrentGame.gameDirectory = gameDir;
                                ((MainWindow)App.Current.MainWindow).textGameDir.Text = gameDir;
                            }
                        }
                    }

                }

            }
            catch { }
        }
    }

    public static class Steam
    {
        public static string SteamLocation;
        public static List<string> steamAppsPaths { get; set; }

        private static List<string> GetPathValues(string fileContent)
        {
            List<string> paths = new();

            // Regular expression pattern to match "path" values
            string pattern = @"""path""\s+""([^""]+?)""";

            // Use regex to find all matches of the pattern in the file content
            MatchCollection matches = Regex.Matches(fileContent, pattern);

            // Extract the path values from the matches and add them to the list
            foreach (Match match in matches.Cast<Match>())
            {
                if (match.Groups.Count >= 2)
                {
                    string pathValue = match.Groups[1].Value;
                    paths.Add(Path.GetFullPath(pathValue)); //getfullpath fixes the extra backslashes, lol
                }
            }

            return paths;
        }

        private static void SetSteamAppsPaths()
        {
            if (SteamLocation is null)
                return;

            string configPath = Path.Combine(SteamLocation, "config", "libraryfolders.vdf");

            if (File.Exists(configPath))
            {
                steamAppsPaths = new();
                string fileContent = File.ReadAllText(configPath);
                steamAppsPaths = GetPathValues(fileContent);
            }
        }

        private static void SetSteamPath()
        {
            if (App.isLinux)
            {
                string home = Environment.GetEnvironmentVariable("WINEHOMEDIR").Replace("\\??\\", "");
                SteamLocation = Path.Combine(home, ".steam/steam"); //unused
            }

            string steamInstallPath = (string)Registry.GetValue(@"HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Valve\Steam", "InstallPath", null);

            if (steamInstallPath == null)
            {
                var key = RegistryKey.OpenBaseKey(RegistryHive.CurrentUser, RegistryView.Default).OpenSubKey("Software\\Valve\\Steam");

                if (key != null && key.GetValue("SteamPath") is string steamPath)
                    SteamLocation = steamPath;
            }
            else
            {
                SteamLocation = steamInstallPath;
            }
        }

        public static bool isSADXGamePath(string path)
        {
            string fullPath = Path.Combine(path, GamesInstall.SonicAdventure.exeList[1]);
            return File.Exists(fullPath) && !File.Exists(Path.Combine(path, GamesInstall.SonicAdventure.exeList[0]));
        }

        public async static Task InstallSADXModInstaller(string GamePath)
        {
            var msg = new MessageWindow(Lang.GetString("MessageWindow.Information.GameDetected.Title"), string.Format(Lang.GetString("MessageWindow.Information.SADXSteamDetectedTemp"), GamePath), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.OK);
            msg.ShowDialog();


            /*var msg = new MessageWindow(Lang.GetString("MessageWindow.Information.GameDetected.Title"), Lang.GetString("MessageWindow.Information.SADXSteamDetected"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);
            msg.ShowDialog();

            if (msg.isYes)
            {
                await GamesInstall.GetSADXModInstaller();
            }*/
        }

        public static async Task<bool> FindAndSetCurGame(bool skipLoader = false)
        {
            bool success = false;
            try
            {
                foreach (var game in GamesInstall.GetSupportedGames())
                {
                    if (await FindAndSetGameInPaths(Environment.CurrentDirectory, game, true))
                    {
                        success = true;
                        break;
                    }
                    else
                    {
                        foreach (var pathValue in steamAppsPaths)
                        {
                            string gameInstallPath = Path.Combine(pathValue, "steamapps", "common", game.gameName);
                            success = await FindAndSetGameInPaths(gameInstallPath, game, skipLoader);

                            if (success)
                                break;
                        }

                    }
                }
            }
            catch { }

            return success;
        }
  

        private static async Task<bool> FindAndSetGameInPaths(string pathValue, Game game, bool skipMSG = false)
        {
            if (Directory.Exists(pathValue))
            {
               var setGame = await GamesInstall.SetGameInstall(pathValue, game, skipMSG);

                if (setGame != SetGame.None)
                {   
                    ((MainWindow)App.Current.MainWindow).tempPath = pathValue;
                    App.CurrentGame.gameDirectory = pathValue;
                    return true;
                }
            }

            return false;
        }

        public static void Init()
        {
            SetSteamPath();
            SetSteamAppsPaths();
        }
    }
}