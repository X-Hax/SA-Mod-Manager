using SAModManager.Properties;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;
using System.Xml.Serialization;
using SAModManager.Configuration;

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
		public bool reset = false;

		public static ModConfig GetInstance()
		{
			if (_Instance == null)
				return null;

			return _Instance;
		}

		public ModConfig(string Modname, string path, bool reset = false)
		{
			InitializeComponent();
			_Instance = this;

			modName = Modname;

			this.reset = reset;
			pathXML = path;
			Title = Lang.GetString("ModConfig.Title") + " " + modName;
			settings = new ConfigSettings(pathXML);
			DelayResetBtn(reset);
			var panel = FormBuilder.ConfigBuild(ref settings);
			panel.HorizontalAlignment = System.Windows.HorizontalAlignment.Stretch;
			panel.VerticalAlignment = VerticalAlignment.Stretch;
			ItemsHost.Children.Add(panel);
		}

		private async void DelayResetBtn(bool reset)
		{
			if (!reset)
				return;

			resetBtn.IsEnabled = false;
			resetBtn.Opacity = 0.3;
			await Task.Delay(1050);
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
			this.Close();
		}

		private async void ResetButton_Click(object sender, RoutedEventArgs e)
		{
			settings.ResetValues();
			await Task.Delay(10);
			settings.Save();
			await Task.Delay(30);

            var config = new ModConfig(modName, pathXML, true);
            this.Close();
			config.ShowDialog();
        }

		private void CancelButton_Click(object sender, RoutedEventArgs e)
		{
			this.Close();
		}


	}

	public class ConfigSettings
	{
		public ConfigSchema schema;
		public Dictionary<string, Dictionary<string, string>> configINI;
		private string configfilename;

		public ConfigSettings(string path)
		{
			schema = ConfigSchema.Load(Path.Combine(path, "configschema.xml"));
			configfilename = Path.Combine(path, "config.ini");

			if (File.Exists(configfilename))
				configINI = Ini.IniFile.Load(configfilename);
			else
				configINI = new Dictionary<string, Dictionary<string, string>>();

			foreach (ConfigSchemaGroup group in schema.Groups)
			{
				if (!configINI.ContainsKey(group.Name))
					configINI.Add(group.Name, new Dictionary<string, string>());
				foreach (ConfigSchemaProperty prop in group.Properties)
					if (!configINI[group.Name].ContainsKey(prop.Name))
						configINI[group.Name].Add(prop.Name, prop.DefaultValue);
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
				}
				if (configINI[group.Name].Count == 0)
					configINI.Remove(group.Name);
			}

			Ini.IniFile.Save(configINI, configfilename);

            FileInfo fileInfo = new(configfilename);

            if (fileInfo is not null)
			{
                if (fileInfo.Length == 0)
				{
                    fileInfo.Delete();
                    File.Delete(configfilename);
                }
            }
        }

		public void ResetValues()
		{
			foreach (ConfigSchemaGroup group in schema.Groups)
				foreach (ConfigSchemaProperty prop in group.Properties)
					configINI[group.Name][prop.Name] = prop.DefaultValue;
		}

		public ConfigSchemaGroup GetGroup(string name)
		{
			return schema.Groups.SingleOrDefault(a => a.Name == name);
		}

		public ConfigSchemaEnum GetEnum(string name)
		{
			return schema.Enums.SingleOrDefault(a => a.Name == name);
		}

		public string GetPropertyValue(string groupName, string propertyName)
		{
			return configINI[groupName][propertyName];
		}

		public void SetPropertyValue(string groupName, string propertyName, string value)
		{
			configINI[groupName][propertyName] = value;
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

			switch (type)
			{
				case "string":
				case "bool":
				case "int":
				case "float":
					break;
				default:
					@enum = settings.GetEnum(type).Members;
					break;
			}
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
					return int.Parse(val.Trim(), CultureInfo.InvariantCulture);
				case "float":
					deciValue = decimal.Parse(val.Trim(), CultureInfo.InvariantCulture);
					formatted = deciValue.ToString("0.0");
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

					settings.SetPropertyValue(groupName, propertyName, @enum[(int)value].Name);
					break;
			}
		}
	}
}
