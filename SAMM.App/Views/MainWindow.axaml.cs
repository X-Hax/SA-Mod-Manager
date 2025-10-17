using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using DialogHostAvalonia;
using SAMM.App.Controls.Dialogs;
using SAMM.App.Models;
using SAMM.Configuration.Mods;
using SAMM.Utilities;
using System;
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

	public void ShowDialog(Control? control)
	{
		PART_DialogHost.Child = control;
		MainWindowDialogHost.IsOpen = true;
	}

	public void ShowException(Exception ex)
	{
		PART_DialogHost.Child = new ExceptionHandlerDialog(ex);
		MainWindowDialogHost.IsOpen = true;
	}

	public MainWindow()
	{
		InitializeComponent();

		DataContext = new MainWindowViewModel();
	}

    public MainWindow(string[]? args)
    {
        InitializeComponent();

		ExceptionHandler.Initialize(ShowException);

        DataContext = new MainWindowViewModel();

		if (args != null)
			RunArgs(args);
    }

	private void RunArgs(string[] args)
	{
		for (int i = 0; i < args.Length; i++)
		{
			string arg = args[i];
			switch (arg)
			{
				case "-l":
					Logger.InitLogger();
					break;
			}
		}
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
		try
		{
			ShowDialog(new ModConfigDialog(selectedMod));
		}
		catch (Exception ex)
		{
			ExceptionHandler.Throw(ex);
		}
	}
}