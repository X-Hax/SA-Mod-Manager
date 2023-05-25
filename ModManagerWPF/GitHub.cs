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
