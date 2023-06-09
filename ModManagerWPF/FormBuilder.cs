using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.ComTypes;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Xml.Linq;
using ModManagerWPF.Common;
using Xceed.Wpf.Toolkit;

namespace ModManagerWPF
{
	public class FormBuilder
	{
		public static Thickness GroupMargin = new(0, 0, 0, 15);
		public static Thickness ElementMargin = new(10, 5, 10, 5);

		public static UIElement CreateLabel(string text)
		{
			Label label = new();
			label.Content = text;
			return label;
		}

		public static string GetElementName(ConfigSchemaProperty element)
		{
			return string.IsNullOrWhiteSpace(element.DisplayName) ? element.Name : element.DisplayName;
		}

		public static Dictionary<string, string> EnumItems(ConfigSchemaEnum config)
		{
			Dictionary<string, string> members = new Dictionary<string, string>();

			foreach (ConfigSchemaEnumMember member in config.Members)
			{
				string key = member.Name;
				string value = member.DisplayName ?? member.Name;
				members.Add(key, value);
			}

			return members;
		}

		public static UIElement CreateComboBox(ConfigSchemaProperty property, List<ConfigSchemaEnum> enums)
		{
			Grid panel = new Grid()
			{
				ColumnDefinitions =
				{
					new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) },
					new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) }
				},
				Margin = ElementMargin,
				Tag = property.HelpText
			};
			panel.Children.Add(new Label()
			{
				Content = GetElementName(property) + ":",
				VerticalAlignment = VerticalAlignment.Center,
				Tag = property.HelpText
			});
			Dictionary<string, string> list = EnumItems(enums.Find(x => x.Name == property.Type));
			ComboBox box = new ComboBox()
			{
				Width = 200,
				SelectedValuePath = "Key",
				DisplayMemberPath = "Value",
				SelectedValue = property.DefaultValue,
				ItemsSource = list,
				VerticalAlignment = VerticalAlignment.Center,
				HorizontalAlignment = HorizontalAlignment.Right,
				Tag = property.HelpText
			};
			panel.Children.Add(box);

			Grid.SetColumn(panel.Children[0], 0);
			Grid.SetColumn(box, 1);
			return panel;
		}

		public static UIElement CreateIntBox(ConfigSchemaProperty property)
		{
			Grid panel = new Grid()
			{
				ColumnDefinitions =
				{
					new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) },
					new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) }
				},
				Margin = ElementMargin,
				Tag = property.HelpText
			};
			panel.Children.Add(new Label()
			{
				Content = GetElementName(property) + ":",
				VerticalAlignment = VerticalAlignment.Center,
				Tag = property.HelpText
			});

			IntegerUpDown element = new IntegerUpDown()
			{
				MinWidth = 100,
				Value = int.Parse(property.DefaultValue),
				HorizontalAlignment = HorizontalAlignment.Right,
				Tag = property.HelpText
			};
			panel.Children.Add(element);

			Grid.SetColumn(panel.Children[0], 0);
			Grid.SetColumn(element, 1);

			return panel;
		}

		public static UIElement CreateFloatBox(ConfigSchemaProperty property)
		{
			Grid panel = new Grid()
			{
				ColumnDefinitions =
				{
					new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) },
					new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) }
				},
				Margin = ElementMargin,
				Tag = property.HelpText
			};
			panel.Children.Add(new Label()
			{
				Content = GetElementName(property) + ":",
				VerticalAlignment = VerticalAlignment.Center,
				Tag = property.HelpText
			});

			decimal result = decimal.Parse(property.DefaultValue.Trim(), CultureInfo.InvariantCulture);

			DecimalUpDown element = new DecimalUpDown()
			{
				MinWidth = 100,
				Value = result,
				HorizontalAlignment = HorizontalAlignment.Right,
				Tag = property.HelpText
			};
			panel.Children.Add(element);

			Grid.SetColumn(panel.Children[0], 0);
			Grid.SetColumn(element, 1);

			return panel;
		}

		public static UIElement CreateStringBox(ConfigSchemaProperty property)
		{
			Grid panel = new Grid()
			{
				ColumnDefinitions =
				{
					new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) },
					new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) }
				},
				Margin = ElementMargin,
				Tag = property.HelpText
			};
			panel.Children.Add(new Label()
			{
				Content = GetElementName(property) + ":",
				VerticalAlignment = VerticalAlignment.Center,
				Tag = property.HelpText
			});

			TextBox element = new TextBox()
			{
				Width = 200,
				Text = property.DefaultValue,
				VerticalAlignment = VerticalAlignment.Center,
				HorizontalAlignment = HorizontalAlignment.Right,
				Tag = property.HelpText,
			};
			panel.Children.Add(element);

			Grid.SetColumn(panel.Children[0], 0);
			Grid.SetColumn(element, 1);

			return panel;
		}

		public static UIElement CreateCheckBox(ConfigSchemaProperty property)
		{
			string isTrue = property.DefaultValue;
			CheckBox checkBox = new CheckBox()
			{
				Content = GetElementName(property),
				Margin = ElementMargin,
				IsChecked = isTrue.Replace(" ", "").ToLower() == "true",
				Tag = property.HelpText
			};

			return checkBox;
		}

		private static UIElement ConfigCreateItem(ConfigSchemaProperty elem, List<ConfigSchemaEnum> enums)
		{
			switch (elem.Type.ToLower())
			{
				case "bool":
					return CreateCheckBox(elem);
				case "int":
					return CreateIntBox(elem);
				case "float":
					return CreateFloatBox(elem);
				case "string":
					return CreateStringBox(elem);
				default:
					return CreateComboBox(elem, enums);
			}
		}

		public static Panel ConfigBuild(ConfigSchema config)
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
					FontWeight = FontWeights.Bold,
				};

				box.Header = headerTex;
				var panel = new StackPanel();

				foreach (var element in group.Properties)
				{
					var item = ConfigCreateItem(element, config.Enums);
					panel.Children.Add(item);
					panel.HorizontalAlignment = HorizontalAlignment.Stretch;
					item.MouseEnter += Item_MouseEnter;
					item.MouseLeave += Item_MouseLeave;
				}

				box.Content = panel;
				stack.Children.Add(box);
			}

			return stack;
		}

		private static void Item_MouseLeave(object sender, System.Windows.Input.MouseEventArgs e)
		{
			var instance = ModConfig.GetInstance();

			instance?.OnItemLeave();
		}

		private static void Item_MouseEnter(object sender, System.Windows.Input.MouseEventArgs e)
		{
			if (sender is FrameworkElement send)
			{
				var instance = ModConfig.GetInstance();

				if (instance is not null && send.Tag is not null)
				{
					var s = send.Tag.ToString();
					instance.OnItemHover(s);
				}
			}
		}
	}


}

