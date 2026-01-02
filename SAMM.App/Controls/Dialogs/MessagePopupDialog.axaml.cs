using Avalonia.Controls;
using DialogHostAvalonia;

namespace SAMM.App.Controls.Dialogs;

public partial class MessagePopupDialog : UserControl
{
    public MessagePopupDialog()
    {
        InitializeComponent();
    }

    public MessagePopupDialog(string header, string message)
    {
        InitializeComponent();

        MessageHeader.Text = header;
        Message.Text = message;
    }

	private void LeftButton_Click(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
	{
        DialogHost.Close("MainWindowDialogHost");
	}

	private void RightButton_Click(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
	{
		DialogHost.Close("MainWindowDialogHost");
	}
}