using ModManagerCommon;
using SAModManager.Common;
using SAModManager.Updater;
using NetCoreInstallChecker;
using NetCoreInstallChecker.Structs.Config;
using NetCoreInstallChecker.Structs.Config.Enum;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Security.Policy;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Net.Mime.MediaTypeNames;

namespace SAModManager
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
			try
			{
				string execPath = Environment.ProcessPath;
				await Process.Start(new ProcessStartInfo(execPath, "urlhandler") { UseShellExecute = true, Verb = "runas" }).WaitForExitAsync();
			}
			catch
			{
				return false;
			}

			return true;
		}

		private static async Task<bool> Net7Check()
		{
			var finder = new FrameworkFinder(Environment.Is64BitOperatingSystem);
			var resolver = new DependencyResolver(finder);
			var framework = new Framework("Microsoft.WindowsDesktop.App", "7.0.9");
			var options = new RuntimeOptions("net7.0", framework, RollForwardPolicy.Minor);
			var result = resolver.Resolve(options);

			if (!result.Available)
			{
				var res = new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Warning"), Lang.GetString("MessageWindow.Warnings.Net7Missing"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.YesNo);
				res.ShowDialog();

				if (res.isYes != true)
				{
					new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), Lang.GetString("MessageWindow.Errors.Net7Missing"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
					return false;
				}

				FrameworkDownloader frameworkDownloader = new(framework.NuGetVersion, framework.FrameworkName);
				var url = await frameworkDownloader.GetDownloadUrlAsync(Architecture.x86);
				Uri uri = new(url + "\r\n");

				if (url != null)
				{
					await DownloadAndInstallAsync(uri);
					return false;
				}

				new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), Lang.GetString("MessageWindow.Errors.Net7Missing"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
				return false;
			}

			return true;
		}

		private static async Task DownloadAndInstallAsync(Uri uri)
		{
			var DL = new GenericDownloadDialog(uri, "Net Core 7.0", "NET7Install.exe");
			DL.StartDL();
			DL.ShowDialog();

			// Asynchronous operation using async/await
			await Process.Start(new ProcessStartInfo(Path.Combine("SATemp", "NET7Install.exe"), "/install /passive /norestart")
			{
				UseShellExecute = true,
				Verb = "runas"
			}).WaitForExitAsync();
		}

		private static async Task<bool> VC_DependenciesCheck()
		{
			if (Environment.OSVersion.Platform >= PlatformID.Unix)
				return true;

			for (int i = 0; i < VCPaths.Count; i++)
			{
				if (!File.Exists(VCPaths[i]))
				{
					var dialog = new MessageWindow(Lang.GetString("MessageWindow.Errors.VCMissing.Title"), Lang.GetString("MessageWindow.Errors.VCMissing"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);
					dialog.ShowDialog();

					if (dialog.isYes)
					{
		
						Uri uri = new(VCURLs[i] + "\r\n");
						var DL = new GenericDownloadDialog(uri, "Visual C++", "vc_redist.x86.exe");
						DL.StartDL();
						DL.ShowDialog();

						// Asynchronous operation using async/await
						await Process.Start(new ProcessStartInfo(Path.Combine("SATemp", "vc_redist.x86.exe"), "/install /passive /norestart")
						{
							UseShellExecute = true,
							Verb = "runas"
						}).WaitForExitAsync();

						
						return false;
					}

				}
			}


			return true;
		}

		private static async Task<bool> UpdateDependenciesFolder()
		{
			string appDataLocalPath = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);
			string managerConfigFolderPath = Path.Combine(appDataLocalPath, "SAManager");
			string configPath = Path.Combine(managerConfigFolderPath, "config.ini");
			string extLibPath = Path.Combine(managerConfigFolderPath, "extlib");
			string bassFullPath = Path.Combine(managerConfigFolderPath, Path.Combine(extLibPath, "BASS"));
			string SDLFullPath = Path.Combine(managerConfigFolderPath, Path.Combine(extLibPath, "SDL2"));
			bool bassAndSDL = (Directory.Exists(bassFullPath) && Directory.Exists(SDLFullPath));

			try
			{
				//look if dependencies are already in appData folder
				if (bassAndSDL && File.Exists(configPath))
				{
					return true;
				}

				if (!Directory.Exists(managerConfigFolderPath))
				{
					Directory.CreateDirectory(managerConfigFolderPath);
				}

				if (!File.Exists(configPath)) //it's the first time the Mod Manager is launched, implement one click install.
				{
					await EnableOneClickInstall();
					File.Create(configPath);

					if (bassAndSDL)
						return true;
				}

				//if not, look if they aren't in the Mod Manager folder...
				if (!Directory.Exists("extlib/BASS"))
				{
					//throw error that they are missing
					return false;
				}

				if (!Directory.Exists("extlib/SDL2"))
				{
					//throw error that they are missing
					return false;
				}

				Util.CopyFolder("extlib/BASS", bassFullPath, true);
				Util.CopyFolder("extlib/SDL2/lib/x86", SDLFullPath, true);

				await Task.Delay(500);
			}
			catch
			{
				return false;
			}

			return true;
		}

		private static void ClearTempFolder()
		{
			try
			{
				if (Directory.Exists("SATemp"))
				{
					Directory.Delete("SATemp", true);
				}
			}
			catch { }
		}

		public static async Task<bool> StartupCheck()
		{
			Console.WriteLine("Checking dependencies...");

			bool net7 = await Net7Check();

			if (net7)
			{
				await UpdateDependenciesFolder();

				if (await VC_DependenciesCheck() == false)
					return false;

				ClearTempFolder();

				return true;
			}

			App.Current.Shutdown();
			return false;
		}
	}
}
