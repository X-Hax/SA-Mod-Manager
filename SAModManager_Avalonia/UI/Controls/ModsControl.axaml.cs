using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using CommunityToolkit.Mvvm.ComponentModel;
using System.Collections.ObjectModel;

namespace SAModManager_Avalonia.UI.Controls
{
	public class ModEntry : ObservableObject
	{
		private bool isChecked = false;
		public bool IsChecked
		{
			get => isChecked;
			set
			{
				isChecked = value;
				OnPropertyChanged(nameof(IsChecked));
			}
		}

		// Replace the following with SAModInfo once ported
		private string mod = string.Empty;
		public string Mod
		{
			get => mod;
			set
			{
				mod = value;
				OnPropertyChanged(nameof(Mod));
			}
		}
	}

	public partial class ModsControl : UserControl
	{
		public ObservableCollection<ModEntry> ModList
		{
			get { return (ObservableCollection<ModEntry>)GetValue(ModListProperty); }
			set { SetValue(ModListProperty, value); }
		}
		public static readonly StyledProperty<ObservableCollection<ModEntry>> ModListProperty =
			AvaloniaProperty.Register<ModsControl, ObservableCollection<ModEntry>>(nameof(ModList), defaultValue: new ObservableCollection<ModEntry>());

		public ModsControl()
		{
			InitializeComponent();
		}
	}
}