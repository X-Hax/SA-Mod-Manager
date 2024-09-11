using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Net.Http;
using System.Security.Policy;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Shell;
using SAModManager.UI;

namespace SAModManager.Updater
{

    /// <summary>
    /// Interaction logic for GenericUpdateDialog.xaml
    /// </summary>
    public partial class DownloadDialog : Window
    {
        private List<DownloadInfo> dlList;

        private readonly CancellationTokenSource tokenSource = new();

        public Action DownloadCompleted;
        public event Action<Exception> DownloadFailed;
        private IProgress<double?> _progress;
        private IProgress<double?> _progressOverall;
        public int errorCount = 0;

        public enum DLType
        {
            Download,
            Install,
            Update
        }
             
        public bool isInError(string name)
        {
            foreach (var dl in dlList)
            {
                if (dl.Name == name && string.IsNullOrEmpty(dl.errorMessage) == false)
                {
                    return true;
                }
            }

            return false;
        }

        public void DisplayFailedDownloadList()
        {
            string error = Lang.GetString("MessageWindow.Errors.GenericDLFail0") + ":\n";
            foreach (var dl in dlList)
            {
                if (Util.IsStringValid(dl.errorMessage))
                    error += "\n" + dl.errorMessage;
            }

            new MessageWindow(Lang.GetString("MessageWindow.Errors.GenericDLFail.Title"), error, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error).ShowDialog();
            this.Close();
        }
        
        public DownloadDialog(List<DownloadInfo> dllist)
        {
            InitializeComponent();


            this.dlList = dllist;


            ProgressOverall.Maximum = this.dlList.Count;

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

            _progressOverall = new Progress<double?>((v) =>
            {
                if (v.HasValue)
                {
                    TaskbarItemInfo.ProgressState = TaskbarItemProgressState.Normal;
                    ProgressOverall.IsIndeterminate = false;
                    TaskbarItemInfo.ProgressValue = v.Value;
                    ProgressOverall.Value = v.Value;
                }
                else
                {
                    ProgressOverall.IsIndeterminate = true;
                    TaskbarItemInfo.ProgressState = TaskbarItemProgressState.Indeterminate;
                }
            });


        }

        private async Task DownloadFileCompleted(DownloadInfo dl)
        {
            await Dispatcher.InvokeAsync(() =>
            {
                curFile.Text = "";
                switch (dl.Type)
                {
                    case DLType.Download:
                    default:
                        DLInfo.Text = Lang.GetString("Updater.DL.Dep.Download") + " " + dl.FileName;
                        break;
                    case DLType.Install:
                        DLInfo.Text = Lang.GetString("Updater.DL.Dep.Install") + " " + dl.FileName;
                        break;
                    case DLType.Update:
                        DLInfo.Text = Lang.GetString("Updater.DL.Dep.Update") + " " + dl.FileName;
                        break;
                }

                DLInfo.Text += " " + Lang.GetString("Updater.DL.Dep.Completed") + "\n";
                curFile.Text = Lang.GetString("Updater.DL.Dep.Copying");
            });

            await Task.Delay(500);

            if (File.Exists(dl.FileName) && Directory.Exists(dl.Folder))
            {
                FileInfo fileInfo = new(dl.FileName);

                if (fileInfo is not null && fileInfo.Length > 0)
                    await Util.MoveFile(dl.FileName, Path.Combine(dl.Folder, dl.FileName), true);
            }

            await Task.Delay(200);

            if (Util.IsStringValid(dl.Folder) && File.Exists(dl.FileName) && dl.Folder != Path.GetDirectoryName(Environment.ProcessPath))
                File.Delete(dl.FileName);

        }

        public void DisplayDownloadFailedMSG(Exception ex, string fileName)
        {
            string s = Lang.GetString("MessageWindow.Errors.GenericDLFail0") + " " + fileName + "\n" + ex?.Message + "\n\n";
            var error = new MessageWindow(Lang.GetString("MessageWindow.Errors.GenericDLFail.Title"), s, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK);
            error.ShowDialog();
        }

        public static void DisplayGenericDownloadFailedMSG(Exception ex)
        {
            string s = Lang.GetString("MessageWindow.Errors.GenericDLFail0") + "\n" + ex?.Message + "\n\n";
            var error = new MessageWindow(Lang.GetString("MessageWindow.Errors.GenericDLFail.Title"), s, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK);
            error.ShowDialog();
        }

        private void UpdateProgressText(string current, string count)
        {
            try
            {
                // Update UI on the UI thread using Dispatcher
                Dispatcher.Invoke(() =>
                {
                    ProgressTxt.Text = current + " " + "/" + " " + count;
                });
            }
            catch { }
        }

        private void SetupDownload(int index)
        {
            if (dlList.Count <= index)
                return;

            Dispatcher.Invoke(() =>
            {
                switch (dlList[index].Type)
                {
                    case DLType.Download:
                    default:
                        Title = Lang.GetString("Updater.DL.Dep.Download") + " - ";
                        DLInfo.Text = Lang.GetString("Updater.DL.Dep.Downloading") + "...";
                        break;
                    case DLType.Install:
                        Title = Lang.GetString("Updater.DL.Dep.Install") + " - ";
                        DLInfo.Text = Lang.GetString("Updater.DL.Dep.Installing") + "...";
                        break;
                    case DLType.Update:
                        Title = Lang.GetString("Updater.DL.Dep.Update") + " - ";
                        DLInfo.Text = Lang.GetString("Updater.DL.Dep.Updating") + "..."; 
                        break;
                }

                Title += dlList[index].Name;
                curFile.Text = dlList[index].FileName + "...";
            });

            try
            {
                Util.CreateSafeDirectory(dlList[index].Folder);

            }
            catch { }
        }

        private async Task DoDownloadAsync()
        {
            int count = 0;

            foreach (var dl in dlList)
            {
                try
                {
                    var httpClient = UpdateHelper.HttpClient;
                    string destination = Path.Combine(dl.Folder, dl.FileName);
                    SetupDownload(count);

                    try
                    {
                        if (File.Exists(destination))
                        {
                            File.Delete(destination);
                        }
                    }
                    catch { }

                    await httpClient.DownloadFileAsync(dl.uri.AbsoluteUri, destination, _progress).ConfigureAwait(false);
                    await DownloadFileCompleted(dl);
                    count++;
                    try
                    {
                        _progressOverall?.Report(count);
                        UpdateProgressText(count.ToString(), dlList.Count.ToString());

                    }
                    catch
                    { }

                }
                catch (Exception ex)
                {
                    await Dispatcher.InvokeAsync(() =>
                    {
                        errorCount++;
                        DownloadFailed?.Invoke(ex);
                        dl.errorMessage = ("Error: " + errorCount + " " + ex.Message + "\n");
                    });
                }
            }
 

            await Dispatcher.InvokeAsync(() =>
            {
                Close();
                DownloadCompleted?.Invoke();
            });
        }

        public void StartDL()
        {
            _ = Task.Run(() => DoDownloadAsync());

            BringIntoView();
            ShowDialog();
        }
    }
}
