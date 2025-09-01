using Avalonia.Data.Converters;
using HtmlAgilityPack;
using SAMM.Configuration.Mods;
using System;
using System.Globalization;

namespace SAMM.App.Converters
{
	/// <summary>
	/// Handles conversion of a SelectedItem to a ModEntry and returns the Config value.
	/// </summary>
	public class ModConfigExists : IValueConverter
	{
		public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			ModEntry entry = value as ModEntry;
			if (entry != null) 
				return entry.HasConfig;
			else
				return false;
		}

		public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			throw new NotImplementedException();
		}
	}
}
