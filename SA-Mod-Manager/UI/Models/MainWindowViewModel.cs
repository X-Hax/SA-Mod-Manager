using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Windows;
using GongSolutions.Wpf.DragDrop;


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

        public void SortViewModelCollection(MainWindowViewModel viewModel, List<string> orderList)
        {
            var orderMap = orderList.Select((value, index) => new { value, index })
                                    .ToDictionary(x => x.value, x => x.index);

            var sortedData = viewModel.Modsdata.OrderBy(customData =>
            {
                //Try to get the index of the customData.Name in the order list
                if (orderMap.TryGetValue(customData.Tag, out int index))
                {
                    return index;
                }
                return int.MaxValue; //Place it at the end if not found
            }).ToList();

            //Replace with the sorted data
            viewModel.Modsdata.Clear();
            foreach (var data in sortedData)
            {
                viewModel.Modsdata.Add(data);
            }

        }

        public void DragOver(IDropInfo dropInfo)
        {
            dropInfo.DropTargetAdorner = DropTargetAdorners.Insert;
            if (dropInfo.Data is IDataObject dataObject && dataObject.GetDataPresent(DataFormats.FileDrop))
                dropInfo.Effects = DragDropEffects.Copy;
            else
                dropInfo.Effects = DragDropEffects.Move;
        }

        public void Drop(IDropInfo dropInfo)
        {
            GongSolutions.Wpf.DragDrop.DragDrop.DefaultDropHandler.Drop(dropInfo);

            var mainWindow = (MainWindow)App.Current.MainWindow;

            if (dropInfo.Data is List<object> selectedItems)
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
