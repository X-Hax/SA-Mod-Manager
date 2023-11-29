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
using System.Windows.Shell;

namespace SAModManager.Common
{
    /// <summary>
    /// Interaction logic for ManagerUpdate.xaml
    /// </summary>
    public partial class ManagerUpdate : Window
    {
        private readonly string URL;
        private readonly string destPath;
        private readonly string fileName;
        public Action DownloadCompleted;
        public Action<Exception> DownloadFailed;
        private IProgress<double?> _progress;


        public ManagerUpdate(string url, string updatePath, string fileName)
        {
            InitializeComponent();
            this.URL = url;
            this.fileName = fileName;
            this.destPath = Path.Combine(updatePath, fileName);

            _progress = new Progress<double?>((v) =>
            {
                if (v.HasValue)
                {
                    TaskbarItemInfo.ProgressState = TaskbarItemProgressState.Normal;
                    UpdateProgress.IsIndeterminate = false;
                    TaskbarItemInfo.ProgressValue = v.Value;
                    UpdateProgress.Value = v.Value;
                }
                else
                {
                    UpdateProgress.IsIndeterminate = true;
                    TaskbarItemInfo.ProgressState = TaskbarItemProgressState.Indeterminate;
                }
            });
        }

        public static async Task DownloadManagerCompleted(string tempFolderPath, string fileName)
        {
            string dest = Path.Combine(tempFolderPath, fileName);

            //once dl is finished, move manager zip in .SATemp folder
            if (File.Exists(dest) && tempFolderPath is not null)
            {
                await Task.Delay(100);
                //extract Manager zip
                await Util.Extract(dest, tempFolderPath);
                //delete zip
                File.Delete(dest);

                await Task.Delay(50);
                string newExec = Path.Combine(tempFolderPath, Path.GetFileName(Environment.ProcessPath));

                if (File.Exists(newExec))
                {
                    Process.Start(newExec, $"doupdate \"{tempFolderPath}\" \"{Environment.ProcessPath}\"");
                    App.Current.Shutdown();
                }
                else
                {
                    throw new Exception("Failed to Extract or Open Manager Update.");
                }
            }
            else
            {
                throw new Exception("Failed to Extract or Open Manager Update. File not found");
            }
        }

        public void StartManagerDL()
        {
            _ = Task.Run(() => DoManagerDownload());

            BringIntoView();
            ShowDialog();
        }

        public async Task DoManagerDownload()
        {
            using (var client = UpdateHelper.HttpClient)
            {

                try
                {
                    await client.DownloadFileAsync(URL, destPath, _progress).ConfigureAwait(false);
                    await Dispatcher.InvokeAsync(() =>
                    {
                        Close();
                        DownloadCompleted?.Invoke();
                    });
                }
                catch (Exception ex)
                {
                    await Dispatcher.InvokeAsync(() =>
                    {
                        // Handle the exception
                        string s = Lang.GetString("MessageWindow.Errors.GenericDLFail0") + " " + this.fileName + "\n" + ex.Message + "\n\n";
                        var error = new MessageWindow(Lang.GetString("MessageWindow.Errors.GenericDLFail.Title"), s, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK);
                        error.ShowDialog();
                        Close();
                        DownloadFailed?.Invoke(ex);

                    });
                }
            }
        }
    }
}
