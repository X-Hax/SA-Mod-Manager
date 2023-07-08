using ModManagerCommon;
using ModManagerWPF.Common;
using System;
using System.Collections.Generic;
using System.IO;
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




namespace ModManagerWPF
{
	/// <summary>
	/// Interaction logic for NewModOptions.xaml
	/// </summary>
	public partial class InstallModOptions : Window
	{
		public enum Type : int
		{
			NewMod,
			ModArchive,
			ModFolder,
		}

		public InstallModOptions()
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

		//to do add extension support
		public void InstallModArchive(string path, string root)
		{
			((MainWindow)App.Current.MainWindow).Save();
		}

		public void InstallMod(string path, string root)
		{
			if (File.Exists(path))
				InstallModArchive(path, root);
			else if (Directory.Exists(path))
				InstallModDirectory(path, root);
		}

		public static void InstallModDirectory(string path, string root)
		{
			if (path == root || path.Contains(root)) //prevent user from trying to install a mod that is already installed.
			{
				MessageWindow msg = new(Lang.GetString("SadxManagerError"), Lang.GetString("ErrorDontWantTo"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning);
				msg.Show();
				return;
			}

			// A list of folders that have mod.ini in them
			var directories = new List<string>();

			// Looks though all the folders for mods
			directories.AddRange(Directory.GetDirectories(path, "*", SearchOption.AllDirectories)
				.Where(t => File.Exists(Path.Combine(t, "mod.ini"))));

			// Checks if there is a file called "mod.ini" inside the selected folder
			if (File.Exists(Path.Combine(path, "mod.ini")))
				directories.Add(path);

			// Check if there is any mods
			if (directories.Count > 0)
			{
				foreach (string folder in directories)
				{
					string directoryName = Path.GetFileName(folder);

					// If it doesn't know the name of the mod its installing

					// Creates all of the directories.
					Directory.CreateDirectory(Path.Combine(root, Path.GetFileName(folder)));
					foreach (string dirPath in Directory.GetDirectories(folder, "*", SearchOption.AllDirectories))
						Directory.CreateDirectory(dirPath.Replace(folder, Path.Combine(root, directoryName)));

					// Copies all the files from the Directories.
					foreach (string filePath in Directory.GetFiles(folder, "*.*", SearchOption.AllDirectories))
						File.Copy(filePath, filePath.Replace(folder, Path.Combine(root, directoryName)), true);
				}
			}

			((MainWindow)App.Current.MainWindow).Save();
		}
	}
}
