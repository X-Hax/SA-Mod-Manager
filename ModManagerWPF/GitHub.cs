using System.Threading.Tasks;
using SAModManager.Properties;
using Octokit;
using Newtonsoft.Json;

namespace SAModManager
{
	public class GitHubUser
	{
		[JsonProperty("login")]
		public string Login { get; set; }
		[JsonProperty("html_url")]
		public string HtmlUrl { get; set; }
	}

	public class GitHubAsset
	{
		[JsonProperty("name")]
		public string Name { get; set; }
		[JsonProperty("uploader")]
		public GitHubUser Uploader { get; set; }
		[JsonProperty("size")]
		public long Size { get; set; }
		[JsonProperty("updated_at")]
		public string Uploaded { get; set; }
		[JsonProperty("browser_download_url")]
		public string DownloadUrl { get; set; }
	}

	public class GitHubRelease
	{
		[JsonProperty("prerelease")]
		public bool PreRelease { get; set; }
		[JsonProperty("draft")]
		public bool Draft { get; set; }
		[JsonProperty("html_url")]
		public string HtmlUrl { get; set; }
		[JsonProperty("name")]
		public string Name { get; set; }
		[JsonProperty("tag_name")]
		public string TagName { get; set; }
		[JsonProperty("published_at")]
		public string Published { get; set; }
		[JsonProperty("assets")]
		public GitHubAsset[] Assets { get; set; }
		[JsonProperty("body")]
		public string Body { get; set; }
	}

	public static class GitHub
	{

		public static GitHubClient client = new(new ProductHeaderValue("sadx-mod-loader"));

		public static string LastCommit = "";
		public static async Task<Repository> GetRepo()
		{
			return await client.Repository.Get("x-hax", "sadx-mod-loader");
		}

		private static async Task<GitHubCommit> GetLastCommit(long repoID, string branchName)
		{
			return await client.Repository.Commit.Get(repoID, branchName);
		}

		/// <summary>
		/// Gets the most recent commit to the repo.
		/// </summary>
		/// <returns>Most Recent Commit as a string</returns>
		public static async Task<string> GetRecentCommit()
		{

			if (client is null)
				return null;

			try
			{
				var Repo = await client.Repository.Get("x-hax", "sadx-mod-loader");

				if (Repo is null)
					return null;

				var id = Repo.Id;
				var lastCommit = await GetLastCommit(id, "wpf"); //todo swap to "master"

				if (lastCommit is not null)
				{
				
					return LastCommit = lastCommit.Sha[..7];
				}
			}
			catch
			{
				return LastCommit = Settings.Default.LastCommit;	
			}

			return LastCommit = Settings.Default.LastCommit;
		}
	}
}
