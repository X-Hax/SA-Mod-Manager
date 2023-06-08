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

		public static UIElement CreateComboBox(string text)
		{
			ComboBox box = new();
			box.Text = text;
			return box;
		}

		public static UIElement CreateCheckBox(string text)
		{
			CheckBox checkBox = new CheckBox();
			checkBox.Content = text;
			return checkBox;
		}

		private static UIElement ConfigCreateItem(ConfigSchemaProperty elem)
		{
			switch (elem.Type)
			{
				case "bool":
					return CreateCheckBox(null);
				default:
					return CreateComboBox(elem.DisplayName);
			}
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
					string PropertyName = string.IsNullOrWhiteSpace(element.DisplayName) ? element.Name : element.DisplayName;
					var label = CreateLabel(PropertyName);	
					panel.Children.Add(label);
					var item = ConfigCreateItem(element);
					panel.Children.Add(item);
					panel.HorizontalAlignment= HorizontalAlignment.Left;
				}

				box.Content = panel;
				stack.Children.Add(box);
			}

			return stack;
		}
	}
}

