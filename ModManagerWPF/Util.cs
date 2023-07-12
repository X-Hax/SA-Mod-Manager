using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows;

namespace ModManagerWPF
{
    class Util
    {
		public static void MoveFile(string origin, string dest)
		{
			try
			{
				File.Move(origin, dest);
			}
			catch //File.Move doesn't work if hard drive destination is different from source, copy doesn't have this problem
			{
				File.Copy(origin, dest);
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


	}
}
