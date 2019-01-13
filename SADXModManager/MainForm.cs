using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Reflection;
using System.Security.Cryptography;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using IniFile;
using ModManagerCommon;
using ModManagerCommon.Forms;
using SADXModManager.Forms;
using SADXModManager.Properties;

namespace SADXModManager
{
	public partial class MainForm : Form
	{
		public MainForm()
		{
			this.Font = SystemFonts.MessageBoxFont;
			InitializeComponent();

			// WORKAROUND: Windows 7's system fonts don't have
			// U+2912 or U+2913. Use Cambria instead.
			// TODO: Check the actual font to see if it has the glyphs.
			Font boldFont = null;
			OperatingSystem os = Environment.OSVersion;
			if ((os.Platform == PlatformID.Win32NT || os.Platform == PlatformID.Win32Windows) &&
				(os.Version.Major < 6 || (os.Version.Major == 6 && os.Version.Minor < 2)))
			{
				// Windows 7 or earlier.
				// TODO: Make sure this font exists.
				// NOTE: U+2912 and U+2913 are missing in Bold, so use Regular.
				boldFont = new Font("Cambria", this.Font.Size * 1.25f, FontStyle.Regular);
			}
			else
			{
				// Newer than Windows 7, or not Windows.
				// Use the default font.
				boldFont = new Font(this.Font.FontFamily, this.Font.Size * 1.25f, FontStyle.Bold);
			}

			modTopButton.Font = boldFont;
			modUpButton.Font = boldFont;
			modDownButton.Font = boldFont;
			modBottomButton.Font = boldFont;
		}

		private bool checkedForUpdates;

		const string updatePath = "mods/.updates";
		const string datadllpath = "system/CHRMODELS.dll";
		const string datadllorigpath = "system/CHRMODELS_orig.dll";
		const string loaderinipath = "mods/SADXModLoader.ini";
		const string loaderdllpath = "mods/SADXModLoader.dll";
		SADXLoaderInfo loaderini;
		Dictionary<string, SADXModInfo> mods;
		const string codelstpath = "mods/Codes.lst";
		const string codexmlpath = "mods/Codes.xml";
		const string codedatpath = "mods/Codes.dat";
		const string patchdatpath = "mods/Patches.dat";
		CodeList mainCodes;
		List<Code> codes;
		bool installed;
		bool suppressEvent;

		readonly ModUpdater modUpdater = new ModUpdater();
		BackgroundWorker updateChecker;
		private bool manualModUpdate;

		private static bool UpdateTimeElapsed(UpdateUnit unit, int amount, DateTime start)
		{
			if (unit == UpdateUnit.Always)
			{
				return true;
			}

			TimeSpan span = DateTime.UtcNow - start;

			switch (unit)
			{
				case UpdateUnit.Hours:
					return span.TotalHours >= amount;

				case UpdateUnit.Days:
					return span.TotalDays >= amount;

				case UpdateUnit.Weeks:
					return span.TotalDays / 7.0 >= amount;

				default:
					throw new ArgumentOutOfRangeException(nameof(unit), unit, null);
			}
		}

		private static void SetDoubleBuffered(Control control, bool enable)
		{
			PropertyInfo doubleBufferPropertyInfo = control.GetType().GetProperty("DoubleBuffered", BindingFlags.Instance | BindingFlags.NonPublic);
			doubleBufferPropertyInfo?.SetValue(control, enable, null);
		}

		private void MainForm_Load(object sender, EventArgs e)
		{
			// Try to use TLS 1.2
			try { ServicePointManager.SecurityProtocol = (SecurityProtocolType)3072; } catch { }
			if (!Debugger.IsAttached)
				Environment.CurrentDirectory = Application.StartupPath;
			SetDoubleBuffered(modListView, true);
			loaderini = File.Exists(loaderinipath) ? IniSerializer.Deserialize<SADXLoaderInfo>(loaderinipath) : new SADXLoaderInfo();

			try
			{
				if (File.Exists(codelstpath))
					mainCodes = CodeList.Load(codelstpath);
				else if (File.Exists(codexmlpath))
					mainCodes = CodeList.Load(codexmlpath);
				else
					mainCodes = new CodeList();
			}
			catch (Exception ex)
			{
				MessageBox.Show(this, $"Error loading code list: {ex.Message}", "SADX Mod Loader", MessageBoxButtons.OK, MessageBoxIcon.Warning);
				mainCodes = new CodeList();
			}

			LoadModList();

			for (int i = 0; i < Screen.AllScreens.Length; i++)
			{
				Screen s = Screen.AllScreens[i];
				screenNumComboBox.Items.Add($"{ i + 1 } { s.DeviceName } ({ s.Bounds.Location.X },{ s.Bounds.Y })"
					+ $" { s.Bounds.Width }x{ s.Bounds.Height } { s.BitsPerPixel } bpp { (s.Primary ? "Primary" : "") }");
			}

			consoleCheckBox.Checked             = loaderini.DebugConsole;
			screenCheckBox.Checked              = loaderini.DebugScreen;
			fileCheckBox.Checked                = loaderini.DebugFile;
			disableCDCheckCheckBox.Checked      = loaderini.DisableCDCheck;
			checkVsync.Checked                  = loaderini.EnableVsync;
			horizontalResolution.Enabled        = !loaderini.ForceAspectRatio;
			horizontalResolution.Value          = Math.Max(horizontalResolution.Minimum, Math.Min(horizontalResolution.Maximum, loaderini.HorizontalResolution));
			verticalResolution.Value            = Math.Max(verticalResolution.Minimum, Math.Min(verticalResolution.Maximum, loaderini.VerticalResolution));
			checkUpdateStartup.Checked          = loaderini.UpdateCheck;
			checkUpdateModsStartup.Checked      = loaderini.ModUpdateCheck;
			comboUpdateFrequency.SelectedIndex  = (int)loaderini.UpdateUnit;
			numericUpdateFrequency.Value        = loaderini.UpdateFrequency;

			suppressEvent = true;
			forceAspectRatioCheckBox.Checked = loaderini.ForceAspectRatio;
			checkScaleHud.Checked = loaderini.ScaleHud;
			suppressEvent = false;

			comboBackgroundFill.SelectedIndex = loaderini.BackgroundFillMode;
			comboFmvFill.SelectedIndex = loaderini.FmvFillMode;

			windowedFullscreenCheckBox.Checked = loaderini.WindowedFullscreen;
			forceMipmappingCheckBox.Checked    = loaderini.AutoMipmap;
			forceTextureFilterCheckBox.Checked = loaderini.TextureFilter;
			pauseWhenInactiveCheckBox.Checked  = loaderini.PauseWhenInactive;
			stretchFullscreenCheckBox.Checked  = loaderini.StretchFullscreen;

			int screenNum = Math.Min(Screen.AllScreens.Length, loaderini.ScreenNum);

			screenNumComboBox.SelectedIndex  = screenNum;
			customWindowSizeCheckBox.Checked = windowHeight.Enabled = maintainWindowAspectRatioCheckBox.Enabled = loaderini.CustomWindowSize;
			windowWidth.Enabled              = loaderini.CustomWindowSize && !loaderini.MaintainWindowAspectRatio;
			Rectangle rect                   = Screen.PrimaryScreen.Bounds;

			foreach (Screen screen in Screen.AllScreens)
				rect = Rectangle.Union(rect, screen.Bounds);

			windowWidth.Maximum  = rect.Width;
			windowWidth.Value    = Math.Max(windowWidth.Minimum, Math.Min(rect.Width, loaderini.WindowWidth));
			windowHeight.Maximum = rect.Height;
			windowHeight.Value   = Math.Max(windowHeight.Minimum, Math.Min(rect.Height, loaderini.WindowHeight));

			suppressEvent = true;
			maintainWindowAspectRatioCheckBox.Checked = loaderini.MaintainWindowAspectRatio;
			suppressEvent = false;

			checkWindowResize.Checked = loaderini.ResizableWindow;
		}

		private void HandleUri(string uri)
		{
			if (WindowState == FormWindowState.Minimized)
			{
				WindowState = FormWindowState.Normal;
			}

			Activate();

			var fields = uri.Substring("sadxmm:".Length).Split(',');

			// TODO: lib-ify
			string itemType = fields.FirstOrDefault(x => x.StartsWith("gb_itemtype", StringComparison.InvariantCultureIgnoreCase));
			itemType = itemType.Substring(itemType.IndexOf(":") + 1);

			string itemId = fields.FirstOrDefault(x => x.StartsWith("gb_itemid", StringComparison.InvariantCultureIgnoreCase));
			itemId = itemId.Substring(itemId.IndexOf(":") + 1);

			var dummyInfo = new ModInfo();

			GameBananaItem gbi = GameBananaItem.Load(itemType, long.Parse(itemId));

			dummyInfo.Name = gbi.Name;

			dummyInfo.Author = gbi.OwnerName;

			DialogResult result = MessageBox.Show(this, $"Do you want to install mod \"{dummyInfo.Name}\" by {dummyInfo.Author} from {new Uri(fields[0]).DnsSafeHost}?", "Mod Download", MessageBoxButtons.YesNo, MessageBoxIcon.Question);

			if (result != DialogResult.Yes)
			{
				return;
			}

			#region create update folder
			do
			{
				try
				{
					result = DialogResult.Cancel;
					if (!Directory.Exists(updatePath))
					{
						Directory.CreateDirectory(updatePath);
					}
				}
				catch (Exception ex)
				{
					result = MessageBox.Show(this, "Failed to create temporary update directory:\n" + ex.Message
												   + "\n\nWould you like to retry?", "Directory Creation Failed", MessageBoxButtons.RetryCancel);
				}
			} while (result == DialogResult.Retry);
			#endregion

			string dummyPath = dummyInfo.Name;

			foreach (char c in Path.GetInvalidFileNameChars())
			{
				dummyPath = dummyPath.Replace(c, '_');
			}

			dummyPath = Path.Combine("mods", dummyPath);

			var updates = new List<ModDownload>
			{
				new ModDownload(dummyInfo, dummyPath, fields[0], null, 0)
			};

			using (var progress = new ModDownloadDialog(updates, updatePath))
			{
				progress.ShowDialog(this);
			}

			do
			{
				try
				{
					result = DialogResult.Cancel;
					Directory.Delete(updatePath, true);
				}
				catch (Exception ex)
				{
					result = MessageBox.Show(this, "Failed to remove temporary update directory:\n" + ex.Message
												   + "\n\nWould you like to retry? You can remove the directory manually later.",
						"Directory Deletion Failed", MessageBoxButtons.RetryCancel);
				}
			} while (result == DialogResult.Retry);

			LoadModList();
		}

		private void MainForm_Shown(object sender, EventArgs e)
		{
			if (CheckForUpdates())
				return;

			if (!File.Exists(datadllpath))
			{
				MessageBox.Show(this, "CHRMODELS.dll could not be found.\n\nCannot determine state of installation.",
					Text, MessageBoxButtons.OK, MessageBoxIcon.Warning);
				installButton.Hide();
			}
			else if (File.Exists(datadllorigpath))
			{
				installed = true;
				installButton.Text = "Uninstall loader";
				using (MD5 md5 = MD5.Create())
				{
					byte[] hash1 = md5.ComputeHash(File.ReadAllBytes(loaderdllpath));
					byte[] hash2 = md5.ComputeHash(File.ReadAllBytes(datadllpath));

					if (!hash1.SequenceEqual(hash2))
					{

						DialogResult result = MessageBox.Show(this, "Installed loader DLL differs from copy in mods folder."
							+ "\n\nDo you want to overwrite the installed copy?", Text, MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

						if (result == DialogResult.Yes)
							File.Copy(loaderdllpath, datadllpath, true);
					}
				}
			}

			List<string> uris = Program.UriQueue.GetUris();

			foreach (string str in uris)
			{
				HandleUri(str);
			}

			Program.UriQueue.UriEnqueued += UriQueueOnUriEnqueued;

			CheckForModUpdates();

			// If we've checked for updates, save the modified
			// last update times without requiring the user to
			// click the save button.
			if (checkedForUpdates)
			{
				IniSerializer.Serialize(loaderini, loaderinipath);
			}
		}

		private void UriQueueOnUriEnqueued(object sender, OnUriEnqueuedArgs args)
		{
			args.Handled = true;

			if (InvokeRequired)
			{
				Invoke((Action<object, OnUriEnqueuedArgs>)UriQueueOnUriEnqueued, sender, args);
				return;
			}

			HandleUri(args.Uri);
		}

		private void LoadModList()
		{
			modTopButton.Enabled = modUpButton.Enabled = modDownButton.Enabled = modBottomButton.Enabled = configureModButton.Enabled = false;
			modDescription.Text = "Description: No mod selected.";
			modListView.Items.Clear();
			mods = new Dictionary<string, SADXModInfo>();
			codes = new List<Code>(mainCodes.Codes);
			string modDir = Path.Combine(Environment.CurrentDirectory, "mods");

			foreach (string filename in SADXModInfo.GetModFiles(new DirectoryInfo(modDir)))
			{
				mods.Add((Path.GetDirectoryName(filename) ?? string.Empty).Substring(modDir.Length + 1), IniSerializer.Deserialize<SADXModInfo>(filename));
			}

			modListView.BeginUpdate();

			foreach (string mod in new List<string>(loaderini.Mods))
			{
				if (mods.ContainsKey(mod))
				{
					SADXModInfo inf = mods[mod];
					suppressEvent = true;
					modListView.Items.Add(new ListViewItem(new[] { inf.Name, inf.Author, inf.Version }) { Checked = true, Tag = mod });
					suppressEvent = false;
					if (!string.IsNullOrEmpty(inf.Codes))
						codes.AddRange(CodeList.Load(Path.Combine(Path.Combine(modDir, mod), inf.Codes)).Codes);
				}
				else
				{
					MessageBox.Show(this, "Mod \"" + mod + "\" could not be found.\n\nThis mod will be removed from the list.",
						Text, MessageBoxButtons.OK, MessageBoxIcon.Warning);
					loaderini.Mods.Remove(mod);
				}
			}

			foreach (KeyValuePair<string, SADXModInfo> inf in mods.OrderBy(x => x.Value.Name))
			{
				if (!loaderini.Mods.Contains(inf.Key))
					modListView.Items.Add(new ListViewItem(new[] { inf.Value.Name, inf.Value.Author, inf.Value.Version }) { Tag = inf.Key });
			}

			modListView.EndUpdate();

			loaderini.EnabledCodes = new List<string>(loaderini.EnabledCodes.Where(a => codes.Any(c => c.Name == a)));
			foreach (Code item in codes.Where(a => a.Required && !loaderini.EnabledCodes.Contains(a.Name)))
				loaderini.EnabledCodes.Add(item.Name);

			codesCheckedListBox.BeginUpdate();
			codesCheckedListBox.Items.Clear();

			foreach (Code item in codes)
				codesCheckedListBox.Items.Add(item.Name, loaderini.EnabledCodes.Contains(item.Name));

			codesCheckedListBox.EndUpdate();
		}

		private bool CheckForUpdates(bool force = false)
		{
			if (!force && !loaderini.UpdateCheck)
			{
				return false;
			}

			if (!force && !UpdateTimeElapsed(loaderini.UpdateUnit, loaderini.UpdateFrequency, DateTime.FromFileTimeUtc(loaderini.UpdateTime)))
			{
				return false;
			}

			checkedForUpdates = true;
			loaderini.UpdateTime = DateTime.UtcNow.ToFileTimeUtc();

			if (!File.Exists("sadxmlver.txt"))
			{
				return false;
			}

			using (var wc = new WebClient())
			{
				try
				{
					string msg = wc.DownloadString("http://mm.reimuhakurei.net/toolchangelog.php?tool=sadxml&rev=" + File.ReadAllText("sadxmlver.txt"));

					if (msg.Length > 0)
					{
						using (var dlg = new UpdateMessageDialog("SADX", msg.Replace("\n", "\r\n")))
						{
							if (dlg.ShowDialog(this) == DialogResult.Yes)
							{
								DialogResult result = DialogResult.OK;
								do
								{
									try
									{
										if (!Directory.Exists(updatePath))
										{
											Directory.CreateDirectory(updatePath);
										}
									}
									catch (Exception ex)
									{
										result = MessageBox.Show(this, "Failed to create temporary update directory:\n" + ex.Message
																	   + "\n\nWould you like to retry?", "Directory Creation Failed", MessageBoxButtons.RetryCancel);
										if (result == DialogResult.Cancel) return false;
									}
								} while (result == DialogResult.Retry);

								using (var dlg2 = new LoaderDownloadDialog("http://mm.reimuhakurei.net/sadxmods/SADXModLoader.7z", updatePath))
									if (dlg2.ShowDialog(this) == DialogResult.OK)
									{
										Close();
										return true;
									}
							}
						}
					}
				}
				catch
				{
					MessageBox.Show(this, "Unable to retrieve update information.", "SADX Mod Manager");
				}
			}

			return false;
		}

		private void InitializeWorker()
		{
			if (updateChecker != null)
			{
				return;
			}

			updateChecker = new BackgroundWorker { WorkerSupportsCancellation = true };
			updateChecker.DoWork += UpdateChecker_DoWork;
			updateChecker.RunWorkerCompleted += UpdateChecker_RunWorkerCompleted;
			updateChecker.RunWorkerCompleted += UpdateChecker_EnableControls;
		}

		private void UpdateChecker_EnableControls(object sender, RunWorkerCompletedEventArgs runWorkerCompletedEventArgs)
		{
			buttonCheckForUpdates.Enabled            = true;
			checkForUpdatesToolStripMenuItem.Enabled = true;
			verifyToolStripMenuItem.Enabled          = true;
			forceUpdateToolStripMenuItem.Enabled     = true;
			uninstallToolStripMenuItem.Enabled       = true;
			developerToolStripMenuItem.Enabled       = true;
		}

		private void CheckForModUpdates(bool force = false)
		{
			if (!force && !loaderini.ModUpdateCheck)
			{
				return;
			}

			InitializeWorker();

			if (!force && !UpdateTimeElapsed(loaderini.UpdateUnit, loaderini.UpdateFrequency, DateTime.FromFileTimeUtc(loaderini.ModUpdateTime)))
			{
				return;
			}

			checkedForUpdates = true;
			loaderini.ModUpdateTime = DateTime.UtcNow.ToFileTimeUtc();
			updateChecker.RunWorkerAsync(mods.Select(x => new KeyValuePair<string, ModInfo>(x.Key, x.Value)).ToList());
			buttonCheckForUpdates.Enabled = false;
		}

		private void UpdateChecker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
		{
			if (modUpdater.ForceUpdate)
			{
				updateChecker?.Dispose();
				updateChecker = null;
				modUpdater.ForceUpdate = false;
				modUpdater.Clear();
			}

			if (e.Cancelled)
			{
				return;
			}

			if (!(e.Result is Tuple<List<ModDownload>, List<string>> data))
			{
				return;
			}

			List<string> errors = data.Item2;
			if (errors.Count != 0)
			{
				MessageBox.Show(this, "The following errors occurred while checking for updates:\n\n" + string.Join("\n", errors),
					"Update Errors", MessageBoxButtons.OK, MessageBoxIcon.Warning);
			}

			bool manual = manualModUpdate;
			manualModUpdate = false;

			List<ModDownload> updates = data.Item1;
			if (updates.Count == 0)
			{
				if (manual)
				{
					MessageBox.Show(this, "Mods are up to date.",
						"No Updates", MessageBoxButtons.OK, MessageBoxIcon.Information);
				}
				return;
			}

			using (var dialog = new ModUpdatesDialog(updates))
			{
				if (dialog.ShowDialog(this) != DialogResult.OK)
				{
					return;
				}

				updates = dialog.SelectedMods;
			}

			if (updates.Count == 0)
			{
				return;
			}

			DialogResult result;

			do
			{
				try
				{
					result = DialogResult.Cancel;
					if (!Directory.Exists(updatePath))
					{
						Directory.CreateDirectory(updatePath);
					}
				}
				catch (Exception ex)
				{
					result = MessageBox.Show(this, "Failed to create temporary update directory:\n" + ex.Message
						+ "\n\nWould you like to retry?", "Directory Creation Failed", MessageBoxButtons.RetryCancel);
				}
			} while (result == DialogResult.Retry);

			using (var progress = new ModDownloadDialog(updates, updatePath))
			{
				progress.ShowDialog(this);
			}

			do
			{
				try
				{
					result = DialogResult.Cancel;
					Directory.Delete(updatePath, true);
				}
				catch (Exception ex)
				{
					result = MessageBox.Show(this, "Failed to remove temporary update directory:\n" + ex.Message
						+ "\n\nWould you like to retry? You can remove the directory manually later.",
						"Directory Deletion Failed", MessageBoxButtons.RetryCancel);
				}
			} while (result == DialogResult.Retry);

			LoadModList();
		}

		private void UpdateChecker_DoWork(object sender, DoWorkEventArgs e)
		{
			if (!(sender is BackgroundWorker worker))
			{
				throw new Exception("what");
			}

			Invoke(new Action(() =>
			{
				buttonCheckForUpdates.Enabled            = false;
				checkForUpdatesToolStripMenuItem.Enabled = false;
				verifyToolStripMenuItem.Enabled          = false;
				forceUpdateToolStripMenuItem.Enabled     = false;
				uninstallToolStripMenuItem.Enabled       = false;
				developerToolStripMenuItem.Enabled       = false;
			}));

			var updatableMods = e.Argument as List<KeyValuePair<string, ModInfo>>;
			List<ModDownload> updates = null;
			List<string> errors = null;

			var tokenSource = new CancellationTokenSource();
			CancellationToken token = tokenSource.Token;

			using (var task = new Task(() => modUpdater.GetModUpdates(updatableMods, out updates, out errors, token), token))
			{
				task.Start();

				while (!task.IsCompleted && !task.IsCanceled)
				{
					Application.DoEvents();

					if (worker.CancellationPending)
					{
						tokenSource.Cancel();
					}
				}

				task.Wait(token);
			}

			e.Result = new Tuple<List<ModDownload>, List<string>>(updates, errors);
		}

		// TODO: merge with ^
		private void UpdateChecker_DoWorkForced(object sender, DoWorkEventArgs e)
		{
			if (!(sender is BackgroundWorker worker))
			{
				throw new Exception("what");
			}

			if (!(e.Argument is List<Tuple<string, ModInfo, List<ModManifestDiff>>> updatableMods) || updatableMods.Count == 0)
			{
				return;
			}

			var updates = new List<ModDownload>();
			var errors = new List<string>();

			using (var client = new UpdaterWebClient())
			{
				foreach (Tuple<string, ModInfo, List<ModManifestDiff>> info in updatableMods)
				{
					if (worker.CancellationPending)
					{
						e.Cancel = true;
						break;
					}

					ModInfo mod = info.Item2;
					if (!string.IsNullOrEmpty(mod.GitHubRepo))
					{
						if (string.IsNullOrEmpty(mod.GitHubAsset))
						{
							errors.Add($"[{ mod.Name }] GitHubRepo specified, but GitHubAsset is missing.");
							continue;
						}

						ModDownload d = modUpdater.GetGitHubReleases(mod, info.Item1, client, errors);
						if (d != null)
						{
							updates.Add(d);
						}
					}
					else if (!string.IsNullOrEmpty(mod.GameBananaItemType) && mod.GameBananaItemId.HasValue)
					{
						ModDownload d = modUpdater.GetGameBananaReleases(mod, info.Item1, errors);
						if (d != null)
						{
							updates.Add(d);
						}
					}
					else if (!string.IsNullOrEmpty(mod.UpdateUrl))
					{
						List<ModManifestEntry> localManifest = info.Item3
							.Where(x => x.State == ModManifestState.Unchanged)
							.Select(x => x.Current).ToList();

						ModDownload d = modUpdater.CheckModularVersion(mod, info.Item1, localManifest, client, errors);
						if (d != null)
						{
							updates.Add(d);
						}
					}
				}
			}

			e.Result = new Tuple<List<ModDownload>, List<string>>(updates, errors);
		}

		private void modListView_SelectedIndexChanged(object sender, EventArgs e)
		{
			int count = modListView.SelectedIndices.Count;
			if (count == 0)
			{
				modTopButton.Enabled = modUpButton.Enabled = modDownButton.Enabled = modBottomButton.Enabled = configureModButton.Enabled = false;
				modDescription.Text = "Description: No mod selected.";
			}
			else if (count == 1)
			{
				modDescription.Text = "Description: " + mods[(string)modListView.SelectedItems[0].Tag].Description;
				modTopButton.Enabled = modListView.SelectedIndices[0] != 0;
				modUpButton.Enabled = modListView.SelectedIndices[0] > 0;
				modDownButton.Enabled = modListView.SelectedIndices[0] < modListView.Items.Count - 1;
				modBottomButton.Enabled = modListView.SelectedIndices[0] != modListView.Items.Count - 1;
				configureModButton.Enabled = File.Exists(Path.Combine("mods", (string)modListView.SelectedItems[0].Tag, "configschema.xml"));
			}
			else if (count > 1)
			{
				modDescription.Text = "Description: Multiple mods selected.";
				modTopButton.Enabled = modUpButton.Enabled = modDownButton.Enabled = modBottomButton.Enabled = true;
				configureModButton.Enabled = false;
			}
		}

		private void modTopButton_Click(object sender, EventArgs e)
		{
			if (modListView.SelectedItems.Count < 1)
				return;

			modListView.BeginUpdate();

			for (int i = 0; i < modListView.SelectedItems.Count; i++)
			{
				int index = modListView.SelectedItems[i].Index;

				if (index > 0)
				{
					ListViewItem item = modListView.SelectedItems[i];
					modListView.Items.Remove(item);
					modListView.Items.Insert(i, item);
				}
			}

			modListView.SelectedItems[0].EnsureVisible();
			modListView.EndUpdate();
		}

		private void modUpButton_Click(object sender, EventArgs e)
		{
			if (modListView.SelectedItems.Count < 1)
				return;

			modListView.BeginUpdate();

			for (int i = 0; i < modListView.SelectedItems.Count; i++)
			{
				int index = modListView.SelectedItems[i].Index;

				if (index-- > 0 && !modListView.Items[index].Selected)
				{
					ListViewItem item = modListView.SelectedItems[i];
					modListView.Items.Remove(item);
					modListView.Items.Insert(index, item);
				}
			}

			modListView.SelectedItems[0].EnsureVisible();
			modListView.EndUpdate();
		}

		private void modDownButton_Click(object sender, EventArgs e)
		{
			if (modListView.SelectedItems.Count < 1)
				return;

			modListView.BeginUpdate();

			for (int i = modListView.SelectedItems.Count - 1; i >= 0; i--)
			{
				int index = modListView.SelectedItems[i].Index + 1;

				if (index != modListView.Items.Count && !modListView.Items[index].Selected)
				{
					ListViewItem item = modListView.SelectedItems[i];
					modListView.Items.Remove(item);
					modListView.Items.Insert(index, item);
				}
			}

			modListView.SelectedItems[modListView.SelectedItems.Count - 1].EnsureVisible();
			modListView.EndUpdate();
		}

		private void modBottomButton_Click(object sender, EventArgs e)
		{
			if (modListView.SelectedItems.Count < 1)
				return;

			modListView.BeginUpdate();

			for (int i = modListView.SelectedItems.Count - 1; i >= 0; i--)
			{
				int index = modListView.SelectedItems[i].Index;

				if (index != modListView.Items.Count - 1)
				{
					ListViewItem item = modListView.SelectedItems[i];
					modListView.Items.Remove(item);
					modListView.Items.Insert(modListView.Items.Count, item);
				}
			}

			modListView.SelectedItems[modListView.SelectedItems.Count - 1].EnsureVisible();
			modListView.EndUpdate();
		}

		static readonly string moddropname = "Mod" + Process.GetCurrentProcess().Id;
		private void modListView_ItemDrag(object sender, ItemDragEventArgs e)
		{
			modListView.DoDragDrop(new DataObject(moddropname, modListView.SelectedItems.Cast<ListViewItem>().ToArray()), DragDropEffects.Move | DragDropEffects.Scroll);
		}

		private void modListView_DragEnter(object sender, DragEventArgs e)
		{
			if (e.Data.GetDataPresent(moddropname))
				e.Effect = DragDropEffects.Move | DragDropEffects.Scroll;
			else
				e.Effect = DragDropEffects.None;
		}

		private void modListView_DragOver(object sender, DragEventArgs e)
		{
			if (e.Data.GetDataPresent(moddropname))
				e.Effect = DragDropEffects.Move | DragDropEffects.Scroll;
			else
				e.Effect = DragDropEffects.None;
		}

		private void modListView_DragDrop(object sender, DragEventArgs e)
		{
			if (e.Data.GetDataPresent(moddropname))
			{
				Point clientPoint = modListView.PointToClient(new Point(e.X, e.Y));
				ListViewItem[] items = (ListViewItem[])e.Data.GetData(moddropname);
				int ind = modListView.GetItemAt(clientPoint.X, clientPoint.Y).Index;
				foreach (ListViewItem item in items)
					if (ind > item.Index)
						ind++;
				modListView.BeginUpdate();
				foreach (ListViewItem item in items)
					modListView.Items.Insert(ind++, (ListViewItem)item.Clone());
				foreach (ListViewItem item in items)
					modListView.Items.Remove(item);
				modListView.EndUpdate();
			}
		}

		private void Save()
		{
			loaderini.Mods.Clear();

			foreach (ListViewItem item in modListView.CheckedItems)
			{
				loaderini.Mods.Add((string)item.Tag);
			}

			loaderini.DebugConsole              = consoleCheckBox.Checked;
			loaderini.DebugScreen               = screenCheckBox.Checked;
			loaderini.DebugFile                 = fileCheckBox.Checked;
			loaderini.DisableCDCheck            = disableCDCheckCheckBox.Checked;
			loaderini.HorizontalResolution      = (int)horizontalResolution.Value;
			loaderini.VerticalResolution        = (int)verticalResolution.Value;
			loaderini.ForceAspectRatio          = forceAspectRatioCheckBox.Checked;
			loaderini.ScaleHud                  = checkScaleHud.Checked;
			loaderini.BackgroundFillMode        = comboBackgroundFill.SelectedIndex;
			loaderini.FmvFillMode               = comboFmvFill.SelectedIndex;
			loaderini.EnableVsync               = checkVsync.Checked;
			loaderini.WindowedFullscreen        = windowedFullscreenCheckBox.Checked;
			loaderini.AutoMipmap                = forceMipmappingCheckBox.Checked;
			loaderini.TextureFilter             = forceTextureFilterCheckBox.Checked;
			loaderini.PauseWhenInactive         = pauseWhenInactiveCheckBox.Checked;
			loaderini.StretchFullscreen         = stretchFullscreenCheckBox.Checked;
			loaderini.ScreenNum                 = screenNumComboBox.SelectedIndex;
			loaderini.CustomWindowSize          = customWindowSizeCheckBox.Checked;
			loaderini.WindowWidth               = (int)windowWidth.Value;
			loaderini.WindowHeight              = (int)windowHeight.Value;
			loaderini.MaintainWindowAspectRatio = maintainWindowAspectRatioCheckBox.Checked;
			loaderini.ResizableWindow           = checkWindowResize.Checked;
			loaderini.UpdateCheck               = checkUpdateStartup.Checked;
			loaderini.ModUpdateCheck            = checkUpdateModsStartup.Checked;
			loaderini.UpdateUnit                = (UpdateUnit)comboUpdateFrequency.SelectedIndex;
			loaderini.UpdateFrequency           = (int)numericUpdateFrequency.Value;

			IniSerializer.Serialize(loaderini, loaderinipath);

			List<Code> selectedCodes = new List<Code>();
			List<Code> selectedPatches = new List<Code>();

			foreach (Code item in codesCheckedListBox.CheckedIndices.OfType<int>().Select(a => codes[a]))
			{
				if (item.Patch)
					selectedPatches.Add(item);
				else
					selectedCodes.Add(item);
			}

			CodeList.WriteDatFile(patchdatpath, selectedPatches);
			CodeList.WriteDatFile(codedatpath, selectedCodes);
		}

		private void saveAndPlayButton_Click(object sender, EventArgs e)
		{
			if (updateChecker?.IsBusy == true)
			{
				var result = MessageBox.Show(this, "Mods are still being checked for updates. Continue anyway?",
					"Busy", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

				if (result == DialogResult.No)
				{
					return;
				}

				Enabled = false;

				updateChecker.CancelAsync();
				while (updateChecker.IsBusy)
				{
					Application.DoEvents();
				}

				Enabled = true;
			}

			Save();
			if (!installed)
				switch (MessageBox.Show(this, "Looks like you're starting the game without the mod loader installed. Without the mod loader, the mods and codes you've selected won't be used, and some settings may not work.\n\nDo you want to install the mod loader now?", "SADX Mod Manager", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button1))
				{
					case DialogResult.Cancel:
						return;
					case DialogResult.Yes:
						File.Move(datadllpath, datadllorigpath);
						File.Copy(loaderdllpath, datadllpath);
						break;
				}
			Process process = Process.Start(loaderini.Mods.Select((item) => mods[item].EXEFile)
												.FirstOrDefault((item) => !string.IsNullOrEmpty(item)) ?? "sonic.exe");
			process?.WaitForInputIdle(10000);
			Close();
		}

		private void saveButton_Click(object sender, EventArgs e)
		{
			Save();
			LoadModList();
		}

		private void installButton_Click(object sender, EventArgs e)
		{
			if (installed)
			{
				File.Delete(datadllpath);
				File.Move(datadllorigpath, datadllpath);
				installButton.Text = "Install loader";
			}
			else
			{
				File.Move(datadllpath, datadllorigpath);
				File.Copy(loaderdllpath, datadllpath);
				installButton.Text = "Uninstall loader";
			}
			installed = !installed;
		}

		private void screenNumComboBox_SelectedIndexChanged(object sender, EventArgs e)
		{
			Size oldsize = resolutionPresets[5];
			Rectangle rect = Screen.PrimaryScreen.Bounds;
			if (screenNumComboBox.SelectedIndex > 0)
				rect = Screen.AllScreens[screenNumComboBox.SelectedIndex - 1].Bounds;
			else
				foreach (Screen screen in Screen.AllScreens)
					rect = Rectangle.Union(rect, screen.Bounds);
			resolutionPresets[5] = rect.Size;
			resolutionPresets[6] = new Size(rect.Width / 2, rect.Height / 2);
			resolutionPresets[7] = new Size(rect.Width * 2, rect.Height * 2);
			if (comboResolutionPreset.SelectedIndex > 4 && comboResolutionPreset.SelectedIndex < 8 && rect.Size != oldsize)
				comboResolutionPreset.SelectedIndex = -1;
		}

		const decimal ratio = 4 / 3m;
		private void forceAspectRatioCheckBox_CheckedChanged(object sender, EventArgs e)
		{
			if (forceAspectRatioCheckBox.Checked)
			{
				horizontalResolution.Enabled = false;
				horizontalResolution.Value = Math.Round(verticalResolution.Value * ratio);
				comboResolutionPreset.SelectedIndex = -1;
			}
			else if (!suppressEvent)
				horizontalResolution.Enabled = true;
		}

		private void horizontalResolution_ValueChanged(object sender, EventArgs e)
		{
			if (!suppressEvent)
				comboResolutionPreset.SelectedIndex = -1;
		}

		private void verticalResolution_ValueChanged(object sender, EventArgs e)
		{
			if (forceAspectRatioCheckBox.Checked)
				horizontalResolution.Value = Math.Round(verticalResolution.Value * ratio);
			if (!suppressEvent)
				comboResolutionPreset.SelectedIndex = -1;
		}

		static readonly Size[] resolutionPresets =
		{
			new Size(640, 480), // 640x480
			new Size(800, 600), // 800x600
			new Size(1024, 768), // 1024x768
			new Size(1152, 864), // 1152x864
			new Size(1280, 1024), // 1280x1024
			new Size(), // Native
			new Size(), // 1/2x Native
			new Size(), // 2x Native
			new Size(1280, 720), // 720p
			new Size(1920, 1080), // 1080p
			new Size(3840, 2160), // 4K
		};
		private void comboResolutionPreset_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (comboResolutionPreset.SelectedIndex == -1) return;
			suppressEvent = true;
			verticalResolution.Value = resolutionPresets[comboResolutionPreset.SelectedIndex].Height;
			if (!forceAspectRatioCheckBox.Checked)
				horizontalResolution.Value = resolutionPresets[comboResolutionPreset.SelectedIndex].Width;
			suppressEvent = false;
		}

		private void configEditorButton_Click(object sender, EventArgs e)
		{
			using (ConfigEditDialog dlg = new ConfigEditDialog())
				dlg.ShowDialog(this);
		}

		private void buttonRefreshModList_Click(object sender, EventArgs e)
		{
			LoadModList();
		}

		private void buttonNewMod_Click(object sender, EventArgs e)
		{
			using (var ModDialog = new NewModDialog())
			{
				if (ModDialog.ShowDialog() == DialogResult.OK)
					LoadModList();
			}
		}

		private void customWindowSizeCheckBox_CheckedChanged(object sender, EventArgs e)
		{
			maintainWindowAspectRatioCheckBox.Enabled = customWindowSizeCheckBox.Checked;
			windowHeight.Enabled = customWindowSizeCheckBox.Checked;
			checkWindowResize.Enabled = !customWindowSizeCheckBox.Checked;
			windowWidth.Enabled = customWindowSizeCheckBox.Checked && !maintainWindowAspectRatioCheckBox.Checked;
		}

		private void maintainWindowAspectRatioCheckBox_CheckedChanged(object sender, EventArgs e)
		{
			if (maintainWindowAspectRatioCheckBox.Checked)
			{
				windowWidth.Enabled = false;
				windowWidth.Value = Math.Round(windowHeight.Value * (horizontalResolution.Value / verticalResolution.Value));
			}
			else if (!suppressEvent)
				windowWidth.Enabled = true;
		}

		private void windowHeight_ValueChanged(object sender, EventArgs e)
		{
			if (maintainWindowAspectRatioCheckBox.Checked)
				windowWidth.Value = Math.Round(windowHeight.Value * (horizontalResolution.Value / verticalResolution.Value));
		}

		private void codesCheckedListBox_ItemCheck(object sender, ItemCheckEventArgs e)
		{
			Code code = codes[e.Index];
			if (code.Required)
				e.NewValue = CheckState.Checked;
			if (e.NewValue == CheckState.Unchecked)
			{
				if (loaderini.EnabledCodes.Contains(code.Name))
					loaderini.EnabledCodes.Remove(code.Name);
			}
			else
			{
				if (!loaderini.EnabledCodes.Contains(code.Name))
					loaderini.EnabledCodes.Add(code.Name);
			}
		}

		private void modListView_ItemCheck(object sender, ItemCheckEventArgs e)
		{
			if (suppressEvent) return;
			codes = new List<Code>(mainCodes.Codes);
			string modDir = Path.Combine(Environment.CurrentDirectory, "mods");
			List<string> modlist = new List<string>();
			foreach (ListViewItem item in modListView.CheckedItems)
				modlist.Add((string)item.Tag);
			if (e.NewValue == CheckState.Unchecked)
				modlist.Remove((string)modListView.Items[e.Index].Tag);
			else
				modlist.Add((string)modListView.Items[e.Index].Tag);
			foreach (string mod in modlist)
				if (mods.ContainsKey(mod))
				{
					SADXModInfo inf = mods[mod];
					if (!string.IsNullOrEmpty(inf.Codes))
						codes.AddRange(CodeList.Load(Path.Combine(Path.Combine(modDir, mod), inf.Codes)).Codes);
				}
			loaderini.EnabledCodes = new List<string>(loaderini.EnabledCodes.Where(a => codes.Any(c => c.Name == a)));
			foreach (Code item in codes.Where(a => a.Required && !loaderini.EnabledCodes.Contains(a.Name)))
				loaderini.EnabledCodes.Add(item.Name);
			codesCheckedListBox.BeginUpdate();
			codesCheckedListBox.Items.Clear();
			foreach (Code item in codes)
				codesCheckedListBox.Items.Add(item.Name, loaderini.EnabledCodes.Contains(item.Name));
			codesCheckedListBox.EndUpdate();
		}

		private void modListView_MouseClick(object sender, MouseEventArgs e)
		{
			if (e.Button != MouseButtons.Right)
			{
				return;
			}

			if (modListView.FocusedItem.Bounds.Contains(e.Location))
			{
				modContextMenu.Show(Cursor.Position);
			}
		}

		private void openFolderToolStripMenuItem_Click(object sender, EventArgs e)
		{
			foreach (ListViewItem item in modListView.SelectedItems)
			{
				Process.Start(Path.Combine("mods", (string)item.Tag));
			}
		}

		private void uninstallToolStripMenuItem_Click(object sender, EventArgs e)
		{
			DialogResult result = MessageBox.Show(this, "This will uninstall all selected mods."
				+ "\n\nAre you sure you wish to continue?", "Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

			if (result != DialogResult.Yes)
			{
				return;
			}

			result = MessageBox.Show(this, "Would you like to keep mod user data where possible? (Save files, config files, etc)",
				"User Data", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);

			if (result == DialogResult.Cancel)
			{
				return;
			}

			foreach (ListViewItem item in modListView.SelectedItems)
			{
				var dir = (string)item.Tag;
				var modDir = Path.Combine("mods", dir);
				var manpath = Path.Combine(modDir, "mod.manifest");

				try
				{
					if (result == DialogResult.Yes && File.Exists(manpath))
					{
						List<ModManifestEntry> manifest = ModManifest.FromFile(manpath);
						foreach (var entry in manifest)
						{
							var path = Path.Combine(modDir, entry.FilePath);
							if (File.Exists(path))
							{
								File.Delete(path);
							}
						}

						File.Delete(manpath);
						var version = Path.Combine(modDir, "mod.version");
						if (File.Exists(version))
						{
							File.Delete(version);
						}
					}
					else
					{
						if (result == DialogResult.Yes)
						{
							var retain = MessageBox.Show(this, $"The mod \"{ mods[dir].Name }\" (\"mods\\{ dir }\") does not have a manifest, so mod user data cannot be retained."
								+ " Do you want to uninstall it anyway?", "Cannot Retain User Data", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

							if (retain == DialogResult.No)
							{
								continue;
							}
						}

						Directory.Delete(modDir, true);
					}
				}
				catch (Exception ex)
				{
					MessageBox.Show(this, $"Failed to uninstall mod \"{ mods[dir].Name }\" from \"{ dir }\": { ex.Message }", "Failed",
						MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
			}

			LoadModList();
		}

		private bool displayedManifestWarning;
		
		private void generateManifestToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (!displayedManifestWarning)
			{
				DialogResult result = MessageBox.Show(this, Resources.GenerateManifestWarning,
					"Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

				if (result != DialogResult.Yes)
				{
					return;
				}

				displayedManifestWarning = true;
			}

			foreach (ListViewItem item in modListView.SelectedItems)
			{
				var modPath = Path.Combine("mods", (string)item.Tag);
				var manifestPath = Path.Combine(modPath, "mod.manifest");

				List<ModManifestEntry> manifest;
				List<ModManifestDiff> diff;

				using (var progress = new ManifestDialog(modPath, $"Generating manifest: {(string)item.Tag}", true))
				{
					progress.SetTask("Generating file index...");
					if (progress.ShowDialog(this) == DialogResult.Cancel)
					{
						continue;
					}

					diff = progress.Diff;
				}

				if (diff == null)
				{
					continue;
				}

				if (diff.Count(x => x.State != ModManifestState.Unchanged) <= 0)
				{
					continue;
				}

				using (var dialog = new ManifestDiffDialog(diff))
				{
					if (dialog.ShowDialog(this) == DialogResult.Cancel)
					{
						continue;
					}

					manifest = dialog.MakeNewManifest();
				}

				ModManifest.ToFile(manifest, manifestPath);
			}
		}

		private void UpdateSelectedMods()
		{
			InitializeWorker();
			manualModUpdate = true;
			updateChecker?.RunWorkerAsync(modListView.SelectedItems.Cast<ListViewItem>()
				.Select(x => (string)x.Tag)
				.Select(x => new KeyValuePair<string, ModInfo>(x, mods[x]))
				.ToList());
		}

		private void checkForUpdatesToolStripMenuItem_Click(object sender, EventArgs e)
		{
			UpdateSelectedMods();
		}

		private void forceUpdateToolStripMenuItem_Click(object sender, EventArgs e)
		{
			var result = MessageBox.Show(this, "This will force all selected mods to be completely re-downloaded."
				+ " Are you sure you want to continue?",
				"Force Update", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

			if (result != DialogResult.Yes)
			{
				return;
			}

			modUpdater.ForceUpdate = true;
			UpdateSelectedMods();
		}

		private void verifyToolStripMenuItem_Click(object sender, EventArgs e)
		{
			List<Tuple<string, ModInfo>> items = modListView.SelectedItems.Cast<ListViewItem>()
				.Select(x => (string)x.Tag)
				.Where(x => File.Exists(Path.Combine("mods", x, "mod.manifest")))
				.Select(x => new Tuple<string, ModInfo>(x, mods[x]))
				.ToList();

			if (items.Count < 1)
			{
				MessageBox.Show(this, "None of the selected mods have manifests, so they cannot be verified.",
					"Missing mod.manifest", MessageBoxButtons.OK, MessageBoxIcon.Warning);
				return;
			}

			using (var progress = new VerifyModDialog(items))
			{
				var result = progress.ShowDialog(this);
				if (result == DialogResult.Cancel)
				{
					return;
				}

				List<Tuple<string, ModInfo, List<ModManifestDiff>>> failed = progress.Failed;
				if (failed.Count < 1)
				{
					MessageBox.Show(this, "All selected mods passed verification.", "Integrity Pass",
						MessageBoxButtons.OK, MessageBoxIcon.Information);
				}
				else
				{
					result = MessageBox.Show(this, "The following mods failed verification:\n"
						+ string.Join("\n", failed.Select(x => $"{x.Item2.Name}: {x.Item3.Count(y => y.State != ModManifestState.Unchanged)} file(s)"))
						+ "\n\nWould you like to attempt repairs?",
						"Integrity Fail", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

					if (result != DialogResult.Yes)
					{
						return;
					}

					InitializeWorker();

					updateChecker.DoWork -= UpdateChecker_DoWork;
					updateChecker.DoWork += UpdateChecker_DoWorkForced;

					updateChecker.RunWorkerAsync(failed);

					modUpdater.ForceUpdate = true;
					buttonCheckForUpdates.Enabled = false;
				}
			}
		}

		private void comboUpdateFrequency_SelectedIndexChanged(object sender, EventArgs e)
		{
			numericUpdateFrequency.Enabled = comboUpdateFrequency.SelectedIndex > 0;
		}

		private void buttonCheckForUpdates_Click(object sender, EventArgs e)
		{
			buttonCheckForUpdates.Enabled = false;

			if (CheckForUpdates(true))
			{
				return;
			}

			manualModUpdate = true;
			CheckForModUpdates(true);
		}

		private void installURLHandlerButton_Click(object sender, EventArgs e)
		{
			Process.Start(new ProcessStartInfo(Application.ExecutablePath, "urlhandler") { UseShellExecute = true, Verb = "runas" }).WaitForExit();
			MessageBox.Show(this, "URL handler installed!", Text);
		}

		private void configureModButton_Click(object sender, EventArgs e)
		{
			using (ModConfigDialog dlg = new ModConfigDialog(Path.Combine("mods", (string)modListView.SelectedItems[0].Tag), modListView.SelectedItems[0].Text))
				dlg.ShowDialog(this);
		}
	}
}
