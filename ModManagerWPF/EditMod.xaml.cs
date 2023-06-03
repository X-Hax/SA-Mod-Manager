using IniFile;
using ModManagerCommon;
using ModManagerWPF.Properties;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;

using System.Text;
using System.Windows;

using static System.Net.Mime.MediaTypeNames;



namespace ModManagerWPF
{
	/// <summary>
	/// Interaction logic for EditMod.xaml
	/// </summary>
	public partial class EditMod : Window
	{
		static bool editMod { get; set; } = false;
		static SADXModInfo Mod { get; set; }
		static string CurrentTime = string.Empty;
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
				nameBox.Text = mod.Name;
				authorBox.Text = mod.Author;
				//authorURLBox.Text = mod.AuthorURL;
				versionBox.Text = mod.Version;
				descriptionBox.Text = mod.Description;
				categoryBox.SelectedIndex = SADXModInfo.ModCategory.IndexOf(mod.Category);
				modIDBox.Text = mod.ModID;
				dllText.Text = mod.DLLFile;
			}
			else
			{
				Title = Lang.GetString("NewModTitle");
				authorBox.Text = Settings.Default.ModAuthor;
				versionBox.Text = "0.1";
			}

			DataContext = new SADXModInfo();
		}

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
			return txt.Length > 0;
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

		private void buttonGenerate_Click(object sender, EventArgs e)
		{
			/*textID.Clear();
			string name = isStringNotEmpty(textModName.Text) ? textModName.Text : null;
			string author = isStringNotEmpty(textModAuthor.Text) ? textModAuthor.Text : null;

			if (name != null && author != null)
			{
				string idName = RemoveSpecialCharacters(name);
				string idAuthor = RemoveSpecialCharacters(author);
				textID.Text = String.Format("sadx.{0}.{1}", idAuthor, idName);
			}
			else
				textID.Text = GenerateModID();*/

		}

		private void BuildModINI(string moddir)
		{
			//Assign variables to null if the string are empty so they won't show up at all in mod.ini.
			SADXModInfo newMod = new SADXModInfo
			{
				Name = nameBox.Text,
				Author = isStringNotEmpty(authorBox.Text) ? authorBox.Text : null,
				Description = descriptionBox.Text.Length > 0 ? descriptionBox.Text : null,
				Version = isStringNotEmpty(versionBox.Text) ? versionBox.Text : null,
				Category = isStringNotEmpty(categoryBox.Text) ? categoryBox.Text : null,
				RedirectMainSave = (bool)mainSaveBox.IsChecked,
				RedirectChaoSave = (bool)chaoSaveBox.IsChecked,
			//	GitHubRepo = isStringNotEmpty(textGitHubRepo.Text) ? textGitHubRepo.Text : null,
				//GitHubAsset = isStringNotEmpty(textGitHubAttachment.Text) ? textGitHubAttachment.Text : null,
				//UpdateUrl = isStringNotEmpty(textDirUrl.Text) ? textDirUrl.Text : null,
				ModID = isStringNotEmpty(modIDBox.Text) ? modIDBox.Text : null,
				DLLFile = isStringNotEmpty(dllText.Text) ? dllText.Text : null,
			};

			IniSerializer.Serialize(newMod, Path.Combine(moddir, "mod.ini"));
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

				if (File.Exists(fullName))
					BuildModINI(Path.GetDirectoryName(fullName));

				if ((bool)openFolderChk.IsChecked)
				{
					var psi = new ProcessStartInfo() { FileName = modPath, UseShellExecute = true };
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

				/*if (checkRedirectMainSave.Checked || checkRedirectChaoSave.Checked)
				{
					Directory.CreateDirectory(Path.Combine(moddir, "SAVEDATA"));
				}*/

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

		private void okBtn_Click(object sender, RoutedEventArgs e)
		{
			string moddir = editMod ? MainWindow.modDirectory: Path.Combine(MainWindow.modDirectory, ValidateFilename(nameBox.Text));

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
	}
}
