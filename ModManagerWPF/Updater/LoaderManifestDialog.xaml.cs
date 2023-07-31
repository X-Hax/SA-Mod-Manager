using ModManagerCommon;
using ModManagerWPF.Common;
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

namespace ModManagerWPF.Updater
{
	/// <summary>
	/// Interaction logic for LoaderManifestDialog.xaml
	/// </summary>
	public partial class LoaderManifestDialog : Window
	{
		private readonly string updatePath;
		private readonly CancellationTokenSource tokenSource = new CancellationTokenSource();
		public event EventHandler CancelEvent;

		public LoaderManifestDialog(string updatePath)
		{
			InitializeComponent();
			this.updatePath = updatePath;

			Loaded += OnLoaded;

		}

		private void OnLoaded(object sender, RoutedEventArgs e)
		{
			DialogResult = true;

			CancellationToken token = tokenSource.Token;
			// Window is shown and loaded, perform your logic here

			try
			{
				// poor man's await Task.Run (not available in .net 4.0)
				using (var task = new Task(() =>
				{
					string newManPath = Path.Combine(updatePath, "loader.manifest");
					string oldManPath = "loader.manifest";

					//SetTaskAndStep("Parsing manifest...");
					if (token.IsCancellationRequested)
					{
						return;
					}

					List<ModManifestEntry> newManifest = ModManifest.FromFile(newManPath);

					//SetTaskAndStep("Applying manifest...");
					if (token.IsCancellationRequested)
					{
						return;
					}

					if (File.Exists(oldManPath))
					{
						List<ModManifestEntry> oldManifest = ModManifest.FromFile(oldManPath);
						List<string> oldFiles = oldManifest.Except(newManifest)
							.Select(x => x.FilePath)
							.ToList();

						foreach (string file in oldFiles)
						{
							if (File.Exists(file))
							{
								File.Delete(file);
							}
						}

						RemoveEmptyDirectories(oldManifest, newManifest);
					}

					foreach (ModManifestEntry file in newManifest)
					{
						string dir = Path.GetDirectoryName(file.FilePath);
						if (!string.IsNullOrEmpty(dir))
						{
							string newDir = dir;
							if (!Directory.Exists(newDir))
							{
								Directory.CreateDirectory(newDir);
							}
						}

						string dest = file.FilePath;

						if (File.Exists(dest))
						{
							File.Delete(dest);
						}

						File.Copy(Path.Combine(updatePath, file.FilePath), dest);
					}

					File.Copy(newManPath, oldManPath, true);

					string executablePath = Assembly.GetEntryAssembly().Location;
					Process.Start(Path.GetFileName(executablePath), $"cleanupdate \"{updatePath}\"");
				}, token))
				{
					task.Start();

					while (!task.IsCompleted && !task.IsCanceled)
					{
						Dispatcher.Invoke(DispatcherPriority.Background, new Action(() => { }));
					}

					task.Wait(token);
				}
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
		}

		private void CancelButton_Click(object sender, RoutedEventArgs e)
		{
			tokenSource.Cancel();
			CancelEvent?.Invoke(this, EventArgs.Empty);
			Close();
		}

		private static void RemoveEmptyDirectories(IEnumerable<ModManifestEntry> oldManifest, IEnumerable<ModManifestEntry> newManifest)
		{
			foreach (string dir in ModManifest.GetOldDirectories(oldManifest, newManifest))
			{
				if (Directory.Exists(dir))
				{
					// Note that this is very intentionally not recursive. If there are
					// any files left over somehow, this SHOULD be considered an Error,
					// as the goal is to exclusively remove empty directories.
					// - SF94
					Directory.Delete(dir);
				}
			}
		}

		protected override void OnClosing(CancelEventArgs e)
		{
			tokenSource.Dispose();

			base.OnClosing(e);
		}
	}
}
