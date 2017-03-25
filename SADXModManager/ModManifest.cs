using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Cryptography;

namespace SADXModManager
{
	public enum ModManifestState
	{
		Unchanged,
		Moved,
		Changed,
		Added,
		Removed
	}

	public class ModManifestDiff
	{
		public readonly ModManifestState State;
		public readonly ModManifest Current;
		public readonly ModManifest Last;

		public ModManifestDiff(ModManifestState state, ModManifest current, ModManifest last)
		{
			State   = state;
			Current = current;
			Last    = last;
		}
	}

	public class FilesIndexedEventArgs : EventArgs
	{
		public FilesIndexedEventArgs(int fileCount)
		{
			FileCount = fileCount;
		}

		public int FileCount { get; }
	}

	public class FileHashEventArgs : EventArgs
	{
		public FileHashEventArgs(string fileName, int fileIndex, int fileCount)
		{
			FileName  = fileName;
			FileIndex = fileIndex;
			FileCount = fileCount;
			Cancel    = false;
		}

		public string FileName  { get; }
		public int    FileIndex { get; }
		public int    FileCount { get; }
		public bool   Cancel    { get; set; }
	}

	public class ModManifestGenerator
	{
		public event EventHandler<FilesIndexedEventArgs> FilesIndexed;
		public event EventHandler<FileHashEventArgs> FileHashStart;
		public event EventHandler<FileHashEventArgs> FileHashEnd;

		public List<ModManifest> Generate(string modPath)
		{
			if (!Directory.Exists(modPath))
			{
				throw new DirectoryNotFoundException();
			}

			int i = 0;
			var result = new List<ModManifest>();
			List<string> fileIndex = Directory.EnumerateFiles(modPath, "*", SearchOption.AllDirectories)
				.Where(x => !string.IsNullOrEmpty(x)
				            && !Path.GetFileName(x).Equals("mod.manifest", StringComparison.InvariantCultureIgnoreCase)
				            && !Path.GetFileName(x).Equals("mod.version", StringComparison.InvariantCultureIgnoreCase))
				.ToList();

			if (fileIndex.Count < 1)
			{
				return result;
			}

			OnFilesIndexed(new FilesIndexedEventArgs(fileIndex.Count));

			foreach (var f in fileIndex)
			{
				var relativePath = f.Substring(modPath.Length + 1);
				var file = new FileInfo(f);

				++i;

				var args = new FileHashEventArgs(relativePath, i, fileIndex.Count);
				OnFileHashStart(args);

				if (args.Cancel)
				{
					return null;
				}

				string hash = GetFileHash(f);

				args = new FileHashEventArgs(relativePath, i, fileIndex.Count);
				OnFileHashEnd(args);

				if (args.Cancel)
				{
					return null;
				}

				result.Add(new ModManifest
				{
					FilePath = relativePath,
					FileSize = file.Length,
					Checksum = hash
				});
			}

			return result;
		}

		public static List<ModManifestDiff> Diff(List<ModManifest> newManifest, List<ModManifest> oldManifest)
		{
			// TODO: handle copies instead of moves to reduce download requirements

			var result = new List<ModManifestDiff>();

			List<ModManifest> old = oldManifest != null && oldManifest.Count > 0
				? new List<ModManifest>(oldManifest) : new List<ModManifest>();

			foreach (ModManifest entry in newManifest)
			{
				// First, check for an exact match. File path/name, hash, size; everything.
				ModManifest exact = old.FirstOrDefault(x => Equals(x, entry));
				if (exact != null)
				{
					old.Remove(exact);
					result.Add(new ModManifestDiff(ModManifestState.Unchanged, entry, null));
					continue;
				}

				// There's no exact match, so let's search by checksum.
				List<ModManifest> checksum = old.Where(x => x.Checksum.Equals(entry.Checksum, StringComparison.InvariantCultureIgnoreCase)).ToList();

				// If we've found matching checksums, we then need to check
				// the file path to see if it's been moved.
				if (checksum.Count > 0)
				{
					foreach (ModManifest c in checksum)
					{
						old.Remove(c);
					}

					if (checksum.All(x => x.FilePath != entry.FilePath))
					{
						old.Remove(old.FirstOrDefault(x => x.FilePath == entry.FilePath));
						result.Add(new ModManifestDiff(ModManifestState.Moved, entry, checksum[0]));
						continue;
					}
				}

				// If we've made it here, there's no matching checksums, so let's search
				// for matching paths. If a path matches, the file has been modified.
				ModManifest nameMatch = old.FirstOrDefault(x => x.FilePath == entry.FilePath);
				if (nameMatch != null)
				{
					old.Remove(nameMatch);
					result.Add(new ModManifestDiff(ModManifestState.Changed, entry, nameMatch));
					continue;
				}

				// In every other case, this file is newly added.
				result.Add(new ModManifestDiff(ModManifestState.Added, entry, null));
			}

			// All files that are still unique to the old manifest should be marked for removal.
			if (old.Count > 0)
			{
				result.AddRange(old.Select(x => new ModManifestDiff(ModManifestState.Removed, x, null)));
			}

			return result;
		}

		public List<ModManifestDiff> Verify(string modPath, List<ModManifest> manifest)
		{
			var result = new List<ModManifestDiff>();
			int i = 0;

			foreach (var m in manifest)
			{
				var filePath = Path.Combine(modPath, m.FilePath);

				++i;

				var args = new FileHashEventArgs(m.FilePath, i, manifest.Count);
				OnFileHashStart(args);

				if (args.Cancel)
				{
					return null;
				}

				try
				{
					if (!File.Exists(filePath))
					{
						result.Add(new ModManifestDiff(ModManifestState.Removed, m, null));
						continue;
					}

					var info = new FileInfo(filePath);

					if (info.Length != m.FileSize)
					{
						result.Add(new ModManifestDiff(ModManifestState.Changed, m, null));
						continue;
					}

					string hash = GetFileHash(filePath);
					if (!hash.Equals(m.Checksum, StringComparison.InvariantCultureIgnoreCase))
					{
						result.Add(new ModManifestDiff(ModManifestState.Changed, m, null));
						continue;
					}

					result.Add(new ModManifestDiff(ModManifestState.Unchanged, m, null));
				}
				finally
				{
					args = new FileHashEventArgs(m.FilePath, i, manifest.Count);
					OnFileHashEnd(args);
				}

				if (args.Cancel)
				{
					return null;
				}
			}

			return result;
		}

		public static string GetFileHash(string f)
		{
			byte[] hash;

			using (var sha = new SHA256Cng())
			{
				using (FileStream stream = File.OpenRead(f))
				{
					hash = sha.ComputeHash(stream);
				}
			}

			return string.Concat(hash.Select(x => x.ToString("x2")));
		}
		
		protected virtual void OnFilesIndexed(FilesIndexedEventArgs e)
		{
			FilesIndexed?.Invoke(this, e);
		}

		protected virtual void OnFileHashStart(FileHashEventArgs e)
		{
			FileHashStart?.Invoke(this, e);
		}

		protected virtual void OnFileHashEnd(FileHashEventArgs e)
		{
			FileHashEnd?.Invoke(this, e);
		}
	}

	public class ModManifest
	{
		public string FilePath;
		public long FileSize;
		public string Checksum;

		public ModManifest(string line)
		{
			string[] fields = line.Split('\t');
			if (fields.Length != 3)
			{
				throw new ArgumentException($"Manifest line must have 3 fields. Provided: { fields.Length }", nameof(line));
			}

			FilePath = fields[0];
			FileSize = long.Parse(fields[1]);
			Checksum = fields[2];
		}

		public ModManifest() {}

		public static List<ModManifest> FromFile(string filePath)
		{
			string[] lines = File.ReadAllLines(filePath);
			return lines.Select(line => new ModManifest(line)).ToList();
		}

		public static List<ModManifest> FromString(string str)
		{
			string[] lines = str.Split(new string[] { "\r\n", "\r", "\n" }, StringSplitOptions.RemoveEmptyEntries);
			return lines.Select(line => new ModManifest(line)).ToList();
		}

		public static void ToFile(IEnumerable<ModManifest> manifest, string path)
		{
			File.WriteAllLines(path, manifest.Select(x => x.ToString()));
		}

		public override string ToString()
		{
			return $"{ FilePath }\t{ FileSize }\t{ Checksum }";
		}

		public override bool Equals(object obj)
		{
			if (ReferenceEquals(this, obj))
			{
				return true;
			}

			var m = obj as ModManifest;
			if (m == null)
			{
				return false;
			}

			return FileSize == m.FileSize && FilePath == m.FilePath && Checksum == m.Checksum;
		}

		public override int GetHashCode()
		{
			return 1;
		}
	}
}
