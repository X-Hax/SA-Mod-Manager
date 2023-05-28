using System.Collections.Generic;

namespace ModManagerWPF.Themes
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
}
