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
			Activate();
			Loaded += OnLoaded;
		}

        private static string GenerateHeaderText(int modIndex, int totalCount, string name)
        {
            string updatingText = Lang.GetString("Updater.DL.Mod.UpdatingMod");
			string ofText = Lang.GetString("Updater.DL.Mod.of");

            return $"{updatingText} {modIndex} {ofText} {totalCount}: {name}";
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
						ProgressTxt.Text = Lang.GetString("Updater.DL.Mod.Extracting");
					});
					args.Cancel = token.IsCancellationRequested;
				}
				void OnParsingManifest(object o, CancelEventArgs args)
				{
					Application.Current.Dispatcher.Invoke(() =>
					{
						ProgressTxt.Text = Lang.GetString("Updater.DL.Mod.ParsingManifest");
                    });
					args.Cancel = token.IsCancellationRequested;
				}
				void OnApplyingManifest(object o, CancelEventArgs args)
				{
					Application.Current.Dispatcher.Invoke(() =>
					{
						ProgressTxt.Text = Lang.GetString("Updater.DL.Mod.ApplyingManifest");
                    });
					args.Cancel = token.IsCancellationRequested;
				}
				void OnDownloadProgress(object o, DownloadProgressEventArgs args)
				{
					Application.Current.Dispatcher.Invoke(() =>
					{
						Progress.Value = Util.SetProgress(args.BytesReceived / (double)args.TotalBytesToReceive);
						ProgressTxt.Text = Lang.GetString("Updater.DL.Mod.DownloadingFile") + " " + args.FileDownloading + " / " + args.FilesToDownload;
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
						HeaderTxt.Text = GenerateHeaderText(++modIndex, updates.Count, update.Info.Name);
						ProgressTxt.Text = Lang.GetString("Updater.DL.Mod.StartingDownload");
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
						ProgressTxt.Text = Lang.GetString($"Updater.DL.Mod.DownloadComplete");
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

