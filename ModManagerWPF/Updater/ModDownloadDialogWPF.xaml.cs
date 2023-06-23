using ModManagerCommon;
using System;
using System.Collections.Generic;

using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Forms;
using System.Windows.Shell;

namespace ModManagerWPF.Updater
{
	/// <summary>
	/// Interaction logic for ModDownloadDialogWPF.xaml
	/// </summary>
	public partial class ModDownloadDialogWPF : Window
	{
		private readonly List<ModDownload> updates;
		private readonly string updatePath;
		private readonly CancellationTokenSource tokenSource = new CancellationTokenSource();
		private IProgress<double?> _progress;

		public ModDownloadDialogWPF(List<ModDownload> updates, string updatePath)
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

				int modIndex = 0;
				foreach (ModDownload update in updates)
				{

					Title = update.Info.Name;
					HeaderTxt.Text = $"Updating mod {++modIndex} of {updates.Count}: {update.Info.Name}";

					/*result = CustomMessageBox.Show(this, $"Failed to update mod {update.Info.Name}:\r\n{ex.Message}"
						+ "\r\n\r\nPress Retry to try again, or Cancel to skip this mod.",
						"Update Failed", MessageBoxButton.RetryCancel, MessageBoxImage.Error);*/
					try
					{
						// poor man's await Task.Run (not available in .net 4.0)
						using (var task = new Task(() => update.Download(client, updatePath), token))
						{
							task.Start();

							while (!task.IsCompleted && !task.IsCanceled)
							{
								App.DoEvents();
							}

							task.Wait(token);
						}
					}
					catch (AggregateException ae)
					{
						ae.Handle(ex =>
						{
							return true;
						});
					}


				}
			}
		}

		private void ButtonCancel_Click(object sender, RoutedEventArgs e)
		{

		}
	}
}

