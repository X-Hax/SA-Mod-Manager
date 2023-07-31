using ModManagerCommon;
using System;
using System.Collections.Generic;
using System.Diagnostics;
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
using static SAModManager.MainWindow;

namespace SAModManager
{
	/// <summary>
	/// Interaction logic for AboutMod.xaml
	/// </summary>
	public partial class AboutMod : Window
	{
		public AboutMod(ModData mod)
		{
			DataContext = mod;
			InitializeComponent();
			Title += " " + mod.Name;
			TitleText.Text = mod.Name + " | v" + mod.Version;
			AuthorText.Text = mod.Author != null ? Lang.GetString("CommonStrings.Author") + ": " + mod.Author : null;
			CategoryText.Text = mod.Category != null ? Lang.GetString("CommonStrings.Category") + ": " + mod.Category + "\n" : null;
			DescBx.Text = mod.Description;
			btnAuthSite.Visibility = string.IsNullOrWhiteSpace(mod.AuthorURL) ? Visibility.Hidden : Visibility.Visible;
			btnModSource.Visibility = string.IsNullOrWhiteSpace(mod.SourceCode) ? Visibility.Hidden : Visibility.Visible;

			if (btnAuthSite.Visibility == Visibility.Hidden && btnModSource.Visibility == Visibility.Hidden)
			{
				btnAuthSite.IsEnabled = false;
				btnModSource.IsEnabled = false;
				MaxHeight -= 20;
				Height -= 20;
			}
		}

		private void btnAuthSite_Click(object sender, RoutedEventArgs e)
		{
			var mod = (ModData)DataContext;

			if (mod is null || mod is not ModData)
				return;

			try
			{
				var ps = new ProcessStartInfo(mod.AuthorURL)
				{
					UseShellExecute = true,
					Verb = "open"
				};
				Process.Start(ps);
			}
			catch { }
		}

		private void btnModSource_Click(object sender, RoutedEventArgs e)
		{
			var mod = (ModData)DataContext;

			if (mod is null || mod is not ModData)
				return;

			try
			{
				var ps = new ProcessStartInfo(mod.SourceCode)
				{
					UseShellExecute = true,
					Verb = "open"
				};
				Process.Start(ps);
			}
			catch
			{}

		}
	}
}
