using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Shapes;

namespace SAModManager.Elements
{
	/// <summary>
	/// Interaction logic for MessageIconHeader.xaml
	/// </summary>
	public partial class MessageElement : UserControl
	{
		private Image IconData = new()
		{
			Margin = new Thickness(5),
			VerticalAlignment = VerticalAlignment.Top,
		};

		private TextBlock MessageText = new()
		{
			Margin = new Thickness(5),
			FontSize = 14,
		};

		private TextBlock HeaderText = new()
		{
			Margin = new Thickness(5),
			FontSize = 20
		};

		private UIElement CustomElement;

		private bool HasHeader = false;
		private bool HasIcon = false;
		private bool HasCustom = false;

		public MessageElement(string message, Image icon = null, string header = "",
			bool hasIcon = false, bool hasHeader = false, bool hasCustom = false,
			UIElement customElement = null)
		{
			InitializeComponent();
			IconData = icon;
			MessageText.Text = message;
			HeaderText.Text = header;

			HasHeader = hasHeader;
			HasCustom = hasCustom;
			HasIcon = hasIcon;

			CustomElement = customElement;

			Loaded += MessageElement_Loaded;
		}

		// Add any custom UIElement checks here for each type when updating.
		private void CheckCustomElement()
		{
			if (CustomElement != null)
			{
				if (CustomElement is ComboBox)
				{
					ComboBox element = CustomElement as ComboBox;
					element.SelectedIndex = 0;
				}
			}
		}

		private void AddHeader()
		{
			GridMessage.Children.Add(HeaderText);
		}

		private void AddIcon()
		{
			StackPanel iconPanel = new StackPanel();
			{
				iconPanel.Children.Add(IconData);
				iconPanel.Children.Add(new Rectangle()
				{
					Width = 5,
				});
			}
			iconPanel.Orientation = Orientation.Horizontal;
			GridMessage.Children.Add(iconPanel);

			if (!HasHeader)
				Grid.SetRow(iconPanel, 1);
		}

		private void AddCustomElement()
		{
			if (CustomElement != null)
			{
				GridMessage.Children.Add(CustomElement);
				Grid.SetColumn(CustomElement, 1);
				Grid.SetRow(CustomElement, 2);
			}
		}

		private void MessageElement_Loaded(object sender, RoutedEventArgs e)
		{
			ScrollViewer scrollViewer = new ScrollViewer();
			scrollViewer.Content = MessageText;
			GridMessage.Children.Add(scrollViewer);
			Grid.SetColumn(scrollViewer, 1);
			Grid.SetRow(scrollViewer, 1);

			if (HasIcon)
				AddIcon();

			if (HasHeader)
				AddHeader();

			if (HasCustom)
			{
				AddCustomElement();
				CheckCustomElement();
			}
		}
	}
}
