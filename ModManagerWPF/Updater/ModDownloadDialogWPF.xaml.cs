using ModManagerCommon;
using ModManagerCommon.Forms;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Shell;

namespace ModManagerWPF.Updater
{
	/// <summary>
	/// Interaction logic for ModDownloadDialogWPF.xaml
	/// </summary>
	public partial class ModDownloadDialogWPF : Window
	{
		private readonly List<Common.ModDownloadWPF> updates;
		private readonly string updatePath;
		private readonly CancellationTokenSource tokenSource = new CancellationTokenSource();
		private IProgress<double?> _progress;


		public ModDownloadDialogWPF(List<Common.ModDownloadWPF> updates, string updatePath)
		{
			InitializeComponent();
			this.updates = updates;
			this.updatePath = updatePath;

			Loaded += OnLoaded;

			_progress = new Progress<double?>((v) =>
			{
				if (v.HasValue)
				{
					TaskbarItemInfo.ProgressState = TaskbarItemProgressState.Normal;
					Progress.IsIndeterminate = false;
					TaskbarItemInfo.ProgressValue = v.Value;
					Progress.Value = v.Value;
				}
				else
				{
					Progress.IsIndeterminate = true;
					TaskbarItemInfo.ProgressState = TaskbarItemProgressState.Indeterminate;
				}
			});

		}

		private async void OnLoaded(object sender, RoutedEventArgs e)
		{
			using (var client = new UpdaterWebClient())
			{
				CancellationToken token = tokenSource.Token;


				void OnExtracting(object o, CancelEventArgs args)
				{
					//SetTaskAndStep("Extracting...");
					Application.Current.Dispatcher.Invoke(() =>
					{
						ProgressTxt.Text = "Extracting...";
					});
					args.Cancel = token.IsCancellationRequested;
				}
				void OnParsingManifest(object o, CancelEventArgs args)
				{
					Application.Current.Dispatcher.Invoke(() =>
					{
						ProgressTxt.Text = "Parsing manifest...";
					});
					args.Cancel = token.IsCancellationRequested;
				}
				void OnApplyingManifest(object o, CancelEventArgs args)
				{
					Application.Current.Dispatcher.Invoke(() =>
					{
						ProgressTxt.Text = "Applying manifest...";
					});
					args.Cancel = token.IsCancellationRequested;
				}
				void OnDownloadProgress(object o, Common.DownloadProgressEventArgs args)
				{
					Application.Current.Dispatcher.Invoke(() =>
					{
						//SetProgress(args.BytesReceived / (double)args.TotalBytesToReceive);
						//SetTaskAndStep($"Downloading file {args.FileDownloading} of {args.FilesToDownload}:",
						//$"({SizeSuffix.GetSizeSuffix(args.BytesReceived)} / {SizeSuffix.GetSizeSuffix(args.TotalBytesToReceive)})");
						ProgressTxt.Text = $"Downloading file {args.FileDownloading} of {args.FilesToDownload}";
					});
					args.Cancel = token.IsCancellationRequested;
				}
				void OnDownloadCompleted(object o, CancelEventArgs args)
				{
					//NextTask();
					args.Cancel = token.IsCancellationRequested;
				}


				int modIndex = 0;
				foreach (Common.ModDownloadWPF update in updates)
				{
					Title = update.Info.Name;
					HeaderTxt.Text = $"Updating mod {++modIndex} of {updates.Count}: {update.Info.Name}";
					ProgressTxt.Text = "Starting Download...";

					update.Extracting += OnExtracting;
					update.ParsingManifest += OnParsingManifest;
					update.ApplyingManifest += OnApplyingManifest;
					update.DownloadProgress += OnDownloadProgress;
					update.DownloadCompleted += OnDownloadCompleted;

					try
					{
						await Task.Run(() => update.Download(client, updatePath), token);
					}
					catch (AggregateException ae)
					{
						// Handle the exception
						ae.Handle(ex =>
						{
							/*MessageBox.Show(this, $"Failed to update mod {update.Info.Name}:\r\n{ex.Message}"
								+ "\r\n\r\nPress Retry to try again, or Cancel to skip this mod.",
								"Update Failed", MessageBoxButton.YesNo, MessageBoxIcon.Error);*/
							return true;
						});
					}

					update.Extracting -= OnExtracting;
					update.ParsingManifest -= OnParsingManifest;
					update.ApplyingManifest -= OnApplyingManifest;
					update.DownloadProgress -= OnDownloadProgress;
					update.DownloadCompleted -= OnDownloadCompleted;
				}
			}
		}

		private void ButtonCancel_Click(object sender, RoutedEventArgs e)
		{
			this.Close();
		}
	}


}

