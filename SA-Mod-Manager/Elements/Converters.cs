using System;
using System.Collections.Generic;
using System.Diagnostics.Eventing.Reader;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;

namespace SAModManager.Elements
{
	public class BoolFlipConverter : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
		{
			if (value is bool boolValue)
			{
				if (boolValue)
					return false;
				else
					return true;
			}

			return Binding.DoNothing;
		}

		public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
		{
			return Binding.DoNothing;
		}
	}

	public class IndexOffsetConverter : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
		{
			if (value is int intValue)
				return intValue - 1;

			return Binding.DoNothing;
		}

		public object ConvertBack(object value, Type targetType, object paremeter, System.Globalization.CultureInfo culture)
		{
			if (value is int intValue)
				return intValue + 1;

			return Binding.DoNothing;
		}
	}

	public class BoolIntConverter : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
		{
			if (value is int intValue)
			{
				if (intValue == 1)
					return true;
				if (intValue == 0)
					return false;
			}

			return Binding.DoNothing;
		}

		public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
		{
			if (value is bool boolValue)
			{
				if (boolValue == true)
					return 1;
				else
					return 0;
			}

			return Binding.DoNothing;
		}
	}

	public class VisibilityConverter : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
		{
			if (value is bool boolValue)
			{
				if (boolValue)
					return Visibility.Visible;
				else
					return Visibility.Collapsed;
			}

			return Binding.DoNothing;
		}

		public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
		{
			return Binding.DoNothing;
		}
	}

	public class CustomWindowEnabledConverter : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
		{
			if (value is int index)
			{
				if (index == 3)
					return true;
				else
					return false;
			}

			return Binding.DoNothing;
		}

		public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
		{
			return Binding.DoNothing;
		}
	}

	public class ShowMouseConverter : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
		{
			if (value is int intValue)
			{
				if (intValue > 0 && intValue < 3)
					return true;
				else
					return false;
			}

			return Binding.DoNothing;
		}

		public object ConvertBack(object value, Type targetType, object paramter, System.Globalization.CultureInfo culture)
		{
			return Binding.DoNothing;
		}
	}

	public class ResizeWindowConverter : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
		{
			if (value is int intValue)
			{
				if (intValue == 0)
					return true;
				else
					return false;
			}

			return Binding.DoNothing;
		}

		public object ConvertBack(object value, Type targetType, object paramter, System.Globalization.CultureInfo culture)
		{
			return Binding.DoNothing;
		}
	}

	public class ScreenModeConverter : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
		{
			if (value is int intValue)
			{
				switch (intValue)
				{
					default:
					case 0:
						return 0;
					// fullscreen
					case 1:
						return 3;
					// borderless
					case 2:
						return 1;
					// custom window
					case 3:
						return 2;
				}
			}
			
			return Binding.DoNothing;
		}

		public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
		{
			if (value is int intValue)
			{
				switch (intValue)
				{
					default:
					case 0:
						return 0;
					case 1:
						return 2;
					case 2:
						return 3;
					case 3:
						return 1;
				}
			}

			return Binding.DoNothing;
		}
	}
}