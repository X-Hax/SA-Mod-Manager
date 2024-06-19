using SAModManager.Configuration;
using SAModManager.Ini;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text.Json;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using SAModManager.UI;
using SAModManager.Configuration.SA2;
using SAModManager.Configuration.SADX;

namespace SAModManager.Profile
{
    /// <summary>
    /// Interaction logic for Profile.xaml
    /// </summary>
    /// 
    public partial class ProfileDialog : Window
    {
        public int SelectedIndex = 0;

        public ProfileDialog()
        {
            InitializeComponent();
			ProfileListView.ItemsSource = App.Profiles.ProfilesList;
			SelectedIndex = App.Profiles.ProfileIndex;
            Title = Lang.GetString("ManagerProfile.Title");

            Loaded += ModProfile_Loaded;
        }

        private void ModProfile_Loaded(object sender, RoutedEventArgs e)
        {
			this.Dispatcher.BeginInvoke(new Action(() =>
			{
				ListViewItem item = null;
				if (SelectedIndex >= 0 && SelectedIndex < ProfileListView.Items.Count)
				{
					ProfileListView.ScrollIntoView(ProfileListView.Items[SelectedIndex]);
					item = ProfileListView.ItemContainerGenerator.ContainerFromIndex(SelectedIndex) as ListViewItem;
					if (item != null)
					{
						item.FontWeight = FontWeights.Bold;
					}
				}
			}), System.Windows.Threading.DispatcherPriority.Background);
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

			ProfileListView.Items.Refresh();
        }
        #endregion

        #region Window
        private void NewProfile_Closed(object sender, EventArgs e)
        {
            //RefreshList();
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
            if (ProfileListView == null || ProfileListView.SelectedItem == null)
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
                    if (!ProfileListView.SelectedItems.Contains(currentProfile))
                    {
                        ProfileRename_Click(null, null);
                        e.Handled = true;
                    }
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

            // Delete and Rename Available Checks
            if (ProfileListView.SelectedItems.Contains(currentProfile))
            {
                ProfileDelete.IsEnabled = false;
                ProfileRename.IsEnabled = false;
            }    
            else
            {
                ProfileDelete.IsEnabled = true;
                ProfileRename.IsEnabled = true;
            }            
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

					App.Profiles.ProfilesList.Add(clonedProfile);
                }
            }

            //RefreshList();
        }

        private void ProfileDelete_Click(object sender, RoutedEventArgs e)
        {
			int index = ProfileListView.SelectedIndex;
			List<ProfileEntry> selection = ProfileListView.SelectedItems.Cast<ProfileEntry>().ToList();

			if (selection.Count > 0)
			{
				var msg = new MessageWindow(Lang.GetString("Warning"), Lang.GetString("MessageWindow.Warnings.DeleteProfile"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);
				msg.ShowDialog();
				
				if (msg.isYes)
				{
					foreach (ProfileEntry profile in selection)
					{
						ProfileManager.RemoveProfile(profile.Name);
					}
				}
			}
		}
        #endregion
        #endregion

        #region Buttons
        private void UI_Add_Click(object sender, RoutedEventArgs e)
        {
            var newProfile = new EditProfile((Owner as MainWindow).EnabledMods, (Owner as MainWindow).EnabledCodes)
            {
                Owner = this,
            };
            bool? dialogResult = newProfile.ShowDialog();

            //if (dialogResult == true)
            //{
			//	ProfileManager.AddNewProfile(newProfile.Result);
            //}

            //RefreshList();
        }

        private void Migrate_Click(object sender, RoutedEventArgs e)
        {
            var dialog = new System.Windows.Forms.OpenFileDialog
            {
                Filter = "Old Profiles|*.ini|Profiles|*.json",
                Multiselect = true
            };

            System.Windows.Forms.DialogResult result = dialog.ShowDialog();

            List<string> failedFiles = new();

	
            if (result == System.Windows.Forms.DialogResult.OK)
            {
                int i = 0;
                foreach (string filename in dialog.FileNames)
                {
					string file = Path.GetFullPath(filename);
                    if (File.Exists(file))
                    {
                        bool invalid = false;
                        Configuration.SADX.GameSettings settings = new();
                        Configuration.SA2.GameSettings settingsSA2 = new();
                        string newFileName = Path.GetFileNameWithoutExtension(file);
                        string newFilePath = Path.Combine(App.CurrentGame.ProfilesDirectory, newFileName + ".json");

                        switch (Path.GetExtension(file))
                        {
                            case ".json":
                                try
                                {
                                    switch (App.CurrentGame.id)
                                    {
                                        case SetGame.SADX:
                                            settings = Configuration.SADX.GameSettings.Deserialize(file);
                                            if (settings.Graphics == null)
                                            {
                                                invalid = true;
                                                throw new Exception();
                                            }
                                            break;
                                        case SetGame.SA2:
                                            settingsSA2 = Configuration.SA2.GameSettings.Deserialize(file);
                                            if (settingsSA2.Graphics == null)
                                            {
                                                invalid = true;
                                                throw new Exception();
                                            }
                                            break;
                                    }
                                }
                                catch
                                {
                                    failedFiles.Add(Path.GetFileName(file));
                                }
                                break;
                            case ".ini":
								if (!ProfileManager.MigrateProfile(file))
									failedFiles.Add(Path.GetFileName(file));

								break;
                        }

                        if (!invalid)
                        {
                            foreach (ProfileEntry entry in App.Profiles.ProfilesList)
                            {
                                if (entry.Name == newFileName)
                                {
                                    newFileName = newFileName + " (" + i.ToString() + ")";
                                    newFilePath = Path.Combine(App.CurrentGame.ProfilesDirectory, newFileName + ".json");
                                }
                            }

                            App.Profiles.ProfilesList.Add(new ProfileEntry(newFileName, newFileName + ".json"));
                        }
                    }
                    i++;
                }

                RefreshList();
            }


			if (failedFiles.Count > 0)
            {
                string failedFilesList = string.Join(Environment.NewLine, failedFiles);
                string failedMessage = Lang.GetString("MessageWindow.Warnings.ProfileMigration.Message1") + "\n" +
                    failedFilesList + Lang.GetString("MessageWindow.Warnings.ProfileMigration.Message2");

                MessageWindow message = new MessageWindow(Lang.GetString("MessageWindow.Warnings.ProfileMigration.Title"), failedMessage, icon: MessageWindow.Icons.Warning);
                message.ShowDialog();
            }
        }

        private void UI_OK_Click(object sender, RoutedEventArgs e)
        {
            ProfileManager.SaveProfiles();
            this.Close();
        }
        #endregion
    }
}