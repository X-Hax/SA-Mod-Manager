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
    /// Interaction logic for Profile.xaml
    /// </summary>
    /// 
    public class Profile
    {
        public string name { get; set; }
        public string jsonPath { get; set; }
    }

    public partial class ProfileDialog : Window
    {
		public int SelectedIndex = 0;
        private Profiles Profiles { get; set; }

        public ProfileDialog(ref Profiles profiles, int index)
        {
            InitializeComponent();
			SelectedIndex = index;
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
			ProfileListView.Items.Refresh();
        }

        private void RenameProfileStart()
        {
			ProfileEntry entry = ProfileListView.SelectedItem as ProfileEntry;

			EditProfile editProfile = new EditProfile(entry);

			bool? result = editProfile.ShowDialog();

			if (result == true)
			{
				Profiles.ProfilesList[ProfileListView.SelectedIndex] = editProfile.Result;
			}
			RefreshList();
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

			var currentProfile = ProfileListView.Items[SelectedIndex];
			var profile = (ProfileEntry)ProfileListView.SelectedItem;

            if (string.IsNullOrEmpty(profile.Name))
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
				if (ProfileListView.SelectedItems.Count > 0 && ProfileListView.SelectedItems.Count < 2)
				{
					ProfileRename_Click(null, null);
					e.Handled = true;
				}
            }


            if (Keyboard.IsKeyDown(Key.Delete))
            {
				if (!ProfileListView.SelectedItems.Contains(currentProfile))
				{
					ProfileDelete_Click(null, null);
					e.Handled = true;
				}
            }
        }

		private void ContextMenu_Opened(object sender, RoutedEventArgs e)
		{
			var currentProfile = ProfileListView.Items[SelectedIndex];

			// Delete Available Checks
			if (ProfileListView.SelectedItems.Contains(currentProfile))
				ProfileDelete.IsEnabled = false;
			else
				ProfileDelete.IsEnabled = true;

			if (ProfileListView.SelectedItems.Count > 1)
				ProfileRename.IsEnabled = false;
			else
				ProfileRename.IsEnabled = true;
		}

		private void ProfileRename_Click(object sender, RoutedEventArgs e)
        {
            RenameProfileStart();
        }

		private string CopyFileWithAutoRename(string sourceFilePath)
		{
			int count = 1;

			string fileNameOnly = Path.GetFileNameWithoutExtension(sourceFilePath);
			string extension = Path.GetExtension(sourceFilePath);
			string path = Path.GetDirectoryName(sourceFilePath);
			string newFullPath = sourceFilePath;

			string tempFileName = "";
			while (File.Exists(newFullPath))
			{
				tempFileName = string.Format("{0} - Copy ({1})", fileNameOnly, count++);
				newFullPath = Path.Combine(path, tempFileName + extension);
			}

			return tempFileName;
		}

		private void ProfileClone_Click(object sender, RoutedEventArgs e)
        {
            List<ProfileEntry> selection = ProfileListView.SelectedItems.Cast<ProfileEntry>().ToList();

            if (selection.Count > 0)
            {
                foreach (ProfileEntry profile in selection) 
                {
                    string fullPath = Path.Combine(App.CurrentGame.ProfilesDirectory, profile.Filename);

					string clonedName = CopyFileWithAutoRename(Path.Combine(App.CurrentGame.ProfilesDirectory, profile.Filename));
					ProfileEntry clonedProfile = new(clonedName, clonedName + ".json");

                    if (File.Exists(fullPath))
                        File.Copy(fullPath, Path.Combine(App.CurrentGame.ProfilesDirectory, clonedProfile.Filename), true);

					Profiles.ProfilesList.Add(clonedProfile);
                }
            }

            RefreshList();
        }

        private void ProfileDelete_Click(object sender, RoutedEventArgs e)
        {
            List<ProfileEntry> selectedItems = ProfileListView.SelectedItems.Cast<ProfileEntry>().ToList();
			var profile = Profiles.ProfilesList[SelectedIndex];

			if (selectedItems.Count > 0)
            {
                var msg = new MessageWindow(Lang.GetString("Warning"), Lang.GetString("MessageWindow.Warnings.DeleteProfile"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);
                msg.ShowDialog();

                if (msg.isYes)
                {
                    foreach (ProfileEntry item in selectedItems)
                    {
                        var fullPath = Path.Combine(App.CurrentGame.ProfilesDirectory, item.Filename);
                        if (File.Exists(fullPath))
                        {
                            File.Delete(fullPath);
							Profiles.ProfilesList.Remove(item);
                        }
                    }

					SelectedIndex = Profiles.ProfilesList.FindIndex(item => item.Name == profile.Name);

                    RefreshList();
                }
            }
        }
        #endregion
        #endregion

        #region Buttons
        private void UI_Add_Click(object sender, RoutedEventArgs e)
        {
            var newProfile = new EditProfile
            {
                Owner = this
            };
            bool? dialogResult = newProfile.ShowDialog();
          
            if (dialogResult == true)
            {
				Profiles.ProfilesList.Add(newProfile.Result);
            }

			RefreshList();
        }

		private void Migrate_Click(object sender, RoutedEventArgs e)
		{
			// TODO: Implement Migration Feature
		}

		private void UI_OK_Click(object sender, RoutedEventArgs e)
        {
			this.Close();
        }
		#endregion
	}
}
