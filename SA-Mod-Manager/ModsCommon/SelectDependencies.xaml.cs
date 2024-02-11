using System.Windows;
using System.Collections.Generic;
using System.IO;
using SAModManager.Properties;
using System.Text;
using SAModManager.Ini;

namespace SAModManager.ModsCommon
{
	/// <summary>
	/// Interaction logic for SelectDependencies.xaml
	/// </summary>
	public partial class SelectDependencies : Window
	{
		#region Enums
		enum UpdateType
		{
			Github = 0,
			Gamebanana = 1,
			Self = 2,
			None = 3
		}
		#endregion

		public class ModToDependency
		{
			public string Name { get; set; }
			public string Author { get; set; }
			public string Folder { get; set; }
			public string ModID { get; set; }
			public string Link { get; set; }
			public bool isGithub { get; set; }
			public bool IsChecked { get; set; }

			public ModToDependency(KeyValuePair<string, SAModInfo> modInfo)
			{
				Name = modInfo.Value.Name;
				Author = modInfo.Value.Author;
				Folder = modInfo.Key;
				ModID = modInfo.Value.ModID;
				isGithub = false;

				if (modInfo.Value.GitHubRepo != null)
				{
					Link = modInfo.Value.GitHubRepo;
					isGithub = true;
				}
				else if (modInfo.Value.GameBananaItemId != null)
					Link = modInfo.Value.GameBananaItemId.ToString();
				else
					Link = string.Empty;
			}

			public ModDependency ToDepdenency()
			{
				StringBuilder sb = new StringBuilder();

				sb.Append(ModID);
				sb.Append('|');
				sb.Append(Folder);
				sb.Append('|');
				sb.Append(Name);
				sb.Append('|');
				sb.Append(Link);

				return new ModDependency(sb.ToString());
			}
		}
		Dictionary<string, SAModInfo> mods = new Dictionary<string, SAModInfo>();
		public bool IsClosed { get; set; }
		public bool NeedRefresh { get; set; }

		public SelectDependencies()
		{
			InitializeComponent();
			IsClosed = false;
			if (LoadMods())
			{
				foreach (KeyValuePair<string, SAModInfo> mod in mods)
				{
					lstModSelect.Items.Add(new ModToDependency(mod));
				}
			}
		}

		private bool LoadMods()
		{
			bool success = false;
			string modDir = Path.Combine(App.CurrentGame.gameDirectory, "mods");

			if (Directory.Exists(modDir))
			{
				success = true;

				foreach (string filename in SAModInfo.GetModFiles(new DirectoryInfo(modDir)))
				{
					SAModInfo mod = IniSerializer.Deserialize<SAModInfo>(filename);
					if (mod.Name != EditMod.Mod.Name)
						mods.Add((Path.GetDirectoryName(filename) ?? string.Empty).Substring(modDir.Length + 1), mod);
				}
			}

			return success;
		}

		private bool CheckDependencies(ModDependency dependency)
		{
			bool modExists = false;

			if (EditMod.dependencies.Count > 0)
			{
				foreach (ModDependency mod in EditMod.dependencies)
				{
					if (mod.ID == dependency.ID)
					{
						if (mod.Name == dependency.Name)
						{
							modExists = false; break;
						}
						else
							modExists = true;
					}
					else
						modExists = true;
				}
			}

			return modExists;
		}

		private string ConvertLink(ModToDependency mod)
		{
			UpdateType type;
			string retLink = string.Empty;

			if (mod.isGithub)
				type = UpdateType.Github;
			else if (!mod.isGithub && mod.Link != string.Empty)
				type = UpdateType.Gamebanana;
			else 
				type = UpdateType.Self;

			switch (type)
			{
				case UpdateType.Github:
					retLink = "https://github.com/" + mod.Link;
					break;
				case UpdateType.Gamebanana:
					retLink = "sadxmm:https://gamebanana.com/mmdl/0,gb_itemtype:Mod,gb_itemid:" + mod.Link;
					break;
			}

			return retLink;
		}

		private void GenerateDependencies()
		{
			foreach (ModToDependency mod in lstModSelect.Items)
			{
				if (mod.IsChecked)
				{
					ModDependency dependency = mod.ToDepdenency();
					dependency.Link = ConvertLink(mod);

					if (EditMod.dependencies.Count <= 0 || CheckDependencies(dependency))
						EditMod.dependencies.Add(dependency);
				}
			}
		}

		private void OK_Click(object sender, RoutedEventArgs e)
		{
			GenerateDependencies();
			NeedRefresh = true;
			this.Close();
		}

		private void Cancel_Click(object sender, RoutedEventArgs e)
		{
			this.Close();
		}

		private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			IsClosed = true;
		}
	}
}
