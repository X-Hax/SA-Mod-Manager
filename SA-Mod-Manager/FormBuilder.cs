using System;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Controls;
using SAModManager.Controls;
using System.Windows.Media;
using SAModManager.ModsCommon;
using SAModManager.Configuration;

namespace SAModManager
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

            TextBlock textBlock = new()
            {
                Text = content,
                TextTrimming = TextTrimming.CharacterEllipsis,
                TextWrapping = TextWrapping.NoWrap,
                Padding = new Thickness(2),
                MaxWidth = 240,
                VerticalAlignment = VerticalAlignment.Center
            };

            Label label = new()
            {
                Content = textBlock,
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
                Width = 190,
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

        public static UIElement CreateNumericBox(ConfigSchemaProperty property, CustomPropertyStore storeInfo)
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

            Decimal numVal = 0;
            if (!Decimal.TryParse(storeInfo.GetConfigValue().ToString(), out numVal))
                numVal = 0;
            Decimal numMax = Decimal.MaxValue;
            if (property.MaxValue.ToString() != "")
                if (!Decimal.TryParse(property.MaxValue.ToString(), out numMax))
                    numMax = Decimal.MaxValue;
            Decimal numMin = Decimal.MinValue;
            if (property.MinValue.ToString() != "")
                if (!Decimal.TryParse(property.MinValue.ToString(), out numMin))
                    numMin = Decimal.MinValue;

            NumberBox element = new()
            {
                MinWidth = 100,
                Height = 22,
                Value = numVal,
                HorizontalAlignment = HorizontalAlignment.Right,
                MinValue = numMin,
                MaxValue = numMax,
                Tag = storeInfo
            };

            switch (property.Type)
            {
                case "float":
                    element.Type = NumberBox.ValueType.Float;
                    element.DecimalCount = 2;
                    break;
                default:
                case "int":
                    element.Type = NumberBox.ValueType.Integer;
                    element.DecimalCount = 0;
                    break;
            }

            element.ValueChanged += ModSetting_NumericElementChanged;
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
                case "float":
                    return CreateNumericBox(elem, storeInfo);
                case "string":
                    return CreateStringBox(elem, storeInfo);
                default:
                    return CreateComboBox(elem, config.schema.Enums, storeInfo);
            }
        }

        //XAML does not support some specific symbols
        private static string CleanupGroupName(string name)
        {
            // Step 1: Remove disallowed symbols
            string noSymbols = Regex.Replace(name, @"[=\\-]", "");

            // Step 2: Remove whitespace
            string noWhitespaceString = Regex.Replace(noSymbols, @"\s", "");

            // Step 3: If numbers are found, put them at the end to avoid crash ie: "46Group" become "Group46"
            Match numberMatch = Regex.Match(noWhitespaceString, @"^(\d+)(.*)$");
            if (numberMatch.Success)
            {
                string numberPart = numberMatch.Groups[1].Value;
                string restOfString = numberMatch.Groups[2].Value;

                if (string.IsNullOrEmpty(restOfString)) //if the string is empty it means the group only had number originally
                {
                    restOfString = "_"; //add an extra "_" to prevent crash 
                }

                noWhitespaceString = restOfString + numberPart;
            }

            return noWhitespaceString;
        }


        public static Panel ConfigBuild(ref ConfigSettings config)
        {
            settings = config;
            var stack = new StackPanel();

            foreach (ConfigSchemaGroup group in settings.schema.Groups)
            {
                string name = CleanupGroupName(group.Name);
                string HeaderName = string.IsNullOrWhiteSpace(group.DisplayName) ? name : group.DisplayName;
                var box = new GroupBox() {
                    Name = name, 
                    Margin = GroupMargin,
                };

                var groupBoxHeader = box.Header as string;

                TextBlock headerTex = new()
                {
                    Text = HeaderName,
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
                ModConfig.allGroupBoxes.Add(box);   
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
                settings.SetPropertyValue(info.groupName, info.propertyName, chk.IsChecked.Value.ToString());
        }

        private static void ModSetting_NumericElementChanged(object sender, RoutedEventArgs e)
        {
            NumberBox box = (NumberBox)sender;

            var info = box.Tag as CustomPropertyStore;
            if (info != null)
                settings.SetPropertyValue(info.groupName, info.propertyName, box.Value.ToString());
        }

        private static void ModSetting_stringElementChanged(object sender, RoutedEventArgs e)
        {
            TextBox text = (TextBox)sender;

            var info = text.Tag as CustomPropertyStore;
            if (info != null)
                settings.SetPropertyValue(info.groupName, info.propertyName, text.Text);
        }

        #endregion
    }
}