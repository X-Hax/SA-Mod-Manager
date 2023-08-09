using SAModManager.Common;
using SAModManager.UI;
using SAModManager.Updater;
using SevenZipExtractor;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using SAModManager.IniSettings.SADX;
using System.Windows.Controls.Primitives;
using SAModManager.Ini;
using SAModManager.Game;
using System.Drawing.Design;

namespace SAModManager.Elements.SADX
{
	/// <summary>
	/// Interaction logic for GameConfig.xaml
	/// </summary>
	public partial class GameConfig : UserControl
	{
		#region Variables

		string gamePath = string.Empty;
		public GameSettings GameProfile;

		private static string d3d8to9InstalledDLLName = Path.Combine(App.CurrentGame.gameDirectory, "d3d8.dll");
		private static string d3d8to9StoredDLLName = Path.Combine(App.extLibPath, "d3d8m", "d3d8m.dll");
        bool suppressEvent = false;

		private readonly double LowOpacityBtn = 0.7;

		private Game.GameConfigFile GameSettings;

		public Game.Graphics graphics;

		public List<PatchesData> Patches;
		#endregion

		public GameConfig(ref GameSettings gameSettings, string gamePath)
		{
			InitializeComponent();
			GameProfile = gameSettings;
			this.gamePath = gamePath;
			graphics = new Game.Graphics(ref comboScreen);
			SetPatches();
            Loaded += GameConfig_Loaded;
		}

		private void GameConfig_Loaded(object sender, RoutedEventArgs e)
		{
			LoadGameConfigIni();
			SetupBindings();
			SetUp_UpdateD3D9();
		}

		public static void UpdateD3D8Paths()
		{
            d3d8to9InstalledDLLName = Path.Combine(App.CurrentGame.gameDirectory, "d3d8.dll");
            d3d8to9StoredDLLName = Path.Combine(App.extLibPath, "d3d8m", "d3d8m.dll");
        }

		private void LoadGameConfigIni()
		{
			GameSettings = IniSerializer.Deserialize<Game.GameConfigFile>(Path.Combine(gamePath, "sonicDX.ini"));

			if (GameSettings.GameConfig == null)
			{
				GameSettings.GameConfig = new()
				{
					FrameRate = (int)FrameRate.High,
					Sound3D = 1,
					SEVoice = 1,
					BGM = 1,
					BGMVolume = 100,
					VoiceVolume = 100
				};
			}
		}

		#region Private Functions
		private void SetupBindings()
		{
			// Graphics Tab Bindings
			// Screen Settings
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

			// Window Settings
			chkRatio.SetBinding(CheckBox.IsCheckedProperty, new Binding("Enable43ResolutionRatio")
			{
				Source = GameProfile.Graphics,
				Mode = BindingMode.TwoWay
			});
			chkVSync.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableVsync")
			{
				Source = GameProfile.Graphics,
				Mode = BindingMode.TwoWay
			});
			chkPause.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnablePauseOnInactive")
			{
				Source = GameProfile.Graphics,
				Mode = BindingMode.TwoWay
			});
			chkDynamicBuffers.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableDynamicBuffer")
			{
				Source = GameProfile.Graphics,
				Mode = BindingMode.TwoWay
			});
			chkBorderless.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableBorderless")
			{
				Source = GameProfile.Graphics,
				Mode = BindingMode.TwoWay
			});
			chkScaleScreen.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableScreenScaling")
			{
				Source = GameProfile.Graphics,
				Mode = BindingMode.TwoWay
			});
			chkResizableWin.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableResizableWindow")
			{
				Source = GameProfile.Graphics,
				Mode = BindingMode.TwoWay
			});
			chkCustomWinSize.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableCustomWindow")
			{
				Source = GameProfile.Graphics,
				Mode = BindingMode.TwoWay
			});
			chkMaintainRatio.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableKeepResolutionRatio")
			{
				Source = GameProfile.Graphics,
				Mode = BindingMode.TwoWay
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
			txtCustomResY.SetBinding(NumericUpDown.ValueProperty, new Binding("CustomWindowHeight")
			{
				Source = GameProfile.Graphics,
				Mode = BindingMode.TwoWay
			});

			// Game Config Settings
			radFullscreen.SetBinding(RadioButton.IsCheckedProperty, new Binding("FullScreen")
			{
				Source = GameSettings.GameConfig,
				Mode = BindingMode.TwoWay
			});
			comboFramerate.SetBinding(ComboBox.SelectedIndexProperty, new Binding("FrameRate")
			{
				Source = GameSettings.GameConfig,
				Converter = new FrameRateConverter(),
				Mode = BindingMode.TwoWay,
			});
			comboDetail.SetBinding(ComboBox.SelectedIndexProperty, new Binding("ClipLevel")
			{
				Source = GameSettings.GameConfig,
				Mode = BindingMode.TwoWay
			});
			comboFog.SetBinding(ComboBox.SelectedIndexProperty, new Binding("Foglation")
			{
				Source = GameSettings.GameConfig,
				Mode = BindingMode.TwoWay
			});

			// Enhancement Settings
			comboBGFill.SetBinding(ComboBox.SelectedIndexProperty, new Binding("FillModeBackground")
			{
				Source = GameProfile.Graphics,
				Mode = BindingMode.TwoWay
			});
			comboFMVFill.SetBinding(ComboBox.SelectedIndexProperty, new Binding("FillModeFMV")
			{
				Source = GameProfile.Graphics,
				Mode = BindingMode.TwoWay
			});
			comboTextureFilter.SetBinding(ComboBox.SelectedIndexProperty, new Binding("ModeTextureFiltering")
			{
				Source = GameProfile.Graphics,
				Mode = BindingMode.TwoWay
			});
			comboUIFilter.SetBinding(ComboBox.SelectedIndexProperty, new Binding("ModeUIFiltering")
			{
				Source = GameProfile.Graphics,
				Mode = BindingMode.TwoWay
			});
			checkMipmapping.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableForcedMipmapping")
			{
				Source = GameProfile.Graphics,
				Mode = BindingMode.TwoWay
			});
			checkUIScale.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableUIScaling")
			{
				Source = GameProfile.Graphics,
				Mode = BindingMode.TwoWay
			});

			// Input Settings
			radBetterInput.SetBinding(RadioButton.IsCheckedProperty, new Binding("EnabledInputMod")
			{
				Source = GameProfile.Controller,
				Mode = BindingMode.TwoWay
			});

			// Audio Settings
			checkEnableMusic.SetBinding(CheckBox.IsCheckedProperty, new Binding("BGM")
			{
				Source = GameSettings.GameConfig,
				Converter = new BoolIntConverter(),
				Mode = BindingMode.TwoWay
			});
			checkEnableSounds.SetBinding(CheckBox.IsCheckedProperty, new Binding("SEVoice")
			{
				Source = GameSettings.GameConfig,
				Converter = new BoolIntConverter(),
				Mode = BindingMode.TwoWay
			});
			checkBassMusic.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableBassMusic")
			{
				Source = GameProfile.Sound,
				Mode = BindingMode.TwoWay
			});
			checkBassSFX.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableBassSFX")
			{
				Source = GameProfile.Sound,
				Mode = BindingMode.TwoWay
			});
			checkEnable3DSound.SetBinding(CheckBox.IsCheckedProperty, new Binding("Sound3D")
			{
				Source = GameSettings.GameConfig,
				Converter = new BoolIntConverter(),
				Mode = BindingMode.TwoWay
			});
			sliderMusic.Minimum = 0;
			sliderMusic.Maximum = 100;
			sliderMusic.SetBinding(ScrollBar.ValueProperty, new Binding("BGMVolume")
			{
				Source = GameSettings.GameConfig,
				Mode = BindingMode.TwoWay
			});
			sliderVoice.Minimum = 0;
			sliderVoice.Maximum = 100;
			sliderVoice.SetBinding(ScrollBar.ValueProperty, new Binding("VoiceVolume")
			{
				Source = GameSettings.GameConfig,
				Mode = BindingMode.TwoWay
			});
			sliderSFX.Minimum = 0;
			sliderSFX.Maximum = 100;
			sliderSFX.SetBinding(ScrollBar.ValueProperty, new Binding("SEVolume")
			{
				Source = GameProfile.Sound,
				Mode = BindingMode.TwoWay
			});

			// Patches
		}

		private void SaveGameConfigIni()
		{
			if (!File.Exists(Path.Combine(App.CurrentGame.gameDirectory, "sonicDX.ini")))
				return;

			IniSerializer.Serialize(GameSettings, Path.Combine(App.CurrentGame.gameDirectory, "sonicDX.ini"));
		}

		private void SaveGameProfile(ref GameSettings settings)
		{
			settings.Graphics = GameProfile.Graphics;
			settings.Controller = GameProfile.Controller;
			settings.Sound = GameProfile.Sound;
			settings.Patches = GameProfile.Patches;
		}
		#endregion

		#region Public Functions
		public void Save(ref GameSettings settings)
		{
			SaveGameProfile(ref settings);
			SaveGameConfigIni();
			SavePatches(ref settings);
		}
		#endregion

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
		}

		private void comboScreen_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			graphics?.screenNumBox_SelectChanged(ref comboScreen, ref comboDisplay);
		}

		private void chkCustomWinSize_Checked(object sender, RoutedEventArgs e)
		{
			chkMaintainRatio.IsEnabled = chkCustomWinSize.IsChecked.GetValueOrDefault();
			chkResizableWin.IsEnabled = !chkCustomWinSize.IsChecked.GetValueOrDefault();

			txtCustomResX.IsEnabled = chkCustomWinSize.IsChecked.GetValueOrDefault() && !chkMaintainRatio.IsChecked.GetValueOrDefault();
			txtCustomResY.IsEnabled = chkCustomWinSize.IsChecked.GetValueOrDefault();
		}

		private void chkRatio_Click(object sender, RoutedEventArgs e)
		{
			if (chkRatio.IsChecked == true)
			{
				txtResX.IsEnabled = false;
				decimal resYDecimal = (decimal)txtResY.Value;
				decimal roundedValue = Math.Round(resYDecimal * Game.Graphics.ratio);
				txtResX.Value = (double)roundedValue;
			}
			else if (!suppressEvent)
			{
				txtResX.IsEnabled = true;
			}
		}

		private void chkBorderless_Checked(object sender, RoutedEventArgs e)
		{
			chkDynamicBuffers.IsChecked = chkBorderless.IsChecked;
		}

		private void comboResolutionPreset_SelectedIndexChanged(object sender, SelectionChangedEventArgs e)
		{
			if (comboDisplay.SelectedIndex == -1)
				return;

			int index = comboDisplay.SelectedIndex;

			suppressEvent = true;
			txtResY.Value = graphics.resolutionPresets[index].Height;

			if (chkRatio.IsChecked == false)
				txtResX.Value = graphics.resolutionPresets[index].Width;

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

		private void SetUp_UpdateD3D9()
		{
			bool isUpdateAvailable = CheckD3D8to9Update();
		
            btnUpdateD3D9.Visibility = isUpdateAvailable ?  Visibility.Visible : Visibility.Hidden;
            btnUpdateD3D9.IsEnabled = !isUpdateAvailable;
            checkD3D9.IsEnabled = File.Exists(d3d8to9StoredDLLName);
			checkD3D9.IsChecked = File.Exists(d3d8to9InstalledDLLName);
		}

		private void CopyD3D9Dll()
		{
			try
			{
				File.Copy(d3d8to9StoredDLLName, d3d8to9InstalledDLLName, true);
			}
			catch (Exception ex)
			{
				string error = Lang.GetString("MessageWindow.Errors.D3D8Update") + "\n" + ex.Message;
				new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle"), error, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
			}
		}

		private bool CheckD3D8to9Update()
		{
			if (!File.Exists(d3d8to9StoredDLLName) || !File.Exists(d3d8to9InstalledDLLName))
				return false;

			try
			{
				long length1 = new FileInfo(d3d8to9InstalledDLLName).Length;
				long length2 = new FileInfo(d3d8to9StoredDLLName).Length;
				if (length1 != length2)
					return true;
				else
				{
					byte[] file1 = File.ReadAllBytes(d3d8to9InstalledDLLName);
					byte[] file2 = File.ReadAllBytes(d3d8to9StoredDLLName);
					for (int i = 0; i < file1.Length; i++)
					{
						if (file1[i] != file2[i])
							return true;
					}
					return false;
				}
			}
			catch (Exception ex)
			{
				string error = Lang.GetString("MessageWindow.Errors.D3D8UpdateCheck") + "\n" + ex.Message;
				new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle"), error, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error).ShowDialog();
				return false;
			}
		}

		private void btnUpdateD3D9_Click(object sender, RoutedEventArgs e)
		{
			string info = Lang.GetString("MessageWindow.Information.D3D8Update");
			var msg = new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle"), Lang.GetString(info), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Information, MessageWindow.Buttons.YesNo);
			msg.ShowDialog();

			if (msg.isYes)
			{
				CopyD3D9Dll();
				btnUpdateD3D9.IsEnabled = CheckD3D8to9Update();
			}
		}

		private void checkD3D9_Click(object sender, RoutedEventArgs e)
		{
			if (checkD3D9.IsChecked == true)
			{
				CopyD3D9Dll();
			}
			else if (checkD3D9.IsChecked == false && File.Exists(d3d8to9InstalledDLLName))
				File.Delete(d3d8to9InstalledDLLName);

		}
		#endregion

		#region Input Tab
		private void InitMouseList()
		{
			List<string> mouseActionList = new()
			{
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Start"),
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Cancel"),
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Jump"),
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Action"),
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Flute"),

			};

			mouseAction.ItemsSource = mouseActionList;

			List<string> mouseBtnAssignList = new()
			{
				Lang.GetString("CommonStrings.None"),
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Group.MouseKeyboard.LeftMouseBtn"),
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Group.MouseKeyboard.RightMouseBtn"),
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Group.MouseKeyboard.MiddleMouseBtn"),
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Group.MouseKeyboard.OtherMouseBtn"),
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Group.MouseKeyboard.LeftRightMouseBtn"),
				Lang.GetString("Manager.Tabs.GameConfig.Tabs.Input.Group.Input.Group.Vanilla.Group.MouseKeyboard.RightLeftMouseBtn"),
			};

			mouseBtnAssign.ItemsSource = mouseBtnAssignList;
		}


		private void radVanillaInput_Checked(object sender, RoutedEventArgs e)
		{
			if (grpVanillaInput is null)
				return;

			int index = tabInputGrid.Children.IndexOf(grpSDLInput); //Graphic Window setting is a children of the graphic grid 

			tabInputGrid.Children.RemoveAt(index); //we remove it so we can only display the full screen options

			if (!tabInputGrid.Children.Contains(grpVanillaInput)) //if the fullscreen grid doesn't exist, add it back
			{
				tabInputGrid.Children.Add(grpVanillaInput);
			}
		}

		private void radBetterInput_Checked(object sender, RoutedEventArgs e)
		{
			if (grpSDLInput is null)
				return;

			int index = tabInputGrid.Children.IndexOf(grpVanillaInput);
			tabInputGrid.Children.RemoveAt(index);

			if (!tabInputGrid.Children.Contains(grpSDLInput))
			{
				tabInputGrid.Children.Add(grpSDLInput);
			}
		}

		#region App Launcher
		private void btnGetAppLauncher_Click(object sender, RoutedEventArgs e)
		{
			Uri uri = new("https://dcmods.unreliable.network/owncloud/data/PiKeyAr/files/Setup/data/AppLauncher.7z" + "\r\n");
			var DL = new GenericDownloadDialog(uri, "App Launcher", "AppLauncher.7z", true);
			DL.StartDL();
			DL.ShowDialog();

			if (DL.DialogResult == true)
			{
				try
				{
					using ArchiveFile archiveFile = new("AppLauncher.7z");
					archiveFile.Extract(Environment.CurrentDirectory);
					btnOpenAppLauncher.IsEnabled = true;
					btnOpenAppLauncher.Opacity = 1;
					btnGetAppLauncher.Opacity = LowOpacityBtn;
					btnGetAppLauncher.IsEnabled = false;
				}
				catch
				{
					throw new Exception("Failed to extract one mod.");
				}
			}

		}

		private void btnOpenAppLauncher_Click(object sender, RoutedEventArgs e)
		{
			if (File.Exists("AppLauncher.exe"))
			{
				Process.Start(new ProcessStartInfo { FileName = "AppLauncher.exe", UseShellExecute = true });
			}
		}

		private void UpdateAppLauncherBtn()
		{
			if (File.Exists("AppLauncher.7z") && File.Exists("AppLauncher.exe"))
			{
				try
				{
					File.Delete("AppLauncher.7z");
				}
				catch
				{ }
			}

			if (File.Exists("AppLauncher.exe"))
			{
				btnGetAppLauncher.IsEnabled = false;
				btnGetAppLauncher.Opacity = LowOpacityBtn;
			}
			else
			{
				btnOpenAppLauncher.IsEnabled = false;
				btnOpenAppLauncher.Opacity = LowOpacityBtn;
			}
		}
		#endregion
		#endregion

		#region Sound Tab
		private void sliderMusic_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
		{
			labelMusicLevel?.SetValue(ContentProperty, $"{(int)sliderMusic.Value}");
		}

		private void sliderVoice_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
		{
			labelVoiceLevel?.SetValue(ContentProperty, $"{(int)sliderVoice.Value}");
		}

		private void sliderSFX_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
		{
			labelSFXLevel?.SetValue(ContentProperty, $"{(int)sliderSFX.Value}");
		}
		private void checkBassSFX_Checked(object sender, RoutedEventArgs e)
		{
			sliderSFX.IsEnabled = true;
		}

		private void checkBassSFX_Unchecked(object sender, RoutedEventArgs e)
		{
			sliderSFX.IsEnabled = false;
		}
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

			PatchDescription.Text += " " + patch.Description;
		}

		private void PatchViewItem_MouseLeave(object sender, MouseEventArgs e)
		{
			PatchDescription.Text = Lang.GetString("CommonStrings.Description");
		}

		private void SavePatches(ref GameSettings settings)
		{
			if (listPatches is null)
				return;

			PatchesData patch = (PatchesData)listPatches.Items[14];

            settings.Patches.DisableCDCheck = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[13];
            settings.Patches.KillGBIX = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[12];
            settings.Patches.LightFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[11];
			settings.Patches.PixelOffSetFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[10];
			settings.Patches.ChaoPanelFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[9];
            settings.Patches.E102NGonFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[8];
            settings.Patches.ChunkSpecularFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[7];
			settings.Patches.Chaos2CrashFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[6];
            settings.Patches.SkyChaseResolutionFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[5];
            settings.Patches.FOVFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[4];
            settings.Patches.InterpolationFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[3];
            settings.Patches.MaterialColorFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[2];
            settings.Patches.FixVertexColorRendering = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[1];
            settings.Patches.KeepCamSettings = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[0];
            settings.Patches.HRTFSound = patch.IsChecked;
		}

        private static List<PatchesData> GetPatches(ref ListView list, GameSettings set)
        {
            list.Items.Clear();

            List<PatchesData> patches = new List<PatchesData>()
            {
                new PatchesData()
                {
                    Name = Lang.GetString("GamePatches.3DSound"),
                    Description = Lang.GetString("GamePatches.3DSoundDesc"),
                    IsChecked = set.Patches.HRTFSound
                },

                new PatchesData()
                {
                    Name = Lang.GetString("GamePatches.CamCode"),
                    Description = Lang.GetString("GamePatches.CamCodeDesc"),
                    IsChecked = set.Patches.KeepCamSettings
                },

                new PatchesData()
                {
                    Name = Lang.GetString("GamePatches.VertexColor"),
                    Description = Lang.GetString("GamePatches.VertexColorDesc"),
                    IsChecked = set.Patches.FixVertexColorRendering
                },

                new PatchesData()
                {
                    Name = Lang.GetString("GamePatches.MaterialColor"),
                    Description = Lang.GetString("GamePatches.MaterialColorDesc"),
                    IsChecked = set.Patches.MaterialColorFix
                },

                new PatchesData()
                {
                    Name = Lang.GetString("GamePatches.Interpolation"),
                    Description = Lang.GetString("GamePatches.InterpolationDesc"),
                    IsChecked = set.Patches.InterpolationFix
                },

                new PatchesData()
                {
                    Name = Lang.GetString("GamePatches.FixFOV"),
                    Description = Lang.GetString("GamePatches.FixFOVDesc"),
                    IsChecked = set.Patches.FOVFix
                },

                new PatchesData()
                {
                    Name = Lang.GetString("GamePatches.Skychase"),
                    Description = Lang.GetString("GamePatches.SkychaseDesc"),
                    IsChecked = set.Patches.SkyChaseResolutionFix
                },

                new PatchesData()
                {
                    Name = Lang.GetString("GamePatches.Chaos2"),
                    Description = Lang.GetString("GamePatches.Chaos2Desc"),
                    IsChecked = set.Patches.Chaos2CrashFix
                },

                new PatchesData()
                {
                    Name = Lang.GetString("GamePatches.ChunkRendering"),
                    Description = Lang.GetString("GamePatches.ChunkRenderingDesc"),
                    IsChecked = set.Patches.ChunkSpecularFix
                },

                new PatchesData()
                {
                    Name = Lang.GetString("GamePatches.E102Lamp"),
                    Description = Lang.GetString("GamePatches.E102LampDesc"),
                    IsChecked = set.Patches.E102NGonFix
                },

                new PatchesData()
                {
                    Name = Lang.GetString("GamePatches.ChaoStats"),
                    Description = Lang.GetString("GamePatches.ChaoStatsDesc"),
                    IsChecked = set.Patches.ChaoPanelFix
                },

                new PatchesData()
                {
                    Name = Lang.GetString("GamePatches.PixelOffset"),
                    Description = Lang.GetString("GamePatches.PixelOffsetDesc"),
                    IsChecked = set.Patches.PixelOffSetFix
                },

                new PatchesData()
                {
                    Name = Lang.GetString("GamePatches.Lights"),
                    Description = Lang.GetString("GamePatches.LightsDesc"),
                    IsChecked = set.Patches.LightFix
                },

                new PatchesData()
                {
                    Name = Lang.GetString("GamePatches.DisableGBIX"),
                    Description = Lang.GetString("GamePatches.DisableGBIXDesc"),
                    IsChecked = set.Patches.KillGBIX
                },

                new PatchesData()
                {
                    Name = Lang.GetString("GamePatches.DisableCDCheck"),
                    Description = Lang.GetString("GamePatches.DisableCDCheckDesc"),
                    IsChecked = set.Patches.DisableCDCheck
                },
            };

            return patches;
        }

        public void SetPatches()
		{	
            listPatches.Items.Clear();

			List<PatchesData> patches = GetPatches(ref listPatches, GameProfile);

			foreach (var patch in patches)
			{
				listPatches.Items.Add(patch);
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
	}
}
