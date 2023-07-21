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

namespace ModManagerWPF
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
