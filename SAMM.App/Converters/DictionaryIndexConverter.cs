using Avalonia.Data.Converters;
using DynamicData;
using SAMM.Configuration.Mods;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAMM.App.Converters
{
	public class DictionaryIndexConverter : IValueConverter
	{
		public string DefaultKey { get; set; } = string.Empty;

		public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			string val = (string)value;
			Dictionary<string, object> dict = (Dictionary<string,object>)parameter;

			if (dict != null)
			{
				if (dict.ContainsKey(val))
				{
					return dict.Keys.IndexOf(val);
				}
			}

			return 0;
		}

		public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			int val = (int)value;
			Dictionary<string, object> dict = (Dictionary<string, object>)parameter;

			if (dict != null)
			{
				if (dict.Count > val)
				{
					string key = dict.ElementAt(val).Key;
				}
			}

			return DefaultKey;

		}
	}
}
