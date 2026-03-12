using Avalonia.Data.Converters;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAMM.App.Converters
{
	public class ActiveProfileToBoolConverter : IMultiValueConverter
	{
		public object Convert(IList<object?> objects, Type? type, object? parameter, CultureInfo cultureInfo)
		{
			if (objects != null)
			{
				int activeIndex = (int)objects[0];
				int referenceIndex = (int)objects[1];

				if (activeIndex != referenceIndex && activeIndex > -1)
					return true;
				else
					return false;
			}
			

			return false;
		}
	}
}
