using ReactiveUI;
using SAMM.Configuration;
using SAMM.Configuration.Mods;
using System.Collections.Generic;

namespace SAMM.App.Models
{
	public class MainWindowViewModel : ReactiveObject
	{
		#region Variables
		public ManagerSettings ManagerSettings { get; set; } = new ManagerSettings();

		public GameConfig GameConfig { get; set; } = new GameConfig();

		public GameSettings? GameSettings { get; set; }

		public List<ModEntry> ModEntries { get; set; } = new List<ModEntry>();

		#endregion

		#region Constructors
		public MainWindowViewModel()
		{
			ModEntry mod = new ModEntry();
			mod.ModInfo = new ModInfo();
			mod.ModInfo.Name = "Test";
			mod.ModInfo.Description = "This is a test description.";
			mod.ModInfo.Author = "ItsATestActually";
			mod.ModInfo.Version = "1.0.0";
			ModEntries.Add(mod);
		}

		#endregion
	}
}
