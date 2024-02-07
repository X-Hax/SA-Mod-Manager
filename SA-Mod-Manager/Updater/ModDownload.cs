using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Threading;
using System.Net;
using SAModManager.Common;
using System.Threading.Tasks;
using System.Net.Http;

namespace SAModManager.Updater
{
    public enum ModDownloadType
    {
        /// <summary>
        /// The mod is contained in a single archive file.
        /// </summary>
        Archive,
        /// <summary>
        /// The mod is hosted in a directory tree with
        /// individually accessible files.
        /// </summary>
        Modular
    }


    public class ModDownload
    {
        public ModInfo Info { get; }
        public bool IsChecked { get; set; }
        public readonly ModDownloadType Type;
        public readonly string Url;
        public string Folder;
        public readonly string Changes;
        public long Size { get; }
        public int FilesToDownload { get; }
        public List<ModManifestDiff> ChangedFiles { get; }

        public string HomePage = string.Empty;
        public string Name = string.Empty;
        public string Version = string.Empty;
        public DateTime Published;
        public DateTime Updated = DateTime.Now;
        public string ReleaseUrl = string.Empty;

        /// <summary>
        /// Constructs a ModDownload instance with the type <value>ModDownloadType.Archive</value>.
        /// </summary>
        /// <param name="info">Metadata for the associated mod.</param>
        /// <param name="folder">The folder containing the mod.</param>
        /// <param name="url">URL of the mod download.</param>
        /// <param name="changes">List of changes for this update.</param>
        /// <param name="size">Size of the archive to download.</param>
        /// <seealso cref="ModDownloadType"/>
        public ModDownload(ModInfo info, string folder, string url, string changes, long size)
        {
            Info = info;
            Type = ModDownloadType.Archive;
            Url = url;
            Folder = folder;
            Changes = changes;
            Size = size;
            FilesToDownload = 1;
        }

        /// <summary>
        /// Constructs a ModDownload instance with the type <value>ModDownloadType.Modular</value>.
        /// </summary>
        /// <param name="info">Metadata for the associated mod.</param>
        /// <param name="folder">The folder containing the mod.</param>
        /// <param name="url">URL of the mod download.</param>
        /// <param name="changes">List of changes for this update.</param>
        /// <param name="diff">A diff of the remote and local manifests.</param>
        /// <seealso cref="ModDownloadType"/>
        public ModDownload(ModInfo info, string folder, string url, string changes, List<ModManifestDiff> diff)
        {
            Info = info;
            Type = ModDownloadType.Modular;
            Url = url;
            Folder = folder;

            ChangedFiles = diff?.Where(x => x.State != ModManifestState.Unchanged).ToList()
                ?? throw new ArgumentNullException(nameof(diff));

            List<ModManifestDiff> toDownload = ChangedFiles
                .Where(x => x.State == ModManifestState.Added || x.State == ModManifestState.Changed)
                .ToList();

            FilesToDownload = toDownload.Count;
            Size = Math.Max(toDownload.Select(x => x.Current.FileSize).Sum(), toDownload.Count);

            Changes = !string.IsNullOrEmpty(changes) ? changes : string.Empty;
        }



    }
}

