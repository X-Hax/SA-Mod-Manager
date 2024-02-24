using System;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Collections.Generic;
using System.IO;
using System.Windows.Input;
using System.Text.RegularExpressions;
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
using SAModManager.Configuration.SA2;
using SAModManager.Configuration.SADX;

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
        string codelstpath = "mods\\Codes.lst";
        string codexmlpath = "mods\\Codes.xml";
        string codedatpath = "mods\\Codes.dat";
        string patchdatpath = "mods\\Patches.dat";

        // Shared Variables
        CodeList mainCodes = null;
        List<Code> codes = null;
        protected Timer StatusTimer;
        public List<CodeData> codesSearch { get; set; }
        bool suppressEvent = false;
        private bool manualModUpdate;
        private bool checkForUpdate = false;
        readonly Updater.ModUpdater modUpdater = new();
        private object gameConfigFile;
        private DebugSettings gameDebugSettings = new();

        private bool displayedManifestWarning;
        public MainWindowViewModel ViewModel = new();
        Profiles GameProfiles = new();
        object GameProfile;
        public string tempPath = "";
        public static bool cancelUpdate { get; set; }

        public Dictionary<string, SAModInfo> mods = null;
        public List<string> EnabledMods = new();
        public List<string> EnabledCodes = new();
        #endregion

        public MainWindow()
        {
            InitializeComponent();
            UpdateDLLData();
        }

        #region Form: Functions
        private async Task VanillaUpdate_CheckGame()
        {
            bool isValid = false;
            foreach (var game in GamesInstall.GetSupportedGames())
            {
                if (File.Exists(game.exeName))
                {
                    if (game == GamesInstall.SonicAdventure)
                        App.CurrentGame = GamesInstall.GetGamePerID(SetGame.SADX);
                    if (game == GamesInstall.SonicAdventure2)
                        App.CurrentGame = GamesInstall.GetGamePerID(SetGame.SA2);

                    string currentPath = Environment.CurrentDirectory;
                    tempPath = currentPath;
                    App.CurrentGame.gameDirectory = currentPath;
                    UIHelper.ToggleButton(ref btnOpenGameDir, true);
                    await VanillaTransition.ConvertOldProfile(currentPath);
                    await Load(true);
                    isValid = true;
                    break;
                }
            }
            if (isValid)
            {
                await ForceInstallLoader();
                UpdateButtonsState();
                Save();
                App.isVanillaTransition = false;
            }
        }

        private async void MainWindowManager_Loaded(object sender, RoutedEventArgs e)
        {
            this.Resources.MergedDictionaries.Clear(); //this is very important to get Theme and Language swap to work on MainWindow

            StatusTimer = new Timer((state) => UpdateManagerStatusText(string.Empty));
            GamesInstall.AddGamesInstall();
            SetModManagerVersion();

            if (App.isFirstBoot)
            {
                new SplashScreenDialog().ShowDialog();
            }

            ViewModel.Games = App.GamesList;

            await Load();
            DataContext = ViewModel;
            SetBindings();

            UIHelper.ToggleImgButton(ref btnCheckUpdates, true);

            if (App.isVanillaTransition && (App.CurrentGame is null || App.CurrentGame.gameDirectory is null))
                await VanillaUpdate_CheckGame();


            if (string.IsNullOrEmpty(App.CurrentGame?.modDirectory) == false)
            {
                var oneClick = new OneClickInstall(updatePath, App.CurrentGame.modDirectory);
                await oneClick.UriInit();

                if (oneClick.isEmpty == false)
                {
                    return;
                }
            }

#if !DEBUG
            checkForUpdate = true;
            UpdateManagerStatusText(Lang.GetString("UpdateStatus.ChkUpdate"));
            UIHelper.ToggleImgButton(ref btnCheckUpdates, false);
            bool managerUpdate = chkUpdateManager.IsChecked == true && await App.PerformUpdateManagerCheck();
            if (managerUpdate)
            {
                Refresh();
                return;
            }


            if (string.IsNullOrEmpty(App.CurrentGame?.modDirectory) == false)
            {
                if (App.CurrentGame.loader.installed)
                {
                    if (chkUpdatesML.IsChecked == true)
                    {
                        await App.PerformUpdateLoaderCheck();
                        await App.PerformUpdateCodesCheck();
                        await App.PerformUpdatePatchesCheck();
                    }

                    if (App.CurrentGame.id == SetGame.SADX)
                    {
                        await App.PerformUpdateAppLauncherCheck();
                    }
                }

                await CheckForModUpdates();
            }

            UIHelper.ToggleImgButton(ref btnCheckUpdates, true);
            checkForUpdate = false;
#endif
            // Save Manager Settings
            Save();
        }

        private void MainForm_FormClosing(object sender, EventArgs e)
        {
            cancelUpdate = true;
            App.UriQueue.Close();
            Save_AppUserSettings();
        }

        private void SaveButton_Click(object sender, RoutedEventArgs e)
        {
            Save();
        }

        private void SaveAndPlayButton_Click(object sender, RoutedEventArgs e)
        {
            Save();
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
            codesSearch = new();
            string modDir = Path.Combine(App.CurrentGame.gameDirectory, "mods");
            List<string> modlistCopy = new();

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
            }

            foreach (string mod in modlistCopy)
            {
                if (mods.TryGetValue(mod, out SAModInfo value))
                {
                    SAModInfo inf = value;
                    if (!string.IsNullOrEmpty(inf.Codes))
                        codes.AddRange(CodeList.Load(Path.Combine(Path.Combine(modDir, mod), inf.Codes)).Codes);
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

                if (CurMod is not null)
                {
                    CurMod.DisableUpdate = false;
                    string fullPath = Path.Combine(App.CurrentGame.modDirectory, item.Tag, "mod.ini");
                    IniSerializer.Serialize(CurMod, fullPath);
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
            IniSerializer.Serialize(modInfo, fullPath);
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

                UIHelper.ToggleImgButton(ref btnCheckUpdates, false);
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

        private async Task ResultPickGame(string path)
        {
            var setGame = await GamesInstall.SetGameInstallManual(path);

            if (setGame == SetGame.None)
            {
                new MessageWindow(Lang.GetString("MessageWindow.Errors.GamePathFailed.Title"), Lang.GetString("MessageWindow.Errors.GamePathFailed"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
            }
            else
            {
                var game = GamesInstall.GetGamePerID(setGame);

                if (App.GamesList.Contains(game) == false)
                    App.GamesList.Add(game);

                tempPath = path;
                App.CurrentGame.gameDirectory = tempPath;
                UIHelper.ToggleButton(ref btnOpenGameDir, true);
                await Load(true);
                await ForceInstallLoader();
                UpdateButtonsState();
                Save();
            }
        }


        private async void btnBrowseGameDir_Click(object sender, RoutedEventArgs e)
        {
            var dialog = new System.Windows.Forms.FolderBrowserDialog();

            System.Windows.Forms.DialogResult result = dialog.ShowDialog();

            if (result == System.Windows.Forms.DialogResult.OK)
            {
                await ResultPickGame(dialog.SelectedPath);
            }
        }

        private async void btnCheckUpdates_Click(object sender, RoutedEventArgs e)
        {
            UIHelper.ToggleImgButton(ref btnCheckUpdates, false);
            checkForUpdate = true;

            if (await App.PerformUpdateManagerCheck())
            {
                return;
            }

            if (App.CurrentGame.loader.installed)
            {
                await App.PerformUpdateLoaderCheck();
                await App.PerformUpdateCodesCheck();
                await App.PerformUpdatePatchesCheck();

                if (App.CurrentGame.id == SetGame.SADX)
                {
                    await App.PerformUpdateAppLauncherCheck();
                }
            }

            manualModUpdate = true;
            await CheckForModUpdates(true);
            checkForUpdate = false;
            UIHelper.ToggleImgButton(ref btnCheckUpdates, true);
            Dispatcher.Invoke(Refresh);
        }

        private void AboutBtn_Click(object sender, RoutedEventArgs e)
        {
            new AboutManager().ShowDialog();
        }

        private async void btnInstallLoader_Click(object sender, RoutedEventArgs e)
        {
            // TODO: Check the todos within this function
            if (string.IsNullOrEmpty(App.CurrentGame.gameDirectory) || !File.Exists(Path.Combine(App.CurrentGame.gameDirectory, App.CurrentGame.exeName)))
            {
                new MessageWindow(Lang.GetString("MessageWindow.Errors.GamePathNotFound.Title"), Lang.GetString("MessageWindow.Errors.GamePathNotFound"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
                return;
            }

            btnBrowseGameDir.IsEnabled = false;
            await HandleLoaderInstall();
            UpdateButtonsState();
            btnBrowseGameDir.IsEnabled = true;
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
                case SetGame.SADX:
                    url += "https://github.com/X-Hax/sadx-mod-loader/issues/new";
                    break;
                case SetGame.SA2:
                    url += "https://github.com/X-Hax/sa2-mod-loader/issues/new";
                    break;
                case SetGame.None:
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
            url += "?template=bug_report.md";   // Add Template

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
            msg.AppendLine($" - {Lang.GetString("MessageWindow.Information.BugReport.Message2")}");
            msg.AppendLine($" - {Lang.GetString("MessageWindow.Information.BugReport.Message3")}");
            msg.AppendLine($" - {Lang.GetString("MessageWindow.Information.BugReport.Message4")}");
            msg.AppendLine();
            msg.AppendLine(Lang.GetString("MessageWindow.Information.BugReport.Message5"));
            msg.AppendLine($" - {Lang.GetString("MessageWindow.Information.BugReport.Message6")}");
            msg.AppendLine($" - {Lang.GetString("MessageWindow.Information.BugReport.Message7")}");

            MessageWindow message = new(Lang.GetString("MessageWindow.Information.BugReport.Title"), msg.ToString(), "Mod Manager", "Mod Loader",
                icon: MessageWindow.Icons.Information);

            message.ShowDialog();

            if (message.isOK)
                OpenManagerIssue();

            if (message.isCancelled)
                OpenLoaderIssue();
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
            if (!App.CurrentGame.loader.installed)
                return;

            int index = comboProfile.SelectedIndex;
            ProfileDialog dialog = new(ref GameProfiles, index);
            UpdateModsCodes();
            dialog.Owner = this;
            dialog.ShowDialog();

            // Save the Profiles file.
            GameProfiles.Serialize(Path.Combine(App.CurrentGame.ProfilesDirectory, "Profiles.json"));
            comboProfile.ItemsSource = GameProfiles.ProfilesList;
            comboProfile.SelectedIndex = dialog.SelectedIndex;
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
                GameProfiles.ProfileIndex = comboProfile.SelectedIndex;

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
            if (string.IsNullOrEmpty(App.CurrentGame.gameDirectory))
            {
                new MessageWindow(Lang.GetString("MessageWindow.Errors.GamePathNotFound.Title"), Lang.GetString("MessageWindow.Errors.GamePathNotFound"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
                return;
            }

            cancelUpdate = true;
            string executablePath = EnabledMods.Select(item => mods[item].EXEFile).FirstOrDefault(item => !string.IsNullOrEmpty(item)) ?? Path.Combine(App.CurrentGame.gameDirectory, App.CurrentGame.exeName);
            string folderPath = Path.GetDirectoryName(executablePath);


            Process process = Process.Start((new ProcessStartInfo(executablePath)
            {
                WorkingDirectory = folderPath,
                UseShellExecute = true,

            }));

            try
            {
                process?.WaitForInputIdle(10000);
            }
            catch { }

            if ((bool)!checkManagerOpen.IsChecked)
                Close();
        }

        public void SetModManagerVersion()
        {
            if (string.IsNullOrEmpty(App.RepoCommit)) //dev
                Title = titleName + " " + "(Dev Build - " + Version + ")";
            else
                Title = titleName + " " + "(" + Version + " - " + App.RepoCommit[..7] + ")";
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
                case SetGame.SADX:
                    EnableUI(true);
                    stackPanel.Children.Add(new Controls.SADX.GameConfig(ref GameProfile));
                    tsPanel.Children.Add(new Controls.SADX.TestSpawn(ref GameProfile, ref mods, ref EnabledMods));
                    break;
                case SetGame.SA2:
                    EnableUI(true);
                    stackPanel.Children.Add(new Controls.SA2.GameConfig(ref GameProfile));
                    tsPanel.Children.Add(new Controls.SA2.TestSpawn(ref GameProfile, ref mods, ref EnabledMods));
                    break;
                case SetGame.None:
                default:
                    EnableUI(false);
                    break;
            }
        }

        //update all the games information such as loader and mods path
        private async Task UpdateManagerInfo()
        {
            if (!string.IsNullOrEmpty(App.CurrentGame.gameDirectory) && File.Exists(Path.Combine(App.CurrentGame.gameDirectory, App.CurrentGame.exeName)))
            {

                App.CurrentGame.modDirectory = Path.Combine(App.CurrentGame.gameDirectory, "mods");

                App.CurrentGame.loader.loaderinipath = Path.Combine(App.CurrentGame.gameDirectory, Path.Combine("mods", App.CurrentGame.defaultIniProfile));
                App.CurrentGame.loader.dataDllOriginPath = Path.Combine(App.CurrentGame.gameDirectory, App.CurrentGame.loader.originPath.defaultDataDllOriginPath);
                App.CurrentGame.loader.loaderdllpath = Path.Combine(App.CurrentGame.gameDirectory, Path.Combine(App.CurrentGame.modDirectory, App.CurrentGame.loader.name + ".dll"));
                App.CurrentGame.loader.dataDllPath = Path.Combine(App.CurrentGame.gameDirectory, App.CurrentGame.loader.originPath.defaultDataDllPath);

                updatePath = Path.GetFullPath(Path.Combine(App.CurrentGame.gameDirectory, "mods", ".updates"));
                codelstpath = Path.GetFullPath(Path.Combine(App.CurrentGame.gameDirectory, "mods", "Codes.lst"));
                codexmlpath = Path.GetFullPath(Path.Combine(App.CurrentGame.gameDirectory, "mods", "Codes.xml"));
                codedatpath = Path.GetFullPath(Path.Combine(App.CurrentGame.gameDirectory, "mods", "Codes.dat"));
                patchdatpath = Path.GetFullPath(Path.Combine(App.CurrentGame.gameDirectory, "mods", "Patches.dat"));

                if (App.CurrentGame?.id == SetGame.SADX)
                    Controls.SADX.GameConfig.UpdateD3D8Paths();

                //this is a failsafe in case the User deleted the Loader file manually without restoring the original files
                if (!File.Exists(App.CurrentGame.loader.loaderdllpath) && File.Exists(App.CurrentGame.loader.dataDllOriginPath))
                {
                    await GamesInstall.InstallDLL_Loader(App.CurrentGame); //re install the loader to avoid corrupted install
                    File.Copy(App.CurrentGame.loader.loaderdllpath, App.CurrentGame.loader.dataDllPath, true);
                }
            }
            else
            {
                App.CurrentGame.gameDirectory = string.Empty;
            }

            App.CurrentGame.loader.installed = File.Exists(App.CurrentGame.loader.dataDllOriginPath) && File.Exists(App.CurrentGame.loader.loaderdllpath);
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
            return GameProfiles.ProfilesList[index].Filename;
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

        private void LoadGameConfigFile()
        {
            // TODO: Properly update this for loading SA2's config file.
            List<string> gameConfig = new();
            foreach (string file in App.CurrentGame.GameConfigFile)
            {
                gameConfig.Add(Path.Combine(App.CurrentGame.gameDirectory, file));
            }

            switch (App.CurrentGame.id)
            {
                case SetGame.SADX:
                    gameConfigFile = File.Exists(gameConfig[0]) ? IniSerializer.Deserialize<SADXConfigFile>(gameConfig[0]) : new SADXConfigFile();
                    break;
                case SetGame.SA2:
                    gameConfigFile = File.Exists(gameConfig[1]) ? SA2GameConfig.Deserialize(gameConfig[1]) : new SA2GameConfig();
                    break;
            }
        }

        private void LoadCodes()
        {
            EnabledCodes = new List<string>(EnabledCodes.Where(a => codes.Any(c => c.Name == a)));

            CodeListView.BeginInit();
            CodeListView.Items.Clear();

            foreach (Code item in codes.Where(a => a.Required && !EnabledCodes.Contains(a.Name)))
                EnabledCodes.Add(item.Name);

            foreach (Code item in codes)
            {
                CodeData extraItem = new()
                {
                    codes = item,
                    IsChecked = EnabledCodes.Contains(item.Name),
                    IsEnabled = !item.Required,
                };

                codesSearch.Add(extraItem);
                CodeListView.Items.Add(extraItem);
            }

            CodeListView.EndInit();
        }

        private void BuildCodeFiles()
        {
            List<Code> selectedCodes = new();
            List<Code> selectedPatches = new();

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

            CodeList.WriteDatFile(patchdatpath, selectedPatches);
            CodeList.WriteDatFile(codedatpath, selectedCodes);
        }

        private void UpdateButtonsState()
        {
            if (App.CurrentGame.loader is null)
                return;

            bool installed = App.CurrentGame != null && App.CurrentGame.loader.installed;
            UIHelper.ToggleImgButton(ref btnCheckUpdates, installed);
            UIHelper.ToggleImgButton(ref btnHealthCheck, installed);
            UpdateBtnInstallLoader_State();
            Update_PlayButtonsState();
        }

        private void LoadSADXSettings(string profilePath, bool newSetup = false)
        {
            Configuration.SADX.GameSettings sadxSettings = File.Exists(profilePath) ? Configuration.SADX.GameSettings.Deserialize(profilePath) : new();
            GameProfile = sadxSettings;

            if (newSetup || sadxSettings.GamePath is null)
                sadxSettings.GamePath = tempPath;

            if (!string.IsNullOrEmpty(sadxSettings.GamePath))
            {
                textGameDir.Text = sadxSettings.GamePath;
                App.CurrentGame.gameDirectory = sadxSettings.GamePath;
                App.CurrentGame.modDirectory = Path.Combine(sadxSettings.GamePath, "mods");
            }

            EnabledMods = sadxSettings.EnabledMods;
            EnabledCodes = sadxSettings.EnabledCodes;
            gameDebugSettings = sadxSettings.DebugSettings;
        }

        private void LoadSA2Settings(string profilePath, bool newSetup = false)
        {

            Configuration.SA2.GameSettings sa2 = File.Exists(profilePath) ? Configuration.SA2.GameSettings.Deserialize(profilePath) : new();
            GameProfile = sa2;

            if (newSetup || sa2.GamePath is null)
                sa2.GamePath = tempPath;


            if (!string.IsNullOrEmpty(sa2.GamePath))
            {
                textGameDir.Text = sa2.GamePath;
                App.CurrentGame.gameDirectory = sa2.GamePath;
                App.CurrentGame.modDirectory = Path.Combine(sa2.GamePath, "mods");
            }

            //to do add XML Config support

            if (sa2.EnabledMods is not null)
                EnabledMods = sa2.EnabledMods;
            if (sa2.EnabledCodes is not null)
                EnabledCodes = sa2.EnabledCodes;

            gameDebugSettings = sa2.DebugSettings;
        }

        private void LoadGameSettings(bool newSetup = false)
        {
            string profilePath = Path.Combine(App.CurrentGame.ProfilesDirectory, GameProfiles.GetProfileFilename());

            switch (App.CurrentGame.id)
            {
                case SetGame.SADX:
                    LoadSADXSettings(profilePath, newSetup);
                    break;
                case SetGame.SA2:
                    LoadSA2Settings(profilePath, newSetup);
                    break;
            }

            if (ComboGameSelection is not null && App.CurrentGame is not null)
                ComboGameSelection.SelectedValue = App.CurrentGame;
        }

        private void SaveSADXSettings()
        {
            // Update any GameSettings Info first.
            (GameProfile as Configuration.SADX.GameSettings).GamePath = App.CurrentGame.gameDirectory;
            Controls.SADX.GameConfig gameConfig = (Controls.SADX.GameConfig)(tabGame.Content as Grid).Children[0];
            gameConfig.SavePatches(ref GameProfile);
            Controls.SADX.TestSpawn spawnConfig = (Controls.SADX.TestSpawn)(tabTestSpawn.Content as Grid).Children[0];

            Configuration.SADX.GameSettings sadxSettings = GameProfile as Configuration.SADX.GameSettings;

            // Save Selected Mods
            sadxSettings.EnabledMods = EnabledMods;
            sadxSettings.EnabledCodes = EnabledCodes;
            sadxSettings.DebugSettings = gameDebugSettings;

            // Save Game Settings to Current Profile
            string profilePath = Path.Combine(App.CurrentGame.ProfilesDirectory, GetCurrentProfileName());
            sadxSettings.Serialize(profilePath, GetCurrentProfileName());

            // Save to Loader Info
            sadxSettings.WriteConfigs();
        }

        private void SaveSA2Settings()
        {
            // Update any GameSettings Info first.
            (GameProfile as Configuration.SA2.GameSettings).GamePath = App.CurrentGame.gameDirectory;
            Controls.SA2.GameConfig gameConfig = (Controls.SA2.GameConfig)(tabGame.Content as Grid).Children[0];
            gameConfig.SavePatches(ref GameProfile);
            Controls.SA2.TestSpawn spawnConfig = (Controls.SA2.TestSpawn)(tabTestSpawn.Content as Grid).Children[0];

            Configuration.SA2.GameSettings sa2 = GameProfile as Configuration.SA2.GameSettings;

            // Save Selected Mods
            sa2.EnabledMods = EnabledMods;
            sa2.EnabledCodes = EnabledCodes;
            sa2.DebugSettings = gameDebugSettings;

            // Save Game Settings to Current Profile
            string profilePath = Path.Combine(App.CurrentGame.ProfilesDirectory, GetCurrentProfileName());
            sa2.Serialize(profilePath, GetCurrentProfileName());

            // Save to Loader Info
            sa2.WriteConfigs();
        }

        private void ManualLoaderUpdateCheck()
        {
            if (File.Exists(App.CurrentGame.loader.dataDllOriginPath) && File.Exists(App.CurrentGame.loader.dataDllPath))
            {
                byte[] hash1 = MD5.HashData(File.ReadAllBytes(App.CurrentGame.loader.loaderdllpath));
                byte[] hash2 = MD5.HashData(File.ReadAllBytes(App.CurrentGame.loader.dataDllPath));

                if (!hash1.SequenceEqual(hash2))
                {
                    File.Copy(App.CurrentGame.loader.loaderdllpath, App.CurrentGame.loader.dataDllPath, true);
                }
            }
        }
        public async Task Load(bool newSetup = false)
        {
            if (App.CurrentGame.id != SetGame.None)
            {
                // Load Profiles before doing anything.
                string profiles = Path.Combine(App.CurrentGame.ProfilesDirectory, "Profiles.json");
                GameProfiles = File.Exists(profiles) ? Profiles.Deserialize(profiles) : Profiles.MakeDefaultProfileFile();
                if (GameProfiles.ValidateProfiles() == false)
                {
                    GameProfiles.ProfileIndex = 0; //if validation fail, default to 0 and save changes
                    GameProfiles.Serialize(Path.Combine(App.CurrentGame.ProfilesDirectory, "Profiles.json"));
                }
                comboProfile.ItemsSource = GameProfiles.ProfilesList;
                comboProfile.DisplayMemberPath = "Name";
                suppressEvent = true;
                comboProfile.SelectedIndex = GameProfiles.ProfileIndex;
                suppressEvent = false;

                // Set the existing profiles to the ones from the loaded Manager Settings.
                LoadGameSettings(newSetup);
                UpdateManagerIcons();
                await UpdateManagerInfo();
                if (!App.isVanillaTransition)
                    ManualLoaderUpdateCheck();
                InitCodes();
                LoadModList();
            }
            else
            {
                UpdateButtonsState();
                UIHelper.ToggleButton(ref btnOpenGameDir, false);

                if (App.GamesList is not null && (App.CurrentGame.loader is null || Directory.Exists(App.CurrentGame.gameDirectory) == false))
                {
                    if (App.Current.MainWindow is not null)
                        ((MainWindow)App.Current.MainWindow).ComboGameSelection_SetNewItem(App.GamesList[0]);
                }
            }

            // Update the UI based on the loaded game.
            SetGameUI();

            await Task.Delay(200);
        }

        public void Save()
        {
            // If the mods folder doesn't exist, don't save anything.
            if (!Directory.Exists(App.CurrentGame.modDirectory))
                return;

            // Save Manager Settings
            App.ManagerSettings.CurrentSetGame = (int)App.CurrentGame?.id;
            App.ManagerSettings.Serialize(App.ManagerConfigFile);

            // Save Mods and Codes
            UpdateModsCodes();

            // Build the Code Files.
            BuildCodeFiles();

            // Create the Profiles Directory if it doesn't exist.
            if (!Directory.Exists(App.CurrentGame.ProfilesDirectory))
                Directory.CreateDirectory(App.CurrentGame.ProfilesDirectory);

            // Save Game Settings here.
            switch (App.CurrentGame.id)
            {
                case SetGame.SADX:
                    SaveSADXSettings();
                    break;
                case SetGame.SA2:
                    SaveSA2Settings();
                    break;
            }

            // Save the Profiles file.
            GameProfiles.Serialize(Path.Combine(App.CurrentGame.ProfilesDirectory, "Profiles.json"));

            // Refresh thing so everything updates as intended.
            Refresh();
        }

        private void LoadModList()
        {
            btnMoveTop.IsEnabled = btnMoveUp.IsEnabled = btnMoveDown.IsEnabled = btnMoveBottom.IsEnabled = ConfigureModBtn.IsEnabled = false;
            ViewModel.Modsdata.Clear();

            mods = new Dictionary<string, SAModInfo>();
            codes = new List<Code>(mainCodes.Codes);
            codesSearch = new();

            bool modFolderExist = Directory.Exists(App.CurrentGame.modDirectory);

            //if mod folder doesn't exist and game path hasn't been set, give up the process of loading mods.

            if (!modFolderExist && string.IsNullOrEmpty(App.CurrentGame.gameDirectory))
            {
                UpdateMainButtonsState();
                return;
            }
            else if (Directory.Exists(App.CurrentGame.gameDirectory) && !modFolderExist)
            {
                try
                {
                    Directory.CreateDirectory(App.CurrentGame.modDirectory);
                }
                catch { }
            }

            if (File.Exists(Path.Combine(App.CurrentGame.modDirectory, "mod.ini")))
            {
                new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), Lang.GetString("MessageWindow.Errors.ModWithoutFolder0") + Lang.GetString("MessageWindow.Errors.ModWithoutFolder1") +
                            Lang.GetString("MessageWindow.Errors.ModWithoutFolder2"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error,
                            MessageWindow.Buttons.OK).ShowDialog();

                Close();
                return;
            }

            //browse the mods folder and get each mod name by their ini file
            foreach (string filename in SAModInfo.GetModFiles(new DirectoryInfo(App.CurrentGame.modDirectory)))
            {
                SAModInfo mod = IniSerializer.Deserialize<SAModInfo>(filename);
                mods.Add((Path.GetDirectoryName(filename) ?? string.Empty).Substring(App.CurrentGame.modDirectory.Length + 1), mod);
            }

            string modNotFound = string.Empty;

            foreach (string mod in EnabledMods.ToList())
            {
                if (mods.ContainsKey(mod))
                {
                    SAModInfo inf = mods[mod];
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
                                    IsChecked = EnabledCodes.Contains(item.Name),
                                    IsEnabled = !code.Required,
                                };

                                extraItem.codes.Category = "Codes From " + inf.Name;

                                codesSearch.Add(extraItem);
                                CodeListView.Items.Add(extraItem);
                            }
                        }
                    }

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

            foreach (KeyValuePair<string, SAModInfo> inf in mods.OrderBy(x => x.Value.Name))
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

            LoadCodes();
            DataContext = ViewModel;
            ConfigureModBtn_UpdateState();
        }

        private void Save_AppUserSettings()
        {
            Settings.Default.Save();
        }
        #endregion

        #region Private: Update

        private void Update_PlayButtonsState()
        {
            bool isInstalled = App.CurrentGame.loader.installed;
            UIHelper.ToggleButton(ref SaveAndPlayButton, isInstalled);
            Image iconSavePlay = FindName("savePlayIcon") as Image;
            UIHelper.ToggleImage(ref iconSavePlay, isInstalled);
        }

        private void UpdateMainButtonsState()
        {
            ConfigureModBtn_UpdateState();
            AddModBtn_UpdateState();
        }

        private void UpdateChecker_EnableControls()
        {
            UpdateManagerStatusText(Lang.GetString("UpdateStatus.NoUpdateFound"));
            UIHelper.ToggleImgButton(ref btnCheckUpdates, true);

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
                UIHelper.ToggleImgButton(ref btnCheckUpdates, false);
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

            if (modUpdater.modUpdatesTuple is not Tuple<List<ModDownload>, List<string>> data)
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
                if (manual && !MainWindow.cancelUpdate)
                {
                    UpdateManagerStatusText(Lang.GetString("UpdateStatus.NoUpdateFound"));
                    new MessageWindow(Lang.GetString("MessageWindow.Information.NoAvailableUpdates.Title"), Lang.GetString("MessageWindow.Information.NoAvailableUpdates"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Information, MessageWindow.Buttons.OK).ShowDialog();
                }
                MainWindow.cancelUpdate = false;
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

            try
            {
                if (!Directory.Exists(updatePath))
                {
                    Directory.CreateDirectory(updatePath);
                }
            }
            catch { }

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

            MainWindow.cancelUpdate = false;

            if (!force && !Updater.UpdateHelper.UpdateTimeElapsed(App.ManagerSettings.UpdateSettings.UpdateCheckCount, App.ManagerSettings.UpdateSettings.UpdateTimeOutCD))
            {
                UpdateHelper.HandleRefreshUpdateCD();
                App.ManagerSettings.Serialize(App.ManagerConfigFile);
                return;
            }

            UpdateManagerStatusText(Lang.GetString("UpdateStatus.ChkModsUpdates"));
            modUpdater.updatableMods = mods.Select(x => new KeyValuePair<string, ModInfo>(x.Key, x.Value)).ToList();

            await ExecuteModsUpdateCheck();

            if (!force)
            {
                App.ManagerSettings.UpdateSettings.UpdateCheckCount++;
                UpdateHelper.HandleRefreshUpdateCD();
                App.ManagerSettings.Serialize(App.ManagerConfigFile);
            }
        }
        #endregion

        #region Setup Bindings
        private void SetGameDebugBindings()
        {
            checkEnableLogConsole.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableDebugConsole")
            {
                Source = gameDebugSettings,
            });
            checkEnableLogScreen.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableDebugScreen")
            {
                Source = gameDebugSettings,
            });
            checkEnableLogFile.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableDebugFile")
            {
                Source = gameDebugSettings,
            });
            checkEnableCrashDump.SetBinding(CheckBox.IsCheckedProperty, new Binding("EnableDebugCrashLog")
            {
                Source = gameDebugSettings,
            });
        }

        private void SetManagerBindings()
        {
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
            tabTestSpawn.SetBinding(TabItem.VisibilityProperty, new Binding("IsChecked")
            {
                Source = checkDevEnabled,
                Mode = BindingMode.OneWay,
                Converter = new VisibilityConverter()
            });
        }

        private void SetBindings()
        {
            SetGameDebugBindings();
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
                listMods.SetValue(GongSolutions.Wpf.DragDrop.DragDrop.IsDragSourceProperty, text.Length == 0 ? true : false);
                listMods.SetValue(GongSolutions.Wpf.DragDrop.DragDrop.IsDropTargetProperty, text.Length == 0 ? true : false);
            }
        }

        public async void Refresh()
        {
            InitCodes();
            LoadModList();
            if (ModsFind.Visibility == Visibility.Visible)
            {
                FilterMods(TextBox_ModsSearch.Text.ToLowerInvariant());
            }

            if (!checkForUpdate)
            {
                if (listMods.Items.Count > 0)
                    UpdateManagerStatusText(string.Format(Lang.GetString("UpdateStatus.TotalMods"), listMods.Items.Count), 2000);
                await Task.Delay(2200);

                if (CodeListView.Items.Count > 0)
                    UpdateManagerStatusText(string.Format(Lang.GetString("UpdateStatus.TotalCodes"), CodeListView.Items.Count), 2000);
            }
            textModsDescription.Text = string.Empty;

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

        private void UpdateGameConfig(SetGame game)
        {
            App.CurrentGame = GamesInstall.GetGamePerID(game);
            Directory.CreateDirectory(App.CurrentGame.ProfilesDirectory);
            LoadGameConfigFile();
            SetGameUI();
            SetBindings();
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

        private async Task InstallLoader()
        {
            if (!File.Exists(App.CurrentGame.loader.dataDllOriginPath))
            {
                UpdateManagerStatusText(Lang.GetString("UpdateStatus.InstallLoader"));
                UIHelper.DisableButton(ref SaveAndPlayButton);

                await GamesInstall.InstallDLL_Loader(App.CurrentGame); //first, we download and extract the loader DLL in the mods folder
                await GamesInstall.CheckAndInstallDependencies(App.CurrentGame); //we check if some libraries are missing (BASS, D3D9...)

                UpdateManagerStatusText(Lang.GetString("UpdateStatus.InstallLoader"));
                //now we can move the loader files to the accurate folders.
                await Util.MoveFileAsync(App.CurrentGame.loader.dataDllPath, App.CurrentGame.loader.dataDllOriginPath, false);
                await Util.CopyFileAsync(App.CurrentGame.loader.loaderdllpath, App.CurrentGame.loader.dataDllPath, false);
                UpdateGameConfig(App.CurrentGame.id);
                await App.EnableOneClickInstall();
                UIHelper.EnableButton(ref SaveAndPlayButton);

                UpdateManagerStatusText(Lang.GetString("UpdateStatus.LoaderInstalled"));
            }
        }

        private async Task ForceInstallLoader()
        {
            UpdateManagerStatusText(Lang.GetString("UpdateStatus.InstallLoader"));
            UIHelper.DisableButton(ref SaveAndPlayButton);
            UIHelper.DisableButton(ref btnInstallLoader);


            //File.WriteAllText(App.CurrentGame.loader.loaderVersionpath, update.Item2);
            if (File.Exists(App.CurrentGame.loader.dataDllOriginPath))
            {
                await GamesInstall.InstallDLL_Loader(App.CurrentGame);
                await GamesInstall.UpdateDependencies(App.CurrentGame);
                await Util.CopyFileAsync(App.CurrentGame.loader.loaderdllpath, App.CurrentGame.loader.dataDllPath, true);
            }
            else
            {
                await GamesInstall.InstallDLL_Loader(App.CurrentGame);
                await GamesInstall.CheckAndInstallDependencies(App.CurrentGame);
                UpdateManagerStatusText(Lang.GetString("UpdateStatus.InstallLoader"));
                //now we can move the loader files to the accurate folders.
                await Util.MoveFileAsync(App.CurrentGame.loader.dataDllPath, App.CurrentGame.loader.dataDllOriginPath, false);
                await Util.CopyFileAsync(App.CurrentGame.loader.loaderdllpath, App.CurrentGame.loader.dataDllPath, false);
            }

            UpdateGameConfig(App.CurrentGame.id);
            await App.EnableOneClickInstall();

            UIHelper.EnableButton(ref SaveAndPlayButton);
            UpdateManagerStatusText(Lang.GetString("UpdateStatus.LoaderInstalled"));
            App.CurrentGame.loader.installed = true;
            UpdateBtnInstallLoader_State();
            UIHelper.EnableButton(ref btnInstallLoader);
        }

        private async Task HandleLoaderInstall()
        {
            UIHelper.DisableButton(ref btnInstallLoader);
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
                await InstallLoader();
            }

            App.CurrentGame.loader.installed = !App.CurrentGame.loader.installed;
            UIHelper.EnableButton(ref btnInstallLoader);
            UpdateBtnInstallLoader_State();
        }
        #endregion

        #endregion

        private void btnHealthCheck_Click(object sender, RoutedEventArgs e)
        {
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
                        case SetGame.SADX:
                        default:
                            icon = "SADXModManager_.png";
                            break;
                        case SetGame.SA2:
                            icon = getSA2Icon();
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
            ComboGameSelection_SelectionChanged(null, null);
        }

        private async void ComboGameSelection_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (ComboGameSelection != null && ComboGameSelection.SelectedItem != App.CurrentGame)
            {
                checkForUpdate = false;
                bool foundGame = false;
                foreach (var game in GamesInstall.GetSupportedGames())
                {
                    if (game == ComboGameSelection.SelectedItem && App.GamesList.Contains(game))
                    {
                        EnabledMods.Clear();
                        EnabledCodes.Clear();
                        App.CurrentGame = game;
                        GamesInstall.HandleGameSwap(game);
                        await Load();
                        await UpdateManagerInfo();
                        UpdateManagerIcons();
                        foundGame = true;
                        break;
                    }
                }

                if (foundGame && File.Exists(App.CurrentGame.loader?.loaderVersionpath) == false)
                {
#if !DEBUG
                    await ForceInstallLoader();
                    UpdateButtonsState();
                    if (string.IsNullOrWhiteSpace(App.CurrentGame.gameDirectory))
                        await VanillaTransition.ConvertOldProfile(App.CurrentGame.gameDirectory);
#endif
                }


                await App.EnableOneClickInstall();
                Refresh();
            }
        }
    }

}