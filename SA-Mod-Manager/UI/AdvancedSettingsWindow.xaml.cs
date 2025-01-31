using SAModManager.Configuration;
using SAModManager.Management;
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
using System.Windows.Shapes;

namespace SAModManager.UI
{
	/// <summary>
	/// Interaction logic for AdvancedSettings.xaml
	/// </summary>
	public partial class AdvancedSettingsWindow : Window
	{
		public AdvancedSettingsWindow()
		{
			InitializeComponent();
			chkPortableMode.IsChecked = SettingsManager.IsPortableMode();
		}

		private void CheckBox_MouseEnter(object sender, MouseEventArgs e)
		{
			CheckBox chk = sender as CheckBox;

			if (chk != null)
			{
				SettingDescription.Text = chk.Tag as string;
			}
		}

		private void CheckBox_MouseLeave(object sender, MouseEventArgs e)
		{
			SettingDescription.Text = string.Empty;
		}

		private void chkPortableMode_Click(object sender, RoutedEventArgs e)
		{
			SettingsManager.ProcessPortableMode();

			chkPortableMode.IsChecked = SettingsManager.IsPortableMode();
		}
	}
}
