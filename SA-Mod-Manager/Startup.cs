using Microsoft.Win32;
using SAModManager.Management;
using SAModManager.UI;
using SAModManager.Updater;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Data;

namespace SAModManager
{
    public class Startup
    {

        static bool IsVCRedistInstalled()
        {
            try
            {
                string regKeyPath = @"SOFTWARE\WOW6432Node\Microsoft\VisualStudio\14.0\VC\Runtimes\x86";

                using RegistryKey key = Registry.LocalMachine.OpenSubKey(regKeyPath);
                if (key != null)
                {
                    object installedValue = key.GetValue("Installed");
                    if (installedValue is int installed && installed == 1)
                    {
                        return true;
                    }
                }

                return false;
            }
            catch { }

            return true;
        }

        static private readonly string VCURLs = "https://aka.ms/vs/17/release/vc_redist.x86.exe";

        private static async Task SetLanguageFirstBoot()
        {
            ComboBox comboLanguage = new()
            {
                Name = "comboLanguageS",
                DisplayMemberPath = ".",
                SelectedIndex = 0,
                SelectedItem = 0,
                VerticalAlignment = System.Windows.VerticalAlignment.Top,
                HorizontalAlignment = System.Windows.HorizontalAlignment.Stretch,
                Margin = new(5),
            };

            Binding itemsSourceBinding = new("LangList")
            {
                Source = App.Current
            };
            BindingOperations.SetBinding(comboLanguage, ComboBox.ItemsSourceProperty, itemsSourceBinding);
            Binding selectedItemBinding = new("CurrentLang")
            {
                Source = App.Current
            };
            BindingOperations.SetBinding(comboLanguage, ComboBox.SelectedItemProperty, selectedItemBinding);

            var langMsg = new MessageWindow("Select a Language", "Please select a language to use.",
                                            type: MessageWindow.WindowType.IconCustom, button: MessageWindow.Buttons.OK,
                                            customElement: comboLanguage);
            langMsg.ShowDialog();

            if (langMsg.isOK == true)
            {
                App.SwitchLanguage();
                if (comboLanguage.SelectedIndex > 0)
                {
                    App.ManagerSettings.Language = comboLanguage.SelectedIndex;
                }
            }


            SettingsManager.SaveSettings();
            await Task.Delay(20);
        }

        private static async Task VC_DependenciesCheck()
        {

            if (!IsVCRedistInstalled())
            {
                var dialog = new MessageWindow(Lang.GetString("MessageWindow.Errors.VCMissing.Title"), Lang.GetString("MessageWindow.Errors.VCMissing"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);
                dialog.ShowDialog();

                if (dialog.isYes)
                {
                    Uri uri = new(VCURLs + "\r\n");
                    var info = new List<DownloadInfo>
                        {
                            new("Visual C++", "vc_redist.x86.exe", null, uri, DownloadDialog.DLType.Download)
                        };

                    var DL = new DownloadDialog(info);

                    DL.DownloadFailed += (ex) =>
                    {
                        DL.DisplayDownloadFailedMSG(ex, "vc_redist.x86.exe");
                    };

                    DL.StartDL();

                    if (DL.errorCount <= 0)
                    {
                        // Asynchronous operation using async/await
                        await Process.Start(new ProcessStartInfo(Path.Combine(App.tempFolder, "vc_redist.x86.exe"), "/Q /install /quiet /norestart")
                        {
                            UseShellExecute = true,
                            Verb = "runas"
                        }).WaitForExitAsync();
                    };

                    return;
                }

            }
        
        }

        private static async Task InitConfigFolder()
        {
            try
            {
                App.CreateConfigFolder();

                if (!File.Exists(App.ManagerConfigFile)) //If config page isn't found, assume this is the first boot.
                {
                    App.isFirstBoot = true;
                    await SetLanguageFirstBoot();
                }

            }
            catch { }

        }

        public static async Task StartupCheck()
        {
            Console.WriteLine("Checking dependencies...");

            await InitConfigFolder();

            if (App.isFirstBoot == true)
            {
                ASLR.CheckIfEnabled();

                if (App.isVanillaTransition == false)
                {
                    await VC_DependenciesCheck();
                }
            }

            Util.ClearTempFolder();
        }
    }
}
