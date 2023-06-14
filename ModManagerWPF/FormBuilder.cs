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
using ModManagerWPF.Properties;
using Xceed.Wpf.Toolkit;

namespace ModManagerWPF
{
	public class FormBuilder
	{
		public static Thickness GroupMargin = new(0, 0, 0, 15);
		public static Thickness ElementMargin = new(10, 5, 10, 5);

		#region Mod Config Form Build
		static private ConfigSettings settings;

		public static UIElement CreateLabel(ConfigSchemaProperty property, bool addColon = true)
		{
			string content = GetElementName(property);

			if (addColon)
				content += ":";

			Label label = new()
			{
				Content = content,
				VerticalAlignment = VerticalAlignment.Center,
				Tag = property.HelpText
			};
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

		public static object GetConfigValue(ConfigSchemaProperty property, ConfigSettings config)
		{
			foreach (ConfigSchemaGroup group in config.schema.Groups)
			{
				if (!config.configINI[group.Name].ContainsKey(property.Name))
				{
					continue;
				}

				string val = config.configINI[group.Name][property.Name];
				decimal deciValue;
				string formatted;

				switch (property.Type.ToLower())
				{
					case "bool":
						return bool.Parse(val);
					case "int":
						return int.Parse(val.Trim(), CultureInfo.InvariantCulture);
					case "float":
						deciValue = decimal.Parse(val.Trim(), CultureInfo.InvariantCulture);
						formatted = deciValue.ToString("0.0");
						return decimal.Parse(formatted);
					case "string":
						return val;
					default:
						return 0;
				}

			}
			return 0;
		}

		public static UIElement CreateComboBox(ConfigSchemaProperty property, ConfigSettings config, List<ConfigSchemaEnum> enums)
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
			panel.Children.Add(CreateLabel(property));
			Dictionary<string, string> list = EnumItems(enums.Find(x => x.Name == property.Type));
			ComboBox box = new ComboBox()
			{
				Width = 200,
				SelectedValuePath = "Key",
				DisplayMemberPath = "Value",
				SelectedValue = GetConfigValue(property, config),
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


		public static UIElement CreateIntBox(ConfigSchemaProperty property, ConfigSettings config)
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
			panel.Children.Add(CreateLabel(property));

			IntegerUpDown element = new IntegerUpDown()
			{
				MinWidth = 100,
				Value = (int)GetConfigValue(property, config),
				HorizontalAlignment = HorizontalAlignment.Right,
				Tag = property.HelpText
			};
			panel.Children.Add(element);

			Grid.SetColumn(panel.Children[0], 0);
			Grid.SetColumn(element, 1);

			return panel;
		}

		public static UIElement CreateFloatBox(ConfigSchemaProperty property, ConfigSettings config)
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
			panel.Children.Add(CreateLabel(property));

			decimal result = (decimal)GetConfigValue(property, config);

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

		public static UIElement CreateStringBox(ConfigSchemaProperty property, ConfigSettings config)
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
			panel.Children.Add(CreateLabel(property));

			TextBox element = new TextBox()
			{
				Width = 200,
				Text = (string)GetConfigValue(property, config),
				VerticalAlignment = VerticalAlignment.Center,
				HorizontalAlignment = HorizontalAlignment.Right,
				Tag = property.HelpText,
			};
			panel.Children.Add(element);

			Grid.SetColumn(panel.Children[0], 0);
			Grid.SetColumn(element, 1);

			return panel;
		}

		public static UIElement CreateCheckBox(ConfigSchemaProperty property, ConfigSettings config)
		{
			Grid grid = new Grid()
			{
				ColumnDefinitions =
				{
					new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Auto) },
					new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) }
				},
				Margin = ElementMargin,
				Tag = property.HelpText
			};
			grid.Children.Add(CreateLabel(property));

			CheckBox checkBox = new CheckBox()
			{
				IsChecked = (bool)GetConfigValue(property, config),
				Tag = property.HelpText,
				HorizontalAlignment = HorizontalAlignment.Right,

			};
			grid.Children.Add(checkBox);
			grid.Children.Add(new Separator()
			{
				VerticalAlignment = VerticalAlignment.Bottom
			});

			Grid.SetColumn(grid.Children[0], 0);
			Grid.SetColumn(checkBox, 1);
			Grid.SetColumnSpan(grid.Children[2], 2);

			return grid;
		}

		private static UIElement ConfigCreateItem(ConfigSchemaProperty elem, ConfigSettings config)
		{
			switch (elem.Type.ToLower())
			{
				case "bool":
					return CreateCheckBox(elem, config);
				case "int":
					return CreateIntBox(elem, config);
				case "float":
					return CreateFloatBox(elem, config);
				case "string":
					return CreateStringBox(elem, config);
				default:
					return CreateComboBox(elem, config, config.schema.Enums);
			}
		}

		public static Panel ConfigBuild(ref ConfigSettings config)
		{
			settings = config;
			var stack = new StackPanel();
			

			foreach (ConfigSchemaGroup group in config.schema.Groups)
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

				foreach (var property in group.Properties)
				{
					var item = ConfigCreateItem(property, config);
					panel.Children.Add(item);
					panel.HorizontalAlignment = HorizontalAlignment.Stretch;
					item.MouseEnter += Item_MouseEnter;
					item.MouseLeave += Item_MouseLeave;
					Grid.SetColumn(panel.Children[0], 0);
				}

				box.Content = panel;
				stack.Children.Add(box);
			}

			return stack;
		}

		#endregion

		#region Mod Config Description
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
		#endregion





	}
}

