using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using SAMM.App.Models.Dialogs;

namespace SAMM.App.Controls.Dialogs;

public partial class TextInputDialog : UserControl
{
    public string OutputText { get => (DataContext as TextInputDialogModel).TextInput; }

    public string HostDialog { get; set; } = string.Empty;

    public bool IsOK = false;

    public TextInputDialog()
    {
        InitializeComponent();
    }

    public TextInputDialog(string text, string hostDialog = "")
    {
        InitializeComponent();
        DataContext = new TextInputDialogModel(text);
        HostDialog = hostDialog;
    }

	private void OK_Click(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
	{
        IsOK = true;
        if (HostDialog.Length > 0)
            DialogHostAvalonia.DialogHost.Close(HostDialog);
	}

    private void Cancel_Click(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
    {
		if (HostDialog.Length > 0)
			DialogHostAvalonia.DialogHost.Close(HostDialog);
	}
}