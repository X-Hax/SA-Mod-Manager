using Avalonia.Controls;
using DialogHostAvalonia;
using SAMM.App.Models.Dialogs;
using SAMM.Configuration;
using SAMM.Configuration.SA2B;
using SAMM.Configuration.SADX;
using SAMM.Utilities;
using System.IO;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace SAMM.App.Controls.Dialogs;

public partial class ProfileManagerDialog : UserControl
{
	private string profilesPath = string.Empty;

	private ProfileManagerDialogModel? profileModel { get => DataContext as ProfileManagerDialogModel; }

    public ProfileManagerDialog() { InitializeComponent(); }

    public ProfileManagerDialog(UserProfiles profiles, string profilespath)
    {
        InitializeComponent();
        DataContext = new ProfileManagerDialogModel(profiles);
		profilesPath = profilespath;
    }

	private string incrementName(string name)
	{
		string nname = name.TrimEnd();
		Regex pattern = new Regex(@"\(\d\)");
		int increment = 1;
		if (pattern.IsMatch(nname))
		{
			Match match = pattern.Match(nname);
			if (match.Captures.Count > 0)
			{
				string val = match.Captures[-1].Value.Trim('(', ')');
				increment += int.Parse(val);
			}
		}

		return $"{nname} {increment}";
	}

	private string validatedFilename(string filename)
	{
		string fullPath = Path.Combine(profilesPath, filename);
		if (File.Exists(fullPath))
		{
			string newFilename = Path.GetFileNameWithoutExtension(filename).Trim();
			Regex pattern = new Regex(@"\(\d\)");
			int increment = 1;
			if (pattern.IsMatch(newFilename))
			{
				Match match = pattern.Match(newFilename);
				if (match.Captures.Count > 0)
				{
					string val = match.Captures[-1].Value.Trim('(', ')');
					increment += int.Parse(val);
				}
			}

			newFilename += $" ({increment})";

			return validatedFilename(newFilename);
		}

		return fullPath;
	}

	private async void _AddProfile(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
	{
		TextInputDialog input = new TextInputDialog("", "ProfileManagerDialogHost");

		await DialogHost.Show(input, "ProfileManagerDialogHost");
		
		if (input.IsOK)
		{
			string name = input.OutputText;
			profileModel.Profiles.Profiles.Add(new UserProfile(name, $"{name}.json"));
			string profileFilename = Path.Combine(profilesPath, $"{name}.json");
			GameSettings? settings = null;
			switch (profileModel.Profiles.GameID)
			{
				case Configuration.Enumeration.GameIDs.SADX:
					settings = new SADXSettings();
					break;
				case Configuration.Enumeration.GameIDs.SA2B:
					settings = new SA2BSettings();
					break;
			}

			if (settings != null)
			{
				settings.Serialize(profileFilename);
				profileModel.SaveProfiles(profilesPath);
			}
		}
	}

	private async void _EditProfile(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
	{
		UserProfile profile = profileModel.Profiles.Profiles[ProfileList.SelectedIndex];
		TextInputDialog input = new TextInputDialog(profile.Name, "ProfileManagerDialogHost");

		await DialogHost.Show(input, "ProfileManagerDialogHost");

		if (input.IsOK)
		{
			string profileName = input.OutputText;
			string newFilename = $"{profileName}.json";
			string oldFilepath = Path.Combine(profilesPath, profile.Filename);
			string newFilepath = Path.Combine(profilesPath, newFilename);

			if (File.Exists(newFilepath))
				MessageHandler.OpenDialog("Invalid Filename", $"{newFilename} already exists, please use a different name.");
			else
			{
				File.Copy(oldFilepath, newFilepath, true);
				File.Delete(oldFilepath);

				profile.Name = profileName;
				profile.Filename = newFilename;

				profileModel.SaveProfiles(profilesPath);
			}
		}
	}

	private async void _DuplicateProfile(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
	{
		await Task.Run(() =>
		{
			foreach (UserProfile profile in ProfileList.SelectedItems)
			{
				string fullPath = Path.Combine(profilesPath, profile.Filename);
				if (File.Exists(fullPath))
				{
					string name = incrementName(profile.Name);
					string filename = $"{incrementName(Path.GetFileNameWithoutExtension(profile.Filename))}.json";
					UserProfile dupe = new UserProfile(name, filename);

					
				}
			}

			profileModel.SaveProfiles(profilesPath);
		});
	}

	private async void _DeleteProfile(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
	{
		// Placeholder, make sure to call this at the end of this function.
		//profileModel.SaveProfiles(profilesPath);
	}

	private async void _ImportProfile(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
	{
		// Placeholder, make sure to call this at the end of this function.
		//profileModel.SaveProfiles(profilesPath);
	}

	private void _OpenProfileFolder(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
	{
		FolderManager.OpenFolder(profilesPath);
	}

	private void CloseDialog(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
	{
		DialogHost.Close("MainWindowDialogHost");
	}
}