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
        private string dest = App.tempFolder;
        private readonly CancellationTokenSource tokenSource = new();
        public bool done = false;
        private DLType Type = DLType.Download;
        public Action<Exception> DownloadFailed;
        public Action DownloadCompleted;
        private IProgress<double?> _progress;

        public enum DLType
        {
            Download,
            Install,
            Update
        }

        public DownloadDialog(Uri uri, string title, string fileName, string dest = null, DLType type = DLType.Download, bool failsafe = false)
        {
            InitializeComponent();

            switch (type)
            {
                case DLType.Download:
                default:
                    Title = Lang.GetString("Updater.DL.Dep.Download") + " - ";
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


            _progress = new Progress<double?>((v) =>
            {
                if (v.HasValue)
                {
                    TaskbarItemInfo.ProgressState = TaskbarItemProgressState.Normal;
                    DLProgress.IsIndeterminate = false;
                    TaskbarItemInfo.ProgressValue = v.Value;
                    DLProgress.Value = v.Value;
                }
                else
                {
                    DLProgress.IsIndeterminate = true;
                    TaskbarItemInfo.ProgressState = TaskbarItemProgressState.Indeterminate;
                }
            });

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
            try
            {
                var httpClient = UpdateHelper.HttpClient;

                string destination = Path.Combine(dest, fileName);
                try
                {
                    if (File.Exists(destination))
                    {
                        File.Delete(destination);
                    }
                }
                catch { }

                await httpClient.DownloadFileAsync(uri.AbsoluteUri, destination, _progress).ConfigureAwait(false);
                await Dispatcher.InvokeAsync(() =>
                {
                    DownloadCompleted = async () => await DownloadFileCompleted();
                    DownloadCompleted?.Invoke();
                });
            }
            catch (Exception ex)
            {
                await Dispatcher.InvokeAsync(() =>
                {
                    string s = Lang.GetString("MessageWindow.Errors.GenericDLFail0") + " " + this.fileName + "\n" + ex.Message + "\n\n";
                    var error = new MessageWindow(Lang.GetString("MessageWindow.Errors.GenericDLFail.Title"), s, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK);
                    error.ShowDialog();
                    DownloadFailed?.Invoke(ex);
                });
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
