using DynamicData;
using ReactiveUI;
using SAMM.App.Views;
using SAMM.Configuration;
using SAMM.Configuration.Enumeration;
using SAMM.Configuration.Mods;
using SAMM.Configuration.SA2B;
using SAMM.Configuration.SADX;
using SAMM.Utilities;
using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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

		private GameSettings gameSettings = null;
		public GameSettings? GameSettings 
		{
			get => gameSettings;
			set => this.RaiseAndSetIfChanged(ref gameSettings, value); 
		}

		private UserProfiles userProfiles = null;
		public UserProfiles? UserProfiles 
		{
			get => userProfiles;
			set => this.RaiseAndSetIfChanged(ref userProfiles, value);
		}

		private ObservableCollection<ModEntry> modEntries = new ObservableCollection<ModEntry>();
		public ObservableCollection<ModEntry> ModEntries 
		{ 
			get { return modEntries; } 
			set { this.RaiseAndSetIfChanged(ref modEntries, value); }
		}

		private string CurrentUserProfilePath { get { return Path.Combine(GameConfig.ProfilesPath, UserProfiles.CurrentProfile.Filename); } }

		#endregion

		#region Add
		public void AddUserProfile(string name)
		{
			UserProfiles.Profiles.Add(new UserProfile(name, $"{name}.json"));
		}

		#endregion

		#region Load
		/// <summary>
		/// Populates the <see cref="ModEntries"/> in the view model.
		/// 
		/// If <paramref name="clearEntries"/> is true, it will ensure the list is cleared prior to loading.
		/// </summary>
		/// <param name="clearEntries"></param>
		private void LoadModEntries(bool clearEntries = true)
		{
			if (GameSettings == null) return;

			if (clearEntries && ModEntries.Count > 0)
				ModEntries.Clear();

			Logger.Log($"Loading Mod Entries");
			Dictionary<string, ModEntry> modList = ModEntry.GetModDictionary(GameConfig.GameDirectory);

			List<string> missingEntries = new List<string>();
			foreach (var modEntry in GameSettings.EnabledMods)
			{
				if (modList.ContainsKey(modEntry))
					modList[modEntry].IsEnabled = true;
				else
					missingEntries.Add(modEntry);
			}

			if (missingEntries.Count > 0)
				GameSettings.EnabledMods.Remove(missingEntries);

			List<ModEntry> sortedMods = new List<ModEntry>();
			if (ManagerSettings.KeepModOrder)
			{
				foreach (var entry in GameSettings.ModsList)
				{
					if (modList.ContainsKey(entry))
					{
						sortedMods.Add(modList[entry]);
						modList.Remove(entry);
					}
					else
					{
						missingEntries.Add($"{entry} {App.GetString("MainWindow.Mods.MissingMods.Addendum")}");
						GameSettings.ModsList.Remove(entry);
					}
				}

				if (modList.Count > 0)
					sortedMods.AddRange(modList.Values.ToList());
			}
			else
			{
				foreach (var entry in GameSettings.EnabledMods)
					sortedMods.Add(modList[entry]);
				var unselected = modList.Values.ToList().Where(m => !m.IsEnabled);
				sortedMods.AddRange(unselected);
			}

			if (missingEntries.Count > 0)
			{
				StringBuilder sb = new StringBuilder();
				foreach (var entry in missingEntries)
					sb.AppendLine($"{entry} was not located.");

				MessageHandler.OpenDialog("Oops", sb.ToString());
			}

			ModEntries = new ObservableCollection<ModEntry>(sortedMods);
		}

		private void LoadGameConfig(bool loadEntries = false)
		{
			GameEntry gameEntry = ManagerSettings.GetCurrentGameEntry();
			Logger.Log($"Loading {gameEntry.Name}");
			GameConfig = GameConfig.GetGameConfig(gameEntry.ID, gameEntry.Directory);
			if (loadEntries)
				LoadModEntries();
		}

		private void LoadGameSettings()
		{
			Logger.Log($"Loading Game Settings");
			if (UserProfiles != null)
			{
				switch (GameConfig.GameID)
				{
					case GameIDs.SADX:
						GameSettings = GameSettings.Deserialize<SADXSettings>(CurrentUserProfilePath);
						break;
					case GameIDs.SA2B:
						GameSettings = GameSettings.Deserialize<SA2BSettings>(CurrentUserProfilePath);
						break;
				}
			}
		}

		private void LoadUserProfiles()
		{
			Logger.Log($"Loading User Profiles");
			UserProfiles = UserProfiles.LoadProfiles(GameConfig.ProfilesPath);
			GameConfig.GameID = UserProfiles.GameID;
		}

		public void Load()
		{
			LoadGameConfig();
			LoadUserProfiles();
			LoadGameSettings();
			LoadModEntries();
		}
		
		#endregion

		#region Save
		public void SaveGameSettings()
		{
			GameSettings.EnabledMods.Clear();
			GameSettings.ModsList.Clear();

			foreach (var entry in ModEntries)
			{
				if (entry.IsEnabled)
					GameSettings.EnabledMods.Add(entry.Key);

				GameSettings.ModsList.Add(entry.Key);
			}

			Logger.Log($"Saving current Game Settings profile: {CurrentUserProfilePath}");
			GameSettings.Serialize(CurrentUserProfilePath);
		}

		public void SaveUserProfiles()
		{
			Logger.Log($"Saving current User Profiles: {GameConfig.ProfilesPath}");
			UserProfiles.Save(GameConfig.ProfilesPath);
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
			Load();
		}

		#endregion
	}
}