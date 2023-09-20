using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Windows;
using System.IO;
using System.Threading;
using SAModManager.Configuration;
using SAModManager.Ini;
using SAModManager.Updater;
using System.Text;
using System.Data;
using Newtonsoft.Json.Linq;
using System.Windows.Threading;

namespace SAModManager.Common
{
    /// <summary>
    /// Interaction logic for HealthProgress.xaml
    /// </summary>
    /// 
    public partial class HealthChecker : Window
    {
		public static Dictionary<int, HealthInfo> Files { get; set; } = new();

		public static List<FileStatus> Fails { get; set; } = new();

		public bool Failed = false;
        SetGame game = SetGame.SADX;

        public HealthChecker(SetGame game)
        {
            InitializeComponent();
            this.game = game;
            Loaded += OnLoaded;
            string title = Lang.GetString("HealthProgress.Title");
            HealthText.Text = title;
            Title = title;
        }

        public class HealthInfo
        {
            [IniName("name")]
            public string Filename { get; set; }

            [IniName("hash")]
            [IniCollection(IniCollectionMode.NoSquareBrackets, StartIndex = 1)]
            public List<string> Hashes { get; set; }
        }

        public class FileStatus
        {
            public enum StatusValue
            {
                Good = 0,
                NotFound = 1,
                Modified = 2,
                NotInList = 3,
            }

            public string Filename { get; set; }

            public StatusValue Status { get; set; }

            public static StatusValue GetFileStatus(string directory, HealthInfo file)
            {
                string filename = Path.Combine(directory, file.Filename);

                if (File.Exists(filename))
                {
                    string currenthash = ModManifestGenerator.GetFileHash(filename);
 
                    foreach (string hash in file.Hashes)
                    {
                        if (hash == currenthash)
                        {
                            return StatusValue.Good;
                        } 
                    }

                    return StatusValue.Modified;
                }
                else
                    return StatusValue.NotFound;
            }

            public FileStatus(string name, StatusValue status)
            {
                Filename = name;
                Status = status;
            }
        }

        public static void RecheckStatus(string newfile, FileStatus status, HealthInfo file)
        {
            file.Filename = newfile;
            status.Status = FileStatus.GetFileStatus(App.CurrentGame.gameDirectory, file);
        }

        public static void SADXRecheck(FileStatus status, HealthInfo file)
        {
            switch (Path.GetFileName(status.Filename))
            {
                case "chrmodels.dll":
                    RecheckStatus("system/chrmodels_orig.dll", status, file);
                    break;
                case "re-jp.mpg":
                    RecheckStatus("system/re-jp.sfd", status, file);
                    break;
                case "re-us.mpg":
                    RecheckStatus("system/re-us.sfd", status, file);
                    break;
                case "sa1.mpg":
                    RecheckStatus("system/sa1.sfd", status, file);
                    break;
                case "sa2.mpg":
                    RecheckStatus("system/sa2.sfd", status, file);
                    break;
                case "sa3.mpg":
                    RecheckStatus("system/sa3.sfd", status, file);
                    break;
                case "sa4.mpg":
                    RecheckStatus("system/sa4.sfd", status, file);
                    break;
                case "sa5.mpg":
                    RecheckStatus("system/sa5.sfd", status, file);
                    break;
                case "sa6.mpg":
                    RecheckStatus("system/sa6.sfd", status, file);
                    break;
                case "sa7.mpg":
                    RecheckStatus("system/sa7.sfd", status, file);
                    break;
                case "sa8.mpg":
                    RecheckStatus("system/sa8.sfd", status, file);
                    break;
            }
        }

        public static void SA2Recheck(FileStatus status, HealthInfo file)
        {
           /* switch (Path.GetFileName(status.Filename))
            {

            }*/
        }

        private async void OnLoaded(object sender, RoutedEventArgs e)
        {
            using (var task = new Task(() =>
            {
                switch (game)
                {
                    default:
                    case SetGame.SADX:
                        using (MemoryStream stream = new(Properties.Resources.SADXFileHashes))
                        {
                            Files = IniSerializer.Deserialize<Dictionary<int, HealthInfo>>(IniFile.Load(stream));
                        }
                        break;
                    case SetGame.SA2:
                        /*using (MemoryStream stream = new MemoryStream(Properties.Resources.SA2FileHashes))
                        {
                            Files = IniSerializer.Deserialize<Dictionary<int, HealthInfo>>(IniFile.Load(stream));
                        }*/
                        break;
                }

                try
                {

                    foreach (KeyValuePair<int, HealthInfo> file in Files)
                    {
                        Application.Current.Dispatcher.Invoke(() =>
                        {
                            HealthProgressBar.Value = Util.SetProgress(file.Key / (double)Files.Count);
                            UpdateStatus(file.Value.Filename);
                        });

                        FileStatus status = new(Path.GetFileName(file.Value.Filename), FileStatus.GetFileStatus(App.CurrentGame.gameDirectory, file.Value));

                        if (status.Status != FileStatus.StatusValue.Good)
                        {
                            switch (game)
                            {
                                case SetGame.SADX:
                                    SADXRecheck(status, file.Value);
                                    break;
                                case SetGame.SA2:
                                    SA2Recheck(status, file.Value);
                                    break;
                            }
                        }

                        if (status.Status != FileStatus.StatusValue.Good)
                            Fails.Add(status);
                    }
                }
                catch
                {
					MessageWindow exception = new(Lang.GetString("MessageWindow.Errors.HealthCheck.UnknownError.Title"), Lang.GetString("MessageWindow.Errors.HealthCheck.UnknownError"),
						type: MessageWindow.WindowType.IconMessage, icon: MessageWindow.Icons.Error, button: MessageWindow.Buttons.OK);

					exception.ShowDialog();
				}

                if (Fails.Count > 0)
                {
                    StringBuilder sb = new StringBuilder();
                    foreach (FileStatus file in Fails)
                    {
                        switch (file.Status)
                        {
                            case FileStatus.StatusValue.Modified:
                                sb.AppendLine(file.Filename + " " + Lang.GetString("HealthProgress.Files.Modified"));
                                break;
                            case FileStatus.StatusValue.NotFound:
                                sb.AppendLine(file.Filename +  " " + Lang.GetString("HealthProgress.Files.Missing"));
                                break;
                        }
                    }

                    Application.Current.Dispatcher.Invoke(() =>
                    {
                        MessageWindow failedFiles = new(Lang.GetString("MessageWindow.Information.HealthCheck.FailedFiles.Title"), 
							Lang.GetString("MessageWindow.Information.HealthCheck.FailedFiles") + "\n\n" +
                        sb.ToString(), type: MessageWindow.WindowType.IconMessage, icon: MessageWindow.Icons.Information, button: MessageWindow.Buttons.OK);

                        failedFiles.ShowDialog();
                    });
                }
                else
                {
                    Application.Current.Dispatcher.Invoke(() =>
                    {
                        MessageWindow success = new(Lang.GetString("MessageWindow.Information.HealthCheck.Verified.Title"), Lang.GetString("MessageWindow.Information.HealthCheck.Verified"),
                        type: MessageWindow.WindowType.IconMessage, icon: MessageWindow.Icons.Information, button: MessageWindow.Buttons.OK);

                        success.ShowDialog();
                    });

                }
            }))
            {
                task.Start();

                while (!task.IsCompleted && !task.IsCanceled)
                {
                    await Dispatcher.Yield(DispatcherPriority.Background);
                }

                Fails.Clear();
                DialogResult = true;
            }
        }

        private void UpdateStatus(string status)
        {
            HealthText.Text = status;
        }

    }
}