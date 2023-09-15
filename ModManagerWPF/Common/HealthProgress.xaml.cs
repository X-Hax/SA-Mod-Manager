using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Windows;
using System.IO;
using System.Threading;

namespace SAModManager.Common
{
	/// <summary>
	/// Interaction logic for HealthProgress.xaml
	/// </summary>
	public partial class HealthProgress : Window
	{
		public bool Failed = false;

		public HealthProgress()
		{
			InitializeComponent();
		}

		private void UpdateStatus(string status)
		{
			HealthText.Text = status;
		}

		public void RunHealthCheck(Dictionary<int, HealthInfo> Files, List<FileStatus> Fails)
		{
			HealthProgressBar.Minimum = 0;
			HealthProgressBar.Maximum = Files.Count;

			Thread thread = new Thread(() =>
			{
				try
				{
					int i = 0;
					foreach (KeyValuePair<int, HealthInfo> file in Files)
					{
						UpdateStatus("Verifying " + file.Value.Filename);

						FileStatus status = new FileStatus(Path.GetFileName(file.Value.Filename), FileStatus.GetFileStatus(App.CurrentGame.gameDirectory, file.Value));

						if (status.Status != FileStatus.StatusValue.Good)
						{
							switch (App.CurrentGame.gameName)
							{
								case "Sonic Adventure DX":
									HealthChecker.SADXRecheck(status, file.Value);
									break;
								case "Sonic Adventure 2":
									HealthChecker.SA2Recheck(status, file.Value);
									break;
							}
						}

						if (status.Status != FileStatus.StatusValue.Good)
							Fails.Add(status);

						Dispatcher.Invoke(() => HealthProgressBar.Value = i);

						i++;
					}

					Failed = false;
					this.Dispatcher.Invoke(() => this.Close());
				}
				catch (Exception ex)
				{
					Failed = true;
					this.Dispatcher.Invoke(() => this.Close());
				}
			});

			thread.Start();
		}

	}
}