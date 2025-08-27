using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAMM.Configuration.Mods
{
	/// <summary>
	/// Stores info on Mods that are found by the Manager
	/// </summary>
	public class ModEntry
	{
		#region Variables
		public ModInfo? ModInfo { get; set; }

		public string Directory { get; private set; } = string.Empty;

		public bool IsEnabled { get; set; } = false;

		public bool HasConfig { get; set; } = false;

		#endregion

		#region Constructors
		public ModEntry() { }

		public ModEntry(string directory)
		{
			if (Path.Exists(directory))
			{
				if (File.Exists(Path.Combine(directory, "mod.ini")))
				{
					Directory = directory;

				}
			}
		}

		#endregion

		#region Functions


		#endregion
	}
}
