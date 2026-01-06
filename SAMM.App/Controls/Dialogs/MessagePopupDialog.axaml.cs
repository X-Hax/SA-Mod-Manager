using Avalonia.Controls;
using DialogHostAvalonia;

namespace SAMM.App.Controls.Dialogs;

public partial class MessagePopupDialog : UserControl
{
    private bool returnBool = false;

    public bool IsOK { get { return returnBool; } }

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
        returnBool = true;
        DialogHost.Close("MainWindowDialogHost");
	}

	private void RightButton_Click(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
	{
        returnBool = false;
		DialogHost.Close("MainWindowDialogHost");
	}
}