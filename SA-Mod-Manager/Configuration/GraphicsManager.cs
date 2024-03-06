using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SAModManager.Configuration
{
	public static class GraphicsManager
	{
		public enum Resolutions
		{
			r640x480,
			r800x600,
			r1024x768,
			r1280x960,
			r1280x1024,
			rNative,
			rHalfNative,
			rDoubleNative,
			r1280x720,
			r1920x1080,
			r3840x2160
		}

		public enum Ratio
		{
			ratio43,
			ratio169
		}

		#region Variables
		public static readonly Size[] ResolutionPresets =
{
			new (640, 480), // 640x480
			new (800, 600), // 800x600
			new (1024, 768), // 1024x768
			new (1152, 864), // 1152x864
			new (1280, 960), // 1280x960
			new (1280, 1024), // 1280x1024
			new (), // Native
			new (), // 1/2x Native
			new (), // 2x Native
			new (1280, 720), // 720p
			new (1920, 1080), // 1080p
			new (3840, 2160), // 4K
		};

		public static Dictionary<string, Screen> Screens = new Dictionary<string, Screen>();
		#endregion

		#region Functions
		#region Private Functions
		private static Decimal Get43Ratio()
		{
			return (4m / 3m);
		}

		private static Decimal Get169Ratio()
		{
			return (16m / 9m);
		}

		private static Rectangle GetAllScreenBounds()
		{
			Rectangle bounds = new Rectangle();

			foreach (Screen screen in Screen.AllScreens)
				bounds = Rectangle.Union(bounds, screen.Bounds);

			return bounds;
		}

		private static Screen GetScreen(int screen)
		{
			return Screens.ElementAt(screen).Value;
		}
		#endregion

		#region Public Functions
		public static void SetupGraphics()
		{
			Screens.Add("All Screens", null);

			int index = 1;
			foreach (var screen in Screen.AllScreens)
			{
				Screens.Add(
					$"Screen {index} " +
					$"({screen.Bounds.Width}x{screen.Bounds.Height}) " + $"{(screen.Primary ? "[Primary]" : string.Empty)}",
					screen);

				index++;
			}
		}

		public static void UpdateResolutionPresets(int index)
		{
			Rectangle rect = GetDisplayBounds(index);

			ResolutionPresets[6] = new Size(rect.Width, rect.Height);
			ResolutionPresets[7] = new Size(rect.Width / 2, rect.Height / 2);
			ResolutionPresets[8] = new Size(rect.Width * 2, rect.Height * 2);
		}

		public static Decimal GetRatio(Ratio ratio)
		{
			switch (ratio)
			{
				case Ratio.ratio43:
				default:
					return Get43Ratio();
				case Ratio.ratio169:
					return Get169Ratio();
			}
		}

		public static Rectangle GetDisplayBounds(int index)
		{
			if (index > 0)
				return GetScreen(index).Bounds;
			else
				return GetAllScreenBounds();
		}
		#endregion
		#endregion
	}
}
