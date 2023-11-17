using SAModManager.Common;
using System;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Net.Http;
using System.Security.Policy;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Shell;

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
        public Action DownloadCompleted;


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
                    Title =  Lang.GetString("Updater.DL.Dep.Download") + " - ";
                    break;
                case DLType.Install:
                    Title = Lang.GetString("Updater.DL.Dep.Install") + " - ";
                    DLInfo.Text = Lang.GetString("Updater.DL.Dep.Installing");
                    break;
                case DLType.Update:
                    Title = Lang.GetString("Updater.DL.Dep.Update") + " - ";
                    DLInfo.Text = Lang.GetString("Updater.DL.Dep.Updating");
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
                        DLInfo.Text = Lang.GetString("Updater.DL.Dep.Download") + " ";
                        break;
                    case DLType.Install:
                        DLInfo.Text = Lang.GetString("Updater.DL.Dep.Install") + " ";
                        break;
                    case DLType.Update:
                        DLInfo.Text = Lang.GetString("Updater.DL.Dep.Update") + " ";
                        break;
                }

                DLInfo.Text += Lang.GetString("Updater.DL.Dep.Completed") + "\n" + Lang.GetString("Updater.DL.Dep.Copying");

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

        private async Task DownloadFileCompleted()
        {

            switch (Type)
            {
                case DLType.Download:
                default:
                    DLInfo.Text = Lang.GetString("Updater.DL.Dep.Download") + " ";
                    break;
                case DLType.Install:
                    DLInfo.Text = Lang.GetString("Updater.DL.Dep.Install") + " ";
                    break;
                case DLType.Update:
                    DLInfo.Text = Lang.GetString("Updater.DL.Dep.Update") + " ";
                    break;
            }

            DLInfo.Text += Lang.GetString("Updater.DL.Dep.Completed") + "\n" + Lang.GetString("Updater.DL.Dep.Copying");
            DLProgress.Value = DLProgress.Maximum;
            await Task.Delay(1000);
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

            Close();
        }

        private async Task DoDownloadAsync()
        {
            bool retry = false;

            try
            {
                var httpClient = new HttpClient();

                httpClient.DefaultRequestHeaders.TryAddWithoutValidation("User-Agent", "SA-Mod-Manager");

                CancellationToken token = tokenSource.Token;

                string destination = Path.Combine(dest, fileName);
                try
                {
                    if (File.Exists(destination))
                    {
                        File.Delete(destination);
                    }
                }
                catch { }
   
                await DownloadFileAsync(httpClient, uri.AbsoluteUri, destination).ConfigureAwait(false);
                await Dispatcher.InvokeAsync(() =>
                {
                    DownloadCompleted = async () => await DownloadFileCompleted();
                    DownloadCompleted?.Invoke();
                });
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

        public async Task DownloadFileAsync(HttpClient httpClient, string url, string destinationFilePath)
        {
            using HttpResponseMessage response = await httpClient.GetAsync(url, HttpCompletionOption.ResponseHeadersRead);
            response.EnsureSuccessStatusCode();

            using Stream contentStream = await response.Content.ReadAsStreamAsync();
            using Stream fileStream = new FileStream(destinationFilePath, FileMode.Create, FileAccess.Write, FileShare.None);
            byte[] buffer = new byte[8192];
            int bytesRead;
            long totalBytesRead = 0;
            long totalBytes = response.Content.Headers.ContentLength ?? -1;

            while ((bytesRead = await contentStream.ReadAsync(buffer, 0, buffer.Length)) > 0)
            {
                await fileStream.WriteAsync(buffer.AsMemory(0, bytesRead));
                totalBytesRead += bytesRead;

                if (totalBytes != -1)
                {
                    int percentage = (int)((totalBytesRead * 100) / totalBytes);

                    // Use Dispatcher to update the ProgressBar on the UI thread
                    Dispatcher.Invoke(() => DLProgress.Value = percentage);
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
