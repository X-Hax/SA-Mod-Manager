using ICSharpCode.AvalonEdit.Highlighting;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Xml;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using ICSharpCode.AvalonEdit.Highlighting.Xshd;

namespace ModManagerWPF.Common
{
    /// <summary>
    /// Interaction logic for NewCode.xaml
    /// </summary>
    public partial class NewCode : Window
    {
        public NewCode()
        {
            InitializeComponent();
			/*
			using (Stream s = File.OpenRead(""))
			{
				using (XmlTextReader reader = new XmlTextReader(s))
				{
					CodeWriter.SyntaxHighlighting = HighlightingLoader.Load(reader, HighlightingManager.Instance);
				}
			}
			*/
		}

		private bool SaveCodeToFile()
		{
			return true;
		}

		#region Window Functions
		private void CancelButton_Click(object sender, RoutedEventArgs e)
		{
			this.Close();
		}

		private void SaveButton_Click(object sender, RoutedEventArgs e)
		{
			if (SaveCodeToFile())
				this.Close();
		}

		private void CheckSaveToMod_Checked(object sender, RoutedEventArgs e)
		{
			if ((bool)CheckSaveToMod.IsChecked)
				CodeCategory.IsEnabled = false;
			else
				CodeCategory.IsEnabled = true;
		}
		#endregion
	}
}
