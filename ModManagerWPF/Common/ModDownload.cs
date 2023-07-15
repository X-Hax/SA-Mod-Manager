using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using ModManagerCommon;
using System.Net;
using System.Diagnostics;
using SevenZipExtractor;

namespace ModManagerWPF.Common
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

	public class DownloadProgressEventArgs : CancelEventArgs
	{
		private readonly DownloadProgressChangedEventArgs args;

		public int ProgressPercentage => args.ProgressPercentage;
		public object UserState => args.UserState;
		public long BytesReceived => args.BytesReceived;
		public long TotalBytesToReceive => args.TotalBytesToReceive;

		public int FileDownloading { get; }
		public int FilesToDownload { get; }

		public DownloadProgressEventArgs(DownloadProgressChangedEventArgs args,
			int fileDownloading, int filesToDownload)
		{
			this.args = args;
			FileDownloading = fileDownloading;
			FilesToDownload = filesToDownload;
		}
	}

	public class ModDownloadWPF
	{
		public ModInfo Info { get; }
		public bool IsChecked { get; set; }
		public readonly ModDownloadType Type;
		public readonly string Url;
		public readonly string Folder;
		public readonly string Changes;
		public long Size { get; }
		public int FilesToDownload { get; }
		public List<Updater.ModManifestDiff> ChangedFiles { get; }

		public string HomePage = string.Empty;
		public string Name = string.Empty;
		public string Version = string.Empty;
		public DateTime Published;
		public DateTime Updated = DateTime.Now;
		public string ReleaseUrl = string.Empty;

		public event CancelEventHandler DownloadStarted;
		public event EventHandler<DownloadProgressEventArgs> DownloadProgress;
		public event CancelEventHandler DownloadCompleted;
		public event CancelEventHandler Extracting;
		public event CancelEventHandler ParsingManifest;
		public event CancelEventHandler ApplyingManifest;

		/// <summary>
		/// Constructs a ModDownload instance with the type <value>ModDownloadType.Archive</value>.
		/// </summary>
		/// <param name="info">Metadata for the associated mod.</param>
		/// <param name="folder">The folder containing the mod.</param>
		/// <param name="url">URL of the mod download.</param>
		/// <param name="changes">List of changes for this update.</param>
		/// <param name="size">Size of the archive to download.</param>
		/// <seealso cref="ModDownloadType"/>
		public ModDownloadWPF(ModInfo info, string folder, string url, string changes, long size)
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
		public ModDownloadWPF(ModInfo info, string folder, string url, string changes, List<Updater.ModManifestDiff> diff)
		{
			Info = info;
			Type = ModDownloadType.Modular;
			Url = url;
			Folder = folder;

			ChangedFiles = diff?.Where(x => x.State != Updater.ModManifestState.Unchanged).ToList()
				?? throw new ArgumentNullException(nameof(diff));

			List<Updater.ModManifestDiff> toDownload = ChangedFiles
				.Where(x => x.State == Updater.ModManifestState.Added || x.State == Updater.ModManifestState.Changed)
				.ToList();

			FilesToDownload = toDownload.Count;
			Size = Math.Max(toDownload.Select(x => x.Current.FileSize).Sum(), toDownload.Count);

			Changes = !string.IsNullOrEmpty(changes) ? changes : string.Empty;
		}

		/// <summary>
		/// Downloads files required for updating according to <see cref="Type"/>.
		/// </summary>
		/// <param name="client"><see cref="WebClient"/> to be used for downloading.</param>
		/// <param name="updatePath">Path to store downloaded files.</param>
		public void Download(WebClient client, string updatePath)
		{
			var cancelArgs = new CancelEventArgs(false);
			DownloadProgressEventArgs downloadArgs = null;

			int fileDownloading = 0;

			void downloadComplete(object sender, AsyncCompletedEventArgs args)
			{
				lock (args.UserState)
				{
					Monitor.Pulse(args.UserState);
				}
			}

			void downloadProgressChanged(object sender, DownloadProgressChangedEventArgs args)
			{
				downloadArgs = new DownloadProgressEventArgs(args, fileDownloading, FilesToDownload);
				if (OnDownloadProgress(downloadArgs))
				{
					((WebClient)sender).CancelAsync();
				}
			}

			switch (Type)
			{
				case ModDownloadType.Archive:
					{
						var uri = new Uri(Url);
						if (!uri.Host.EndsWith("github.com", StringComparison.OrdinalIgnoreCase))
						{
							var request = (HttpWebRequest)WebRequest.Create(uri);
							request.Method = "HEAD";
							var response = (HttpWebResponse)request.GetResponse();
							uri = response.ResponseUri;
							response.Close();
						}

						string filePath = Path.Combine(updatePath, uri.Segments.Last());

						var info = new FileInfo(filePath);
						if (info.Exists && info.Length == Size)
						{
							if (OnDownloadCompleted(cancelArgs))
							{
								return;
							}
						}
						else
						{
							if (OnDownloadStarted(cancelArgs))
							{
								return;
							}

							client.DownloadFileCompleted += downloadComplete;
							client.DownloadProgressChanged += downloadProgressChanged;
							++fileDownloading;

							var sync = new object();
							lock (sync)
							{
								client.DownloadFileAsync(uri, filePath, sync);
								Monitor.Wait(sync);
							}

							client.DownloadProgressChanged -= downloadProgressChanged;
							client.DownloadFileCompleted -= downloadComplete;

							if (cancelArgs.Cancel || downloadArgs?.Cancel == true)
							{
								return;
							}

							if (OnDownloadCompleted(cancelArgs))
							{
								return;
							}
						}

						string dataDir = Path.GetFileNameWithoutExtension(filePath);
						if (dataDir.Length > 20)
							dataDir = dataDir.Remove(20).TrimEnd(' ');
						dataDir = Path.Combine(updatePath, dataDir);
						if (!Directory.Exists(dataDir))
						{
							Directory.CreateDirectory(dataDir);
						}

						if (OnExtracting(cancelArgs))
						{
							return;
						}

						try
						{
							using (ArchiveFile archiveFile = new(filePath))
							{
								archiveFile.Extract(dataDir);
							}
						}
						catch
						{
							throw new Exception("Failed to extract one mod.");
						}

						string workDir = Path.GetDirectoryName(ModInfo.GetModFiles(new DirectoryInfo(dataDir)).FirstOrDefault());

						if (string.IsNullOrEmpty(workDir))
						{
							throw new DirectoryNotFoundException($"Unable to locate mod.ini in \"{dataDir}\"");
						}

						string newManPath = Path.Combine(workDir, "mod.manifest");
						string oldManPath = Path.Combine(Folder, "mod.manifest");

						if (OnParsingManifest(cancelArgs))
						{
							return;
						}

						if (!File.Exists(newManPath) || !File.Exists(oldManPath))
						{
							CopyDirectory(new DirectoryInfo(workDir), Directory.CreateDirectory(Folder));
							Directory.Delete(dataDir, true);

							if (File.Exists(filePath))
							{
								File.Delete(filePath);
							}

							File.WriteAllText(Path.Combine(Folder, "mod.version"), Updated.ToString(DateTimeFormatInfo.InvariantInfo));
							return;
						}

						if (OnParsingManifest(cancelArgs))
						{
							return;
						}

						List<Updater.ModManifestEntry> newManifest = Updater.ModManifest.FromFile(newManPath);

						if (OnApplyingManifest(cancelArgs))
						{
							return;
						}

						List<Updater.ModManifestEntry> oldManifest = Updater.ModManifest.FromFile(oldManPath);
						List<string> oldFiles = oldManifest.Except(newManifest)
							.Select(x => Path.Combine(Folder, x.FilePath))
							.ToList();

						foreach (string file in oldFiles)
						{
							if (File.Exists(file))
							{
								File.Delete(file);
							}
						}

						RemoveEmptyDirectories(oldManifest, newManifest);

						foreach (Updater.ModManifestEntry file in newManifest)
						{
							string dir = Path.GetDirectoryName(file.FilePath);
							if (!string.IsNullOrEmpty(dir))
							{
								string newDir = Path.Combine(Folder, dir);
								if (!Directory.Exists(newDir))
								{
									Directory.CreateDirectory(newDir);
								}
							}

							var sourceFile = new FileInfo(Path.Combine(workDir, file.FilePath));
							var destFile = new FileInfo(Path.Combine(Folder, file.FilePath));

							if (destFile.Exists)
							{
								destFile.Delete();
							}

							sourceFile.Attributes &= ~FileAttributes.ReadOnly;
							sourceFile.MoveTo(destFile.FullName);
						}

						File.Copy(newManPath, oldManPath, true);

						void removeReadOnly(DirectoryInfo dir)
						{
							foreach (DirectoryInfo d in dir.GetDirectories())
							{
								removeReadOnly(d);
								d.Attributes &= ~FileAttributes.ReadOnly;
							}
						}

						removeReadOnly(new DirectoryInfo(dataDir));

						Directory.Delete(dataDir, true);
						File.WriteAllText(Path.Combine(Folder, "mod.version"), Updated.ToString(DateTimeFormatInfo.InvariantInfo));

						if (File.Exists(filePath))
						{
							File.Delete(filePath);
						}

						break;
					}

				case ModDownloadType.Modular:
					{
						List<Updater.ModManifestDiff> newEntries = ChangedFiles
							.Where(x => x.State == Updater.ModManifestState.Added || x.State == Updater.ModManifestState.Changed)
							.ToList();

						var uri = new Uri(Url);
						string tempDir = Path.Combine(updatePath, uri.Segments.Last());

						if (!Directory.Exists(tempDir))
						{
							Directory.CreateDirectory(tempDir);
						}

						var sync = new object();

						foreach (Updater.ModManifestDiff i in newEntries)
						{
							string filePath = Path.Combine(tempDir, i.Current.FilePath);
							string dir = Path.GetDirectoryName(filePath);

							if (!string.IsNullOrEmpty(dir) && !Directory.Exists(dir))
							{
								Directory.CreateDirectory(dir);
							}

							if (OnDownloadStarted(cancelArgs))
							{
								return;
							}

							var info = new FileInfo(filePath);
							++fileDownloading;

							if (!info.Exists || info.Length != i.Current.FileSize ||
								!i.Current.Checksum.Equals(Updater.ModManifestGenerator.GetFileHash(filePath), StringComparison.OrdinalIgnoreCase))
							{
								client.DownloadFileCompleted += downloadComplete;
								client.DownloadProgressChanged += downloadProgressChanged;

								lock (sync)
								{
									client.DownloadFileAsync(new Uri(uri, i.Current.FilePath), filePath, sync);
									Monitor.Wait(sync);
								}

								client.DownloadProgressChanged -= downloadProgressChanged;
								client.DownloadFileCompleted -= downloadComplete;

								info.Refresh();

								if (info.Length != i.Current.FileSize)
								{
									throw new Exception(string.Format("Size of downloaded file \"{0}\" ({1}) differs from manifest ({2}).",
										i.Current.FilePath, SizeSuffix.GetSizeSuffix(info.Length), SizeSuffix.GetSizeSuffix(i.Current.FileSize)));
								}

								string hash = Updater.ModManifestGenerator.GetFileHash(filePath);
								if (!i.Current.Checksum.Equals(hash, StringComparison.OrdinalIgnoreCase))
								{
									throw new Exception(string.Format("Checksum of downloaded file \"{0}\" ({1}) differs from manifest ({2}).",
										i.Current.FilePath, hash, i.Current.Checksum));
								}
							}

							if (cancelArgs.Cancel || downloadArgs?.Cancel == true)
							{
								return;
							}

							if (OnDownloadCompleted(cancelArgs))
							{
								return;
							}
						}

						client.DownloadFileCompleted += downloadComplete;
						lock (sync)
						{
							client.DownloadFileAsync(new Uri(uri, "mod.manifest"), Path.Combine(tempDir, "mod.manifest"), sync);
							Monitor.Wait(sync);
						}

						client.DownloadFileCompleted -= downloadComplete;

						// Handle all non-removal file operations (move, rename)
						List<Updater.ModManifestDiff> movedEntries = ChangedFiles.Except(newEntries)
							.Where(x => x.State == Updater.ModManifestState.Moved)
							.ToList();

						if (OnApplyingManifest(cancelArgs))
						{
							return;
						}

						// Handle existing entries marked as moved.
						foreach (Updater.ModManifestDiff i in movedEntries)
						{
							Updater.ModManifestEntry old = i.Last;

							// This would be considered an error...
							if (old == null)
							{
								continue;
							}

							string oldPath = Path.Combine(Folder, old.FilePath);
							string newPath = Path.Combine(tempDir, i.Current.FilePath);

							string dir = Path.GetDirectoryName(newPath);

							if (!string.IsNullOrEmpty(dir) && !Directory.Exists(dir))
							{
								Directory.CreateDirectory(dir);
							}

							File.Copy(oldPath, newPath, true);
						}

						// Now move the stuff from the temporary folder over to the working directory.
						foreach (Updater.ModManifestDiff i in newEntries.Concat(movedEntries))
						{
							string tempPath = Path.Combine(tempDir, i.Current.FilePath);
							string workPath = Path.Combine(Folder, i.Current.FilePath);
							string dir = Path.GetDirectoryName(workPath);

							if (!string.IsNullOrEmpty(dir) && !Directory.Exists(dir))
							{
								Directory.CreateDirectory(dir);
							}

							File.Copy(tempPath, workPath, true);
						}

						// Once that has succeeded we can safely delete files that have been marked for removal.
						List<Updater.ModManifestDiff> removedEntries = ChangedFiles
							.Where(x => x.State == Updater.ModManifestState.Removed)
							.ToList();

						foreach (string path in removedEntries.Select(i => Path.Combine(Folder, i.Current.FilePath)).Where(File.Exists))
						{
							File.Delete(path);
						}

						// Same for files that have been moved.
						foreach (string path in movedEntries
							.Where(x => newEntries.All(y => y.Current.FilePath != x.Last.FilePath))
							.Select(i => Path.Combine(Folder, i.Last.FilePath)).Where(File.Exists))
						{
							File.Delete(path);
						}

						string oldManPath = Path.Combine(Folder, "mod.manifest");
						string newManPath = Path.Combine(tempDir, "mod.manifest");

						if (File.Exists(oldManPath))
						{
							List<Updater.ModManifestEntry> oldManifest = Updater.ModManifest.FromFile(oldManPath);
							List<Updater.ModManifestEntry> newManifest = Updater.ModManifest.FromFile(newManPath);

							// Remove directories that are now empty.
							RemoveEmptyDirectories(oldManifest, newManifest);
						}

						// And last but not least, copy over the new manifest.
						File.Copy(newManPath, oldManPath, true);
						break;
					}

				default:
					throw new ArgumentOutOfRangeException();
			}
		}

		private void CopyDirectory(DirectoryInfo oldDir, DirectoryInfo newDir)
		{
			foreach (DirectoryInfo dir in oldDir.EnumerateDirectories())
			{
				CopyDirectory(dir, newDir.CreateSubdirectory(dir.Name));
			}

			foreach (FileInfo file in oldDir.EnumerateFiles())
			{
				file.CopyTo(Path.Combine(newDir.FullName, file.Name), true);
			}
		}

		private void RemoveEmptyDirectories(IEnumerable<Updater.ModManifestEntry> oldManifest, IEnumerable<Updater.ModManifestEntry> newManifest)
		{
			foreach (string dir in Updater.ModManifest.GetOldDirectories(oldManifest, newManifest)
											  .Select(x => Path.Combine(Folder, x)))
			{
				if (Directory.Exists(dir))
				{
					// Note that this is very intentionally not recursive. If there are
					// any files left over somehow, this SHOULD be considered an error,
					// as the goal is to exclusively remove empty directories.
					// - SF94
					Directory.Delete(dir);
				}
			}
		}

		private bool OnDownloadStarted(CancelEventArgs e)
		{
			DownloadStarted?.Invoke(this, e);
			return e.Cancel;
		}

		private bool OnDownloadProgress(DownloadProgressEventArgs e)
		{
			DownloadProgress?.Invoke(this, e);
			return e.Cancel;
		}

		private bool OnDownloadCompleted(CancelEventArgs e)
		{
			DownloadCompleted?.Invoke(this, e);
			return e.Cancel;
		}

		private bool OnExtracting(CancelEventArgs e)
		{
			Extracting?.Invoke(this, e);
			return e.Cancel;
		}

		private bool OnParsingManifest(CancelEventArgs e)
		{
			ParsingManifest?.Invoke(this, e);
			return e.Cancel;
		}

		private bool OnApplyingManifest(CancelEventArgs e)
		{
			ApplyingManifest?.Invoke(this, e);
			return e.Cancel;
		}
	}
}

