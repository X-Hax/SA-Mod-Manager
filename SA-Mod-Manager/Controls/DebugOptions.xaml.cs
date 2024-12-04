using SAModManager.Configuration;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace SAModManager.Controls
{
	/// <summary>
	/// Interaction logic for DebugOptions.xaml
	/// </summary>
	public partial class DebugOptions : UserControl
	{
		public DebugSettings Settings
		{
			get { return (DebugSettings)GetValue(SettingsProperty); }
			set { SetValue(SettingsProperty, value); }
		}

		public static readonly DependencyProperty SettingsProperty = 
			DependencyProperty.Register("Settings", typeof(DebugSettings), typeof(DebugOptions), new PropertyMetadata(null));

		public DebugOptions()
		{
			InitializeComponent();
		}

		private void DebugSettingsControl_Loaded(object sender, RoutedEventArgs e)
		{
			checkEnableLogConsole.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableDebugConsole")
			{
				Source = Settings,
				Mode = BindingMode.TwoWay,
			});
			checkEnableLogScreen.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableDebugScreen")
			{
				Source = Settings,
				Mode = BindingMode.TwoWay,
			});
			checkEnableLogFile.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableDebugFile")
			{
				Source = Settings,
				Mode = BindingMode.TwoWay,
			});
			checkEnableCrashDump.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableDebugCrashLog")
			{
				Source = Settings,
				Mode = BindingMode.TwoWay,
			});
		}
	}
}
