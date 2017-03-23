using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SADXModManager.Forms
{
	class DownloadDialog : ProgressDialog
	{
		private readonly List<ModDownload> updates;
		private readonly string updatePath;
		private readonly CancellationTokenSource tokenSource = new CancellationTokenSource();

		public DownloadDialog(List<ModDownload> updates, string updatePath)
			: base("Update Progress", true)
		{
			this.updates    = updates;
			this.updatePath = updatePath;

			Shown += OnShown;
			CancelEvent += OnCancelEvent;
		}

		private void OnCancelEvent(object sender, EventArgs eventArgs)
		{
			tokenSource.Cancel();
		}

		private void OnShown(object sender, EventArgs eventArgs)
		{
			DialogResult = DialogResult.OK;

			SetTaskCount(updates.Sum(update => update.FilesToDownload));

			using (var client = new UpdaterWebClient())
			{
				CancellationToken token = tokenSource.Token;

				void OnExtracting(object o, CancelEventArgs args)
				{
					SetTaskAndStep("Extracting...");
					args.Cancel = token.IsCancellationRequested;
				}
				void OnParsingManifest(object o, CancelEventArgs args)
				{
					SetTaskAndStep("Parsing manifest...");
					args.Cancel = token.IsCancellationRequested;
				}
				void OnApplyingManifest(object o, CancelEventArgs args)
				{
					SetTaskAndStep("Applying manifest...");
					args.Cancel = token.IsCancellationRequested;
				}
				void OnDownloadProgress(object o, DownloadProgressEventArgs args)
				{
					SetProgress(args.BytesReceived / (double)args.TotalBytesToReceive);
					SetTaskAndStep($"Downloading file {args.FileDownloading} of {args.FilesToDownload}:",
						$"({SizeSuffix.GetSizeSuffix(args.BytesReceived)} / {SizeSuffix.GetSizeSuffix(args.TotalBytesToReceive)})");
					args.Cancel = token.IsCancellationRequested;
				}
				void OnDownloadCompleted(object o, CancelEventArgs args)
				{
					NextTask();
					args.Cancel = token.IsCancellationRequested;
				}

				int modIndex = 0;
				foreach (ModDownload update in updates)
				{
					DialogResult result;

					Title = $"Updating mod {++modIndex} of {updates.Count}: {update.Info.Name}";
					SetTaskAndStep("Starting download...");

					update.Extracting        += OnExtracting;
					update.ParsingManifest   += OnParsingManifest;
					update.ApplyingManifest  += OnApplyingManifest;
					update.DownloadProgress  += OnDownloadProgress;
					update.DownloadCompleted += OnDownloadCompleted;

					do
					{
						result = DialogResult.Cancel;

						try
						{
							// poor man's await Task.Run (not available in .net 4.0)
							using (var task = new Task(() => update.Download(client, updatePath), token))
							{
								task.Start();

								while (!task.IsCompleted && !task.IsCanceled)
								{
									Application.DoEvents();
								}

								task.Wait(token);
							}
						}
						catch (AggregateException ae)
						{
							ae.Handle(ex =>
							{
								result = MessageBox.Show(this, $"Failed to update mod { update.Info.Name }:\r\n{ ex.Message }"
									+ "\r\n\r\nPress Retry to try again, or Cancel to skip this mod.",
									"Update Failed", MessageBoxButtons.RetryCancel, MessageBoxIcon.Error);
								return true;
							});
						}
					} while (result == DialogResult.Retry);

					update.Extracting        -= OnExtracting;
					update.ParsingManifest   -= OnParsingManifest;
					update.ApplyingManifest  -= OnApplyingManifest;
					update.DownloadProgress  -= OnDownloadProgress;
					update.DownloadCompleted -= OnDownloadCompleted;
				}
			}
		}

		protected override void Dispose(bool disposing)
		{
			tokenSource.Dispose();
			base.Dispose(disposing);
		}
	}
}
