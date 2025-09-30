using ReactiveUI;
using SAMM.Configuration;
using SAMM.Configuration.Mods;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Runtime.CompilerServices;
using System.Threading.Tasks;
using System.Windows.Input;

namespace SAMM.App.Models
{
	public class MainWindowViewModel : ReactiveObject
	{
		#region Variables
		private Dictionary<string, string> supportedLanguages = new Dictionary<string, string>()
		{
			{ "en-US", "English (US)" },
		};

		public ReadOnlyDictionary<string, string> SupportedLanguages { get { return new ReadOnlyDictionary<string,string>(supportedLanguages); } }

		public ManagerSettings ManagerSettings { get; set; } = new ManagerSettings();

		private GameConfig gameConfig = new GameConfig();

		public GameConfig GameConfig 
		{ 
			get { return gameConfig; } 
			set { this.RaiseAndSetIfChanged(ref gameConfig, value); }
		}

		public GameSettings? GameSettings { get; set; }

		private ObservableCollection<ModEntry> modEntries = new ObservableCollection<ModEntry>();

		public ObservableCollection<ModEntry> ModEntries 
		{ 
			get { return modEntries; } 
			set { this.RaiseAndSetIfChanged(ref modEntries, value); }
		}

		#endregion

		#region Internal Functions
		/// <summary>
		/// Populates the <see cref="ModEntries"/> in the view model.
		/// 
		/// If <paramref name="clearEntries"/> is true, it will ensure the list is cleared prior to loading.
		/// </summary>
		/// <param name="clearEntries"></param>
		private void LoadModEntries(bool clearEntries = true)
		{
			if (clearEntries && ModEntries.Count > 0)
				ModEntries.Clear();

			ModEntries = new ObservableCollection<ModEntry>(ModEntry.GetModEntries(GameConfig.GameDirectory));

			int i = 0;
		}

		#endregion

		#region Public Functions
		public void LoadGameConfig(bool loadEntries = false)
		{
			GameEntry gameEntry = ManagerSettings.GetCurrentGameEntry();
			GameConfig = GameConfig.GetGameConfig(gameEntry.ID, gameEntry.Directory);
			if (loadEntries)
				LoadModEntries();
		}

		#endregion

		#region Constructors
		public MainWindowViewModel()
		{
			// The following code is essentially a unit test for the visual component of the SAMM for development.
			// This will all be removed once the UI is mostly complete for the MainWindow and replaced with a proper initializer system.
			string sadx_path = "D:\\SteamLibrary\\steamapps\\common\\Sonic Adventure DX\\sonic.exe";
			string sa2_path = "D:\\SteamLibrary\\steamapps\\common\\Sonic Adventure 2\\sonic2app.exe";
			GameEntry sa1entry = new GameEntry();
			sa1entry.Create(sadx_path);
			ManagerSettings.GameEntries.Add(sa1entry);
			GameEntry sa2entry = new GameEntry();
			sa2entry.Create(sa2_path);
			ManagerSettings.GameEntries.Add(sa2entry);
			ManagerSettings.Language = "en-US";
			ManagerSettings.CurrentSetGame = 0;
			LoadGameConfig();
			LoadModEntries();
		}

		#endregion

		
	}
}