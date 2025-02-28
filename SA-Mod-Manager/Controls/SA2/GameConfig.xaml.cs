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
using SAModManager.Management;

namespace SAModManager.Controls.SA2
{
    /// <summary>
    /// Interaction logic for GameConfig.xaml
    /// </summary>
    public partial class GameConfig : UserControl
    {
        #region Variables
        public GameSettings GameProfile;

        bool suppressEvent = false;
        private static string patchesPath = null;
        #endregion

        

        public GameConfig(ref object gameSettings, ref bool suppressEvent_)
        {
            InitializeComponent();
            suppressEvent = suppressEvent_;
            GameProfile = (GameSettings)gameSettings;
            InitPatches();
            Loaded += GameConfig_Loaded;
        }

        #region Internal Functions
        private void GameConfig_Loaded(object sender, RoutedEventArgs e)
        {
            SetupBindings();
            InitPatches();
        }

        #region Graphics Tab
        private void ResolutionChanged(object sender, RoutedEventArgs e)
        {

            NumberBox box = sender as NumberBox;

            switch (box.Name)
            {
                case "txtResY":
                    if (chkRatio.IsChecked == true)
                    {
                        txtResX.Value = Math.Ceiling(txtResY.Value * GraphicsManager.GetRatio(GraphicsManager.Ratio.ratio43));
                    }
                    break;
                case "txtCustomResY":
                    if (chkMaintainRatio.IsChecked == true)
                    {
                        txtCustomResX.Value = Math.Ceiling(txtCustomResY.Value * GraphicsManager.GetRatio(GraphicsManager.Ratio.ratio43));
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
            if (GraphicsManager.Screens.Count > 1)
                GraphicsManager.UpdateResolutionPresets(comboScreen.SelectedIndex);
        }

        private void chkRatio_Click(object sender, RoutedEventArgs e)
        {
            if (chkRatio.IsChecked == true)
            {
                txtResX.IsEnabled = false;
                decimal resYDecimal = txtResY.Value;
                Decimal roundedValue = Math.Round(resYDecimal * GraphicsManager.GetRatio(GraphicsManager.Ratio.ratio43));
                txtResX.Value = roundedValue;
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
                    txtResY.Value = GraphicsManager.ResolutionPresets[index].Height;

                    if (chkRatio.IsChecked == false)
                        txtResX.Value = GraphicsManager.ResolutionPresets[index].Width;
                    break;

                case "comboCustomWindow":
                    txtCustomResY.Value = GraphicsManager.ResolutionPresets[index].Height;

                    if (chkRatio.IsChecked == false)
                        txtCustomResX.Value = GraphicsManager.ResolutionPresets[index].Width;
                    break;
            }

            suppressEvent = false;
        }

        private void chkMaintainRatio_Click(object sender, RoutedEventArgs e)
        {
            if (chkMaintainRatio.IsChecked == true)
            {
                txtCustomResX.IsEnabled = false;
                decimal ratio = txtResX.Value / txtResY.Value;
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
		private void InitPatches()
		{
			if (Directory.Exists(App.CurrentGame.modLoaderDirectory))
			{
				string pathDest = Path.Combine(App.CurrentGame.modLoaderDirectory, "Patches.json");
				if (File.Exists(pathDest))
					patchesPath = pathDest;

				SetPatches();
			}
		}

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
		private bool GetPatchCheckState(PatchesData patch)
		{
			if (GameProfile.EnabledGamePatches.ContainsKey(patch.Name))
				return GameProfile.EnabledGamePatches[patch.Name];
			else
				return patch.IsChecked;
		}

		private List<PatchesData> GetPatches(ref ListView list)
		{
			list.Items.Clear();

			var patches = PatchesList.Deserialize(patchesPath);

			if (patches is not null)
			{
				var listPatch = patches.Patches;

				foreach (var patch in listPatch)
				{
					string nKey = "GamePatches." + patch.Name;              // Display Name Key
					string lnString = Lang.GetString(nKey);
					string dKey = "GamePatches." + patch.Name + "Desc";     // Description Key
					string ldString = Lang.GetString(dKey);

					patch.InternalName = lnString == nKey ? patch.InternalName : lnString;
					patch.Description = ldString == dKey ? patch.Description : ldString;

					patch.IsChecked = GetPatchCheckState(patch);
				}

				return listPatch;
			}

			return null;
		}

		public void SetPatches()
        {
            listPatches.Items.Clear();

            List<PatchesData> patches = GetPatches(ref listPatches);

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

		private void btnResetPatches_Click(object sender, RoutedEventArgs e)
		{
			PatchesList defaults = PatchesList.Deserialize(patchesPath);

			foreach (PatchesData patch in listPatches.Items)
			{
				foreach (var value in defaults.Patches)
				{
					if (patch.Name == value.Name)
					{
						patch.IsChecked = value.IsChecked;
						defaults.Patches.Remove(value);
						break;
					}
				}
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

            settings.EnabledGamePatches.Clear();
            foreach (PatchesData patch in listPatches.Items)
				settings.EnabledGamePatches.Add(patch.Name, patch.IsChecked);
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
            comboScreen.ItemsSource = GraphicsManager.Screens;
            comboScreen.DisplayMemberPath = "Key";
            txtResX.MinValue = 0;
            txtResY.MinValue = 0;
            txtResX.SetBinding(NumberBox.ValueProperty, new Binding("HorizontalResolution")
            {
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay
            });
            txtResY.SetBinding(NumberBox.ValueProperty, new Binding("VerticalResolution")
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
            txtCustomResX.MinValue = 0;
            txtCustomResY.MinValue = 0;
            txtCustomResX.SetBinding(NumberBox.ValueProperty, new Binding("CustomWindowWidth")
            {
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay
            });
            txtCustomResX.SetBinding(NumberBox.IsEnabledProperty, new Binding("ScreenMode")
            {
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay,
                Converter = new SA2CustomWindowEnabledConverter()
            });
            txtCustomResY.SetBinding(NumberBox.ValueProperty, new Binding("CustomWindowHeight")
            {
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay
            });
            txtCustomResY.SetBinding(NumberBox.IsEnabledProperty, new Binding("ScreenMode")
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
            refreshRateNum.SetBinding(NumberBox.ValueProperty, new Binding("RefreshRate")
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
            chkStretchToWindow.SetBinding(CheckBox.IsCheckedProperty, new Binding("StretchToWindow")
            {
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay,
            });
            chkDisableBorderImage.SetBinding(CheckBox.IsCheckedProperty, new Binding("DisableBorderImage")
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
                Source = GameProfile.Graphics,
                Mode = BindingMode.TwoWay
            });
            tsVoiceLanguage.SetBinding(ComboBox.SelectedIndexProperty, new Binding("GameVoiceLanguage")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });

            DebugConfig.SetBinding(DebugOptions.SettingsProperty, new Binding("DebugSettings")
            {
                Source = GameProfile,
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
