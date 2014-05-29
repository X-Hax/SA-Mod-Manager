using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using IniSerializer;

namespace SADXModManager
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

			if (textModName.Text.Length > 0)
			{
				try
				{

					if (!Directory.Exists(moddir))
					{
						Directory.CreateDirectory(moddir);

						if (checkOpenFolder.Checked)
							System.Diagnostics.Process.Start(moddir);

						var newMod = new ModTemplate();

						newMod.Name = textModName.Text;
						newMod.Author = textModAuthor.Text;
						newMod.Description = textModDescription.Text;

						IniFile.Serialize(newMod, Path.Combine(moddir, "mod.ini"));

						// I feel like this is a little hackish, but it works.
						// Setting it on the button causes it to close regardless
						// of whether or not you call Close(), which we don't want.
						DialogResult = DialogResult.OK;
						Close();
					}
					else
					{
						MessageBox.Show("A mod with that name already exists.\nPlease choose a different name or rename the existing one.", "Mod already exists", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
					}
				}
				catch (Exception error)
				{
					MessageBox.Show(this, error.ToString(), "OH NOES", MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
			}
			else
			{
				MessageBox.Show("You can't have a mod without a name.", "Invalid mod name", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
		}

		string ValidateFilename(string filename)
		{
			string result = filename;

			foreach (char c in Path.GetInvalidFileNameChars())
				result = result.Replace(c, '_');

			return result;
		}
	}

	class ModTemplate
	{
		public string Name;
		public string Description;
		public string Author;
	}
}
