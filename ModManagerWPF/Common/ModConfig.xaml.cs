using ModManagerWPF.Properties;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Windows;

using System.Xml.Serialization;



namespace ModManagerWPF.Common
{
	/// <summary>
	/// Interaction logic for ModConfig.xaml
	/// </summary>
	public partial class ModConfig : Window
	{
		string pathXML = string.Empty;
		ConfigSettings settings;
		private static ModConfig _Instance;

		public static ModConfig GetInstance()
		{
			if (_Instance == null)
				return null;

			return _Instance;
		}

		public ModConfig(SADXModInfo Mod, string path)
		{
			InitializeComponent();
			_Instance = this;
			if (Mod is null)
			{
				Close();
			}

			pathXML = path;
			Title = Lang.GetString("TitleConfigureMod") + " " + Mod.Name;
			settings = new ConfigSettings(pathXML);

			var panel = FormBuilder.ConfigBuild(ref settings);
			panel.HorizontalAlignment = System.Windows.HorizontalAlignment.Stretch;
			panel.VerticalAlignment = VerticalAlignment.Stretch;
			ItemsHost.Children.Add(panel);		
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

		private void ResetButton_Click(object sender, RoutedEventArgs e)
		{
			settings.ResetValues();
		}

		private void CancelButton_Click(object sender, RoutedEventArgs e)
		{
			this.Close();
		}


	}

	public class ConfigSettings : ICustomTypeDescriptor
	{
		public ConfigSchema schema;
		public Dictionary<string, Dictionary<string, string>> configINI;
		private string configfilename;

		public ConfigSettings(string path)
		{
			schema = ConfigSchema.Load(Path.Combine(path, "configschema.xml"));
			configfilename = Path.Combine(path, "config.ini");

			if (File.Exists(configfilename))
				configINI = IniFile.IniFile.Load(configfilename);
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

			if (configINI.Count > 0) //don't save an empty config.ini file
				IniFile.IniFile.Save(configINI, configfilename);
		}

		public void ResetValues()
		{
			foreach (ConfigSchemaGroup group in schema.Groups)
				foreach (ConfigSchemaProperty prop in group.Properties)
					configINI[group.Name][prop.Name] = prop.DefaultValue;
		}

		AttributeCollection ICustomTypeDescriptor.GetAttributes() { return AttributeCollection.Empty; }

		string ICustomTypeDescriptor.GetClassName() { return TypeDescriptor.GetClassName(this, true); }

		string ICustomTypeDescriptor.GetComponentName() { return TypeDescriptor.GetComponentName(this, true); }

		TypeConverter ICustomTypeDescriptor.GetConverter() { return null; }

		EventDescriptor ICustomTypeDescriptor.GetDefaultEvent() { return null; }

		PropertyDescriptor ICustomTypeDescriptor.GetDefaultProperty() { return TypeDescriptor.GetDefaultProperty(this, true); }

		object ICustomTypeDescriptor.GetEditor(Type editorBaseType) { return TypeDescriptor.GetEditor(this, editorBaseType, true); }

		EventDescriptorCollection ICustomTypeDescriptor.GetEvents() { return EventDescriptorCollection.Empty; }

		EventDescriptorCollection ICustomTypeDescriptor.GetEvents(Attribute[] attributes) { return EventDescriptorCollection.Empty; }

		PropertyDescriptorCollection ICustomTypeDescriptor.GetProperties() { return ((ICustomTypeDescriptor)this).GetProperties(new Attribute[0]); }

		PropertyDescriptorCollection ICustomTypeDescriptor.GetProperties(Attribute[] attributes)
		{
			List<PropertyDescriptor> props = new List<PropertyDescriptor>();
			foreach (ConfigSchemaGroup group in schema.Groups)
				foreach (ConfigSchemaProperty prop in group.Properties)
					props.Add(new CustomPropertyDescriptor(this, group.Name, prop.Name));
			return new PropertyDescriptorCollection(props.ToArray(), true);
		}

		object ICustomTypeDescriptor.GetPropertyOwner(PropertyDescriptor pd) { return this; }

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

		public void SetPropertyValue(string groupName, string propertyName, string value, bool enum_ = false)
		{
			configINI[groupName][propertyName] = value;
		}


		public bool ShowCategories { get { return schema.Groups.Count > 1 || schema.Groups[0].Name.Length != 0; } }
	}

	public class CustomPropertyStore
	{
		public string groupName { get; set; }
		public string propertyName { get; set; }
		public string helpText { get; set; }
		public string type { get; set; }

		List<ConfigSchemaEnumMember> @enum;

		public CustomPropertyStore(string groupName, string propertyName, string helpText, string type)
		{
			this.groupName = groupName;
			this.propertyName = propertyName;
			this.helpText = helpText;
			this.type = type;
		}

		public int GetEnumIndex(string disp)
		{
			for (int i = 0; i < @enum.Count; i++)
				if (@enum[i].DisplayName == disp || @enum[i].Name == disp)
					return i;
			return 0;
		}

	}

	class CustomPropertyDescriptor : PropertyDescriptor
	{
		ConfigSettings settings;
		string groupName;
		string propertyName;
		ConfigSchemaProperty prop;
		string category = null;
		long iminval, imaxval;
		double fminval, fmaxval;
		List<ConfigSchemaEnumMember> @enum;

		public CustomPropertyDescriptor(ConfigSettings settings, string groupName, string propertyName) :
			base(propertyName, new Attribute[0])
		{
			this.settings = settings;
			this.groupName = groupName;
			this.propertyName = propertyName;
			ConfigSchemaGroup group = settings.GetGroup(groupName);
			prop = group.GetProperty(propertyName);
			if (groupName.Length != 0)
				category = group.DisplayName ?? groupName;
			switch (prop.Type)
			{
				case "bool":
					break;
				case "int":
					if (prop.MinValue == null)
						iminval = int.MinValue;
					else
						iminval = long.Parse(prop.MinValue, NumberFormatInfo.InvariantInfo);
					if (prop.MaxValue == null)
						imaxval = uint.MaxValue;
					else
						imaxval = long.Parse(prop.MaxValue, NumberFormatInfo.InvariantInfo);
					break;
				case "float":
					if (prop.MinValue == null)
						fminval = double.MinValue;
					else
						fminval = double.Parse(prop.MinValue, NumberFormatInfo.InvariantInfo);
					if (prop.MaxValue == null)
						fmaxval = double.MaxValue;
					else
						fmaxval = double.Parse(prop.MaxValue, NumberFormatInfo.InvariantInfo);
					break;
				case "string":
					break;
				default:
					@enum = settings.GetEnum(prop.Type).Members;
					break;
			}
		}

		public override Type ComponentType { get { return typeof(ConfigSettings); } }

		public override bool IsReadOnly { get { return (Attributes.Matches(ReadOnlyAttribute.Yes)); } }

		public override Type PropertyType
		{
			get
			{
				switch (prop.Type)
				{
					case "bool":
						return typeof(bool);
					case "int":
						return typeof(long);
					case "float":
						return typeof(double);
					case "string":
						return typeof(string);
					default:
						return typeof(int);
				}
			}
		}

		public override bool CanResetValue(object component)
		{
			if (prop.DefaultValue == null)
				return false;
			else
				return !settings.GetPropertyValue(groupName, propertyName).Equals(prop.DefaultValue);
		}

		public override object GetValue(object component)
		{
			string val = settings.GetPropertyValue(groupName, propertyName);
			switch (prop.Type)
			{
				case "bool":
					return bool.Parse(val);
				case "int":
					return long.Parse(val, NumberFormatInfo.InvariantInfo);
				case "float":
					return float.Parse(val, NumberFormatInfo.InvariantInfo);
				case "string":
					return val;
				default:
					for (int i = 0; i < @enum.Count; i++)
						if (@enum[i].Name == val)
							return i;
					return 0;
			}
		}

		public override void ResetValue(object component)
		{
			settings.SetPropertyValue(groupName, propertyName, prop.DefaultValue);
		}

		public override void SetValue(object component, object value)
		{
			switch (prop.Type)
			{
				case "bool":
				case "string":
					settings.SetPropertyValue(groupName, propertyName, value.ToString());
					break;
				case "int":
					settings.SetPropertyValue(groupName, propertyName, Math.Max(iminval, Math.Min(imaxval, (long)value)).ToString(NumberFormatInfo.InvariantInfo));
					break;
				case "float":
					settings.SetPropertyValue(groupName, propertyName, Math.Max(fminval, Math.Min(fmaxval, (double)value)).ToString(NumberFormatInfo.InvariantInfo));
					break;
				default:
					if (value is string)
						value = GetEnumIndex((string)value);
					settings.SetPropertyValue(groupName, propertyName, @enum[(int)value].Name);
					break;
			}
		}

		public override bool ShouldSerializeValue(object component)
		{
			object val = settings.GetPropertyValue(groupName, propertyName);

			if (prop.DefaultValue == null && val == null)
				return false;
			else
				return !val.Equals(prop.DefaultValue);
		}

		public override TypeConverter Converter
		{
			get
			{
				switch (prop.Type)
				{
					case "bool":
					case "int":
					case "float":
					case "string":
						return base.Converter;
					default:
						return new EnumConverter();
				}
			}
		}

		public override string DisplayName { get { return prop.DisplayName ?? prop.Name; } }

		public override string Category { get { return category; } }

		public override string Description { get { return prop.HelpText; } }

		public List<string> GetEnumValues()
		{
			return new List<string>(@enum.Select(a => a.DisplayName ?? a.Name));
		}

		public string GetEnumDisplay(int i)
		{
			return @enum[i].DisplayName ?? @enum[i].Name;
		}

		public int GetEnumIndex(string disp)
		{
			for (int i = 0; i < @enum.Count; i++)
				if (@enum[i].DisplayName == disp || @enum[i].Name == disp)
					return i;
			return 0;
		}
	}

	public class EnumConverter : TypeConverter
	{
		public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
		{
			if (sourceType == typeof(string))
				return true;
			return base.CanConvertFrom(context, sourceType);
		}

		public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
		{
			if (destinationType == typeof(int))
				return true;
			return base.CanConvertTo(context, destinationType);
		}

		public override object ConvertFrom(ITypeDescriptorContext context, CultureInfo culture, object value)
		{
			if (value is string)
				return ((CustomPropertyDescriptor)context.PropertyDescriptor).GetEnumIndex((string)value);
			return base.ConvertFrom(context, culture, value);
		}

		public override object ConvertTo(ITypeDescriptorContext context, CultureInfo culture, object value, Type destinationType)
		{
			if (destinationType == typeof(string) && value is int)
				return ((CustomPropertyDescriptor)context.PropertyDescriptor).GetEnumDisplay((int)value);
			else if (destinationType == typeof(int) && value is string)
				return ((CustomPropertyDescriptor)context.PropertyDescriptor).GetEnumIndex((string)value);
			return base.ConvertTo(context, culture, value, destinationType);
		}

		public override StandardValuesCollection GetStandardValues(ITypeDescriptorContext context)
		{
			return new StandardValuesCollection(((CustomPropertyDescriptor)context.PropertyDescriptor).GetEnumValues());
		}

		public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
		{
			return true;
		}
	}

	[XmlRoot(Namespace = "http://www.sonicretro.org")]
	public class ConfigSchema
	{
		[XmlArray]
		[XmlArrayItem("Group")]
		public List<ConfigSchemaGroup> Groups { get; set; }
		[XmlArray]
		[XmlArrayItem("Enum")]
		public List<ConfigSchemaEnum> Enums { get; set; }

		private static XmlSerializer xs = new XmlSerializer(typeof(ConfigSchema));
		public static ConfigSchema Load(string filename)
		{
			using (FileStream fs = File.OpenRead(filename))
				return (ConfigSchema)xs.Deserialize(fs);
		}

		public ConfigSchema() 
		{
			Groups = new List<ConfigSchemaGroup>();
			Enums = new List<ConfigSchemaEnum>();
		}
	}

	public class ConfigSchemaGroup
	{
		[XmlAttribute("name")]
		public string Name { get; set; }
		[XmlAttribute("display")]
		public string DisplayName { get; set; }
		[XmlElement("Property")]
		public List<ConfigSchemaProperty> Properties { get; set; }

		public ConfigSchemaGroup()
		{
			Name = string.Empty;
			DisplayName = string.Empty;
		}

		public ConfigSchemaProperty GetProperty(string name)
		{
			return Properties.Single(a => a.Name == name);
		}
	}

	public class ConfigSchemaProperty
	{
		[XmlAttribute("name")]
		public string Name { get; set; }
		[XmlAttribute("display")]
		public string DisplayName { get; set; }
		[XmlAttribute("type")]
		public string Type { get; set; }
		[XmlAttribute("defaultvalue")]
		public string DefaultValue { get; set; }
		[XmlAttribute("alwaysinclude")]
		public bool AlwaysInclude { get; set; }
		[XmlAttribute("minvalue")]
		public string MinValue { get; set; }
		[XmlAttribute("maxvalue")]
		public string MaxValue { get; set; }
		[XmlElement]
		public string HelpText { get; set; }

		public ConfigSchemaProperty()
		{
			Name = string.Empty;
			DisplayName = string.Empty;
			Type = string.Empty;
			DefaultValue = string.Empty;
			AlwaysInclude = false;
			MinValue = string.Empty;
			MaxValue = string.Empty;
			HelpText = string.Empty;
		}
	}

	public class ConfigSchemaEnum
	{
		[XmlAttribute("name")]
		public string Name { get; set; }
		[XmlElement("EnumMember")]
		public List<ConfigSchemaEnumMember> Members { get; set; }

		public ConfigSchemaEnum()
		{
			Name = string.Empty;
			Members = new List<ConfigSchemaEnumMember>();
		}
	}

	public class ConfigSchemaEnumMember
	{
		[XmlAttribute("name")]
		public string Name { get; set; }
		[XmlAttribute("display")]
		public string DisplayName { get; set; }

		public ConfigSchemaEnumMember()
		{
			Name = string.Empty;
			DisplayName = string.Empty;
		}
	}
}
