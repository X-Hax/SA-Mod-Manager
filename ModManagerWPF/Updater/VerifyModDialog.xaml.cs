using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;
using ModManagerCommon;

namespace ModManagerWPF.Updater
{
	/// <summary>
	/// Interaction logic for VerifyModDialog.xaml
	/// </summary>
	public partial class VerifyModDialog : Window
	{
		private readonly string ModDirectory;
		private readonly List<Tuple<string, ModInfo>> mods;
		private readonly CancellationTokenSource tokenSource = new();

		public List<Tuple<string, ModInfo, List<ModManifestDiff>>> Failed { get; }
		
		public VerifyModDialog(List<Tuple<string, ModInfo>> mods, string modDirectory)
		{
			InitializeComponent();
			Failed = new List<Tuple<string, ModInfo, List<ModManifestDiff>>>();
			this.mods = mods;
			this.ModDirectory = modDirectory;
			Loaded += OnLoaded;
		}

		private void CancelButton_Click(object sender, EventArgs e)
		{
			tokenSource.Cancel();
			this.Close();
		}

		private async void OnLoaded(object sender, EventArgs e)
		{

			int modCount = mods.Count;
			int modIndex = 0;
			Util.SetTaskCount(modCount, Progress.Maximum);

			CancellationToken token = tokenSource.Token;
			var generator = new Updater.ModManifestGenerator();

			void hashStart(object o, FileHashEventArgs args)
			{
				args.Cancel = token.IsCancellationRequested;

				Application.Current.Dispatcher.Invoke(() =>
				{
					TxtProgress.Text = $"Checking file { args.FileIndex}/{ args.FileCount}: { args.FileName}";
					double value = Util.SetProgress(args.FileIndex / (double)args.FileCount);
					Progress.Value = value;
				});
			}

			void hashEnd(object o, FileHashEventArgs args)
			{
				args.Cancel = token.IsCancellationRequested;
			}

			generator.FileHashStart += hashStart;
			generator.FileHashEnd += hashEnd;


			await Task.Run(() =>
			{
				foreach (Tuple<string, ModInfo> i in mods)
				{
					string path = Path.Combine(ModDirectory, i.Item1);
					ModInfo info = i.Item2;
					string manifPath = Path.Combine(path, "mod.manifest");
				
					Application.Current.Dispatcher.Invoke(() =>
					{
						Title = info.Name;
						HeaderTxt.Text = info.Name;
						TxtProgress.Text = $"Verifying mod {++modIndex}/{modCount}: {info.Name}";
					});

					List<ModManifestEntry> local = ModManifest.FromFile(manifPath);
					List<ModManifestDiff> diff = generator.Verify(path, local);

					if (diff?.Any(x => x.State != ModManifestState.Unchanged) == true)
					{
						Application.Current.Dispatcher.Invoke(() =>
						{
							Failed.Add(new Tuple<string, ModInfo, List<ModManifestDiff>>(i.Item1, info, diff));
						});
					}
				}
			});

			DialogResult = true;
		}
	}
}
