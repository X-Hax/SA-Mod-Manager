using Newtonsoft.Json.Linq;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Runtime.Serialization;
using System.Threading.Tasks;

namespace SAModManager.Updater
{
	public class GameBananaItem
	{
		[JsonProperty("name")]
		public string Name { get; set; }
		[JsonProperty("Owner().name")]
		public string OwnerName { get; set; }
		[JsonProperty("screenshots")]
		public string ScreenshotsRaw { get; set; }
		[JsonProperty("text")]
		public string Body { get; set; }
		[JsonProperty("description")]
		public string Subtitle { get; set; }
		[JsonProperty("Credits().aAuthorsAndGroups()")]
		public GameBananaCreditGroups Credits { get; set; }
		[JsonProperty("Url().sGetProfileUrl()")]
		public string ProfileUrl { get; set; }
		[JsonProperty("Updates().bSubmissionHasUpdates()")]
		public bool HasUpdates { get; set; }
		[JsonProperty("Updates().aGetLatestUpdates()")]
		public GameBananaItemUpdate[] Updates { get; set; }
		[JsonProperty("Files().aFiles()")]
		public Dictionary<string, GameBananaItemFile> Files { get; set; }

		public static async Task<GameBananaItem> Load(string itemType, long itemId)
		{
			Uri uri= new($"https://api.gamebanana.com/Core/Item/Data?itemtype={itemType}&itemid={itemId}&fields=name%2COwner().name%2Ctext%2Cdescription%2CCredits().aAuthorsAndGroups()%2CUrl().sGetProfileUrl()%2CUpdates().bSubmissionHasUpdates()%2CUpdates().aGetLatestUpdates()%2CFiles().aFiles()&return_keys=1");

			string response;
			using (var client = new UpdaterWebClient())
				response = await client.DownloadStringTaskAsync(uri);

			return JsonConvert.DeserializeObject<GameBananaItem>(response);
		}
	}

	public class GameBananaItemUpdate
	{
		static readonly DateTime epoch = new DateTime(1970, 1, 1);

		[JsonProperty("_sTitle")]
		public string Title { get; set; }
		[JsonProperty("_aChangeLog")]
		public GameBananaItemUpdateChange[] Changes { get; set; }
		[JsonProperty("_sText")]
		public string Text { get; set; }
		[JsonProperty("_tsDateAdded")]
		public long DateAddedInt { get; set; }
		[JsonIgnore]
		public DateTime DateAdded { get { return epoch.AddSeconds(DateAddedInt); } }
	}

	public class GameBananaItemUpdateChange
	{
		[JsonProperty("cat")]
		public string Category { get; set; }
		[JsonProperty("text")]
		public string Text { get; set; }
	}

	public class GameBananaItemFile
	{
		[JsonProperty("_nFilesize")]
		public long Filesize { get; set; }
		[JsonProperty("_sDownloadUrl")]
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
