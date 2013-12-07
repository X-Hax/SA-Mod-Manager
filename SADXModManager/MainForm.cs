using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Windows.Forms;
using IniSerializer;

namespace SADXModManager
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        const string datadllpath = @"system\CHRMODELS.dll";
        const string datadllorigpath = @"system\CHRMODELS_orig.dll";
        const string loaderinipath = @"mods\SADXModLoader.ini";
        const string loaderdllpath = @"mods\SADXModLoader.dll";
        LoaderInfo loaderini;
        Dictionary<string, ModInfo> mods;
        bool installed;

        private void MainForm_Load(object sender, EventArgs e)
        {
            if (File.Exists(loaderinipath))
                loaderini = IniFile.Deserialize<LoaderInfo>(loaderinipath);
            else
                loaderini = new LoaderInfo();
            mods = new Dictionary<string, ModInfo>();
            string modfolder = Path.Combine(Environment.CurrentDirectory, "mods");
            foreach (string filename in Directory.GetFiles(modfolder, "mod.ini", SearchOption.AllDirectories))
                mods.Add(Path.GetDirectoryName(filename).Remove(0, modfolder.Length + 1), IniFile.Deserialize<ModInfo>(filename));
            modListView.BeginUpdate();
            foreach (string mod in new List<string>(loaderini.Mods))
            {
                if (mods.ContainsKey(mod))
                {
                    ModInfo inf = mods[mod];
                    modListView.Items.Add(new ListViewItem(new[] { inf.Name, inf.Author }) { Checked = true, Tag = mod });
                }
                else
                {
                    MessageBox.Show(this, "Mod \"" + mod + "\" could not be found.\n\nThis mod will be removed from the list.", Text, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    loaderini.Mods.Remove(mod);
                }
            }
            foreach (KeyValuePair<string, ModInfo> inf in mods)
                if (!loaderini.Mods.Contains(inf.Key))
                    modListView.Items.Add(new ListViewItem(new[] { inf.Value.Name, inf.Value.Author }) { Tag = inf.Key });
            modListView.EndUpdate();
            consoleCheckBox.Checked = loaderini.ShowConsole;
            SADXDebugCheckBox.Checked = loaderini.ShowSADXDebugOutput;
            dontFixWindowCheckBox.Checked = loaderini.DontFixWindow;
            disableCDCheckCheckBox.Checked = loaderini.DisableCDCheck;
            useCustomResolutionCheckBox.Checked = horizontalResolution.Enabled = verticalResolution.Enabled = loaderini.UseCustomResolution;
            horizontalResolution.Value = Math.Max(horizontalResolution.Minimum, Math.Min(horizontalResolution.Maximum, loaderini.HorizontalResolution));
            verticalResolution.Value = Math.Max(verticalResolution.Minimum, Math.Min(verticalResolution.Maximum, loaderini.VerticalResolution));
            if (!File.Exists(datadllpath))
            {
                MessageBox.Show(this, "CHRMODELS.dll could not be found.\n\nCannot determine state of installation.", Text, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                installButton.Hide();
            }
            else if (File.Exists(datadllorigpath))
            {
                installed = true;
                installButton.Text = "Uninstall loader";
                MD5 md5 = MD5.Create();
                byte[] hash1 = md5.ComputeHash(File.ReadAllBytes(loaderdllpath));
                byte[] hash2 = md5.ComputeHash(File.ReadAllBytes(datadllpath));
                if (!hash1.SequenceEqual(hash2))
                    if (MessageBox.Show(this, "Installed loader DLL differs from copy in mods folder.\n\nDo you want to overwrite the installed copy?", Text, MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == System.Windows.Forms.DialogResult.Yes)
                        File.Copy(loaderdllpath, datadllpath, true);
            }
        }

        private void modListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (modListView.SelectedIndices.Count == 0)
            {
                modUpButton.Enabled = modDownButton.Enabled = false;
                modDescription.Text = "Description: No mod selected.";
            }
            else
            {
                modDescription.Text = "Description: " + mods[(string)modListView.SelectedItems[0].Tag].Description;
                modUpButton.Enabled = modListView.SelectedIndices[0] > 0;
                modDownButton.Enabled = modListView.SelectedIndices[0] < modListView.Items.Count - 1;
            }
        }

        private void modUpButton_Click(object sender, EventArgs e)
        {
            int i = modListView.SelectedIndices[0];
            ListViewItem item = modListView.Items[i];
            modListView.BeginUpdate();
            modListView.Items.Remove(item);
            modListView.Items.Insert(i - 1, item);
            modListView.EndUpdate();
        }

        private void modDownButton_Click(object sender, EventArgs e)
        {
            int i = modListView.SelectedIndices[0];
            ListViewItem item = modListView.Items[i];
            modListView.BeginUpdate();
            modListView.Items.Remove(item);
            modListView.Items.Insert(i + 1, item);
            modListView.EndUpdate();
        }

        private void Save()
        {
            loaderini.Mods.Clear();
            foreach (ListViewItem item in modListView.CheckedItems)
                loaderini.Mods.Add((string)item.Tag);
            loaderini.ShowConsole = consoleCheckBox.Checked;
            loaderini.ShowSADXDebugOutput = SADXDebugCheckBox.Checked;
            loaderini.DontFixWindow = dontFixWindowCheckBox.Checked;
            loaderini.DisableCDCheck = disableCDCheckCheckBox.Checked;
            loaderini.UseCustomResolution = useCustomResolutionCheckBox.Checked;
            loaderini.HorizontalResolution = (int)horizontalResolution.Value;
            loaderini.VerticalResolution = (int)verticalResolution.Value;
            IniFile.Serialize(loaderini, loaderinipath);
        }

        private void saveAndPlayButton_Click(object sender, EventArgs e)
        {
            Save();
            string exe = "sonic.exe";
            foreach (string item in loaderini.Mods)
                if (!string.IsNullOrEmpty(mods[item].EXEFile))
                    exe = mods[item].EXEFile;
            System.Diagnostics.Process.Start(exe);
            Close();
        }

        private void saveButton_Click(object sender, EventArgs e)
        {
            Save();
        }

        private void installButton_Click(object sender, EventArgs e)
        {
            if (installed)
            {
                File.Delete(datadllpath);
                File.Move(datadllorigpath, datadllpath);
                installButton.Text = "Install loader";
            }
            else
            {
                File.Move(datadllpath, datadllorigpath);
                File.Copy(loaderdllpath, datadllpath);
                installButton.Text = "Uninstall loader";
            }
            installed = !installed;
        }

        private void useCustomResolutionCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            horizontalResolution.Enabled = verticalResolution.Enabled = useCustomResolutionCheckBox.Checked;
        }

        private void configEditorButton_Click(object sender, EventArgs e)
        {
            using (ConfigEditDialog dlg = new ConfigEditDialog())
                dlg.ShowDialog(this);
        }
    }

    class LoaderInfo
    {
        public bool ShowConsole { get; set; }
        public bool ShowSADXDebugOutput { get; set; }
        public bool DontFixWindow { get; set; }
        public bool DisableCDCheck { get; set; }
        public bool UseCustomResolution { get; set; }
        [DefaultValue(640)]
        public int HorizontalResolution { get; set; }
        [DefaultValue(480)]
        public int VerticalResolution { get; set; }
        [IniName("Mod")]
        [IniCollection(IniCollectionMode.NoSquareBrackets, StartIndex = 1)]
        public List<string> Mods { get; set; }

        public LoaderInfo()
        {
            Mods = new List<string>();
        }
    }

    class ModInfo
    {
        public string Name { get; set; }
        public string Author { get; set; }
        public string Description { get; set; }
        public string EXEFile { get; set; }
        public string DLLFile { get; set; }
    }
}
