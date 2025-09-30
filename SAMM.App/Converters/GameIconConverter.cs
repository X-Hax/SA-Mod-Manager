using ADVTheme;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Data.Converters;
using Avalonia.Media;
using Avalonia.Media.Imaging;
using DynamicData;
using SAMM.Configuration.Enumeration;
using System;
using System.Globalization;

namespace SAMM.App.Converters
{
	public class GameIconConverter : IValueConverter
	{
		public object? Convert(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			string resourcekey = string.Empty;
			GameIDs gameID = (GameIDs)value;
			switch (gameID)
			{
				case GameIDs.SADX:
					resourcekey = "GameIcon.SADX";
					break;
				case GameIDs.SA2B:
					resourcekey = "GameIcon.SA2B";
					break;
			}

			if (resourcekey.Length > 0)
				return App.GetResource<Bitmap>(resourcekey);

			return null;
		}

		public object? ConvertBack(object? value, Type targetType, object? parameter, CultureInfo culture)
		{
			throw new NotImplementedException();
		}
	}
}
