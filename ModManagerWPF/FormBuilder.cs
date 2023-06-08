using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Xml.Linq;
using ModManagerWPF.Common;

namespace ModManagerWPF
{
    public class FormBuilder
    {
		public static Thickness GroupMargin = new(0, 0, 0, 15);

		public static UIElement CreateLabel(string text)
		{
			Label label = new();
			label.Content = text;
			return label;
		}


		public static Panel ConfigBuild(ConfigSchema config, Action<string> descripHover = null)
		{
			var stack = new StackPanel();

			foreach (ConfigSchemaGroup group in config.Groups)
			{
				string name = group.Name;
				string HeaderName = string.IsNullOrWhiteSpace(group.DisplayName) ? group.Name : group.DisplayName;
				var box = new GroupBox() { Name = name.Replace(" ", ""), Header = HeaderName, Margin = GroupMargin };
				var groupBoxHeader = box.Header as string;
		
				TextBlock headerTex = new()
				{
					Text = groupBoxHeader,
					FontSize = 14,
					FontWeight= FontWeights.Bold,
				};

				box.Header = headerTex;
				var panel = new StackPanel();

				foreach (var element in group.Properties)
				{
					var grid = new Grid();
				
					string PropertyName = string.IsNullOrWhiteSpace(element.DisplayName) ? element.Name : element.DisplayName;
					var item = CreateLabel(PropertyName);
					grid.Children.Add(item);
					panel.Children.Add(grid);
				}

				box.Content = panel;
				stack.Children.Add(box);
			}

			return stack;
		}
	}
}

