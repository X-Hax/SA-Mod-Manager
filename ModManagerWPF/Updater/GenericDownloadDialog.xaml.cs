using ModManagerCommon;
using ModManagerWPF.Common;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Security.Policy;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace ModManagerWPF.Updater
{

	/// <summary>
	/// Interaction logic for GenericUpdateDialog.xaml
	/// </summary>
	public partial class GenericDownloadDialog : Window
	{
		private readonly Uri uri;
		private readonly string fileName;
		private readonly string dest = "SATemp";

		private readonly CancellationTokenSource tokenSource = new();

		public GenericDownloadDialog(Uri uri, string title, string fileName)
		{
			InitializeComponent();

			Title = "Download - " + title;
			this.fileName = fileName;
			this.uri = uri;

			try
			{
				if (!Directory.Exists(dest))
				{
					Directory.CreateDirectory(dest);
				}
			}
			catch { }

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

			await Task.Delay(500);
			if (File.Exists(fileName)) 
			{ 
				Util.MoveFile(fileName, Path.Combine(dest, fileName));
			}

			await Task.Delay(1000);
			Application.Current.Dispatcher.Invoke(() =>
			{
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
