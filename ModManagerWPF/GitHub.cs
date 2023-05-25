using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Octokit;

namespace ModManagerWPF
{
    public class GitHub
    {
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

			var Repo = await client.Repository.Get("x-hax", "sadx-mod-loader");

			if (Repo is null) 
				return;

			var id = Repo.Id;
			var lastCommit = await GetLastCommit(id, "wpf"); //todo swap to "master"
		
			if (lastCommit is not null)
			{
				LastCommit = lastCommit.Sha[..7];
				return;
			}
		}

		/// <summary>
		/// Gets full commit history and returns a list.
		/// </summary>
		/// <returns></returns>
		public List<string> GetCommits()
		{
			List<string> commits = new List<string>();

			return commits;
		}

		/// <summary>
		/// Gets full list of contributors to the repo and returns the list.
		/// </summary>
		/// <returns></returns>
		public List<string> GetContributors()
		{
			List<string> contributors = new List<string>();

			return contributors;
		}
    }
}
