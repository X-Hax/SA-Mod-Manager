
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Threading;
using Newtonsoft.Json;
using SAModManager.Ini;

namespace SAModManager.Updater
{

    public class ModUpdateAndErrors
    {
        public List<ModDownloadWPF> updates = new();
        public List<string> errors = new();
    };

    public partial class ModUpdater
    {
        public ModUpdateAndErrors modUpdateHelper { get; set; } = new();
        public Tuple<List<ModDownloadWPF>, List<string>> modUpdatesTuple = null;
        public List<Tuple<string, ModInfo, List<ModManifestDiff>>> modManifestTuple = null;
        public List<KeyValuePair<string, ModInfo>> updatableMods = null;

        private readonly Dictionary<string, List<GitHubRelease>> gitHubCache = new();
        public bool ForceUpdate;

        private static DateTime? GetLocalVersion(string folder, string modsFolder, string basePath = null)
        {
            string versionPath = Path.Combine(modsFolder, folder, "mod.version");
            if (basePath != null)
                versionPath = Path.Combine(basePath, versionPath);
            DateTime? localVersion = null;

            if (File.Exists(versionPath))
            {
                string dateString = File.ReadAllText(versionPath).Trim();

                if (DateTime.TryParse(dateString, CultureInfo.InvariantCulture, DateTimeStyles.AssumeLocal, out DateTime date) ||
                    DateTime.TryParse(dateString, CultureInfo.CurrentCulture, DateTimeStyles.AssumeLocal, out date))
                {
                    localVersion = date;
                }
            }

            if (!localVersion.HasValue)
            {
                var info = new FileInfo(basePath == null ? Path.Combine(modsFolder, folder, "mod.manifest") : Path.Combine(basePath, modsFolder, folder, "mod.manifest"));
                if (info.Exists)
                {
                    localVersion = info.LastWriteTimeUtc;
                }
            }

            return localVersion;
        }

        public async Task<ModDownloadWPF> GetGitHubReleases(ModInfo mod, string modsFolder, string folder, HttpClient client, List<string> errors, string basePath = null)
        {
            List<GitHubRelease> releases;
            string url = "https://api.github.com/repos/" + mod.GitHubRepo + "/releases";

            if (!gitHubCache.TryGetValue(url, out List<GitHubRelease> value))
            {
                try
                {
                    string text = await client.GetStringAsync(url);
                    releases = JsonConvert.DeserializeObject<List<GitHubRelease>>(text)
                        .Where(x => !x.Draft && !x.PreRelease)
                        .ToList();

                    if (releases.Count > 0)
                    {
                        gitHubCache[url] = releases;
                    }
                }
                catch (Exception ex)
                {
                    errors.Add($"[{mod.Name}] Error checking for updates at {url}: {ex.Message}");
                    return null;
                }
            }
            else
            {
                releases = value;
            }

            if (releases == null || releases.Count == 0)
            {
                // No releases available.
                return null;
            }

            DateTime? localVersion = GetLocalVersion(folder, modsFolder, basePath);

            GitHubRelease latestRelease = null;
            GitHubAsset latestAsset = null;

            foreach (GitHubRelease release in releases)
            {
                GitHubAsset asset = release.Assets
                    .FirstOrDefault(x => x.Name.Equals(mod.GitHubAsset, StringComparison.OrdinalIgnoreCase));

                if (asset == null)
                {
                    continue;
                }

                latestRelease = release;

                if (!ForceUpdate && localVersion.HasValue)
                {
                    DateTime uploaded = DateTime.Parse(asset.Uploaded, CultureInfo.InvariantCulture, DateTimeStyles.AssumeLocal);

                    if (localVersion >= uploaded)
                    {
                        // No updates available.
                        break;
                    }
                }

                latestAsset = asset;
                break;
            }

            if (latestRelease == null)
            {
                errors.Add($"[{mod.Name}] No releases with matching asset \"{mod.GitHubAsset}\" could be found in {releases.Count} release(s).");
                return null;
            }

            if (latestAsset == null)
            {
                return null;
            }

            string body = Regex.Replace(latestRelease.Body, "(?<!\r)\n", "\r\n");

            return new ModDownloadWPF(mod, basePath == null ? Path.Combine(modsFolder, folder) : Path.Combine(basePath, modsFolder, folder), latestAsset.DownloadUrl, body, latestAsset.Size)
            {
                HomePage = "https://github.com/" + mod.GitHubRepo,
                Name = latestRelease.Name,
                Version = latestRelease.TagName,
                Published = DateTime.Parse(latestRelease.Published, DateTimeFormatInfo.InvariantInfo),
                Updated = DateTime.Parse(latestAsset.Uploaded, DateTimeFormatInfo.InvariantInfo),
                ReleaseUrl = latestRelease.HtmlUrl
            };
        }

        public async Task<ModDownloadWPF> GetGameBananaReleases(ModInfo mod, string modsFolder, string folder, List<string> errors, string basePath = null)
        {
            GameBananaItem gbi;
            try
            {
                gbi = await GameBananaItem.Load(mod.GameBananaItemType, mod.GameBananaItemId.Value);
            }
            catch (Exception ex)
            {
                errors.Add($"[{mod.Name}] Error checking for updates: {ex.Message}");
                return null;
            }

            if (!gbi.HasUpdates)
            {
                // No releases available.
                return null;
            }

            GameBananaItemUpdate latestUpdate = gbi.Updates[0];
            DateTime? localVersion = GetLocalVersion(folder, modsFolder, basePath);

            if (!ForceUpdate && localVersion.HasValue)
            {
                if (localVersion >= latestUpdate.DateAdded)
                {
                    // No updates available.
                    return null;
                }
            }

            string body = string.Join(Environment.NewLine, latestUpdate.Changes.Select(a => a.Category + ": " + a.Text)) + Environment.NewLine + latestUpdate.Text;

            GameBananaItemFile dl = gbi.Files.First().Value;

            return new ModDownloadWPF(mod, basePath == null ? Path.Combine(modsFolder, folder) : Path.Combine(basePath, modsFolder, folder), dl.DownloadUrl, body, dl.Filesize)
            {
                HomePage = gbi.ProfileUrl,
                Name = latestUpdate.Title,
                Version = latestUpdate.Title,
                Published = latestUpdate.DateAdded,
                Updated = latestUpdate.DateAdded,
                ReleaseUrl = gbi.ProfileUrl
            };
        }

        public async Task<ModDownloadWPF> CheckModularVersion(ModInfo mod, string modsFolder, string folder, List<ModManifestEntry> localManifest,
                                               HttpClient client, List<string> errors, string basePath = null)
        {
            if (!mod.UpdateUrl.StartsWith("http://", StringComparison.InvariantCulture)
                && !mod.UpdateUrl.StartsWith("https://", StringComparison.InvariantCulture))
            {
                mod.UpdateUrl = "http://" + mod.UpdateUrl;
            }

            if (!mod.UpdateUrl.EndsWith("/", StringComparison.InvariantCulture))
            {
                mod.UpdateUrl += "/";
            }

            var url = new Uri(mod.UpdateUrl);
            url = new Uri(url, "mod.ini");

            ModInfo remoteInfo;

            try
            {
                Dictionary<string, Dictionary<string, string>> dict = IniFile.Load(await client.GetStreamAsync(url));
                remoteInfo = IniSerializer.Deserialize<ModInfo>(dict);
            }
            catch (Exception ex)
            {
                errors.Add($"[{mod.Name}] Error pulling mod.ini from \"{mod.UpdateUrl}\": {ex.Message}");
                return null;
            }

            if (!ForceUpdate && remoteInfo.Version == mod.Version)
            {
                return null;
            }

            string manString;

            try
            {
                manString = await client.GetStringAsync(new Uri(new Uri(mod.UpdateUrl), "mod.manifest"));
            }
            catch (Exception ex)
            {
                errors.Add($"[{mod.Name}] Error pulling mod.manifest from \"{mod.UpdateUrl}\": {ex.Message}");
                return null;
            }

            List<ModManifestEntry> remoteManifest;

            try
            {
                remoteManifest = ModManifest.FromString(manString);
            }
            catch (Exception ex)
            {
                errors.Add($"[{mod.Name}] Error parsing remote manifest from \"{mod.UpdateUrl}\": {ex.Message}");
                return null;
            }

            List<ModManifestDiff> diff = ModManifestGenerator.Diff(remoteManifest, localManifest);

            if (diff.Count < 1 || diff.All(x => x.State == ModManifestState.Unchanged))
            {
                return null;
            }

            string changes;

            if (!string.IsNullOrEmpty(mod.ChangelogUrl))
            {
                try
                {
                    changes = await client.GetStringAsync(new Uri(mod.ChangelogUrl));
                }
                catch (Exception ex)
                {
                    changes = ex.Message;
                }
            }
            else
            {
                try
                {
                    changes = await client.GetStringAsync(new Uri(new Uri(mod.UpdateUrl), "changelog.txt"));
                }
                catch
                {
                    // ignored
                    changes = string.Empty;
                }
            }

            if (!string.IsNullOrEmpty(changes))
            {
                changes = Regex.Replace(changes, "(?<!\r)\n", "\r\n");
            }

            return new ModDownloadWPF(mod, basePath == null ? Path.Combine(modsFolder, folder) : Path.Combine(basePath, modsFolder, folder), mod.UpdateUrl, changes, diff);
        }

        // TODO: cancel

        /// <summary>
        /// Get mod update metadata for the provided mods.
        /// </summary>
        /// <param name="updatableMods">Key-value pairs of mods to be checked, where the key is the mod path and the value is the mod metadata.</param>
        /// <param name="updates">Output list of mods with available updates.</param>
        /// <param name="errors">Output list of errors encountered during the update process.</param>
        /// <param name="cancellationToken"><see cref="CancellationToken"/> for cancelling the operation. Currently unused.</param>
        public async Task GetModUpdates(string modsFolder, CancellationToken cancellationToken, string baseFolder = null)
        {
            modUpdateHelper.updates.Clear();
            modUpdateHelper.errors.Clear();

            if (updatableMods == null || updatableMods.Count == 0)
            {
                return;
            }

            var client = UpdateHelper.HttpClient;

            foreach (KeyValuePair<string, ModInfo> info in updatableMods)
            {
                ModInfo mod = info.Value;

                if (mod.DisableUpdate == true)
                {
                    continue;
                }

                if (System.Windows.Application.Current == null || MainWindow.cancelUpdate)
                {
                    break;
                }

                System.Windows.Application.Current.Dispatcher.Invoke(() =>
                {
                    ((MainWindow)System.Windows.Application.Current.MainWindow).UpdateManagerStatusText(string.Format(Lang.GetString("UpdateStatus.ChkCurModUpdate"), mod.Name));
                });

                if (!string.IsNullOrEmpty(mod.GitHubRepo))
                {
                    if (string.IsNullOrEmpty(mod.GitHubAsset))
                    {

                        System.Windows.Application.Current.Dispatcher.Invoke(() =>
                        {
                            ((MainWindow)System.Windows.Application.Current.MainWindow).UpdateManagerStatusText(string.Format(Lang.GetString("UpdateStatus.ModUpdateFail"), mod.Name));
                        });

                        modUpdateHelper.errors.Add($"[{mod.Name}] GitHubRepo specified, but GitHubAsset is missing.");
                        continue;
                    }

                    ModDownloadWPF d = await GetGitHubReleases(mod, modsFolder, info.Key, client, modUpdateHelper.errors, baseFolder);
                    if (d != null)
                    {
                        modUpdateHelper.updates.Add(d);
                    }
                }
                else if (!string.IsNullOrEmpty(mod.GameBananaItemType) && mod.GameBananaItemId.HasValue)
                {
                    ModDownloadWPF d = await GetGameBananaReleases(mod, modsFolder, info.Key, modUpdateHelper.errors, baseFolder);
                    if (d != null)
                    {
                        modUpdateHelper.updates.Add(d);
                    }
                }
                else if (!string.IsNullOrEmpty(mod.UpdateUrl))
                {
                    List<ModManifestEntry> localManifest = null;
                    string manPath = Path.Combine(modsFolder, info.Key, "mod.manifest");
                    if (baseFolder != null)
                        manPath = Path.Combine(baseFolder, manPath);

                    if (!ForceUpdate && File.Exists(manPath))
                    {
                        try
                        {
                            localManifest = ModManifest.FromFile(manPath);
                        }
                        catch (Exception ex)
                        {
                            System.Windows.Application.Current.Dispatcher.Invoke(() =>
                            {
                                ((MainWindow)System.Windows.Application.Current.MainWindow).UpdateManagerStatusText(string.Format(Lang.GetString("UpdateStatus.ModUpdateFail"), mod.Name));
                            });
                            modUpdateHelper.errors.Add($"[{mod.Name}] Error parsing local manifest: {ex.Message}");
                            continue;
                        }
                    }

                    ModDownloadWPF d = await CheckModularVersion(mod, modsFolder, info.Key, localManifest, client, modUpdateHelper.errors, baseFolder);
                    if (d != null)
                    {
                        modUpdateHelper.updates.Add(d);
                    }
                }
            }

        }


        /// <summary>
        /// Clears update metadata cache.
        /// </summary>
        public void Clear()
        {
            gitHubCache.Clear();
        }

        [GeneratedRegex("(?<!\r)\n")]
        private static partial Regex MyRegex();
    }
}
