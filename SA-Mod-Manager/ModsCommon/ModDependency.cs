

using SAModManager.Codes;
using SAModManager.Configuration;
using SAModManager.UI;
using System.CodeDom;
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

		private static List<SAModInfo> GetEnabledMods(List<string> modList, Dictionary<string, SAModInfo> allMods)
		{
			List<SAModInfo> outModList = new List<SAModInfo>();
			
			foreach (var mod in modList)
			{
				if (allMods.ContainsKey(mod))
					outModList.Add(allMods[mod]);
			}

			return outModList;
		}

		private static List<SAModInfo> GetDisabledMods(List<string> modList, Dictionary<string, SAModInfo> allMods)
		{
			List<SAModInfo> outModList = new List<SAModInfo>();

			foreach (var mod in allMods)
			{
				if (!modList.Contains(mod.Key))
					outModList.Add(mod.Value);
			}

			return outModList;
		}

		/// <summary>
		/// Checks through mod dependencies for all enabled mods, verifies dependencies are all enabled and in their correct order.
		/// 
		/// If the dependencies are not enabled or are missing, messages are triggered for the end user to inform them of the issue.
		/// </summary>
		/// <param name="enabledMods"></param>
		/// <param name="allMods"></param>
		/// <returns>False if no issues are found, True if any dependency issues are found.</returns>
		public static bool CheckDependencies(List<string> enabledMods, Dictionary<string, SAModInfo> allMods)
		{
			List<SAModInfo> enabledModInfoList = GetEnabledMods(enabledMods, allMods);
			List<SAModInfo> disabledModInfoList = GetDisabledMods(enabledMods, allMods);

			// Loop through enabled mods and check for dependencies.
			foreach (SAModInfo mod in enabledModInfoList)
			{
				// If Dependencies are 0, we move on to the next one.
				if (mod.Dependencies.Count <= 0)
					continue;

				int modIndex = enabledModInfoList.IndexOf(mod);	// Current mod's index in the list.

				foreach (string dep in mod.Dependencies)
				{
					// This variable is used to check if the mod was validated before running further checks after checking the enabled mods list.
					bool validated = false; 
					ModDependency dependency = new ModDependency(dep);
					StringBuilder sb = new StringBuilder();

					// Loop through enabled mods for dependency matching and verify their placement in the list is higher than the current mod.
					foreach (SAModInfo enabledMod in enabledModInfoList)
					{
						if (enabledMod.ModID == dependency.ID || enabledMod.Name == dependency.Name)
						{
							int depIndex = enabledModInfoList.IndexOf(enabledMod);

							// If Dependency is a lower index than the Mod's index, we can go ahead and return false.
							if (depIndex < modIndex)
							{
								validated = true;
								break;
							}
							else
							{
								// If the Dependency is not a lower index, alert the user and return true.
								sb.AppendLine($"{dependency.Name} {Lang.GetString("ModDependency.Dependency.EnabledOrder")} {mod.Name}" + ".");
								new MessageWindow(Lang.GetString("ModDependency.Dependency.Header"), sb.ToString(), button: MessageWindow.Buttons.OKCancel).ShowDialog();
								return true;
							}
						}
					}

					// If the dependency was validated in the previous check, we can exit this current loop.
					if (validated)
						break;

					// Loop through disabled mods and check for the dependency.
					foreach (SAModInfo disabledMod in disabledModInfoList)
					{
						if (disabledMod.ModID == dependency.ID || disabledMod.Name == dependency.Name)
						{
							sb.AppendLine($"{dependency.Name} {Lang.GetString("ModDependency.Dependency.DisabledOrder1")}");
							sb.AppendLine($"{Lang.GetString("ModDependency.Dependency.DisabledOrder2")} {mod.Name}" + ".");
							new MessageWindow(Lang.GetString("ModDependency.Dependency.Header"), sb.ToString(), button: MessageWindow.Buttons.OKCancel).ShowDialog();
							return true;
						}
					}

					// If the dependency does not exist in any capacity, alert the user and offer a download link if possible.
					sb.AppendLine($"{Lang.GetString("CommonStrings.Dependency")} {dependency.Name} {Lang.GetString("ModDependency.Dependency.NotInstalled1")}");
					sb.AppendLine($"{Lang.GetString("ModDependency.Dependency.NotInstalled2")} {dependency.Name} {Lang.GetString("ModDependency.Dependency.NotInstalled3")} {mod.Name}" + ".");

					if (dependency.Link == "")
					{
						// Dependency Mod does not have a link provided.
						new MessageWindow(Lang.GetString("ModDependency.Dependency.Header"), sb.ToString(), button: MessageWindow.Buttons.OKCancel).ShowDialog();
						return true;
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
						return true;
					}
				}
			}

			// If we get here, return false so the game will boot as usual.
			return false;
		}
	}
}
