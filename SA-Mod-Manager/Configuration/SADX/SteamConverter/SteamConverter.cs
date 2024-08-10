using System.Collections.Generic;
using System.IO;
using System;

//todo add sharpHDDiff patch back 
namespace SAModManager.Configuration.SADX
{
    public partial class SteamConverter
    {
        public enum ConversionResult
        {
            Success = 0,
            AlreadyPatched = 1,
            Error = 2,
            Unsupported = 3
        }

        private class PatchHashData
        {
            public string checksum;
            public string filename;

            public PatchHashData(string MD5Sum, string Filename)
            {
                checksum = MD5Sum;
                filename = Filename;
            }
        }

        private class PatchData
        {
            public string GameName;
            public string PatchFilename;
            public PatchHashData[] FileHashes;

            public PatchData(string game, string patch, PatchHashData[] hashes)
            {
                GameName = game;
                PatchFilename = patch;
                FileHashes = hashes;
            }
        }

        private static string FileHash(string path, int rangeStart = 0, int rangeFinish = 0) { return FileHash(File.ReadAllBytes(path), rangeStart, rangeFinish); }

        // From SA Tools
        private static string FileHash(byte[] file, int rangeStart = 0, int rangeFinish = 0)
        {
            if (rangeStart != 0 || rangeFinish != 0)
            {
                byte[] newfile = new byte[rangeFinish - rangeStart];
                Array.Copy(file, rangeStart, newfile, 0, newfile.Length);
                file = newfile;
            }
            System.Security.Cryptography.MD5 md5 = System.Security.Cryptography.MD5.Create();
            file = md5.ComputeHash(file);
            string result = string.Empty;
            foreach (byte item in file)
                result += item.ToString("x2");
            return result;
        }

        private static void DeleteBackupFolder(string backupFolder)
        {
            if (Directory.Exists(backupFolder))
                Directory.Delete(backupFolder, true);
        }

        public static ConversionResult PatchGame(string sonicexe, out string message)
        {
            // Patch backup folder
            string backupFolder = Path.Combine(Path.GetDirectoryName(sonicexe), "PatchBackup");

            // This needs to be edited to point to where patch .dat files are now
            string patchFolder = AppContext.BaseDirectory;

            // Patches list
            List<PatchData> Patches = new List<PatchData>
            {
                // Supported games
                new PatchData("Steam / Dreamcast Collection Remastered", "patch_steam.dat", SteamSourceData),
                new PatchData("2004 (Korean)", "patch_kr.dat", KoreanSourceData),
                new PatchData("2004 (EU / DRM) to 2004 US", "patch_eu_orig.dat", EUOrigSourceData),
                new PatchData("2004 (Best Buy / DRM) to 2004 US", "patch_bestbuy.dat", BestBuySourceData),
                new PatchData("2004 (SEGA PC Collection)", "patch_eu_pccol.dat", PCColSourceData),                
                // Unsupported games
                new PatchData("Dreamcast Collection 1.0", "", DCColSourceData),
                new PatchData("2004 (Japanese)", "", JPSourceData),
            };

            // Check if the EXE is already compatible
            string hash_check = FileHash(sonicexe, 0x004DB2A0, 0x034F6FF0);
            if (hash_check == "491c8a38328377bccff34810afb5f760")
            {
                message = "This version of the game is already compatible with the Mod Loader.";
                return ConversionResult.AlreadyPatched;
            }

            // Remove the old backup folder
            DeleteBackupFolder(backupFolder);

            // Find the relevant patch
            string hash = FileHash(sonicexe);
            foreach (PatchData patch in Patches)
            {
                if (patch.FileHashes[0].filename.ToLowerInvariant() == Path.GetFileName(sonicexe.ToLowerInvariant()) && patch.FileHashes[0].checksum == hash)
                {
                    // Check unsupported
                    if (patch.PatchFilename == "")
                    {
                        message = "Sorry, " + patch.GameName + " is not supported.";
                        return ConversionResult.Unsupported;
                    }

                    // Create backup folder
                    Util.CreateSafeDirectory(backupFolder);

                    // Check files included in the patch and copy them to the temp folder
                    foreach (PatchHashData patchHash in patch.FileHashes)
                    {
                        string checkpath = Path.Combine(Path.GetDirectoryName(sonicexe), patchHash.filename);
                        if (!File.Exists(checkpath))
                        {
                            message = "File '" + checkpath + "' was not found.\n\nVerify game integrity and try again.";
                            DeleteBackupFolder(backupFolder);
                            return ConversionResult.Error;
                        }
                        string filehash = FileHash(checkpath);
                        if (filehash != patchHash.checksum)
                        {
                            message = "File '" + patchHash.filename + "' has a different checksum.\n\nVerify game integrity on Steam and try again.\n\n" + "File checksum: " + filehash + "\nExpected: " + patchHash.checksum;
                            DeleteBackupFolder(backupFolder);
                            return ConversionResult.Error;
                        }

                        Util.CreateSafeDirectory(Path.GetDirectoryName(Path.Combine(backupFolder, patchHash.filename)));
                        File.Copy(checkpath, Path.Combine(backupFolder, patchHash.filename), true);
                    }

                    // Start patching
                    /*HDiffPatch patcher = new();
                    patcher.Initialize(Path.Combine(patchFolder, patch.PatchFilename));
                    patcher.Patch(backupFolder, Path.GetDirectoryName(sonicexe), true, default, true, false);
                    message = patch.GameName + " was patched successfully.";
                    return ConversionResult.Success;*/
                }
            }
            message = "Unknown game version.";
            return ConversionResult.Unsupported;
        }
    }
}
