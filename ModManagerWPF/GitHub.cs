using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SAModManager.Properties;
using Octokit;


namespace SAModManager
{
	public class GitHub
	{
		MainWindow Window { get; set; }
		public GitHub(MainWindow window)
		{
			Window = window;
		}
		public GitHubClient client = new GitHubClient(new ProductHeaderValue("sadx-mod-loader"));

		public string LastCommit = "";
		public async Task<Repository> GetRepo()
		{
			return await client.Repository.Get("x-hax", "sadx-mod-loader");
		}

		private async Task<GitHubCommit> GetLastCommit(long repoID, string branchName)
		{
			return await client.Repository.Commit.Get(repoID, branchName);
		}

		/// <summary>
		/// Gets the most recent commit to the repo.
		/// </summary>
		/// <returns>Most Recent Commit as a string</returns>
		public async Task GetRecentCommit()
		{

			if (client is null)
				return;

			try
			{
				var Repo = await client.Repository.Get("x-hax", "sadx-mod-loader");

				if (Repo is null)
					return;

				var id = Repo.Id;
				var lastCommit = await GetLastCommit(id, "wpf"); //todo swap to "master"

				if (lastCommit is not null)
				{
					LastCommit = lastCommit.Sha[..7];
					Window.SetModManagerVersion();	
					return;
				}
			}
			catch
			{

				LastCommit = Settings.Default.LastCommit;
				Window.SetModManagerVersion();
			}
		}
	}
}
