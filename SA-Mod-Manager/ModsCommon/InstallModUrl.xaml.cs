using SAModManager.UI;
using SAModManager.Updater;
using System;
using System.Collections.Generic;
using System.Diagnostics.Eventing.Reader;
using System.IO;
using System.Net.Http;
using System.Security.Policy;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using static System.Runtime.InteropServices.JavaScript.JSType;


namespace SAModManager
{
    public static class DownloadModUrls
    {
        private static bool IsValidUrl(string url)
        {
            if (Util.IsStringValid(url))
            {
                // Check if the string can be converted into a valid URI
                return Uri.TryCreate(url, UriKind.Absolute, out var uriResult)
                       && (uriResult.Scheme == Uri.UriSchemeHttp || uriResult.Scheme == Uri.UriSchemeHttps);
            }

            return false;
        }

        private static bool IsOneClick(string url)
        {
            if (Util.IsStringValid(url))
            {
                // Check if the string can be converted into a valid URI
                return url.Contains(App.CurrentGame.oneClickName);
            }

            return false;
        }

        public static async Task Download(string[] lines)
        {
            List<Uri> Urilist = [];
            List<Uri> UriOneClicklist = [];
            string updatePath;
  
            foreach (string line in lines) //add each link the user added in an Uri list
            {
                if (IsOneClick(line))
                {
                    string oneClickName = App.CurrentGame.oneClickName + ":";
                    string[] split = line[oneClickName.Length..].Split(',');
                    Urilist.Add(new Uri(split[0]));
                }
                else //if direct link
                {
                    if (IsValidUrl(line))
                    {
                        Urilist.Add(new Uri(line));
                    }
                }
            }


            bool retry = false;
            App.CancelUpdate = false;
            updatePath = Path.GetFullPath(Path.Combine(App.CurrentGame.modDirectory, ".updates"));
            Logger.Log("Downloading mods from Install Mod Url feature...");
            Logger.Log("Current Mod Install Path: " + updatePath);

            do
            {
                try
                {
                    Util.CreateSafeDirectory(updatePath);
                }
                catch (Exception ex)
                {
                    var s = Lang.GetString("MessageWindow.Errors.DirectoryCreateFail0") + ":\n" + ex.Message + "\n\n" + Lang.GetString("MessageWindow.Errors.DirectoryCreateFail1");
                    var msg = new MessageWindow(Lang.GetString("MessageWindow.Errors.DirectoryCreateFail.Title"), s, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.RetryCancel);
                    msg.ShowDialog();
                    retry = msg.isRetry;

                }
            } while (retry == true);

            var updates = new List<ModDownload>();
            var httpClient = UpdateHelper.HttpClient;

            //we will try to get the name of the mod from the Uri then add it to the download list
            foreach (var uri in Urilist)
            {
                HttpResponseMessage response = await httpClient.GetAsync(uri);
                response.EnsureSuccessStatusCode();

                string ext = Path.GetExtension(uri.AbsolutePath);
                string name = Path.GetFileName(uri.AbsolutePath);
                var dummyInfo = new ModInfo //try to get name from URL
                {
                    Name = Util.IsStringValid(ext) ? Path.GetFileNameWithoutExtension(uri.AbsolutePath) : "mod" + name,
                    Author = string.Empty
                };

                //if we didn't get the name from the URL take it from Uri's absolute path (might not always be accurate)
                if (!Util.IsStringValid(ext) && response.RequestMessage != null && response.RequestMessage.RequestUri != null)
                    dummyInfo.Name = Path.GetFileNameWithoutExtension(response.RequestMessage.RequestUri.AbsolutePath);

                string dummyPath = dummyInfo.Name;

                foreach (char c in Path.GetInvalidFileNameChars())
                {
                    dummyPath = dummyPath.Replace(c, '_');
                }

                dummyPath = Path.Combine(App.CurrentGame.modDirectory, dummyPath);

                updates.Add(new ModDownload(dummyInfo, dummyPath, uri.AbsoluteUri, null, 0));
            }

            var modDL = new ModDownloadDialog(updates, updatePath, false);
            modDL.StartDL();

            await Task.Delay(100);

            if (Directory.Exists(updatePath))
            {
                try
                {
                    Directory.Delete(updatePath, true);
                }
                catch
                { }
            }

            Logger.Log("Finished dowloading mod. " + updatePath);
            ((MainWindow)App.Current.MainWindow).Refresh();
        }
    }
    /// <summary>
    /// Interaction logic for NewModOptions.xaml
    /// </summary>
    public partial class InstallModUrl : Window
    {

        public InstallModUrl()
        {
            InitializeComponent();
            Header.Text = Lang.GetString("InstallModUrl.Radio.Header");
            Title = Lang.GetString("InstallModOptions.Radio.Url");
            cancelBtn.IsCancel = true;
        }


        private void Window_ContentRendered(object sender, EventArgs e)
        {
            InvalidateVisual(); //fix render
            UrlTextBlock.Focus();
        }

        private void ButtonCancel_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void UrlTextBlock_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (sender is TextBox txt)
            {
                txt.TextChanged -= UrlTextBlock_TextChanged;

                string text = txt.Text;
                int cursorPosition = txt.SelectionStart;
                txt.Text = txt.Text.Insert(cursorPosition, Environment.NewLine);
                txt.SelectionStart = cursorPosition + Environment.NewLine.Length;
                txt.TextChanged += UrlTextBlock_TextChanged;
            }
        }

        private async void Button_OkClick(object sender, RoutedEventArgs e)
        {
            okBtn.IsEnabled = false;
            cancelBtn.IsEnabled = false;
            string text = UrlTextBlock.Text;
            string[] lines = text.Split([Environment.NewLine], StringSplitOptions.RemoveEmptyEntries);

            await DownloadModUrls.Download(lines);
            Close();
        }
    }
}
