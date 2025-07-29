using System.Text.Json.Serialization;
using System.Runtime.Serialization;

namespace SAMM.Utilities.Updater
{
	public class GameBananaItem
	{
		[JsonPropertyName("name")]
		public string Name { get; set; }
		[JsonPropertyName("Owner().name")]
		public string OwnerName { get; set; }
		[JsonPropertyName("screenshots")]
		public string ScreenshotsRaw { get; set; }
		[JsonPropertyName("text")]
		public string Body { get; set; }
		[JsonPropertyName("description")]
		public string Subtitle { get; set; }
		[JsonPropertyName("Credits().aAuthorsAndGroups()")]
		public GameBananaCreditGroups Credits { get; set; }
		[JsonPropertyName("Url().sGetProfileUrl()")]
		public string ProfileUrl { get; set; }
		[JsonPropertyName("Updates().bSubmissionHasUpdates()")]
		public bool HasUpdates { get; set; }
		[JsonPropertyName("Updates().aGetLatestUpdates()")]
		public GameBananaItemUpdate[] Updates { get; set; }
		[JsonPropertyName("Files().aFiles()")]
		public Dictionary<string, GameBananaItemFile> Files { get; set; }

		public static async Task<GameBananaItem> Load(string itemType, long itemId)
		{
			Uri uri= new($"https://api.gamebanana.com/Core/Item/Data?itemtype={itemType}&itemid={itemId}&fields=name%2COwner().name%2Ctext%2Cdescription%2CCredits().aAuthorsAndGroups()%2CUrl().sGetProfileUrl()%2CUpdates().bSubmissionHasUpdates()%2CUpdates().aGetLatestUpdates()%2CFiles().aFiles()&return_keys=1");

			string response;
			var client = UpdateHelper.HttpClient;
			response = await client.GetStringAsync(uri);

			return JsonConvert.DeserializeObject<GameBananaItem>(response);
		}
	}

	public class GameBananaItemUpdate
	{
        static readonly DateTime epoch = new(1970, 1, 1);

		[JsonPropertyName("_sTitle")]
		public string Title { get; set; }
		[JsonPropertyName("_aChangeLog")]
		public GameBananaItemUpdateChange[] Changes { get; set; }
		[JsonPropertyName("_sText")]
		public string Text { get; set; }
		[JsonPropertyName("_tsDateAdded")]
		public long DateAddedInt { get; set; }
		[JsonIgnore]
		public DateTime DateAdded { get { return epoch.AddSeconds(DateAddedInt); } }
	}

	public class GameBananaItemUpdateChange
	{
		[JsonPropertyName("cat")]
		public string Category { get; set; }
		[JsonPropertyName("text")]
		public string Text { get; set; }
	}

	public class GameBananaItemFile
	{
		[JsonPropertyName("_nFilesize")]
		public long Filesize { get; set; }
		[JsonPropertyName("_sDownloadUrl")]
		public string DownloadUrl { get; set; }
	}

	public class GameBananaCredit
	{
		public string MemberName { get; set; }
		public string Role { get; set; }
		public int MemberID { get; set; }
	}

	public class GameBananaCreditGroups
	{
		[JsonIgnoreAttribute]
		public Dictionary<string, List<GameBananaCredit>> Credits = new Dictionary<string, List<GameBananaCredit>>();

		[JsonExtensionData]
		private Dictionary<string, JToken> CreditsData { get; set; }

		[OnDeserialized]
		private void OnDeserialized(StreamingContext context)
		{
			foreach (var credit in CreditsData)
			{
				var credits = new List<GameBananaCredit>();

				foreach (var cred in credit.Value)
				{
					credits.Add(new GameBananaCredit()
					{
						MemberName = cred[0].ToString(),
						Role = cred[1].ToString(),
						MemberID = cred[2].ToObject<int>()
					});
				}

				Credits.Add(credit.Key, credits);
			}
		}
	}
}
