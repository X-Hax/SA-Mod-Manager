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
using System.Net.Http;
using System.Buffers;

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
                if (!is7ZipInstalled())
                {
                    string path = Path.Combine(App.extLibPath, "7z");
                    Directory.CreateDirectory(path);
                    await Exec7zipInstall();
                }
            }
            catch
            {
                throw new Exception("What");
            }
        }

        public static bool is7ZipInstalled()
        {

            if (File.Exists(App.ziplibPath))
            {
                return true;
            }

            string currentArchitecture = IntPtr.Size == 4 ? "x86" : "x64"; // magic check

            if (File.Exists(Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "7z-" + currentArchitecture + ".dll")))
            {
                return true;
            }
            else if (File.Exists(Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "bin", "7z-" + currentArchitecture + ".dll")))
            {
                return true;
            }
            else if (File.Exists(Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "bin", currentArchitecture, "7z.dll")))
            {
                return true;
            }
            else if (File.Exists(Path.Combine(AppDomain.CurrentDomain.BaseDirectory, currentArchitecture, "7z.dll")))
            {
                return true;
            }
            else if (File.Exists(Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles), "7-Zip", "7z.dll")))
            {
                return true;
            }

            return false;
        }

        public static async Task<bool> Exec7zipInstall()
        {
            var msg = new MessageWindow(Lang.GetString("CommonStrings.Warning"), Lang.GetString("MessageWindow.Warnings.7zMissing"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);
            msg.ShowDialog();

            if (msg.isYes)
            {
                bool is64Bits = Environment.Is64BitOperatingSystem;
                Uri uri = new(is64Bits ? Properties.Resources.URL_7Z_x64 : Properties.Resources.URL_7Z_x86 + "\r\n");

                var dl = new DownloadDialog(uri, "7-zip", "7z.dll", Path.Combine(App.extLibPath, "7z"), DownloadDialog.DLType.Download);

                dl.StartDL();
                await Task.Delay(10);

                return dl.done;
            }

            return false;
        }

        public static async Task Extract(string zipPath, string destFolder, bool overwright = false)
        {
            try
            {
                if (!is7ZipInstalled())
                {
                    if (await Exec7zipInstall() == false)
                    {
                        return;
                    }
                }

                string libPath = File.Exists(App.ziplibPath) ? App.ziplibPath : null;
                using (ArchiveFile archiveFile = new(zipPath, libPath))
                {
                    archiveFile.Extract(destFolder, overwright);
                }
            }
            catch (Exception ex)
            {
                throw new Exception(ex.Message);
            }
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
                    Directory.Move(subfolder, destinationSubfolderPath);
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
    }
}
