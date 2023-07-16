using ModManagerCommon;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;

namespace ModManagerWPF.Updater
{
	/// <summary>
	/// Interaction logic for ManifestDialog.xaml
	/// </summary>
	public partial class ManifestDialog : Window
	{
		private readonly string modPath;
		private readonly string manifestPath;
		private readonly CancellationTokenSource tokenSource = new CancellationTokenSource();

		private List<ModManifestEntry> manifest;
		public List<Updater.ModManifestDiff> Diff { get; private set; }

		public ManifestDialog(string path, string title, bool allowCancel)
		{
			InitializeComponent();
			modPath = path;
			manifestPath = Path.Combine(path, "mod.manifest");
			Loaded += OnLoaded;
		}

		private async void OnLoaded(object sender, RoutedEventArgs e)
		{
			CancellationToken token = tokenSource.Token;
			var generator = new ModManifestGenerator();

			generator.FilesIndexed += (o, args) =>
			{
				//SetTask("Manifest generation complete!");
				//SetTaskCount(args.FileCount);
			};

			generator.FileHashStart += (o, args) =>
			{
				args.Cancel = token.IsCancellationRequested;
				//SetTaskAndStep($"Hashing file: {args.FileIndex}/{args.FileCount}", args.FileName);
				//SetProgress(args.FileIndex / (double)args.FileCount);
			};

			generator.FileHashEnd += (o, args) =>
			{
				args.Cancel = token.IsCancellationRequested;
				//NextTask();
			};

			using (var task = new Task(() =>
			{
				manifest = generator.Generate(modPath);

				if (!token.IsCancellationRequested)
				{
					Diff = ModManifestGenerator.Diff(manifest, File.Exists(manifestPath) ? ModManifest.FromFile(manifestPath) : null);
				}
			}))
			{
				task.Start();

				while (!task.IsCompleted && !task.IsCanceled)
				{
					await Dispatcher.Yield(DispatcherPriority.Background);
				}

				task.Wait(token);
			}

			DialogResult = true;

		}

		private void CancelButton_Click(object sender, RoutedEventArgs e)
		{
			tokenSource.Cancel();
			Close();
		}
	}
}
