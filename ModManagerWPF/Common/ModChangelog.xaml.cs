using ModManagerCommon;
using ModManagerCommon.Controls;
using Octokit;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace ModManagerWPF.Common
{
	/// <summary>
	/// Interaction logic for ModChangelog.xaml
	/// </summary>
	

	public partial class ModChangelog : Window
	{ 
		ModUpdateDetails modUpdateDetails;
		private readonly List<ModDownload> mods;

		public List<ModDownload> SelectedMods { get; } = new List<ModDownload>();

		private void SetModDetails(ModDownload entry)
		{
			modUpdateDetails = new();
			textChangeLog.Text = entry?.Changes.Trim();
			modUpdateDetails.SetData(entry);

			FilesList.BeginInit();
			FilesList.Items.Clear();

			if (entry?.Type.Equals(ModDownloadType.Modular) == true)
			{
				tabPageFiles.IsEnabled = true;

				foreach (ModManifestDiff i in entry.ChangedFiles)
				{
					string file = i.State == ModManifestState.Moved ? $"{i.Last.FilePath} -> {i.Current.FilePath}" : i.Current.FilePath;
					FilesList.Items.Add(new { State = i.State.ToString(), File = file });
				}

				// Auto-resize columns based on content
				if (FilesList.View is GridView gridView)
				{
					foreach (GridViewColumn column in gridView.Columns)
					{
						column.Width = 0;
						column.Width = double.NaN;
					}
				}
			}
			else
			{
				tabPageFiles.IsEnabled = false;
			}

			FilesList.EndInit();
		}

		private void AdjustDetailsDisplay(ModDownload mod)
		{
			bool IsSelfHosted = string.IsNullOrEmpty(mod.Info.UpdateUrl);

			if (IsSelfHosted)
			{
				UpdateDetails.RowDefinitions[2].Height = new GridLength(30);
				UpdateDetails.RowDefinitions[3].Height = new GridLength(0);
				UpdateDetails.RowDefinitions[4].Height = new GridLength(0);
			}
			else
			{
				UpdateDetails.RowDefinitions[2].Height = new GridLength(0);
				UpdateDetails.RowDefinitions[3].Height = new GridLength(30);
				UpdateDetails.RowDefinitions[4].Height = new GridLength(30);
			}
		}

		public ModChangelog(List<ModDownload> mods)
        {
			InitializeComponent();
			this.mods = mods;
		}

		private void Window_Loaded(object sender, RoutedEventArgs e)
		{
			ModsList.BeginInit();

			foreach (ModDownload download in mods)
			{
				download.IsChecked = true;
				ModsList.Items.Add(download);
			}

			ModsList.SelectedIndex = 0;

			ModsList.EndInit();
		}

		private void ImageButton_Click(object sender, RoutedEventArgs e)
		{
			foreach (ModDownload item in mods)
			{
				if (item.IsChecked == true)
					SelectedMods.Add(item);
			}

			DialogResult = true;
		}

		private void CheckBox_Checked(object sender, RoutedEventArgs e)
		{
			btnInstallNow.IsEnabled = ModsList.Items.Cast<ModDownload>().Any(x => x.IsChecked);
		}

		private void ModsList_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			var items = ModsList.SelectedItems;

			if (items.Count > 1 || items.Count == 0)
			{
				SetModDetails(null);
			}
			else
			{
				var entry = items[0] as ModDownload;
				SetModDetails(entry);
				AdjustDetailsDisplay(entry);
			}
		}

		private void ImageButton_Click_1(object sender, RoutedEventArgs e)
		{
			DialogResult = false;
			this.Close();
		}
	}
}
