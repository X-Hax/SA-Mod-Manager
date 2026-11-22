using System;
using System.Diagnostics;
using System.IO;
using System.Linq;

namespace SAModManager
{
    public static class Flatpak
    {
        private static readonly string[] flatpakBinaries = { "/usr/bin/flatpak", "/usr/local/bin/flatpak" };

        public static string GetHostHomeDirectory()
        {
            string home = Environment.GetEnvironmentVariable("WINEHOMEDIR");
            if (Util.IsStringValid(home))
                return home.Replace("\\??\\", "");

            home = Environment.GetEnvironmentVariable("HOME");
            if (Util.IsStringValid(home) && home.StartsWith('/'))
            {
                Util.AdjustPathForLinux(ref home);
                return home;
            }

            return string.Empty;
        }

        public static string GetInstalledBinary(Game game)
        {
            if (App.isLinux == false || game is null || Util.IsStringValid(game.flatpakId) == false || Util.IsStringValid(game.linuxExe) == false)
                return string.Empty;

            string binary = Path.Combine("app", game.flatpakId, "current", "active", "files", "bin", game.linuxExe);

            // user scoped flatpaks first
            string home = GetHostHomeDirectory();
            if (Util.IsStringValid(home))
            {
                string userFlatpak = Path.Combine(home, ".local", "share", "flatpak", binary);
                if (File.Exists(userFlatpak))
                    return userFlatpak;
            }

            // system scope
            string systemRoot = "/var/lib/flatpak";
            Util.AdjustPathForLinux(ref systemRoot);
            string systemFlatpak = Path.Combine(systemRoot, binary);
            if (File.Exists(systemFlatpak))
                return systemFlatpak;

            return string.Empty;
        }

        public static string GetDataDirectory(Game game)
        {
            if (App.isLinux == false || game is null || Util.IsStringValid(game.flatpakId) == false)
                return string.Empty;

            string home = GetHostHomeDirectory();
            if (Util.IsStringValid(home) == false)
                return string.Empty;

            return Path.Combine(home, ".var", "app", game.flatpakId, "data");
        }

        public static bool IsFlatpakGameDirectory(Game game, string directory)
        {
            string dataDir = GetDataDirectory(game);
            if (Util.IsStringValid(dataDir) == false || Util.IsStringValid(directory) == false)
                return false;

            try
            {
                if (string.Equals(Path.GetFullPath(directory).TrimEnd('\\', '/'), Path.GetFullPath(dataDir).TrimEnd('\\', '/'), StringComparison.OrdinalIgnoreCase) == false)
                    return false;
            }
            catch
            {
                return false;
            }

            return Directory.Exists(dataDir) && Util.IsStringValid(GetInstalledBinary(game));
        }

        public static Game GetGameFromBinary(string exePath)
        {
            if (App.isLinux == false || Util.IsStringValid(exePath) == false)
                return null;

            return GamesInstall.GetSupportedGames().FirstOrDefault(game =>
            {
                string binary = GetInstalledBinary(game);
                return Util.IsStringValid(binary) && string.Equals(Path.GetFullPath(binary), Path.GetFullPath(exePath), StringComparison.OrdinalIgnoreCase);
            });
        }

        public static Process Run(Game game, string workingDirectory, string arguments = "")
        {
            string flatpak = flatpakBinaries.Select(path => { Util.AdjustPathForLinux(ref path); return path; }).FirstOrDefault(File.Exists);
            if (flatpak is null)
            {
                Logger.Log("Flatpak: support for Flatpak not detected");
                return null;
            }

            string args = "run " + game.flatpakId;
            if (Util.IsStringValid(arguments))
                args += " " + arguments;

            Logger.Log("Flatpak: launching " + flatpak + " " + args);
            try
            {
                return Process.Start(new ProcessStartInfo(flatpak)
                {
                    UseShellExecute = false,
                    WorkingDirectory = Directory.Exists(workingDirectory) ? workingDirectory : Path.GetDirectoryName(flatpak),
                    Arguments = args
                });
            }
            catch (Exception ex)
            {
                Logger.Log("Flatpak: failed to start " + game.flatpakId + ": " + ex.Message);
                return null;
            }
        }
    }
}
