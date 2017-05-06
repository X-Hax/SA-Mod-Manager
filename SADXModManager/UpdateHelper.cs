using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using IniFile;
using Newtonsoft.Json;

namespace SADXModManager
{
	public static class UpdateHelper
	{
		public static bool ForceUpdate;

		public static ModDownload GetGitHubReleases(ModInfo mod, string folder, 
			UpdaterWebClient client, Dictionary<string, List<GitHubRelease>> cache, List<string> errors)
		{
			List<GitHubRelease> releases;
			var url = "https://api.github.com/repos/" + mod.GitHubRepo + "/releases";
			if (!cache.ContainsKey(url))
			{
				try
				{
					var text = client.DownloadString(url);
					releases = JsonConvert.DeserializeObject<List<GitHubRelease>>(text)
						.Where(x => !x.Draft && !x.PreRelease)
						.ToList();

					if (releases.Count > 0)
					{
						cache[url] = releases;
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
				releases = cache[url];
			}

			// No releases available.
			if (releases == null || releases.Count == 0)
			{
				return null;
			}

			string versionPath = Path.Combine("mods", folder, "mod.version");
			DateTime? localVersion = null;

			if (File.Exists(versionPath))
			{
				localVersion = DateTime.Parse(File.ReadAllText(versionPath).Trim());
			}
			else
			{
				var info = new FileInfo(Path.Combine("mods", folder, "mod.manifest"));
				if (info.Exists)
				{
					localVersion = info.LastWriteTimeUtc;
				}
			}

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
					DateTime uploaded = DateTime.Parse(asset.Uploaded);

					// No updates available.
					if (localVersion >= uploaded)
					{
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

			var body = Regex.Replace(latestRelease.Body, "(?<!\r)\n", "\r\n");

			return new ModDownload(mod, Path.Combine("mods", folder), latestAsset.DownloadUrl, body, latestAsset.Size)
			{
				HomePage   = "https://github.com/" + mod.GitHubRepo,
				Name       = latestRelease.Name,
				Version    = latestRelease.TagName,
				Published  = latestRelease.Published,
				Updated    = latestAsset.Uploaded,
				ReleaseUrl = latestRelease.HtmlUrl
			};
		}

		public static ModDownload CheckModularVersion(ModInfo mod, string folder, List<ModManifest> localManifest,
			UpdaterWebClient client, List<string> errors)
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
				var dict = IniFile.IniFile.Load(client.OpenRead(url));
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
				manString = client.DownloadString(new Uri(new Uri(mod.UpdateUrl), "mod.manifest"));
			}
			catch (Exception ex)
			{
				errors.Add($"[{mod.Name}] Error pulling mod.manifest from \"{mod.UpdateUrl}\": {ex.Message}");
				return null;
			}

			List<ModManifest> remoteManifest;

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
					changes = client.DownloadString(new Uri(mod.ChangelogUrl));
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
					changes = client.DownloadString(new Uri(new Uri(mod.UpdateUrl), "changelog.txt"));
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

			return new ModDownload(mod, Path.Combine("mods", folder), mod.UpdateUrl, changes, diff);
		}
	}
}