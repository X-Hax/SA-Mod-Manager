using Avalonia.Data.Converters;
using SAMM.Configuration.Mods;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAMM.App.Converters
{
	public class ModInfoDescriptionConverter : IValueConverter
	{
		public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			ModEntry entry = value as ModEntry;
			if (entry != null)
				return entry.ModInfo.Description;
			else
				return string.Empty;
		}

		public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			throw new NotImplementedException();
		}
	}
}
