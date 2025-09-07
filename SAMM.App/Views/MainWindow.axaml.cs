using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using SAMM.App.Models;

namespace SAMM.App.Views;

public partial class MainWindow : Window
{
    public MainWindow()
    {
        InitializeComponent();

        DataContext = new MainWindowViewModel();
    }

	private void DataGridTemplateColumn_HeaderPointerPressed(object? sender, Avalonia.Input.PointerPressedEventArgs e)
	{
		ModList.CollectionView.SortDescriptions.Clear();
	}
}