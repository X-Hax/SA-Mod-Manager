using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SAModManager.Configuration;
using SAModManager.UI;

namespace SAModManager.Profile
{
    public static class VanillaTransition
    {
        static private readonly List<string> VanillaManagerFiles = new()
        {
            "7z.dll",
            "7z.exe",
            "bass.dll",
            "libatrac9.dll",
            "libcelt-0061.dll",
            "libcelt-0110.dll",
            "libg719_decode.dll",
            "libg7221_decode.dll",
            "libmpg123-0.dll",
            "libogg.dll",
            "libspeex.dll",
            "libvorbis.dll",
            "libvorbisfile.dll",
            "avutil-vgmstream-56.dll",
            "avcodec-vgmstream-58.dll",
            "avformat-vgmstream-58.dll",
            "jansson.dll",
            "swresample-vgmstream-3.dll",
            "bass_vgmstream.dll",
            "COPYING_BASS_VGMSTREAM",
            "COPYING_D3D8TO9",
            "COPYING_VGMSTREAM",
            "d3d8m.dll",
            "loader.manifest",
            "ModManagerCommon.dll",
            "ModManagerCommon.pdb",
            "Newtonsoft.Json.dll",
            "sadxmlver.txt",
            "SADXModManager.exe",
            "SADXModManager.exe.config",
            "SharpDX.DirectInput.dll",
            "SharpDX.dll",
            "/mods/SADXModLoader.dll"
        };


        private static async Task BackupAndRestoreManagerFiles(bool installed, string gamePath)
        {
            string backupFolder = Path.Combine(gamePath, "BackupOldManagerFiles");

            if (installed && Directory.Exists(backupFolder))
            {
                foreach (var file in VanillaManagerFiles)
                {
                    string backupFile = Path.Combine(backupFolder, file);

                    if (File.Exists(backupFile))
                    {
                        await Util.MoveFile(backupFile, Path.Combine(gamePath, file), true);
                    }
                }
            }

            if (!installed)
            {
                Directory.CreateDirectory(backupFolder);

                foreach (var file in VanillaManagerFiles)
                {
                    string backupFile = Path.Combine(gamePath, file);

                    if (File.Exists(backupFile))
                    {
                        await Util.MoveFile(backupFile, Path.Combine(backupFolder, file), true);
                    }
                }

            }
        }

        public async static Task HandleVanillaManagerFiles(bool installed, string gamePath)
        {
            await BackupAndRestoreManagerFiles(installed, gamePath);

        }

        public async static Task<List<ProfileEntry>> ConvertOldProfile(string gamePath)
        {
            if (!Directory.Exists(gamePath))
            {
                return null;
            }

            Profiles profiles = new();
            bool converted = false;

            try
            {
                string modFolder = Path.Combine(gamePath, "mods");

                Directory.CreateDirectory(App.CurrentGame.ProfilesDirectory);
                string defaultJson = Path.Combine(App.CurrentGame.ProfilesDirectory, "Default.json");
                //if user is about to install the loader backup vanilla profiles

                foreach (var item in Directory.EnumerateFiles(modFolder, "*.ini"))
                {
                    string destinationPath = Path.Combine(App.CurrentGame.ProfilesDirectory, Path.GetFileName(item));
                    string newFileName = Path.GetFileNameWithoutExtension(destinationPath) + ".json";
                    //don't duplicate profile
                    if (!File.Exists(destinationPath) && !File.Exists(Path.Combine(App.CurrentGame.ProfilesDirectory, newFileName)))
                    {
                        await Util.CopyFileAsync(item, destinationPath, false);
                        Util.ConvertProfiles(destinationPath, ref profiles);
                        converted = true;
                    }
                }

                if (!converted) 
                {
                    return null;
                }

                return profiles.ProfilesList;

            }
            catch { }

            return null;
        }
    }
}
