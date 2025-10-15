using Avalonia;
using Avalonia.Controls;
using DialogHostAvalonia;
using SAMM.App.Models.Dialogs;
using System;

namespace SAMM.App.Controls.Dialogs;

public partial class ExceptionHandlerDialog : UserControl
{
    public ExceptionHandlerDialog()
    {
        InitializeComponent();
    }

    public ExceptionHandlerDialog(Exception report)
    {
        InitializeComponent();

        DataContext = new ExceptionHandlerModel(report);

	}

	private void SendReportButton(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
	{

	}

	private void CopyReportButton(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
	{
		
	}

	private void CloseButton(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
	{
		DialogHost.Close("MainWindowDialogHost");
	}
}