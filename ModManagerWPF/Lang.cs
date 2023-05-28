using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Resources;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace ModManagerWPF
{
	public static class Lang
	{
		public static string GetString(string key)
		{
			var resource = Application.Current.TryFindResource(key);

			if (resource is string str)
				return str;

			return key;
		}
	}
}
