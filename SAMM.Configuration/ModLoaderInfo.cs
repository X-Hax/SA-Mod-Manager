using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAMM.Configuration
{
	public class ModLoaderInfo
	{
		#region Variables
		/// <summary>
		/// Name of the ModLoader (without the extension)
		/// </summary>
		public string Filename { get; set; } = string.Empty;

		/// <summary>
		/// Name of the file the ModLoader will replace (without the extension).
		/// </summary>
		public string InjectionFile { get; set; } = string.Empty;

		public string Repository { get; set; } = string.Empty;

		public string VersionFile { get; set; } = string.Empty;

		#endregion

		#region Functions
		/// <summary>
		/// Installs the loader over the injection point for the game. 
		/// 
		/// Will create a backup of the injection file if the backup is not already made and the createBackup bool is set to true.
		/// </summary>
		/// <param name="modloaderDirectory"></param>
		/// <param name="gameSystemDirectory"></param>
		/// <param name="createBackup"></param>
		public void InstallLoader(string modloaderDirectory, string gameSystemDirectory, bool createBackup = true)
		{
			// Set the full paths for the relevant files.
			string loaderFilepath = Path.Combine(modloaderDirectory, Filename + ".dll");
			string injectionFilepath = Path.Combine(gameSystemDirectory, InjectionFile + ".dll");
			string origInjectionFilepath = Path.Combine(gameSystemDirectory, InjectionFile + "_orig.dll");

			// Copy the injection file. We don't overwrite in the event this "orig" file already exists and this still gets run.
			if (createBackup)
				File.Copy(injectionFilepath, origInjectionFilepath, false);

			// Copy the loader file over to the injection file.
			File.Copy(loaderFilepath, injectionFilepath, true);
		}

		/// <summary>
		/// Restores the "orig" copy of the injection point and deletes the backup. This removes the ModLoader's injection.
		/// </summary>
		/// <param name="gameSystemDirectory"></param>
		public void UninstallLoader(string gameSystemDirectory)
		{
			string injectionFilepath = Path.Combine(gameSystemDirectory, InjectionFile + ".dll");
			string origInjectionFilepath = Path.Combine(gameSystemDirectory, InjectionFile + "_orig.dll");

			if (File.Exists(origInjectionFilepath))
			{
				File.Copy(origInjectionFilepath, injectionFilepath, true);
				File.Delete(origInjectionFilepath);
			}
		}

		public async Task UpdateLoader()
		{
			await Task.Run(() =>
			{
				// TODO: Finish this.
				string url = $"https://github.com/X-Hax/{Repository}";
			});
		}

		public void CheckVersion()
		{
			// TODO: Write this.
		}

		#endregion
	}
}
