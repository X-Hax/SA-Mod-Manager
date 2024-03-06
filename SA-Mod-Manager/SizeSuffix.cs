using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAModManager
{
	// Source: http://stackoverflow.com/questions/14488796/does-net-provide-an-easy-way-convert-bytes-to-kb-mb-gb-etc
	public static class SizeSuffix
	{
		private static readonly string[] sizeSuffixes =
			{ "bytes", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };

		public static string GetSizeSuffix(long value, int decimalPlaces = 2)
		{
			if (value < 0) { return "-" + GetSizeSuffix(-value); }
			if (value == 0) { return "0.0 bytes"; }

			// mag is 0 for bytes, 1 for KB, 2, for MB, etc.
			int mag = (int)Math.Log(value, 1024);

			// 1L << (mag * 10) == 2 ^ (10 * mag) 
			// [i.e. the number of bytes in the unit corresponding to mag]
			decimal adjustedSize = (decimal)value / (1L << (mag * 10));

			// make adjustment when the value is large enough that
			// it would round up to 1000 or more
			if (Math.Round(adjustedSize, decimalPlaces) >= 1000)
			{
				mag += 1;
				adjustedSize /= 1024;
			}

			return string.Format("{0:n" + decimalPlaces + "} {1}",
				adjustedSize,
				sizeSuffixes[mag]);
		}
	}
}
