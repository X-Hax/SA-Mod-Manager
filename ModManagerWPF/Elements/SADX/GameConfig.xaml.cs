using SAModManager.Common;
using SAModManager.UI;
using SAModManager.Updater;
using SevenZipExtractor;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace SAModManager.Elements.SADX
{
	/// <summary>
	/// Interaction logic for GameConfig.xaml
	/// </summary>
	public partial class GameConfig : UserControl
	{
		#region Variables

		private string sadxIni = "sonicDX.ini";

		string gamePath = string.Empty;
		SADXLoaderInfo loaderini;

		private string d3d8to9InstalledDLLName = "d3d8.dll";
		private string d3d8to9StoredDLLName = "d3d8m.dll";
		bool suppressEvent = false;

		private readonly double LowOpacityBtn = 0.7;


		private Game.GameConfigFile gameConfigFile;

		public Game.Graphics graphics;
		#endregion

		public GameConfig(SADXLoaderInfo loaderini, string gamePath)
		{
			InitializeComponent();
			this.loaderini = loaderini;
			this.gamePath = gamePath;
			graphics = new Game.Graphics(ref comboScreen);

			Loaded += GameConfig_Loaded;
		}

		private void GameConfig_Loaded(object sender, RoutedEventArgs e)
		{
			SetupBindings();
		}

		#region Private Functions
		private bool SetupBindings()
		{
			try
			{
				// Graphics Tab Bindings
				// Screen Settings
				comboScreen.SetBinding(ComboBox.SelectedIndexProperty, new Binding("ScreenNum")
				{
					Source = loaderini,
					Mode = BindingMode.TwoWay,
				});
				txtResX.SetBinding(NumericUpDown.ValueProperty, new Binding("HorizontalResolution")
				{
					Source = loaderini,
					Mode = BindingMode.TwoWay
				});
				txtResY.SetBinding(NumericUpDown.ValueProperty, new Binding("VerticalResolution")
				{
					Source = loaderini,
					Mode = BindingMode.TwoWay
				});

				// Window Settings
				chkRatio.SetBinding(CheckBox.IsCheckedProperty, new Binding("ForceAspectRatio")
				{
					Source = loaderini,
					Mode = BindingMode.TwoWay
				});
				chkVSync.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableVsync")
				{
					Source = loaderini,
					Mode = BindingMode.TwoWay
				});
				chkPause.SetBinding(CheckBox.IsCheckedProperty, new Binding("PauseWhenInactive")
				{
					Source = loaderini,
					Mode = BindingMode.TwoWay
				});
				chkDynamicBuffers.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableDynamicBuffer")
				{
					Source = loaderini,
					Mode = BindingMode.TwoWay
				});
				// Radio Button Handles
				chkBorderless.SetBinding(CheckBox.IsCheckedProperty, new Binding("Borderless")
				{
					Source = loaderini,
					Mode = BindingMode.TwoWay
				});
				chkScaleScreen.SetBinding(CheckBox.IsCheckedProperty, new Binding("StretchFullscreen")
				{
					Source = loaderini,
					Mode = BindingMode.TwoWay
				});
				chkResizableWin.SetBinding(CheckBox.IsCheckedProperty, new Binding("ResizableWindow")
				{
					Source = loaderini,
					Mode = BindingMode.TwoWay
				});
				chkCustomWinSize.SetBinding(CheckBox.IsCheckedProperty, new Binding("CustomWindowSize")
				{
					Source = loaderini,
					Mode = BindingMode.TwoWay
				});
				chkMaintainRatio.SetBinding(CheckBox.IsCheckedProperty, new Binding("MaintainWindowAspectRatio")
				{
					Source = loaderini,
					Mode = BindingMode.TwoWay
				});
				System.Drawing.Rectangle rect = graphics.GetRectangleStruct();
				txtCustomResX.MinValue = 0;
				txtCustomResY.MinValue = 0;
				txtCustomResX.MaxValue = rect.Width;
				txtCustomResY.MaxValue = rect.Height;
				txtCustomResX.SetBinding(NumericUpDown.ValueProperty, new Binding("WindowWidth")
				{
					Source = loaderini,
					Mode = BindingMode.TwoWay
				});
				txtCustomResY.SetBinding(NumericUpDown.ValueProperty, new Binding("WindowHeight")
				{
					Source = loaderini,
					Mode = BindingMode.TwoWay
				});

				// Game Config Settings


				// Enhancement Settings


				// Input Settings


				// Audio Settings


				// Patches


				return true;
			}
			catch { return false; }
		}
		#endregion

		#region Public Functions
		public void SaveGameSettings(SADXLoaderInfo managerSettings)
		{
			managerSettings.ScreenNum = loaderini.ScreenNum;
			managerSettings.HorizontalResolution = loaderini.HorizontalResolution;
			managerSettings.VerticalResolution = loaderini.VerticalResolution;

			managerSettings.ForceAspectRatio = loaderini.ForceAspectRatio;
			managerSettings.EnableVsync = loaderini.EnableVsync;
			managerSettings.PauseWhenInactive = loaderini.PauseWhenInactive;
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
			btnUpdateD3D9.IsEnabled = CheckD3D8to9Update();
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

		private void SavePatches()
		{
			if (listPatches is null)
				return;


			PatchesData patch = (PatchesData)listPatches.Items[14];
			loaderini.DisableCDCheck = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[13];
			loaderini.KillGbix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[12];
			loaderini.LightFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[11];
			loaderini.PixelOffSetFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[10];
			loaderini.ChaoPanelFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[9];
			loaderini.E102PolyFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[8];
			loaderini.ChunkSpecFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[7];
			loaderini.Chaos2CrashFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[6];
			loaderini.SCFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[5];
			loaderini.FovFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[4];
			loaderini.InterpolationFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[3];
			loaderini.MaterialColorFix = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[2];
			loaderini.PolyBuff = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[1];
			loaderini.CCEF = patch.IsChecked;
			patch = (PatchesData)listPatches.Items[0];
			loaderini.HRTFSound = patch.IsChecked;
		}

		private void UpdatePatches()
		{
			listPatches.Items.Clear();

			if (loaderini is null)
				return;

			List<PatchesData> patches = new List<PatchesData>()
			{
				new PatchesData() { Name = Lang.GetString("GamePatches.3DSound"),
					Description = Lang.GetString("GamePatches.3DSoundDesc"), IsChecked = loaderini.HRTFSound },
				new PatchesData() { Name = Lang.GetString("GamePatches.CamCode"),
					Description = Lang.GetString("GamePatches.CamCodeDesc"),IsChecked = loaderini.CCEF },
				new PatchesData() { Name = Lang.GetString("GamePatches.VertexColor"),
					Description = Lang.GetString("GamePatches.VertexColorDesc"),IsChecked = loaderini.PolyBuff },
				new PatchesData() { Name = Lang.GetString("GamePatches.MaterialColor"),
					Description = Lang.GetString("GamePatches.MaterialColorDesc"),IsChecked = loaderini.MaterialColorFix},
				new PatchesData() { Name = Lang.GetString("GamePatches.Interpolation"),
					Description = Lang.GetString("GamePatches.InterpolationDesc"),IsChecked = loaderini.InterpolationFix},
				new PatchesData() { Name = Lang.GetString("GamePatches.FixFOV"),
					Description = Lang.GetString("GamePatches.FixFOVDesc"),IsChecked = loaderini.FovFix },
				new PatchesData() { Name = Lang.GetString("GamePatches.Skychase"),
					Description = Lang.GetString("GamePatches.SkychaseDesc"),IsChecked = loaderini.SCFix },
				new PatchesData() { Name = Lang.GetString("GamePatches.Chaos2"),
					Description = Lang.GetString("GamePatches.Chaos2Desc"),IsChecked = loaderini.Chaos2CrashFix },
				new PatchesData() { Name = Lang.GetString("GamePatches.ChunkRendering"),
					Description = Lang.GetString("GamePatches.ChunkRenderingDesc"),IsChecked = loaderini.ChunkSpecFix},
				new PatchesData() { Name = Lang.GetString("GamePatches.E102Lamp"),
					Description = Lang.GetString("GamePatches.E102LampDesc"),IsChecked = loaderini.E102PolyFix},
				new PatchesData() { Name = Lang.GetString("GamePatches.ChaoStats"), IsChecked = loaderini.ChaoPanelFix},
				new PatchesData() { Name = Lang.GetString("GamePatches.PixelOffset"),
					Description = Lang.GetString("GamePatches.PixelOffsetDesc"),IsChecked = loaderini.PixelOffSetFix},
				new PatchesData() { Name = Lang.GetString("GamePatches.Lights"),
					Description = Lang.GetString("GamePatches.LightsDesc"),IsChecked = loaderini.LightFix},
				new PatchesData() { Name = Lang.GetString("GamePatches.DisableGBIX"),
					Description = Lang.GetString("GamePatches.DisableGBIXDesc"),IsChecked = loaderini.KillGbix},
				new PatchesData() { Name = Lang.GetString("GamePatches.DisableCDCheck"), IsChecked = loaderini.DisableCDCheck},
			};

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
