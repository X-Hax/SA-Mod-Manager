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

		/// <summary>
		/// Gets the most recent commit to the repo.
		/// </summary>
		/// <returns>Most Recent Commit as a string</returns>
		public string GetRecentCommit()
		{
			string commit = "";

			return commit;
		}

		public List<string> GetCommits()
		{
			List<string> commits = new List<string>();

			return commits;
		}

		public List<string> GetContributors()
		{
			List<string> contributors = new List<string>();

			client.Repository.

			return contributors;
		}
    }
}
