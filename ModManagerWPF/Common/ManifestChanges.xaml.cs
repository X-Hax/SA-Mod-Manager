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

namespace ModManagerWPF.Common
{
    /// <summary>
    /// Interaction logic for ManifestChanges.xaml
    /// </summary>
    public partial class ManifestChanges : Window
    {
		private readonly List<Updater.ModManifestDiff> diff;

		public ManifestChanges(List<Updater.ModManifestDiff> diff)
        {
			this.diff = diff;
			InitializeComponent();
			Loaded += ManifestDiff_Load;

		}

		public class ManifestDiffItem
		{
			public string FilePath { get; set; }
			public Updater.ModManifestState State { get; set; }
			public bool IsChecked { get; set; }
			public Updater.ModManifestDiff Entry { get; set; }
		}

		private void ManifestDiff_Load(object sender, EventArgs e)
		{
			ManifestList.BeginInit();

			List<ManifestDiffItem> items = diff
				.Where(x => x.State != Updater.ModManifestState.Unchanged)
				.Select(entry => new ManifestDiffItem
				{
					FilePath = entry.Current.FilePath,
					State = entry.State,
					IsChecked = entry.State != Updater.ModManifestState.Removed,
					Entry = entry
				}).ToList();

			ManifestList.ItemsSource = items;

			ManifestList.EndInit();
		}

		public List<Updater.ModManifestEntry> MakeNewManifest()
		{
			var result = new List<Updater.ModManifestEntry>();

			foreach (ManifestDiffItem item in ManifestList.Items)
			{
				if (item.IsChecked)
				{
					result.Add(item.Entry.Current);
				}
			}

			result.AddRange(diff.Where(x => x.State == Updater.ModManifestState.Unchanged).Select(x => x.Current));

			return result;
		}

		private void CancelBtn_Click(object sender, RoutedEventArgs e)
		{
			DialogResult = false;
			this.Close();
		}
	}

}
