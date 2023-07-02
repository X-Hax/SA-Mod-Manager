using ModManagerCommon;
using System;
using System.Collections.Generic;
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

namespace ModManagerWPF.Common
{
	/// <summary>
	/// Interaction logic for ModChangelog.xaml
	/// </summary>
	public partial class ModChangelog : Window
    {
		public List<ModDownload> ModUpdates = new List<ModDownload>();

		private void AdjustDetailsDisplay(ModDownload mod)
		{
			bool IsSelfHosted = string.IsNullOrEmpty(mod.Info.UpdateUrl);

			if (IsSelfHosted)
			{
				UpdateDetails.RowDefinitions[2].Height = new GridLength(30);
				UpdateDetails.RowDefinitions[3].Height = new GridLength(0);
				UpdateDetails.RowDefinitions[4].Height = new GridLength(0);
			}
			else
			{
				UpdateDetails.RowDefinitions[2].Height = new GridLength(0);
				UpdateDetails.RowDefinitions[3].Height = new GridLength(30);
				UpdateDetails.RowDefinitions[4].Height = new GridLength(30);
			}
		}

		public ModChangelog()
        {
			InitializeComponent();
        }
    }
}
