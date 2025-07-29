using System.Text.Json.Serialization;
using static SAMM.Utilities.Updater.WorkflowRunInfo;
using static SAMM.Utilities.Updater.GitHubAction;
using static SAMM.Utilities.Updater.GitHubArtifact;
using static SAMM.Utilities.Updater.WorkflowRunInfo.GitHubTagInfo;
using System.Text.RegularExpressions;

namespace SAMM.Utilities.Updater
{
    public class GitHubUser
    {
        [JsonPropertyName("login")]
        public string Login { get; set; }
        [JsonPropertyName("html_url")]
        public string HtmlUrl { get; set; }
    }

    public class GitHubAsset
    {
        [JsonPropertyName("name")]
        public string Name { get; set; }
        [JsonPropertyName("uploader")]
        public GitHubUser Uploader { get; set; }
        [JsonPropertyName("size")]
        public long Size { get; set; }
        [JsonPropertyName("updated_at")]
        public string Uploaded { get; set; }
        [JsonPropertyName("browser_download_url")]
        public string DownloadUrl { get; set; }
    }

    public class GitHubRelease
    {
        [JsonPropertyName("prerelease")]
        public bool PreRelease { get; set; }
        [JsonPropertyName("draft")]
        public bool Draft { get; set; }
        [JsonPropertyName("html_url")]
        public string HtmlUrl { get; set; }
        [JsonPropertyName("name")]
        public string Name { get; set; }
        [JsonPropertyName("tag_name")]
        public string TagName { get; set; }
        [JsonPropertyName("published_at")]
        public string Published { get; set; }
        [JsonPropertyName("assets")]
        public GitHubAsset[] Assets { get; set; }
        [JsonPropertyName("body")]
        public string Body { get; set; }
    }

    public class GitHubAction
    {
        [JsonPropertyName("id")]
        public long Id { get; set; }

        [JsonPropertyName("name")]
        public string Name { get; set; }

        [JsonPropertyName("status")]
        public string Status { get; set; }

        [JsonPropertyName("url")]
        public string Url { get; set; }

        public class GitHubActionList
        {
            [JsonPropertyName("workflow_runs")]
            public List<GitHubAction> Actions { get; set; }
        }
    }

    public class GitHubArtifact
    {
        [JsonPropertyName("id")]
        public long Id { get; set; }

        [JsonPropertyName("name")]
        public string Name { get; set; }

        [JsonPropertyName("size_in_bytes")]
        public long SizeInBytes { get; set; }
        [JsonPropertyName("expired")]
        public bool Expired { get; set; }

        [JsonPropertyName("url")]
        public string Url { get; set; }
        public class ArtifactList
        {
            [JsonPropertyName("artifacts")]
            public List<GitHubArtifact> Artifacts { get; set; }
        }
    }

    public class CommitInfo
    {
        // From: https://github.blog/changelog/2021-02-08-github-actions-skip-pull-request-and-push-workflows-with-skip-ci/
        private readonly string[] noCIStrings = { "[skip ci]", "[ci skip]", "[no ci]", "[skip actions]", "[actions skip]" };

        [JsonPropertyName("id")]
        public string ID { get; set; }

        [JsonPropertyName("tree_id")]
        public string TreeID { get; set; }

        [JsonPropertyName("message")]
        public string Message { get; set; }

        [JsonPropertyName("timestamp")]
        public DateTimeOffset Timestamp { get; set; }

        [JsonPropertyName("Author")]
        public AuthorInfo Author { get; set; }

        [JsonPropertyName("committer")]
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
        [JsonPropertyName("sha")]
        public string SHA { get; set; }

        [JsonPropertyName("node_id")]
        public string NodeID { get; set; }

        [JsonPropertyName("commit")]
        public CommitInfo Commit { get; set; }

        [JsonPropertyName("url")]
        public Uri URL { get; set; }

        [JsonPropertyName("html_url")]
        public Uri HTMLURL { get; set; }

        [JsonPropertyName("comments_url")]
        public Uri CommentsURL { get; set; }


        // ...
    }

    public class Commit
    {
        public string sha { get; set; }
        public CommitDetails commit { get; set; }
    }

    public class CommitAuthor
    {
        public string name { get; set; }
    }

    public class CommitDetails
    {
        public string message { get; set; }
        public CommitAuthor author { get; set; }
    }

    public class CommitComparisonResponse
    {
        public List<Commit> commits { get; set; }
    }

    public class AuthorInfo
    {
        [JsonPropertyName("name")]
        public string Name { get; set; }

        [JsonPropertyName("email")]
        public string Email { get; set; }
    }

    public class WorkflowRunInfo
    {
        [JsonPropertyName("id")]
        public ulong ID { get; set; }

        [JsonPropertyName("name")]
        public string Name { get; set; }

        [JsonPropertyName("node_id")]
        public string NodeID { get; set; }

        [JsonPropertyName("head_branch")]
        public string HeadBranch { get; set; }

        [JsonPropertyName("head_sha")]
        public string HeadSHA { get; set; }

        [JsonPropertyName("run_number")]
        public ulong RunNumber { get; set; }

        [JsonPropertyName("event")]
        public string Event { get; set; }

        [JsonPropertyName("status")]
        public string Status { get; set; }

        [JsonPropertyName("conclusion")]
        public string Conclusion { get; set; }

        [JsonPropertyName("workflow_id")]
        public ulong WorkflowID { get; set; }

        [JsonPropertyName("check_suite_id")]
        public ulong CheckSuiteID { get; set; }

        [JsonPropertyName("check_suite_node_id")]
        public string CheckSuiteNodeID { get; set; }


        // ...

        [JsonPropertyName("url")]
        public Uri URL { get; set; }

        [JsonPropertyName("html_url")]
        public Uri HTMLURL { get; set; }

        // ...

        [JsonPropertyName("created_at")]
        public DateTimeOffset CreatedAt { get; set; }

        [JsonPropertyName("updated_at")]
        public DateTimeOffset UpdatedAt { get; set; }

        // ...

        [JsonPropertyName("artifacts_url")]
        public Uri ArtifactsURL { get; set; }

        // ...

        [JsonPropertyName("head_commit")]
        public CommitInfo HeadCommit { get; set; }

        // ...

        public class WorkflowList
        {
            [JsonPropertyName("workflow_runs")]
            public List<WorkflowRunInfo> Runs { get; set; }
        }

        public class GitHubTag
        {
            [JsonPropertyName("object")]
            public GitHubTagObject Object { get; set; }
        }

        public class GitHubTagObject
        {
            [JsonPropertyName("sha")]
            public string Sha { get; set; }
        }

        public class GitHubTagInfo
        {
            [JsonPropertyName("name")]
            public string TagName { get; set; }

            [JsonPropertyName("commit")]
            public GitHubCommit Commit { get; set; }
        }

        public class GitHubCommit
        {
            [JsonPropertyName("sha")]
            public string Sha { get; set; }
        }
    }

    public static class GitHub
    {
        private static readonly string owner = "X-Hax";
        private static readonly string repo = "SA-Mod-Manager";
        private static readonly string branch = "master"; //case-sensitive


        public static async Task<GitHubAction> GetLatestAction()
        {
            var httpClient = UpdateHelper.HttpClient;

            string apiUrl = $"https://api.github.com/repos/{owner}/{repo}/actions/runs?branch={branch}&per_page=1";

            HttpResponseMessage response = await httpClient.GetAsync(apiUrl);

            if (response.IsSuccessStatusCode)
            {
                response.Content = response.Content;
                string jsonResult = await response.Content.ReadAsStringAsync();
                var actions = JsonConvert.DeserializeObject<GitHubActionList>(jsonResult);

                if (actions != null && actions.Actions.Count > 0)
                {
                    return actions.Actions[0]; //The first workflow run in the list is the most recent one
                }

            }

          ((MainWindow)App.Current.MainWindow)?.UpdateManagerStatusText("Error GitHub Action: " + response.StatusCode);
            return null;

        }


        public static async Task<List<GitHubArtifact>> GetArtifactsForAction(long actionId)
        {
            var httpClient = UpdateHelper.HttpClient;

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
            else
            {
                ((MainWindow)App.Current.MainWindow)?.UpdateManagerStatusText("Error Artifacts: " + response.StatusCode);
            }

            return null;

        }

        //left over from previous update system
        public static async Task<WorkflowRunInfo> GetLatestWorkflowRun()
        {
            var httpClient = UpdateHelper.HttpClient;

            string apiUrl = $"https://api.github.com/repos/{owner}/{repo}/actions/runs";

            try
            {
				HttpResponseMessage response = await httpClient.GetAsync(apiUrl);

				if (response.IsSuccessStatusCode)
				{
					string jsonResult = await response.Content.ReadAsStringAsync();
					var apiResponse = JsonConvert.DeserializeObject<WorkflowList>(jsonResult);

					if (apiResponse != null && apiResponse.Runs.Count > 0)
					{
						for (int i = 0; i < apiResponse.Runs.Count; i++)
						{
							if (apiResponse.Runs[i].HeadBranch.Equals(branch, StringComparison.CurrentCultureIgnoreCase)) //only get builds from the current branch.
							{
								return apiResponse.Runs[i]; //return the first build that contains the right branch, this should be the last update every time.
							}
						}
					}
				}

			    ((MainWindow)App.Current.MainWindow)?.UpdateManagerStatusText("Error WorkFlow: " + response.StatusCode);
				return null;
			}
            catch
            {
                MessageWindow msg = new MessageWindow(Lang.GetString("MessageWindow.Errors.ConnectionFailed.Title"), Lang.GetString("MessageWindow.Errors.ConnectionFailed.Message"), type: MessageWindow.WindowType.IconMessage, icon: MessageWindow.Icons.Error);
                msg.ShowDialog();
				((MainWindow)App.Current.MainWindow)?.UpdateManagerStatusText("Failed to establish Connection.");
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

        public static async Task<(GitHubAsset, string)> GetLatestManagerReleaseChannelUpdate()
        {

            try
            {
                var httpClient = UpdateHelper.HttpClient;

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
                            string version = release.TagName;

                            try
                            {
                                string pattern = @"^\w+\s+"; // This regular expression matches any word followed by one or more spaces at the beginning of the string.
                                string result = Regex.Replace(version, pattern, "").Trim();

                            }
                            catch
                            {
                                throw new Exception("Couldn't check version difference, update won't work.");
                            }


                            return (targetAsset, version);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error fetching latest release: " + ex.Message);
            }

            return (null, null);
        }


        public static async Task<(bool, string, GitHubAsset, string)> GetLatestManagerRelease()
        {
            bool hasUpdate = false;

            try
            {
                var httpClient = UpdateHelper.HttpClient;

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
                                Version managerVersion = new(App.VersionString);
                                int comparison = commitVersion.CompareTo(managerVersion);
                                hasUpdate = comparison > 0;
                            }
                            catch
                            {
                                throw new Exception("Couldn't check version difference, update won't work.");
                            }


                            return (hasUpdate, sha, targetAsset, version);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error fetching latest release: " + ex.Message);
            }

            return (false, null, null, null);
        }

        public static async Task<string> GetGitChangeLog(string hash)
        {
            var httpClient = UpdateHelper.HttpClient;

            // string apiUrl = $"https://api.github.com/repos/{owner}/{repo}/commits?sha={hash}&per_page=100&sha={branch}";


            string apiUrl = $"https://api.github.com/repos/{owner}/{repo}/compare/{App.RepoCommit}...{hash[..7]}";

            HttpResponseMessage response = await httpClient.GetAsync(apiUrl);
            string text = "No changelog found";

            if (response.IsSuccessStatusCode)
            {
                text = "";
                string jsonResult = await response.Content.ReadAsStringAsync();


                var info = JsonConvert.DeserializeObject<CommitComparisonResponse>(jsonResult);

                foreach (var commit in info.commits)
                {
                    //we skip the message "merge branch master" thing as the end user wouldn't understand
                    if (commit.commit.message.Contains("Merge branch") && text.Length > 5)
                        continue;

                    string author = commit.commit.author?.name ?? "Unknown";
                    text += $" - {commit.commit.message} - {author}\n";
                }

            }

            return text;
        }


        public static async Task<string> GetGitLoaderChangeLog(string hash)
        {
            var httpClient = UpdateHelper.HttpClient;
            string apiUrl = $"https://api.github.com/repos/{owner}/{App.CurrentGame.loader.repoName}/commits?sha={hash}&per_page=100&sha=master";

            HttpResponseMessage response = await httpClient.GetAsync(apiUrl);
            string text = "";

            if (response.IsSuccessStatusCode)
            {
                string jsonResult = await response.Content.ReadAsStringAsync();
                var info = JsonConvert.DeserializeObject<GHCommitInfo[]>(jsonResult);
                string loaderCommitPath = App.CurrentGame?.loader.mlverPath;
                string curCommitID = File.Exists(loaderCommitPath) ? File.ReadAllText(loaderCommitPath) : App.RepoCommit;

                int limit = info.ToList().FindIndex(t => t.SHA == curCommitID);

                if (limit == -1)
                    limit = info.Length;

                for (int i = 0; i < limit; ++i)
                {
                    if (info[i].Commit.IsSkipCI())
                        continue;

                    string message = info[i].Commit.Message.Replace("\r", "");
                    if (message.Contains("\n"))
                        message = message[..message.IndexOf("\n", StringComparison.Ordinal)];

                    string author = info[i].Commit.Author.Name;
                    text += $" - {message} - {author}\n";
                }
            }

            return text;
        }


        private static async Task<string> GetLastCommitHash(string repo, string branch)
        {
            try
            {
                HttpClient client = UpdateHelper.HttpClient;
                HttpResponseMessage response = await client.GetAsync($"https://api.github.com/repos/{owner}/{repo}/commits/{branch}");

                if (response.IsSuccessStatusCode)
                {
                    string content = await response.Content.ReadAsStringAsync();
                    // Parse the JSON response to get the commit hash
                    int startIndex = content.IndexOf("\"sha\":\"") + 7;
                    int endIndex = content.IndexOf("\"", startIndex);
                    return content[startIndex..endIndex];
                }
            }
            catch
            {
            }

            return null;
        }

        public static async Task<string> GetLoaderHashCommit()
        {
            return await GetLastCommitHash(App.CurrentGame.loader.repoName, branch);
        }
    }
}
