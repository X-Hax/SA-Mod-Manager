using SAModManager.Configuration;
using System.Collections.Generic;
using System.IO;

using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using static SAModManager.MainWindow;

namespace SAModManager.Common
{
	/// <summary>
	/// Interaction logic for EditProfile.xaml
	/// </summary>
	/// 
	public partial class EditProfile : Window
	{
		private bool isEditing { get; set; } = false;
		private string origProfile = string.Empty;

		private List<string> Mods;
		private List<string> Codes;

		public ProfileEntry Result { get; set; }

		public EditProfile(List<string> mods = null, List<string> codes = null)
		{
			InitializeComponent();
			Title = Lang.GetString("ManagerProfile.Buttons.Create");
			Header.Text = Title;

			Mods = mods ?? new List<string>();
			Codes = codes ?? new List<string>();
		}

		public EditProfile(ProfileEntry profile)
		{
			InitializeComponent();
			Title = Lang.GetString("ManagerProfile.Buttons.Rename");
			Header.Text = Title;
			ProfileNameTextbox.Text = profile.Name;
			origProfile = profile.Filename;
			KeepMods.Visibility = Visibility.Collapsed;
			isEditing = true;
		}

		private void Window_PreviewKeyDown(object sender, KeyEventArgs e)
		{
			if (e.Key == Key.Enter)
			{
				SaveProfile();
				e.Handled = true;
			}
		}

		private void Window_Loaded(object sender, RoutedEventArgs e)
		{
			ProfileNameTextbox.Focus();
		}

		private void SaveNewProfile()
		{
			string profileName = ProfileNameTextbox.Text;
			string profileFilename = profileName + ".json";

			if (profileName != string.Empty)
			{
				if (File.Exists(Path.Combine(App.CurrentGame.ProfilesDirectory, profileFilename)))
				{
					new MessageWindow(Lang.GetString("ManagerProfile.Errors.ProfileExists.Title"), Lang.GetString("ManagerProfile.Errors.ProfileExists"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
					return;
				}

				switch (App.CurrentGame.gameName)
				{
					case "Sonic Adventure DX":
						Configuration.SADX.GameSettings sadxSettings = new()
						{
							GamePath = App.CurrentGame.gameDirectory,
                        };
						if ((bool)KeepMods.IsChecked)
						{
							sadxSettings.EnabledMods = Mods;
							sadxSettings.EnabledCodes = Codes;
						}
                        sadxSettings.Serialize(Path.Combine(App.CurrentGame.ProfilesDirectory, profileFilename), profileFilename);
						break;
					case "Sonic Adventure 2":
                        Configuration.SADX.GameSettings sa2Settings = new()
                        {
                            GamePath = App.CurrentGame.gameDirectory,
						};
                        if ((bool)KeepMods.IsChecked)
                        {
							sa2Settings.EnabledMods = Mods;
							sa2Settings.EnabledCodes = Codes;
                        }
                        sa2Settings.Serialize(Path.Combine(App.CurrentGame.ProfilesDirectory, profileFilename), profileFilename);
						break;
				}

				Result = new(profileName, profileFilename);
			}
		}

		private void SaveEditedProfile()
		{
			string profileName = ProfileNameTextbox.Text;
			string profileFilename = profileName + ".json";

			if (profileFilename == origProfile)
			{
				Result = new ProfileEntry(profileName, profileFilename);
				return;
			}

			if (profileName != string.Empty)
			{
				string originalProfile = Path.Combine(App.CurrentGame.ProfilesDirectory, origProfile);

				if (File.Exists(Path.Combine(App.CurrentGame.ProfilesDirectory, profileFilename)))
				{
					new MessageWindow(Lang.GetString("ManagerProfile.Errors.ProfileExists.Title"), Lang.GetString("ManagerProfile.Errors.ProfileExists"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
					return;
				}


				switch (App.CurrentGame.gameName)
				{
					case "Sonic Adventure DX":
						Configuration.SADX.GameSettings sadxSettings =	Configuration.SADX.GameSettings.Deserialize(originalProfile);
						sadxSettings.GamePath = App.CurrentGame.gameDirectory;
						sadxSettings.Serialize(Path.Combine(App.CurrentGame.ProfilesDirectory, profileFilename), profileFilename);
						break;
					case "Sonic Adventure 2":
						Configuration.SADX.GameSettings sa2Settings = new Configuration.SADX.GameSettings();
						sa2Settings.GamePath = App.CurrentGame.gameDirectory;
						sa2Settings.Serialize(Path.Combine(App.CurrentGame.ProfilesDirectory, profileFilename), profileFilename);
						break;
				}
				File.Delete(originalProfile);

				Result = new(profileName, profileFilename);
			}
		}

		private void SaveProfile()
		{
			if (isEditing)
				SaveEditedProfile();
			else
				SaveNewProfile();

			DialogResult = true;
		}

		private void UI_OK_Click(object sender, RoutedEventArgs e)
		{
			SaveProfile();
		}

		private void UI_Cancel_Click(object sender, RoutedEventArgs e)
		{
			this.Close();
		}
	}
}
