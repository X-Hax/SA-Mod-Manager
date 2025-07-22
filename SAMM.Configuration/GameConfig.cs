using SAMM.Configuration.Enumeration;
using System.Diagnostics;

namespace SAMM.Configuration
{
	public abstract class GameConfig
	{
		#region Variables
		public GameIDs GameID { get; set; } = GameIDs.Unsupported;

		public Uri? GameIcon { get; set; }

		public string GameDirectory { get; set; } = string.Empty;

		public string GameExecutable { get; set; } = string.Empty;

		private string gameSystemFolder = string.Empty;

		public string GameSystemFolder
		{
			get { return gameSystemFolder; }

			set { gameSystemFolder = Path.Combine(GameDirectory, value); }
		}

		public string GameModsFolder { get { return Path.Combine(GameDirectory, "mods"); } }

		public string ModloaderPath { get { return Path.Combine(GameDirectory, ".modloader"); } }

		public ModLoaderInfo? ModLoaderInfo { get; set; }

		public string OneClickProtocol { get; set; } = string.Empty;

		#endregion

		public virtual void LaunchGame(string cmds = "")
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

		abstract protected bool verifyExecutable();

		public async Task<bool> VerifyExecutable()
		{
			return await Task.Run(() =>
			{
				return verifyExecutable();
			});
		}

		abstract protected bool installModloader();

		public async Task<bool> InstallModloader()
		{
			return await Task.Run(() =>
			{
				return installModloader();
			});
		}

		abstract protected bool uninstallModloader();

		public async Task<bool> UninstallModloader()
		{
			return await Task.Run(() =>
			{
				return uninstallModloader();
			});
		}
	}
}
