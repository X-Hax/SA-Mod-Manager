using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using SAMM.Configuration.Mods;
using SAMM.App.Models.Dialogs;

namespace SAMM.App.Controls.Dialogs;

public partial class ModConfigDialog : UserControl
{
    private ModConfigDialogModel modConfig { get { return (ModConfigDialogModel)DataContext; } }

    public ModConfigDialog(ModEntry entry)
    {
        InitializeComponent();

        DataContext = new ModConfigDialogModel(entry);
    }

	private void SaveButton(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
	{
        modConfig.WriteConfigFile();
	}

    private void ResetButton(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
    {
        modConfig.ResetConfig();
    }
}