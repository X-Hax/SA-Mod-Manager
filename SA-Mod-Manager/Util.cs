using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using SAModManager.UI;
using SAModManager.Updater;
using System.Diagnostics;
using System.IO.Compression;
using Microsoft.Win32;
using System.Security.Principal;
using System.Reflection;
using NetCoreInstallChecker.Structs.Config;
using NetCoreInstallChecker;
using NetCoreInstallChecker.Structs.Config.Enum;
using SharpCompress.Archives;
using SharpCompress.Common;
using SharpCompress.Readers;

namespace SAModManager
{
    class Util
    {
        public static List<string> SADXManagerFiles = new()
        {
            "7z.dll",
            "7z.exe",
            "ModManagerCommon.dll",
            "ModManagerCommon.pdb",
            "d3d8m.dll",
            "SADXModManager.exe.config",
            "SADXModManager.pdb",
            "sadxmlver.txt",
            "loader.manifest",
            "SharpDX.dll",
            "SharpDX.DirectInput.dll",
            "COPYING_D3D8TO9",
            "Newtonsoft.Json.dll",
            "SADXModManager.exe",
        };

        public static List<string> BASSFiles = new()
        {
            "libogg.dll",
            "libvorbisfile.dll",
            "avcodec-vgmstream-58.dll",
            "avformat-vgmstream-58.dll",
            "avutil-vgmstream-56.dll",
            "libatrac9.dll",
            "libcelt-0061.dll",
            "libcelt-0110.dll",
            "libg719_decode.dll",
            "libmpg123-0.dll",
            "libvorbis.dll",
            "bass.dll",
            "jansson.dll",
            "libg7221_decode.dll",
            "libspeex.dll",
            "swresample-vgmstream-3.dll",
            "bass_vgmstream.dll",
            "COPYING_BASS_VGMSTREAM",
            "COPYING_VGMSTREAM",
        };

        public static List<string> SA2ManagerFiles = new()
        {
            "7z.dll",
            "7z.exe",
            "ModManagerCommon.dll",
            "ModManagerCommon.pdb",
            "SA2ModManager.exe.config",
            "SA2ModManager.pdb",
            "SA2ModLoader.exp",
            "sa2mlver.txt",
            "loader.manifest",
            "Newtonsoft.Json.dll",
            "SA2ModManager.exe",
        };

        public static List<string> OldManagersName = new()
        {
            "SADXModManager.exe",
            "SA2ModManager.exe",
            "SonicRModManager.exe"
        };

        private static void DeleteLegacyFiles(string root, List<string> LegacyFiles)
        {
            try
            {
                foreach (var file in LegacyFiles)
                {
                    if (File.Exists(Path.Combine(root, file)))
                        File.Delete(Path.Combine(root, file));
                }
            }
            catch { }
        }

        public static void DoVanillaFilesCleanup(string[] args)
        {
            string root = null;

            bool foundGameFolder = false;
            foreach (string exeName in Util.OldManagersName)
            {
                root = exeName;

                for (int i = 0; i < args.Length; i++)
                {
                    if (File.Exists(root))
                    {
                        foundGameFolder = true;
                        break;
                    }
                    else
                    {
                        root = Path.GetFullPath(Path.Combine(args[i], exeName));

                        if (File.Exists(root))
                        {
                            foundGameFolder = true;
                            break;
                        }
                    }
                }

                if (foundGameFolder)
                    break;
            }

            if (File.Exists(root))
            {
                var gameDir = Path.GetDirectoryName(root);
                DeleteLegacyFiles(gameDir, Util.SADXManagerFiles);
                DeleteLegacyFiles(gameDir, Util.SA2ManagerFiles);
                DeleteLegacyFiles(gameDir, Util.BASSFiles);
            }
        }

        public static async Task<bool> MoveFileAsync(string sourceFile, string destinationFile, bool overwrite)
        {
            if (File.Exists(destinationFile) && overwrite == false)
            {
                Console.WriteLine($"Skipped copy, file already exist.", destinationFile);
                return false;
            }


            try
            {
                await Task.Run(() => File.Move(sourceFile, destinationFile, overwrite));
                return true; // File move completed successfully
            }
            catch (Exception ex)
            {
                Logger.Log("Failed to move File." + ex.Message);
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
                Logger.Log("failed to copy file!");
                Console.WriteLine($"File copy failed: {ex.Message}");
                return false; // File copy failed
            }
        }

        public static async Task MoveFile(string origin, string dest, bool overwrite = false)
        {
            try
            {
                Logger.Log("Moving File: " + origin + " to " + dest);
                if (await MoveFileAsync(origin, dest, overwrite) == false)
                {
                    Logger.Log("Attempting to copy the file instead...");
                    if (await CopyFileAsync(origin, dest, overwrite))
                    {
                        Logger.Log("Success!");
                        File.Delete(origin);
                    }
                }
            }
            catch //File.Move doesn't work if hard drive destination is different from source, copy doesn't have this problem
            {
                Logger.Log("Attempting to copy the file instead...");
                if (await CopyFileAsync(origin, dest, overwrite))
                {
                    Logger.Log("Success!");
                    File.Delete(origin);
                }
            }
        }

		public static void CopyAllFiles(string sourceDirectory, string destinationDirectory, bool overwrite = true)
		{
			if (Directory.Exists(sourceDirectory))
			{
				Directory.CreateDirectory(destinationDirectory);
				DirectoryInfo sDir = new DirectoryInfo(sourceDirectory);
				DirectoryInfo dDir = new DirectoryInfo(destinationDirectory);

				foreach (FileInfo fileInfo in sDir.GetFiles())
				{
					string destName = Path.Combine(dDir.FullName, fileInfo.Name);
					fileInfo.CopyTo(destName, true);
				}

				foreach (DirectoryInfo subDir in sDir.GetDirectories())
				{
					CopyAllFiles(subDir.FullName, Path.Combine(destinationDirectory, subDir.Name), overwrite);
				}
			}
		}

        public static bool ExtractEmbeddedDLL(byte[] resource, string resourceName, string outputDirectory)
        {
            string outputFilePath = null;
            try
            {
                Util.CreateSafeDirectory(outputDirectory);
                outputFilePath = Path.Combine(outputDirectory, resourceName + ".dll");

                // Get the resource stream from Properties.Resources
                using Stream resourceStream = new MemoryStream(resource);
                using (FileStream fileStream = new(outputFilePath, FileMode.Create, FileAccess.Write))
                {
                    // Asynchronously copy the stream to the output file
                    resourceStream.CopyTo(fileStream);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Exception during DLL extraction: {ex}");
                return false;
            }

            FileInfo fileInfo = new(outputFilePath);
            return fileInfo is not null && fileInfo.Length > 0;
        }

        private static string FindExePath(string exeName)
        {
            string path = Path.Combine(App.StartDirectory, exeName);
            return File.Exists(path) ? path : null;
        }

        private static string FindExePathFromRegistry(string registryKeyPath)
        {
            var key = Registry.LocalMachine.OpenSubKey(registryKeyPath);

            key ??= RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64)
                    .OpenSubKey(registryKeyPath);

            return key?.GetValue("Path") as string;
        }

        public static async Task Exec7zipInstall()
        {

            await Application.Current.Dispatcher.InvokeAsync(() =>
             {
                 var msg = new MessageWindow(Lang.GetString("CommonStrings.Warning"), Lang.GetString("MessageWindow.Warnings.7zMissing"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);
                 msg.ShowDialog();

                 if (msg.isYes)
                 {
                     bool is64Bits = Environment.Is64BitOperatingSystem;
                     Uri uri = new(is64Bits ? "https://www.7-zip.org/a/7z2301-x64.exe" : "https://www.7-zip.org/a/7z2301.exe" + "\r\n");
                     var info = new List<DownloadInfo>
                     {
                         new DownloadInfo("7-zip", "7z.exe", App.tempFolder, uri, DownloadDialog.DLType.Download)
                     };

                     var dl = new DownloadDialog(info);
                     dl.StartDL();
                 }
                 else
                 {
                     return;
                 }
             });

            await Process.Start(new ProcessStartInfo(Path.Combine(App.tempFolder, "7z.exe"), "/S")
            {
                UseShellExecute = true,
                Verb = "runas"
            }).WaitForExitAsync();
            await Task.Delay(100);
        }

        public static void ClearTempFolder()
        {
            Util.DeleteReadOnlyDirectory(App.tempFolder);
            if (Directory.Exists(".SATemp")) //temp support for previous version but no longer used
                Util.DeleteReadOnlyDirectory(".SATemp");
        }

        public static void DeleteReadOnlyDirectory(string dir)
        {
            if (Directory.Exists(dir))
                DeleteReadOnlyDirectory(new DirectoryInfo(dir));
        }

        public static void DeleteReadOnlyDirectory(DirectoryInfo dir)
        {
            // Recursively perform this function 
            foreach (var subDir in dir.GetDirectories())
                DeleteReadOnlyDirectory(subDir);

            // Delete all files in the directory and remove readonly
            foreach (var file in dir.GetFiles())
            {
                try
                {
                    file.Attributes = FileAttributes.Normal;
                    file.Delete();
                }
                catch { }
            }

            try
            {
                // remove readonly from the directory
                dir.Attributes = FileAttributes.Normal;
                // Delete the directory
                dir.Delete();
            }
            catch { }
        }

        private static async Task<bool> ExtractWithSharpCompressSpecificFolder(string zipPath, string specificFolder, string destFolder)
        {
            try
            {
                await Task.Run(() =>
                {
                    using var archive = ArchiveFactory.Open(zipPath);

                    foreach (var entry in archive.Entries)
                    {
                        if (!entry.IsDirectory && entry.Key.StartsWith(specificFolder, StringComparison.OrdinalIgnoreCase))
                        {
                            string destinationPath = Path.Combine(destFolder, entry.Key);
                            Util.CreateSafeDirectory(Path.GetDirectoryName(destinationPath));
                            entry.WriteToFile(destinationPath, new ExtractionOptions()
                            {
                                ExtractFullPath = true,
                                Overwrite = true
                            });
                        }
                    }
                });


                return true;
            }
            catch
            {
                return false;
            }
        }

        private static async Task<bool> ExtractWithSharpCompress(string zipPath, string destFolder, List<string> excludeFolder = null)
        {

            try
            {
                await Task.Run(() =>
                {
                    using var archive = ArchiveFactory.Open(zipPath);

                    if (excludeFolder is null)
                    {
                        using (var reader = archive.ExtractAllEntries())
                        {
                            reader.WriteAllToDirectory(destFolder, new ExtractionOptions()
                            {
                                ExtractFullPath = true,
                                Overwrite = true

                            });
                        }

                    }
                    else
                    {

                        foreach (var entry in archive.Entries)
                        {
                            bool skip = false;
                            foreach (string exclude in excludeFolder)
                            {
                                if (entry.Key.StartsWith(exclude, StringComparison.OrdinalIgnoreCase))
                                {
                                    skip = true;
                                }
                            }

                            if (skip)
                                continue;

                            if (!entry.IsDirectory)
                            {
                                string destinationPath = Path.Combine(destFolder, entry.Key);
                                Util.CreateSafeDirectory(Path.GetDirectoryName(destinationPath));
                                entry.WriteToFile(destinationPath);
                            }
                        }

                    }
                });

                return true;
            }
            catch
            {
                return false;
            }
        }

        public static async Task ExtractArchive(string zipPath, string destFolder, List<string> excludeFolder = null, bool overwrite = false)
        {
            Util.CreateSafeDirectory(destFolder);

            if (Path.GetExtension(zipPath) == ".zip")
            {
                ZipFile.ExtractToDirectory(zipPath, destFolder, overwrite);
                return;
            }

            if (await ExtractWithSharpCompress(zipPath, destFolder, excludeFolder) == false)
            {
                if (await ExtractArchiveUsing7Zip(zipPath, destFolder, excludeFolder) == false)
                {
                    await Exec7zipInstall();
                    await ExtractArchiveUsing7Zip(zipPath, destFolder, excludeFolder);

                }
            }

            await Task.Delay(150);
        }

        public static async Task ExtractSpecificFile(string zipPath, string specificFile, string destFolder)
        {
            Util.CreateSafeDirectory(destFolder);

            if (await ExtractWithSharpCompressSpecificFolder(zipPath, specificFile, destFolder) == false)
            {
                if (await ExtractArchiveUsing7Zip(zipPath, destFolder, null, specificFile) == false)
                {
                    await Exec7zipInstall();
                    await ExtractArchiveUsing7Zip(zipPath, destFolder, null, specificFile);

                }
            }

            await Task.Delay(150);
        }

        public static async Task<bool> ExtractArchiveUsing7Zip(string path, string dest, List<string> excludeFolder = null, string specificFile = null)
        {
            // Check if file exists in the root folder of the Manager
            string exePath = FindExePath("7z.exe");

            //If it fails, Find the path from the registry
            exePath ??= FindExePathFromRegistry("SOFTWARE\\7-Zip");

            // If still not found, try with the PATH variable
            exePath ??= Environment.GetEnvironmentVariable("PATH").Split(';').ToList()
                .Where(s => File.Exists(Path.Combine(s, "7z.exe"))).FirstOrDefault();

            if (exePath != null)
            {
                string exe = Path.GetFullPath(Path.Combine(exePath, "7z.exe"));

                if (File.Exists(exe))
                {
                    string fullLine = $"x \"{path}\" -o\"{dest}\" -y";
                    if (excludeFolder is not null && excludeFolder.Count > 0)
                    {
                        foreach (var banned in excludeFolder)
                        {
                            fullLine += $" -xr!\"{banned}\"";
                        }
                    }
                    else if (Util.IsStringValid(specificFile))
                    {
                        fullLine += $" -ir!\"{specificFile}\\*\"";
                    }

                    await Process.Start(new ProcessStartInfo(exe, fullLine)
                    {
                        UseShellExecute = true,
                        CreateNoWindow = true,
                        WindowStyle = ProcessWindowStyle.Hidden
                    }).WaitForExitAsync();

                    return true;
                }
            }

            return false;
        }

        public static async Task Extract(string zipPath, string destFolder, bool overwrite = false)
        {
            try
            {
                await ExtractArchive(zipPath, destFolder, null, overwrite);
            }
            catch (Exception ex)
            {
                new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), ex.Message, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error).ShowDialog();
            }

            await Task.Delay(100);
        }

        public static async Task ExtractWExcludeFile(string zipPath, string destFolder, List<string> excludeList)
        {
            try
            {
                await ExtractArchive(zipPath, destFolder, excludeList);
            }
            catch (Exception ex)
            {
                new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), ex.Message, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error).ShowDialog();
            }

            await Task.Delay(100);
        }

        public static bool ExtractZipFromResource(byte[] resource, string outputDirectory)
        {
            try
            {
                using (MemoryStream zipStream = new(resource))
                {
                    using (ZipArchive archive = new(zipStream, ZipArchiveMode.Read))
                    {
                        archive.ExtractToDirectory(outputDirectory, true);
                    }
                }
                return true;

            }
            catch
            { }

            return false;
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

        public static void MoveDirectory(string sourcePath, string destinationPath)
        {

            CreateSafeDirectory(destinationPath);

            foreach (string file in Directory.GetFiles(sourcePath))
            {
                string destFile = Path.Combine(destinationPath, Path.GetFileName(file));
                File.Copy(file, destFile, true); // Set overwrite to true to allow overwriting if the file already exists
            }

            foreach (string subDirectory in Directory.GetDirectories(sourcePath))
            {
                string destDirectory = Path.Combine(destinationPath, Path.GetFileName(subDirectory));
                MoveDirectory(subDirectory, destDirectory);
            }

            Directory.Delete(sourcePath, true);
        }

        public static bool MoveAllFilesAndSubfolders(string sourceFolderPath, string destinationFolderPath, string exception = null)
        {
            // Move all files in the current folder to the destination subfolder
            string[] files = Directory.GetFiles(sourceFolderPath);
            foreach (string file in files)
            {
                if (exception is not null && exception.ToLower().Contains(file.ToLower()))
                    continue;

                string fileName = Path.GetFileName(file);
                string destinationFilePath = Path.Combine(destinationFolderPath, fileName);

                File.Move(file, destinationFilePath, true);
            }

            // Move all subfolders in the current folder to the destination subfolder
            string[] subfolders = Directory.GetDirectories(sourceFolderPath);
            foreach (string subfolder in subfolders)
            {
                if (exception is not null && exception.ToLower().Contains(subfolder.ToLower()))
                    continue;

                string subfolderName = Path.GetFileName(subfolder);
                string destinationSubfolderPath = Path.Combine(destinationFolderPath, subfolderName);

                // Recursively move the subfolder and its contents to the destination subfolder
                if (Directory.Exists(destinationSubfolderPath))
                {
                    MoveAllFilesAndSubfolders(subfolder, destinationSubfolderPath, destinationSubfolderPath);
                }
                else
                {
                    MoveDirectory(subfolder, destinationSubfolderPath);
                }
            }

            return true;
        }

        public static async Task<bool> Net8Check()
        {
            var finder = new FrameworkFinder(Environment.Is64BitOperatingSystem);
            var resolver = new DependencyResolver(finder);
            var framework = new Framework("Microsoft.WindowsDesktop.App", "8.0.7");
            var options = new RuntimeOptions("net8.0", framework, RollForwardPolicy.Minor);
            var result = resolver.Resolve(options);

            if (!result.Available)
            {
                var res = new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Warning"), Lang.GetString("MessageWindow.Warnings.Net8Missing"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);
                res.ShowDialog();

                if (res.isYes != true)
                {
                    return false;
                }

                FrameworkDownloader frameworkDownloader = new(framework.NuGetVersion, framework.FrameworkName);
                var url = await frameworkDownloader.GetDownloadUrlAsync(Environment.Is64BitOperatingSystem ? Architecture.Amd64 : Architecture.x86);
                Uri uri = new(url + "\r\n");

                if (url != null)
                {
                    string name = "aspnetcore-runtime-8.0.exe";
                    string fullPath = Path.GetFullPath(Path.Combine(App.tempFolder, name));
                    var netDL = new List<DownloadInfo>
                    {
                        new("Net Core 8.0", name, App.tempFolder, uri, DownloadDialog.DLType.Download)
                    };
                    var DL = new DownloadDialog(netDL);
                    DL.StartDL();

                    if (File.Exists(fullPath))
                        Process.Start(fullPath);
                }

                return false;
            }

            return true;
        }

        private static string CombinePathURL(params string[] paths)
        {
            return string.Join("/", paths);
        }

        private static void UpdatePathsForPortableMode()
        {
            if (App.isLinux && Directory.Exists(App.extLibPath) == false) //force portable mode for new users on Linux since it tends to work better.
            {
                App.ConfigFolder = Path.Combine(App.StartDirectory, "SAManager");
                App.extLibPath = Path.Combine(App.ConfigFolder, "extlib");
                App.crashFolder = Path.Combine(App.ConfigFolder, "CrashDump");
            }
        }

        public static void CheckLinux()
        {
            RegistryKey key;
            if ((key = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Default).OpenSubKey("SOFTWARE\\Wine")) != null)
            {
                key.Close();
                App.isLinux = true;
                Logger.Log("Linux detected with first method");
                UpdatePathsForPortableMode();
                return;
            }

            // Method 2: Check for Wine Environment Variables
            string winePrefix = Environment.GetEnvironmentVariable("WINEPREFIX");
            if (!string.IsNullOrEmpty(winePrefix))
            {
                App.isLinux = true;
                Logger.Log("Linux detected with second method");
                UpdatePathsForPortableMode();
                return;
            }

            // Method 3: Check for Common Linux Paths
            string[] commonLinuxPaths = { "/usr/bin/wine", "/usr/bin/steam", "/opt/steam" };
            foreach (string path in commonLinuxPaths)
            {
                if (File.Exists(path))
                {
                    App.isLinux = true;
                    Logger.Log("Linux detected with last method, wow that was close.");
                    UpdatePathsForPortableMode();
                    return;
                }
            }

            Logger.Log("Linux wasn't detected, the Manager will act like we are on Windows.");
        }

        public static bool RunningAsAdmin()
        {
            return new WindowsPrincipal(WindowsIdentity.GetCurrent()).IsInRole(WindowsBuiltInRole.Administrator);
        }

        public static void RequestAdminPrivileges()
        {
            var processInfo = new ProcessStartInfo
            {
                UseShellExecute = true,
                WorkingDirectory = Environment.CurrentDirectory,
                FileName = Assembly.GetEntryAssembly().Location,
                Verb = "runas" // This will prompt for admin rights
            };

            try
            {
                Process.Start(processInfo);
                Environment.Exit(0); // Exit the current instance
            }
            catch (System.ComponentModel.Win32Exception)
            {
                // The user refused the elevation
                Console.WriteLine("The application requires administrative privileges to continue.");
            }
        }

        public static void CreateSafeDirectory(string path)
        {
            try
            {
                if (!string.IsNullOrEmpty(path) && !Directory.Exists(path))
                {
                    Directory.CreateDirectory(path);
                }
            }
            catch (UnauthorizedAccessException ex)
            {
                new MessageWindow(Lang.GetString("MessageWindow.Errors.PermissionTitle"), string.Format(Lang.GetString("MessageWindow.Errors.FolderPermission"), path) + "\n\n" + ex.Message, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
            }
        }

        public static void AdjustPathForLinux(ref string s)
        {
            if (App.isLinux && s.StartsWith("/"))
                s = $"Z:{s}";
        }

        public static bool IsStringValid(string str)
        {
            return !string.IsNullOrEmpty(str) && !string.IsNullOrWhiteSpace(str);
        }
    }
}
