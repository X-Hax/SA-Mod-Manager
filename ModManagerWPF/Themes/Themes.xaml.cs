using System.Collections.Generic;
using System.Windows.Media;

namespace SAModManager.Themes
{
	public class ThemeEntry
	{
		public string FileName { get; set; }
		public string Name { get; set; }

		public override string ToString() //Show original Theme name instead of class namespace in the Resource
		{
			return Name;
		}
	}

	public class ThemeList : List<ThemeEntry>
	{

	}

	public class ThemeBrush
	{
		public static Brush GetThemeBrush(string key)
		{
			return App.Current.TryFindResource(key) as Brush;
		}

		public static Color GetThemeColor(string key)
		{
			var resource = App.Current.TryFindResource(key);

			if (resource is null)
				return Colors.Black;

			if (resource is SolidColorBrush brush)
				return brush.Color;
			else if (resource is Color color)
				return color;

			return Colors.Black;
		}
	}


}
