using ModManagerWPF.Updater;
using NetCoreInstallChecker;
using NetCoreInstallChecker.Structs.Config;
using NetCoreInstallChecker.Structs.Config.Enum;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Net.Mime.MediaTypeNames;

namespace ModManagerWPF
{

	public class Startup
	{

		static private readonly List<string> VCPaths = new()
		{
			Environment.SystemDirectory + "/vcruntime140.dll",
			Environment.SystemDirectory + "/msvcr120.dll",
			Environment.SystemDirectory + "/msvcr110.dll"
		};

		static private readonly List<string> VCURLs = new()
		{
			"https://aka.ms/vs/17/release/vc_redist.x86.exe",
			"https://aka.ms/highdpimfc2013x86enu",
			"https://download.microsoft.com/download/1/6/B/16B06F60-3B20-4FF2-B699-5E9B7962F9AE/VSU_4/vcredist_x86.exe"
		};

		private static async Task<bool> EnableOneClickInstall()
		{
			string execPath = AppDomain.CurrentDomain.BaseDirectory;
			await Process.Start(new ProcessStartInfo(execPath, "urlhandler") { UseShellExecute = true, Verb = "runas" }).WaitForExitAsync();
			return true;
		}

		private static async Task<bool> Net7Check()
		{
			var finder = new FrameworkFinder(Environment.Is64BitOperatingSystem);
			var resolver = new DependencyResolver(finder);
			var framework = new Framework("Microsoft.WindowsDesktop.App", "7.0.3");
			var options = new RuntimeOptions("net7.0", framework, RollForwardPolicy.Minor);
			var result = resolver.Resolve(options);

			/*if (!result.Available)
			{
				if (MessageBox.Show(".NET 7.0 Desktop Runtime is not installed! Would you like to install it now?", "SADX Mod Manager", MessageBoxButtons.YesNo, MessageBoxIcon.Warning) != DialogResult.Yes)
				{
					MessageBox.Show("SADX Mod Manager cannot run without .NET 7.", "SADX Mod Manager", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return false;
				}

				//FrameworkDownloader frameworkDownloader = new FrameworkDownloader(framework.NuGetVersion, framework.FrameworkName);
				//string t = await frameworkDownloader.GetDownloadUrlAsync(Environment.Is64BitOperatingSystem ? Architecture.Amd64 : Architecture.x86);
		
				var url = new Uri("https://download.visualstudio.microsoft.com/download/pr/342ba160-3776-4ffa-91dd-e3cd9dc0f817/ba649d6b80b27ca164d80bd488cdb51f/windowsdesktop-runtime-7.0.7-win-x64.exe\r\n");

				var dialog = new FolderBrowserDialog();

				DialogResult filePath = dialog.ShowDialog();

				if (filePath == DialogResult.OK)
				{
					// Create a FileStream to save the file
					await using var fileStream = new FileStream(dialog.SelectedPath, FileMode.Create);

					await HttpClientHelper.DownloadFileAsync(url, fileStream, CancellationToken.None, (bytesDownloaded, totalBytes) =>
					{
						var progress = (int)((bytesDownloaded / (double)totalBytes) * 100);
						Console.WriteLine($"Download progress: {progress}%");
					});

					Process.Start(new ProcessStartInfo("NET7Install.exe", "/install /passive /norestart") { UseShellExecute = true, Verb = "runas" }).WaitForExit();
				}
				else
				{
					MessageBox.Show("SADX Mod Manager cannot run without .NET 7.", "SADX Mod Manager", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return false;
				}
			}*/

			return true;
		}

		private static async Task<bool> VC_DependenciesCheck()
		{
			if (Environment.OSVersion.Platform >= PlatformID.Unix)
				return true;

			foreach (string dll in VCPaths)
			{
				if (!File.Exists(dll))
				{

					DialogResult dg = MessageBox.Show("It looks like you're missing some Visual C++ Redistributables, some mods won't work without them. Would you like to download and install them now?", "Missing VC++", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

					if (dg == DialogResult.Yes)
					{
						//Process.Start("https://aka.ms/vs/17/release/vc_redist.x86.exe");
						return false;
					}

				}
			}


			return false;
		}

		public async Task<bool> StartupCheck()
		{
			Console.WriteLine("Checking dependencies...");

			bool net7 = await Net7Check();

			if (net7)
			{
				bool OneClick = await EnableOneClickInstall();
				bool VC = await VC_DependenciesCheck();

				return true;
			}
	
			App.Current.Shutdown();
			return false;
		}



	}
}
