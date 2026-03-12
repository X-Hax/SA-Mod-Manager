using SAMM.Configuration.Enumeration;
using SAMM.Utilities;
using System.Collections.ObjectModel;
using System.Text.Json.Serialization;

namespace SAMM.Configuration
{
	public class UserProfile : NotifyObject
	{
		#region Variables
		private string name = string.Empty;

		/// <summary>
		/// Display name of the User Profile.
		/// </summary>
		public string Name 
		{
			get => name;
			set => RaiseAndSetIfChanged<string>(ref name, value);
		}

		private string filename = string.Empty;

		/// <summary>
		/// Name of the file storing the user profile information.
		/// </summary>
		public string Filename 
		{
			get => filename;
			set => RaiseAndSetIfChanged<string>(ref filename, value);
		}

		#endregion

		#region Constructors
		public UserProfile() { }

		public UserProfile(string name, string filename)
		{
			Name = name;
			Filename = filename;
		}

		#endregion

		public override string ToString()
		{
			return Name;
		}
	}

	public class UserProfiles : NotifyObject
	{
		#region Variables
		[JsonIgnore]
		public GameIDs GameID { get; set; }

		private int profileIndex = 0;
		/// <summary>
		/// Index of the current/last selected profile.
		/// </summary>
		public int ProfileIndex 
		{ 
			get => profileIndex;
			set => RaiseAndSetIfChanged<int>(ref profileIndex, value);
		}

		private ObservableCollection<UserProfile> profiles = new ObservableCollection<UserProfile>();

		/// <summary>
		/// List of all profiles the user currently has saved.
		/// </summary>
		[JsonPropertyName("ProfilesList")]
		public ObservableCollection<UserProfile> Profiles 
		{
			get => profiles;
			set => RaiseAndSetIfChanged<ObservableCollection<UserProfile>>(ref profiles, value);
		}

		/// <summary>
		/// This is the currently selected profile in the loaded profiles. 
		/// 
		/// If the selected index is greater than the amount of profiles in the list, it will return the profile that's index is 0.
		/// </summary>
		[JsonIgnore]
		public UserProfile CurrentProfile 
		{ 
			get 
			{
				if (!(ProfileIndex <= Profiles.Count))
					ProfileIndex = 0;

				return Profiles[ProfileIndex];
			} 
		}

		#endregion

		#region Constructors
		public UserProfiles() { }

		#endregion

		public void Save(string profilePath)
		{
			profilePath = Path.Combine(profilePath, "Profiles.json");
			JSON.Serialize(profilePath, this);
		}

		#region Static
		public static UserProfiles LoadProfiles(string profilePath)
		{
			profilePath = Path.Combine(profilePath, "Profiles.json");
			return JSON.Deserialize<UserProfiles>(profilePath);
		}

		#endregion
	}
}
