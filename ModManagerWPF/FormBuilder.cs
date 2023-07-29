using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.IO.Packaging;
using System.Linq;
using System.Runtime.InteropServices.ComTypes;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Xml.Linq;
using GongSolutions.Wpf.DragDrop.Utilities;
using ModManagerWPF.Common;
using ModManagerWPF.Properties;
using ModManagerWPF.Elements;
using Xceed.Wpf.Toolkit;
using Xceed.Wpf.Toolkit.PropertyGrid.Editors;
using System.Windows.Media;
using System.Windows.Data;

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
				string value = string.IsNullOrEmpty(member.DisplayName) ? member.Name : member.DisplayName;
				members.Add(key, value);
			}

			return members;
		}


		public static UIElement CreateComboBox(ConfigSchemaProperty property, List<ConfigSchemaEnum> enums, CustomPropertyStore storeInfo)
		{
			Grid panel = new()
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

			Border backing = new Border()
			{
				Background = new SolidColorBrush(Color.FromArgb(0, 0, 0, 0)),
				HorizontalAlignment = HorizontalAlignment.Stretch,
				VerticalAlignment = VerticalAlignment.Stretch,
				Margin = new Thickness(0)
			};
			panel.Children.Add(backing);

			Dictionary<string, string> list = EnumItems(enums.Find(x => x.Name == property.Type));
			ComboBox box = new()
			{
				Width = 200,
				SelectedValuePath = "Key",
				DisplayMemberPath = "Value",
				SelectedValue = storeInfo.GetConfigValue(),
				SelectedIndex = (int)storeInfo.GetConfigValue(),
				ItemsSource = list,
				VerticalAlignment = VerticalAlignment.Center,
				HorizontalAlignment = HorizontalAlignment.Right,
				Tag = storeInfo
			};

			box.SelectionChanged += ComboBox_ModConfigSelectionChanged;
			panel.Children.Add(box);

			Grid.SetColumn(panel.Children[0], 0);
			Grid.SetColumn(backing, 0);
			Grid.SetColumnSpan(backing, 2);
			Grid.SetColumn(box, 1);

			panel.Children.Add(new Separator()
			{
				Margin = new Thickness(0, 2, 0, 0),
				VerticalAlignment = VerticalAlignment.Bottom
			});
			Grid.SetColumnSpan(panel.Children[3], 2);
			return panel;
		}

		public static UIElement CreateNumericBox(ConfigSchemaProperty property, CustomPropertyStore storeInfo, NumericUpDown.DataType dataType)
		{
			Grid panel = new()
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

			Border backing = new Border()
			{
				Background = new SolidColorBrush(Color.FromArgb(0, 0, 0, 0)),
				HorizontalAlignment = HorizontalAlignment.Stretch,
				VerticalAlignment = VerticalAlignment.Stretch,
				Margin = new Thickness(0)
			};
			panel.Children.Add(backing);

			NumericUpDown element = new()
			{
				MinWidth = 100,
				Height = 22,
				ValueType = dataType,
				Value = double.Parse(storeInfo.GetConfigValue().ToString()),
				HorizontalAlignment = HorizontalAlignment.Right,
				//MinValue = (double)storeInfo.GetConfigMinValue(),
				//MaxValue = (double)storeInfo.GetConfigMaxValue(),
			};

			//element.ValueChanged += ModSetting_intElementChanged;
			panel.Children.Add(element);

			Grid.SetColumn(panel.Children[0], 0);
			Grid.SetColumn(backing, 0);
			Grid.SetColumnSpan(backing, 2);
			Grid.SetColumn(element, 1);

			panel.Children.Add(new Separator()
			{
				Margin = new Thickness(0, 2, 0, 0),
				VerticalAlignment = VerticalAlignment.Bottom
			});
			Grid.SetColumnSpan(panel.Children[3], 2);

			return panel;
		}

		public static UIElement CreateStringBox(ConfigSchemaProperty property, CustomPropertyStore storeInfo)
		{
			Grid panel = new()
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

			Border backing = new Border()
			{
				Background = new SolidColorBrush(Color.FromArgb(0, 0, 0, 0)),
				HorizontalAlignment = HorizontalAlignment.Stretch,
				VerticalAlignment = VerticalAlignment.Stretch,
				Margin = new Thickness(0)
			};
			panel.Children.Add(backing);

			TextBox element = new()
			{
				Width = 200,
				Text = (string)storeInfo.GetConfigValue(),
				VerticalAlignment = VerticalAlignment.Center,
				HorizontalAlignment = HorizontalAlignment.Right,
				Tag = storeInfo,
			};

			element.TextChanged += ModSetting_stringElementChanged;
			panel.Children.Add(element);

			Grid.SetColumn(panel.Children[0], 0);
			Grid.SetColumn(backing, 0);
			Grid.SetColumnSpan(backing, 2);
			Grid.SetColumn(element, 1);

			panel.Children.Add(new Separator()
			{
				Margin = new Thickness(0, 2, 0, 0),
				VerticalAlignment = VerticalAlignment.Bottom
			});
			Grid.SetColumnSpan(panel.Children[3], 2);

			return panel;
		}

		public static UIElement CreateCheckBox(ConfigSchemaProperty property, CustomPropertyStore storeInfo)
		{
			Grid panel = new()
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

			Border backing = new Border()
			{
				Background = new SolidColorBrush(Color.FromArgb(0, 0, 0, 0)),
				HorizontalAlignment = HorizontalAlignment.Stretch,
				VerticalAlignment = VerticalAlignment.Stretch,
				Margin = new Thickness(0)
			};
			panel.Children.Add(backing);

			CheckBox checkBox = new()
			{
				IsChecked = (bool)storeInfo.GetConfigValue(),
				Tag = storeInfo,
				HorizontalAlignment = HorizontalAlignment.Right,
			
			};

			checkBox.Checked += CheckBox_ModConfigSelectionChanged;
			checkBox.Unchecked += CheckBox_ModConfigSelectionChanged;
			panel.Children.Add(checkBox);


			Grid.SetColumn(panel.Children[0], 0);
			Grid.SetColumn(backing, 0);
			Grid.SetColumnSpan(backing, 2);
			Grid.SetColumn(checkBox, 1);

			panel.Children.Add(new Separator()
			{
				Margin = new Thickness(0, 2, 0, 0),
				VerticalAlignment = VerticalAlignment.Bottom
			});
			Grid.SetColumnSpan(panel.Children[3], 2);

			return panel;
		}

		private static UIElement ConfigCreateItem(ConfigSchemaProperty elem, ConfigSettings config, CustomPropertyStore storeInfo)
		{
			switch (elem.Type.ToLower())
			{
				case "bool":
					return CreateCheckBox(elem, storeInfo);
				case "int":
					return CreateNumericBox(elem, storeInfo, NumericUpDown.DataType.Integer);
				case "float":
					return CreateNumericBox(elem, storeInfo, NumericUpDown.DataType.Float);
				case "string":
					return CreateStringBox(elem, storeInfo);
				default:
					return CreateComboBox(elem, config.schema.Enums, storeInfo);
			}
		}

		public static Panel ConfigBuild(ref ConfigSettings config)
		{
			settings = config;
			var stack = new StackPanel();
			
			foreach (ConfigSchemaGroup group in settings.schema.Groups)
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
					var settingInfo = new CustomPropertyStore(group.Name, property.Name, property.HelpText, property.Type, ref settings);
					var item = ConfigCreateItem(property, settings, settingInfo);
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

		#region Mod Config Save

		private static void ComboBox_ModConfigSelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			ComboBox comboBox = (ComboBox)sender;
			// Handle the selection change here
			var info = comboBox.Tag as CustomPropertyStore;
			if (info != null)
			{
				var txt = comboBox.SelectedValue;

				info.SetValue(txt.ToString());
			}
		}

		private static void CheckBox_ModConfigSelectionChanged(object sender, RoutedEventArgs e)
		{
			CheckBox chk = (CheckBox)sender;
			var info = chk.Tag as CustomPropertyStore;
			if (info != null) 
			{
				settings.SetPropertyValue(info.groupName, info.propertyName, chk.IsChecked.Value.ToString());
			}
		}

		private static void ModSetting_intElementChanged(object sender, RoutedEventArgs e)
		{
			IntegerUpDown number = (IntegerUpDown)sender;

			var info = number.Tag as CustomPropertyStore;
			if (info != null)
			{
				settings.SetPropertyValue(info.groupName, info.propertyName, number.Value.ToString());
			}
		}

		private static void ModSetting_floatElementChanged(object sender, RoutedEventArgs e)
		{
			DecimalUpDown number = (DecimalUpDown)sender;

			var info = number.Tag as CustomPropertyStore;
			if (info != null)
			{
				settings.SetPropertyValue(info.groupName, info.propertyName, number.Value.ToString());
			}
		}

		private static void ModSetting_stringElementChanged(object sender, RoutedEventArgs e)
		{
			TextBox text = (TextBox)sender;

			var info = text.Tag as CustomPropertyStore;
			if (info != null)
			{
				settings.SetPropertyValue(info.groupName, info.propertyName, text.Text);
			}
		}

		#endregion
	}
}

