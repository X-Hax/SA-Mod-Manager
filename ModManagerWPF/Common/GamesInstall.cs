using Microsoft.Win32;
using SAModManager.Configuration;
using SAModManager.Updater;
using SevenZipExtractor;
using System;
using System.Collections.Generic;
using System.Data;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace SAModManager.Common
{
    public class Game
    {
        /// <summary>
        /// The Name of the Game.
        /// </summary>
        public string gameName;

        /// <summary>
        /// Executables list, necessary due to SADX.
        /// </summary>
        public List<string> exeList { get; set; } //only because SADX has multiple exe names due to different versions

        /// <summary>
        /// This is the executable expected for launching the game.
        /// </summary>
        public string exeName;

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

        /// <summary>
        /// Default Profile, used?
        /// </summary>
        public string defaultIniProfile;

        /// <summary>
        /// List of the game's expected configuration files. Is a List due to SA2.
        /// </summary>
        public List<string> GameConfigFile { get; set; }
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

        private static async Task InstallDependenciesOffline(Dependencies dependency)
        {
            switch (dependency.format)
            {
                case Format.zip:
                    await Util.ExtractZipFromResource(dependency.data, dependency.path);
                    break;
                case Format.dll:
                    await Util.ExtractEmbeddedDLL(dependency.data, dependency.name, dependency.path);
                    break;
                default:
                    throw new Exception("What");
            }
        }

        public static async Task InstallDLL_Loader(Game game, bool force = false)
        {
            if (game is null)
                return;

            try
            {
                Uri uri = new(game.loader.URL + "\r\n");
                var dl = new DownloadDialog(uri, game.loader.name, Path.GetFileName(game.loader.URL), game.modDirectory, DownloadDialog.DLType.Install);

                dl.StartDL();

                if (dl.done == false && !force)
                {
                    await Util.ExtractEmbeddedDLL(game.loader.data, game.loader.name, game.modDirectory);
                }
            }
            catch
            {
                if (!force)
                    await Util.ExtractEmbeddedDLL(game.loader.data, game.loader.name, game.modDirectory);
            }

            await UpdateCodes(App.CurrentGame); //update codes
        }

        public static async Task<bool> UpdateLoader(Game game)
        {
            if (game is null)
                return false;

            try
            {
                Uri uri = new(game.loader.URL + "\r\n");
                var dl = new DownloadDialog(uri, game.loader.name, Path.GetFileName(game.loader.URL), game.modDirectory, DownloadDialog.DLType.Update);

                dl.StartDL();
                await Task.Delay(10);
                if (dl.done == true)
                {
                    if (File.Exists(App.CurrentGame.loader.dataDllOriginPath))
                    {
                        File.Copy(App.CurrentGame.loader.loaderdllpath, App.CurrentGame.loader.dataDllPath, true);
                        return true;
                    }
                }
                return false;
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
                Console.WriteLine("Failed to update mod loader\n");
                ((MainWindow)App.Current.MainWindow).UpdateManagerStatusText(Lang.GetString("UpdateStatus.FailedUpdateCodes"));
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
                        var dl = new DownloadDialog(uri, dependency.name, Path.GetFileName(dependency.URL), dependency.path, DownloadDialog.DLType.Update);

                        dl.StartDL();

                        if (dl.done == false)
                        {
                            await Task.Delay(500);
                        }

                        if (dl.done != false)
                        {
                            string dest = Path.Combine(dependency.path, dependency.name);
                            string fullPath = dest + ".zip";
                            if (dependency.format == Format.zip)
                            {
                                using (ArchiveFile archiveFile = new(fullPath))
                                {
                                    archiveFile.Extract(dependency.path, true);
                                }

                                File.Delete(fullPath);
                            }
                            success = true;
                        }

                        dl.Close();
                    }
                    catch
                    {
                        return false;
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
                        var dl = new DownloadDialog(uri, dependency.name, Path.GetFileName(dependency.URL), dependency.path, DownloadDialog.DLType.Update);
         
                        dl.StartDL();

                        if (dl.done == false)
                        {
                            await Task.Delay(500);
                        }

                        if (dl.done == false)
                        {
                            await InstallDependenciesOffline(dependency);
                        }
                        else
                        {
                            string dest = Path.Combine(dependency.path, dependency.name);
                            string fullPath = dest + ".zip";
                            if (dependency.format == Format.zip)
                            {
                                using (ArchiveFile archiveFile = new(fullPath))
                                {
                                    archiveFile.Extract(dependency.path, true);
                                }

                                File.Delete(fullPath);
                            }

                        }
                        dl.Close();
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
            exeList = new() { "sonic.exe", "Sonic Adventure DX.exe" },
            exeName = "sonic.exe",
            defaultIniProfile = "SADXModLoader.ini",
            codeURL = Properties.Resources.URL_SADX_CODE,

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
                    data = Properties.Resources.SDL2,
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
            exeName = "sonic2app.exe",
            defaultIniProfile = "SA2ModLoader.ini",

            loader = new()
            {
                name = "SA2ModLoader",
                repoName = "sa2-mod-loader",
                loaderVersionpath = Path.Combine(App.ConfigFolder, "SA2LoaderVersion.ini"),
                loaderinipath = "mods/SA2ModLoader.ini"
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
            // yield return SonicAdventure2;
        }

        //will probably end making our own installer ig
        public static async Task GetSADXModInstaller()
        {
            var destFolder = Path.Combine(Environment.CurrentDirectory, ".SATemp");
            var zipPath = Path.Combine(Environment.CurrentDirectory, ".SATemp", "sadx_setup_full.zip");

            try
            {
                Uri uri = new("https://dcmods.unreliable.network/owncloud/data/PiKeyAr/files/Setup/offline/sadx_setup_full.zip" + "\r\n");

                var DL = new DownloadDialog(uri, "SADX Mod Installer (Steam to 2004)", "sadx_setup_full.zip");

                DL.StartDL();

                if (DL.done == true)
                {
                    using (ArchiveFile archiveFile = new(zipPath))
                    {
                        archiveFile.Extract(destFolder);
                    }

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

        private static IEnumerable<string> SearchExeFiles(string folderPath, string targetExeName)
        {
            // Use Directory.EnumerateFiles with SearchOption.AllDirectories to search recursively
            // The search is non-recursive by default.
            return Directory.EnumerateFiles(folderPath, "*.exe", SearchOption.AllDirectories)
                            .Where(filePath => Path.GetFileName(filePath).Equals(targetExeName, StringComparison.OrdinalIgnoreCase));
        }

        public static string GetExePath(string rootDirectory, string exe)
        {
            var files = SearchExeFiles(rootDirectory, exe);

            foreach (string file in files)
            {
                string folderPath = Path.GetDirectoryName(file);

                if (File.Exists(Path.Combine(folderPath, exe)))
                {
                    return Path.GetDirectoryName(file);
                }
            }

            return null;
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
            string pattern = @"""path""\s+""([^""]+)""";

            // Use regex to find all matches of the pattern in the file content
            MatchCollection matches = Regex.Matches(fileContent, pattern);

            // Extract the path values from the matches and add them to the list
            foreach (Match match in matches)
            {
                if (match.Groups.Count >= 2)
                {
                    string pathValue = match.Groups[1].Value;
                    paths.Add(pathValue);
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

        public static string GetGamePath(Game game, bool multipleExes = false)
        {
            if (steamAppsPaths is null || game is null)
                return null;

            try
            {

                foreach (string pathValue in steamAppsPaths)
                {
                    string gameInstallPath = Path.Combine(pathValue, "steamapps", "common", game.gameName);

                    if (Directory.Exists(gameInstallPath))
                    {
                        if (multipleExes)
                        {
                            foreach (var exe in game.exeList)
                            {
                                string exePath = GamesInstall.GetExePath(gameInstallPath, exe);

                                if (exePath != null)
                                {
                                    return exePath;
                                }
                            }
                        }
                        else
                        {
                            string exePath = GamesInstall.GetExePath(gameInstallPath, game.exeName);

                            if (exePath != null)
                                return exePath;
                        }
                    }
                }
            }
            catch { }

            return null;
        }

        public static void Init()
        {
            SetSteamPath();
            SetSteamAppsPaths();

        }
    }
}