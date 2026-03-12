using ReactiveUI;
using SAMM.Configuration;

namespace SAMM.App.Models.Dialogs
{
	public class ProfileManagerDialogModel : ReactiveObject
	{
		private int selectedProfileIndex = -1;

		private UserProfiles profiles = new UserProfiles();

		public UserProfiles Profiles
		{
			get => profiles; 
			set => this.RaiseAndSetIfChanged(ref profiles, value);
		}

		public ProfileManagerDialogModel(UserProfiles profiles)
		{
			Profiles = profiles;
		}

		public async void SaveProfiles(string profilePath)
		{
			profiles.Save(profilePath);
		}
	}
}
