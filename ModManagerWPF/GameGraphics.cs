using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Forms;
using Size = System.Drawing.Size;

namespace ModManagerWPF
{
    public class GameGraphics
    {
		public GameGraphics(System.Windows.Controls.ComboBox screenNumComboBox) 
		{
			SetAllScreens(screenNumComboBox);
		}

		public readonly Size[] resolutionPresets =
{
			new Size(640, 480), // 640x480
			new Size(800, 600), // 800x600
			new Size(1024, 768), // 1024x768
			new Size(1152, 864), // 1152x864
			new Size(1280, 960), // 1280x960
			new Size(1280, 1024), // 1280x1024
			new Size(), // Native
			new Size(), // 1/2x Native
			new Size(), // 2x Native
			new Size(1280, 720), // 720p
			new Size(1920, 1080), // 1080p
			new Size(3840, 2160), // 4K
		};

		public void SetAllScreens(System.Windows.Controls.ComboBox screenNumComboBox)
		{
			for (int i = 0; i < Screen.AllScreens.Length; i++)
			{
				Screen s = Screen.AllScreens[i];
				screenNumComboBox.Items.Add($"{i + 1} {s.DeviceName} ({s.Bounds.Location.X},{s.Bounds.Y})"
					+ $" {s.Bounds.Width}x{s.Bounds.Height} {s.BitsPerPixel} bpp {(s.Primary ? "Primary" : "")}");
			}
		}

		public void screenNumBox_SelectChanged(System.Windows.Controls.ComboBox screenNumComboBox, System.Windows.Controls.ComboBox comboResolutionPreset)
		{
			Size oldsize = resolutionPresets[6];
			Rectangle rect = Screen.PrimaryScreen.Bounds;
			if (screenNumComboBox.SelectedIndex > 0)
				rect = Screen.AllScreens[screenNumComboBox.SelectedIndex - 1].Bounds;
			else
				foreach (Screen screen in Screen.AllScreens)
					rect = Rectangle.Union(rect, screen.Bounds);

			resolutionPresets[6] = rect.Size;
			resolutionPresets[7] = new Size(rect.Width / 2, rect.Height / 2);
			resolutionPresets[8] = new Size(rect.Width * 2, rect.Height * 2);

			if (comboResolutionPreset.SelectedIndex > 4 && comboResolutionPreset.SelectedIndex < 8 && rect.Size != oldsize)
				comboResolutionPreset.SelectedIndex = -1;
		}
	}


}
