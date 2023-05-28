using System.Collections.Generic;

namespace ModManagerWPF.Languages
{
	public class LangEntry
	{
		public string FileName { get; set; }
		public string Name { get; set; }

		public override string ToString() //Show original language name instead of class namespace in the Resource
		{
			return Name;
		}
	}
	public class LanguageList : List<LangEntry>
	{
		public static int TotalLines;
	}
}
