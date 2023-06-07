using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using ModManagerWPF.Common;

namespace ModManagerWPF
{
    public class FormBuilder
    {
		public static Thickness GroupMargin = new(0, 0, 0, 15);
		public static Thickness BorderThick = new(0.3, 0.3, 0.3, 0.3);

		public static UIElement CreateLabel(string text)
		{
			Label label = new();
			label.Content = text;
			return label;
		}

		public static Panel ConfigBuild(ConfigSchema config)
		{
			var stack = new StackPanel();

			foreach (ConfigSchemaGroup group in config.Groups)
			{
				var box = new GroupBox() { Name = group.Name, Header = group.DisplayName, Margin = GroupMargin, BorderThickness = BorderThick };
				var panel = new StackPanel();

				box.Content = panel;
		
				foreach (var element in group.Properties)
				{
					var item = CreateLabel(element.DisplayName);
					panel.Children.Add(item);
				}

				stack.Children.Add(box);
			}

			return stack;
		}
	}
}

