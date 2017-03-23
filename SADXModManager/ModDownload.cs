using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Net;
using System.Threading;
using SharpCompress.Archives.SevenZip;
using SharpCompress.Readers;

namespace SADXModManager
{
	public enum ModDownloadType
	{
		Archive,
		Modular
	}

	public class DownloadProgressEventArgs : CancelEventArgs
	{
		private readonly DownloadProgressChangedEventArgs args;

		public int    ProgressPercentage  => args.ProgressPercentage;
		public object UserState           => args.UserState;
		public long   BytesReceived       => args.BytesReceived;
		public long   TotalBytesToReceive => args.TotalBytesToReceive;

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

	public class ModDownload
	{
		public ModInfo Info { get; private set; }
		public readonly ModDownloadType Type;
		public readonly string Url;
		public readonly string Folder;
		public readonly string Changes;
		public long Size { get; private set; }
		public int FilesToDownload { get; private set; }
		public List<ModManifestDiff> ChangedFiles { get; private set; }

		public string HomePage   = string.Empty;
		public string Name       = string.Empty;
		public string Version    = string.Empty;
		public string Published  = string.Empty;
		public string Updated    = string.Empty;
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
		public ModDownload(ModInfo info, string folder, string url, string changes, long size)
		{
			Info            = info;
			Type            = ModDownloadType.Archive;
			Url             = url;
			Folder          = folder;
			Changes         = changes;
			Size            = size;
			FilesToDownload = 1;
		}

		/// <summary>
		/// Constructs a ModDownload instance with the type <value>ModDownloadType.Modular</value>.
		/// </summary>
		/// <param name="info">Metadata for the associated mod.</param>
		/// <param name="folder">The folder containing the mod.</param>
		/// <param name="url">URL of the mod download.</param>
		/// <param name="diff">A diff of the remote and local manifests.</param>
		/// <seealso cref="ModDownloadType"/>
		public ModDownload(ModInfo info, string folder, string url, List<ModManifestDiff> diff)
		{
			Info         = info;
			Type         = ModDownloadType.Modular;
			Url          = url;
			Folder       = folder;
			ChangedFiles = diff?.Where(x => x.State != ModManifestState.Unchanged).ToList()
				?? throw new ArgumentNullException(nameof(diff));

			List<ModManifestDiff> toDownload = ChangedFiles
				.Where(x => x.State == ModManifestState.Added || x.State == ModManifestState.Changed)
				.ToList();

			FilesToDownload = toDownload.Count;
			Size = Math.Max(toDownload.Select(x => x.Current.FileSize).Sum(), toDownload.Count);

			Changes = "Files changed in this update:\n\n"
				+ string.Join("\n", ChangedFiles.Select(x => "- " + x.State.ToString() + ":\t" + x.Current.FilePath));
		}

		private static void Extract(IReader reader, string outDir)
		{
			var options = new ExtractionOptions
			{
				ExtractFullPath = true,
				Overwrite = true,
				PreserveAttributes = true,
				PreserveFileTime = true
			};

			while (reader.MoveToNextEntry())
			{
				if (reader.Entry.IsDirectory)
				{
					continue;
				}

				reader.WriteEntryToDirectory(outDir, options);
			}
		}

		public void Download(WebClient client, string updatePath)
		{
			var cancelArgs = new CancelEventArgs(false);
			DownloadProgressEventArgs downloadArgs = null;

			int fileDownloading = 0;

			void DownloadComplete(object sender, AsyncCompletedEventArgs args)
			{
				lock (args.UserState)
				{
					Monitor.Pulse(args.UserState);
				}
			}

			void DownloadProgressChanged(object sender, DownloadProgressChangedEventArgs args)
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

							client.DownloadFileCompleted += DownloadComplete;
							client.DownloadProgressChanged += DownloadProgressChanged;
							++fileDownloading;

							var sync = new object();
							lock (sync)
							{
								client.DownloadFileAsync(uri, filePath, sync);
								Monitor.Wait(sync);
							}

							client.DownloadProgressChanged -= DownloadProgressChanged;
							client.DownloadFileCompleted -= DownloadComplete;

							if (cancelArgs.Cancel || downloadArgs?.Cancel == true)
							{
								return;
							}

							if (OnDownloadCompleted(cancelArgs))
							{
								return;
							}
						}

						string dataDir = Path.Combine(updatePath, Path.GetFileNameWithoutExtension(filePath));
						if (!Directory.Exists(dataDir))
						{
							Directory.CreateDirectory(dataDir);
						}

						if (OnExtracting(cancelArgs))
						{
							return;
						}

						using (Stream fileStream = File.OpenRead(filePath))
						{
							if (SevenZipArchive.IsSevenZipFile(fileStream))
							{
								using (SevenZipArchive archive = SevenZipArchive.Open(fileStream))
								{
									Extract(archive.ExtractAllEntries(), dataDir);
								}
							}
							else
							{
								using (IReader reader = ReaderFactory.Open(fileStream))
								{
									Extract(reader, dataDir);
								}
							}
						}

						string workDir = Path.GetDirectoryName(ModInfo.GetModFiles(new DirectoryInfo(dataDir)).FirstOrDefault());

						if (string.IsNullOrEmpty(workDir))
						{
							throw new DirectoryNotFoundException("Unable to locate mod.ini in " + dataDir);
						}

						string newManPath = Path.Combine(workDir, "mod.manifest");
						string oldManPath = Path.Combine(Folder, "mod.manifest");

						if (OnParsingManifest(cancelArgs))
						{
							return;
						}

						if (!File.Exists(newManPath))
						{
							throw new FileNotFoundException("This mod is missing a manifest!");
						}

						if (OnParsingManifest(cancelArgs))
						{
							return;
						}

						List<ModManifest> newManifest = ModManifest.FromFile(newManPath);

						if (OnApplyingManifest(cancelArgs))
						{
							return;
						}

						if (File.Exists(oldManPath))
						{
							List<ModManifest> oldManifest = ModManifest.FromFile(oldManPath);
							IEnumerable<ModManifest> unique = oldManifest.Except(newManifest);

							foreach (string removed in unique.Select(x => Path.Combine(Folder, x.FilePath)))
							{
								if (File.Exists(removed))
								{
									File.Delete(removed);
								}
								else if (Directory.Exists(removed))
								{
									Directory.Delete(removed, true);
								}
							}
						}

						foreach (ModManifest file in newManifest)
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

							string dest = Path.Combine(Folder, file.FilePath);

							if (File.Exists(dest))
							{
								File.Delete(dest);
							}

							File.Move(Path.Combine(workDir, file.FilePath), dest);
						}

						File.Copy(newManPath, oldManPath, true);
						Directory.Delete(dataDir, true);
						File.WriteAllText(Path.Combine(Folder, "mod.version"), Updated);

						if (File.Exists(filePath))
						{
							File.Delete(filePath);
						}
						break;
					}

				case ModDownloadType.Modular:
					{
						// First let's download all the new stuff.
						List<ModManifestDiff> newStuff = ChangedFiles
							.Where(x => x.State == ModManifestState.Added || x.State == ModManifestState.Changed)
							.ToList();

						var uri = new Uri(Url);
						string tempDir = Path.Combine(updatePath, uri.Segments.Last());

						if (!Directory.Exists(tempDir))
						{
							Directory.CreateDirectory(tempDir);
						}

						var sync = new object();

						foreach (ModManifestDiff i in newStuff)
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

							client.DownloadFileCompleted += DownloadComplete;
							client.DownloadProgressChanged += DownloadProgressChanged;
							++fileDownloading;

							lock (sync)
							{
								client.DownloadFileAsync(new Uri(uri, i.Current.FilePath), filePath, sync);
								Monitor.Wait(sync);
							}

							client.DownloadProgressChanged -= DownloadProgressChanged;
							client.DownloadFileCompleted -= DownloadComplete;

							if (cancelArgs.Cancel || downloadArgs?.Cancel == true)
							{
								return;
							}

							if (OnDownloadCompleted(cancelArgs))
							{
								return;
							}
						}

						client.DownloadFileCompleted += DownloadComplete;
						lock (sync)
						{
							client.DownloadFileAsync(new Uri(uri, "mod.manifest"), Path.Combine(tempDir, "mod.manifest"), sync);
							Monitor.Wait(sync);
						}
						client.DownloadFileCompleted -= DownloadComplete;

						// Now handle all file operations except where removals are concerned.
						List<ModManifestDiff> movedStuff = ChangedFiles.Except(newStuff)
							.Where(x => x.State == ModManifestState.Moved)
							.ToList();

						if (OnApplyingManifest(cancelArgs))
						{
							return;
						}

						foreach (ModManifestDiff i in movedStuff)
						{
							ModManifest old = i.Last;
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
						foreach (ModManifestDiff i in newStuff.Concat(movedStuff))
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
						List<ModManifestDiff> removedFiles = ChangedFiles
							.Where(x => x.State == ModManifestState.Removed)
							.ToList();

						foreach (string path in removedFiles.Select(i => Path.Combine(Folder, i.Current.FilePath)).Where(File.Exists))
						{
							File.Delete(path);
						}

						// Same for files that have been moved.
						foreach (string path in movedStuff.Select(i => Path.Combine(Folder, i.Last.FilePath)).Where(File.Exists))
						{
							File.Delete(path);
						}

						// And last but not least, copy over the new manifest.
						string oldManPath = Path.Combine(Folder, "mod.manifest");
						File.Copy(Path.Combine(tempDir, "mod.manifest"), oldManPath, true);
						break;
					}

				default:
					throw new ArgumentOutOfRangeException();
			}
		}

		protected virtual bool OnDownloadStarted(CancelEventArgs e)
		{
			DownloadStarted?.Invoke(this, e);
			return e.Cancel;
		}

		protected virtual bool OnDownloadProgress(DownloadProgressEventArgs e)
		{
			DownloadProgress?.Invoke(this, e);
			return e.Cancel;
		}

		protected virtual bool OnDownloadCompleted(CancelEventArgs e)
		{
			DownloadCompleted?.Invoke(this, e);
			return e.Cancel;
		}

		protected virtual bool OnExtracting(CancelEventArgs e)
		{
			Extracting?.Invoke(this, e);
			return e.Cancel;
		}

		protected virtual bool OnParsingManifest(CancelEventArgs e)
		{
			ParsingManifest?.Invoke(this, e);
			return e.Cancel;
		}

		protected virtual bool OnApplyingManifest(CancelEventArgs e)
		{
			ApplyingManifest?.Invoke(this, e);
			return e.Cancel;
		}
	}
}
