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
			RetryCancel,
		}

		/// <summary>
		/// Window options to if the header has a header, icon, or is just a message.
		/// </summary>
		public enum WindowType
		{
			IconHeader,
			IconMessage,
			Message
		}

		private static bool Accepted { get; set; }

		/// <summary>
		/// Returns true when MessageWindow is closed. Can be used to check when the window has been closed.
		/// </summary>
		public bool isClosed;

		/// <summary>
		/// Returns true Yes has been pressed.
		/// </summary>
		public bool isYes { get { return Accepted; } }

		/// <summary>
		/// Returns true when Retry has been pressed.
		/// </summary>
		public bool isRetry { get { return Accepted; } }

		/// <summary>
		/// Returns true when OK has been pressed.
		/// </summary>
		public bool isOK { get { return Accepted; } }
		

		/// <summary>
		/// Constructs a MessageWindow using preset Icons.
		/// </summary>
		/// <param name="windowName"></param>
		/// <param name="errorText"></param>
		/// <param name="icon"></param>
		/// <param name="button"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		public MessageWindow(string windowName, string errorText, WindowType type=WindowType.IconMessage, Icons icon=Icons.Caution, Buttons button=Buttons.OK, double width=40, double height=40, string headerText="")
		{
			InitializeComponent();

			DrawingImage drawing = GetIcon(icon);
			Image image = new Image();
			image.Source = drawing;
			image.Width = width;
			image.Height = height;

			InitializeMessageWindow(windowName, errorText, headerText, image, button, type);
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
		public MessageWindow(string windowName, string messageText, Image image, WindowType type = WindowType.IconMessage, Buttons button=Buttons.OK, double width=40, double height=40, string headerText="")
		{
			InitializeComponent();

			image.Width = width;
			image.Height = height;

			InitializeMessageWindow(windowName, messageText, headerText, image, button, type);
		}

		private void InitializeMessageWindow(string windowName, string messageText, string headerText, Image icon, Buttons button, WindowType type)
		{
			isClosed = false;
			Accepted = false;
			Window.Title = windowName;
			Image image = (Image)TryFindResource("MessageIcon");
			image.Source = icon.Source;
			image.Width = icon.Width;
			image.Height = icon.Height;
			SetupWindow(messageText, headerText, type, image);
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
					ButtonLeft.Visibility = Visibility.Hidden;
					ButtonRight.Content = Lang.GetString("BtnOK");
					break;
				case Buttons.OKCancel:
					ButtonLeft.Content = Lang.GetString("BtnOK");
					ButtonRight.Content = Lang.GetString("BtnCancel");
					break;
				case Buttons.YesNo:
					ButtonLeft.Content = Lang.GetString("BtnYes");
					ButtonRight.Content = Lang.GetString("BtnNo");
					break;
				case Buttons.RetryCancel:
					ButtonLeft.Content = Lang.GetString("BtnRetry");
					ButtonRight.Content = Lang.GetString("BtnCancel");
					break;
			}
		}

		private void SetupWindow(string messageText, string headerText, WindowType windowType, Image image)
		{
			switch (windowType)
			{
				case WindowType.IconMessage:
					MasterGrid.RowDefinitions[0].Height = new GridLength(1, GridUnitType.Star);
					MasterGrid.RowDefinitions[1].Height = new GridLength(0);
					MasterGrid.RowDefinitions[2].Height = new GridLength(0);
					GridIconMessage.Children.Add(image);
					IconMessage.Text = messageText;
					break;
				case WindowType.IconHeader:
					MasterGrid.RowDefinitions[0].Height = new GridLength(0);
					MasterGrid.RowDefinitions[1].Height = new GridLength(1, GridUnitType.Star);
					MasterGrid.RowDefinitions[2].Height = new GridLength(0);
					GridIconHeader.Children.Add(image);
					HeaderHeader.Text = headerText;
					HeaderMessage.Text = messageText;
					break;
				case WindowType.Message:
					MasterGrid.RowDefinitions[0].Height = new GridLength(0);
					MasterGrid.RowDefinitions[1].Height = new GridLength(0);
					MasterGrid.RowDefinitions[2].Height = new GridLength(1, GridUnitType.Star);
					MessageMessage.Text = messageText;
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
			Accepted = true;
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
