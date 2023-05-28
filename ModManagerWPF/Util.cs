using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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
    }
}
