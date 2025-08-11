using System.ComponentModel;
using System.Security.Cryptography;
/*
namespace SAMM.Configuration.Mods
{
	/// <summary>
	/// Represents the difference between two <see cref="ModManifestEntry"/>s.
	/// </summary>
	public enum ModManifestState
	{
		/// <summary>
		/// The file is unchanged.
		/// </summary>
		Unchanged,

		/// <summary>
		/// Indicates that a file has been moved, renamed, or both.
		/// </summary>
		Moved,

		/// <summary>
		/// The file has been modified in some way.
		/// </summary>
		Changed,

		/// <summary>
		/// The file has been added to the new manifest.
		/// </summary>
		Added,

		/// <summary>
		/// The file has been removed from the new manifest.
		/// </summary>
		Removed
	}

	/// <summary>
	/// Holds two instances of <see cref="ModManifestEntry"/> and their differences.
	/// </summary>
	/// <seealso cref="ModManifestState"/>
	public class ModManifestDiff
	{
		/// <summary>
		/// The state of the file.
		/// </summary>
		/// <seealso cref="ModManifestState"/>
		public readonly ModManifestState State;

		/// <summary>
		/// The newer of the two entries.
		/// </summary>
		public readonly ModManifestEntry Current;

		/// <summary>
		/// The older of the two entries.
		/// </summary>
		public readonly ModManifestEntry Last;

		public ModManifestDiff(ModManifestState state, ModManifestEntry current, ModManifestEntry last)
		{
			State = state;
			Current = current;
			Last = last;
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
			FileName = fileName;
			FileIndex = fileIndex;
			FileCount = fileCount;
			Cancel = false;
		}

		public string FileName { get; }
		public int FileIndex { get; }
		public int FileCount { get; }
		public bool Cancel { get; set; }
	}

	public class ModManifestGenerator
	{
		public event EventHandler<FilesIndexedEventArgs> FilesIndexed;
		public event EventHandler<FileHashEventArgs> FileHashStart;
		public event EventHandler<FileHashEventArgs> FileHashEnd;

		/// <summary>
		/// Generates a manifest for a given mod.
		/// </summary>
		/// <param name="modPath">The path to the mod directory.</param>
		/// <returns>A list of <see cref="ModManifestEntry"/>.</returns>
		public List<ModManifestEntry> Generate(string modPath)
		{
			if (!Directory.Exists(modPath))
			{
				throw new DirectoryNotFoundException($"Directory does not exist: {modPath}");
			}

			var result = new List<ModManifestEntry>();

			bool FileIndexFilter(string filePath)
			{
				if (string.IsNullOrEmpty(filePath))
				{
					return false;
				}

				string fileName = Path.GetFileName(filePath);
				return !fileName.Equals("mod.manifest") && !fileName.Equals("mod.version");
			}

			List<string> fileIndex = Directory.EnumerateFiles(modPath, "*", SearchOption.AllDirectories)
											  .Where(FileIndexFilter)
											  .ToList();

			if (fileIndex.Count < 1)
			{
				return result;
			}

			OnFilesIndexed(new FilesIndexedEventArgs(fileIndex.Count));

			int index = 0;

			foreach (string filePath in fileIndex)
			{
				++index;
				string relativePath = filePath.Substring(modPath.Length + 1);
				FileInfo fileInfo = GetFileInfo(filePath);

				var args = new FileHashEventArgs(relativePath, index, fileIndex.Count);
				OnFileHashStart(args);

				if (args.Cancel)
				{
					return null;
				}

				string hash = GetFileHash(filePath);

				args = new FileHashEventArgs(relativePath, index, fileIndex.Count);
				OnFileHashEnd(args);

				if (args.Cancel)
				{
					return null;
				}

				result.Add(new ModManifestEntry(relativePath, fileInfo.Length, hash));
			}

			result.TrimExcess();
			return result;
		}

		/// <summary>
		/// Follows symbolic links and constructs a <see cref="FileInfo"/> of the actual file.
		/// </summary>
		/// <param name="filePath">Path to the file.</param>
		/// <returns>The <seealso cref="FileInfo"/> of the real file.</returns>
		private static FileInfo GetFileInfo(string filePath)
		{
			var fileInfo = new FileInfo(filePath);

			if ((fileInfo.Attributes & FileAttributes.ReparsePoint) == 0)
			{
				return fileInfo;
			}

			string reparsed;

			try
			{
				reparsed = NativeMethods.GetFinalPathName(filePath);
			}
			catch (Win32Exception ex)
			{
				if (ex.NativeErrorCode == 2)
				{
					throw new FileNotFoundException($"Failed to retrieve final path for symbolic link: {filePath}", filePath);
				}

				throw;
			}

			const string prefix = @"\\?\";

			if (reparsed.StartsWith(prefix))
			{
				reparsed = reparsed.Substring(prefix.Length);
			}

			fileInfo = new FileInfo(reparsed);

			return fileInfo;
		}

		/// <summary>
		/// Generates a diff of two mod manifests.
		/// </summary>
		/// <param name="newManifest">The new manifest.</param>
		/// <param name="oldManifest">The old manifest.</param>
		/// <returns>A list of <see cref="ModManifestDiff"/> containing change information.</returns>
		public static List<ModManifestDiff> Diff(List<ModManifestEntry> newManifest, List<ModManifestEntry> oldManifest)
		{
			// TODO: handle copies instead of moves to reduce download requirements (or cache downloads by hash?)

			var result = new List<ModManifestDiff>();

			List<ModManifestEntry> old = oldManifest != null && oldManifest.Count > 0
				? new List<ModManifestEntry>(oldManifest)
				: new List<ModManifestEntry>();

			foreach (ModManifestEntry entry in newManifest)
			{
				// First, check for an exact match. File path/name, hash, size; everything.
				ModManifestEntry exact = old.FirstOrDefault(x => Equals(x, entry));

				if (entry.FilePath == "config.ini")
				{
					continue;
				}

				if (exact != null)
				{
					old.Remove(exact);
					result.Add(new ModManifestDiff(ModManifestState.Unchanged, entry, null));
					continue;
				}

				// There's no exact match, so let's search by checksum.
				List<ModManifestEntry> checksum = old.Where(x => x.Checksum.Equals(entry.Checksum, StringComparison.InvariantCultureIgnoreCase)).ToList();

				// If we've found matching checksums, we then need to check
				// the file path to see if it's been moved.
				if (checksum.Count > 0)
				{
					old.Remove(checksum[0]);

					if (checksum.All(x => x.FilePath != entry.FilePath))
					{
						old.Remove(old.FirstOrDefault(x => x.FilePath.Equals(entry.FilePath, StringComparison.InvariantCultureIgnoreCase)));
						result.Add(new ModManifestDiff(ModManifestState.Moved, entry, checksum[0]));
						continue;
					}
				}

				// If we've made it here, there's no matching checksums, so let's search
				// for matching paths. If a path matches, the file has been modified.
				ModManifestEntry nameMatch = old.FirstOrDefault(x => x.FilePath.Equals(entry.FilePath, StringComparison.InvariantCultureIgnoreCase));

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

		/// <summary>
		/// Verifies the integrity of a mod against a mod manifest.
		/// </summary>
		/// <param name="modPath">Path to the mod to verify.</param>
		/// <param name="manifest">Manifest to check against.</param>
		/// <returns>A list of <see cref="ModManifestDiff"/> containing change information.</returns>
		public List<ModManifestDiff> Verify(string modPath, List<ModManifestEntry> manifest)
		{
			var result = new List<ModManifestDiff>(manifest.Count);
			int index = 0;

			foreach (ModManifestEntry m in manifest)
			{
				string filePath = Path.Combine(modPath, m.FilePath);

				++index;

				var args = new FileHashEventArgs(m.FilePath, index, manifest.Count);
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

					FileInfo info;

					try
					{
						info = GetFileInfo(filePath);
					}
					catch (FileNotFoundException)
					{
						result.Add(new ModManifestDiff(ModManifestState.Removed, m, null));
						continue;
					}

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
					args = new FileHashEventArgs(m.FilePath, index, manifest.Count);
					OnFileHashEnd(args);
				}

				if (args.Cancel)
				{
					return null;
				}
			}

			return result;
		}

		/// <summary>
		/// Computes a SHA-256 hash of a given file
		/// </summary>
		/// <param name="filePath">Path to the file to hash.</param>
		/// <returns>Lowercase string representation of the hash.</returns>
		public static string GetFileHash(string filePath)
		{
			byte[] hash;
			using (SHA256 sha = SHA256.Create())
			{
				using (FileStream stream = File.OpenRead(filePath))
				{
					hash = sha.ComputeHash(stream);
				}
			}

			return string.Concat(hash.Select(x => x.ToString("x2")));
		}

		private void OnFilesIndexed(FilesIndexedEventArgs e)
		{
			FilesIndexed?.Invoke(this, e);
		}

		private void OnFileHashStart(FileHashEventArgs e)
		{
			FileHashStart?.Invoke(this, e);
		}

		private void OnFileHashEnd(FileHashEventArgs e)
		{
			FileHashEnd?.Invoke(this, e);
		}
	}

	public static class ModManifest
	{
		/// <summary>
		/// Produces a mod manifest from a file.
		/// </summary>
		/// <param name="filePath">The path to the mod manifest file.</param>
		/// <returns>List of <see cref="ModManifestEntry"/></returns>
		public static List<ModManifestEntry> FromFile(string filePath)
		{
			string[] lines = File.ReadAllLines(filePath);
			return lines.Select(line => new ModManifestEntry(line)).ToList();
		}

		/// <summary>
		/// Parses a mod manifest file in string form and produces a mod manifest.
		/// </summary>
		/// <param name="str">The mod manifest file string to parse.</param>
		/// <returns>List of <see cref="ModManifestEntry"/></returns>
		public static List<ModManifestEntry> FromString(string str)
		{
			string[] lines = str.Split(new string[] { "\r\n", "\r", "\n" }, StringSplitOptions.RemoveEmptyEntries);
			return lines.Select(line => new ModManifestEntry(line)).ToList();
		}

		/// <summary>
		/// Writes a mod manifest to a file.
		/// </summary>
		/// <param name="manifest">The manifest to write.</param>
		/// <param name="filePath">The file to write the manifest to.</param>
		public static void ToFile(IEnumerable<ModManifestEntry> manifest, string filePath)
		{
			File.WriteAllLines(filePath, manifest.Select(x => x.ToString()));
		}

		/// <summary>
		/// Get all distinct directories from an old manifest which no longer exist in a new manifest.
		/// </summary>
		/// <param name="oldManifest">The old manifest.</param>
		/// <param name="newManifest">The new manifest.</param>
		/// <returns>
		/// All distinct directories exclusive to <paramref name="oldManifest"/> sorted by number of
		/// directory separators in descending order (deepest directory first), with all platform-agnostic
		/// directory separators replaced with <see cref="Path.DirectorySeparatorChar"/>.
		/// </returns>
		public static IEnumerable<string> GetOldDirectories(IEnumerable<ModManifestEntry> oldManifest, IEnumerable<ModManifestEntry> newManifest)
		{
			IEnumerable<string> GetDistinctPaths(IEnumerable<ModManifestEntry> manifest)
			{
				return manifest.Select(x => Path.GetDirectoryName(x.FilePath))
							   .Where(s => !string.IsNullOrEmpty(s))
							   .Select(s => s.Replace('/', Path.DirectorySeparatorChar))
							   .Distinct(StringComparer.OrdinalIgnoreCase);
			}

			HashSet<string> GetAllDirectoriesRecursively(IEnumerable<ModManifestEntry> manifest)
			{
				var directories = new HashSet<string>(StringComparer.OrdinalIgnoreCase);

				foreach (string newPath in GetDistinctPaths(manifest))
				{
					string path = newPath;

					do
					{
						// If the directory is already in the set (Add() returns false), then
						// so too are all of its parent directories, and we can stop iterating.
						if (!directories.Add(path))
						{
							break;
						}

						// Keep asking for and adding the parent directory until there isn't one.
						path = Path.GetDirectoryName(path);
					} while (!string.IsNullOrEmpty(path));
				}

				return directories;
			}

			// Collect all directories and their parent directories for both manifests.
			HashSet<string> newDirectories = GetAllDirectoriesRecursively(newManifest);
			HashSet<string> oldDirectories = GetAllDirectoriesRecursively(oldManifest);

			// Return all directories that exist only in the old manifest.
			return oldDirectories.Where(s => !newDirectories.Contains(s))
								 .OrderByDescending(s => s.Count(c => c == Path.DirectorySeparatorChar));
		}
	}

	/// <summary>
	/// An entry in a mod manifest describing a file's path, size, and checksum.
	/// </summary>
	public class ModManifestEntry
	{
		/// <summary>
		/// The name/path of the file relative to the root of the mod directory.
		/// </summary>
		public readonly string FilePath;

		/// <summary>
		/// The size of the file in bytes.
		/// </summary>
		public readonly long FileSize;

		/// <summary>
		/// String representation of the SHA-256 checksum of the file.
		/// </summary>
		public readonly string Checksum;

		/// <summary>
		/// Parses a line from a mod manifest line and constructs a <see cref="ModManifestEntry"/> .
		/// </summary>
		/// <param name="line">
		/// The line to parse.
		/// Each field of the line must be separated by tab (\t) and contain 3 fields total.
		/// Expected format is: [name]\t[size]\t[checksum]
		/// </param>
		/// <exception cref="ArgumentException">Thrown when an invalid number of fields parsed from the line.</exception>
		/// <exception cref="Exception">Thrown when an absolute path is provided, or parent directory traversal was attempted ("..\").</exception>
		public ModManifestEntry(string line)
		{
			string[] fields = line.Split('\t');

			if (fields.Length != 3)
			{
				throw new ArgumentException($"Manifest line must have 3 fields. Provided: {fields.Length}", nameof(line));
			}

			FilePath = fields[0];
			FileSize = long.Parse(fields[1]);
			Checksum = fields[2];

			if (Path.IsPathRooted(FilePath))
			{
				throw new Exception($"Absolute paths are forbidden: {FilePath}");
			}

			if (FilePath.StartsWith(@"..\", StringComparison.Ordinal) || FilePath.Contains(@"\..\"))
			{
				throw new Exception($"Parent directory traversal is forbidden: {FilePath}");
			}
		}

		public ModManifestEntry(string filePath, long fileSize, string checksum)
		{
			FilePath = filePath;
			FileSize = fileSize;
			Checksum = checksum;
		}

		public override string ToString()
		{
			return $"{FilePath}\t{FileSize}\t{Checksum}";
		}

		public override bool Equals(object obj)
		{
			if (ReferenceEquals(this, obj))
			{
				return true;
			}

			if (!(obj is ModManifestEntry m))
			{
				return false;
			}

			return FileSize == m.FileSize &&
				   FilePath.Equals(m.FilePath, StringComparison.OrdinalIgnoreCase) &&
				   Checksum.Equals(m.Checksum, StringComparison.OrdinalIgnoreCase);
		}

		public override int GetHashCode()
		{
			unchecked
			{
				int hashCode = FilePath?.GetHashCode() ?? 0;
				hashCode = hashCode * 397 ^ FileSize.GetHashCode();
				hashCode = hashCode * 397 ^ (Checksum?.GetHashCode() ?? 0);
				return hashCode;
			}
		}
	}
}
*/