using System.Threading.Tasks;
using SAModManager.Properties;
using Newtonsoft.Json;
using System.Collections.Generic;
using System.Net.Http;
using System;
using System.Linq;
using static SAModManager.WorkflowRunInfo;
using static SAModManager.GitHubAction;
using NetCoreInstallChecker.Misc;
using static SAModManager.GitHubArtifact;

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

    public class GitHubAction
    {
        [JsonProperty("id")]
        public long Id { get; set; }

        [JsonProperty("name")]
        public string Name { get; set; }

        [JsonProperty("status")]
        public string Status { get; set; }

        [JsonProperty("url")]
        public string Url { get; set; }

        public class GitHubActionList
        {
            [JsonProperty("workflow_runs")]
            public List<GitHubAction> Actions { get; set; }
        }
    }


    public class GitHubArtifact
    {
        [JsonProperty("id")]
        public int Id { get; set; }

        [JsonProperty("name")]
        public string Name { get; set; }

        [JsonProperty("size_in_bytes")]
        public long SizeInBytes { get; set; }
        [JsonProperty("expired")]
        public bool Expired { get; set; }

        [JsonProperty("url")]
        public string Url { get; set; }
        public class ArtifactList
        {
            [JsonProperty("artifacts")]
            public List<GitHubArtifact> Artifacts { get; set; }
        }
    }

    public class CommitInfo
    {
        // From: https://github.blog/changelog/2021-02-08-github-actions-skip-pull-request-and-push-workflows-with-skip-ci/
        private readonly string[] noCIStrings = { "[skip ci]", "[ci skip]", "[no ci]", "[skip actions]", "[actions skip]" };

        [JsonProperty("id")]
        public string ID { get; set; }

        [JsonProperty("tree_id")]
        public string TreeID { get; set; }

        [JsonProperty("message")]
        public string Message { get; set; }

        [JsonProperty("timestamp")]
        public DateTimeOffset Timestamp { get; set; }

        [JsonProperty("Author")]
        public AuthorInfo Author { get; set; }

        [JsonProperty("committer")]
        public AuthorInfo Committer { get; set; }

        public bool IsSkipCI()
        {
            foreach (string noCiString in noCIStrings)
            {
                if (Message.Contains(noCiString)) return true;
            }
            return false;
        }
    }

    public class GHCommitInfo
    {
        [JsonProperty("sha")]
        public string SHA { get; set; }

        [JsonProperty("node_id")]
        public string NodeID { get; set; }

        [JsonProperty("commit")]
        public CommitInfo Commit { get; set; }

        [JsonProperty("url")]
        public Uri URL { get; set; }

        [JsonProperty("html_url")]
        public Uri HTMLURL { get; set; }

        [JsonProperty("comments_url")]
        public Uri CommentsURL { get; set; }


        // ...
    }

    public class AuthorInfo
    {
        [JsonProperty("name")]
        public string Name { get; set; }

        [JsonProperty("email")]
        public string Email { get; set; }
    }

    public class WorkflowRunInfo
    {
        [JsonProperty("id")]
        public ulong ID { get; set; }

        [JsonProperty("name")]
        public string Name { get; set; }

        [JsonProperty("node_id")]
        public string NodeID { get; set; }

        [JsonProperty("head_branch")]
        public string HeadBranch { get; set; }

        [JsonProperty("head_sha")]
        public string HeadSHA { get; set; }

        [JsonProperty("run_number")]
        public ulong RunNumber { get; set; }

        [JsonProperty("event")]
        public string Event { get; set; }

        [JsonProperty("status")]
        public string Status { get; set; }

        [JsonProperty("conclusion")]
        public string Conclusion { get; set; }

        [JsonProperty("workflow_id")]
        public ulong WorkflowID { get; set; }

        [JsonProperty("check_suite_id")]
        public ulong CheckSuiteID { get; set; }

        [JsonProperty("check_suite_node_id")]
        public string CheckSuiteNodeID { get; set; }


        // ...

        [JsonProperty("url")]
        public Uri URL { get; set; }

        [JsonProperty("html_url")]
        public Uri HTMLURL { get; set; }

        // ...

        [JsonProperty("created_at")]
        public DateTimeOffset CreatedAt { get; set; }

        [JsonProperty("updated_at")]
        public DateTimeOffset UpdatedAt { get; set; }

        // ...

        [JsonProperty("artifacts_url")]
        public Uri ArtifactsURL { get; set; }

        // ...

        [JsonProperty("head_commit")]
        public CommitInfo HeadCommit { get; set; }

        // ...

        public class WorkflowList
        {
            [JsonProperty("workflow_runs")]
            public List<WorkflowRunInfo> Runs { get; set; }
        }
    }

    public static class GitHub
    {
        public static readonly string owner = "X-Hax";
        public static readonly string repo = "SA-Mod-Manager";
        public static readonly string workflowWindows = "build";
        private static readonly string AppName = "SA Mod Manager";

        public static async Task<GitHubAction> GetLatestAction(string owner, string repo)
        {
            using (var httpClient = new HttpClient())
            {
                httpClient.DefaultRequestHeaders.Add("User-Agent", AppName);

                string apiUrl = $"https://api.github.com/repos/{owner}/{repo}/actions/runs?per_page=1";

                HttpResponseMessage response = await httpClient.GetAsync(apiUrl);

                if (response.IsSuccessStatusCode)
                {
                    response.Content = response.Content;
                    string jsonResult = await response.Content.ReadAsStringAsync();
                    var actions = JsonConvert.DeserializeObject<GitHubActionList>(jsonResult);

                    if (actions != null && actions.Actions.Count > 0)
                    {
                        return actions.Actions[0]; // The first workflow run in the list is the most recent one
                    }

                }

                Console.WriteLine($"Error: {response.StatusCode}");
                return null;
            }
        }


        public static async Task<List<GitHubArtifact>> GetArtifactsForAction(string owner, string repo, long actionId)
        {
            using (var httpClient = new HttpClient())
            {
                httpClient.DefaultRequestHeaders.Add("User-Agent", AppName); // Replace 'AwesomeApp' with your app name

                string apiUrl = $"https://api.github.com/repos/{owner}/{repo}/actions/runs/{actionId}/artifacts";

                HttpResponseMessage response = await httpClient.GetAsync(apiUrl);

                if (response.IsSuccessStatusCode)
                {
                    string jsonResult = await response.Content.ReadAsStringAsync();
                    var artifacts = JsonConvert.DeserializeObject<ArtifactList>(jsonResult);

                    if (artifacts != null && artifacts.Artifacts.Count > 0)
                    {
                        return artifacts.Artifacts; // The first workflow run in the list is the most recent one
                    }
                }

                Console.WriteLine($"Error: {response.StatusCode}");
                return null;
            }
        }

        public static async Task<WorkflowRunInfo> GetLatestWorkflowRun(string owner, string repo)
        {
            using (var httpClient = new HttpClient())
            {
                httpClient.DefaultRequestHeaders.Add("User-Agent", AppName); // Replace 'AwesomeApp' with your app name

                string apiUrl = $"https://api.github.com/repos/{owner}/{repo}/actions/runs";

                HttpResponseMessage response = await httpClient.GetAsync(apiUrl);

                if (response.IsSuccessStatusCode)
                {
                    string jsonResult = await response.Content.ReadAsStringAsync();
                    var apiResponse = JsonConvert.DeserializeObject<WorkflowList>(jsonResult);

                    if (apiResponse != null && apiResponse.Runs.Count > 0)
                    {
                        return apiResponse.Runs[0]; // The first workflow run in the list is the most recent one
                    }
                }

                Console.WriteLine($"Error: {response.StatusCode}");
                return null;
            }
        }

        public static async Task<string> GetGitChangeLog(string hash)
        {
            var httpClient = new HttpClient();

            httpClient.DefaultRequestHeaders.Add("User-Agent", AppName); // Replace 'AwesomeApp' with your app name
            string apiUrl = $"https://api.github.com/repos/{owner}/{repo}/commits?sha" + hash;

            HttpResponseMessage response = await httpClient.GetAsync(apiUrl);
            string text = "";

            if (response.IsSuccessStatusCode)
            {

                string jsonResult = await response.Content.ReadAsStringAsync();
                var info = JsonConvert.DeserializeObject<GHCommitInfo[]>(jsonResult);
              
                int limit = info.ToList().FindIndex(t => t.SHA == App.RepoCommit);
                if (limit == -1)
                    limit = info.Length;

                for (int i = 0; i < limit; ++i)
                {
                    if (info[i].Commit.IsSkipCI())
                        continue;

                    string message = info[i].Commit.Message.Replace("\r", "");
                    if (message.Contains("\n"))
                        message = message.Substring(0, message.IndexOf("\n", StringComparison.Ordinal));

                    text += $" - {info[i].SHA.Substring(0, 7)} - {message}\n";
                }


            }

            return text;
        }

    }
}
