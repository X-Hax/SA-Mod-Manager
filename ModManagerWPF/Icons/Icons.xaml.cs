using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace ModManagerWPF.Icons
{
	public static class Icons
	{
		public static ResourceDictionary InstallIcon { get; } = GetResourceDictionary("Icons/Install.xaml");
		public static ResourceDictionary UninstallIcon { get; } = GetResourceDictionary("Icons/Uninstall.xaml");

		private static ResourceDictionary GetResourceDictionary(string resourceName)
		{
			var resourceUri = new Uri(resourceName, UriKind.RelativeOrAbsolute);
			return new ResourceDictionary { Source = resourceUri };
		}
	}
}
