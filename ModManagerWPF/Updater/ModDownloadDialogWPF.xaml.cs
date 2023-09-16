using SAModManager.Common;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;

namespace SAModManager.Updater
{
	/// <summary>
	/// Interaction logic for ModDownloadDialogWPF.xaml
	/// </summary>
	public partial class ModDownloadDialogWPF : Window
	{
		private readonly List<ModDownloadWPF> updates;
		private readonly string updatePath;
		private readonly CancellationTokenSource tokenSource = new CancellationTokenSource();

		public ModDownloadDialogWPF(List<ModDownloadWPF> updates, string updatePath)
		{
			InitializeComponent();
			this.updates = updates;
			this.updatePath = updatePath;

			Loaded += OnLoaded;
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
				void OnDownloadProgress(object o, DownloadProgressEventArgs args)
				{
					Application.Current.Dispatcher.Invoke(() =>
					{
						Progress.Value = Util.SetProgress(args.BytesReceived / (double)args.TotalBytesToReceive);
						ProgressTxt.Text = $"Downloading file {args.FileDownloading} of {args.FilesToDownload}";
					});
					args.Cancel = token.IsCancellationRequested;
				}

				void OnDownloadCompleted(object o, CancelEventArgs args)
				{
					args.Cancel = token.IsCancellationRequested;
					Application.Current.Dispatcher.Invoke(() =>
					{
						tokenSource.Dispose();
					});
				}

				int modIndex = 0;
				foreach (ModDownloadWPF update in updates)
				{
					Application.Current.Dispatcher.Invoke(() =>
					{
						Title = update.Info.Name;
						HeaderTxt.Text = $"Updating mod {++modIndex} of {updates.Count}: {update.Info.Name}";
						ProgressTxt.Text = "Starting Download...";
					});

					update.Extracting += OnExtracting;
					update.ParsingManifest += OnParsingManifest;
					update.ApplyingManifest += OnApplyingManifest;
					update.DownloadProgress += OnDownloadProgress;
					update.DownloadCompleted += OnDownloadCompleted;

					bool retry = false;
					do
					{
						try
						{
							await Task.Run(() => update.Download(client, updatePath), token);
						}
						catch (AggregateException ae)
						{
							// Handle the exception
							ae.Handle(ex =>
							{

								string error = Lang.GetString("MessageWindow.Errors.ModUpdateFailed0") + $"{update.Info.Name}" + $":\r\n{ex.Message}" + "\r\n\r\n" + Lang.GetString("MessageWindow.Errors.ModUpdateFailed1");
								var res = new MessageWindow(Lang.GetString("MessageWindow.Errors.UpdateFailed.Title"), error, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.RetryCancel);
								retry = res.isRetry;
								res.ShowDialog();

								return true;
							});
						}
					} while (retry == true);

					update.Extracting -= OnExtracting;
					update.ParsingManifest -= OnParsingManifest;
					update.ApplyingManifest -= OnApplyingManifest;
					update.DownloadProgress -= OnDownloadProgress;
					update.DownloadCompleted -= OnDownloadCompleted;

					Application.Current.Dispatcher.Invoke(() =>
					{
						Progress.Value = Progress.Maximum;
						ProgressTxt.Text = $"Download complete.";
					});

					await Task.Delay(1000);
                    }
			}

			this.Close();
		}

		private void ButtonCancel_Click(object sender, RoutedEventArgs e)
		{
			this.Close();
		}
	}

}

