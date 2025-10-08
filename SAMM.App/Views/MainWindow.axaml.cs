using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using DialogHostAvalonia;
using SAMM.App.Controls.Dialogs;
using SAMM.App.Models;
using SAMM.Configuration.Mods;
using System.Threading.Tasks;

namespace SAMM.App.Views;

public partial class MainWindow : Window
{
	private ModEntry selectedMod 
	{
		get
		{
			if (ModList != null)
				return (ModEntry)ModList.SelectedItem;
			else
				return null;
		}
	}

    public MainWindow()
    {
        InitializeComponent();

        DataContext = new MainWindowViewModel();
    }

	private MainWindowViewModel ViewModel { get { return (MainWindowViewModel)DataContext; } }

	private void DataGridTemplateColumn_HeaderPointerPressed(object? sender, Avalonia.Input.PointerPressedEventArgs e)
	{
		ModList.CollectionView.SortDescriptions.Clear();
	}

	private void GameSelectionChanged(object? sender, Avalonia.Controls.SelectionChangedEventArgs e)
	{
		ViewModel.LoadGameConfig(true);
	}

	private void OpenModConfigDialog(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
	{
		ModConfigDialog config = new ModConfigDialog(selectedMod);
		PART_DialogHost.Child = config;
		MainWindowDialogHost.IsOpen = true;
	}
}