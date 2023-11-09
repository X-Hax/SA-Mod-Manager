using SAModManager.Common;
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
        static private readonly List<string> VCPaths = new()
        {
            Path.Combine(Environment.SystemDirectory, "vcruntime140.dll"),
            Path.Combine(Environment.SystemDirectory, "msvcr120.dll"),
            Path.Combine(Environment.SystemDirectory, "msvcr110.dll")
        };

        static private readonly List<string> VCURLs = new()
        {
            "https://aka.ms/vs/17/release/vc_redist.x86.exe",
            "https://aka.ms/highdpimfc2013x86enu",
            "https://download.microsoft.com/download/1/6/B/16B06F60-3B20-4FF2-B699-5E9B7962F9AE/VSU_4/vcredist_x86.exe"
        };


        private static async Task<bool> EnableOneClickInstall()
        {
            try
            {
                string execPath = Environment.ProcessPath;
                await Process.Start(new ProcessStartInfo(execPath, "urlhandler") { UseShellExecute = true, Verb = "runas" }).WaitForExitAsync();
            }
            catch
            {
                return false;
            }

            return true;
        }


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
                if (langMsg.genericValue > 0)
                    App.ManagerSettings.Language = langMsg.genericValue;

            }

            if (!Directory.Exists(App.ConfigFolder))
                Directory.CreateDirectory(App.ConfigFolder);

            App.ManagerSettings.Serialize(App.ManagerConfigFile);
            await Task.Delay(20);
        }

        private static async Task<bool> VC_DependenciesCheck()
        {
            if (Environment.OSVersion.Platform >= PlatformID.Unix)
                return true;

            for (int i = 0; i < VCPaths.Count; i++)
            {
                if (!File.Exists(VCPaths[i]))
                {
                    var dialog = new MessageWindow(Lang.GetString("MessageWindow.Errors.VCMissing.Title"), Lang.GetString("MessageWindow.Errors.VCMissing"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning, MessageWindow.Buttons.YesNo);
                    dialog.ShowDialog();

                    if (dialog.isYes)
                    {

                        Uri uri = new(VCURLs[i] + "\r\n");
                        var DL = new DownloadDialog(uri, "Visual C++", "vc_redist.x86.exe");

                        DL.StartDL();
                        DL.ShowDialog();

                        if (DL.done)
                        {
                            // Asynchronous operation using async/await
                            await Process.Start(new ProcessStartInfo(Path.Combine(".SATemp", "vc_redist.x86.exe"), "/install /passive /norestart")
                            {
                                UseShellExecute = true,
                                Verb = "runas"
                            }).WaitForExitAsync();
                        }

                        return false;
                    }

                }
            }

            return true;
        }

        private static async Task<bool> UpdateDependenciesFolder()
        {
            try
            {
                GamesInstall.SetDependencyPath();
                Directory.CreateDirectory(App.ConfigFolder);

                if (!File.Exists(App.ManagerConfigFile)) //If config page isn't found, assume this is the first boot.
                {
                    App.isFirstBoot = true;
                    await EnableOneClickInstall();
                    await SetLanguageFirstBoot();
                    await Util.Install7Zip();
                }

            }
            catch
            {
                return false;
            }

            return true;
        }

        private static void ClearTempFolder()
        {
            try
            {
                if (Directory.Exists(".SATemp"))
                {
                    Directory.Delete(".SATemp", true);
                }
            }
            catch { }
        }

        public static async Task<bool> StartupCheck()
        {
            Console.WriteLine("Checking dependencies...");

            if (await VC_DependenciesCheck() == false)
                return false;

            await UpdateDependenciesFolder();
            ClearTempFolder();

            return true;
        }
    }
}
