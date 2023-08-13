using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAModManager.Common
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
        };

        private static async Task HandleProfiles(bool installed, string gamePath)
        {
            string modFolder = Path.Combine(gamePath, "mods");
            string backupFolder = Path.Combine(gamePath, "BackupOldManagerFiles", "profiles");

            //if user is about to uninstall the loader restore vanilla saved profiles
            if (installed && Directory.Exists(backupFolder))
            {
                Directory.CreateDirectory(App.CurrentGame.ProfilesDirectory);

                foreach (var item in Directory.EnumerateFiles(backupFolder, "*.ini"))
                {
                    if (!item.EndsWith("SADXModLoader.ini", StringComparison.OrdinalIgnoreCase))
                    { 
                        await Util.MoveFileAsync(item, Path.Combine(modFolder, Path.GetFileName(item)), true);
                    }
                }
            }

            //if user is about to install the loader backup vanilla profiles
            if (!installed)
            {
                Directory.CreateDirectory(backupFolder);

                foreach (var item in Directory.EnumerateFiles(App.CurrentGame.modDirectory, "*.ini"))
                {
                    if (!item.EndsWith("SADXModLoader.ini", StringComparison.OrdinalIgnoreCase))
                    {
						// Deserializes old profiles and converts to new profile format, then saves to Profiles folder.
						await Util.ConvertProfiles(item, Path.Combine(App.CurrentGame.ProfilesDirectory, Path.GetFileName(item)));
                        //move old profiles in a backup folder
                        await Util.MoveFileAsync(item, Path.Combine(backupFolder, Path.GetFileName(item)), true);
                    }
                }

            }
        }

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
           await HandleProfiles(installed, gamePath);
        }
    }
}
