using SAMM.Configuration.Enumeration;
using System.Diagnostics;

namespace SAMM.Configuration
{
	public class GameConfig
	{
		#region Variables
		public GameIDs GameID { get; set; } = GameIDs.Unsupported;

		public string GameDirectory { get; set; } = string.Empty;

		public string GameExecutable { get; set; } = string.Empty;

		private string gameSystemFolder = string.Empty;

		public string GameSystemFolder
		{
			get { return gameSystemFolder; }

			set { gameSystemFolder = Path.Combine(GameDirectory, value); }
		}

		public string GameModsFolder { get { return Path.Combine(GameDirectory, "mods"); } }

		public string ModloaderPath { get { return Path.Combine(GameModsFolder, ".modloader"); } }

		public string ProfilesPath { get { return Path.Combine(ModloaderPath, "profiles"); } }

		public ModLoaderInfo? ModLoaderInfo { get; set; }

		public string OneClickProtocol { get; set; } = string.Empty;

		public bool IsModLoaderInstalled
		{
			get
			{
				if (ModLoaderInfo != null)
				{
					if (ModLoaderInfo.InjectionFile.Length > 0)
					{
						return File.Exists(Path.Combine(GameSystemFolder, ModLoaderInfo.InjectionFile + "_orig.dll"));
					}
				}

				return false;
			}
		}

		public bool HideMods { get; set; } = false;

		public bool HideCodes { get; set; } = false;

		public bool HideTestSpawn { get; set; } = false;

		public bool HideConfig { get; set; } = false;

		#endregion

		public GameConfig() { }

		#region Functions
		public void LaunchGame(string customExecutable = "", string cmds = "")
		{
			string gameExecutablePath = Path.Combine(GameDirectory, GameExecutable);

			ProcessStartInfo processStartInfo = new ProcessStartInfo()
			{
				WorkingDirectory = GameDirectory,
				FileName = gameExecutablePath,
				Arguments = cmds,
			};

			Process.Start(processStartInfo);
		}

		private bool verifyExecutable()
		{
			foreach (var file in Directory.GetFiles(GameDirectory, "*.exe"))
			{
				if (Path.GetFileName(file) == GameExecutable)
					return true;
			}
			return false;
		}

		public async Task<bool> VerifyExecutable()
		{
			return await Task.Run(() =>
			{
				return verifyExecutable();
			});
		}

		private bool installModloader()
		{
			try
			{
				ModLoaderInfo.InstallLoader(ModloaderPath, GameDirectory, true);
				return true;
			}
			catch
			{
				return false;
			}
		}

		public async Task<bool> InstallModloader()
		{
			return await Task.Run(() =>
			{
				return installModloader();
			});
		}

		private bool uninstallModloader()
		{
			try
			{
				ModLoaderInfo.UninstallLoader(GameDirectory);
				return true;
			}
			catch
			{
				return false;
			}
		}

		public async Task<bool> UninstallModloader()
		{
			return await Task.Run(() =>
			{
				return uninstallModloader();
			});
		}

		#endregion

		#region Static
		private static Dictionary<GameIDs, GameConfig> gameConfigurations = new Dictionary<GameIDs, GameConfig>()
		{
			{ GameIDs.Unsupported, new GameConfig() },
			{ 
				GameIDs.SADX, new GameConfig()
				{
					GameID = GameIDs.SADX,
					GameExecutable = "sonic.exe",
					GameSystemFolder = "system",
					ModLoaderInfo = new ModLoaderInfo()
					{
						Filename = "SADXModLoader",
						InjectionFile = "CHRMODELS",
						Repository = "sadx-mod-loader",
						VersionFile = "sadxmlver.txt"
					},
					OneClickProtocol = "sadxmm:"
				}
			},
			{ 
				GameIDs.SA2B, new GameConfig()
				{
					GameID = GameIDs.SA2B,
					GameExecutable = "sonic2app.exe",
					GameSystemFolder = Path.Combine("resource", "gd_PC"),
					ModLoaderInfo = new ModLoaderInfo()
					{
						Filename = "SA2ModLoader",
						InjectionFile = Path.Combine("DLL", "Data_DLL"),
						Repository = "sa2-mod-loader",
						VersionFile = "sa2mlver.txt"
					},
					OneClickProtocol = "sa2mm:"
				}
			},
			{
				GameIDs.SHDR, new GameConfig()
				{
					GameID = GameIDs.SHDR,
					GameExecutable = "ShadowRecomp.exe",
					GameSystemFolder = Path.Combine(""),
					ModLoaderInfo = new ModLoaderInfo(),
					OneClickProtocol = "",
					HideConfig = true,
					HideTestSpawn = true
				}
			}
		};

		public static GameConfig GetGameConfig(GameIDs gameID, string gameDirectory)
		{
			if (gameConfigurations.ContainsKey(gameID))
			{
				GameConfig gameConfig = gameConfigurations[gameID];
				gameConfig.GameDirectory = gameDirectory;
				return gameConfig;
			}
			else
			{
				return gameConfigurations[GameIDs.Unsupported];
			}
		}

		#endregion
	}
}
