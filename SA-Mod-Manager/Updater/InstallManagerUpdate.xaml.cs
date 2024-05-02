using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using SAModManager.UI;

namespace SAModManager.Updater
{
    /// <summary>
    /// Interaction logic for LoaderManifestDialog.xaml
    /// </summary>
    public partial class InstallManagerUpdate : Window
    {
        private readonly string updatePath;
        private readonly string managerExePath;
        private readonly CancellationTokenSource tokenSource = new();
        public event EventHandler CancelEvent;

        public InstallManagerUpdate(string updatePath, string managerPath)
        {
            InitializeComponent();
            this.updatePath = updatePath;
            this.managerExePath = managerPath;
        }

        public async Task InstallUpdate()
        {
            string executablePath = Environment.ProcessPath;
            Logger.Log("Now Replacing old exe...");
            try
            {
                await Util.MoveFile(Path.Combine(updatePath, executablePath), managerExePath, true);
                Process.Start(new ProcessStartInfo { FileName = managerExePath, UseShellExecute = true });

            }
            catch (AggregateException ae)
            {
                ae.Handle(ex =>
                {
                    string error = Lang.GetString("MessageWindow.Errors.UpdateFailed") + $"\r\n{ex.Message}";
                    new MessageWindow(Lang.GetString("MessageWindow.Errors.UpdateFailed.Title"), error, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error).ShowDialog();
                    return true;
                });
            }

            this.Close();

        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            tokenSource.Cancel();
            CancelEvent?.Invoke(this, EventArgs.Empty);
            Close();
        }

        protected override void OnClosing(CancelEventArgs e)
        {
            tokenSource.Dispose();
        }
    }
}
