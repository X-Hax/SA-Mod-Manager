using System;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Collections.Generic;
using System.IO;
using System.Windows.Input;
using System.Windows.Threading;
using System.Diagnostics;
using SAModManager.Properties;
using System.Windows.Media;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Windows.Data;
using System.Security.Cryptography;
using System.Threading;
using Keyboard = System.Windows.Input.Keyboard;
using Key = System.Windows.Input.Key;
using SAModManager.UI;
using SAModManager.Updater;
using SAModManager.Controls;
using SAModManager.Ini;
using SAModManager.Configuration;
using System.Text;
using SAModManager.Codes;
using System.Windows.Media.Imaging;
using System.Reflection;
using SAModManager.Profile;
using SAModManager.ModsCommon;
using System.DirectoryServices.ActiveDirectory;
using System.Collections.ObjectModel;
using SAModManager.Management;
using PropertyChanged;
using System.Media;

namespace SAModManager
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>

    public partial class MainWindow : Window
    {
        #region Variables
        // Global Window Strings
        public readonly string titleName = "SA Mod Manager";
        private readonly string Version = App.VersionString;
        private static string updatePath = "mods\\.updates";
        string codelstpath = string.Empty;
        string codexmlpath = string.Empty;
        string codedatpath = string.Empty;
        string patchdatpath = string.Empty;

        // Shared Variables
        CodeList mainCodes = null;
        List<Code> codes = null;
        protected Timer StatusTimer;
        public List<CodeData> codesSearch { get; set; }
        public bool suppressEvent = false;
        private bool manualModUpdate;
        readonly Updater.ModUpdater modUpdater = new();
        private DebugSettings gameDebugSettings = new();

        private bool displayedManifestWarning;
        public MainWindowViewModel ViewModel;
        object GameProfile;
        public string tempPath = "";

        public Dictionary<string, SAModInfo> mods = null;
        public List<string> EnabledMods = new();
        public List<string> EnabledCodes = new();

        #endregion


        public MainWindow()
        {
            InitializeComponent();
            UpdateDLLData();
            GraphicsManager.SetupGraphics();

            try
            {
                var set = App.ManagerSettings;
                if (set is not null)
                {
                    if (set.managerWidth >= this.MinWidth && set.managerWidth <= this.MaxWidth)
                        this.Width = set.managerWidth;

                    if (set.managerHeight >= this.MinHeight && set.managerHeight <= this.MaxHeight)
                        this.Height = set.managerHeight;
                }
            }
            catch { }

        }

        #region Form: Functions
        private async void MainWindowManager_Loaded(object sender, RoutedEventArgs e)
        {
            this.Resources.MergedDictionaries.Clear(); //this is very important to get Theme and Language swap to work on MainWindow

            StatusTimer = new Timer((state) => UpdateManagerStatusText(string.Empty));

            SetModManagerVersion();

            if (App.isFirstBoot)
            {
                new SplashScreenDialog().ShowDialog();
            }


            Load();
            SetBindings(); //theme is set here

            if (Util.IsStringValid(App.CurrentGame?.modDirectory))
            {
                var oneClick = new OneClickInstall(updatePath);
                await oneClick.UriInit();

                if (oneClick.isEmpty == false)
                {
                    return;
                }
            }

#if !DEBUG
            if (App.isFirstBoot == false)
            {
                if (chkUpdateManager.IsChecked == true)
                {
                    UpdateManagerStatusText(Lang.GetString("UpdateStatus.ChkUpdate"));
                    UIHelper.ToggleButton(ref btnCheckUpdates, false);
                    bool isDev = App.isDev;

                    bool managerUpdate = isDev ? await App.PerformDevUpdateManagerCheck() : await App.PerformUpdateManagerCheck();
                    if (managerUpdate)
                    {
                        Refresh();
                        return;
                    }
                }

                if (App.CurrentGame != GamesInstall.Unknown)
                {

                    if (string.IsNullOrEmpty(App.CurrentGame?.modDirectory) == false)
                    {
                        if (App.CurrentGame.loader.installed)
                        {
                            if (chkUpdatesML.IsChecked == true)
                            {
                                UpdateManagerStatusText(Lang.GetString("UpdateStatus.ChkUpdate"));
                                await App.PerformUpdateLoaderCheck();
                                Refresh();
                            }

                            if (App.CurrentGame.id == GameEntry.GameType.SADX)
                            {
                                await App.PerformUpdateAppLauncherCheck();
                            }

                            Util.ClearTempFolder();
                        }

                        await CheckForModUpdates();
                    }
                }
            }
#endif

            if (App.isVanillaTransition && App.CurrentGame?.loader?.installed == false)
            {
                await ForceInstallLoader();
            }
            App.isVanillaTransition = false;
            UIHelper.ToggleButton(ref btnCheckUpdates, true);

            // Save Manager Settings
            Save();
            suppressEvent = false;
        }

        private void MainForm_FormClosing(object sender, EventArgs e)
        {
            App.CancelUpdate = true;
            App.UriQueue.Close();
            Save_AppUserSettings();
			SettingsManager.SaveAppDirectoryInfo();
        }

        private void SaveButton_Click(object sender, RoutedEventArgs e)
        {
            Save();
            Refresh();
        }

        private void SaveAndPlayButton_Click(object sender, RoutedEventArgs e)
        {
            Save();
            Refresh();


            if (App.CurrentGame.id == GameEntry.GameType.Unsupported || ModDependency.CheckDependencies(EnabledMods, mods))
                return;

            StartGame();
        }

        #region Form: Mods Tab Functions
        private void btnMoveTop_Click(object sender, RoutedEventArgs e)
        {
            if (listMods.SelectedItems.Count < 1)
                return;

            int index = listMods.SelectedIndex;

            if (index > 0)
            {
                var item = ViewModel.Modsdata[index];
                ViewModel.Modsdata.Remove(item);
                ViewModel.Modsdata.Insert(0, item);
                listMods.SelectedIndex = 0;
                listMods.SelectedItem = item;
                listMods.ScrollIntoView(item);
            }
        }

        private void btnMoveUp_Click(object sender, RoutedEventArgs e)
        {
            if (listMods.SelectedItems.Count < 1)
                return;

            int index = listMods.SelectedIndex;

            if (index > 0)
            {
                var item = ViewModel.Modsdata[index];
                ViewModel.Modsdata.Remove(item);
                ViewModel.Modsdata.Insert(index - 1, item);
                listMods.SelectedIndex = index - 1;
                listMods.SelectedItem = item;
                listMods.ScrollIntoView(item);
            }
        }

        private void btnMoveBottom_Click(object sender, RoutedEventArgs e)
        {
            if (listMods.SelectedItems.Count < 1)
                return;

            int index = listMods.SelectedIndex;

            if (index != listMods.Items.Count - 1)
            {
                var item = ViewModel.Modsdata[index];
                ViewModel.Modsdata.Remove(item);
                ViewModel.Modsdata.Insert(listMods.Items.Count, item);
                listMods.SelectedIndex = listMods.Items.Count;
                listMods.SelectedItem = item;
                listMods.ScrollIntoView(item);
            }
        }

        private void btnMoveDown_Click(object sender, RoutedEventArgs e)
        {
            if (listMods.SelectedItems.Count < 1)
                return;

            var list = new List<ModData>(listMods.SelectedItems.Cast<ModData>());

            foreach (var mod in list)
            {
                int index = listMods.Items.IndexOf(mod);

                if (index < listMods.Items.Count)
                {
                    var item = ViewModel.Modsdata[index];
                    ViewModel.Modsdata.Remove(item);
                    ViewModel.Modsdata.Insert(index + 1, item);
                    listMods.SelectedIndex = index + 1;
                    listMods.SelectedItem = item;
                    listMods.ScrollIntoView(item);
                }
            }
        }

        private async void btnSelectAll_Click(object sender, RoutedEventArgs e)
        {
            foreach (ModData mod in listMods.Items)
            {
                mod.IsChecked = true;
            }

            RefreshModList();
            btnSelectAll.IsEnabled = false;
            await Task.Delay(150);
            btnSelectAll.IsEnabled = true;
        }

        private async void btnDeselectAll_Click(object sender, RoutedEventArgs e)
        {
            foreach (ModData mod in listMods.Items)
            {
                mod.IsChecked = false;
            }

            RefreshModList();
            btnDeselectAll.IsEnabled = false;
            await Task.Delay(150);
            btnDeselectAll.IsEnabled = true;
        }

        private async void RefreshBtn_Click(object sender, RoutedEventArgs e)
        {
            Refresh();
            RefreshBtn.IsEnabled = false;
            await Task.Delay(150);
            RefreshBtn.IsEnabled = true;
        }

        private async void NewModBtn_Click(object sender, RoutedEventArgs e)
        {
            if (App.CurrentGame == GamesInstall.Unknown || App.CurrentGame?.id == GameEntry.GameType.Unsupported)
                return;

            var form = new InstallModOptions();
            var choice = form.Ask();

            switch (choice)
            {
                case (int)InstallModOptions.Type.ModArchive:
                    var archiveFile = new System.Windows.Forms.OpenFileDialog
                    {
                        Multiselect = true,

                        Filter = "archive files|*.zip;*.7z;*.rar;*.tar"
                    };
                    System.Windows.Forms.DialogResult result_ = archiveFile.ShowDialog();

                    if (result_ == System.Windows.Forms.DialogResult.OK)
                    {
                        string[] sFileName = archiveFile.FileNames;
                        await form.InstallMod(sFileName, App.CurrentGame.modDirectory);

                    }
                    break;
                case (int)InstallModOptions.Type.ModFolder:
                    var newModFolder = new System.Windows.Forms.FolderBrowserDialog();

                    System.Windows.Forms.DialogResult result = newModFolder.ShowDialog();

                    if (result == System.Windows.Forms.DialogResult.OK)
                    {
                        string[] FileName = { newModFolder.SelectedPath };

                        await form.InstallMod(FileName, App.CurrentGame.modDirectory);
                    }
                    break;
                case (int)InstallModOptions.Type.ModUrl:
                    InstallModUrl url = new();
                    url.ShowDialog();
                    break;
                case (int)InstallModOptions.Type.NewMod: //create mod
                    EditMod Edit = new(null, null);
                    Edit.ShowDialog();
                    Edit.Closed += EditMod_FormClosing;

                    break;
            }

        }

        private void ConfigureModBtn_Click(object sender, RoutedEventArgs e)
        {
            InitModConfig();
        }

        private void ModContextConfigureMod_Click(object sender, RoutedEventArgs e)
        {
            if (ConfigureModBtn.IsEnabled)
                InitModConfig();
        }

        private void modChecked_Click(object sender, RoutedEventArgs e)
        {
            if (suppressEvent)
                return;

            codes = new List<Code>(mainCodes.Codes);
            codesSearch = [];
            string modDir = Path.Combine(App.CurrentGame.gameDirectory, "mods");
            List<string> modlistCopy = [];

            //backup mod list here
            foreach (ModData mod in listMods.Items)
            {
                if (mod?.IsChecked == true)
                {
                    modlistCopy.Add(mod.Tag);
                }
            }

            if (sender is CheckBox check)
            {
                var curItem = check.DataContext as ModData;
                var index = listMods.Items.IndexOf(curItem);

                var curModCopy = listMods.Items[index] as ModData;
                if (check.IsChecked == false)
                {
                    modlistCopy.Remove((string)curModCopy.Tag);
                }
                else
                {
                   // ModDependency.AutoCheckBox(mods[curItem.Name], mods);
                }
            }
            //reload codes from mods
            foreach (string mod in modlistCopy)
            {
                if (mods.TryGetValue(mod, out SAModInfo value))
                {
                    SAModInfo inf = value;
                    if (!string.IsNullOrEmpty(inf.Codes))
                    {
                        string codePath = Path.Combine(Path.Combine(modDir, mod), inf.Codes);
                        if (File.Exists(codePath))
                            codes.AddRange(CodeList.Load(codePath).Codes);
                    }

                }
            }


            EnabledCodes = new List<string>(EnabledCodes.Where(a => codes.Any(c => c.Name == a)));

            foreach (Code item in codes.Where(a => a.Required && !EnabledCodes.Contains(a.Name)))
                EnabledCodes.Add(item.Name);

            CodeListView.BeginInit();

            foreach (Code item in codes)
            {
                CodeData extraItem = new()
                {
                    codes = item,
                    IsChecked = EnabledCodes.Contains(item.Name),
                    IsEnabled = !item.Required,
                };

                CodeListView.Items.Add(extraItem);
                codesSearch.Add(extraItem);
            }

            CodeListView.EndInit();
        }

        private void EditMod_FormClosing(object sender, EventArgs e)
        {
            Refresh();
        }

        #region Form: Mods Tab: Context Menu
        private void ModsView_Item_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (!(GetModFromView(sender) is ModData mod))
                return;

            OpenAboutModWindow(mod);
        }

        private void ModList_MouseEnter(object sender, MouseEventArgs e)
        {
            if (GetModFromView(sender) is not ModData mod)
                return;

            if (mods.TryGetValue(mod.Tag, out SAModInfo value))
                textModsDescription.Text = Lang.GetString("CommonStrings.Description") + " " + value.Description;
        }

        private void ModList_MouseLeave(object sender, MouseEventArgs e)
        {
            var item = GetModFromView(sender);
            textModsDescription.Text = (item is not null) ? $"{Lang.GetString("CommonStrings.Description")} {item.Description}" : Lang.GetString("Manager.Tabs.Mods.Text.NoModSelected");
        }

        private void modListView_SelectedIndexChanged(object sender, SelectionChangedEventArgs e)
        {
            int count = listMods.SelectedItems.Count;

            if (count == 0)
            {
                btnMoveTop.IsEnabled = btnMoveUp.IsEnabled = btnMoveDown.IsEnabled = btnMoveBottom.IsEnabled = ConfigureModBtn.IsEnabled = false;
            }
            else if (count == 1)
            {
                var mod = GetModFromView(sender);

                btnMoveTop.IsEnabled = listMods.Items.IndexOf(mod) != 0;
                btnMoveUp.IsEnabled = listMods.Items.IndexOf(mod) > 0;
                btnMoveDown.IsEnabled = listMods.Items.IndexOf(mod) < listMods.Items.Count - 1;
                btnMoveBottom.IsEnabled = listMods.Items.IndexOf(mod) != listMods.Items.Count - 1;

                ConfigureModBtn.IsEnabled = File.Exists(Path.Combine(App.CurrentGame.modDirectory, mod.Tag, "configschema.xml"));
                ConfigureModBtn_UpdateState();

                textModsDescription.Text = Lang.GetString("CommonStrings.Description") + " " + mods[mod.Tag].Description;
            }
            else if (count > 1)
            {
                btnMoveTop.IsEnabled = btnMoveUp.IsEnabled = btnMoveDown.IsEnabled = btnMoveBottom.IsEnabled = true;
                ConfigureModBtn.IsEnabled = false;
            }
        }

        private void ModList_ContextMenuOpening(object sender, ContextMenuEventArgs e)
        {
            bool isEnabled = ConfigureModBtn.IsEnabled;
            var curMod = (ModData)listMods.SelectedItem;

            if (ModContextMenu is null || curMod is null)
                return;

            var item = ModContextMenu.Items.OfType<MenuItem>().FirstOrDefault(item => item.Name == "ModContextConfigureMod");

            if (item is not null)
            {
                UIHelper.ToggleMenuItem(ref item, isEnabled);
                Image iconConfig = FindName("menuIconConfig") as Image;
                UIHelper.ToggleImage(ref menuIconConfig, isEnabled);
            }

            ModContextDeveloper.Visibility = (bool)checkDevEnabled.IsChecked ? Visibility.Visible : Visibility.Collapsed;

            string UpdateStateString = "Manager.Tabs.Mods.ContextMenu.Developer.DisableUpdate";

            if (mods[curMod.Tag].DisableUpdate == true)
                UpdateStateString = "Manager.Tabs.Mods.ContextMenu.Developer.EnableUpdate";

            var resource = App.Current.TryFindResource(UpdateStateString);
            if (resource is not null)
                ModsContextDeveloperIgnoreUpdate.Header = resource;
        }

        private void ModsContextDeveloperManifest_Click(object sender, RoutedEventArgs e)
        {
            if (!displayedManifestWarning)
            {
                var result = new MessageWindow(Lang.GetString("Warning"), Lang.GetString("MessageWindow.Warnings.GenerateManifest"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);
                result.ShowDialog();
                if (result.isYes != true)
                {
                    return;
                }

                displayedManifestWarning = true;
            }

            foreach (ModData item in listMods.SelectedItems.Cast<ModData>().ToList())
            {

                var modPath = Path.Combine(App.CurrentGame.modDirectory, (string)item.Tag);
                var manifestPath = Path.Combine(modPath, "mod.manifest");

                foreach (string filename in SAModInfo.GetModFiles(new DirectoryInfo(modPath)))
                {
                    if (filename.Contains("mod.ini")) //reload mod ini in case the user edited it
                    {
                        mods[item.Tag] = IniSerializer.Deserialize<SAModInfo>(filename);
                        break;
                    }
                }

                var CurMod = mods[item.Tag];

                if (CurMod is not null && CurMod.DisableUpdate == true)
                {
                    CurMod.DisableUpdate = false;
                    string fullPath = Path.Combine(App.CurrentGame.modDirectory, item.Tag, "mod.ini");
                    string[] modIniString = File.ReadAllLines(fullPath);
                    var modIni = IniFile.Load(modIniString);
                    IniFile.RemoveGroupLine(modIni, "DisableUpdate");
                    Ini.IniFile.Save(modIni, fullPath);
                }

                List<Updater.ModManifestEntry> manifest;
                List<Updater.ModManifestDiff> diff;

                var progress = new Updater.ManifestDialog(modPath, $"Generating manifest: {(string)item.Tag}", true);
                progress.ShowDialog();

                bool? ManifDialog = progress.DialogResult;

                //progress.SetTask("Generating file index...");
                if (ManifDialog != true)
                {
                    continue;
                }

                diff = progress.Diff;

                if (diff == null)
                {
                    continue;
                }

                if (!diff.Any(x => x.State != Updater.ModManifestState.Unchanged))
                {
                    continue;
                }

                var dialog = new ManifestChanges(diff);
                dialog.ShowDialog();
                bool? resultManifChange = dialog.DialogResult;

                if (resultManifChange != true)
                {
                    continue;
                }

                manifest = dialog.MakeNewManifest();

                Updater.ModManifest.ToFile(manifest, manifestPath);
                Refresh();
            }
        }

        private void ModsContextDeveloperIgnoreUpdate_Click(object sender, RoutedEventArgs e)
        {
            var mod = (ModData)listMods.SelectedItem;

            if (mod == null)
                return;

            SAModInfo modInfo = mods[mod.Tag];

            if (modInfo.DisableUpdate != true)
            {
                var msg = new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Warning"), Lang.GetString("MessageWindow.Warnings.DisableUpdates"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);
                msg.ShowDialog();

                if (msg.isYes != true)
                    return;
            }

            modInfo.DisableUpdate = !modInfo.DisableUpdate;

            string fullPath = Path.Combine(App.CurrentGame.modDirectory, mod.Tag, "mod.ini");
            string[] modIniString = File.ReadAllLines(fullPath);
            var modIni = IniFile.Load(modIniString);
            IniFile.AddModIniGroup(modIni, "DisableUpdate", modInfo.DisableUpdate);
            Ini.IniFile.Save(modIni, fullPath);
        }

        private void ModContextOpenFolder_Click(object sender, RoutedEventArgs e)
        {
            var selectedMods = listMods.SelectedItems.OfType<ModData>();

            foreach (var mod in selectedMods.Where(mod => !string.IsNullOrEmpty(mod.Tag)))
            {
                string fullPath = Path.Combine(App.CurrentGame.modDirectory, mod.Tag);
                if (Directory.Exists(fullPath))
                    Process.Start(new ProcessStartInfo { FileName = fullPath, UseShellExecute = true });
            }
        }

        private async void ModContextChkUpdate_Click(object sender, RoutedEventArgs e)
        {
            App.CancelUpdate = false;
            await UpdateSelectedMods();
        }

        private async void ModContextVerifyIntegrity_Click(object sender, RoutedEventArgs e)
        {

            List<Tuple<string, ModInfo>> items = listMods.SelectedItems.Cast<ModData>()
                .Select(x => (string)x.Tag)
                .Where(x => File.Exists(Path.Combine(App.CurrentGame.modDirectory, x, "mod.manifest")))
                .Select(x => new Tuple<string, ModInfo>(x, mods[x]))
                .ToList();

            if (items.Count < 1)
            {
                new MessageWindow(Lang.GetString("MessageWindow.Errors.MissingManifest.Title"), Lang.GetString("MessageWindow.Errors.MissingManifest"),
            MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Information).ShowDialog();
                return;
            }

            var progress = new Updater.VerifyModDialog(items, App.CurrentGame.modDirectory);

            progress.ShowDialog();

            bool? res = progress.DialogResult;

            if (res == false)
            {
                return;
            }

            modUpdater.modManifestTuple = progress.Failed;

            if (modUpdater.modManifestTuple is null)
                return;

            if (modUpdater.modManifestTuple.Count < 1)
            {
                new MessageWindow(Lang.GetString("MessageWindow.Information.ModPassedVerif.Title"), Lang.GetString("MessageWindow.Information.ModPassedVerif"),
                    MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Information).ShowDialog();
            }
            else
            {

                var error = Lang.GetString("MessageWindow.Errors.ModFailedVerif0")
                        + string.Join("\n", modUpdater.modManifestTuple.Select(x => $"{x.Item2.Name}: "
                        + Util.GetFileCountString(x.Item3.Count(y => y.State != Updater.ModManifestState.Unchanged), "MessageWindow.Errors.ModFailedVerif1")))
                        + Lang.GetString("MessageWindow.Errors.ModFailedVerif2");


                var result = new MessageWindow(Lang.GetString("MessageWindow.Errors.ModFailedVerif.Title"), error, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Information, MessageWindow.Buttons.YesNo);
                result.ShowDialog();

                if (result.isYes != true)
                {
                    return;
                }

                UIHelper.ToggleButton(ref btnCheckUpdates, false);
                await UpdateChecker_DoWork();
                modUpdater.ForceUpdate = true;
                await UpdateChecker_DoWorkForced();
                modUpdater.ForceUpdate = false;
                await UpdateChecker_RunWorkerCompleted();
                UpdateChecker_EnableControls();
            }

        }

        private async void ForceModUpdate_Click(object sender, RoutedEventArgs e)
        {
            var result = new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Warning"), Lang.GetString("MessageWindow.Warnings.ForceModUpdate"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Caution, MessageWindow.Buttons.YesNo);
            result.ShowDialog();

            if (result.isYes)
            {
                modUpdater.ForceUpdate = true;
                await UpdateSelectedMods();
            }
        }

        private void ModContextEditMod_Click(object sender, RoutedEventArgs e)
        {
            var mod = (ModData)listMods.SelectedItem;

            if (mod == null)
                return;

            SAModInfo modInfo = mods[mod.Tag];
            EditMod Edit = new(modInfo, mod.Tag);
            Edit.ShowDialog();
            Edit.Closed += EditMod_FormClosing;
        }

        private void ModContextDeleteMod_Click(object sender, RoutedEventArgs e)
        {
            var selectedItems = listMods.SelectedItems;
            var count = selectedItems.Count > 0;
            int index = 0;
            if (count)
            {
                var confirmMessage = Lang.GetString("MessageWindow.Warnings.DeleteMod0");
                foreach (var selectedItem in selectedItems)
                {
                    if (selectedItem is ModData item)
                    {
                        index++;
                        confirmMessage += "- " + item.Name;
                        confirmMessage += "\n";
                    }
                }

                confirmMessage += "\n" + Lang.GetString("MessageWindow.Warnings.DeleteMod1");
                var deleteConfirmation = new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Warning"), confirmMessage, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);

                deleteConfirmation.ShowDialog();
                if (deleteConfirmation.isYes)
                {
                    foreach (var selectedItem in selectedItems)
                    {
                        if (selectedItem is ModData item)
                        {
                            try
                            {
                                string fullPath = Path.Combine(App.CurrentGame.modDirectory, item.Tag);

                                if (Directory.Exists(fullPath))
                                {
                                    Directory.Delete(fullPath, true);
                                }
                            }
                            catch
                            {
                                new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), string.Format(Lang.GetString("MessageWindow.Errors.ModDelete"), item.Name), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error).ShowDialog();
                            }
                        }
                    }

                    ModsFind.Visibility = Visibility.Collapsed;
                    FilterMods("");
                    TextBox_ModsSearch.Text = "";
                    LoadModList();
                }
            }
        }
        #endregion

        #region Form: Mods Tab: Shortcuts

        private void SelectModByFirstLetter(string letter)
        {
            if (listMods.Items.Count == 0)
                return;

            int selectedIndex = listMods.SelectedIndex;
            int startIndex = selectedIndex >= 0 ? selectedIndex + 1 : 0;
            bool letterFound = false;

            void SearchAndUpdate(int index)
            {
                var item = ViewModel.Modsdata[index];
                if (!string.IsNullOrEmpty(item.Name) && letter[0] == item.Name[0])
                {
                    listMods.SelectedIndex = index;
                    listMods.ScrollIntoView(item);
                    letterFound = true;
                }
            }

            for (int i = startIndex; i < ViewModel.Modsdata.Count && !letterFound; i++)
                SearchAndUpdate(i);

            if (!letterFound)
                for (int i = 0; i < startIndex && !letterFound; i++)
                    SearchAndUpdate(i);

            if (!letterFound && selectedIndex >= 0)
                listMods.ScrollIntoView(ViewModel.Modsdata[selectedIndex]);
        }


        private void ModsList_OnPreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (listMods == null)
                return;

            ModData mod = ViewModel.SelectedMod;

            if (mod == null)
                return;

            var ctrlKey = Keyboard.IsKeyDown(Key.LeftCtrl) || Keyboard.IsKeyDown(Key.RightCtrl);

            if (Keyboard.IsKeyDown(Key.Space))
            {
                mod.IsChecked = !mod.IsChecked;
            }
            else if (ctrlKey)
            {

                if (Keyboard.IsKeyDown(Key.E))
                    ModContextEditMod_Click(null, null);
                else if (Keyboard.IsKeyDown(Key.O))
                    ModContextOpenFolder_Click(null, null);
                else if (Keyboard.IsKeyDown(Key.C))
                    ModContextConfigureMod_Click(null, null);
                else if (Keyboard.IsKeyDown(Key.U))
                    ModContextChkUpdate_Click(null, null);
                else if (Keyboard.IsKeyDown(Key.Enter))
                    AboutBtn_Click(null, null);

                e.Handled = true;
            }
            else if (Keyboard.IsKeyDown(Key.Enter))
            {
                OpenAboutModWindow(mod);
                e.Handled = true;
            }

            if (Keyboard.IsKeyDown(Key.Delete))
            {
                ModContextDeleteMod_Click(null, null);
                e.Handled = true;
            }

            if (!ctrlKey)
            {
                if (e.Key >= Key.A && e.Key <= Key.Z)
                {
                    e.Handled = true;
                    KeyConverter converter = new();
                    string keyString = (string)converter.ConvertTo(e.Key, typeof(string));
                    SelectModByFirstLetter(keyString);
                }
            }
        }

        private void CodesList_OnPreviewKeyDown(object sender, KeyEventArgs e)
        {
            var code = ViewModel.SelectedCode;

            if (code == null)
                return;

            if (Keyboard.IsKeyDown(Key.Space) && !code.codes.Required)
            {
                code.IsChecked = !code.IsChecked;
            }

            if (Keyboard.IsKeyDown(Key.Enter))
                OpenAboutCodeWindow(code.codes);
        }

        private void Window_PreviewKeyDown(object sender, KeyEventArgs e)
        {

            var ctrlkey = Keyboard.IsKeyDown(Key.LeftCtrl) || Keyboard.IsKeyDown(Key.RightCtrl);

            if (Keyboard.IsKeyDown(Key.F5))
            {
                Refresh();
            }
            else if (ctrlkey)
            {
                if (Keyboard.IsKeyDown(Key.F)) //enable or disable search feature
                {
                    if (tcMain.SelectedItem == tabMain)
                    {
                        if (ModsFind.Visibility == Visibility.Visible)
                        {
                            ModsFind.Visibility = Visibility.Collapsed;
                            FilterMods("");
                            TextBox_ModsSearch.Text = "";
                        }
                        else
                        {
                            ModsFind.Visibility = Visibility.Visible;
                            FilterMods(TextBox_ModsSearch.Text.ToLowerInvariant());
                            TextBox_ModsSearch.Focus();
                        }

                    }
                    else if (tcMain.SelectedItem == tbCodes)
                    {
                        if (CodesFind.Visibility == Visibility.Visible)
                        {
                            CodesFind.Visibility = Visibility.Collapsed;
                            FilterCodes("");
                            TextBox_CodesSearch.Text = "";
                        }
                        else
                        {

                            CodesFind.Visibility = Visibility.Visible;
                            FilterCodes(TextBox_CodesSearch.Text.ToLowerInvariant());
                            TextBox_CodesSearch.Focus();

                        }
                    }
                }
            }

            if (Keyboard.IsKeyDown(Key.Escape))
            {
                if (tcMain.SelectedItem == tabMain)
                {
                    ModsFind.Visibility = Visibility.Collapsed;
                    FilterMods("");
                }
                else if (tcMain.SelectedItem == tbCodes)
                {
                    CodesFind.Visibility = Visibility.Collapsed;
                    FilterCodes("");
                }
            }

        }
        #endregion

        #region Form: Mods Tab: Searchbar
        //more info in the keyboard shortcut functions above.
        private void TextBox_ModsSearch_LostFocus(object sender, RoutedEventArgs e)
        {
            // Close if focus is lost with no text
            if (TextBox_ModsSearch.Text.Length == 0)
                ModsFind.Visibility = Visibility.Collapsed;
        }

        private void TextBox_ModsSearch_TextChanged(object sender, TextChangedEventArgs e)
        {
            FilterMods(TextBox_ModsSearch.Text.ToLowerInvariant());
        }
        #endregion
        #endregion

        #region Form: Codes Tab Functions
        private void CodesView_Item_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            var code = GetCodeFromView(sender);

            if (code == null)
                return;

            OpenAboutCodeWindow(code.codes);
        }

        private void CodesView_Item_MouseEnter(object sender, MouseEventArgs e)
        {
            var codes = GetCodeFromView(sender);

            if (codes == null)
                return;

            var code = codes.codes;

            CodeAuthorGrid.Text += ": " + code.Author;
            CodeDescGrid.Text += " " + code.Description;
            CodeCategoryGrid.Text += ": " + code.Category;
        }

        private void CodesView_Item_MouseLeave(object sender, MouseEventArgs e)
        {
            CodeAuthorGrid.Text = Lang.GetString("CommonStrings.Author");
            CodeDescGrid.Text = Lang.GetString("CommonStrings.Description");
            CodeCategoryGrid.Text = Lang.GetString("CommonStrings.Category");
        }

        private void CodeListView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            CodesView_Item_MouseLeave(null, null);
            CodesView_Item_MouseEnter(sender, null);
        }

        private void btnSelectAllCode_Click(object sender, RoutedEventArgs e)
        {
            CodeListView.BeginInit();
            foreach (CodeData code in CodeListView.Items)
            {
                code.IsChecked = true;
            }
            CodeListView.EndInit();
        }

        private void btnDeselectAllCode_Click(object sender, RoutedEventArgs e)
        {
            CodeListView.BeginInit();
            foreach (CodeData code in CodeListView.Items)
            {
                code.IsChecked = false;
            }
            CodeListView.EndInit();
        }

        private void TextBox_CodesSearch_TextChanged(object sender, TextChangedEventArgs e)
        {
            FilterCodes(TextBox_CodesSearch.Text.ToLowerInvariant());
        }

        private void TextBox_CodesSearch_LostFocus(object sender, RoutedEventArgs e)
        {
            if (TextBox_CodesSearch.Text.Length == 0)
                CodesFind.Visibility = Visibility.Collapsed;
        }
        #endregion

        #region Form: Manager Tab: Functions
        private async Task<bool> ResultPickGame(string path)
        {
            var setGame = GamesInstall.SetGameInstallManual(path);

            if (setGame == GameEntry.GameType.Unsupported)
            {
                new MessageWindow(Lang.GetString("MessageWindow.Errors.GamePathFailed.Title"), Lang.GetString("MessageWindow.Errors.GamePathFailed"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
                return false;
            }
            else
            {
                App.GamesList.Remove(GamesInstall.Unknown);
                UIHelper.ToggleImgButton(ref btnBrowseGameDir, false);
                UIHelper.ToggleImgButton(ref btnProfileSettings, false);
                App.CancelUpdate = true;

                var game = GamesInstall.GetGamePerID(setGame);
                bool isMultipleInstall = App.GamesList.Contains(game) && GamesInstall.IsMultipleGamesInstall(game, path);

                if (App.GamesList.Contains(game) == false || isMultipleInstall)
                {
                    game.gameDirectory = path;

                    if (isMultipleInstall)
                        App.GamesList.Remove(game);


                    App.GamesList.Add(game);

                }

                tempPath = path;
                UIHelper.ToggleButton(ref btnOpenGameDir, true);
                suppressEvent = true;


                if (DoGameSwap((Game)game, path))
                {
                    ComboGameSelection.SelectedValue = App.CurrentGame;
                    await ForceInstallLoader();
                    SetBindings();
                }

                UpdateButtonsState();
                UIHelper.ToggleImgButton(ref btnProfileSettings, true);
                UIHelper.ToggleImgButton(ref btnBrowseGameDir, true);
                suppressEvent = false;
                Save();
                return true;
            }
        }

        private async Task<bool> AddGameManually()
        {
            var dialog = new System.Windows.Forms.FolderBrowserDialog();

            System.Windows.Forms.DialogResult result = dialog.ShowDialog();

            if (result == System.Windows.Forms.DialogResult.OK)
            {
                return await ResultPickGame(dialog.SelectedPath);
            }

            return false;
        }

        private async void btnBrowseGameDir_Click(object sender, RoutedEventArgs e)
        {
            await AddGameManually();
        }

        private async void btnCheckUpdates_Click(object sender, RoutedEventArgs e)
        {
            UIHelper.ToggleButton(ref btnCheckUpdates, false);
            bool isDev = App.isDev;
            bool managerUpdate = isDev ? await App.PerformDevUpdateManagerCheck() : await App.PerformUpdateManagerCheck();
            App.CancelUpdate = false;

            if (managerUpdate)
            {
                return;
            }

            if (App.CurrentGame.id != GameEntry.GameType.Unsupported && App.CurrentGame.loader.installed)
            {
                await App.PerformUpdateLoaderCheck();

                if (App.CurrentGame.id == GameEntry.GameType.SADX)
                {
                    await App.PerformUpdateAppLauncherCheck();
                }
            }

            manualModUpdate = true;
            await CheckForModUpdates(true);
            UIHelper.ToggleButton(ref btnCheckUpdates, true);
            Util.ClearTempFolder();
            Dispatcher.Invoke(Refresh);
        }

        private void AboutBtn_Click(object sender, RoutedEventArgs e)
        {
            new AboutManager().ShowDialog();
        }

        private async void btnInstallLoader_Click(object sender, RoutedEventArgs e)
        {
            // TODO: Check the todos within this function
            if (App.CurrentGame == GamesInstall.Unknown || App.CurrentGame.id == GameEntry.GameType.Unsupported)
                return;

            if (string.IsNullOrEmpty(App.CurrentGame.gameDirectory) || !File.Exists(Path.Combine(App.CurrentGame.gameDirectory, App.CurrentGame.exeName)))
            {
                new MessageWindow(Lang.GetString("MessageWindow.Errors.GamePathFailed.Title"), Lang.GetString("MessageWindow.Errors.GamePathFailed"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
                return;
            }

            btnBrowseGameDir.IsEnabled = false;
            await HandleLoaderInstall();
            UpdateButtonsState();
            btnBrowseGameDir.IsEnabled = true;
            Refresh();
            Save();
        }

        private void btnSource_Click(object sender, RoutedEventArgs e)
        {
            var ps = new ProcessStartInfo("https://github.com/X-Hax/sa-mod-manager")
            {
                UseShellExecute = true,
                Verb = "open"
            };
            Process.Start(ps);
        }

        private void OpenLoaderIssue()
        {
            string url = "";

            switch (App.CurrentGame.id)
            {
                case GameEntry.GameType.SADX:
                    url += "https://github.com/X-Hax/sadx-mod-loader/issues/new";
                    break;
                case GameEntry.GameType.SA2:
                    url += "https://github.com/X-Hax/sa2-mod-loader/issues/new";
                    break;
                case GameEntry.GameType.Unsupported:
                    return;
            }

            url += "?template=bug_report.md";   // Add Template

            var ps = new ProcessStartInfo(url)
            {
                UseShellExecute = true,
                Verb = "open"
            };

            Process.Start(ps);
        }

        private void OpenManagerIssue()
        {
            string url = "https://github.com/X-Hax/SA-Mod-Manager/issues/new";
            url += "?template=01-bug-report.yml";   // Add Configuration.

            var ps = new ProcessStartInfo(url)
            {
                UseShellExecute = true,
                Verb = "open"
            };

            Process.Start(ps);
        }

        private void btnReport_Click(object sender, RoutedEventArgs e)
        {
            var msg = new StringBuilder();
            msg.AppendLine(Lang.GetString("MessageWindow.Information.BugReport.Message1"));
            msg.AppendLine(Lang.GetString("MessageWindow.Information.BugReport.Message2"));
            msg.AppendLine(Lang.GetString("MessageWindow.Information.BugReport.Message3"));

            MessageWindow message = new(Lang.GetString("MessageWindow.Information.BugReport.Title"), msg.ToString(), Lang.GetString("CommonStrings.Yes"), Lang.GetString("CommonStrings.No"),
                icon: MessageWindow.Icons.Information);

            message.ShowDialog();

            if (message.isOK)
                OpenManagerIssue();
        }

        private void comboThemes_Loaded(object sender, RoutedEventArgs e)
        {
            comboThemes.GetBindingExpression(ComboBox.ItemsSourceProperty).UpdateTarget();
            comboThemes.GetBindingExpression(ComboBox.SelectedItemProperty).UpdateTarget();
        }

        private void comboThemes_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            App.SwitchTheme();
        }

        private void comboLanguage_Loaded(object sender, RoutedEventArgs e)
        {
            comboLanguage.GetBindingExpression(ComboBox.ItemsSourceProperty).UpdateTarget();
            comboLanguage.GetBindingExpression(ComboBox.SelectedItemProperty).UpdateTarget();
        }

        private void comboLanguage_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            App.SwitchLanguage();
            UpdateBtnInstallLoader_State();
            FlowDirectionHelper.UpdateFlowDirection();
        }

        private void btnProfileSettings_Click(object sender, RoutedEventArgs e)
        {
            if (App.CurrentGame == GamesInstall.Unknown || !App.CurrentGame.loader.installed)
                return;

            ProfileDialog dialog = new();
            UpdateModsCodes();
            dialog.Owner = this;
            dialog.ShowDialog();
            comboProfile.Items.Refresh();
        }

        private void ModProfile_FormClosing(object sender, EventArgs e)
        {
            Refresh();
        }

        private void comboProfile_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (suppressEvent)
                return;

            var selectedItem = comboProfile.SelectedItem;

            if (selectedItem != null)
            {
                LoadGameSettings();
                SetBindings();
                SetGameUI();
                Refresh();
                Save();
            }
        }
        #endregion
        #endregion

        #region Private Functions
        private void StartGame()
        {
            if (App.CurrentGame == GamesInstall.Unknown)
                return;

            if (string.IsNullOrEmpty(App.CurrentGame.gameDirectory))
            {
                new MessageWindow(Lang.GetString("MessageWindow.Errors.GamePathFailed.Title"), Lang.GetString("MessageWindow.Errors.GamePathFailed"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
                return;
            }

            App.CancelUpdate = true;
            string executablePath = Path.Combine(App.CurrentGame.gameDirectory, App.CurrentGame.exeName);
            foreach (var mod in EnabledMods)
            {
                SAModInfo checkmod = mods[mod];
                if (checkmod.EXEFile?.Length > 0)
                    executablePath = Path.Combine(App.CurrentGame.modDirectory, mod, checkmod.EXEFile);
            }

            Process process = Process.Start((new ProcessStartInfo(executablePath)
            {
                WorkingDirectory = App.CurrentGame.gameDirectory,
                UseShellExecute = true,
            }));

            try
            {
                process?.WaitForInputIdle(10000);
            }
            catch { }

            if ((bool)!checkManagerOpen.IsChecked)
            {
                App.Current.Shutdown();
            }

        }

        public void SetModManagerVersion()
        {
            bool isPortable = Directory.Exists(Path.Combine(App.StartDirectory, "SAManager"));
            string portable = isPortable ? " Portable" : "";
            if (App.isDev == false) //release
            {
                Title = titleName + portable + " " + "(" + Version + ")";
            }
            else
            {
                Title = titleName + portable + " " + "(Dev Build - " + Version + " - " + App.RepoCommit[..7] + ")";

            }

        }

        private void EnableUI(bool enable)
        {
            if (enable)
            {
                tabGame.Visibility = Visibility.Visible;
                comboProfile.IsEnabled = true;

                checkDevEnabled.IsEnabled = true;
                checkManagerOpen.IsEnabled = true;
                grpManUpdates.IsEnabled = true;

                btnInstallLoader.IsEnabled = true;
                RefreshBtn.IsEnabled = true;
                btnSelectAll.IsEnabled = true;
                btnDeselectAll.IsEnabled = true;
                SaveButton.IsEnabled = true;
                SaveAndPlayButton.IsEnabled = true;
                btnSelectAllCode.IsEnabled = true;
                btnDeselectAllCode.IsEnabled = true;
                btnProfileSettings.IsEnabled = true;
            }
            else
            {
                tabGame.Visibility = Visibility.Collapsed;
                comboProfile.IsEnabled = false;

                checkDevEnabled.IsEnabled = false;
                checkManagerOpen.IsEnabled = false;
                grpManUpdates.IsEnabled = false;

                btnInstallLoader.IsEnabled = false;
                RefreshBtn.IsEnabled = false;
                btnSelectAll.IsEnabled = false;
                btnDeselectAll.IsEnabled = false;
                SaveButton.IsEnabled = false;
                SaveAndPlayButton.IsEnabled = false;
                btnSelectAllCode.IsEnabled = false;
                btnDeselectAllCode.IsEnabled = false;
                btnProfileSettings.IsEnabled = false;
            }
        }

        private void SetGameUI()
        {
            Grid stackPanel;
            stackPanel = (Grid)tabGame.Content;
            stackPanel.Children.Clear();
            Grid tsPanel;
            tsPanel = (Grid)tabTestSpawn.Content;
            tsPanel.Children.Clear();
            switch (App.CurrentGame.id)
            {
                case GameEntry.GameType.SADX:
                    EnableUI(true);
                    stackPanel.Children.Add(new Controls.SADX.GameConfig(ref GameProfile, ref suppressEvent));
                    tsPanel.Children.Add(new Controls.SADX.TestSpawn(ref GameProfile));
                    break;
                case GameEntry.GameType.SA2:
                    EnableUI(true);
                    stackPanel.Children.Add(new Controls.SA2.GameConfig(ref GameProfile, ref suppressEvent));
                    tsPanel.Children.Add(new Controls.SA2.TestSpawn(ref GameProfile));
                    break;
                case GameEntry.GameType.Unsupported:
                default:
                    EnableUI(false);
                    break;
            }
        }

        //update all the games information such as loader and mods path
        private void UpdateManagerInfo()
        {
            if (!string.IsNullOrEmpty(App.CurrentGame.gameDirectory) && File.Exists(Path.Combine(App.CurrentGame.gameDirectory, App.CurrentGame.exeName)))
            {

                //App.CurrentGame.modDirectory = Path.Combine(App.CurrentGame.gameDirectory, "mods");
                App.CurrentGame.loader.mlverPath = Path.Combine(App.CurrentGame.modLoaderDirectory, App.CurrentGame.loader.mlverfile);
                App.CurrentGame.loader.IniPath = Path.Combine(App.CurrentGame.gameDirectory, Path.Combine("mods", App.CurrentGame.defaultIniProfile));
                App.CurrentGame.loader.dataDllOriginPath = Path.Combine(App.CurrentGame.gameDirectory, App.CurrentGame.loader.originPath.defaultDataDllOriginPath);
                App.CurrentGame.loader.loaderdllpath = Path.Combine(App.CurrentGame.modLoaderDirectory, App.CurrentGame.loader.name + ".dll");

                var dllPath = App.CurrentGame.loader.loaderdllpath;
                App.CurrentGame.loader.dataDllPath = Path.Combine(App.CurrentGame.gameDirectory, App.CurrentGame.loader.originPath.defaultDataDllPath);

                updatePath = Path.GetFullPath(Path.Combine(App.CurrentGame.modDirectory, ".updates"));
                codelstpath = Path.GetFullPath(Path.Combine(App.CurrentGame.modLoaderDirectory, "Codes.lst"));
                codexmlpath = Path.GetFullPath(Path.Combine(App.CurrentGame.modLoaderDirectory, "Codes.xml"));
                codedatpath = Path.GetFullPath(Path.Combine(App.CurrentGame.modLoaderDirectory, "Codes.dat"));
                patchdatpath = Path.GetFullPath(Path.Combine(App.CurrentGame.modLoaderDirectory, "Patches.dat"));

                App.extLibPath = Path.Combine(App.CurrentGame.modLoaderDirectory, "extlib");

                Controls.SADX.GameConfig.UpdateD3D8Paths();

                App.UpdateDependenciesLocation();

            }
            else
            {
                App.CurrentGame.gameDirectory = string.Empty;
            }

            App.CurrentGame.loader.installed = File.Exists(App.CurrentGame.loader.dataDllOriginPath) && File.Exists(App.CurrentGame.loader.loaderdllpath);
            MigrateLoaderDLL();
            UpdateButtonsState();
        }

        public void UpdateManagerStatusText(string message, int timer = 3000)
        {
            try
            {
                Dispatcher?.Invoke(() => WhatTheManagerDoin.Text = message);
                StatusTimer?.Change(timer, Timeout.Infinite);
            }
            catch
            { }
        }

        public string GetCurrentProfileName()
        {
            int index = comboProfile.SelectedIndex >= 0 ? comboProfile.SelectedIndex : 0;
            return App.Profiles.ProfilesList[index].Filename;
        }

        private void SortCodeAlphanumerically()
        {
            List<CodeData> newCodeList = new();
            foreach (Code item in codes)
            {
                CodeData extraItem = new()
                {
                    codes = item,
                    IsChecked = EnabledCodes.Contains(item.Name),
                    IsEnabled = !item.Required,
                };

                newCodeList.Add(extraItem);
            }

            var sortedCodeDataList = newCodeList.OrderBy(codeData => codeData.codes.Name).ToList();

            foreach (var code in sortedCodeDataList)
            {
                CodeListView.Items.Add(code);
                codesSearch.Add(code);
            }

            newCodeList.Clear();
        }

        private void UpdateModsCodes()
        {
            // Update EnabledMods for saving.
            EnabledMods.Clear();
            foreach (ModData mod in ViewModel.Modsdata)
                if (mod?.IsChecked == true)
                    EnabledMods.Add(mod.Tag);

            // Update EnabledCodes for saving.
            CodesFind.Visibility = Visibility.Collapsed;
            FilterCodes("");
            TextBox_CodesSearch.Text = "";
            EnabledCodes.Clear();
            foreach (CodeData code in CodeListView.Items)
                if (code?.IsChecked == true)
                    EnabledCodes.Add(code.codes.Name);
        }

        #region Private: Load & Save
        private void LoadCodes()
        {
            EnabledCodes = new List<string>(EnabledCodes.Where(a => codes.Any(c => c.Name == a)));

            CodeListView.BeginInit();
            CodeListView.Items.Clear();
            codesSearch.Clear();

            foreach (Code item in codes.Where(a => a.Required && !EnabledCodes.Contains(a.Name)))
                EnabledCodes.Add(item.Name);


            SortCodeAlphanumerically();

            CodeListView.EndInit();
        }

        private void BuildCodeFiles()
        {
            if (CodeListView.Items is null || CodeListView.Items.Count == 0)
                return;


            List<Code> selectedCodes = [];
            List<Code> selectedPatches = [];

            foreach (CodeData code in CodeListView.Items)
            {
                if (code?.IsChecked == true)
                {
                    if (code.codes.Patch)
                    {
                        selectedPatches.Add(code.codes);
                    }
                    else
                    {
                        selectedCodes.Add(code.codes);
                    }
                }
            }

            if (Directory.Exists(App.CurrentGame.modDirectory))
            {
                if (Util.IsStringValid(patchdatpath))
                    CodeList.WriteDatFile(patchdatpath, selectedPatches);

                if (Util.IsStringValid(codedatpath))
                    CodeList.WriteDatFile(codedatpath, selectedCodes);
            }
        }

        private void UpdateButtonsState()
        {
            if (App.CurrentGame.loader is null)
                return;

            bool installed = App.CurrentGame != null && App.CurrentGame.loader.installed;
            UIHelper.ToggleButton(ref btnCheckUpdates, installed);
            UIHelper.ToggleImgButton(ref btnHealthCheck, installed);
            UpdateBtnInstallLoader_State();
        }

        private void LoadSADXSettings(string profilePath, bool newSetup = false)
        {

            Configuration.SADX.GameSettings sadxSettings = File.Exists(profilePath) ? Configuration.SADX.GameSettings.Deserialize(profilePath) : new();
            GameProfile = sadxSettings;

            if (newSetup || Util.IsStringValid(sadxSettings.GamePath) == false)
                sadxSettings.GamePath = tempPath;

            // TODO: Reimplement Check for invalid game directory.
            /*
			 * if (gameMissingFlag == false)
                {
                    var msg = new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), string.Format(Lang.GetString("MessageWindow.Errors.GameNoLongerFound"), App.CurrentGame.gameName, "\n'" + sadxSettings.GamePath + "'\n"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK);
                    msg.ShowDialog();
                    gameMissingFlag = true;
                }
			 */

            EnabledMods = sadxSettings.EnabledMods;
            EnabledCodes = sadxSettings.EnabledCodes;
            gameDebugSettings = sadxSettings.DebugSettings;
        }

        private void LoadSA2Settings(string profilePath, bool newSetup = false)
        {
            Configuration.SA2.GameSettings sa2 = File.Exists(profilePath) ? Configuration.SA2.GameSettings.Deserialize(profilePath) : new();
            GameProfile = sa2;

            if (newSetup || Util.IsStringValid(sa2.GamePath) == false)
                sa2.GamePath = tempPath;

            // TODO: Reimplement Game Missing Check
            /*
			 * if (gameMissingFlag == false)
                {
                    var msg = new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), string.Format(Lang.GetString("MessageWindow.Errors.GameNoLongerFound"), App.CurrentGame.gameName, "\n'" + sa2.GamePath + "'\n"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK);
                    msg.ShowDialog();
                    gameMissingFlag = true;
                }
			 */

            //to do add XML Config support

            if (sa2.EnabledMods is not null)
                EnabledMods = sa2.EnabledMods;
            if (sa2.EnabledCodes is not null)
                EnabledCodes = sa2.EnabledCodes;

            gameDebugSettings = sa2.DebugSettings;
        }

        private void LoadGameSettings(bool newSetup = false)
        {
            var id = App.CurrentGame.id;
            var isnull = ProfileManager.GetProfilesDirectory();
            if (id == GameEntry.GameType.Unsupported || string.IsNullOrEmpty(isnull))
                return;

            ProfileManager.ValidateProfiles();

            string profilePath = Path.Combine(ProfileManager.GetProfilesDirectory(), ProfileManager.GetCurrentProfile().Filename);

            switch (App.CurrentGame.id)
            {
                case GameEntry.GameType.SADX:
                    LoadSADXSettings(profilePath, newSetup);
                    break;
                case GameEntry.GameType.SA2:
                    LoadSA2Settings(profilePath, newSetup);
                    break;
            }
        }

        private void SaveSADXSettings()
        {
            if (string.IsNullOrEmpty(App.CurrentGame.gameDirectory))
                return;

            // Update any GameSettings Info first.
            (GameProfile as Configuration.SADX.GameSettings).GamePath = Path.GetFullPath(App.CurrentGame.gameDirectory);
            Controls.SADX.GameConfig gameConfig = (Controls.SADX.GameConfig)(tabGame.Content as Grid).Children[0];
            gameConfig.SavePatches(ref GameProfile);
            Configuration.SADX.GameSettings sadxSettings = GameProfile as Configuration.SADX.GameSettings;

            // Save Selected Mods
            sadxSettings.EnabledMods = EnabledMods;
            sadxSettings.EnabledCodes = EnabledCodes;
            sadxSettings.DebugSettings = gameDebugSettings;
            sadxSettings.ModsList.Clear();
            foreach (var mod in ViewModel.Modsdata.ToList())
                sadxSettings.ModsList.Add(mod.Tag);

            // Save Game Settings to Current Profile
            sadxSettings.Serialize(ProfileManager.GetCurrentProfile().Filename);

            // Save to Loader Info
            sadxSettings.WriteConfigs();
        }

        private void SaveSA2Settings()
        {
            if (string.IsNullOrEmpty(App.CurrentGame.gameDirectory))
                return;

            // Update any GameSettings Info first.
            (GameProfile as Configuration.SA2.GameSettings).GamePath = Path.GetFullPath(App.CurrentGame.gameDirectory);
            Controls.SA2.GameConfig gameConfig = (Controls.SA2.GameConfig)(tabGame.Content as Grid).Children[0];
            gameConfig.SavePatches(ref GameProfile);
            Configuration.SA2.GameSettings sa2 = GameProfile as Configuration.SA2.GameSettings;

            // Save Selected Mods
            sa2.EnabledMods = EnabledMods;
            sa2.ModsList.Clear();
            foreach (var mod in ViewModel.Modsdata.ToList())
                sa2.ModsList.Add(mod.Tag);

            sa2.EnabledCodes = EnabledCodes;
            sa2.DebugSettings = gameDebugSettings;

            // Save Game Settings to Current Profile
            sa2.Serialize(ProfileManager.GetCurrentProfile().Filename);

            // Save to Loader Info
            sa2.WriteConfigs();
        }

        private void ManualLoaderUpdateCheck()
        {
            if (!File.Exists(App.CurrentGame.loader?.loaderdllpath))
                return;

            try
            {
                if (File.Exists(App.CurrentGame.loader?.dataDllOriginPath) && File.Exists(App.CurrentGame.loader?.dataDllPath))
                {
                    byte[] hash1 = MD5.HashData(File.ReadAllBytes(App.CurrentGame.loader.loaderdllpath));
                    byte[] hash2 = MD5.HashData(File.ReadAllBytes(App.CurrentGame.loader.dataDllPath));

                    if (!hash1.SequenceEqual(hash2))
                    {
                        UpdateManagerStatusText(Lang.GetString("UpdateStatus.LoaderUpdateManual"), 4000);
                        File.Copy(App.CurrentGame.loader.loaderdllpath, App.CurrentGame.loader.dataDllPath, true);
                    }
                }
            }
            catch { }
        }

        public void Load(bool newSetup = false)
        {
            if (newSetup || App.isFirstBoot)
                ProfileManager.CreateProfiles();

            if (App.CurrentGame.id != GameEntry.GameType.Unsupported)
            {
                // Set the existing profiles to the ones from the loaded Manager Settings.
                ProfileManager.ValidateProfileFolder();
                ProfileManager.SetProfile();
                LoadGameSettings(newSetup);
                UpdateManagerIcons();
                UpdateManagerInfo();
                if (!App.isVanillaTransition)
                    ManualLoaderUpdateCheck();


                // Update the UI based on the loaded game.
                SetGameUI();
            }

            Refresh();
            UpdateButtonsState();
        }

        public void Save()
        {
            // Save Manager Settings
            SettingsManager.SaveSettings();

            // If the mods folder doesn't exist, don't save anything.
            if (!Directory.Exists(App.CurrentGame.gameDirectory) || !Directory.Exists(App.CurrentGame.modDirectory))
                return;

            UpdateManagerInfo();

            // Save Mods and Codes
            UpdateModsCodes();

            // Build the Code Files.
            BuildCodeFiles();

            // Save Game Settings here.
            switch (App.CurrentGame.id)
            {
                case GameEntry.GameType.SADX:
                    SaveSADXSettings();
                    break;
                case GameEntry.GameType.SA2:
                    SaveSA2Settings();
                    break;
            }

            // Save the Profiles file.
            ProfileManager.SaveProfiles();

			// Save Manager location.
			SettingsManager.SaveAppDirectoryInfo();

		}

        private void LoadCodesFromMods(SAModInfo inf, string mod)
        {
            //if a mod has a code, add it to the list
            if (!string.IsNullOrEmpty(inf.Codes))
            {
                string fullPath = Path.Combine(Path.Combine(App.CurrentGame.modDirectory, mod), inf.Codes);
                if (File.Exists(fullPath))
                {
                    var t = CodeList.Load(fullPath);
                    codes.AddRange(t.Codes);

                    foreach (var code in t.Codes)
                    {
                        CodeData extraItem = new()
                        {
                            codes = code,
                            IsChecked = EnabledCodes.Contains(inf.Name),
                            IsEnabled = !code.Required,
                        };

                        extraItem.codes.Category = "Codes From " + inf.Name;
                        codesSearch.Add(extraItem);
                    }
                }
            }
        }

        private void LoadConsistentModList()
        {

            //browse the mods folder and get each mod name by their ini file
            foreach (string filename in SAModInfo.GetModFiles(new DirectoryInfo(App.CurrentGame.modDirectory)))
            {
                SAModInfo mod = IniSerializer.Deserialize<SAModInfo>(filename);
                if (mod is null)
                {
                    continue;
                }
                mods.Add((Path.GetDirectoryName(filename) ?? string.Empty)[(App.CurrentGame.modDirectory.Length + 1)..], mod);
            }

            var enabledList = EnabledMods.ToList();


            foreach (KeyValuePair<string, SAModInfo> inf in mods?.OrderBy(x => x.Value?.Name))
            {
                bool Checked = false;

                if (enabledList.Contains(inf.Key))
                    Checked = true;

                var item = new ModData()
                {
                    Name = inf.Value.Name,
                    Author = inf.Value.Author,
                    AuthorURL = inf.Value.AuthorURL,
                    Description = inf.Value.Description,
                    Version = inf.Value.Version,
                    Category = inf.Value.Category,
                    SourceCode = inf.Value.SourceCode,
                    IsChecked = Checked,
                    Tag = inf.Key,
                };

                ViewModel.Modsdata.Add(item);


            }

            string modNotFound = string.Empty;

            //check for errors
            foreach (string mod in enabledList)
            {
                if (!mods.TryGetValue(mod, out SAModInfo value))
                {
                    modNotFound += mod + "\n";
                    EnabledMods.Remove(mod);
                }
                else
                {
                    SAModInfo inf = value;
                    LoadCodesFromMods(inf, mod);
                }
            }

            switch (App.CurrentGame.id)
            {
                case GameEntry.GameType.SADX:
                    {
                        Configuration.SADX.GameSettings sadxSettings = GameProfile as Configuration.SADX.GameSettings;
                        ViewModel.SortViewModelCollection(ViewModel, sadxSettings.ModsList);
                    }
                    break;
                case GameEntry.GameType.SA2:
                    {
                        Configuration.SA2.GameSettings sa2Settings = GameProfile as Configuration.SA2.GameSettings;
                        ViewModel.SortViewModelCollection(ViewModel, sa2Settings.ModsList);
                    }
                    break;
            }



            if (!string.IsNullOrEmpty(modNotFound))
                new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle"), Lang.GetString("MessageWindow.Errors.ModNotFound") + modNotFound, MessageWindow.WindowType.Message, MessageWindow.Icons.Information, MessageWindow.Buttons.OK).ShowDialog();

        }

        private void LoadRegularModList()
        {
            //browse the mods folder and get each mod name by their ini file
            foreach (string filename in SAModInfo.GetModFiles(new DirectoryInfo(App.CurrentGame.modDirectory)))
            {
                SAModInfo mod = IniSerializer.Deserialize<SAModInfo>(filename);
                if (mod is null)
                {
                    continue;
                }
                mods.Add((Path.GetDirectoryName(filename) ?? string.Empty)[(App.CurrentGame.modDirectory.Length + 1)..], mod);
            }

            string modNotFound = string.Empty;

            //load checked mods
            foreach (string mod in EnabledMods.ToList())
            {
                if (mods.TryGetValue(mod, out SAModInfo value))
                {
                    SAModInfo inf = value;
                    suppressEvent = true;
                    var item = new ModData()
                    {
                        Name = inf.Name,
                        Author = inf.Author,
                        AuthorURL = inf.AuthorURL,
                        Description = inf.Description,
                        Version = inf.Version,
                        Category = inf.Category,
                        SourceCode = inf.SourceCode,
                        IsChecked = true,
                        Tag = mod
                    };

                    ViewModel.Modsdata.Add(item);

                    LoadCodesFromMods(inf, mod);
                    suppressEvent = false;
                }
                else
                {
                    modNotFound += mod + "\n";
                    EnabledMods.Remove(mod);
                }
            }

            if (!string.IsNullOrEmpty(modNotFound))
                new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle"), Lang.GetString("MessageWindow.Errors.ModNotFound") + modNotFound, MessageWindow.WindowType.Message, MessageWindow.Icons.Information, MessageWindow.Buttons.OK).ShowDialog();

            //load unchecked mods
            foreach (KeyValuePair<string, SAModInfo> inf in mods?.OrderBy(x => x.Value?.Name))
            {
                if (!EnabledMods.Contains(inf.Key))
                {
                    var item = new ModData()
                    {
                        Name = inf.Value.Name,
                        Author = inf.Value.Author,
                        AuthorURL = inf.Value.AuthorURL,
                        Description = inf.Value.Description,
                        Version = inf.Value.Version,
                        Category = inf.Value.Category,
                        SourceCode = inf.Value.SourceCode,
                        IsChecked = false,
                        Tag = inf.Key,
                    };

                    ViewModel.Modsdata.Add(item);
                }
            }
        }

        private void LoadModList()
        {
            btnMoveTop.IsEnabled = btnMoveUp.IsEnabled = btnMoveDown.IsEnabled = btnMoveBottom.IsEnabled = ConfigureModBtn.IsEnabled = false;
            ViewModel.Modsdata.Clear();

            mods = [];
            codes = new List<Code>(mainCodes.Codes);
            codesSearch = [];

            if (CodeListView is not null)
            {
                CodeListView.BeginInit();
                CodeListView.Items.Clear();
                codesSearch?.Clear();
                EnabledCodes?.Clear();
                CodeListView.EndInit();
            }

            //if game path hasn't been set, give up the process of loading mods.

            if (GamesInstall.IsGameListEmpty() || App.CurrentGame == GamesInstall.Unknown || Directory.Exists(App.CurrentGame.gameDirectory) == false)
            {
                UpdateMainButtonsState();
                return;
            }


            if (File.Exists(Path.Combine(App.CurrentGame.modDirectory, "mod.ini")))
            {
                new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), Lang.GetString("MessageWindow.Errors.ModWithoutFolder0") + Lang.GetString("MessageWindow.Errors.ModWithoutFolder1") +
                            Lang.GetString("MessageWindow.Errors.ModWithoutFolder2"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error,
                            MessageWindow.Buttons.OK).ShowDialog();

                App.Current.Shutdown();
                return;
            }

            if (Directory.Exists(App.CurrentGame.modDirectory) == false)
                Util.CreateSafeDirectory(App.CurrentGame.modDirectory);

            //if setting enabled, don't put checked mods on top of the list
            if (checkKeepModOrder.IsChecked == true)
            {
                LoadConsistentModList();
            }
            else
            {
                LoadRegularModList();
            }

            LoadCodes();
            // DataContext = ViewModel;
            ConfigureModBtn_UpdateState();
        }

        private void Save_AppUserSettings()
        {

            Settings.Default.Save();
        }
        #endregion

        #region Private: Update


        private void UpdateMainButtonsState()
        {
            ConfigureModBtn_UpdateState();
            AddModBtn_UpdateState();
        }

        private void UpdateChecker_EnableControls()
        {
            UpdateManagerStatusText(Lang.GetString("UpdateStatus.NoUpdateFound"));
            UIHelper.ToggleButton(ref btnCheckUpdates, true);

            ModContextChkUpdate.IsEnabled = true;

            ModContextDeveloper.IsEnabled = true;

            ModContextDeleteMod.IsEnabled = true;
            ModContextForceModUpdate.IsEnabled = true;
            ModContextVerifyIntegrity.IsEnabled = true;
            ClearUpdateFolder();
            Util.ClearTempFolder();

        }

        private async Task UpdateSelectedMods()
        {
            manualModUpdate = true;
            modUpdater.updatableMods = listMods.SelectedItems
            .OfType<ModData>()
            .Select(mod => new KeyValuePair<string, ModInfo>(mod.Tag, mods[mod.Tag]))
            .ToList();

            await ExecuteModsUpdateCheck();
        }

        private async Task UpdateChecker_DoWork()
        {

            Dispatcher.Invoke(() =>
            {
                UIHelper.ToggleButton(ref btnCheckUpdates, false);
                ModContextChkUpdate.IsEnabled = false;
                ModContextDeleteMod.IsEnabled = false;

                ModContextDeveloper.IsEnabled = false;
                ModContextForceModUpdate.IsEnabled = false;
                ModContextVerifyIntegrity.IsEnabled = false;
            });

            var tokenSource = new CancellationTokenSource();
            CancellationToken token = tokenSource.Token;
            // out updates, out errors,

            try
            {
                await Task.Run(() => modUpdater.GetModUpdates(App.CurrentGame.modDirectory, token), token);
            }
            catch (OperationCanceledException)
            {
                // Handle cancellation if needed
            }


            modUpdater.modUpdatesTuple = new(modUpdater.modUpdateHelper.updates, modUpdater.modUpdateHelper.errors);
        }

        private async Task UpdateChecker_DoWorkForced()
        {

            if (modUpdater.modUpdatesTuple is null || modUpdater.modManifestTuple.Count == 0)
            {
                return;
            }

            var updatableMods = modUpdater.modManifestTuple;

            var updates = new List<ModDownload>();
            var errors = new List<string>();

            var client = UpdateHelper.HttpClient;

            foreach (Tuple<string, ModInfo, List<Updater.ModManifestDiff>> info in updatableMods)
            {

                ModInfo mod = info.Item2;
                if (!string.IsNullOrEmpty(mod.GitHubRepo))
                {
                    if (string.IsNullOrEmpty(mod.GitHubAsset))
                    {
                        errors.Add($"[{mod.Name}] GitHubRepo specified, but GitHubAsset is missing.");
                        continue;
                    }

                    ModDownload d = await modUpdater.GetGitHubReleases(mod, App.CurrentGame.modDirectory, info.Item1, client, errors);
                    if (d != null)
                    {
                        updates.Add(d);
                    }
                }
                else if (!string.IsNullOrEmpty(mod.GameBananaItemType) && mod.GameBananaItemId.HasValue)
                {
                    ModDownload d = await modUpdater.GetGameBananaReleases(mod, App.CurrentGame.modDirectory, info.Item1, errors);
                    if (d != null)
                    {
                        updates.Add(d);
                    }
                }
                else if (!string.IsNullOrEmpty(mod.UpdateUrl))
                {
                    List<Updater.ModManifestEntry> localManifest = info.Item3
                        .Where(x => x.State == Updater.ModManifestState.Unchanged)
                        .Select(x => x.Current)
                        .ToList();

                    ModDownload d = await modUpdater.CheckModularVersion(mod, App.CurrentGame.modDirectory, info.Item1, localManifest, client, errors);
                    if (d != null)
                    {
                        updates.Add(d);
                    }
                }
            }

            modUpdater.modUpdatesTuple = new Tuple<List<ModDownload>, List<string>>(updates, errors);
        }

        private async Task ExecuteModsUpdateCheck()
        {
            if (!(Util.IsStringValid(App.CurrentGame.gameDirectory) || File.Exists(Path.Combine(App.CurrentGame.gameDirectory, App.CurrentGame.exeName))))
                return;

            try
            {
                ClearUpdateFolder();
                await UpdateChecker_DoWork();
                await UpdateChecker_RunWorkerCompleted();
                UpdateChecker_EnableControls();
            }
            catch (Exception ex)
            {
                Dispatcher.Invoke(() => new ExceptionHandler(ex).ShowDialog());
            }
        }

        private async Task UpdateChecker_RunWorkerCompleted()
        {
            if (modUpdater.ForceUpdate)
            {
                modUpdater.ForceUpdate = false;
                modUpdater.Clear();
            }

            if (modUpdater.modUpdatesTuple is not Tuple<List<ModDownload>, List<string>> data || App.CancelUpdate == true)
            {
                return;
            }

            List<string> Errors = data.Item2;
            if (Errors.Count != 0)
            {
                string msgError = Lang.GetString("MessageWindow.Errors.CheckUpdateError");
                string title = Lang.GetString("MessageWindow.Errors.CheckUpdateError.Title");

                foreach (var error in Errors)
                {
                    msgError += "\n" + error;
                }

                if (!msgError.Contains("403"))
                {
                    new MessageWindow(title, msgError, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
                    await Task.Delay(0);
                }
            }

            bool manual = manualModUpdate;
            manualModUpdate = false;

            List<ModDownload> updates = data.Item1;
            if (updates.Count == 0)
            {
                if (manual && App.CancelUpdate == false)
                {
                    UpdateManagerStatusText(Lang.GetString("UpdateStatus.NoUpdateFound"));
                    new MessageWindow(Lang.GetString("MessageWindow.Information.NoAvailableUpdates.Title"), Lang.GetString("MessageWindow.Information.NoAvailableUpdates"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Information, MessageWindow.Buttons.OK).ShowDialog();
                }

                return;
            }


            var dialog = new ModChangelog(updates);
            dialog.ShowDialog();

            bool? res = dialog.DialogResult;
            if (res != true)
            {
                return;
            }

            updates = dialog.SelectedMods;


            if (updates.Count == 0)
            {
                return;
            }


            Util.CreateSafeDirectory(updatePath);

            var modDL = new Updater.ModDownloadDialog(updates, updatePath);
            modDL.StartDL();
            await Task.Delay(500);
            Refresh();
        }

        private static void ClearUpdateFolder()
        {

            try
            {
                if (App.CurrentGame is not null)
                {
                    string fullPath = Path.Combine(App.CurrentGame.modDirectory, ".updates");
                    Util.DeleteReadOnlyDirectory(fullPath);
                }

            }
            catch { }
        }

        private async Task CheckForModUpdates(bool force = false)
        {
            if (!force && !App.ManagerSettings.UpdateSettings.EnableModsBootCheck || mods is null || mods.Count == 0)
            {
                return;
            }

            if (!force && !Updater.UpdateHelper.UpdateTimeElapsed(App.ManagerSettings.UpdateSettings.UpdateCheckCount, App.ManagerSettings.UpdateSettings.UpdateTimeOutCD))
            {
                UpdateHelper.HandleRefreshUpdateCD();
                SettingsManager.SaveSettings();
                return;
            }

            UpdateManagerStatusText(Lang.GetString("UpdateStatus.ChkModsUpdates"));
            modUpdater.updatableMods = mods.Select(x => new KeyValuePair<string, ModInfo>(x.Key, x.Value)).ToList();

            await ExecuteModsUpdateCheck();

            if (!force)
            {
                App.ManagerSettings.UpdateSettings.UpdateCheckCount++;
                UpdateHelper.HandleRefreshUpdateCD();
                SettingsManager.SaveSettings();
            }
        }
        #endregion

        #region Setup Bindings
        private void SetManagerBindings()
        {
            this.SetBinding(Window.WidthProperty, new Binding("managerWidth")
            {
                Source = App.ManagerSettings,
                Mode = BindingMode.TwoWay
            });
            this.SetBinding(Window.HeightProperty, new Binding("managerHeight")
            {
                Source = App.ManagerSettings,
                Mode = BindingMode.TwoWay
            });
            if (App.Profiles.ProfilesList is not null)
            {
                comboProfile.ItemsSource = App.Profiles.ProfilesList;
                comboProfile.DisplayMemberPath = "Name";
                comboProfile.SetBinding(ComboBox.SelectedIndexProperty, new Binding("ProfileIndex")
                {
                    Source = App.Profiles
                });
            }
            comboLanguage.SetBinding(ComboBox.SelectedIndexProperty, new Binding("Language")
            {
                Source = App.ManagerSettings
            });
            comboThemes.SetBinding(ComboBox.SelectedIndexProperty, new Binding("Theme")
            {
                Source = App.ManagerSettings
            });
            chkUpdateManager.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableManagerBootCheck")
            {
                Source = App.ManagerSettings.UpdateSettings,
            });
            chkUpdatesML.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableLoaderBootCheck")
            {
                Source = App.ManagerSettings.UpdateSettings,
            });
            chkUpdatesMods.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableModsBootCheck")
            {
                Source = App.ManagerSettings.UpdateSettings
            });
            checkDevEnabled.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableDeveloperMode")
            {
                Source = App.ManagerSettings
            });
            checkManagerOpen.SetBinding(CheckBox.IsCheckedProperty, new Binding("KeepManagerOpen")
            {
                Source = App.ManagerSettings
            });
            checkKeepModOrder.SetBinding(CheckBox.IsCheckedProperty, new Binding("KeepModOrder")
            {
                Source = App.ManagerSettings
            });
            textGameDir.SetBinding(TextBox.TextProperty, new Binding("gameDirectory")
            {
                Source = App.CurrentGame,
                Mode = BindingMode.TwoWay
            });
            checkUseSoftwareRendering.SetBinding(CheckBox.IsCheckedProperty, new Binding("UseSoftwareRendering")
            {
                Source = App.ManagerSettings,
                Mode = BindingMode.TwoWay
            });

        }

        private void SetBindings()
        {
            SetManagerBindings();
        }
        #endregion

        #region Private: Mods Tab
        public void FilterMods(string text)
        {
            ViewModel.ModsSearch.Clear();

            foreach (var mod in ViewModel.Modsdata)
            {
                if (mod.Name.ToLowerInvariant().Contains(text) || mod.Author is not null && mod.Author.ToLowerInvariant().Contains(text))
                {
                    ViewModel.ModsSearch.Add(mod); // Add filtered items to the ModsSearch collection.
                }
            }

            string path = BindingOperations.GetBinding(listMods, ListView.ItemsSourceProperty).Path.Path;
            string newPath = text.Length == 0 ? "Modsdata" : "ModsSearch";

            if (path != newPath)
            {
                Binding binding = new()
                {
                    Path = new PropertyPath(newPath)
                };

                listMods.SetBinding(ListView.ItemsSourceProperty, binding);
                listMods.SetValue(GongSolutions.Wpf.DragDrop.DragDrop.IsDragSourceProperty, text.Length == 0);
                listMods.SetValue(GongSolutions.Wpf.DragDrop.DragDrop.IsDropTargetProperty, text.Length == 0);
            }
        }

        private void InitViewModelGames()
        {
            if (App.GamesList[App.GamesList.Count - 1] != GamesInstall.AddGame)
            {
                App.GamesList.Remove(GamesInstall.AddGame);
                App.GamesList.Add(GamesInstall.AddGame);
            }

            ViewModel = new MainWindowViewModel
            {
                Games = new ObservableCollection<Game>(App.GamesList)
            };

            DataContext = ViewModel;
        }

        public void Refresh()
        {
            InitViewModelGames();

            if (App.CurrentGame == GamesInstall.Unknown)
            {
                ComboGameSelection.SelectedValue = App.GamesList.FirstOrDefault();
                return;
            }


            InitCodes();
            LoadModList();
            if (ModsFind.Visibility == Visibility.Visible)
            {
                FilterMods(TextBox_ModsSearch.Text.ToLowerInvariant());
            }

            if (listMods.Items.Count > 0)
                UpdateManagerStatusText(string.Format(Lang.GetString("UpdateStatus.TotalMods"), listMods.Items.Count), 2000);
            else if (CodeListView.Items.Count > 0)
                UpdateManagerStatusText(string.Format(Lang.GetString("UpdateStatus.TotalCodes"), CodeListView.Items.Count), 2000);

            textModsDescription.Text = string.Empty;
            ComboGameSelection.SelectedValue = App.CurrentGame;

        }

        private void OpenAboutModWindow(ModData mod)
        {
            new AboutMod(mod).ShowDialog();
        }

        private void RefreshModList()
        {
            ICollectionView view = CollectionViewSource.GetDefaultView(listMods.Items);
            view.Refresh();
        }

        private ModData GetModFromView(object sender)
        {
            if (sender is ListViewItem lvItem)
                return lvItem.Content as ModData;
            else if (sender is ListView lv)
                return lv.SelectedItem as ModData;
            else if (sender is ModData)
                return sender as ModData;

            return null;
        }

        private void InitModConfig()
        {
            var mod = (ModData)listMods.SelectedItem;

            if (mod is not null)
            {
                SAModInfo modInfo = mods[mod.Tag];
                string fullPath = Path.Combine(App.CurrentGame.modDirectory, mod.Tag);
                ModsCommon.ModConfig config = new(modInfo.Name, fullPath);
                config.ShowDialog();
            }
        }

        private void ConfigureModBtn_UpdateState()
        {
            bool configMod = ConfigureModBtn.IsEnabled;
            //get the config icon image, check if it's not null, then change its oppacity depending if the button is enabled or not.
            Image iconConfig = FindName("configIcon") as Image;
            UIHelper.ToggleImage(ref iconConfig, configMod);
            ConfigureModBtn.Opacity = ConfigureModBtn.IsEnabled ? 1 : 0.3;
        }

        private void AddModBtn_UpdateState()
        {
            if (App.CurrentGame.loader is not null)
            {
                bool isInstalled = App.CurrentGame.loader.installed;
                Image iconConfig = FindName("newIcon") as Image;
                UIHelper.ToggleImage(ref iconConfig, isInstalled);
                UIHelper.ToggleImgButton(ref NewModBtn, isInstalled);
            }
        }
        #endregion

        #region Private: Codes Tab
        public void FilterCodes(string text)
        {
            CodeListView.Items.Clear();

            int enabledIndex = 0;

            foreach (CodeData item in codesSearch)
            {
                if (item.codes.Name.ToLowerInvariant().Contains(text) || (item.codes.Author != null && item.codes.Author.ToLowerInvariant().Contains(text)))
                {
                    if (item.IsChecked)
                        CodeListView.Items.Insert(enabledIndex++, item);
                    else
                        CodeListView.Items.Add(item);
                }
            }
        }

        private void InitCodes()
        {
            try
            {
                if (mainCodes is not null && mainCodes.Codes is not null)
                    mainCodes.Codes.Clear();

                if (File.Exists(codelstpath))
                    mainCodes = CodeList.Load(codelstpath);
                else if (File.Exists(codexmlpath))
                    mainCodes = CodeList.Load(codexmlpath);
                else
                    mainCodes = new CodeList();
            }
            catch (Exception ex)
            {
                string msg = " " + ex.Message;
                new MessageWindow(Lang.GetString("MessageWindow.Errors.CodesListFailed.Title"), Lang.GetString("MessageWindow.Errors.CodesListFailed") + msg, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
                mainCodes = new CodeList();
            }
        }

        private void OpenAboutCodeWindow(Code code)
        {
            new AboutCode(code).ShowDialog();
        }

        private CodeData GetCodeFromView(object sender)
        {
            if (sender is ListViewItem lvItem)
                return lvItem.Content as CodeData;
            else if (sender is ListView lv)
                return lv.SelectedItem as CodeData;
            else if (sender is CodeData)
                return sender as CodeData;


            return CodeListView.Items[CodeListView.SelectedIndex] as CodeData;
        }
        #endregion

        #region Private: Manager Config Tab

        private void btnOpenGameDir_Click(object sender, RoutedEventArgs e)
        {
            string fullPath = App.CurrentGame.gameDirectory;

            if (Directory.Exists(fullPath))
                Process.Start(new ProcessStartInfo { FileName = fullPath, UseShellExecute = true });
        }

        private void UpdateBtnInstallLoader_State()
        {
            if (btnInstallLoader is null || App.CurrentGame is null || App.CurrentGame.loader is null)
                return;

            //Update Text Button of Mod Loader Installer

            string textKey = App.CurrentGame.loader.installed ? "Manager.Tabs.ManagerConfig.Group.Options.Buttons.UninstallLoader" : "Manager.Tabs.ManagerConfig.Group.Options.Buttons.InstallLoader";
            TextBlock txt = FindName("txtInstallLoader") as TextBlock;
            txt.Text = Lang.GetString(textKey);

            //update icon image depending if the Mod Loader is installed or not
            string iconName = App.CurrentGame.loader.installed ? "IconUninstall" : "IconInstall";
            var dic = App.CurrentGame.loader.installed ? Icons.Icons.UninstallIcon : Icons.Icons.InstallIcon;

            DrawingImage Icon = dic[iconName] as DrawingImage;

            Image imgInstall = FindName("imgInstall") as Image;

            if (imgInstall is not null)
                imgInstall.Source = Icon;
        }

        private void UpdateDLLData()
        {
            if (App.CurrentGame.loader is null)
                return;

            if (File.Exists(App.CurrentGame.loader.loaderdllpath) && File.Exists(App.CurrentGame.loader.dataDllOriginPath))
            {
                File.Copy(App.CurrentGame.loader.loaderdllpath, App.CurrentGame.loader.dataDllPath, true);
            }
        }

        //todo delete next update
        private void MigrateLoaderDLL()
        {
            try
            {
                string oldLoaderPath = Path.Combine(App.CurrentGame.modDirectory, App.CurrentGame.loader.name + ".dll");
                if (App.CurrentGame.loader.installed == false && File.Exists(App.CurrentGame.loader.dataDllOriginPath) && File.Exists(oldLoaderPath))
                {
                    if (!File.Exists(App.CurrentGame.loader.loaderdllpath))
                    {
                        //this will fire the mod loader update and it will download the rest
                        File.Copy(oldLoaderPath, App.CurrentGame.loader.loaderdllpath);
                    }
       
                }
            }
            catch { }
        }

        private async Task InstallLoader()
        {
            bool dataDllOrigExist = File.Exists(App.CurrentGame.loader.dataDllOriginPath);
            bool loaderExist = File.Exists(App.CurrentGame.loader.loaderdllpath);

            if (!loaderExist || !dataDllOrigExist)
            {
                UpdateManagerStatusText(Lang.GetString("UpdateStatus.InstallLoader"));
                UIHelper.DisableButton(ref SaveAndPlayButton);

                await GamesInstall.InstallDLL_Loader(App.CurrentGame, false); //first, we download and extract the loader DLL in the mods folder

                UpdateManagerStatusText(Lang.GetString("UpdateStatus.InstallLoader"));
                //now we can move the loader files to the accurate folders.
                await Util.MoveFileAsync(App.CurrentGame.loader.dataDllPath, App.CurrentGame.loader.dataDllOriginPath, false);
                await Util.CopyFileAsync(App.CurrentGame.loader.loaderdllpath, App.CurrentGame.loader.dataDllPath, false);
                await App.EnableOneClickInstall();
                UpdateBtnInstallLoader_State();
                UIHelper.EnableButton(ref btnInstallLoader);
                UIHelper.EnableButton(ref SaveAndPlayButton);

                UpdateManagerStatusText(Lang.GetString("UpdateStatus.LoaderInstalled"));
            }
        }

        private async Task ForceInstallLoader()
        {
            UpdateManagerStatusText(Lang.GetString("UpdateStatus.InstallLoader"));
            UIHelper.DisableButton(ref SaveAndPlayButton);
            UIHelper.DisableButton(ref btnInstallLoader);
            bool retry = false;

            do
            {
                try
                {
                    //this is a failsafe in case the user has already the origin file, such as with the legacy loader
                    if (File.Exists(App.CurrentGame.loader.dataDllOriginPath))
                    {
                        await GamesInstall.InstallDLL_Loader(App.CurrentGame);
                        await Util.CopyFileAsync(App.CurrentGame.loader.loaderdllpath, App.CurrentGame.loader.dataDllPath, true);
                    }
                    else //install normally
                    {
                        await GamesInstall.InstallDLL_Loader(App.CurrentGame, false);
                        UpdateManagerStatusText(Lang.GetString("UpdateStatus.InstallLoader"));
                        //now we can move the loader files to the accurate folders.
                        await Util.MoveFileAsync(App.CurrentGame.loader.dataDllPath, App.CurrentGame.loader.dataDllOriginPath, false);
                        await Util.CopyFileAsync(App.CurrentGame.loader.loaderdllpath, App.CurrentGame.loader.dataDllPath, false);
                    }

                    await App.EnableOneClickInstall();

                    UIHelper.EnableButton(ref SaveAndPlayButton);
                    UpdateManagerStatusText(Lang.GetString("UpdateStatus.LoaderInstalled"));
                    App.CurrentGame.loader.installed = true;
                    retry = false;
                }
                catch (Exception ex)
                {
                    var msg = new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), string.Format(Lang.GetString("MessageWindow.Errors.LoaderCopy"), App.CurrentGame?.loader?.name) + "\n\n" + ex.Message, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.RetryCancel);
                    msg.ShowDialog();
                    retry = msg.isRetry;
                }

            } while (retry == true);


            UpdateBtnInstallLoader_State();
            UIHelper.EnableButton(ref btnInstallLoader);
        }

        private async Task HandleLoaderInstall()
        {
            UIHelper.DisableButton(ref btnInstallLoader);

            bool retry = false;
            do
            {
                try
                {
                    //if user requested to uninstall the loader...
                    if (App.CurrentGame.loader.installed && File.Exists(App.CurrentGame.loader.dataDllOriginPath))
                    {
                        UIHelper.DisableButton(ref SaveAndPlayButton);

                        UpdateManagerStatusText(Lang.GetString("UpdateStatus.UninstallLoader"));

                        File.Delete(App.CurrentGame.loader.dataDllPath);
                        await Util.MoveFile(App.CurrentGame.loader.dataDllOriginPath, App.CurrentGame.loader.dataDllPath);
                        UpdateManagerStatusText(Lang.GetString("UpdateStatus.LoaderUninstalled"));
                        UIHelper.EnableButton(ref SaveAndPlayButton);
                    }
                    else //if user asked to install the loader
                    {
                        ASLR.CheckIfEnabled();
                        await InstallLoader();
                    }
                    retry = false;
                }
                catch (Exception ex)
                {
                    var msg = new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), string.Format(Lang.GetString("MessageWindow.Errors.LoaderCopy"), App.CurrentGame?.loader?.name) + "\n\n" + ex.Message, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.RetryCancel);
                    msg.ShowDialog();
                    retry = msg.isRetry;
                }

            } while (retry == true);


            App.CurrentGame.loader.installed = !App.CurrentGame.loader.installed;
            UIHelper.EnableButton(ref btnInstallLoader);
            UpdateBtnInstallLoader_State();

        }

        #endregion

        #endregion

        private void btnHealthCheck_Click(object sender, RoutedEventArgs e)
        {
            if (App.CurrentGame?.id == GameEntry.GameType.Unsupported)
                return;

            MessageWindow message = new(Lang.GetString("MessageWindow.Warnings.HealthCheckTitle"), Lang.GetString("MessageWindow.Warnings.HealthCheck"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Caution, button: MessageWindow.Buttons.YesNo);

            message.ShowDialog();

            if (message.isYes != true)
                return;

            var progress = new HealthChecker(App.CurrentGame.id);
            progress.ShowDialog();

        }

        private async void textGameDir_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                await ResultPickGame(Path.GetFullPath(textGameDir.Text));
            }
        }

        private string getSA2Icon()
        {
            Random rand = new();
            string name = "Manager/";

            switch (rand.Next(12))
            {
                default:
                    name += "SA2MM.png";
                    break;
                case 0:
                    name += "SA2Sonic.ico";
                    break;
                case 1:
                    name += "SA2Tails.ico";
                    break;
                case 2:
                    name += "SA2Knux.ico";
                    break;
                case 3:
                    name += "SA2Shadow.ico";
                    break;
                case 4:
                    name += "SA2Eggman.ico";
                    break;
                case 5:
                    name += "SA2Rouge.ico";
                    break;
                case 6:
                    name += "SA2Amy.ico";
                    break;
                case 7:
                    name += "SA2Omochao.ico";
                    break;
                case 8:
                    name += "SA2Maria.ico";
                    break;
                case 9:
                    name += "SA2SE.ico";
                    break;
            }

            return name;
        }

        private void SetNewIcon(string titleBarName)
        {
            try
            {
                if (GetTemplateChild(titleBarName) is Image iconTitleBar)
                {
                    Assembly assembly = Assembly.GetExecutingAssembly();
                    string fullResourceName = assembly.GetName().Name;
                    string icon = null;
                    switch (App.CurrentGame.id)
                    {
                        case GameEntry.GameType.SADX:
                        default:
                            icon = "SADXModManager_.png";
                            iconTitleBar.Margin = new Thickness(0, 0, 5, 5);
                            break;
                        case GameEntry.GameType.SA2:
                            icon = getSA2Icon();
                            iconTitleBar.Margin = new Thickness(2, 2, 5, 5);
                            break;
                    }

                    var newImg = new BitmapImage(new Uri("pack://application:,,,/" + fullResourceName + ";component/Icons/" + icon));
                    iconTitleBar.Source = newImg;
                }
            }
            catch { }
        }

        private void UpdateManagerIcons()
        {
            SetNewIcon("IconTitleBar");
            SetNewIcon("IconTitleBar2");
        }

        public void ComboGameSelection_SetNewItem(Game game)
        {
            ComboGameSelection.SelectedItem = game;
            // ComboGameSelection_SelectionChanged(null, null);
        }

        private async Task FirstBootInstallLoader()
        {
            if (string.IsNullOrWhiteSpace(App.CurrentGame?.gameDirectory) == false && File.Exists(Path.Combine(App.CurrentGame.gameDirectory, App.CurrentGame.exeName)))
            {
                await ForceInstallLoader();
                UpdateButtonsState();
                Refresh();
                Save();
            }
        }

        private bool DoGameSwap(Game newGame = null, string newPath = null)
        {
            App.CancelUpdate = true;
            if (newGame is not null)
            {
                EnabledMods.Clear();
                EnabledCodes.Clear();
                App.CurrentGame = newGame;
                GamesInstall.HandleGameSwap(newGame, newPath);
                Load(newPath is not null);
                return true;
            }
            else
            {
                foreach (var game in GamesInstall.GetSupportedGames())
                {
                    if (game == ComboGameSelection.SelectedItem && App.GamesList.Contains(game))
                    {
                        App.ManagerSettings.CurrentSetGame = ComboGameSelection.SelectedIndex;
                        EnabledMods.Clear();
                        EnabledCodes.Clear();
                        App.CurrentGame = game;
                        GamesInstall.HandleGameSwap(game, newPath);
                        Load(newPath is not null);
                        return true;
                    }
                }
            }

            return false;
        }

        private async void ComboGameSelection_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (suppressEvent)
            {
                suppressEvent = false;
                return;
            }

            Game entry = ComboGameSelection.SelectedItem as Game;

            if (entry == GamesInstall.AddGame)
            {
                e.Handled = true;
                ComboGameSelection.SelectedItem = App.CurrentGame;
                await AddGameManually();
                return;
            }


            if (ComboGameSelection != null && ComboGameSelection.SelectedItem != App.CurrentGame)
            {

                bool foundGame = DoGameSwap();

                if (foundGame)
                {
                    Save();
                    SetBindings();
#if !DEBUG
                    if (File.Exists(App.CurrentGame?.loader?.mlverPath) == false || App.isVanillaTransition && App.CurrentGame?.loader?.installed == false)
                        await FirstBootInstallLoader();
#endif

                    await App.EnableOneClickInstall();

                }

            }
        }

        private async void ComboBoxChannel_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            bool isDev = App.isDev;
            string currentChannel = isDev ? App.UpdateChannels[1] : App.UpdateChannels[0];

            if (comboUpdateChannel.SelectedItem != null && currentChannel != comboUpdateChannel.SelectedItem as string)
            {
                if (isDev)
                {
                    var msg = new MessageWindow(Lang.GetString("CommonStrings.Warning"), Lang.GetString("MessageWindow.Warnings.ChannelReleaseSwap"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);
                    msg.ShowDialog();
                    if (msg.isYes)
                    {
                        var update = await GitHub.GetLatestManagerReleaseChannelUpdate();
                        if (update.Item2 is not null)
                        {

                            Logger.Log("Now Installing Latest Manager Update from Channel swap (Release)...");

                            string dlLink = update.Item1.DownloadUrl;
                            string fileName = update.Item1.Name;
                            string version = update.Item2;
                            string destFolder = App.tempFolder;
                            Util.CreateSafeDirectory(destFolder);

                            var dl = new ManagerUpdate(dlLink, destFolder, fileName, version)
                            {
                                DownloadCompleted = async () => await ManagerUpdate.DownloadManagerCompleted(destFolder, fileName)
                            };

                            dl.StartManagerDL();
                        }
                    }
                    else
                    {
                        comboUpdateChannel.SelectedItem = currentChannel;
                    }
                }
                else
                {

                    var msg = new MessageWindow(Lang.GetString("CommonStrings.Warning"), Lang.GetString("MessageWindow.Warnings.ChannelDevSwap"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);
                    msg.ShowDialog();
                    if (msg.isYes)
                    {
                        bool success = await App.PerformDevUpdateManagerCheck();
                        if (!success)
							comboUpdateChannel.SelectedItem = currentChannel;
					}
                    else
                    {
                        comboUpdateChannel.SelectedItem = currentChannel;
                    }
                }
            }
        }

        private void btnAdvancedOptions_Click(object sender, RoutedEventArgs e)
        {
            AdvancedSettingsWindow wind = new AdvancedSettingsWindow();
            wind.Show();
        }

		private void checkUseSoftwareRendering_Click(object sender, RoutedEventArgs e)
		{
            if (App.ManagerSettings.UseSoftwareRendering)
                RenderOptions.ProcessRenderMode = System.Windows.Interop.RenderMode.SoftwareOnly;
            else
                RenderOptions.ProcessRenderMode = System.Windows.Interop.RenderMode.Default;
        }
    }

}