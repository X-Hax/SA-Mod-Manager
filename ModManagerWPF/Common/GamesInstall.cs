using Microsoft.Win32;
using SAModManager.Updater;
using SevenZipExtractor;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;

namespace SAModManager.Common
{
	public class Game
	{
		public string gameName;
		public List<string> exeList { get; set; } //only because SADX has multiple exe names due to different versions
		public string exeName;
		public string gameDirectory;
	}

	public static class GamesInstall
	{
		public static Game SonicAdventure = new()
		{
			gameName = "Sonic Adventure DX",
			exeList = new() { "sonic.exe", "Sonic Adventure DX.exe" },
			exeName = "sonic.exe"
		};

		public static IEnumerable<Game> GetSupportedGames()
		{
			yield return SonicAdventure;
		}

		public static async Task GetSADXModInstaller()
		{
			var destFolder = Path.Combine(Environment.CurrentDirectory, "SATemp");
			var zipPath = Path.Combine(Environment.CurrentDirectory, "SATemp", "sadx_setup_full.zip");

			try
			{
				Uri uri = new("https://dcmods.unreliable.network/owncloud/data/PiKeyAr/files/Setup/offline/sadx_setup_full.zip" + "\r\n");

				var DL = new GenericDownloadDialog(uri, "SADX Mod Installer (Steam to 2004)", "sadx_setup_full.zip");
				DL.StartDL();
				DL.ShowDialog();

				if (DL.DialogResult == true)
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
