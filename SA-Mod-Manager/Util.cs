using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows;
using SAModManager.Common;
using SAModManager.Updater;
using System.Diagnostics;
using System.IO.Compression;
using SAModManager.Ini;
using Microsoft.Win32;
using System.Security.Principal;

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
                newProfile.LoadConfigs();
                await Task.Run(() => newProfile.Serialize(destinationFile, "Default.json"));
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

        public static async Task<bool> ExtractEmbeddedDLL(byte[] resource, string resourceName, string outputDirectory)
        {
            string outputFilePath = null;
            // Get the resource stream from Properties.Resources
            using (Stream resourceStream = new MemoryStream(resource))
            {
                byte[] buffer = new byte[resourceStream.Length];
                resourceStream.Read(buffer, 0, buffer.Length);

                outputFilePath = Path.Combine(outputDirectory, resourceName + ".dll");

                // Write the DLL data to the output file
                using (FileStream fileStream = new(outputFilePath, FileMode.Create, FileAccess.Write))
                {
                    fileStream.Write(buffer, 0, buffer.Length);
                }
            }

            await Task.Delay(100);
            FileInfo fileInfo = new(outputFilePath);
            return fileInfo is not null && fileInfo.Length > 0;
        }

        public static async Task Install7Zip()
        {
            try
            {
                if (!IsZipToolInstalled())
                {
                    await Exec7zipInstall();
                }
            }
            catch
            {
                throw new Exception("What");
            }
        }

        public static bool IsZipToolInstalled()
        {
            string exePath = FindExePath("7z.exe");

            if (exePath != null)
                return true;

            exePath = FindExePathFromRegistry("SOFTWARE\\7-Zip");

            if (exePath != null)
                return true;

            exePath = Environment.GetEnvironmentVariable("PATH")
                .Split(';')
                .Select(s => Path.Combine(s, "7z.exe"))
                .FirstOrDefault(File.Exists);

            if (exePath != null)
                return true;

            if (Registry.LocalMachine.OpenSubKey("SOFTWARE\\WinRAR") != null ||
                RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64).OpenSubKey("SOFTWARE\\WinRAR") != null)
            {
                return true;
            }

            return false;
        }

        private static string FindExePath(string exeName)
        {
            string path = Path.Combine(App.StartDirectory, exeName);
            return File.Exists(path) ? path : null;
        }

        private static string FindExePathFromRegistry(string registryKeyPath)
        {
            var key = Registry.LocalMachine.OpenSubKey(registryKeyPath);

            if (key == null)
                key = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64)
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

                     var dl = new DownloadDialog(uri, "7-zip", "7z.exe", App.tempFolder, DownloadDialog.DLType.Download);
                     dl.StartDL();
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


        public static async Task ExtractArchive(string zipPath, string destFolder, bool overwright = false)
        {
            Directory.CreateDirectory(destFolder);

            if (Path.GetExtension(zipPath) == ".zip")
            {
                ZipFile.ExtractToDirectory(zipPath, destFolder, overwright);
                return;
            }

            if (await ExtractArchiveUsing7Zip(zipPath, destFolder) == false)
            {
                if (await ExtractArchiveUsingWinRAR(zipPath, destFolder) == false)
                {
                    await Exec7zipInstall();
                    await ExtractArchiveUsing7Zip(zipPath, destFolder);
                }
            }

            await Task.Delay(1000);
        }

        public static async Task<bool> ExtractArchiveUsing7Zip(string path, string dest)
        {
            // Check if file exists in the root folder of the Manager
            string exePath = FindExePath("7z.exe");

            //If it fails, Find the path from the registry
            exePath ??= FindExePathFromRegistry("SOFTWARE\\7-Zip");

            // If still not found, try with the PATH variable
            exePath ??=  Environment.GetEnvironmentVariable("PATH").Split(';').ToList()
                .Where(s => File.Exists(Path.Combine(s, "7z.exe"))).FirstOrDefault();

            if (exePath != null)
            {
                string exe = Path.GetFullPath(Path.Combine(exePath, "7z.exe"));

                if (File.Exists(exe))
                {
                    await Process.Start(new ProcessStartInfo(exe, $"x \"{path}\" -o\"{dest}\" -y")
                    {
                        UseShellExecute = true,
                    }).WaitForExitAsync();

                    return true;
                }
            }

            return false;
        }

        public static async Task<bool> ExtractArchiveUsingWinRAR(string path, string dest)
        {
            // Gets WinRAR's Registry Key
            var key = Registry.LocalMachine.OpenSubKey("SOFTWARE\\WinRAR");

            key ??= RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Registry64)
                    .OpenSubKey("SOFTWARE\\WinRAR");
            // Checks if WinRAR is installed by checking if the key and path value exists
            if (key != null && key.GetValue("exe64") is string exePath)
            {
                exePath = Path.GetFullPath(exePath);
                // Extracts the archive to the temp directory
                if (File.Exists(exePath))
                {
                    await Process.Start(new ProcessStartInfo(exePath, $"x \"{path}\" -IBCK \"{dest}\"")
                    {
                        UseShellExecute = true,
                    }).WaitForExitAsync();

                    key.Close();
                    return true;
                }
            }
            // WinRAR is not installed
            return false;
        }

        public static async Task Extract(string zipPath, string destFolder, bool overwright = false)
        {
            try
            {
                await ExtractArchive(zipPath, destFolder, overwright);
            }
            catch (Exception ex)
            {
                new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), ex.Message, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error).ShowDialog();
            }

            await Task.Delay(1000);
        }

        public static async Task<bool> ExtractZipFromResource(byte[] resource, string outputDirectory)
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
                await Task.Delay(100);
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
            if (!Directory.Exists(destinationPath))
            {
                Directory.CreateDirectory(destinationPath);
            }

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

        private static string CombinePathURL(params string[] paths)
        {
            return string.Join("/", paths);
        }

        public static void CheckLinux()
        {
            RegistryKey key = null;
            if ((key = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine, RegistryView.Default).OpenSubKey("SOFTWARE\\Wine")) != null)
            {
                key.Close();
                App.isLinux = true;
            }
        }

        public static bool RunningAsAdmin()
        {
            return new WindowsPrincipal(WindowsIdentity.GetCurrent()).IsInRole(WindowsBuiltInRole.Administrator);
        }
    }
}
