using SAModManager.Common;
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
		public static SADXModInfo Mod { get; set; }
		static string CurrentTime = string.Empty;

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
				Title = Lang.GetString("EditMod.Header.EditMod") + " " + mod.Name;

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
				LoadConfigSchema(mod.Name);

				openFolderChk.IsChecked = false;
			}
			else
			{
				Title = Lang.GetString("EditMod.Header.NewMod");
				authorBox.Text = App.configIni.ModAuthor;
				versionBox.Text = "0.1";
				bottomGrid.Children.Remove(checkAdvancedOptions);
			}

			DataContext = new SADXModInfo();

			DependencyGrid.ItemsSource = dependencies;
			GroupsTree.ItemsSource = Schema.Groups;
			GroupsTree.Tag = PropertyTypes;
			EnumsTree.ItemsSource = Schema.Enums;


			if (dependencies.Count > 0 || Schema.Groups.Count > 0)
			{
				checkAdvancedOptions.IsChecked = true;
				tabDepdencies.Visibility = Visibility.Visible;
				tabSchema.Visibility = Visibility.Visible;
			}
			else
			{
				tabDepdencies.Visibility = Visibility.Hidden;
				tabSchema.Visibility = Visibility.Hidden;
			}

		}
		#endregion

		#region Form Functions

		#region Main Window Functions
		private void okBtn_Click(object sender, RoutedEventArgs e)
		{
			string moddir = editMod ? App.CurrentGame.modDirectory : Path.Combine(App.CurrentGame.modDirectory, ValidateFilename(nameBox.Text));

			if (nameBox.Text.Length <= 0)
			{
				new MessageWindow(Lang.GetString("MessageWindow.Errors.NoModName.Title"), Lang.GetString("MessageWindow.Errors.NoModName"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
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
				foreach (string filename in SADXModInfo.GetModFiles(new DirectoryInfo(moddir)))
				{
					SADXModInfo mod = IniSerializer.Deserialize<SADXModInfo>(filename);
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
		#endregion

		#region Dependency Functions
		private void SaveModDependencies(SADXModInfo mod)
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

		private void LoadDependencies(SADXModInfo mod)
		{
			dependencies.Clear();
			foreach (string dep in mod.Dependencies)
			{
				dependencies.Add(new ModDependency(dep));
			}
		}
		#endregion

		#region Build Functions
		private void BuildModINI(string moddir)
		{

			//Assign variables to null if the string are empty so they won't show up at all in mod.ini.
			SADXModInfo newMod = editMod ? Mod : new SADXModInfo();
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
			SaveModDependencies(newMod);

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
					Process.Start(new ProcessStartInfo { FileName = Path.GetDirectoryName(fullName), UseShellExecute = true });
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
					new MessageWindow(Lang.GetString("MessageWindow.Errors.ModAlreadyExists.Title"), Lang.GetString("MessageWindow.Errors.ModAlreadyExists"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
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
					App.configIni.ModAuthor = authorBox.Text;
				}

				BuildModINI(moddir);

				if ((bool)openFolderChk.IsChecked)
				{
					var psi = new ProcessStartInfo() { FileName = moddir, UseShellExecute = true };
					Process.Start(psi);
				}

				Close();
			}
			catch (Exception Error)
			{
				new MessageWindow(Lang.GetString("MessageWindow.Errors.ModCreationFailed.Title"), Error.Message, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
			}

		}
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

		private string GetStringContent(string value)
		{
			return isStringNotEmpty(value) ? value : null;
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
					//if user unchecked save redirect and the mod has existing save files, throw Warning
					if (Directory.GetFiles(fullSavepath, "*.snc").Length > 0)
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
				tabSchema.Visibility = Visibility.Visible;
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
				tabSchema.Visibility = Visibility.Hidden;
			}
		}

		private void btnNewCode_Click(object sender, RoutedEventArgs e)
		{

		}

		private void btnDelCode_Click(object sender, RoutedEventArgs e)
		{

		}

		private void btnEditCode_Click(object sender, RoutedEventArgs e)
		{

		}
	}
}
