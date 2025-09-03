using Avalonia;
using Avalonia.Controls;
using Avalonia.Markup.Xaml;
using Avalonia.Styling;

namespace SAMM.App.Styles
{
	public class SAMMStyles : Avalonia.Styling.Styles
	{
		public SAMMStyles()
		{
			AvaloniaXamlLoader.Load(this);
		}
	}
}
