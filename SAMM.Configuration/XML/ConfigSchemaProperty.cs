using System.Reflection;
using System.Runtime.CompilerServices;
using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;

namespace SAMM.Configuration.XML
{
	public class ConfigSchemaProperty : ConfigSchemaObject
	{

		#region Config Properties
		private string type = string.Empty;

		/// <summary>
		/// The type of value the property is. If it's an enum group, the Type name is expected to match the enum group name.
		/// 
		/// Currently reserved types:
		/// - int
		/// - string
		/// - bool
		/// - float
		/// </summary>
		[XmlAttribute("type")]
		public string Type
		{
			get => type;
			set => RaiseAndSetIfChanged<string>(ref type, value);
		}

		private bool alwaysInclude = false;

		/// <summary>
		/// When true, this property will always be written to the ini file.
		/// </summary>
		[XmlAttribute("alwaysinclude")]
		public bool AlwaysInclude
		{
			get => alwaysInclude;
			set => RaiseAndSetIfChanged<bool>(ref alwaysInclude, value);
		}

		private object defaultValue = new object();

		/// <summary>
		/// The default value for this property.
		/// </summary>
		[XmlAttribute("defaultvalue")]
		public string DefaultValue
		{
			get => (defaultValue as string);
			set => RaiseAndSetIfChanged<object>(ref defaultValue, value);
		}

		private string helpText = string.Empty;

		/// <summary>
		/// The help text that will display when the property is highlighted.
		/// </summary>
		[XmlElement("HelpText")]
		public string HelpText
		{
			get => helpText;
			set => RaiseAndSetIfChanged<string>(ref helpText, value);
		}


		private object minValue = null;

		/// <summary>
		/// The minimum value for an int or float property.
		/// </summary>
		[XmlAttribute("minvalue")]
		public string MinValue
		{
			get => (minValue as string);
			set => RaiseAndSetIfChanged<object>(ref minValue, value);
		}

		public object maxValue = null;

		/// <summary>
		/// The maximum value for an int or float property.
		/// </summary>
		[XmlAttribute("maxvalue")]
		public string MaxValue
		{
			get => (maxValue as string);
			set => RaiseAndSetIfChanged<object>(ref maxValue, value);
		}

		private bool useSlider = false;

		/// <summary>
		/// Enables the UI to draw a slider instead of a number box for int or float properties.
		/// </summary>
		[XmlAttribute("slider")]
		public bool UseSlider
		{
			get => useSlider;
			set => RaiseAndSetIfChanged<bool>(ref useSlider, value);
		}

		#endregion

		private object value = new object();

		/// <summary>
		/// The stored value for this property. Not utilized in the XML.
		/// </summary>
		[XmlIgnore]
		public object Value
		{
			get => this.value;
			set
			{
				RaiseAndSetIfChanged<object>(ref this.value, value);
				IsValueDefault = CheckIfDefaultValue();
			}
		}

		[XmlIgnore]
		public List<ConfigSchemaEnumMember> EnumItems { get; private set; } = null;

		private bool isValueDefault = true;

		/// <summary>
		/// Returns true if the currently set property value is equal to the default value.
		/// </summary>
		[XmlIgnore]
		public bool IsValueDefault
		{
			get => isValueDefault;
			private set => RaiseAndSetIfChanged<bool>(ref isValueDefault, value);
		}

		public ConfigSchemaProperty() { }

		private bool CheckIfDefaultValue()
		{
			if (IsEnumType())
			{
				return GetEnumValue().Equals(DefaultValue);
			}
			else
			{
				return Value.ToString().Equals(DefaultValue);
			}
		}

		private object StringPropertyToObject(string propValue, bool isMinValue = false)
		{
			if (propValue == null)
				return new object();

			switch (Type)
			{
				case "int":
					if (!string.IsNullOrEmpty(propValue))
						return int.Parse(propValue);
					else
					{
						if (isMinValue)
							return int.MinValue;
						else
							return int.MaxValue;
					}
				case "float":
					if (!string.IsNullOrEmpty(propValue))
						return float.Parse(propValue);
					else
					{
						if (isMinValue)
							return float.MinValue;
						else
							return float.MaxValue;
					}
				case "bool":
					if (!string.IsNullOrEmpty(propValue))
						return bool.Parse(propValue);
					else
						return true;
				case "string":
					if (propValue.Length > 0)
						return propValue;
					else
						return string.Empty;
				default:
					if (EnumItems != null)
					{
						int i = EnumItems.FindIndex(x => x.Name == propValue);
						if (i == -1)
							i = 0;

						return i;
					}

					return 0;
			}
		}

		public bool IsEnumType()
		{
			switch (Type)
			{
				default:
					return true;
				case "int":
				case "float":
				case "bool":
				case "string":
					return false;
			}
		}

		public string GetEnumValue()
		{
			if (int.TryParse(Value.ToString(), out int d))
			{
				return EnumItems[d].Name;
			}
			return EnumItems[(int)Value].Name;
		}

		public object GetPropertyValue()
		{
			switch (type)
			{
				case "bool":
					return (bool)Value;
				case "string":
				default:
					return (string)Value;
				case "int":
					return (int)Value;
				case "float":
					return (float)Value;
			}
		}

		public void ResetProperty()
		{
			Value = StringPropertyToObject(DefaultValue);
		}

		public void UpdateFromConfigFile(Dictionary<string, string> config)
		{
			if (config.ContainsKey(Name))
			{
				Value = StringPropertyToObject(config[Name]);
			}
		}

		public void SetEnumItems(List<ConfigSchemaEnumMember> enumitems)
		{
			EnumItems = enumitems;
		}
	}
}