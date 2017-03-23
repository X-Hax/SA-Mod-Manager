using System;
using System.Collections.Generic;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SADXModManager.Forms
{
	class ManifestDialog : ProgressDialog
	{
		private string modPath;
		private string manifestPath;
		private readonly CancellationTokenSource tokenSource = new CancellationTokenSource();

		public List<ModManifest> manifest;
		public List<ModManifestDiff> diff;

		public ManifestDialog(string path, string title, bool allowCancel) : base(title, allowCancel)
		{
			modPath = path;
			manifestPath = Path.Combine(path, "mod.manifest");
			Shown += OnShown;
			CancelEvent += OnCancelEvent;
		}

		private void OnCancelEvent(object sender, EventArgs eventArgs)
		{
			tokenSource.Cancel();
		}

		private void OnShown(object sender, EventArgs e)
		{
			CancellationToken token = tokenSource.Token;
			var generator = new ModManifestGenerator();

			generator.FilesIndexed += (o, args) =>
			{
				SetTask("Manifest generation complete!");
				SetTaskCount(args.FileCount);
			};

			generator.FileHashStart += (o, args) =>
			{
				args.Cancel = token.IsCancellationRequested;
				SetTaskAndStep($"Hashing file: {args.FileIndex}/{args.FileCount}", args.FileName);
				SetProgress(args.FileIndex / (double)args.FileCount);
			};

			generator.FileHashEnd += (o, args) =>
			{
				args.Cancel = token.IsCancellationRequested;
				NextTask();
			};

			using (var task = new Task(() =>
			{
				manifest = generator.Generate(modPath);

				if (!token.IsCancellationRequested)
				{
					diff = ModManifestGenerator.Diff(manifest, File.Exists(manifestPath) ? ModManifest.FromFile(manifestPath) : null);
				}
			}))
			{
				task.Start();

				while (!task.IsCompleted && !task.IsCanceled)
				{
					Application.DoEvents();
				}

				task.Wait(token);
			}

			DialogResult = DialogResult.OK;
		}

		protected override void Dispose(bool disposing)
		{
			tokenSource.Dispose();
			base.Dispose(disposing);
		}
	}
}
