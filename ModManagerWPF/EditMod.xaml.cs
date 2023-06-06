using IniFile;
using ModManagerCommon;
using ModManagerWPF.Properties;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Input;

namespace ModManagerWPF
{
	/// <summary>
	/// Interaction logic for EditMod.xaml
	/// </summary>
	public partial class EditMod : Window
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

		#region Variables
		static bool editMod { get; set; } = false;
		public static SADXModInfo Mod { get; set; }
		static string CurrentTime = string.Empty;

		public static List<ModDependency> dependencies = new List<ModDependency>();
		SelectDependencies selectWindow;
		#endregion

		#region Initializer
		public EditMod(SADXModInfo mod)
		{
			InitializeComponent();

			AddColon(modName);
			AddColon(modAuthor);
			AddColon(modVersion);
			AddColon(modCategory);

			editMod = mod is not null;
			CurrentTime = ((uint)DateTime.Now.GetHashCode()).ToString();

			if (editMod)
			{
				Mod = mod;
				Title = Lang.GetString("EditModTitle") + " " + mod.Name;

				//get and assign mod Information
				nameBox.Text = mod.Name;
				authorBox.Text = mod.Author;

				authorURLBox.Text = mod.AuthorURL;
				versionBox.Text = mod.Version;
				descriptionBox.Text = mod.Description;
				categoryBox.SelectedIndex = SADXModInfo.ModCategory.IndexOf(mod.Category);
				modIDBox.Text = mod.ModID;
				dllText.Text = mod.DLLFile;
				sourceURLBox.Text = mod.SourceCode;
				mainSaveBox.IsChecked = mod.RedirectMainSave;
				chaoSaveBox.IsChecked = mod.RedirectChaoSave;

				LoadModUpdates(mod);
				LoadDependencies(mod);

				openFolderChk.IsChecked = false;
			}
			else
			{
				Title = Lang.GetString("NewModTitle");
				authorBox.Text = Settings.Default.ModAuthor;
				versionBox.Text = "0.1";
			}

			DataContext = new SADXModInfo();

			DependencyGrid.ItemsSource = dependencies;
		}
		#endregion

		#region Form Functions

		#region Main Window Functions
		private void okBtn_Click(object sender, RoutedEventArgs e)
		{
			string moddir = editMod ? MainWindow.modDirectory : Path.Combine(MainWindow.modDirectory, ValidateFilename(nameBox.Text));

			if (nameBox.Text.Length <= 0)
			{
				MessageBox.Show(Lang.GetString("ErrorNoNameSet"), Lang.GetString("ErrorNoNameSetTitle"), MessageBoxButton.OK, MessageBoxImage.Error);
				return;
			}

			if (editMod)
			{
				ModifyMod(moddir);
			}
			else
			{
				CreateNewMod(moddir);
			}

		}

		private void cancelBtn_Click(object sender, RoutedEventArgs e)
		{
			this.Close();
		}
		#endregion

		#region Properties Tab Functions
		private void nameBox_TextChanged(object sender, System.Windows.Controls.TextChangedEventArgs e)
		{
			if (!string.IsNullOrEmpty(nameBox.Text))
			{
				modIDBox.Text = GenerateModID();
			}
			else
			{
				modIDBox.Text = string.Empty;
			}
		}
		#endregion

		#region Updates Tab Functions
		private void radGithub_Checked(object sender, RoutedEventArgs e)
		{
			gridUpdates.RowDefinitions[1].Height = new GridLength(1, GridUnitType.Star);
			gridUpdates.RowDefinitions[2].Height = new GridLength(0);
			gridUpdates.RowDefinitions[3].Height = new GridLength(0);
		}

		private void radGamebanana_Checked(object sender, RoutedEventArgs e)
		{
			gridUpdates.RowDefinitions[1].Height = new GridLength(0);
			gridUpdates.RowDefinitions[2].Height = new GridLength(1, GridUnitType.Star);
			gridUpdates.RowDefinitions[3].Height = new GridLength(0);
		}

		private void radSelf_Checked(object sender, RoutedEventArgs e)
		{
			gridUpdates.RowDefinitions[1].Height = new GridLength(0);
			gridUpdates.RowDefinitions[2].Height = new GridLength(0);
			gridUpdates.RowDefinitions[3].Height = new GridLength(1, GridUnitType.Star);
		}
		#endregion

		#region Dependency Tab Functions

		#endregion

		#endregion

		static private void AddColon(System.Windows.Controls.Label lab)
		{
			lab.Content += ":";
		}

		string GenerateModID()
		{
			return "sadx." + nameBox.Text + "." + CurrentTime;
		}

		static string ValidateFilename(string filename)
		{
			string result = filename;

			foreach (char c in Path.GetInvalidFileNameChars())
				result = result.Replace(c, '_');

			return result;
		}

		static bool isStringNotEmpty(string txt)
		{
			return string.IsNullOrWhiteSpace(txt) == false;
		}

		static string RemoveSpecialCharacters(string str)
		{
			StringBuilder sb = new StringBuilder();
			foreach (char c in str)
			{
				if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '.' || c == '_' || c == '-')
				{
					sb.Append(c);
				}
			}
			return sb.ToString().ToLowerInvariant();
		}

		private void HandleSaveRedirection(string modDirectory)
		{
			var fullSavepath = Path.Combine(modDirectory, "SAVEDATA");
			bool saveDirExist = Directory.Exists(fullSavepath);

			if ((bool)!mainSaveBox.IsChecked && (bool)!chaoSaveBox.IsChecked)
			{
				if (saveDirExist)
				{
					//if user unchecked save redirect and the mod has existing save files, throw warning
					if (Directory.GetFiles(fullSavepath, "*.snc").Length > 0)
					{
						var dialogue = MessageBox.Show(Lang.GetString("WarningDelSaveRedir"), Lang.GetString("Warning"), MessageBoxButton.YesNo, MessageBoxImage.Warning);

						if (dialogue != MessageBoxResult.Yes)
						{
							return;
						}
					}

					//delete existing savedata folder in mod directory
					Directory.Delete(fullSavepath, true);
				}
			}
			else
			{
				if (!saveDirExist)
				{
					Directory.CreateDirectory(fullSavepath);
				}
			}
		}

		private void BuildModINI(string moddir)
		{
			//Assign variables to null if the string are empty so they won't show up at all in mod.ini.
			SADXModInfo newMod = editMod ? Mod : new SADXModInfo();

			newMod.Name = nameBox.Text;
			newMod.Author = isStringNotEmpty(authorBox.Text) ? authorBox.Text : null;
			newMod.AuthorURL = isStringNotEmpty(authorURLBox.Text) ? authorURLBox.Text : null;
			newMod.Description = isStringNotEmpty(descriptionBox.Text) ? descriptionBox.Text : null;
			newMod.Version = isStringNotEmpty(versionBox.Text) ? versionBox.Text : null;
			newMod.Category = isStringNotEmpty(categoryBox.Text) ? categoryBox.Text : null;
			newMod.SourceCode = isStringNotEmpty(sourceURLBox.Text) ? sourceURLBox.Text : null;
			newMod.RedirectMainSave = (bool)mainSaveBox.IsChecked;
			newMod.RedirectChaoSave = (bool)chaoSaveBox.IsChecked;
			newMod.ModID = isStringNotEmpty(modIDBox.Text) ? modIDBox.Text : null;
			newMod.DLLFile = isStringNotEmpty(dllText.Text) ? dllText.Text : null;

			SaveModUpdates(newMod);

			var modIniPath = Path.Combine(moddir, "mod.ini");
			IniSerializer.Serialize(newMod, modIniPath);
		}

		private void ModifyMod(string modPath)
		{
			string fullName = string.Empty;

			if (Mod is not null)
			{
				//browse the mods folder and get each mod name by their ini file
				foreach (string filename in SADXModInfo.GetModFiles(new DirectoryInfo(modPath)))
				{
					SADXModInfo mod = IniSerializer.Deserialize<SADXModInfo>(filename);
					if (mod.Name == Mod.Name)
					{
						fullName = filename;
					}
				}

				var modDirectory = Path.GetDirectoryName(fullName);

				HandleSaveRedirection(modDirectory);

				if (File.Exists(fullName))
					BuildModINI(modDirectory);


				if ((bool)openFolderChk.IsChecked)
				{
					var psi = new ProcessStartInfo() { FileName = Path.GetDirectoryName(fullName), UseShellExecute = true };
					Process.Start(psi);
				}
			}

			Close();
		}

		private void CreateNewMod(string moddir)
		{
			try
			{
				if (Directory.Exists(moddir))
				{
					MessageBox.Show(Lang.GetString("ErrorModDuplicate"), Lang.GetString("ErrorModDuplicateTitle"), MessageBoxButton.OK, MessageBoxImage.Asterisk);
					return;
				}

				Directory.CreateDirectory(moddir);

				if ((bool)mainSaveBox.IsChecked || (bool)chaoSaveBox.IsChecked)
				{
					Directory.CreateDirectory(Path.Combine(moddir, "SAVEDATA"));
				}

				if (categoryBox.Text == "Music")
				{
					Directory.CreateDirectory(@Path.Combine(moddir, "system/SoundData/bgm/wma"));
				}

				if (categoryBox.Text == "Sound")
				{
					Directory.CreateDirectory(@Path.Combine(moddir, "system/SoundData/SE"));
				}

				if (categoryBox.Text == "Textures")
				{
					Directory.CreateDirectory(Path.Combine(moddir, "textures"));
				}

				if (isStringNotEmpty(authorBox.Text)) //save mod author
				{
					Settings.Default.ModAuthor = authorBox.Text;
				}

				BuildModINI(moddir);

				if ((bool)openFolderChk.IsChecked)
				{
					var psi = new ProcessStartInfo() { FileName = moddir, UseShellExecute = true };
					Process.Start(psi);
				}

				Close();
			}
			catch (Exception error)
			{
				MessageBox.Show(this, error.Message, Lang.GetString("ErrorModCreation"), MessageBoxButton.OK, MessageBoxImage.Error);
			}

		}

		private void LoadModUpdates(SADXModInfo mod)
		{
			if (mod.GitHubRepo != null)
			{
				radGithub.IsChecked = true;
				boxGitURL.Text = mod.GitHubRepo;
				boxGitAsset.Text = mod.GitHubAsset;
			}
			else if (mod.GameBananaItemId != null)
			{
				radGamebanana.IsChecked = true;
				boxGBURL.Text = mod.GameBananaItemId.ToString();
			}
			else if (mod.UpdateUrl != null)
			{
				radSelf.IsChecked = true;
				boxSelfURL.Text = mod.UpdateUrl;
				boxChangeURL.Text = mod.ChangelogUrl;
			}
			else
			{
				radGithub.IsChecked = true;
			}
		}

		private void SaveModUpdates(SADXModInfo mod)
		{
			if (radGithub.IsChecked == true && isStringNotEmpty(boxGitURL.Text))
			{
				mod.GitHubRepo = boxGitURL.Text;
				mod.GitHubAsset = boxGitAsset.Text;
			}
			else if (radGamebanana.IsChecked == true && isStringNotEmpty(boxGBURL.Text))
			{
				mod.GameBananaItemId = int.Parse(boxGBURL.Text);
			}
			else if (radSelf.IsChecked == true && isStringNotEmpty(boxSelfURL.Text))
			{
				mod.UpdateUrl = boxSelfURL.Text;
				mod.ChangelogUrl = boxChangeURL.Text;
			}
		}

		private string ConvertLink(SADXModInfo mod)
		{
			UpdateType type;
			string retLink = string.Empty;

			if (mod.GitHubRepo != null)
				type = UpdateType.Github;
			else if (mod.GameBananaItemId != 0)
				type = UpdateType.Gamebanana;
			else if (mod.UpdateUrl != null)
				type = UpdateType.Self;
			else
				type = UpdateType.Github;

			switch (type)
			{
				case UpdateType.Github:
					retLink = "https://github.com/" + mod.GitHubRepo;
					break;
				case UpdateType.Gamebanana:
					retLink = "sadxmm:https://gamebanana.com/mmdl/0,gb_itemtype:Mod,gb_itemid:" + mod.GameBananaItemId.ToString();
					break;
			}

			return retLink;
		}

		private void LoadDependencies(SADXModInfo mod)
		{
			foreach (string dep in mod.Dependencies)
			{
				dependencies.Add(new ModDependency(dep));
			}
		}

		private void btnAddDependency_Click(object sender, RoutedEventArgs e)
		{
			selectWindow = new SelectDependencies();
			selectWindow.ShowDialog();
			if (selectWindow.IsClosed)
				if (selectWindow.NeedRefresh)
					DependencyGrid.Items.Refresh();
		}

		private void btnRemDependency_Click(object sender, RoutedEventArgs e)
		{
			List<ModDependency> selected = DependencyGrid.SelectedItems.Cast<ModDependency>().ToList();
			foreach (ModDependency dep in selected)
			{
				dependencies.Remove(dep);
			}
			DependencyGrid.Items.Refresh();
		}
	}
}
