using SAModManager.Common;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
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
using System.Windows.Threading;

namespace SAModManager.Updater
{
    /// <summary>
    /// Interaction logic for LoaderManifestDialog.xaml
    /// </summary>
    public partial class InstallManagerUpdate : Window
    {
        private readonly string updatePath;
        private readonly string managerPath;
        private readonly CancellationTokenSource tokenSource = new();
        public event EventHandler CancelEvent;
        public bool done = false;

        public InstallManagerUpdate(string updatePath, string managerPath)
        {
            InitializeComponent();
            this.updatePath = updatePath;
            this.managerPath = managerPath;
        }

        public async Task InstallUpdate()
        {
            await Application.Current.Dispatcher.Invoke(async () =>
            {
                try
                {
                    string executablePath = Environment.ProcessPath;
                    await Util.MoveFile(Path.Combine(updatePath, executablePath), managerPath, true);
                    Process.Start(Path.GetFileName(executablePath), $"cleanupdate \"{updatePath}\"");
                    done = true;

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
            });

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
