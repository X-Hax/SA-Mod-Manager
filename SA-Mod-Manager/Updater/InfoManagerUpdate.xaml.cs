using System;

using System.IO;
using System.Windows;

namespace SAModManager.Common
{
    /// <summary>
    /// Interaction logic for InfoManagerUpdate.xaml
    /// </summary>
    public partial class InfoManagerUpdate : Window
    {
        public InfoManagerUpdate(string changelog, string loader = null)
        {
            InitializeComponent();
            UpdateInfoText.Text = changelog;

            if (!string.IsNullOrEmpty(loader))
                Title = "New " + loader + " Update(s)";
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
                    if (!Directory.Exists(App.tempFolder))
                    {
                        Directory.CreateDirectory(App.tempFolder);
                    }
                }
                catch {}
            } while (retry == true);

            DialogResult = true;
        }
    }
}
