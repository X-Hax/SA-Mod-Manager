using SAModManager.Common;
using System.Windows;


namespace SAModManager
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
			AuthorText.Text = code.Author != null ? Lang.GetString("CommonStrings.Author") + " " + code.Author : null;
			DescBx.Text = code.Description;
		}
	}
}
