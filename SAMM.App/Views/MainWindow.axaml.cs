using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using SAMM.App.Models;
using System.Threading.Tasks;

namespace SAMM.App.Views;

public partial class MainWindow : Window
{
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
}