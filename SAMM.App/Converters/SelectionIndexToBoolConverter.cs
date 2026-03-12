using Avalonia.Data.Converters;
using DynamicData;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAMM.App.Converters
{
	/// <summary>
	/// Converts a selection index to a bool by verifying the value is greater than -1.
	/// 
	/// If the parameter is supplied, it acts as a mask that returns true only if the index provided is not equal.
	/// </summary>
	public class SelectionIndexToBoolConverter : IValueConverter
	{
		public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			if (value != null)
			{
				int index = (int)value;

				if (index > -1)
				{
					if (parameter != null)
					{
						int refVal = (int)parameter;

						bool isSame = index == refVal;
						if (isSame)
							return false;
					}

					return true;
				}
				else
					return false;
			}

			return false;
		}

		public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			throw new NotImplementedException();
		}
	}
}
