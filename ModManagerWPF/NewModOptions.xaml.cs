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
using static System.Windows.Forms.Design.AxImporter;


namespace ModManagerWPF
{
	/// <summary>
	/// Interaction logic for NewModOptions.xaml
	/// </summary>
	public partial class NewModOptions : Window
	{
		public enum Type : int
		{
			NewMod,
			ModArchive,
			ModFolder,
		}

		public NewModOptions()
		{
			InitializeComponent();
			Header.Text = Lang.GetString("InstallModTitle");
	
			CheckStack.Children.Add(new RadioButton() { IsChecked = true, Content = Lang.GetString("InstallMakeMod"), Margin = new Thickness(2) });;
			CheckStack.Children.Add(new RadioButton() { Content = Lang.GetString("InstallModArchive"), Margin = new Thickness(2) });
			CheckStack.Children.Add(new RadioButton() { Content = Lang.GetString("InstallModFolder"), Margin = new Thickness(2) });
		}


		private void Window_ContentRendered(object sender, EventArgs e)
		{
			InvalidateVisual(); //fix render
		}

		private void ButtonCancel_Click(object sender, RoutedEventArgs e)
		{
			this.Close();
        }

		public int Ask()
		{
			ShowDialog();
			if (DialogResult == false)
				return -1;

			return GetSelected();
		}

		private int GetSelected()
		{
			for (int i = 0; i < CheckStack.Children.Count; i++)
			{
				if (((RadioButton)CheckStack.Children[i]).IsChecked.Value)
				{
					return i;
				}
			}
			return -1;
		}

		private void OK_Click(object sender, RoutedEventArgs e)
		{
			if (GetSelected() < 0)
			{
				return;
			}
			DialogResult = true;
		}
	}
}
