using System.Threading.Tasks;
using Newtonsoft.Json;
using System.Collections.Generic;
using System.Net.Http;
using System;
using System.Linq;
using static SAModManager.WorkflowRunInfo;
using static SAModManager.GitHubAction;
using static SAModManager.GitHubArtifact;
using static SAModManager.WorkflowRunInfo.GitHubTagInfo;
using System.Text.RegularExpressions;

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
                if (Message.Contains(noCiString))
                    return true;
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

    public class Commit
    {
        public string sha { get; set; }
        public CommitDetails commit { get; set; }
    }

    public class CommitDetails
    {
        public string message { get; set; }
    }

    public class CommitComparisonResponse
    {
        public List<Commit> commits { get; set; }
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

        public class GitHubTag
        {
            [JsonProperty("object")]
            public GitHubTagObject Object { get; set; }
        }

        public class GitHubTagObject
        {
            [JsonProperty("sha")]
            public string Sha { get; set; }
        }

        public class GitHubTagInfo
        {
            [JsonProperty("name")]
            public string TagName { get; set; }

            [JsonProperty("commit")]
            public GitHubCommit Commit { get; set; }
        }

        public class GitHubCommit
        {
            [JsonProperty("sha")]
            public string Sha { get; set; }
        }
    }

    public static class GitHub
    {
        private static readonly string owner = "X-Hax";
        private static readonly string repo = "SA-Mod-Manager";
        private static readonly string AppName = "SA Mod Manager";
        private static readonly string branch = "master"; //case-sensitive


        public static async Task<GitHubAction> GetLatestAction()
        {
            using (var httpClient = new HttpClient())
            {
                httpClient.DefaultRequestHeaders.Add("User-Agent", AppName);

                string apiUrl = $"https://api.github.com/repos/{owner}/{repo}/actions/runs?branch={branch}&per_page=1";

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


        public static async Task<List<GitHubArtifact>> GetArtifactsForAction(long actionId)
        {
            using (var httpClient = new HttpClient())
            {
                httpClient.DefaultRequestHeaders.Add("User-Agent", AppName);

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

        //left over from previous update system
        public static async Task<WorkflowRunInfo> GetLatestWorkflowRun()
        {
            using (var httpClient = new HttpClient())
            {
                httpClient.DefaultRequestHeaders.Add("User-Agent", AppName);

                string apiUrl = $"https://api.github.com/repos/{owner}/{repo}/actions/runs";

                HttpResponseMessage response = await httpClient.GetAsync(apiUrl);

                if (response.IsSuccessStatusCode)
                {
                    string jsonResult = await response.Content.ReadAsStringAsync();
                    var apiResponse = JsonConvert.DeserializeObject<WorkflowList>(jsonResult);

                    if (apiResponse != null && apiResponse.Runs.Count > 0)
                    {
                        for (int i = 0; i < apiResponse.Runs.Count; i++)
                        {
                            if (apiResponse.Runs[i].HeadBranch.ToLower() == branch.ToLower()) //only get builds from the current branch.
                            {
                                return apiResponse.Runs[i]; //return the first build that contains the right branch, this should be the last update every time.
                            }
                        }
                    }
                }

                Console.WriteLine($"Error: {response.StatusCode}");
                return null;
            }
        }

        private static async Task<string> GetSHAFromLastTag(GitHubRelease release, HttpClient httpClient)
        {
            string lastTagName = release.TagName;
            string urlTag = $"https://api.github.com/repos/{owner}/{repo}/tags";
            using var responseHtmlTAG = await httpClient.GetAsync(urlTag);

            if (!responseHtmlTAG.IsSuccessStatusCode)
                return null;

            var tagResponse = await responseHtmlTAG.Content.ReadAsStringAsync();
            var tagInfo = JsonConvert.DeserializeObject<List<GitHubTagInfo>>(tagResponse);
            var lastTag = tagInfo?.FirstOrDefault(hash => hash.TagName.Contains(lastTagName));

            return lastTag?.Commit.Sha;
        }


        public static async Task<(bool, string, GitHubAsset)> GetLatestManagerRelease()
        {
            bool hasUpdate = false;

            try
            {
                var httpClient = new HttpClient();

                httpClient.DefaultRequestHeaders.Add("User-Agent", AppName);
                string apiUrl = $"https://api.github.com/repos/{owner}/{repo}/releases/latest";

                HttpResponseMessage response = await httpClient.GetAsync(apiUrl);

                if (response.IsSuccessStatusCode)
                {
                    string responseBody = await response.Content.ReadAsStringAsync();
                    var release = JsonConvert.DeserializeObject<GitHubRelease>(responseBody);
                    if (release != null && release.Assets != null)
                    {
                        var targetAsset = release.Assets.FirstOrDefault(asset => asset.Name.Contains(Environment.Is64BitOperatingSystem ? "x64" : "x86"));
                        if (targetAsset != null)
                        {
                            string sha = await GetSHAFromLastTag(release, httpClient);
                            string version = release.TagName;

                            try
                            {
                                string pattern = @"^\w+\s+"; // This regular expression matches any word followed by one or more spaces at the beginning of the string.
                                string result = Regex.Replace(version, pattern, "").Trim();
                                Version commitVersion = new(result);
                                int comparison = commitVersion.CompareTo(App.Version);
                                hasUpdate = comparison > 0;
                            }
                            catch
                            {
                                throw new Exception("Couldn't check version difference, update won't work.");
                            }

                            return (hasUpdate, sha, targetAsset);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error fetching latest release: " + ex.Message);
            }

            return (false, null, null);
        }

        public static async Task<string> GetGitChangeLog(string hash)
        {
            var httpClient = new HttpClient();

            httpClient.DefaultRequestHeaders.Add("User-Agent", AppName);

            // string apiUrl = $"https://api.github.com/repos/{owner}/{repo}/commits?sha={hash}&per_page=100&sha={branch}";

            bool isEmpty = string.IsNullOrEmpty(App.RepoCommit); //empty means we are on dev version
            string apiUrl = isEmpty == false ? $"https://api.github.com/repos/{owner}/{repo}/compare/{App.RepoCommit}...{hash[..7]}" : $"https://api.github.com/repos/{owner}/{repo}/commits?sha={hash}&per_page=100&sha={branch}"; 

            HttpResponseMessage response = await httpClient.GetAsync(apiUrl);
            string text = "";

            if (response.IsSuccessStatusCode)
            {
                string jsonResult = await response.Content.ReadAsStringAsync();
          
                if (isEmpty) //if we are on dev version get the last 100 commits
                {
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
                            message = message[..message.IndexOf("\n", StringComparison.Ordinal)];

                        text += " - " + message + "\n";
                    }
                }
                else //get all the commits between new version and current version
                {
                    var info = JsonConvert.DeserializeObject<CommitComparisonResponse>(jsonResult);
                    
                    foreach (var commit in info.commits)
                    {
                        //we skip the message "merge branch master" thing as the end user wouldn't understand
                        if (commit.commit.message.Contains("Merge branch") && text.Length > 5)
                            continue;

                        text += " - " + commit.commit.message + "\n";
                    }
                }
            }

            return text;
        }


        public static async Task<string> GetGitLoaderChangeLog(string hash)
        {
            var httpClient = new HttpClient();

            httpClient.DefaultRequestHeaders.Add("User-Agent", AppName); //To do update with master when it's ready
            string apiUrl = $"https://api.github.com/repos/{owner}/{App.CurrentGame.loader.repoName}/commits?sha={hash}&per_page=100&sha=wpf";

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
                        message = message[..message.IndexOf("\n", StringComparison.Ordinal)];

                    text += $" - {info[i].SHA[..7]} - {message}\n";
                }
            }

            return text;
        }


        private static async Task<string> GetLastCommitHash(string repo, string branch)
        {
            using (HttpClient client = new())
            {
                client.BaseAddress = new Uri("https://api.github.com");
                client.DefaultRequestHeaders.Add("User-Agent", AppName);

                HttpResponseMessage response = await client.GetAsync($"repos/{owner}/{repo}/commits/{branch}");

                if (response.IsSuccessStatusCode)
                {
                    string content = await response.Content.ReadAsStringAsync();
                    // Parse the JSON response to get the commit hash
                    int startIndex = content.IndexOf("\"sha\":\"") + 7;
                    int endIndex = content.IndexOf("\"", startIndex);
                    return content[startIndex..endIndex];
                }
                else
                {
                    Console.WriteLine($"Error: {response.StatusCode}");
                    return null;
                }
            }
        }

        public static async Task<string> GetLoaderHashCommit()
        {
            return await GetLastCommitHash(App.CurrentGame.loader.repoName, "wpf");
        }
    }
}
