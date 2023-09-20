using SAModManager.Common;
using System;
using System.Diagnostics;
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
    public partial class DownloadDialog : Window
    {
        private readonly Uri uri;
        private readonly string fileName;
        private string dest = ".SATemp";
        private readonly CancellationTokenSource tokenSource = new();
        public bool done = false;
        private DLType Type = DLType.Download;

        public enum DLType
        {
            Download,
            Install,
            Update
        }

        public DownloadDialog(Uri uri, string title, string fileName, string dest = null, DLType type = DLType.Download)
        {
            InitializeComponent();


            switch (type)
            {
                case DLType.Download:
                default:
                    Title = "Download - ";
                    break;
                case DLType.Install:
                    Title = "Install - ";
                    DLInfo.Text = "Installing";
                    break;
                case DLType.Update:
                    Title = "Update - ";
                    DLInfo.Text = "Updating";
                    break;
            }


            this.Type = type;


            Title += title;
            DLInfo.Text += " " + title + "...";
            this.fileName = fileName;
            this.uri = uri;

            if (!string.IsNullOrEmpty(dest))
            {
                this.dest = dest;
            }

            try
            {
                Directory.CreateDirectory(this.dest);

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
            await Application.Current.Dispatcher.Invoke(async () =>
            {
                switch (Type)
                {
                    case DLType.Download:
                    default:
                        DLInfo.Text = "Download ";
                        break;
                    case DLType.Install:
                        DLInfo.Text = "Install ";
                        break;
                    case DLType.Update:
                        DLInfo.Text = "Update ";
                        break;
                }

                DLInfo.Text += "Completed." + "\n Copying files...";

                await Task.Delay(200);
                if (File.Exists(fileName) && Directory.Exists(dest))
                {
                    FileInfo fileInfo = new(fileName);

                    if (fileInfo is not null && fileInfo.Length > 0)
                        await Util.MoveFile(fileName, Path.Combine(dest, fileName), true);
                }

                done = true;

                await Task.Delay(200);

                if (!string.IsNullOrEmpty(dest) && File.Exists(fileName) && dest != Path.GetDirectoryName(Environment.ProcessPath))
                    File.Delete(fileName);

     
                  this.Close();
            });
        }
        private async Task DoDownloadAsync()
        {
            using (var client = new UpdaterWebClient())
            {
                CancellationToken token = tokenSource.Token;
                client.DownloadProgressChanged += WebClient_DownloadProgressChanged;
                client.DownloadFileCompleted += WebClient_DownloadFileCompleted;

                bool retry = false;


                try
                {
                    await Task.Run(() => client.DownloadFileTaskAsync(this.uri, fileName)).ConfigureAwait(false);
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
            }
        }

        public void StartDL()
        {
            _ = Task.Run(() => DoDownloadAsync());

            BringIntoView();
            ShowDialog();
        }
    }
}
