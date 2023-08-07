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

namespace SAModManager.Common
{
	public class Game
	{
		public string gameName;
		public List<string> exeList { get; set; } //only because SADX has multiple exe names due to different versions
		public string exeName;
		public string gameDirectory;
		public string modDirectory;
		public string ProfilesDirectory { get; set; }
		public List<Dependencies> Dependencies { get; set; }
		public Loader loader { get; set; }
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

	public class Loader
	{
		public string name;
		public byte[] data;
		public string URL;
		public bool installed = false;
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
			}
		}

		public static async Task InstallLoader(Game game)
		{
			if (game is null)
				return;

			try
			{
				Uri uri = new(game.loader.URL + "\r\n");
				var dl = new GenericDownloadDialog(uri, game.loader.name, Path.GetFileName(game.loader.URL), false, game.modDirectory, false, true);
			
				await dl.StartDL();
				dl.ShowDialog();

				if (dl.done == false)
				{
					await Util.ExtractEmbeddedDLL(game.loader.data, game.loader.name, game.modDirectory);
				}

			}
			catch
			{
				await Util.ExtractEmbeddedDLL(game.loader.data, game.loader.name, game.modDirectory);
			}

		}

		public static async Task CheckAndInstallDependencies(Game game)
		{
			if (game is null)
				return;

			foreach (var dependency in game.Dependencies)
			{
				if (!DependencyInstalled(dependency))
				{					
					try
					{
						Uri uri = new(dependency.URL + "\r\n");
						var dl = new GenericDownloadDialog(uri, dependency.name, Path.GetFileName(dependency.URL), false, dependency.path, true);
						dl.Show();
						await dl.StartDL();

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
									archiveFile.Extract(dependency.path);
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

			loader = new()
			{
				name = "SADXModLoader",
				data = Properties.Resources.SADXModLoader,
				URL = Properties.Resources.URL_SADX_DL
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
		};

		public static Game SonicAdventure2 = new()
		{
			gameName = "Sonic Adventure 2",
			exeName = "sonic2app.exe",

			loader = new()
			{
				name = "SA2ModLoader",
			},

			ProfilesDirectory = Path.Combine(App.ConfigFolder, "SA2"),
		};

        public static IEnumerable<Game> GetSupportedGames()
		{
			yield return SonicAdventure;
            yield return SonicAdventure2;
        }

		public static async Task GetSADXModInstaller()
		{
			var destFolder = Path.Combine(Environment.CurrentDirectory, "SATemp");
			var zipPath = Path.Combine(Environment.CurrentDirectory, "SATemp", "sadx_setup_full.zip");

			try
			{
				Uri uri = new("https://dcmods.unreliable.network/owncloud/data/PiKeyAr/files/Setup/offline/sadx_setup_full.zip" + "\r\n");

				var DL = new GenericDownloadDialog(uri, "SADX Mod Installer (Steam to 2004)", "sadx_setup_full.zip");
			
				await DL.StartDL();
				DL.ShowDialog();

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
