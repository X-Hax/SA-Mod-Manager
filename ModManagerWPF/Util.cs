using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Input;
using System.Threading;

namespace SAModManager
{
    class Util
	{
		private static double multiplier;

		public static void MoveFile(string origin, string dest)
		{
			try
			{
				File.Move(origin, dest);
			}
			catch //File.Move doesn't work if hard drive destination is different from source, copy doesn't have this problem
			{
				int failSafe = 0;
				File.Copy(origin, dest);

				do
				{
					if (failSafe == 10)
					{
						return;
					}
					Thread.Sleep(1000);
					failSafe++;
				} while(!File.Exists(dest));


				File.Delete(origin);
			}
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

		public static string GetSaveNumber(string s)
		{
			string number = s[^2..];

			if (int.TryParse(number, out int result))
			{
				// The last two characters are numbers
				if (number[0] == '0')
				{
					number = number.Substring(1);
				}
			}

			return number;
		}

		public static void SetTaskCount(int count, double max)
		{
			multiplier = max / (double)count;
		}

		public static double SetProgress(double value)
		{
			return (int)(value * 100);
		}


	}
}
