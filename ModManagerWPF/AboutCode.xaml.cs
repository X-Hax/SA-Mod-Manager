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
using ModManagerCommon;

namespace ModManagerWPF
{
	/// <summary>
	/// Interaction logic for AboutCode.xaml
	/// </summary>
	public partial class AboutCode : Window
	{
		public AboutCode(Code code)
		{
			DataContext = code;
			InitializeComponent();
			Title += " " + code.Name;
			TitleText.Text = code.Name;
			AuthorText.Text = code.Author != null ? "By " + code.Author : null;
			DescBx.Text = code.Description;
		}
	}
}
