namespace SAMM.Utilities
{
	public static class FolderManager
	{
		public static void CreateSafeDirectory(string path)
		{
			try
			{
				if (!string.IsNullOrEmpty(path) && !Directory.Exists(path))
				{
					Directory.CreateDirectory(path);
				}
			}
			catch (UnauthorizedAccessException ex)
			{
				//new MessageWindow(Lang.GetString("MessageWindow.Errors.PermissionTitle"), string.Format(Lang.GetString("MessageWindow.Errors.FolderPermission"), path) + "\n\n" + ex.Message, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
			}
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

		public static void ClearTempFolder()
		{
			//DeleteReadOnlyDirectory(App.tempFolder);
			//if (Directory.Exists(".SATemp")) //temp support for previous version but no longer used
			//	DeleteReadOnlyDirectory(".SATemp");
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

		public static void MoveDirectory(string sourcePath, string destinationPath)
		{

			CreateSafeDirectory(destinationPath);

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
	}
}
