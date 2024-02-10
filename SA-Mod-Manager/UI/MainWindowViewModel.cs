using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows;
using GongSolutions.Wpf.DragDrop;
using SAModManager.Common;


namespace SAModManager.UI
{
	public class MainWindowViewModel : INotifyPropertyChanged, IDropTarget
	{
		public ObservableCollection<ModData> Modsdata { get; set; } = new();
		public ObservableCollection<ModData> ModsSearch { get; set; } = new();
        public ModData SelectedMod { get; set; }
        public CodeData SelectedCode { get; set; }
        public IEnumerable<Game> Games { get; set; }

        public event PropertyChangedEventHandler PropertyChanged;

		public void DragOver(IDropInfo dropInfo)
		{
			if (dropInfo.Data is IDataObject dataObject && dataObject.GetDataPresent(DataFormats.FileDrop))
				dropInfo.Effects = DragDropEffects.Copy;
			else
				dropInfo.Effects = DragDropEffects.Move;
		}

        public void Drop(IDropInfo dropInfo)
        {
            GongSolutions.Wpf.DragDrop.DragDrop.DefaultDropHandler.Drop(dropInfo);

            var mainWindow = (MainWindow)App.Current.MainWindow;
            var selectedItems = dropInfo.Data as List<object>;

            if (selectedItems != null)
            {
                mainWindow.listMods.SelectedItems.Clear();

                foreach (var selectedItem in selectedItems)
                {
                    if (selectedItem is ModData modData)
                    {
                        mainWindow.listMods.SelectedItems.Add(modData);
                    }
                }
            }
            else if (dropInfo.Data is ModData modData)
            {
                int index = mainWindow.listMods.Items.IndexOf(modData);
                mainWindow.listMods.SelectedItems.Clear();
                mainWindow.listMods.SelectedIndex = index;
                mainWindow.listMods.SelectedItem = modData;
            }
        }
    }
}
