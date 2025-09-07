using Avalonia.Collections;
using ReactiveUI;
using SAMM.Configuration;
using SAMM.Configuration.Mods;
using System.Collections.ObjectModel;
using System.Reactive;

namespace SAMM.App.Models
{
	public class MainWindowViewModel : ReactiveObject
	{
		#region Variables
		public ManagerSettings ManagerSettings { get; set; } = new ManagerSettings();

		public GameConfig GameConfig { get; set; } = new GameConfig();

		public GameSettings? GameSettings { get; set; }

		public ObservableCollection<ModEntry> ModEntries { get; set; } = new ObservableCollection<ModEntry>();

		#endregion

		#region Commands
		public ReactiveCommand<int, Unit> RefreshModListCommand { get; }

		private void RefreshModList(int index)
		{
			ModEntries = new ObservableCollection<ModEntry>(ModEntry.GetModEntries(GameConfig.GameDirectory));
		}

		#endregion

		#region Internal Functions
	

		#endregion

		#region Constructors
		public MainWindowViewModel()
		{
			// The following code is essentially a unit test for the visual component of the SAMM for development.
			// This will all be removed once the UI is mostly complete for the MainWindow and replaced with a proper initializer system.
			GameConfig = GameConfig.GetGameConfig(Configuration.Enumeration.GameIDs.SADX, "D:\\SteamLibrary\\steamapps\\common\\Sonic Adventure DX");
			ModEntries = new ObservableCollection<ModEntry>(ModEntry.GetModEntries(GameConfig.GameDirectory));

			// All Command Registering should go below this comment.
			RefreshModListCommand = ReactiveCommand.Create<int>(RefreshModList);
		}

		#endregion

		
	}
}