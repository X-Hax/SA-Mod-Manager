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
        private ComboBox _modProfile { get; set; }
        private Profiles Profiles { get; set; }


        public ProfileDialog(ref Profiles profiles)
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
            var profile = (KeyValuePair<string, string>)ProfileListView.SelectedItem;

            if (profile.Key.ToLower() == "default")
            {

                return;
            }

            EditProfile edit = new(profile.Key, (char)ProfileListView.SelectedIndex)
            {
                Owner = this
            };
            bool? dialogResult = edit.ShowDialog();

            if (dialogResult == true)
            {
                EditProfileResult result = edit.GetEditProfileResult(); // Get the result from EditProfile
                Profile item = (Profile)ProfileListView.Items[result.Index];
                if (File.Exists(item.jsonPath))
                {
                    string newName = Path.Combine(App.CurrentGame.ProfilesDirectory, result.Name + ".json");

                    try
                    {
                        File.Move(item.jsonPath, newName);
                        item.name = result.Name;
                        item.jsonPath = newName;
                        RefreshList();
                    }
                    catch
                    {
                        new MessageWindow(Lang.GetString("Error"), Lang.GetString("ManagerProfile.Errors.Unexpected"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
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

            var profile = (KeyValuePair<string, string>)ProfileListView.SelectedItem;

            if (string.IsNullOrEmpty(profile.Key))
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
            var list = ProfileListView.SelectedItems;
            var listView = ProfileListView.Items.Cast<KeyValuePair<string, string>>().ToList();

            if (list != null && list.Count > 0)
            {
                foreach ( var item in list) 
                {
                    var profile = (KeyValuePair<string, string>)item;
                    string fullPath = Path.Combine(App.CurrentGame.ProfilesDirectory, profile.Value);

                    Profile clonedProfile = new()
                    {
                        name = profile.Key,
                        jsonPath = profile.Value,
                    };

                    Random rng = new();
                    string cloneID = rng.Next().ToString();
                    string newName = clonedProfile.jsonPath.Replace(".json", "");
                    clonedProfile.name += "_" + cloneID;
                    clonedProfile.jsonPath = newName + "_" + cloneID + ".json";

                    KeyValuePair<string, string> clonedConvert = new(clonedProfile.name, clonedProfile.jsonPath);
                    listView.Add(clonedConvert);

                    if (File.Exists(fullPath))
                    {
                        File.Copy(fullPath, Path.Combine(App.CurrentGame.ProfilesDirectory, clonedProfile.jsonPath), true);
                    }
                }
            }

            ProfileListView.ItemsSource = listView;
            RefreshList();
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
                    var listView = ProfileListView.Items.Cast<KeyValuePair<string, string>>().ToList();

                    foreach (var sel in selectedItems)
                    {
                        var converted = (KeyValuePair<string, string>)sel;
                        var fullPath = Path.Combine(App.CurrentGame.ProfilesDirectory, converted.Value);
                        if (File.Exists(fullPath))
                        {
                            File.Delete(fullPath);
                        }
                        listView.Remove(converted);
                    }

                    ProfileListView.ItemsSource = listView;
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
                var listView = ProfileListView.Items.Cast<KeyValuePair<string, string>>().ToList();
               var profile = newProfile.GetNewProfileResult();

                KeyValuePair<string, string> profileConvert = new(profile.name, profile.jsonPath);
                listView.Add(profileConvert);
                ProfileListView.ItemsSource = listView;
                RefreshList();
            }
        }

        private void UI_OK_Click(object sender, RoutedEventArgs e)
        {
            Profiles.ProfilesList.Clear();

            foreach (var profile in ProfileListView.Items) 
            {
                KeyValuePair<string, string> profileConvert = (KeyValuePair<string, string>)profile;
                Profiles.ProfilesList.Add(profileConvert.Key, Path.GetFileName(profileConvert.Value));
            }
        
            this.Close();
        }
        #endregion
    }
}
