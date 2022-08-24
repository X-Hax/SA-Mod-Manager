using System;
using System.IO;
using System.Windows.Forms;
using IniFile;

namespace SADXModManager.Forms
{
	public partial class NewModDialog : Form
	{
		public NewModDialog()
		{
			InitializeComponent();
		}

		private void buttonOK_Click(object sender, EventArgs e)
		{
			string moddir = Path.Combine(Path.Combine(Environment.CurrentDirectory, "mods"), ValidateFilename(textModName.Text));

			if (textModName.Text.Length <= 0)
			{
				MessageBox.Show("You can't have a mod without a name.", "Invalid mod name", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return;
			}

			try
			{
				if (Directory.Exists(moddir))
				{
					MessageBox.Show("A mod with that name already exists."
					                + "\nPlease choose a different name or rename the existing one.", "Mod already exists",
						MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
					return;
				}

				Directory.CreateDirectory(moddir);

				if (checkRedirectMainSave.Checked || checkRedirectChaoSave.Checked)
				{
					Directory.CreateDirectory(Path.Combine(moddir, "SAVEDATA"));
				}

				if (comboModCategory.Text == "Music")
				{
					Directory.CreateDirectory(@Path.Combine(moddir, "system/SoundData/bgm/wma"));
				}

				if (comboModCategory.Text == "Sound")
				{
					Directory.CreateDirectory(@Path.Combine(moddir, "system/SoundData/SE"));
				}

				if (comboModCategory.Text == "Textures")
				{
					Directory.CreateDirectory(Path.Combine(moddir, "textures"));
				}

				if (comboModCategory.SelectedIndex < 0)
				{
					comboModCategory.Text = "";
				}

				if (isStringNotEmpty(textModAuthor.Text)) //save mod author
				{
					Properties.Settings.Default.ModAuthor = textModAuthor.Text;
				}

				//Assign variables to null if the string are empty so they won't show up at all in mod.ini.
				SADXModInfo newMod = new SADXModInfo
				{
					Name = textModName.Text,
					Author = isStringNotEmpty(textModAuthor.Text) ? textModAuthor.Text : null,
					Description = textModDescription.Text.Length > 0 ? textModDescription.Text : null,
					Version = isStringNotEmpty(textVersion.Text) ? textVersion.Text : null,
					Category = isStringNotEmpty(comboModCategory.Text) ? comboModCategory.Text : null,
					RedirectMainSave = checkRedirectMainSave.Checked,
					RedirectChaoSave = checkRedirectChaoSave.Checked,
					GitHubRepo = isStringNotEmpty(textGitHubRepo.Text) ? textGitHubRepo.Text : null,
					GitHubAsset = isStringNotEmpty(textGitHubAttachment.Text) ? textGitHubAttachment.Text : null,
					UpdateUrl = isStringNotEmpty(textDirUrl.Text) ? textDirUrl.Text : null
				};

				IniSerializer.Serialize(newMod, Path.Combine(moddir, "mod.ini"));

				if (checkOpenFolder.Checked)
				{
					System.Diagnostics.Process.Start(moddir);
				}

				DialogResult = DialogResult.OK;
				Close();
			}
			catch (Exception error)
			{
				MessageBox.Show(this, error.Message, "Mod Creation Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
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
	}
}
