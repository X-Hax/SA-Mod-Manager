using SAModManager.Codes;
using SAModManager.Ini;
using SAModManager.ModsCommon;
using SAModManager.Properties;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Security.AccessControl;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using SAModManager.UI;
using SAModManager.Configuration;

namespace SAModManager
{
    /// <summary>
    /// Interaction logic for EditMod.xaml
    /// </summary>
    public partial class EditMod : Window
	{
		#region Enums
		public enum UpdateType
		{
			Github = 0,
			Gamebanana = 1,
			Self = 2,
			None = 3
		}
		#endregion

		#region Variables
		static bool editMod { get; set; } = false;
		private string folderName;
		public static SAModInfo Mod { get; set; } = new();
		static string CurrentTime = string.Empty;

		public static EditCodeList CodeList { get; set; } = new EditCodeList();

		public static List<ModDependency> dependencies = new List<ModDependency>();
		SelectDependencies selectWindow;

		public static ConfigSchema Schema = new ConfigSchema();
		public static List<string> PropertyTypes = new List<string>()
		{
			"bool",
			"int",
			"float",
			"string"
		};
		#endregion

		#region Initializer
		public EditMod(SAModInfo mod, string tag)
		{
			InitializeComponent();

			AddColon(modName);
			AddColon(modAuthor);
			AddColon(modVersion);
			AddColon(modCategory);

			editMod = mod is not null;

			if (editMod)
			{
				Mod = mod;
				Title = Lang.GetString("EditMod.Header.EditMod") + " " + mod.Name;

				//get and assign mod Information
				nameBox.Text = mod.Name;
				authorBox.Text = mod.Author;

				authorURLBox.Text = mod.AuthorURL;
				versionBox.Text = mod.Version;
				descriptionBox.Text = mod.Description;
				categoryBox.SelectedIndex = SAModInfo.ModCategory.IndexOf(mod.Category);
				modIDBox.Text = mod.ModID;
				dllText.Text = mod.DLLFile;
				sourceURLBox.Text = mod.SourceCode;
				mainSaveBox.IsChecked = mod.RedirectMainSave;
				chaoSaveBox.IsChecked = mod.RedirectChaoSave;
                folderName = tag;
                LoadModUpdates(mod);
				LoadDependencies(mod);
				LoadCodes(mod);
                LoadConfigSchema(mod.Name);

				openFolderChk.IsChecked = false;
			}
			else
			{
				Title = Lang.GetString("EditMod.Header.NewMod");
				authorBox.Text = App.ManagerSettings.ModAuthor;
				versionBox.Text = "0.1";
				bottomGrid.Children.Remove(checkAdvancedOptions);
				tabCodes.IsEnabled = false;
				tabCodes.Visibility = Visibility.Collapsed;
            }

			DataContext = new SAModInfo();

			DependencyGrid.ItemsSource = dependencies;
			GroupsTree.ItemsSource = Schema.Groups;
			GroupsTree.Tag = PropertyTypes;
			EnumsTree.ItemsSource = Schema.Enums;
			CodeListView.ItemsSource = CodeList.Codes;

			// TODO: Remove when schema builder is fully implemented.
			tabSchema.Visibility = Visibility.Collapsed;

			if (dependencies.Count > 0 || Schema.Groups.Count > 0)
			{
				checkAdvancedOptions.IsChecked = true;
				tabDepdencies.Visibility = Visibility.Visible;
				// TODO: Uncomment once schema builder is implemented.
				//tabSchema.Visibility = Visibility.Visible;
			}
			else
			{
				tabDepdencies.Visibility = Visibility.Hidden;
				// TODO: Uncomment once the schema builder is implemented.
				//tabSchema.Visibility = Visibility.Hidden;
			}

		}
		#endregion

		#region Form Functions

		#region Main Window Functions
		private void okBtn_Click(object sender, RoutedEventArgs e)
		{
			string moddir = editMod ? Path.Combine(App.CurrentGame.modDirectory, folderName) : Path.Combine(App.CurrentGame.modDirectory, ValidateFilename(nameBox.Text));

			if (nameBox.Text.Length <= 0)
			{
				new MessageWindow(Lang.GetString("MessageWindow.Errors.NoModName.Title"), Lang.GetString("MessageWindow.Errors.NoModName"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
				return;
			}

			SaveMod(moddir);

			((MainWindow)Application.Current.MainWindow).Save();

		}

		private void cancelBtn_Click(object sender, RoutedEventArgs e)
		{
			this.Close();
		}

		private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
		{
			Schema = new ConfigSchema();
			dependencies.Clear();
		}
		#endregion

		#region Properties Tab Functions
		private void nameBox_TextChanged(object sender, System.Windows.Controls.TextChangedEventArgs e)
		{
			if (!string.IsNullOrEmpty(nameBox.Text))
			{
                modIDBox.Text = GenerateModID().Replace(" ", "");
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
		private void btnAddDependency_Click(object sender, RoutedEventArgs e)
		{
			if (editMod)
			{
				selectWindow = new SelectDependencies();
				selectWindow.ShowDialog();
				if (selectWindow.IsClosed)
					if (selectWindow.NeedRefresh)
						DependencyGrid.Items.Refresh();
			}
		}

		private void btnRemDependency_Click(object sender, RoutedEventArgs e)
		{
			if (editMod)
			{
				List<ModDependency> selected = DependencyGrid.SelectedItems.Cast<ModDependency>().ToList();
				foreach (ModDependency dep in selected)
				{
					dependencies.Remove(dep);
				}
				DependencyGrid.Items.Refresh();
			}
		}
		#endregion

		#region Config Schema Tab Functions
		public void LoadConfigSchema(string modName)
		{
			string fullName = string.Empty;
			string moddir = editMod ? App.CurrentGame.modDirectory : Path.Combine(App.CurrentGame.modDirectory, ValidateFilename(nameBox.Text));

			if (Mod != null)
			{
				//browse the mods folder and get each mod name by their ini file
				foreach (string filename in SAModInfo.GetModFiles(new DirectoryInfo(moddir)))
				{
					SAModInfo mod = IniSerializer.Deserialize<SAModInfo>(filename);
					if (mod.Name == Mod.Name)
					{
						fullName = filename;
					}
				}


				string schema = Path.Combine(Path.GetDirectoryName(fullName), "configschema.xml");
				if (File.Exists(schema))
					Schema = ConfigSchema.Load(schema);
			}
		}

		public bool DeleteGroup()
		{

			return false;
		}

		private void DeleteGroup(object sender, RoutedEventArgs e)
		{
			var btn = (Button)sender;
			var item = (ConfigSchemaGroup)btn.DataContext;
			int index = GroupsTree.Items.IndexOf(item);

			Schema.Groups.Remove(Schema.Groups[index]);
			GroupsTree.Items.Refresh();
		}

		private void DeleteProperty(object sender, RoutedEventArgs e)
		{
			var btn = (Button)sender;
			var item = (ConfigSchemaProperty)btn.DataContext;



		}

		private void PropertyTypeChanged(object sender, SelectionChangedEventArgs e)
		{
			ComboBox comboBox = (ComboBox)sender;
			var btn = (Button)sender;
			var item = (ConfigSchemaGroup)btn.DataContext;

			switch (comboBox.SelectedItem)
			{
				case "string":
					break;
				case "int":
					break;
				case "float":
					break;
				case "bool":
					break;
			}
		}
		#endregion
		#endregion

		#region Mod Updates Functions
		private void LoadModUpdates(SAModInfo mod)
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

		private void SaveModUpdates(SAModInfo mod)
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
		#endregion

		#region Dependency Functions
		private void SaveModDependencies(SAModInfo mod)
		{
			mod.Dependencies.Clear();
			if (DependencyGrid.Items.Count > 0)
			{
				foreach (ModDependency dep in DependencyGrid.Items)
				{
					StringBuilder sb = new StringBuilder();
					sb.Append(dep.ID);
					sb.Append('|');
					sb.Append(dep.Folder);
					sb.Append('|');
					sb.Append(dep.Name);
					sb.Append('|');
					sb.Append(dep.Link);
					mod.Dependencies.Add(sb.ToString());
				}
			}
		}

		private void SaveCodes(string path)
		{
			if (CodeListView.Items.Count > 0)
			{
				if (Mod.Codes is null)
					Mod.Codes = "codes.lst";

				string fullPath = Path.Combine(Path.Combine(path, Mod.Codes));

				CodeList.Save(fullPath);
			}
			else
			{
				if (Mod.Codes is not null)
				{
					string codesfile = Path.Combine(path, Mod.Codes);
					if (File.Exists(codesfile))
						File.Delete(codesfile);
					Mod.Codes = null;
				}
			}
		}

		private void LoadCodes(SAModInfo mod)
		{
			if (mod.Codes is null)
				CodeList = new();
			else
			{
				string fullPath = Path.Combine(Path.Combine(App.CurrentGame.modDirectory, folderName), mod.Codes);
				if (File.Exists(fullPath))
					CodeList = EditCodeList.Load(Path.Combine(fullPath));
				else
					CodeList = new();
			}
		}

		private void LoadDependencies(SAModInfo mod)
		{
			dependencies.Clear();
			foreach (string dep in mod.Dependencies)
				dependencies.Add(new ModDependency(dep));
		}
		#endregion

		#region Build Functions
		private void NewModSetup(string moddir)
		{
			if (Directory.Exists(moddir))
			{
				new MessageWindow(Lang.GetString("MessageWindow.Errors.ModAlreadyExists.Title"), Lang.GetString("MessageWindow.Errors.ModAlreadyExists"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
				return;
			}

			Directory.CreateDirectory(moddir);

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
				App.ManagerSettings.ModAuthor = authorBox.Text;
			}
		}

		private void BuildModINI(string moddir)
		{

			//Assign variables to null if the string are empty so they won't show up at all in mod.ini.
			SAModInfo newMod = editMod ? Mod : new SAModInfo();
			newMod.Name = nameBox.Text;
			newMod.Author = GetStringContent(authorBox.Text);
			newMod.AuthorURL = GetStringContent(authorURLBox.Text);
			newMod.Description = GetStringContent(descriptionBox.Text);
			newMod.Version = GetStringContent(versionBox.Text);
			newMod.Category = GetStringContent(categoryBox.Text);
			newMod.SourceCode = GetStringContent(sourceURLBox.Text);
			newMod.RedirectMainSave = mainSaveBox.IsChecked.GetValueOrDefault();
			newMod.RedirectChaoSave = chaoSaveBox.IsChecked.GetValueOrDefault();
			newMod.ModID = GetStringContent(modIDBox.Text);
			newMod.DLLFile = GetStringContent(dllText.Text);

            SaveModUpdates(newMod);
			if (editMod)
				SaveModDependencies(newMod);

			var modIniPath = Path.Combine(moddir, "mod.ini");
			IniSerializer.Serialize(newMod, modIniPath);
		}

		private void SaveMod(string modPath)
		{
			if (!editMod)
				NewModSetup(modPath);

			HandleSaveRedirection(modPath);
			SaveCodes(modPath);
			BuildModINI(modPath);

			if ((bool)openFolderChk.IsChecked)
				Process.Start(new ProcessStartInfo { FileName = modPath, UseShellExecute = true });

			Close();
		}
		#endregion

		static private void AddColon(System.Windows.Controls.Label lab)
		{
			lab.Content += ":";
		}

		string GenerateModID()
		{
			return App.CurrentGame?.gameAbbreviation.ToLower() + "." + nameBox.Text;
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

		private string GetStringContent(string value)
		{
			return isStringNotEmpty(value) ? value : null;
		}

		private void HandleSaveRedirection(string modDirectory)
		{
			bool isSADX = App.CurrentGame?.id == Configuration.SetGame.SADX;
			bool isSA2 = App.CurrentGame?.id == Configuration.SetGame.SA2;
			string save = "SAVEDATA";

			if (isSA2)
				save = Path.Combine("gd_PC", "SAVEDATA");

			var fullSavepath = Path.Combine(modDirectory, save);
			bool saveDirExist = Directory.Exists(fullSavepath);

			if ((bool)!mainSaveBox.IsChecked && (bool)!chaoSaveBox.IsChecked)
			{
				if (saveDirExist)
				{
					//if user unchecked save redirect and the mod has existing save files, throw Warning
					if (isSADX && Directory.GetFiles(fullSavepath, "*.snc").Length > 0 || isSA2 && Directory.GetFiles(fullSavepath).Length > 0)
					{
						var dialogue = new MessageWindow(Lang.GetString("Warning"), Lang.GetString("MessageWindow.Warnings.DisableSaveRedirect"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);
						dialogue.ShowDialog();
						if (dialogue.isYes != true)
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

		private void checkAdvancedOptions_Click(object sender, RoutedEventArgs e)
		{

			if ((bool)checkAdvancedOptions.IsChecked)
			{
				tabDepdencies.Visibility = Visibility.Visible;
				// TODO: Once the schema builder is properly implemented, this can be restored.
				//tabSchema.Visibility = Visibility.Visible;
			}
			else
			{
				if (tabDepdencies.IsSelected || tabSchema.IsSelected)
				{
					tabSchema.IsSelected = false;
					tabDepdencies.IsSelected = false;
					tabProperties.IsSelected = true;
				}

				tabDepdencies.Visibility = Visibility.Hidden;
				// TODO: Also remove this comment once schema builder is finished.
				//tabSchema.Visibility = Visibility.Hidden;
			}
		}

		private void btnNewCode_Click(object sender, RoutedEventArgs e)
		{
			NewCode newCodeWindow = new();
			newCodeWindow.ShowDialog();

			if (newCodeWindow.Code != null)
			{
				CodeList.Codes.Add(newCodeWindow.Code);
			}

			SaveCodes(Path.Combine(App.CurrentGame.modDirectory, folderName));

			CodeListView.Items.Refresh();
		}

		private void btnDelCode_Click(object sender, RoutedEventArgs e)
		{
			if (CodeListView.SelectedItems.Count < 1)
				return;

			var error = new MessageWindow(Lang.GetString("CommonStrings.Warning"), Lang.GetString("MessageWindow.Warnings.NewCodeDelete"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);
			error.ShowDialog();

			if (error.isYes)
			{
				foreach (EditCode code in CodeListView.SelectedItems)
				{
					if (CodeList.Codes.Contains(code))
						CodeList.Codes.Remove(code);
				}
			}

			SaveCodes(Path.Combine(App.CurrentGame.modDirectory, folderName));

			CodeListView.Items.Refresh();
        }

		private void btnEditCode_Click(object sender, RoutedEventArgs e)
		{
			if (CodeListView.SelectedItems.Count < 1)
				return;

			NewCode editCodeWindow = new((EditCode)CodeListView.SelectedItems[0]);
			editCodeWindow.ShowDialog();

			if (editCodeWindow.Code != null)
				CodeListView.SelectedItems[0] = editCodeWindow.Code;

			SaveCodes(Path.Combine(App.CurrentGame.modDirectory, folderName));

			CodeListView.Items.Refresh();
		}
	}
}
