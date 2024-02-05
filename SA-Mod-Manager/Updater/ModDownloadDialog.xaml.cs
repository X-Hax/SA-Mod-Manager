using SAModManager.Common;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Security.Policy;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Shell;
using System.Windows.Threading;

namespace SAModManager.Updater
{
    /// <summary>
    /// Interaction logic for ModDownloadDialog.xaml
    /// </summary>
    public partial class ModDownloadDialog : Window
    {
        public List<ModDownload> updates;
        private string dest = string.Empty;
        public bool done = false;

        public Action<Exception> DownloadFailed;
        public Action DownloadCompleted;
        private IProgress<double?> _progress;
        private IProgress<double?> _progressOverall;
        private bool isUpdate;

        public ModDownloadDialog(List<ModDownload> updates, string dest, bool update = true)
        {
            InitializeComponent();
            this.updates = updates;
            this.dest = dest;
            this.isUpdate = update;
            Title = update ? Lang.GetString("Updater.DL.Mod.UpdatingMod") : Lang.GetString("Updater.DL.Dep.Downloading");
            HeaderTxt.Text = Title;


            ProgressOverall.Maximum = this.updates.Count;

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

        private void UpdateHeaderText(string txt)
        {
            try
            {
                // Update UI on the UI thread using Dispatcher
                Dispatcher.Invoke(() =>
                {
                    HeaderTxt.Text = this.isUpdate ? Lang.GetString("Updater.DL.Mod.UpdatingMod") : Lang.GetString("Updater.DL.Dep.Downloading");
                    HeaderTxt.Text += " " + "'" + txt + "'";
                });
            }
            catch { }
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

        private void UpdateCurFileText(string filename)
        {
            try
            {
                // Update UI on the UI thread using Dispatcher
                Dispatcher.Invoke(() =>
                {
                    curFile.Text = Lang.GetString("Updater.DL.Mod.DownloadingFile") + " '" + filename + "'...";
                });
            }
            catch { }
        }


        private async Task ModsUpdateComplete()
        {
            done = true;
            await Task.Delay(1000);
            Close();
        }

        private async Task Extracting(string dataDir, string filePath)
        {
            try
            {
                if (!Directory.Exists(dataDir))
                {
                    Directory.CreateDirectory(dataDir);
                }

                await Util.Extract(filePath, dataDir);
            }
            catch (Exception ex) 
            {
                throw new Exception("Failed to extract '" + filePath +"'\n at " + dataDir + "\n\n", ex);
            }
        }

        private async Task ParsingManifest(ModDownload file, string filePath, string dataDir)
        {
            string[] subfolders = Directory.GetDirectories(dataDir);

            //move all folders in mods folder (sometimes a zip can have multiple mods)
            foreach (var subfolder in subfolders)
            {
                var subFolder = new DirectoryInfo(subfolder);
                string subFolderName = subFolder?.Name;
                string workDir = Path.GetDirectoryName(ModInfo.GetModFiles(subFolder).FirstOrDefault());

                if (string.IsNullOrEmpty(workDir))
                {
                    continue;
                }

                if (subfolders.Length > 1 && this.isUpdate == false) //handle packaged mods (does not support updates atm)
                {
                    if (subFolderName.Contains(Path.GetFileNameWithoutExtension(file.Folder)) == false)
                    {
                        file.Name = subFolderName;
                        file.Folder = Path.Combine(App.CurrentGame.modDirectory, subFolderName);
                    }
                }

                string newManPath = Path.Combine(workDir, "mod.manifest");
                string oldManPath = Path.Combine(file.Folder, "mod.manifest");

                if (!File.Exists(newManPath) || !File.Exists(oldManPath))
                {                        
                    CopyDirectory(new DirectoryInfo(workDir), Directory.CreateDirectory(file.Folder));
                    Directory.Delete(subfolder, true);

                    if (File.Exists(filePath))
                    {
                        File.Delete(filePath);
                    }

                    File.WriteAllText(Path.Combine(file.Folder, "mod.version"), file.Updated.ToString(DateTimeFormatInfo.InvariantInfo));
                    continue;
                }

                await ApplyingManifest(oldManPath, newManPath, file, workDir);
            }
        }

        async Task ApplyingManifest(string oldManPath, string newManPath, ModDownload mod, string workDir)
        {
            List<ModManifestEntry> newManifest = ModManifest.FromFile(newManPath);
            List<ModManifestEntry> oldManifest = ModManifest.FromFile(oldManPath);
            List<string> oldFiles = oldManifest.Except(newManifest).Select(x => Path.Combine(mod.Folder, x.FilePath)).ToList();

            foreach (string file in oldFiles)
            {
                if (File.Exists(file))
                {
                    File.Delete(file);
                }
            }

            RemoveEmptyDirectories(mod, oldManifest, newManifest);

            foreach (ModManifestEntry file in newManifest)
            {
                string dir = Path.GetDirectoryName(file.FilePath);
                if (!string.IsNullOrEmpty(dir))
                {
                    string newDir = Path.Combine(mod.Folder, dir);
                    if (!Directory.Exists(newDir))
                    {
                        Directory.CreateDirectory(newDir);
                    }
                }

                var sourceFile = new FileInfo(Path.Combine(workDir, file.FilePath));
                var destFile = new FileInfo(Path.Combine(mod.Folder, file.FilePath));

                if (destFile.Exists)
                {
                    destFile.Delete();
                }

                sourceFile.Attributes &= ~FileAttributes.ReadOnly;
                sourceFile.MoveTo(destFile.FullName);
            }

            await Util.CopyFileAsync(newManPath, oldManPath, true);
        }

        private void CopyDirectory(DirectoryInfo oldDir, DirectoryInfo newDir)
        {
            foreach (DirectoryInfo dir in oldDir.EnumerateDirectories())
            {
                CopyDirectory(dir, newDir.CreateSubdirectory(dir.Name));
            }

            foreach (FileInfo file in oldDir.EnumerateFiles())
            {
                file.CopyTo(Path.Combine(newDir.FullName, file.Name), true);
            }
        }


        private async Task Download_Modular(ModDownload file, string updatePath, CancellationToken cancellationToken)
        {
            List<ModManifestDiff> newEntries = file.ChangedFiles
                            .Where(x => x.State == ModManifestState.Added || x.State == ModManifestState.Changed)
                            .ToList();

            var uri = new Uri(file.Url);
            string tempDir = Path.Combine(updatePath, uri.Segments.Last());
            var httpClient = UpdateHelper.HttpClient;

            if (!Directory.Exists(tempDir))
            {
                Directory.CreateDirectory(tempDir);
            }
  
            foreach (ModManifestDiff i in newEntries)
            {
                string filePath = Path.GetFullPath(Path.Combine(tempDir, i.Current.FilePath));
                string dir = Path.GetDirectoryName(filePath);

                if (!string.IsNullOrEmpty(dir) && !Directory.Exists(dir))
                {
                    Directory.CreateDirectory(dir);
                }

                var info = new FileInfo(filePath);

                if (!info.Exists || info.Length != i.Current.FileSize ||
                    !i.Current.Checksum.Equals(ModManifestGenerator.GetFileHash(filePath), StringComparison.OrdinalIgnoreCase))
                {

                    //dl each mod file one by one
                    var curFile = Path.Combine(uri.AbsoluteUri, i.Current.FilePath);
                    UpdateCurFileText(i.Current.FilePath);
                    await httpClient.DownloadFileAsync(curFile, filePath, _progress, cancellationToken).ConfigureAwait(false);
                    info.Refresh();

                    if (info.Length != i.Current.FileSize)
                    {
                        throw new Exception(string.Format("Size of downloaded file \"{0}\" ({1}) differs from manifest ({2}).",
                            i.Current.FilePath, SizeSuffix.GetSizeSuffix(info.Length), SizeSuffix.GetSizeSuffix(i.Current.FileSize)));
                    }

                    string hash = ModManifestGenerator.GetFileHash(filePath);
                    if (!i.Current.Checksum.Equals(hash, StringComparison.OrdinalIgnoreCase))
                    {
                        throw new Exception(string.Format("Checksum of downloaded file \"{0}\" ({1}) differs from manifest ({2}).",
                            i.Current.FilePath, hash, i.Current.Checksum));
                    }
                }
            }


            var uriMa = new Uri(uri, "mod.manifest"); //dl mod manifest
            await httpClient.DownloadFileAsync(uriMa.AbsoluteUri, Path.Combine(tempDir, "mod.manifest"), _progress, cancellationToken).ConfigureAwait(false);

            List<ModManifestDiff> movedEntries = file.ChangedFiles.Except(newEntries)
                    .Where(x => x.State == ModManifestState.Moved)
                    .ToList();

            foreach (ModManifestDiff i in movedEntries)
            {
                ModManifestEntry old = i.Last;

                // This would be considered an Error...
                if (old == null)
                {
                    continue;
                }

                string oldPath = Path.Combine(file.Folder, old.FilePath);
                string newPath = Path.Combine(tempDir, i.Current.FilePath);

                string dir = Path.GetDirectoryName(newPath);

                if (!string.IsNullOrEmpty(dir) && !Directory.Exists(dir))
                {
                    Directory.CreateDirectory(dir);
                }

                await Util.CopyFileAsync(oldPath, newPath, true);
            }


            // Now move the stuff from the temporary folder over to the working directory.
            foreach (ModManifestDiff i in newEntries.Concat(movedEntries))
            {
                string tempPath = Path.Combine(tempDir, i.Current.FilePath);
                string workPath = Path.Combine(file.Folder, i.Current.FilePath);
                string dir = Path.GetDirectoryName(workPath);

                if (!string.IsNullOrEmpty(dir) && !Directory.Exists(dir))
                {
                    Directory.CreateDirectory(dir);
                }

                await Util.CopyFileAsync(tempPath, workPath, true);
            }

            // Once that has succeeded we can safely delete files that have been marked for removal.
            List<ModManifestDiff> removedEntries = file.ChangedFiles
                .Where(x => x.State == ModManifestState.Removed)
                .ToList();

            foreach (string path in removedEntries.Select(i => Path.Combine(file.Folder, i.Current.FilePath)).Where(File.Exists))
            {
                File.Delete(path);
            }

            // Same for files that have been moved.
            foreach (string path in movedEntries
                .Where(x => newEntries.All(y => y.Current.FilePath != x.Last.FilePath))
                .Select(i => Path.Combine(file.Folder, i.Last.FilePath)).Where(File.Exists))
            {
                File.Delete(path);
            }

            string oldManPath = Path.Combine(file.Folder, "mod.manifest");
            string newManPath = Path.Combine(tempDir, "mod.manifest");

            if (File.Exists(oldManPath))
            {
                List<ModManifestEntry> oldManifest = ModManifest.FromFile(oldManPath);
                List<ModManifestEntry> newManifest = ModManifest.FromFile(newManPath);

                // Remove directories that are now empty.
                RemoveEmptyDirectories(file, oldManifest, newManifest);
            }

            // And last but not least, copy over the new manifest.
            await Util.CopyFileAsync(newManPath, oldManPath, true);
        }

        private async Task DownloadFileCompleted_Archive(ModDownload file, string filePath, string updatePath)
        {
            bool extract = false;
            bool removereadOnly = false;
            string dataDir = null;

            try
            {
                dataDir = Path.GetFileNameWithoutExtension(filePath);

                if (dataDir.Length > 20)
                    dataDir = dataDir.Remove(20).TrimEnd(' ');

                dataDir = Path.Combine(updatePath, dataDir);

                await Extracting(dataDir, filePath);
                extract = true;
          
                await ParsingManifest(file, filePath, dataDir);

                void removeReadOnly(DirectoryInfo dir)
                {
                    foreach (DirectoryInfo d in dir.GetDirectories())
                    {
                        removeReadOnly(d);
                        d.Attributes &= ~FileAttributes.ReadOnly;
                    }
                }

                if (Directory.Exists(dataDir))
                {
                    removeReadOnly(new DirectoryInfo(dataDir));
                    Directory.Delete(dataDir, true);
                }

                removereadOnly = true;
                File.WriteAllText(Path.Combine(file.Folder, "mod.version"), file.Updated.ToString(DateTimeFormatInfo.InvariantInfo));

                if (File.Exists(filePath))
                {
                    File.Delete(filePath);
                }
            }
            catch
            {
                throw new Exception("Mod Update: Failed to apply post download operations:'\n" +
                    "FilePath: " + filePath + "\n" 
                    + "Directory out: " + dataDir + "\n"
                    + "Extracted: " + extract.ToString() + "\n"
                    + "Removed Read Only: " + removereadOnly.ToString() + "\n");
            }
        }

        private async Task DoDownloadAsync(CancellationToken cancelToken = default)
        {
            string curName = string.Empty;
            try
            {
                var httpClient = UpdateHelper.HttpClient;
                int count = 0;
                UpdateProgressText(count.ToString(), updates.Count.ToString());

                foreach (var file in this.updates)
                {
                    curName = file.Name;
                    var uri = new Uri(file.Url);

                    UpdateHeaderText(file.Info.Name);

                    switch (file.Type)
                    {
                        case Updater.ModDownloadType.Archive:

                            if (!uri.Host.EndsWith("github.com", StringComparison.OrdinalIgnoreCase))
                            {
                                var response = await httpClient.SendAsync(new HttpRequestMessage(HttpMethod.Head, uri), cancelToken);
                                uri = response.RequestMessage.RequestUri;
                            }

                            string destination = Path.Combine(dest, Path.GetFileName(uri.LocalPath));

                            try
                            {
                                if (File.Exists(destination))
                                {
                                    File.Delete(destination);
                                }
                            }
                            catch { }

                            await httpClient.DownloadFileAsync(uri.AbsoluteUri, destination, _progress, cancelToken).ConfigureAwait(false);
                            string filePath = Path.Combine(this.dest, uri.Segments.Last());
                            if (File.Exists(filePath))
                            {
                                await DownloadFileCompleted_Archive(file, filePath, dest);
                            }
                            else
                            {
                                string s = Lang.GetString("MessageWindow.Errors.GenericDLFail0") + " " + uri.Segments.Last() +"\n'" + filePath +"'";
                                var error = new MessageWindow(Lang.GetString("MessageWindow.Errors.GenericDLFail.Title"), s, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK);
                                error.ShowDialog();
                            }
                            break;
                        case Updater.ModDownloadType.Modular:
                            await Download_Modular(file, dest, cancelToken);
                            break;
                        default:
                            throw new ArgumentOutOfRangeException();
                    }

                    count++;
                    try
                    {
                        UpdateProgressText(count.ToString(), updates.Count.ToString());
                        _progressOverall?.Report(count);
                    }
                    catch { }
                }

                await Dispatcher.InvokeAsync(() =>
                {
                    DownloadCompleted = async () => await ModsUpdateComplete();
                    DownloadCompleted?.Invoke();
                });
            }
            catch (Exception ex)
            {
                await Dispatcher.InvokeAsync(() =>
                {
                    string s = Lang.GetString("MessageWindow.Errors.GenericDLFail0") + " " + curName + "\n" + ex.Message + "\n\n";
                    var error = new MessageWindow(Lang.GetString("MessageWindow.Errors.GenericDLFail.Title"), s, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK);
                    error.ShowDialog();
                    Close();
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


        private void RemoveEmptyDirectories(ModDownload file, IEnumerable<ModManifestEntry> oldManifest, IEnumerable<ModManifestEntry> newManifest)
        {
            foreach (string dir in ModManifest.GetOldDirectories(oldManifest, newManifest)
                                              .Select(x => Path.Combine(file.Folder, x)))
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
    }

}

