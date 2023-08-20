using SAModManager.Configuration;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;

namespace SAModManager.Common
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
		private Profiles Profiles { get; set; }

		public ModProfile(ref Profiles profiles)
		{
			InitializeComponent();
			Profiles = profiles;
			Title = Lang.GetString("ManagerProfile.Title");

			Loaded += ModProfile_Loaded;
        }

		private void ModProfile_Loaded(object sender, RoutedEventArgs e)
		{
			ProfileListView.ItemsSource = Profiles.ProfilesList;
		}

		#region Private Functions
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
				EditProfile edit = new(profile.name, (char)ProfileListView.SelectedIndex)
				{
					Owner = this
				};
				bool? dialogResult = edit.ShowDialog();

				if (dialogResult == true)
				{
					EditProfileResult result = edit.GetEditProfileResult(); // Get the result from EditProfile
					Profile item = (Profile)ProfileListView.Items[result.Index];
					if (File.Exists(item.iniPath))
					{
						string newName = Path.Combine(App.CurrentGame.modDirectory, result.Name + ".ini");

						try
						{
							File.Move(item.iniPath, newName);
							item.name = result.Name;
							item.iniPath = newName;
							RefreshList();
						}
						catch
						{
							new MessageWindow(Lang.GetString("Error"), Lang.GetString("ManagerProfile.Errors.Unexpected"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
						}
					}
				}
			}
		}
		#endregion

		#region Window
		private void NewProfile_Closed(object sender, EventArgs e)
		{
			RefreshList();
		}
		#endregion

		#region Mouse Controls
		private void UI_MouseDoubleClick(object sender, MouseButtonEventArgs e)
		{
			RenameProfileStart();
		}

		#region Context Menu
		private void ProfileList_OnPreviewKeyDown(object sender, KeyEventArgs e)
		{
			if (ProfileListView == null)
				return;

			var profile = (Profile)ProfileListView.SelectedItem;

			if (profile == null)
				return;

			var ctrlKey = Keyboard.IsKeyDown(Key.LeftCtrl) || Keyboard.IsKeyDown(Key.RightCtrl);


			if (ctrlKey)
			{
				if (Keyboard.IsKeyDown(Key.D))
					ProfileClone_Click(null, null);

				e.Handled = true;
			}

			if (Keyboard.IsKeyDown(Key.F2))
			{
				ProfileRename_Click(null, null);
				e.Handled = true;
			}


			if (Keyboard.IsKeyDown(Key.Delete))
			{
				ProfileDelete_Click(null, null);
				e.Handled = true;
			}

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

				Random rng = new();
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
			var selectedItems = ProfileListView.SelectedItems;
			var count = selectedItems.Count > 0;

			if (count)
			{
				var msg = new MessageWindow(Lang.GetString("Warning"), Lang.GetString("MessageWindow.Warnings.DeleteProfile"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);
				msg.ShowDialog();

				if (msg.isYes)
				{
					ProfileListView.BeginInit();
					foreach (var sel in selectedItems)
					{
						var item = (Profile)sel;

						if (File.Exists(item.iniPath))
						{
							File.Delete(item.iniPath);
							ProfileListView.Items.Remove(item);
						}
					}

					RefreshList();
					ProfileListView.EndInit();
				}
			}
		}
		#endregion
		#endregion

		#region Buttons
		private void UI_Add_Click(object sender, RoutedEventArgs e)
		{
			var newProfile = new EditProfile();

			newProfile.Owner = this;
			bool? dialogResult = newProfile.ShowDialog();

			if (dialogResult == true)
			{
				Profile result = newProfile.GetNewProfileResult();
				
				RefreshList();
			}
		}

		private void UI_OK_Click(object sender, RoutedEventArgs e)
		{
			this.Close();
		}
		#endregion
	}
}
