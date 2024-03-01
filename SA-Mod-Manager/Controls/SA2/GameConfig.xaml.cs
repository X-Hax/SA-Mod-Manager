using SAModManager.Configuration;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using SAModManager.Configuration.SA2;

namespace SAModManager.Controls.SA2
{
	/// <summary>
	/// Interaction logic for GameConfig.xaml
	/// </summary>
	public partial class GameConfig : UserControl
	{
        #region Variables
        public GameSettings GameProfile;
        public GraphicsHelper graphics;

        bool suppressEvent = false;
        private static string patchesPath = null;
        #endregion

        public GameConfig(ref object gameSettings)
        {
            InitializeComponent();
            GameProfile = (GameSettings)gameSettings;
            graphics = new GraphicsHelper(ref comboScreen);
            if (App.CurrentGame?.modDirectory != null)
            {
                string pathDest = Path.Combine(App.CurrentGame.modDirectory, "Patches.json");
                if (File.Exists(pathDest))
                    patchesPath = pathDest;
                SetPatches();
            }
            Loaded += GameConfig_Loaded;
        }

        #region Internal Functions
        private void GameConfig_Loaded(object sender, RoutedEventArgs e)
        {
            SetupBindings();
            SetPatches();
        }

        #region Graphics Tab
        private void ResolutionChanged(object sender, RoutedEventArgs e)
        {

            NumericUpDown box = sender as NumericUpDown;

            switch (box.Name)
            {
                case "txtResY":
                    if (chkRatio.IsChecked == true)
                    {
                        double ratio = (4.0 / 3.0);
                        txtResX.Value = Math.Ceiling(txtResY.Value * ratio);
                    }
                    break;
                case "txtCustomResY":
                    if (chkMaintainRatio.IsChecked == true)
                    {
                        double ratio = txtResX.Value / txtResY.Value;
                        txtCustomResX.Value = Math.Ceiling(txtCustomResY.Value * ratio);
                    }
                    break;
            }

            if (!suppressEvent)
                comboDisplay.SelectedIndex = -1;

        }

        private void HorizontalRes_Changed(object sender, RoutedEventArgs e)
        {
            if (!suppressEvent)
                comboDisplay.SelectedIndex = -1;
        }

        private void comboScreen_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            graphics?.screenNumBox_SelectChanged(ref comboScreen, ref comboDisplay);
        }

        private void chkRatio_Click(object sender, RoutedEventArgs e)
        {
            if (chkRatio.IsChecked == true)
            {
                txtResX.IsEnabled = false;
                decimal resYDecimal = (decimal)txtResY.Value;
                decimal roundedValue = Math.Round(resYDecimal * GraphicsHelper.ratio);
                txtResX.Value = (double)roundedValue;
            }
            else if (!suppressEvent)
            {
                txtResX.IsEnabled = true;
            }
        }

        private void DisplaySize_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            ComboBox box = (ComboBox)sender;

            if (box.SelectedIndex == -1)
                return;

            int index = box.SelectedIndex;

            suppressEvent = true;

            switch (box.Name)
            {
                case "comboDisplay":
                    txtResY.Value = graphics.resolutionPresets[index].Height;

                    if (chkRatio.IsChecked == false)
                        txtResX.Value = graphics.resolutionPresets[index].Width;
                    break;

                case "comboCustomWindow":
                    txtCustomResY.Value = graphics.resolutionPresets[index].Height;

                    if (chkRatio.IsChecked == false)
                        txtCustomResX.Value = graphics.resolutionPresets[index].Width;
                    break;
            }

            suppressEvent = false;
        }

        private void chkMaintainRatio_Click(object sender, RoutedEventArgs e)
        {
            if (chkMaintainRatio.IsChecked == true)
            {
                txtCustomResX.IsEnabled = false;
                double ratio = txtResX.Value / txtResY.Value;
                txtCustomResX.Value = Math.Ceiling(txtCustomResY.Value * ratio);
            }
            else if (!suppressEvent)
            {
                txtCustomResX.IsEnabled = true;
            }
        }

        #endregion

     
        #endregion

        #region Patches Tab
        private PatchesData GetPatchFromView(object sender)
        {
            if (sender is ListViewItem lvItem)
                return lvItem.Content as PatchesData;
            else if (sender is ListView lv)
                return lv.SelectedItem as PatchesData;


            return listPatches.Items[listPatches.SelectedIndex] as PatchesData;
        }

        private void PatchViewItem_MouseEnter(object sender, MouseEventArgs e)
        {

            var patch = GetPatchFromView(sender);

            if (patch is null)
                return;

            PatchAuthor.Text += ": " + patch.Author;
            PatchCategory.Text += ": " + patch.Category;
            PatchDescription.Text += " " + patch.Description;
        }

        private void PatchViewItem_MouseLeave(object sender, MouseEventArgs e)
        {
            PatchAuthor.Text = Lang.GetString("CommonStrings.Author");
            PatchCategory.Text = Lang.GetString("CommonStrings.Category");
            PatchDescription.Text = Lang.GetString("CommonStrings.Description");
        }

        private static List<PatchesData> GetPatches(ref ListView list, GameSettings set)
        {
            list.Items.Clear();

            var patches = PatchesList.Deserialize(patchesPath);

            if (patches is not null)
            {
                var listPatch = patches.Patches;

                foreach (var patch in listPatch)
                {
                    // Convert patch name to the corresponding property name in GamePatches class
                    string propertyName = patch.Name.Replace(" ", ""); // Adjust the naming convention as needed
                    var property = typeof(GamePatches).GetProperty(propertyName);

                    if (property != null)
                    {
                        // Update the IsChecked property based on the GamePatches class
                        patch.IsChecked = (bool)property.GetValue(set.Patches);
                    }

                    string desc = "GamePatchesSA2." + patch.Name + "Desc";
                    patch.InternalName = patch.Name;
                    patch.Name = Lang.GetString("GamePatchesSA2." + patch.Name);
                    patch.Description = Lang.GetString(desc); //need to use a variable otherwise it fails for some reason
                }

                return listPatch;
            }

            return null;
        }

        public void SetPatches()
        {
            listPatches.Items.Clear();

            List<PatchesData> patches = GetPatches(ref listPatches, GameProfile);

            if (patches is not null)
            {
                foreach (var patch in patches)
                {
                    listPatches.Items.Add(patch);
                }
            }
        }

        private void btnSelectAllPatch_Click(object sender, RoutedEventArgs e)
        {
            foreach (PatchesData patch in listPatches.Items)
            {
                patch.IsChecked = true;
            }
            RefreshPatchesList();
        }

        private void btnDeselectAllPatch_Click(object sender, RoutedEventArgs e)
        {
            foreach (PatchesData patch in listPatches.Items)
            {
                patch.IsChecked = false;
            }
            RefreshPatchesList();

        }

        private void RefreshPatchesList()
        {
            ICollectionView view = CollectionViewSource.GetDefaultView(listPatches.Items);
            view.Refresh();
        }
        #endregion

        public void SavePatches(ref object input)
        {
            GameSettings settings = input as GameSettings;

            if (listPatches is null)
                return;

            foreach (PatchesData patch in listPatches.Items)
            {
                string propertyName = patch.InternalName;
                var propertyInfo = typeof(GamePatches).GetProperty(propertyName);

                if (propertyInfo != null && propertyInfo.CanWrite)
                {
                    propertyInfo.SetValue(settings.Patches, patch.IsChecked);
                }
                else
                {
                    throw new InvalidOperationException($"Property {propertyName} not found or read-only.");
                }
            }
        }

        #region Private Functions
        private void SetupBindings()
        {
            // Graphics Bindings

            // Display Options
            comboScreen.SetBinding(ComboBox.SelectedIndexProperty, new Binding("SelectedScreen")
            {
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay,
            });
            txtResX.MinValue = 0;
            txtResY.MinValue = 0;
            txtResX.SetBinding(NumericUpDown.ValueProperty, new Binding("HorizontalResolution")
            {
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay
            });
            txtResY.SetBinding(NumericUpDown.ValueProperty, new Binding("VerticalResolution")
            {
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay
            });
            chkRatio.SetBinding(CheckBox.IsCheckedProperty, new Binding("Enable43ResolutionRatio")
            {
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay
            });
            comboScreenMode.SetBinding(ComboBox.SelectedIndexProperty, new Binding("ScreenMode")
            {
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay,
            });
            System.Drawing.Rectangle rect = graphics.GetRectangleStruct();
            txtCustomResX.MinValue = 0;
            txtCustomResY.MinValue = 0;
            txtCustomResX.MaxValue = rect.Width;
            txtCustomResY.MaxValue = rect.Height;
            txtCustomResX.SetBinding(NumericUpDown.ValueProperty, new Binding("CustomWindowWidth")
            {
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay
            });
            txtCustomResX.SetBinding(NumericUpDown.IsEnabledProperty, new Binding("ScreenMode")
            {
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay,
                Converter = new SA2CustomWindowEnabledConverter()
            });
            txtCustomResY.SetBinding(NumericUpDown.ValueProperty, new Binding("CustomWindowHeight")
            {
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay
            });
            txtCustomResY.SetBinding(NumericUpDown.IsEnabledProperty, new Binding("ScreenMode")
            {
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay,
                Converter = new SA2CustomWindowEnabledConverter()
            });
            comboCustomWindow.SetBinding(ComboBox.IsEnabledProperty, new Binding("ScreenMode")
            {
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay,
                Converter = new SA2CustomWindowEnabledConverter()
            });
            chkMaintainRatio.SetBinding(CheckBox.IsEnabledProperty, new Binding("ScreenMode")
            {
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay,
                Converter = new SA2CustomWindowEnabledConverter()
            });
            chkMaintainRatio.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableKeepResolutionRatio")
            {
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay
            });
			chkResizeAspect.SetBinding(CheckBox.IsCheckedProperty, new Binding("KeepAspectWhenResizing")
			{
				Source = GameProfile.Graphics,
				Mode = BindingMode.TwoWay
			});

            // Settings
            chkPause.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnablePauseOnInactive")
            {
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay
            });
            chkResizableWin.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableResizableWindow")
            {
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay
            });
            ChkSkipIntro.SetBinding(CheckBox.IsCheckedProperty, new Binding("SkipIntro")
            {
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay
            });
			tsTextLanguage.SetBinding(ComboBox.SelectedIndexProperty, new Binding("GameTextLanguage")
			{
				Source = GameProfile.TestSpawn,
				Mode = BindingMode.TwoWay
			});
			tsVoiceLanguage.SetBinding(ComboBox.SelectedIndexProperty, new Binding("GameVoiceLanguage")
			{
				Source = GameProfile.TestSpawn,
				Mode = BindingMode.TwoWay
			});
        }
		#endregion

		private void DownloadDXVK_Click(object sender, RoutedEventArgs e)
		{
			var ps = new ProcessStartInfo("https://github.com/doitsujin/dxvk/releases")
			{
				UseShellExecute = true,
				Verb = "open"
			};
			Process.Start(ps);
		}
	}
}
