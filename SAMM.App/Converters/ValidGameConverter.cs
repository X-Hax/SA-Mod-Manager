using Avalonia.Data.Converters;
using SAMM.Configuration;
using SAMM.Configuration.Mods;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAMM.App.Converters
{
	/// <summary>
	/// Validates if the Game Config that is currently set is valid or not.
	/// </summary>
	public class ValidGameConverter : IValueConverter
	{
		public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			GameConfig config = (GameConfig)value;
			if (config != null)
			{
				return Path.Exists(config.GameDirectory);
			}
			else
				return false;
		}

		public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			throw new NotImplementedException();
		}
	}
}
