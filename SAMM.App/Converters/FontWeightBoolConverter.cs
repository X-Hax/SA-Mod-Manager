using Avalonia.Data.Converters;
using Avalonia.Media;
using System;
using System.Globalization;

namespace SAMM.App.Converters
{
	public class FontWeightBoolConverter : IValueConverter
	{
		public FontWeight FalseWeight { get; set; } = FontWeight.Regular;
		public FontWeight TrueWeight { get; set; } = FontWeight.Bold;

		public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			if ((bool)value)
				return TrueWeight;
			else
				return FalseWeight;
		}

		public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			throw new NotImplementedException();
		}
	}
}
