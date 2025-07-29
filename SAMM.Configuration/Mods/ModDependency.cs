namespace SAMM.Configuration.Mods
{
	public class ModDependency
	{
		#region Variables
		/// <summary>
		/// ID of the Dependency.
		/// </summary>
		public string ID { get; set; } = string.Empty;

		/// <summary>
		/// Folder of the Dependency.
		/// </summary>
		public string Folder { get; set; } = string.Empty;

		/// <summary>
		/// Name of the Dependency.
		/// </summary>
		public string Name { get; set; } = string.Empty;

		/// <summary>
		/// URL to download the Dependency.
		/// </summary>
		public string Link { get; set; } = string.Empty;

		#endregion

		#region Constructors
		public ModDependency() { }

		public ModDependency(string depdency)
		{
			string[] strings = depdency.Split('|');

			ID = strings[0];
			Folder = strings[1];
			Name = strings[2];
			Link = strings[3];
		}

		#endregion

		#region Functions
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

		#endregion
	}
}
