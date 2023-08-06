using SAModManager.Updater;
using System.Threading.Tasks;
using System.Threading;
using System;
using System.Windows;
using System.Security.Policy;
using System.Net;
using System.IO;
using SevenZipExtractor;
using System.Diagnostics;

namespace SAModManager.Common
{
	/// <summary>
	/// Interaction logic for ManagerUpdate.xaml
	/// </summary>
	public partial class ManagerUpdate : Window
    {
        private string URL;
        private string tempFolderPath;
        private string fileName;
        private readonly CancellationTokenSource tokenSource = new();

        public ManagerUpdate(string url, string updatePath, string fileName)
        {
            InitializeComponent();
            this.URL = url;
            this.fileName = fileName;
            this.tempFolderPath = updatePath;
        }

        private void WebClient_DownloadProgressChanged(object sender, DownloadProgressChangedEventArgs e)
        {
            Application.Current.Dispatcher.Invoke(() =>
            {
                UpdateProgress.Value = e.ProgressPercentage;
            });
        }

        private async void WebClient_DownloadFileCompleted(object sender, System.ComponentModel.AsyncCompletedEventArgs e)
        {
            Application.Current.Dispatcher.Invoke(() =>
            {
                UpdateInfo.Text = "Download completed." + "\n Installing New Version...";

            });

            await Application.Current.Dispatcher.Invoke(async () =>
            {
                string dest = Path.Combine(tempFolderPath, fileName);

                //once dl is finished, move manager zip in .SATemp folder
                if (File.Exists(fileName) && tempFolderPath is not null)
                {
                    await Util.MoveFile(fileName, dest, true);

                    await Task.Delay(100);
                    //extract Manager zip
                    using (ArchiveFile archiveFile = new(dest))
                    {
                        archiveFile.Extract(tempFolderPath);
                    }

                    //delete zip
                    File.Delete(dest);

                    string newExec = Path.Combine(tempFolderPath, Path.GetFileName(Environment.ProcessPath));
                    DialogResult = true;
                    Process.Start(new ProcessStartInfo(Path.Combine(tempFolderPath, Path.GetFileName(Environment.ProcessPath)), $"doupdate \"{tempFolderPath}\"")
                    {
                        UseShellExecute = true,
                    });
                }
            });

        }

        public async Task StartManagerDL()
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
                        await Task.Run(() => client.DownloadFileTaskAsync(URL, fileName));
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
