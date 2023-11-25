using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace SAModManager.Updater
{
    /// <summary>
    /// Interaction logic for OfflineInstall.xaml
    /// </summary>
    public partial class OfflineInstall : Window
    {
        public OfflineInstall(string file)
        {
            InitializeComponent();
            FailedText.Text = string.Format(Lang.GetString("Updater.DL.Offline.Failed"), file);
        }

        public void CheckSuccess(bool success)
        {
            InstallProgress.Value = 100;

            if (success)
            {
                StatusText.Text = Lang.GetString("CommonStrings.Success");
            }
            else
            {
                StatusText.Text = Lang.GetString("CommonStrings.Failed");
            }
        }
    }
}
