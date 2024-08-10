using Microsoft.Win32;
using SAModManager.Configuration;
using SAModManager.Updater;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using SAModManager.UI;

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

        private static bool InstallDependenciesOffline(Dependencies dependency)
        {
            bool success = false;
            switch (dependency.format)
            {
                case Format.zip:
                    success = Util.ExtractZipFromResource(dependency.data, dependency.path);
                    break;
                case Format.dll:
                    success = Util.ExtractEmbeddedDLL(dependency.data, dependency.name, dependency.path);
                    break;
            }

            return success;
        }

        public static async Task InstallDLL_Loader(Game game, bool isupdate = false)
        {
            if (game is null || !File.Exists(Path.Combine(game?.gameDirectory, game?.exeName)))
                return;


            Util.CreateSafeDirectory(game.modDirectory);

            string loaderPath = Path.Combine(game.modDirectory, game.loader.name + ".dll");

            try
            {
                Uri uri = new(game.loader.URL + "\r\n");
                var loader = new List<DownloadInfo>
                {
                    new DownloadInfo(game.loader.name, Path.GetFileName(game.loader.URL), game.modDirectory, uri, DownloadDialog.DLType.Install)
                };

                var dl = new DownloadDialog(loader);
                dl.StartDL();

                if (dl.errorCount > 0 && !isupdate)
                {
                    if (!File.Exists(loaderPath))
                    {
                        var offline = new OfflineInstall(game.loader.name);
                        offline.Show();
                        await Task.Delay(1000);
                        bool success = Util.ExtractEmbeddedDLL(game.loader.data, game.loader.name, game.modDirectory);
                        offline.CheckSuccess(success);
                        File.WriteAllText(App.CurrentGame.loader.loaderVersionpath, "offlineVersionInstalled");
                        await Task.Delay(500);
                        offline.Close();
                    }
                    else
                    {
                        dl.DisplayDownloadFailedMSG(null, game.loader.name);
                    }
                }
                else
                {
                    var lastCommit = await GitHub.GetLoaderHashCommit();
                    if (lastCommit is not null)
                    {
                        File.WriteAllText(App.CurrentGame.loader.loaderVersionpath, lastCommit);
                    }
                    else
                    {
                        File.WriteAllText(App.CurrentGame.loader.loaderVersionpath, "loaderInstalledNoCommitIDFound");
                    }
                }
            }
            catch (Exception ex)
            {
                DownloadDialog.DisplayGenericDownloadFailedMSG(ex);
            }

            var resources = new List<DownloadInfo>();


            SetUpdateCodes(App.CurrentGame, ref resources); //update codes
            SetUpdatePatches(App.CurrentGame, ref resources); //update patches

            string border = Path.Combine(game.modDirectory, "Border_Default.png");

            if (!File.Exists(border) && !string.IsNullOrEmpty(game.BorderLoaderLink))
            {
                Uri uri = new(game.BorderLoaderLink);
                resources.Add(new DownloadInfo("Border Loader", "Border_Default.png", game.modDirectory, uri, DownloadDialog.DLType.Download));
            }

            //gross and hopefully Temp
            if (App.CurrentGame?.id == SetGame.SA2)
            {
                string shader = Path.Combine(game.modDirectory, "DebugTextShader.hlsl");
                if (!File.Exists(shader))
                {
                    Uri debugUriTex = new(Properties.Resources.URL_SA2_DEBUGFONT_TEX);
                    resources.Add(new DownloadInfo("DebugFontTexture", "DebugFontTexture.dds", game.modDirectory, debugUriTex, DownloadDialog.DLType.Download));

                    Uri debugUriShader = new(Properties.Resources.URL_SA2_DEBUGTEXT_SHADER);
                    resources.Add(new DownloadInfo("DebugFontShader", "DebugTextShader.hlsl", game.modDirectory, debugUriShader, DownloadDialog.DLType.Download));
                }

            }

            if (resources.Count > 0)
            {
                var DL = new DownloadDialog(resources);
                try
                {
                    DL.StartDL();
                }
                catch
                {
                    if (DL.errorCount > 0)
                    {
                        DL.DisplayFailedDownloadList();
                    }
                }
            }
        }

        public static async Task<bool> UpdateLoader(Game game)
        {
            if (game is null)
                return false;

            try
            {
                Uri uri = new(game.loader.URL + "\r\n");
                var loaderInfo = new List<DownloadInfo>
                {
                     new DownloadInfo(game.loader.name, Path.GetFileName(game.loader.URL), game.modDirectory, uri, DownloadDialog.DLType.Update)
                };

                var dl = new DownloadDialog(loaderInfo);

                await Task.Delay(10);
                bool success = false;

                dl.DownloadFailed += (ex) =>
                {
                    dl.DisplayDownloadFailedMSG(ex, Path.GetFileName(game.loader.URL));
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

        public static void SetUpdateCodes(Game game, ref List<DownloadInfo> updates)
        {
            if (game is null)
                return;

            try
            {
                ((MainWindow)App.Current.MainWindow).UpdateManagerStatusText(Lang.GetString("UpdateStatus.UpdateCodes"));
                string codePath = Path.Combine(game.modDirectory, "Codes.lst");
                Uri uri = new(game.codeURL + "\r\n");
                updates.Add(new DownloadInfo("Codes", "Codes.lst", game.modDirectory, uri, DownloadDialog.DLType.Update));
            }
            catch
            {
                Console.WriteLine("Failed to update code\n");
                ((MainWindow)App.Current.MainWindow).UpdateManagerStatusText(Lang.GetString("UpdateStatus.FailedUpdateCodes"));
            }
        }

        private static async Task<bool> PerformOfflineInstall(Dependencies dependency)
        {
            var offline = new OfflineInstall(dependency.name);
            offline.Show();
            await Task.Delay(250);
            bool success = InstallDependenciesOffline(dependency);
            offline.CheckSuccess(success);
            await Task.Delay(250);
            offline.Close();
            return success;
        }

        public static void SetUpdatePatches(Game game, ref List<DownloadInfo> updates)
        {
            if (game is null)
                return;

            try
            {
                ((MainWindow)App.Current.MainWindow).UpdateManagerStatusText(Lang.GetString("UpdateStatus.ChkPatchesUpdates"));
                string codePath = Path.Combine(game.modDirectory, "Patches.json");
                Uri uri = new(game.patchURL + "\r\n");
                updates.Add(new DownloadInfo("Patches", "Patches.json", game.modDirectory, uri, DownloadDialog.DLType.Update));
            }
            catch
            {
                Console.WriteLine("Failed to update patches\n");
                ((MainWindow)App.Current.MainWindow).UpdateManagerStatusText(Lang.GetString("UpdateStatus.FailedUpdatePatches"));
            }
        }

        public static async Task InstallAndUpdateDependencies(Game game, bool isUpdate)
        {
            if (game is null)
                return;

            try
            {
                List<DownloadInfo> updates = new();
                ((MainWindow)App.Current.MainWindow).UpdateManagerStatusText(Lang.GetString(isUpdate ? "UpdateStatus.UpdateDependencies" : "UpdateStatus.InstallDependencies"));

                foreach (var dependency in game.Dependencies)
                {
                    Uri uri = new(dependency.URL + "\r\n");
                    updates.Add(new DownloadInfo(dependency.name, Path.GetFileName(dependency.URL), dependency.path, uri, DependencyInstalled(dependency) ? DownloadDialog.DLType.Update : DownloadDialog.DLType.Download));
                }

                if (updates.Count == 0) //todo add proper check for dependency update
                    return;

                DownloadDialog dl = new(updates);
                dl.StartDL();

                foreach (var dependency in game.Dependencies)
                {
                    string dest = Path.Combine(dependency.path, dependency.name);
                    string fullPath = dest + ".zip";

                    if (!GamesInstall.DependencyInstalled(dependency) && !isUpdate && (!File.Exists(fullPath) || dl.isInError(dependency.name)))
                    {
                        await PerformOfflineInstall(dependency);
                    }
                    else
                    {
                        //check if file need to be extracted


                        if (File.Exists(fullPath))
                        {
                            await Util.Extract(fullPath, dependency.path, true);
                            File.Delete(fullPath);
                        }
                    }
                }
            }
            catch
            {
                Console.WriteLine("Failed to update Dependencies\n");
                ((MainWindow)App.Current.MainWindow).UpdateManagerStatusText(Lang.GetString("UpdateStatus.FailedUpdateDependencies"));
            }
        }

        public static Game Unknown = new();

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
            codeURL = Properties.Resources.URL_SA2_CODE,
            patchURL = Properties.Resources.URL_SA2_PATCH,
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

        public static void LoadMissingGamesList()
        {
            foreach (var id in App.ManagerSettings?.gamesInstalled)
            {
                var game = GetGamePerID((SetGame)id);
                if (id > 0 && App.GamesList.Contains(game) == false && game != null)
                {
                    App.GamesList.Add(game);
                }
            }
        }

        public static void AddMissingGamesList(Game game)
        {
            if (App.ManagerSettings?.gamesInstalled.Contains((uint)game.id) == false)
                App.ManagerSettings?.gamesInstalled.Add((uint)game.id);
        }


        public static SetGame SetGameInstallManual(string GamePath)
        {
            foreach (var game in GamesInstall.GetSupportedGames())
            {
                string path = Path.Combine(GamePath, game.exeName);

                if (Steam.isSADXGamePath(GamePath))
                {
                    Steam.DisplaySADXSteamWarning(GamePath);
                }
                else if (File.Exists(path)) //game Path valid 
                {
                    return game.id;
                }
            }

            return SetGame.None;
        }

        public static SetGame SetGameInstall(string GamePath, Game game, bool skipMSG = false)
        {

            string path = Path.Combine(GamePath, game.exeName);

            if (Steam.isSADXGamePath(GamePath))
            {
                Steam.DisplaySADXSteamWarning(GamePath);
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
            Logger.Log("\nAuto Game Detection starts now...");
            Logger.Log("Now looking for games in the same folder as the Manager...");
            try
            {
                foreach (var game in GamesInstall.GetSupportedGames())
                {
                    string path = Path.Combine(App.StartDirectory, game.exeName);
                    Logger.Log("Checking for: " + path);
                    if (File.Exists(path))
                    {
                        App.GamesList.Add(game);
                        AddMissingGamesList(game);
                    }
                }

                Logger.Log("Now looking for games through Steam...");

                foreach (var game in GamesInstall.GetSupportedGames())
                {
                    foreach (var pathValue in Steam.steamAppsPaths)
                    {
                        string gameInstallPath = Path.Combine(pathValue, "steamapps", "common", game.gameName);

                        Logger.Log("Checking for: " + gameInstallPath);
                        if (Directory.Exists(gameInstallPath) && !App.GamesList.Contains(game))
                        {
                            Logger.Log("Found Game!");
                            App.GamesList.Add(game);
                            AddMissingGamesList(game);
                        }

                    }
                }

                GamesInstall.LoadMissingGamesList();


            }
            catch { }

        }

        public static void HandleGameSwap(Game game, string newPath = null)
        {
            try
            {
                if (newPath == null && (string.IsNullOrEmpty(game.gameDirectory) == false))
                {
                    if (File.Exists(Path.Combine(game.gameDirectory, game.exeName)))
                    {
                        return;
                    }
                }

                string path = Path.Combine(App.StartDirectory, game.exeName);

                string gameDir = string.Empty;

                if (string.IsNullOrEmpty(newPath) == false)
                {
                    string newpath = Path.Combine(newPath, game.exeName);

                    if (File.Exists(newpath))
                        gameDir = Path.GetFullPath(Path.GetDirectoryName(newpath));
                }
                else if (File.Exists(path))
                {
                    gameDir = Path.GetFullPath(Path.GetDirectoryName(path));
                }


                if (string.IsNullOrEmpty(gameDir) == false)
                {
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
                            App.ManagerSettings.CurrentSetGame = (int)game.id;
                            if (App.CurrentGame.id != SetGame.None)
                            {
                                ((MainWindow)App.Current.MainWindow).tempPath = path;
                                App.CurrentGame.gameDirectory = path;
                                ((MainWindow)App.Current.MainWindow).textGameDir.Text = path;
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

                    if (App.isLinux)
                    {
                        if (pathValue.Contains('\\'))
                        {
                            pathValue = pathValue.Replace('\\', '/');
                            Logger.Log("Linux: Detected backslashes, adjusted to forward slashes.");
                        }

                        Util.AdjustPathForLinux(ref pathValue);
                    }

                    paths.Add(Path.GetFullPath(pathValue)); //getfullpath fixes the extra backslashes, lol
                    Logger.Log("Path Found: " + Path.GetFullPath(pathValue));
                }
            }

            return paths;
        }

        private static void SetSteamAppsPaths()
        {
            if (SteamLocation is null)
                return;

            Logger.Log("Now looking for Steam config file...");
            string configPath = Path.Combine(SteamLocation, "config", "libraryfolders.vdf");

            if (File.Exists(configPath))
            {
                Logger.Log("libraryfolders.vdf file found... now attempting to get paths...");
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
                if (string.IsNullOrEmpty(home) == false)
                {
                    Logger.Log("Steam Folder using Linux found at: " + home);
                    SteamLocation = Path.Combine(home, ".steam/steam");
                    return;
                }
                else
                {
                    string steamDir = Environment.GetEnvironmentVariable("STEAM_DIR");
                    if (string.IsNullOrEmpty(steamDir) == false)
                    {
                        Logger.Log("Steam Folder using Linux found at: " + steamDir);
                        SteamLocation = steamDir;
                        return;
                    }
                }
            }


            string steamInstallPath = (string)Registry.GetValue(@"HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Valve\Steam", "InstallPath", null);

            if (steamInstallPath == null)
            {
                var key = RegistryKey.OpenBaseKey(RegistryHive.CurrentUser, RegistryView.Default).OpenSubKey("Software\\Valve\\Steam");

                if (key != null && key.GetValue("SteamPath") is string steamPath)
                {
                    SteamLocation = steamPath;
                    Logger.Log("Steam Folder was found through Windows method at: " + steamPath);
                }
            }
            else
            {
                SteamLocation = steamInstallPath;
                Logger.Log("Steam Folder was found through Windows method at: " + steamInstallPath);
            }
        }

        public static bool isSADXGamePath(string path)
        {
            string fullPath = Path.Combine(path, GamesInstall.SonicAdventure.exeList[1]);
            return File.Exists(fullPath) && !File.Exists(Path.Combine(path, GamesInstall.SonicAdventure.exeList[0]));
        }

        public static void DisplaySADXSteamWarning(string GamePath)
        {
            var stringURL = "https://sadxmodinstaller.unreliable.network/";
            var msg = new MessageWindow(Lang.GetString("MessageWindow.Information.GameDetected.Title"), string.Format(Lang.GetString("MessageWindow.Information.SADXSteamDetectedTemp") + "\n\n" + stringURL, GamePath), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.OK);
            msg.ShowDialog();
        }

        public static bool FindAndSetCurGame(bool skipLoader = false)
        {
            bool success = false;
            try
            {
                foreach (var game in GamesInstall.GetSupportedGames())
                {
                    if (FindAndSetGameInPaths(App.StartDirectory, game, true))
                    {
                        success = true;
                        break;
                    }
                    else
                    {
                        foreach (var pathValue in steamAppsPaths)
                        {
                            string gameInstallPath = Path.Combine(pathValue, "steamapps", "common", game.gameName);
                            success = FindAndSetGameInPaths(gameInstallPath, game, skipLoader);

                            if (success)
                                break;
                        }
                    }
                }
            }
            catch { }

            return success;
        }


        private static bool FindAndSetGameInPaths(string pathValue, Game game, bool skipMSG = false)
        {
            if (Directory.Exists(pathValue))
            {
                var setGame = GamesInstall.SetGameInstall(pathValue, game, skipMSG);

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