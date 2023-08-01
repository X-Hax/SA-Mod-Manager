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

namespace SAModManager.Common
{
    /// <summary>
    /// Interaction logic for InfoManagerUpdate.xaml
    /// </summary>
    public partial class InfoManagerUpdate : Window
    {
        public InfoManagerUpdate()
        {
            InitializeComponent();
        }

		private void CancelUpdate_Click(object sender, RoutedEventArgs e)
		{
			this.Close();
        }
    }
}
