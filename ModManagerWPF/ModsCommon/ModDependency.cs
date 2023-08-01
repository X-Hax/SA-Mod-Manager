

namespace SAModManager.ModsCommon
{
	public class ModDependency
	{
		public string ID { get; set; }
		public string Folder { get; set; }
		public string Name { get; set; }
		public string Link { get; set; }

		public ModDependency(string depdency)
		{
			string[] strings = depdency.Split('|');

			ID = strings[0];
			Folder = strings[1];
			Name = strings[2];
			Link = strings[3];
		}

		public string GetDependencyName()
		{
			if (Name != "")
				return Name;
			else if (Folder != "")
				return Folder;
			else if (ID != "")
				return ID;
			else
				return "[Mod Name Not Provided]";
		}
	}
}
