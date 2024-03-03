using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using SAModManager.UI;
using System.IO.Compression;
using Microsoft.Win32;
using System.Security.Principal;
using SAModManager.Configuration.SADX;
using SAModManager.Configuration.SA2;
using SAModManager.Configuration;
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

        private static void MoveFilesToArchive(string root, string archive, List<string> FilesToMove)
        {
            foreach (var file in FilesToMove)
            {
                if (File.Exists(Path.Combine(root, file)))
                    File.Move(Path.Combine(root, file), Path.Combine(archive, file), true);
            }
        }

        public static void DoVanillaFilesCleanup(string[] args, int index)
        {
            string root = null;

            foreach (string exeName in Util.OldManagersName)
            {
                root = exeName;
                if (!File.Exists(root) && index + 1 < args.Length)
                {
                    root = Path.GetFullPath(Path.Combine(args[index + 1], exeName));

                    if (File.Exists(root))
                        break;
                }
            }

            if (File.Exists(root))
            {
                string archive = Path.Combine(root, "Archive_Old_Manager");
                Directory.CreateDirectory(archive);
                MoveFilesToArchive(root, archive, Util.SADXManagerFiles);
                MoveFilesToArchive(root, archive, Util.SA2ManagerFiles);
                MoveFilesToArchive(root, archive, Util.BASSFiles);
            }

        }

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

        public static void ConvertProfiles(string sourceFile, ref Profiles pro)
        {
            if (!File.Exists(sourceFile))
                return;

            Configuration.SADX.GameSettings settings = new();
            Configuration.SA2.GameSettings settingsSA2 = new();
            string newFileName = Path.GetFileNameWithoutExtension(sourceFile);
            string newFilePath = Path.Combine(App.CurrentGame.ProfilesDirectory, newFileName + ".json");

            try
            {
                switch (App.CurrentGame.id)
                {
                    case Configuration.SetGame.SADX:
                        SADXLoaderInfo info = IniSerializer.Deserialize<SADXLoaderInfo>(sourceFile);
                        settings.ConvertFromV0(info);
                        settings.Serialize(newFilePath, newFileName + ".json");
                        break;
                    case Configuration.SetGame.SA2:
                        SA2LoaderInfo sa2INFO = IniSerializer.Deserialize<SA2LoaderInfo>(sourceFile);
                        settingsSA2.ConvertFromV0(sa2INFO);
                        settingsSA2.Serialize(newFilePath, newFileName + ".json");
                        break;
                }

                pro.ProfilesList.Add(new ProfileEntry(newFileName, newFileName + ".json"));
                File.Delete(sourceFile);

            }
            catch (Exception ex)
            {
                Console.WriteLine($"Profile Conversion Failed: {ex.Message}");
                return;
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
            using (var archive = ArchiveFactory.Open(zipPath))
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
            await Task.Delay(100);
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
