using System;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using SAMM.Configuration.Mods;
using SAMM.App.Models.Dialogs;
using DialogHostAvalonia;

namespace SAMM.App.Controls.Dialogs;

public partial class ModConfigDialog : UserControl
{
    private ModConfigDialogModel modConfig { get { return (ModConfigDialogModel)DataContext; } }

    public ModConfigDialog(ModEntry entry)
    {
        InitializeComponent();

        try
        {
			DataContext = new ModConfigDialogModel(entry);
		}
        catch (Exception ex)
        {
            PART_Root.Child = new ExceptionHandlerDialog(ex);
        }
    }

	private void SaveButton(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
	{
        modConfig.WriteConfigFile();
	}

    private void ResetButton(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
    {
        modConfig.ResetConfig();
    }

	private void CloseButton(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
	{
        DialogHost.Close("MainWindowDialogHost");
	}
}