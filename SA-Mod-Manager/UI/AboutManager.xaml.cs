using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;


namespace SAModManager.UI
{

	/// <summary>
	/// Interaction logic for AboutManager.xaml
	/// </summary>
	public partial class AboutManager : Window
	{
		public AboutManager()
		{
			InitializeComponent();
			Title += " SA Mod Manager";

		}

		private void ButtonGitHub_Click(object sender, RoutedEventArgs e)
		{
			var ps = new ProcessStartInfo("https://github.com/X-Hax/sadx-mod-loader")
			{
				UseShellExecute = true,
				Verb = "open"
			};
			Process.Start(ps);
		}
	}
}
