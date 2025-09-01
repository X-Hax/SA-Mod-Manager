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
			// The following code is essentially a unit test for the visual component of the SAMM for development.
			// This will all be removed once the UI is mostly complete for the MainWindow and replaced with a proper initializer system.
			GameConfig = SAMM.Configuration.GameConfig.GetGameConfig(Configuration.Enumeration.GameIDs.SADX, "test");
			ModEntry mod = new ModEntry();
			mod.ModInfo = new ModInfo();
			mod.ModInfo.Name = "Test";
			mod.ModInfo.Description = "This is a test description.";
			mod.ModInfo.Author = "ItsATestActually";
			mod.ModInfo.Version = "1.0.0";
			mod.HasConfig = true;
			ModEntries.Add(mod);
			ModEntries.Add(mod);
			ModEntries.Add(mod);
			ModEntries.Add(mod);
			ModEntries.Add(mod);
		}

		#endregion
	}
}
