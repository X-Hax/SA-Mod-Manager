using Avalonia.Data.Converters;
using System;
using System.Collections.Generic;
using System.Globalization;

namespace SAMM.App.Converters
{
	/// <summary>
	/// Returns either the parameter (if length is greater than 0) or returns the value.
	/// 
	/// Consider Value as the fallback, Parameter is the overwriter.
	/// </summary>
	public class ConfigDisplayNameConverter : IMultiValueConverter
	{
		public object? Convert(IList<object?> value, Type targetType, object? parameter, CultureInfo culture)
		{
			string fallback = value[0] as string;
			string overwrite = value[1] as string;
			if (overwrite != null && overwrite.Length > 0)
				if (overwrite != "DisplayName")
					return overwrite;

			return fallback;
		}
	}
}
