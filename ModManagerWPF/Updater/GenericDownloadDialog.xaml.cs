using SAModManager.Common;
using System;
using System.IO;
using System.Net;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;

namespace SAModManager.Updater
{

	/// <summary>
	/// Interaction logic for GenericUpdateDialog.xaml
	/// </summary>
	public partial class GenericDownloadDialog : Window
	{
		private readonly Uri uri;
		private readonly string fileName;
		private readonly string dest = "SATemp";
		private bool defaultFolder = false;
		private readonly CancellationTokenSource tokenSource = new();

		public GenericDownloadDialog(Uri uri, string title, string fileName, bool defaultFolder = false)
		{
			InitializeComponent();

			Title = "Download - " + title;
			DLInfo.Text += " " + title + "...";
			this.fileName = fileName;
			this.uri = uri;
			this.defaultFolder = defaultFolder;

			if (!defaultFolder)
			{
				try
				{
					if (!Directory.Exists(dest))
					{
						Directory.CreateDirectory(dest);
					}
				}
				catch { }
			}
		}

		private void WebClient_DownloadProgressChanged(object sender, DownloadProgressChangedEventArgs e)
		{
			Application.Current.Dispatcher.Invoke(() =>
			{
				DLProgress.Value = e.ProgressPercentage;
			});
		}

		private async void WebClient_DownloadFileCompleted(object sender, System.ComponentModel.AsyncCompletedEventArgs e)
		{
			Application.Current.Dispatcher.Invoke(() =>
			{
				DLInfo.Text = "Download completed.";

			});


			if (File.Exists(fileName) && !defaultFolder) 
			{ 
				File.Move(fileName, Path.Combine(dest, fileName));
			}

			await Task.Delay(3000);
			Application.Current.Dispatcher.Invoke(() =>
			{
				DialogResult = true;
				this.Close();
			});
		}

		public async void StartDL()
		{
			
			using (var client = new UpdaterWebClient())
			{
				CancellationToken token = tokenSource.Token;
				client.DownloadProgressChanged += WebClient_DownloadProgressChanged;
				client.DownloadFileCompleted += WebClient_DownloadFileCompleted;
	
				bool retry = false;

				do
				{
					try
					{
						await Task.Run(() => client.DownloadFileAsync(this.uri, fileName));
					}
					catch (AggregateException ae)
					{
						// Handle the exception
						ae.Handle(ex =>
						{
							string s = Lang.GetString("MessageWindow.Errors.GenericDLFail0") + this.fileName + "\n" + ex.Message + "\n\n" + Lang.GetString("Lang.GetString(\"MessageWindow.Errors.GenericDLFail1");

							var error = new MessageWindow(Lang.GetString("MessageWindow.Errors.GenericDLFail.Title"), s, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.RetryCancel);
							error.ShowDialog();
							retry = error.isRetry;
							return true;
						});
					}
				} while (retry == true);


			}
		}
	}
}
