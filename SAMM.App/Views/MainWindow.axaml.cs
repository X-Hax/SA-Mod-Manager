using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using DialogHostAvalonia;
using SAMM.App.Controls.Dialogs;
using SAMM.App.Models;
using SAMM.Configuration;
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
		try
		{
			PART_DialogHost.Child = control;
			DialogHost.Show(PART_DialogHost, "MainWindowDialogHost");
		}
		catch (Exception ex)
		{
			ExceptionHandler.Throw(ex);
		}
	}

	public bool ShowMessage(string header, string message)
	{
		MessagePopupDialog popup = new MessagePopupDialog(header, message);
		PART_DialogHost.Child = popup;
		MainWindowDialogHost.IsOpen = true;

		return popup.IsOK;
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
		MessageHandler.Initialize(ShowMessage);

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
		ViewModel.Load();
	}

	private void OpenModConfigDialog(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
	{
		if (selectedMod != null)
			ShowDialog(new ModConfigDialog(selectedMod));
	}

	private void Window_Closing(object? sender, Avalonia.Controls.WindowClosingEventArgs e)
	{
		ViewModel.SaveManagerSettings();
	}

	private void OpenProfileDialog(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
	{
		if (ViewModel.UserProfiles != null)
			ShowDialog(new ProfileManagerDialog(ViewModel.UserProfiles, ViewModel.GameConfig.ProfilesPath));
	}
}