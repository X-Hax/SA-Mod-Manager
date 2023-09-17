using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Input;
using System.Threading;
using Microsoft.Win32;
using System.Text.RegularExpressions;
using SAModManager.Common;
using SAModManager.Updater;
using System.Diagnostics;
using SevenZipExtractor;
using System.IO.Compression;
using SAModManager.Ini;

namespace SAModManager
{
	class Util
	{
        public static async Task<bool> MoveFileAsync(string sourceFile, string destinationFile, bool overwrite)
		{
			try
			{
				await Task.Run(() => File.Move(sourceFile, destinationFile, overwrite));
				return true; // File move completed successfully
			}
			catch (Exception ex)
			{
				Console.WriteLine($"File move failed: {ex.Message}");
				return false; // File move failed
			}
		}

		public static async Task<bool> CopyFileAsync(string sourceFile, string destinationFile, bool overwrite)
		{
			try
			{
				await Task.Run(() => File.Copy(sourceFile, destinationFile, overwrite));
				return true; // File copy completed successfully
			}
			catch (Exception ex)
			{
				Console.WriteLine($"File copy failed: {ex.Message}");
				return false; // File copy failed
			}
		}

		public static async Task<bool> ConvertProfiles(string sourceFile, string destinationFile)
		{
			try
			{
				// TODO: Add switch case to properly do the new config for either game.
				Configuration.SADX.GameSettings newProfile = new();
				newProfile.ConvertFromV0(IniSerializer.Deserialize<SADXLoaderInfo>(sourceFile));
				await Task.Run(() => newProfile.Serialize(destinationFile));
                return true;
			}
			catch (Exception ex)
			{
				Console.WriteLine($"Profile Conversion Failed: {ex.Message}");
				return false;
			}
		}

		public static async Task MoveFile(string origin, string dest, bool overwrite = false)
		{
			try
			{
				await MoveFileAsync(origin, dest, overwrite);
			}
			catch //File.Move doesn't work if hard drive destination is different from source, copy doesn't have this problem
			{
				if (await CopyFileAsync(origin, dest, overwrite))
				{
					File.Delete(origin);
				}
			}
		}

		public static async Task ExtractEmbeddedDLL(byte[] resource, string resourceName, string outputDirectory)
		{
			// Get the resource stream from Properties.Resources
			using (Stream resourceStream = new MemoryStream(resource))
			{
				byte[] buffer = new byte[resourceStream.Length];
				resourceStream.Read(buffer, 0, buffer.Length);

				string outputFilePath = Path.Combine(outputDirectory, resourceName + ".dll");

				// Write the DLL data to the output file
				using (FileStream fileStream = new(outputFilePath, FileMode.Create, FileAccess.Write))
				{
					fileStream.Write(buffer, 0, buffer.Length);
				}
			}

			await Task.Delay(100);
		}

		public static async Task ExtractZipFromResource(byte[] resource, string outputDirectory)
		{
			using (MemoryStream zipStream = new(resource))
			{
				using (ZipArchive archive = new(zipStream, ZipArchiveMode.Read))
				{
					archive.ExtractToDirectory(outputDirectory, true);
				}
			}

			await Task.Delay(100);
		}

		public static void CopyFolder(string origin, string dest, bool dllCheck = false)
		{
			DirectoryInfo sourceDirectory = new(origin);
			DirectoryInfo destinationDirectory = new(dest);

			if (!destinationDirectory.Exists)
			{
				destinationDirectory.Create();
			}

			foreach (FileInfo file in sourceDirectory.GetFiles())
			{
				string destinationFilePath = Path.Combine(dest, file.Name);
				if (dllCheck)
				{
					string ext = Path.GetExtension(destinationFilePath);
					if (ext.ToLower() == ".dll")
						file.CopyTo(destinationFilePath, true);
				}

			}

			foreach (DirectoryInfo subDir in sourceDirectory.GetDirectories())
			{
				string destinationSubDir = Path.Combine(dest, subDir.Name);
				CopyFolder(subDir.FullName, destinationSubDir);
			}
		}

		public static double SetProgress(double value)
		{
			return (int)(value * 100);
		}

		public static string GetFileCountString(int fileCount, string s)
		{
			string fileString = Lang.GetString(s);
			return $"{fileCount} {fileString}";
		}

		public static bool MoveAllFilesAndSubfolders(string sourceFolderPath, string destinationFolderPath, string exception = null)
		{
			// Move all files in the current folder to the destination subfolder
			string[] files = Directory.GetFiles(sourceFolderPath);
			foreach (string file in files)
			{
				if (exception.ToLower().Contains(file.ToLower()))
					continue;

				string fileName = Path.GetFileName(file);
				string destinationFilePath = Path.Combine(destinationFolderPath, fileName);

				File.Move(file, destinationFilePath);
			}

			// Move all subfolders in the current folder to the destination subfolder
			string[] subfolders = Directory.GetDirectories(sourceFolderPath);
			foreach (string subfolder in subfolders)
			{
				if (exception.ToLower().Contains(subfolder.ToLower()))
					continue;

				string subfolderName = Path.GetFileName(subfolder);
				string destinationSubfolderPath = Path.Combine(destinationFolderPath, subfolderName);

				// Recursively move the subfolder and its contents to the destination subfolder
				Directory.Move(subfolder, destinationSubfolderPath);
			}

			return true;
		}

		public async static Task<string> GetSADXGamePath()
		{
			//Look for the steam version..
			var fullPath = Steam.GetGamePath(GamesInstall.SonicAdventure, true);

			if (Directory.Exists(fullPath))
			{
				//if steam version is found, but not converted to 2004, ask the user to convert it.
				if (File.Exists(Path.Combine(fullPath, GamesInstall.SonicAdventure.exeList[1])) && !File.Exists(Path.Combine(fullPath, GamesInstall.SonicAdventure.exeList[0])))
				{
					//Disabled for now, ToDo rewrite Mod Installer

					/*var msg = new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle"), Lang.GetString("MessageWindow.Information.SADXSteamDetected"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);
					msg.ShowDialog();

					if (msg.isYes)
					{
						await GamesInstall.GetSADXModInstaller();

					}*/

					return null;
				}
			}

			return fullPath;
		}

        private static string CombinePathURL(params string[] paths)
        {
            return string.Join("/", paths);
        }
    }
}
