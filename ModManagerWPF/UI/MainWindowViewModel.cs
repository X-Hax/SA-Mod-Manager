using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows;
using GongSolutions.Wpf.DragDrop;


namespace SAModManager.UI
{
	public class MainWindowViewModel : INotifyPropertyChanged, IDropTarget
	{
		public ObservableCollection<ModData> Modsdata { get; set; } = new();
		public ObservableCollection<ModData> ModsSearch { get; set; } = new();

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
		}

	}
}
