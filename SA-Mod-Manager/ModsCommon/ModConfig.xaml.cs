﻿using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using SAModManager.Configuration;
using SAModManager.Themes;

namespace SAModManager.ModsCommon
{
    /// <summary>
    /// Interaction logic for ModConfig.xaml
    /// </summary>
    public partial class ModConfig : Window
    {
        string pathXML = string.Empty;
        ConfigSettings settings;
        private static ModConfig _Instance;
        string modName = string.Empty;
        public static List<GroupBox> allGroupBoxes = new();
        public Grid modConfigSearch = new();

        public static ModConfig GetInstance()
        {
            if (_Instance == null)
                return null;

            return _Instance;
        }

        public ModConfig(string Modname, string path)
        {
            InitializeComponent();
            _Instance = this;
            modName = Modname;
            pathXML = path;
            Title = Lang.GetString("ModConfig.Title") + " " + modName;
            SearchTextBox.Text = Lang.GetString("CommonStrings.Search") + "...";
            Init();
        }

        void Init()
        {
            ItemsHost?.Children?.Clear();
            settings = new ConfigSettings(pathXML);
            var panel = FormBuilder.ConfigBuild(ref settings);
            panel.HorizontalAlignment = System.Windows.HorizontalAlignment.Stretch;
            panel.VerticalAlignment = VerticalAlignment.Stretch;
            ItemsHost.Children.Add(panel);

        }

        private async Task DelayResetBtn()
        {
            resetBtn.IsEnabled = false;
            resetBtn.Opacity = 0.3;
            await Task.Delay(100);
            resetBtn.IsEnabled = true;
            resetBtn.Opacity = 1.0;
        }

        public void OnItemHover(string des)
        {
            DescBox.Text = des;
        }

        public void OnItemLeave()
        {
            DescBox.Text = string.Empty;
        }

        private void SaveButton_Click(object sender, RoutedEventArgs e)
        {
            settings.Save();
            if (settings.modNeedRefresh)
                ((MainWindow)Application.Current.MainWindow)?.Refresh();

            this.Close();
        }

        private async void ResetButton_Click(object sender, RoutedEventArgs e)
        {
            ModConfig.allGroupBoxes.Clear();
            settings.ResetValues();
            settings.Save();

            modConfigSearch.Children.Clear();
            await DelayResetBtn();
            Init();
        }


        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            ModConfig.allGroupBoxes.Clear();

            this.Close();
        }

        private void Header_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (sender is Border header)
            {
                var groupBox = header.TemplatedParent as GroupBox;

                if (groupBox != null)
                {
                    // Toggle between Collapsed and Visible
                    if (groupBox.Tag?.ToString() == "Collapsed")
                    {
                        groupBox.Tag = "Visible";
                    }
                    else
                    {
                        groupBox.Tag = "Collapsed";
                    }

                }
            }
        }

        private bool IsSearchTextPlaceHolder()
        {
            return SearchTextBox.Text.ToLower() == Lang.GetString("CommonStrings.Search").ToLower() + "...";

        }

        private void ToggleHeader_GroupBox(object sender, bool toggle)
        {
            if (sender is GroupBox header)
            {
                if (header != null)
                {
                    // Toggle between Collapsed and Visible
                    if (toggle)
                    {
                        header.Tag = "Visible";
                    }
                    else
                    {
                        header.Tag = "Collapsed";
                    }

                }
            }
        }


        private void SearchTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {

            if (IsSearchTextPlaceHolder())
                return;

            string searchText = SearchTextBox.Text.ToLower();

            if (!Util.IsStringValid(searchText))
            {
                foreach (var control in FindAllControls(ItemsHost)) // Pass the grid that holds everything
                {
                    if (control is Label lab)
                    {
                        if (lab.Content is TextBlock && lab.Parent is Grid grid)
                        {
                            grid.Visibility = Visibility.Visible;
                        }
                    }

                    if (control is GroupBox groupBox)
                    {
                        if (groupBox.Header is TextBlock)
                        {
                            ToggleHeader_GroupBox(groupBox, true);
                        }
                    }
                }
            }


            scrollViewer?.ScrollToTop();
            SearchAndFilter(searchText);
        }

        private void SearchBox_GotFocus(object sender, RoutedEventArgs e)
        {
            if (IsSearchTextPlaceHolder())
            {
                SearchTextBox.Text = "";
            }

            SearchTextBox.Foreground = ThemeBrush.GetThemeBrush("TextBox.Brushes.Foreground");
        }

        private void SearchBox_LostFocus(object sender, RoutedEventArgs e)
        {
            // Restore placeholder text when the TextBox loses focus and is empty
            if (string.IsNullOrWhiteSpace(SearchTextBox.Text))
            {
                SearchTextBox.Foreground = ThemeBrush.GetThemeBrush("TextBox.Brushes.Placeholder");
                SearchTextBox.Text = Lang.GetString("CommonStrings.Search") + "...";
            }
        }


        private IEnumerable<FrameworkElement> FindAllControls(DependencyObject parent)
        {
            if (parent == null)
                yield break;

            for (int i = 0; i < VisualTreeHelper.GetChildrenCount(parent); i++)
            {
                var child = VisualTreeHelper.GetChild(parent, i);
                if (child is FrameworkElement element)
                {
                    yield return element;

                    // Recursively check the children of this element
                    foreach (var subChild in FindAllControls(child))
                    {
                        yield return subChild;
                    }
                }
            }
        }

        private void SearchAndFilter(string searchTerm)
        {
            foreach (var control in FindAllControls(ItemsHost)) // Pass the grid that holds everything
            {
                if (control is Label lab)
                {
                    if (lab.Content is TextBlock txt && lab.Parent is Grid grid)
                    {
                        if (txt.Text.Contains(searchTerm, StringComparison.OrdinalIgnoreCase))
                        {
                            grid.Visibility = Visibility.Visible;
                        }
                        else
                        {
                            grid.Visibility = Visibility.Collapsed;
                        }

                    }
                }
            }
        }

    }



    public class ConfigSettings
    {
        public ConfigSchema schema;
        public Dictionary<string, Dictionary<string, string>> configINI;
        private string configfilename;
        public Dictionary<string, Dictionary<string, string>> modINI;
        private string modiniFilename;
        public string modPath;
        public bool modNeedRefresh = false;

        public ConfigSettings(string path)
        {
            modPath = path;
            schema = ConfigSchema.Load(Path.Combine(path, "configschema.xml"));
            configfilename = Path.Combine(path, "config.ini");
            modiniFilename = Path.Combine(modPath, "mod.ini");
            modINI = Ini.IniFile.Load(modiniFilename);

            if (File.Exists(configfilename))
                configINI = Ini.IniFile.Load(configfilename);
            else
                configINI = new Dictionary<string, Dictionary<string, string>>();

            foreach (ConfigSchemaGroup group in schema.Groups)
            {
                if (!configINI.ContainsKey(group.Name))
                    configINI.Add(group.Name, new Dictionary<string, string>());

                foreach (ConfigSchemaProperty prop in group.Properties)
                {
                    if (!configINI[group.Name].ContainsKey(prop.Name))
                        configINI[group.Name].Add(prop.Name, prop.DefaultValue);

                    //for basic mods config without code injection
                    if (modINI is not null && prop.Name.ToLower().Contains("includedir"))
                    {
                        if (!modINI.ContainsKey("Config"))
                            modINI.Add("Config", []);

                        var value = prop.DefaultValue;
                        if (IsEnum(prop.Type) && int.TryParse(value, out int result))
                        {
                            var EnumProp = GetEnum(prop.Type).Members[result];
                            value = string.IsNullOrEmpty(EnumProp.DisplayName) ? EnumProp.Name : EnumProp.DisplayName;
                        }

                         modINI["Config"].TryAdd(prop.Name, value);
                    }
                }


            }
        }

        public void Save()
        {
            foreach (ConfigSchemaGroup group in schema.Groups)
            {
                foreach (ConfigSchemaProperty prop in group.Properties)
                {
                    if (!prop.AlwaysInclude && configINI[group.Name][prop.Name] == prop.DefaultValue)
                        configINI[group.Name].Remove(prop.Name);

                    if (configINI[group.Name].Count == 0)
                        configINI.Remove(group.Name);
                }
            }

            Ini.IniFile.Save(configINI, configfilename);
            if (modINI != null)
            {
                Ini.IniFile.Save(modINI, modiniFilename);
            }


            FileInfo fileInfo = new(configfilename);

            if (fileInfo is not null && fileInfo.Length == 0)
            {
                fileInfo.Delete();
                File.Delete(configfilename);
            }
        }

        public void ResetValues()
        {
            foreach (ConfigSchemaGroup group in schema.Groups)
                foreach (ConfigSchemaProperty prop in group.Properties)
                {
                    configINI[group.Name][prop.Name] = prop.DefaultValue;

                    if (prop.Name.ToLower().Contains("includedir"))
                    {
                        modNeedRefresh = true;
                        var value = prop.DefaultValue;
                        if (IsEnum(prop.Type) && int.TryParse(value, out int result))
                        {
                            var EnumProp = GetEnum(prop.Type).Members[result];
                            value = string.IsNullOrEmpty(EnumProp.DisplayName) ? EnumProp.Name : EnumProp.DisplayName;
                        }

                        if (IsEnum(prop.Type))
                            modINI["Config"][prop.Name] = value;
                    }

                }
        }

        public ConfigSchemaGroup GetGroup(string name)
        {
            return schema.Groups.SingleOrDefault(a => a.Name == name);
        }

        public ConfigSchemaEnum GetEnum(string name)
        {
            return schema.Enums.SingleOrDefault(a => a.Name == name);
        }


        public bool IsEnum(string name)
        {
            string s = name.ToLower();
            return s != "bool" && s != "string" && s != "int" && s != "float";
        }

        public string GetPropertyValue(string groupName, string propertyName)
        {
            return configINI[groupName][propertyName];
        }

        public void SetPropertyValue(string groupName, string propertyName, string value, string type = null)
        {
            configINI[groupName][propertyName] = value;
            if (propertyName.ToLower().Contains("includedir"))
            {
                modNeedRefresh = true;
                if (string.IsNullOrEmpty(type) == false && IsEnum(type))
                {
                    if (int.TryParse(value, out int result))
                    {
                        string enumName = GetEnum(type).Members[result].DisplayName;
                        value = string.IsNullOrEmpty(enumName) ? value : enumName;
                    }

                    modINI["Config"][propertyName] = value;
                }
            }

        }
    }

    public class CustomPropertyStore
    {
        public string groupName { get; set; }
        public string propertyName { get; set; }
        public string helpText { get; set; }
        public string type { get; set; }

        List<ConfigSchemaEnumMember> @enum;

        private ConfigSettings settings;

        public CustomPropertyStore(string groupName, string propertyName, string helpText, string type, ref ConfigSettings settings)
        {
            this.groupName = groupName;
            this.propertyName = propertyName;
            this.helpText = helpText;
            this.type = type;
            this.settings = settings;

            if (settings.IsEnum(type))
                @enum = settings.GetEnum(type).Members;
        }

        public object GetConfigValue()
        {
            var val = settings.GetPropertyValue(groupName, propertyName);

            decimal deciValue;
            string formatted;

            switch (type.ToLower())
            {
                case "bool":
                    return bool.Parse(val);
                case "int":
                    if (int.TryParse(val.Trim(), NumberStyles.Integer, CultureInfo.InvariantCulture, out int intValue))
                    {
                        return intValue;
                    }
                    else
                    {
                        // If overflow, decide whether to use max or min value
                        if (val.StartsWith("-"))
                            return int.MinValue;
                        else
                            return int.MaxValue;
                    }
                case "float":
                    deciValue = decimal.Parse(val.Trim(), CultureInfo.InvariantCulture);
                    formatted = deciValue.ToString("0.00");
                    return decimal.Parse(formatted);
                case "string":
                    return val;
                default:
                    for (int i = 0; i < @enum.Count; i++)
                        if (@enum[i].Name == val)
                            return i;

                    return 0;

            }
        }

        public int GetEnumIndex(string disp)
        {
            for (int i = 0; i < @enum.Count; i++)
                if (@enum[i].DisplayName == disp || @enum[i].Name == disp)
                    return i;
            return 0;
        }

        public void SetValue(object value)
        {
            switch (type)
            {
                case "bool":
                case "string":
                case "int":
                case "float":
                    settings.SetPropertyValue(groupName, propertyName, value.ToString());
                    break;
                default:
                    if (value is string)
                    {
                        value = GetEnumIndex((string)value);
                    }

                    settings.SetPropertyValue(groupName, propertyName, @enum[(int)value].Name, type);
                    break;
            }
        }
    }
}
