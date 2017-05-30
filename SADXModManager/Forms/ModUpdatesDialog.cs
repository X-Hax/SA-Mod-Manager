using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace SADXModManager.Forms
{
	public partial class ModUpdatesDialog : Form
	{
		private readonly List<ModDownload> mods;

		public List<ModDownload> SelectedMods { get; } = new List<ModDownload>();

		public ModUpdatesDialog(List<ModDownload> mods)
		{
			this.mods = mods;
			InitializeComponent();
		}

		private void ModUpdatesDialog_Load(object sender, EventArgs e)
		{
			listModUpdates.BeginUpdate();
			foreach (ModDownload download in mods)
			{
				listModUpdates.Items.Add(new ListViewItem(new[] { download.Info.Name })
				{
					Checked = true, Tag = download
				});
			}

			listModUpdates.Items[0].Selected = true;
			listModUpdates.EndUpdate();
		}

		private void buttonInstall_Click(object sender, EventArgs e)
		{
			foreach (ListViewItem item in listModUpdates.Items.Cast<ListViewItem>().Where(item => item.Checked))
			{
				SelectedMods.Add((ModDownload)item.Tag);
			}
		}

		private void listModUpdates_ItemChecked(object sender, ItemCheckedEventArgs e)
		{
			buttonInstall.Enabled = listModUpdates.Items.Cast<ListViewItem>().Any(x => x.Checked);
		}

		private void SetModDetails(ModDownload entry)
		{
			textChangeLog.Text = entry?.Changes.Trim();
			modUpdateDetails.SetData(entry);

			listFiles.BeginUpdate();
			listFiles.Items.Clear();

			if (entry?.Type == ModDownloadType.Modular)
			{
				tabPageFiles.Enabled = true;

				foreach (ModManifestDiff i in entry.ChangedFiles)
				{
					string file = i.State == ModManifestState.Moved ? $"{i.Last.FilePath} -> {i.Current.FilePath}" : i.Current.FilePath;
					listFiles.Items.Add(new ListViewItem(new[] { i.State.ToString(), file }));
				}

				listFiles.AutoResizeColumns(ColumnHeaderAutoResizeStyle.ColumnContent);
			}
			else
			{
				tabPageFiles.Enabled = false;
			}

			listFiles.EndUpdate();
		}

		private void listModUpdates_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
		{
			var items = listModUpdates.SelectedItems;
			if (items.Count > 1 || items.Count == 0)
			{
				SetModDetails(null);
			}
			else
			{
				var entry = items[0].Tag as ModDownload;
				SetModDetails(entry);
			}
		}
	}
}
