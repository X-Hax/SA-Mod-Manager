using System.IO;
using System.Windows;

namespace SAModManager.Updater
{
    /// <summary>
    /// Interaction logic for InfoManagerUpdate.xaml
    /// </summary>
    public partial class InfoManagerUpdate : Window
    {
        public InfoManagerUpdate(string changelog, string version, string loader = null)
        {
            InitializeComponent();
            UpdateInfoText.Text = changelog;

            if (!string.IsNullOrEmpty(loader)) //if loader update
            {
                Title = string.Format(Lang.GetString("InfoLoaderUpdate.Title"), loader);
                Header.Text = Title + " ("  + version + ")";
                Header.FontStyle = FontStyles.Italic;
            }
            else //if Manager update
            {
                Header.Text = Title + " v" + version;
                Header.FontWeight = FontWeights.Bold;
            }
        }

        private void CancelUpdate_Click(object sender, RoutedEventArgs e)
		{
			this.Close();
        }

        private void DownloadUpdate_Click(object sender, RoutedEventArgs e)
        {
            bool retry = false;

            do
            {
                try
                {

                    Util.CreateSafeDirectory(App.tempFolder);
                }
                catch {}
            } while (retry == true);

            DialogResult = true;
        }
    }
}
