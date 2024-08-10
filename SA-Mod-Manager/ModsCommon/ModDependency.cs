

using SAModManager.Codes;
using SAModManager.Configuration;
using SAModManager.UI;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SAModManager.ModsCommon
{
	public class ModDependency
	{
		public string ID { get; set; }
		public string Folder { get; set; }
		public string Name { get; set; }
		public string Link { get; set; }

		public ModDependency(string depdency)
		{
			string[] strings = depdency.Split('|');

			ID = strings[0];
			Folder = strings[1];
			Name = strings[2];
			Link = strings[3];
		}

		public string GetDependencyName()
		{
			if (Name != "")
				return Name;
			else if (Folder != "")
				return Folder;
			else if (ID != "")
				return ID;
			else
				return "[Mod Name Not Provided]";
		}

		public static Dictionary<string, string> GetModReferences(Dictionary<string, SAModInfo> mods)
		{
			Dictionary<string, string> activeMods = new Dictionary<string, string>();

			foreach (string mod in mods.Keys)
			{
				SAModInfo modinfo = mods[mod];
				string id = mod;
				if (modinfo.ModID != null)
					id = modinfo.ModID;
				if (!activeMods.ContainsKey(id))
					activeMods.Add(id, mod);
				else
				{
					StringBuilder sb = new StringBuilder();
					sb.AppendLine(Lang.GetString("ModDependency.Duplicate.DuplicatesFound1"));
					sb.AppendLine($"{Lang.GetString("CommonStrings.Name")}: {modinfo.Name}, {Lang.GetString("CommonStrings.ID")}: {id}");
					sb.AppendLine(Lang.GetString("ModDependency.Duplicate.DuplicatesFound2"));
					
					new MessageWindow(Lang.GetString("ModDependency.Duplicate.Header"), sb.ToString(), icon: MessageWindow.Icons.Error).ShowDialog();
				}
			}

			return activeMods;
		}

		public static bool CheckDependencies(List<string> modlist, Dictionary<string, SAModInfo> allmods)
		{
			bool check = false;
			Dictionary<string, string> cMods = GetModReferences(allmods);

			if (allmods is null || allmods.Count == 0)
				return true;

			foreach (string checkmod in modlist)
			{
				SAModInfo mod = allmods[checkmod];
				if (mod.Dependencies.Count > 0)
				{
					int mID = modlist.IndexOf(checkmod);
					foreach (string sDependency in mod.Dependencies)
					{
                        ModDependency dependency = new ModDependency(sDependency);
						if (dependency.ID == "" && dependency.Folder == "")
							return false;

						string depName = dependency.GetDependencyName();

						bool modExists = false;
						if (cMods.ContainsKey(dependency.ID))
							modExists = true;
						else if (cMods.ContainsValue(dependency.Folder))
							modExists = true;
						else if (cMods.ContainsValue(dependency.Name))
						{
							dependency.Folder = dependency.Name;
							modExists = true;
						}

						StringBuilder sb = new StringBuilder();
						if (modExists)
						{
							// If Dependency Mod Exists, check if mod is active.
							if (modlist.Contains(dependency.Folder))
							{
								int cID = modlist.IndexOf(dependency.Folder);
								if (mID < cID)
								{
									// Dependency Mod is enabled, but is lower in priority.

									sb.AppendLine($"{depName} {Lang.GetString("ModDependency.Dependency.EnabledOrder")} {mod.Name}" + ".");
                                    new MessageWindow(Lang.GetString("ModDependency.Dependency.Header"), sb.ToString(), button: MessageWindow.Buttons.OKCancel).ShowDialog();
									check = true;
								}
							}
							else
							{
								// Dependency Mod is not enabled but exists.

								sb.AppendLine($"{depName} {Lang.GetString("ModDependency.Dependency.DisabledOrder1")}");
								sb.AppendLine($"{Lang.GetString("ModDependency.Dependency.DisabledOrder2")} {mod.Name}" + ".");
								new MessageWindow(Lang.GetString("ModDependency.Dependency.Header"), sb.ToString(), button: MessageWindow.Buttons.OKCancel).ShowDialog();
								check = true;
							}
						}
						else
						{
							// Dependency Mod is not enabled.
							sb.AppendLine($"{Lang.GetString("CommonStrings.Dependency")} {depName} {Lang.GetString("ModDependency.Dependency.NotInstalled1")}");
							sb.AppendLine($"{Lang.GetString("ModDependency.Dependency.NotInstalled2")} {depName} {Lang.GetString("ModDependency.Dependency.NotInstalled3")} {mod.Name}" + ".");
							
							if (dependency.Link == "")
							{
								// Dependency Mod does not have a link provided.
								new MessageWindow(Lang.GetString("ModDependency.Dependency.Header"), sb.ToString(), button: MessageWindow.Buttons.OKCancel).ShowDialog();
								check = true;
							}
							else
							{
								// Dependency Mod has a link and we offer to take the user to the download directly.
								sb.AppendLine(Lang.GetString("ModDependency.Dependency.NotInstalled4"));
								MessageWindow dg = new MessageWindow(Lang.GetString("ModDependency.Dependency.Header"), sb.ToString(), button: MessageWindow.Buttons.YesNo);
								dg.ShowDialog();
								if (dg.isYes)
								{
									var ps = new ProcessStartInfo(dependency.Link)
									{
										UseShellExecute = true,
										Verb = "open"
									};
									Process.Start(ps);
								}
								check = true;
							}
						}
					}
				}
			}

			return check;
		}
	}
}
