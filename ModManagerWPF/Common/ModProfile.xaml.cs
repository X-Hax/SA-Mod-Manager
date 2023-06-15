using IniFile;
using System;

using System.ComponentModel;
using System.IO;

using System.Windows;
using System.Windows.Automation;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using static ModManagerWPF.MainWindow;

namespace ModManagerWPF.Common
{
	/// <summary>
	/// Interaction logic for ModProfile.xaml
	/// </summary>
	/// 
	public class Profile
	{
		public string name { get; set; }
		public string iniPath { get; set; }
	}

	public partial class ModProfile : Window
	{
		private ComboBox _modProfile { get; set; }
		public ModProfile(ref ComboBox modProfile)
		{
			InitializeComponent();
			_modProfile = modProfile; 
			Title = Lang.GetString("TitleModProfile");

			foreach (Profile profile in modProfile.Items)
			{
				ProfileListView.Items.Add(profile);
			}
		}

		private void RefreshList()
		{
			ICollectionView view = CollectionViewSource.GetDefaultView(ProfileListView.Items);
			view.Refresh();
		}

		private void RenameProfileStart()
		{
			var profile = ProfileListView.SelectedItem as Profile;

			if (profile != null)
			{
				EditProfile edit = new EditProfile(profile.name, (char)ProfileListView.SelectedIndex);
				edit.Owner = this;
				bool? dialogResult = edit.ShowDialog();

				if (dialogResult == true) 
				{
					EditProfileResult result = edit.GetEditProfileResult(); // Get the result from EditProfile
					Profile item = (Profile)ProfileListView.Items[result.Index];	
					if (File.Exists(item.iniPath))
					{
						string newName = Path.Combine(modDirectory, result.Name + ".ini");

						try
						{
							File.Move(item.iniPath, newName);
							item.name = result.Name;
							item.iniPath = newName;
							RefreshList();
						}
						catch
						{
							MessageBox.Show(Lang.GetString("ProfileRenameError"), Lang.GetString("Error"), MessageBoxButton.OK, MessageBoxImage.Error);
						}
					}
				}		
			}
		}

		private void UI_MouseDoubleClick(object sender, MouseButtonEventArgs e)
		{
			RenameProfileStart();
		}

		private void UI_Add_Click(object sender, RoutedEventArgs e)
		{
			var newProfile = new EditProfile();

			newProfile.Owner = this;
			bool? dialogResult = newProfile.ShowDialog();

			if (dialogResult == true)
			{
				Profile result = newProfile.GetNewProfileResult();
				ProfileListView.Items.Add(result);
				RefreshList();
			}
		}

		private void NewProfile_Closed(object sender, EventArgs e)
		{
			RefreshList();
		}

		private void UpdateModProfile()
		{
			_modProfile.Items.Clear();

			foreach (Profile profile in ProfileListView.Items)
			{
				_modProfile.Items.Add(profile);
			}
		}

		private void UI_OK_Click(object sender, RoutedEventArgs e)
		{
			this.Close();
		}

		private void ProfileRename_Click(object sender, RoutedEventArgs e)
		{
			RenameProfileStart();
		}

		private void ProfileClone_Click(object sender, RoutedEventArgs e)
		{
			var profile = ProfileListView.SelectedItem as Profile;

			if (profile != null)
			{
				Profile clonedProfile = new()
				{
					name = profile.name,
					iniPath = profile.iniPath,
				};

				Random rng = new Random();
				string cloneID = rng.Next().ToString();
				string newName = clonedProfile.iniPath.Replace(".ini", "");
				clonedProfile.name += "_" + cloneID;
				clonedProfile.iniPath = newName + "_" + cloneID + ".ini";

				//unlikely since random, but just in case
				if (ProfileListView.Items.Contains(clonedProfile.name))
				{
					ProfileListView.Items.Remove(clonedProfile);
				}
		
				ProfileListView.Items.Add(clonedProfile);
				RefreshList();

				if (File.Exists(profile.iniPath))
				{
					File.Copy(profile.iniPath, clonedProfile.iniPath, true);	
				}
			}
		}

		private void ProfileDelete_Click(object sender, RoutedEventArgs e)
		{
			var profile = ProfileListView.SelectedItem as Profile;

			if (profile == null)
				return;

			var msg = MessageBox.Show(Lang.GetString("DeleteProfileWarning"), Lang.GetString("Warning"), MessageBoxButton.YesNo, MessageBoxImage.Warning);
			
			if (msg == MessageBoxResult.Yes)
			{
				if (File.Exists(profile.iniPath))
				{
					File.Delete(profile.iniPath);
					ProfileListView.Items.Remove(profile);		
					RefreshList();
				}
			}
		}

		private void Window_Closed(object sender, EventArgs e)
		{
			UpdateModProfile();
		}
	}

}
