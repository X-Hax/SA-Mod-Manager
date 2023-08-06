using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using SAModManager.Elements;

namespace SAModManager.Common
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
			Message,
			IconCustom,
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
        /// Used to store and get a value for convenience.
        /// </summary>
        public int genericValue { get; set; }

		/// <summary>
		/// Accessible list for setting the entries to be used by the combo box entries.
		/// </summary>
		public List<object> ComboBoxEntries { get; set; }

		public MessageElement MessageElement { get; set; }

		/// <summary>
		/// Constructs a MessageWindow using preset Icons.
		/// </summary>
		/// <param name="windowName"></param>
		/// <param name="ErrorText"></param>
		/// <param name="icon"></param>
		/// <param name="button"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		public MessageWindow(string windowName, string ErrorText, 
							WindowType type = WindowType.IconMessage, Icons icon = Icons.Caution, Buttons button = Buttons.OK, 
							double width = 40, double height = 40, string headerText = "", UIElement customElement = null)
		{
			InitializeComponent();

			DrawingImage drawing = GetIcon(icon);
			Image image = new()
			{
				Source = drawing,
				Width = width,
				Height = height
			};

			InitializeMessageWindow(windowName, ErrorText, headerText, image, button, type, customElement);
		}

		/// <summary>
		/// Constructs a MessageWindow with a custom image.
		/// </summary>
		/// <param name="windowName"></param>
		/// <param name="ErrorText"></param>
		/// <param name="image"></param>
		/// <param name="button"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		public MessageWindow(string windowName, string messageText, Image image, 
							WindowType type = WindowType.IconMessage, Buttons button = Buttons.OK, 
							double width = 40, double height = 40, string headerText = "", UIElement customElement = null)
		{
			InitializeComponent();

			image.Width = width;
			image.Height = height;

			InitializeMessageWindow(windowName, messageText, headerText, image, button, type, customElement);
		}

		#region Message 
		private DrawingImage GetIcon(Icons icon)
		{
			switch (icon)
			{
				case Icons.Warning:
					return (DrawingImage)TryFindResource("Icon.Warning");
				case Icons.Information:
					return (DrawingImage)TryFindResource("IconInfo");
				case Icons.Caution:
					return (DrawingImage)TryFindResource("IconCaution");
				case Icons.Error:
					return (DrawingImage)TryFindResource("Icon.Error");

			}

			return new DrawingImage();
		}

		private void AddMessageElement(WindowType type, string message = "", Image icon = null, string header = "", UIElement customElement = null)
		{
			bool hasHeader = false;
			bool hasIcon = false;
			bool hasCombo = false;

			switch (type)
			{
				case WindowType.IconMessage:
					hasIcon = true;
					break;
				case WindowType.IconHeader:
					hasHeader = true;
					hasIcon = true;
					break;
				case WindowType.IconCustom:
					hasIcon = true;
					hasCombo = true;
					break;
			}

			MessageElement = new MessageElement(message, icon: icon, header: header,
												hasIcon: hasIcon, hasHeader: hasHeader, hasCustom: hasCombo,
												customElement: customElement);
			MessageGrid.Children.Add(MessageElement);
		}

		private void InitializeMessageWindow(string windowName, string messageText, string headerText, Image icon, Buttons button, WindowType type, UIElement customElement)
		{
			isClosed = false;
			Accepted = false;
			Window.Title = windowName;
			Image image = (Image)TryFindResource("MessageIcon");
			image.Source = icon.Source;
			image.Width = icon.Width;
			image.Height = icon.Height;
			AddMessageElement(type, message: messageText, icon: icon, header: headerText, customElement: customElement);
			UpdateButtons(button);
		}
		#endregion

		#region Window Functions
		private void UpdateButtons(Buttons buttons)
		{
			switch (buttons)
			{
				case Buttons.OK:
					ButtonLeft.Visibility = Visibility.Hidden;
					ButtonRight.Content = Lang.GetString("CommonStrings.OK");
					ButtonRight.Click += ButtonYes_Click;
					break;
				case Buttons.OKCancel:
					ButtonLeft.Content = Lang.GetString("CommonStrings.OK");
					ButtonLeft.Click += ButtonYes_Click;
					ButtonRight.Content = Lang.GetString("CommonStrings.Cancel");
					ButtonRight.Click += ButtonClick;
					break;
				case Buttons.YesNo:
					ButtonLeft.Content = Lang.GetString("CommonStrings.Yes");
					ButtonLeft.Click += ButtonYes_Click;
					ButtonRight.Content = Lang.GetString("CommonStrings.No");
					ButtonRight.Click += ButtonClick;
					break;
				case Buttons.RetryCancel:
					ButtonLeft.Content = Lang.GetString("CommonStrings.Retry");
					ButtonLeft.Click += ButtonYes_Click;
					ButtonRight.Content = Lang.GetString("CommonStrings.Cancel");
					ButtonRight.Click += ButtonClick;
					break;
			}
		}

		private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			isClosed = true;
		}

		private void Window_Loaded(object sender, RoutedEventArgs e)
		{
			Window.SizeToContent = SizeToContent.WidthAndHeight;
		}
		#endregion

		#region Button Functions
		private void ButtonClick(object sender, RoutedEventArgs e)
		{
			this.Close();
		}

		private void ButtonYes_Click(object sender, RoutedEventArgs e)
		{
			Accepted = true;
			this.Close();
		}
		#endregion
	}
}