using System;
using System.Collections.Generic;
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
using System.Windows.Shapes;

namespace ModManagerWPF.Common
{
	/// <summary>
	/// Interaction logic for MessageWindow.xaml
	/// </summary>
	public partial class MessageWindow : Window
	{
		/// <summary>
		/// Prebuilt Icons that are supported with the MessageWindow.
		/// </summary>
		public enum Icons
		{
			Warning,
			Information,
			Caution,
			Error,
		}

		/// <summary>
		/// Button options provided by the window to the user.
		/// </summary>
		public enum Buttons
		{
			OK,
			OKCancel,
			YesNo,
		}

		/// <summary>
		/// Returns true when MessageWindow is closed. Can be used to check when the window has been closed.
		/// </summary>
		public bool isClosed;

		/// <summary>
		/// Returns true when OK or Yes buttons are pressed. Can be used to check when those buttons have been pressed.
		/// </summary>
		public bool isYes;

		/// <summary>
		/// Constructs a MessageWindow using preset Icons.
		/// </summary>
		/// <param name="windowName"></param>
		/// <param name="errorText"></param>
		/// <param name="icon"></param>
		/// <param name="button"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		public MessageWindow(string windowName, string errorText, Icons icon=Icons.Caution, Buttons button=Buttons.OK, double width=40, double height=40)
		{
			InitializeComponent();

			DrawingImage drawing = GetIcon(icon);
			Image image = new Image();
			image.Source = drawing;
			image.Width = width;
			image.Height = height;

			InitializeMessageWindow(windowName, errorText, image, button);
		}

		/// <summary>
		/// Constructs a MessageWindow with a custom image.
		/// </summary>
		/// <param name="windowName"></param>
		/// <param name="errorText"></param>
		/// <param name="image"></param>
		/// <param name="button"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		public MessageWindow(string windowName, string errorText, Image image, Buttons button=Buttons.OK, double width=40, double height=40)
		{
			InitializeComponent();

			image.Width = width;
			image.Height = height;

			InitializeMessageWindow(windowName, errorText, image, button);
		}

		private void InitializeMessageWindow(string windowName, string errorText, Image icon, Buttons button)
		{
			isClosed = false;
			isYes = false;
			Window.Title = windowName;
			MessageIcon.Source = icon.Source;
			MessageIcon.Width = icon.Width;
			MessageIcon.Height = icon.Height;
			MessageText.Text = errorText;
			UpdateButtons(button);
		}

		private DrawingImage GetIcon(Icons icon)
		{
			switch (icon)
			{
				case Icons.Warning:
					return (DrawingImage)TryFindResource("IconWarning");
				case Icons.Information:
					return (DrawingImage)TryFindResource("IconInfo");
				case Icons.Caution:
					return (DrawingImage)TryFindResource("IconCaution");
				case Icons.Error:
					return (DrawingImage)TryFindResource("IconError");

			}

			return new DrawingImage();
		}

		private void UpdateButtons(Buttons buttons)
		{
			switch (buttons)
			{
				case Buttons.OK:
					ButtonsGrid.RowDefinitions[0].Height = new GridLength(1, GridUnitType.Auto);
					ButtonsGrid.RowDefinitions[1].Height = new GridLength(0);
					ButtonsGrid.RowDefinitions[2].Height = new GridLength(0);
					break;
				case Buttons.OKCancel:
					ButtonsGrid.RowDefinitions[0].Height = new GridLength(0);
					ButtonsGrid.RowDefinitions[1].Height = new GridLength(1, GridUnitType.Auto);
					ButtonsGrid.RowDefinitions[2].Height = new GridLength(0);
					break;
				case Buttons.YesNo:
					ButtonsGrid.RowDefinitions[0].Height = new GridLength(0);
					ButtonsGrid.RowDefinitions[1].Height = new GridLength(0);
					ButtonsGrid.RowDefinitions[2].Height = new GridLength(1, GridUnitType.Auto);
					break;
			}
		}

		private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			isClosed = true;
		}

		private void ButtonClick(object sender, RoutedEventArgs e)
		{
			this.Close();
		}

		private void ButtonYes_Click(object sender, RoutedEventArgs e)
		{
			isYes = true;
			this.Close();
		}

		private void LoopMedia(object sender, RoutedEventArgs e)
		{
			MediaElement gif = (MediaElement)TryFindResource("GifTest");
			gif.Position = new TimeSpan(0, 0, 1);
			gif.Play();
		}
	}
}
