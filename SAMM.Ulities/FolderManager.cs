namespace SAMM.Utilities
{
	public static class FolderManager
	{
		private static string appFolder = "SAManager";
		private static string tempFolder = ".satemp";

		private static string userFolder = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);

		#region Directory Management
		/// <summary>
		/// Creates a directory at the provided path. 
		/// </summary>
		/// <param name="path"></param>
		public static void CreateSafeDirectory(string path)
		{
			try
			{
				if (!string.IsNullOrEmpty(path) && !Directory.Exists(path))
				{
					Directory.CreateDirectory(path);
					Logger.Log($"{path} successfully created!");
				}
				Logger.Log($"{path} already exists!");
			}
			catch (UnauthorizedAccessException ex)
			{
				ExceptionHandler.Throw(ex);
			}
		}

		/// <summary>
		/// Deletes a ReadOnly directory from a <see cref="DirectoryInfo"/>.
		/// </summary>
		/// <param name="dir"></param>
		public static Task DeleteReadOnlyDirectory(DirectoryInfo dir)
		{
			return Task.Run(() => 
			{
				// Recursively perform this function 
				foreach (var subDir in dir.GetDirectories())
					DeleteReadOnlyDirectory(subDir);

				// Delete all files in the directory and remove readonly
				try
				{
					foreach (var file in dir.GetFiles())
					{
						file.Attributes = FileAttributes.Normal;
						file.Delete();
						Logger.Log($"Deleted {file}");
					}

					dir.Attributes = FileAttributes.Normal;
					dir.Delete();
					Logger.Log($"Deleted {dir}");
				}
				catch (Exception ex)
				{
					ExceptionHandler.Throw(ex);
				}
			});
		}

		/// <summary>
		/// Deletes a provided directory path.
		/// </summary>
		/// <param name="dir"></param>
		public static Task DeleteReadOnlyDirectory(string dir)
		{
			return Task.Run(() =>
			{
				if (Directory.Exists(dir))
					DeleteReadOnlyDirectory(new DirectoryInfo(dir));
			});
		}

		/// <summary>
		/// Copies a folder's content from one directory to another. Creates the destination directory if it doesn't exist.
		/// </summary>
		/// <param name="sourceFolder"></param>
		/// <param name="dest"></param>
		/// <param name="dllCheck"></param>
		public static Task CopyFolder(string sourceFolder, string destinationFolder, string searchPattern = "", bool recursive = false)
		{
			return Task.Run(() =>
			{
				DirectoryInfo sourceDirectory = new(sourceFolder);
				DirectoryInfo destinationDirectory = new(destinationFolder);

				CreateSafeDirectory(destinationFolder);
				Logger.Log($"Copying files from {sourceFolder} to {destinationFolder}.");

				List<FileInfo> files = sourceDirectory.GetFiles(searchPattern).ToList();
				Parallel.ForEach(files, file =>
				{
					string destFilepath = Path.Combine(destinationDirectory.FullName, file.Name);
					Logger.Log($"Copying {file.Name}");
					file.CopyTo(destFilepath, true);
				});

				List<DirectoryInfo> subdirectories = sourceDirectory.GetDirectories().ToList();
				Parallel.ForEach(subdirectories, subDirectory =>
				{
					string destSubDirectory = Path.Combine(destinationDirectory.FullName, subDirectory.Name);
					CopyFolder(subDirectory.FullName, destSubDirectory, searchPattern);
				});
			});
		}

		/// <summary>
		/// Moves a directory's contents to a destination directory.
		/// </summary>
		/// <param name="sourceFolder"></param>
		/// <param name="destinationFolder"></param>
		/// <returns></returns>
		public static Task MoveDirectory(string sourceFolder, string destinationFolder)
		{
			return Task.Run(() =>
			{
				DirectoryInfo sourceDirectory = new(sourceFolder);
				DirectoryInfo destinationDirectory = new(destinationFolder);

				CreateSafeDirectory(destinationFolder);
				Logger.Log($"Moving files from {sourceFolder} to {destinationFolder}.");

				sourceDirectory.MoveTo(destinationFolder);
			});
		}

		/// <summary>
		/// Moves a directory's contents if the match the supplied <paramref name="searchPattern"/>.
		/// </summary>
		/// <param name="sourceFolder"></param>
		/// <param name="destinationFolder"></param>
		/// <param name="searchPattern"></param>
		/// <param name="recursive"></param>
		/// <returns></returns>
		public static Task MoveDirectory(string sourceFolder, string destinationFolder, string searchPattern = "", bool recursive = false)
		{
			return Task.Run(() =>
			{
				DirectoryInfo sourceDirectory = new(sourceFolder);
				DirectoryInfo destinationDirectory = new(destinationFolder);

				CreateSafeDirectory(destinationFolder);
				Logger.Log($"Copying files from {sourceFolder} to {destinationFolder}.");

				List<FileInfo> files = sourceDirectory.GetFiles(searchPattern).ToList();
				Parallel.ForEach(files, file =>
				{
					string destFilepath = Path.Combine(destinationDirectory.FullName, file.Name);
					Logger.Log($"Copying {file.Name}");
					file.CopyTo(destFilepath, true);
				});

				List<DirectoryInfo> subdirectories = sourceDirectory.GetDirectories().ToList();
				Parallel.ForEach(subdirectories, subDirectory =>
				{
					string destSubDirectory = Path.Combine(destinationDirectory.FullName, subDirectory.Name);
					CopyFolder(subDirectory.FullName, destSubDirectory, searchPattern);
				});
			});
		}

		#endregion

		#region Variable Folders
		private static Task<bool> CreateLocalOrUserFolder(string path, bool localMode = false)
		{
			return Task.Run(() =>
			{
				if (localMode)
				{
					Logger.Log($"Creating local folder: {path}");
					CreateSafeDirectory(path);
					if (Directory.Exists(path))
					{
						Logger.Log($"{path} successfully created!");
						return true;
					}
					else
						return false;
				}
				else
				{
					string userPath = Path.Combine(userFolder, path);
					CreateSafeDirectory(userPath);
					if (Directory.Exists(userPath))
						return true;
					else
						return false;
				}
			});
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="localMode"></param>
		/// <returns></returns>
		public static Task<bool> CreateAppFolder(bool localMode = false)
		{
			return CreateLocalOrUserFolder(appFolder, localMode);
		}

		/// <summary>
		/// Returns the full path to the App Folder. Creates the App Folder if it does not exist.
		/// </summary>
		/// <param name="localMode"></param>
		/// <returns></returns>
		public static string GetAppFolder(bool localMode = false)
		{
			if (CreateAppFolder(localMode).Result)
			{
				if (localMode)
					return Path.GetFullPath(appFolder);
				else
					return Path.GetFullPath(Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), appFolder));
			}
			else
				return null;
		}

		/// <summary>
		/// Moves the App Folder from being in the User folder to a local folder or vice versa.
		/// </summary>
		/// <param name="movingToLocal"></param>
		public static async void MoveAppFolder(bool movingToLocal)
		{
			string localPath = Path.GetFullPath(appFolder);
			string userPath = Path.GetFullPath(Path.Combine(userFolder, appFolder));

			if (movingToLocal)
			{
				if (Directory.Exists(userPath))
				{
					await CreateAppFolder();
					await MoveDirectory(userPath, localPath);
				}
			}
			else
			{
				if (Directory.Exists(localPath))
				{
					await CreateAppFolder(true);
					await MoveDirectory(localPath, userPath);
				}
			}
		}

		/// <summary>
		/// Creates the temporary folder for downloads.
		/// </summary>
		/// <returns>True if the folder exists after creation, false if not.</returns>
		public static Task<bool> CreateTempFolder()
		{
			return CreateLocalOrUserFolder(tempFolder, true);
		}

		/// <summary>
		/// Returns the full path to the temporary folder. Creates it if it does not exist.
		/// </summary>
		/// <returns></returns>
		public static string GetTempFolder()
		{
			if (CreateTempFolder().Result)
				return Path.GetFullPath(tempFolder);
			else
				return null;
		}

		/// <summary>
		/// Deletes the temporary folder.
		/// </summary>
		public static void DeleteTempFolder()
		{
			DeleteReadOnlyDirectory(tempFolder);
		}

		#endregion
	}
}
