using System;
using System.Net;
using Newtonsoft.Json;

namespace SADXModManager
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

	public class UpdaterWebClient : WebClient
	{
		protected override WebRequest GetWebRequest(Uri address)
		{
			var request = base.GetWebRequest(address) as HttpWebRequest;

			if (request != null)
			{
				request.UserAgent = "sadx-mod-manager";
			}

			return request;
		}
	}
}
