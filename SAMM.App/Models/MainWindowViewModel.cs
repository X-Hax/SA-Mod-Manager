using DynamicData;
using ReactiveUI;
using SAMM.Configuration;
using SAMM.Configuration.Mods;
using SAMM.Utilities;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
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

		private Dictionary<string, string> supportedThemes = ADVTheme.ADVTheme.ThemeVariants;
		public ReadOnlyDictionary<string, string> SupportedThemes {  get { return new ReadOnlyDictionary<string, string>(supportedThemes); } }

		public ManagerSettings ManagerSettings { get; set; } = new ManagerSettings();

		public int SelectedLanguage
		{
			get
			{
				int i = SupportedLanguages.Keys.IndexOf(ManagerSettings.Language);
				if (i < 0)
					i = 0;

				return i;
			}
			set
			{
				ManagerSettings.Language = SupportedLanguages.ElementAt(value).Key;
				ADVTheme.ADVTheme.SetLanguage(ManagerSettings.Language);
				this.RaisePropertyChanged(nameof(SelectedLanguage));
			}
		}

		public int SelectedTheme
		{
			get
			{
				int i = SupportedThemes.Keys.IndexOf(ManagerSettings.Theme);
				if (i < 0)
					i = 0;

				return i;
			}
			set
			{
				ManagerSettings.Theme = SupportedThemes.ElementAt(value).Key;
				ADVTheme.ADVTheme.SetTheme(ManagerSettings.Theme);
				this.RaisePropertyChanged(nameof(SelectedTheme));
			}
		}

		private GameConfig gameConfig = new GameConfig();
		public GameConfig GameConfig 
		{ 
			get { return gameConfig; } 
			set { this.RaiseAndSetIfChanged(ref gameConfig, value); }
		}

		public GameSettings? GameSettings { get; set; }

		public UserProfiles? UserProfiles { get; set; }

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

			Logger.Log($"Loading Mod Entries");
			ModEntries = new ObservableCollection<ModEntry>(ModEntry.GetModEntries(GameConfig.GameDirectory));
		}

		#endregion

		#region Public Functions
		public void LoadGameConfig(bool loadEntries = false)
		{
			GameEntry gameEntry = ManagerSettings.GetCurrentGameEntry();
			Logger.Log($"Loading {gameEntry.Name}");
			GameConfig = GameConfig.GetGameConfig(gameEntry.ID, gameEntry.Directory);
			if (loadEntries)
				LoadModEntries();
		}

		public void LoadGameSettings()
		{

		}

		public void LoadUserProfiles()
		{
			Logger.Log($"Loading User Profiles");
			UserProfiles = UserProfiles.LoadProfiles(Path.Combine(GameConfig.GameModsFolder, ".modloader", "profiles"));
		}

		public void SaveManagerSettings()
		{
			string settings = Path.Combine(FolderManager.GetAppFolder(ManagerSettings.AdvancedSettings.IsLocal), "Manager.json");
			ManagerSettings.Serialize(settings);
		}

		#endregion

		#region Constructors
		public MainWindowViewModel()
		{
			string settings = Path.Combine(FolderManager.GetAppFolder(), "Manager.json");
			ManagerSettings = ManagerSettings.Deserialize(settings);
			LoadGameConfig();
			LoadModEntries();
			LoadUserProfiles();
		}

		#endregion

		
	}
}