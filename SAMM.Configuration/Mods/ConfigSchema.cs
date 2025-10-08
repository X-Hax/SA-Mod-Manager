using System.ComponentModel;
using System.Xml.Serialization;

namespace SAMM.Configuration.Mods
{
	[XmlRoot(Namespace = "http://www.sonicretro.org")]
	public class ConfigSchema
	{
		[XmlArray]
		[XmlArrayItem("Group")]
		public List<ConfigSchemaGroup> Groups { get; set; } = new List<ConfigSchemaGroup>();
		[XmlArray]
		[XmlArrayItem("Enum")]
		public List<ConfigSchemaEnum> Enums { get; set; } = new List<ConfigSchemaEnum>();

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
		public string Name { get; set; } = "Group";
		[XmlAttribute("display")]
		public string DisplayName { get; set; } = string.Empty;
		[XmlElement("Property")]
		public List<ConfigSchemaProperty> Properties { get; set; } = new List<ConfigSchemaProperty>();

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
		public string Name { get; set; } = "Property";
		[XmlAttribute("display")]
		public string DisplayName { get; set; } = string.Empty;
		[XmlAttribute("type")]
		public string Type { get; set; } = string.Empty;
		[XmlAttribute("defaultvalue")]
		public string DefaultValue { get; set; } = string.Empty;
		[XmlAttribute("alwaysinclude")]
		public bool AlwaysInclude { get; set; } = false;
		[XmlAttribute("minvalue")]
		public string MinValue { get; set; } = string.Empty;
		[XmlAttribute("maxvalue")]
		public string MaxValue { get; set; } = string.Empty;
		[XmlElement]
		public string HelpText { get; set; } = string.Empty;
		[XmlAttribute("slider")]
		public bool Slider { get; set; } = false;

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
			Slider = false;
		}
	}

	public class ConfigSchemaEnum
	{
		[XmlAttribute("name")]
		public string Name { get; set; } = "EnumGroup";
		[XmlElement("EnumMember")]
		public List<ConfigSchemaEnumMember> Members { get; set; } = new List<ConfigSchemaEnumMember>();

		public ConfigSchemaEnum()
		{
			Name = string.Empty;
			Members = new List<ConfigSchemaEnumMember>();
		}
	}

	public class ConfigSchemaEnumMember
	{
		[XmlAttribute("name")]
		public string Name { get; set; } = "Enum";
		[XmlAttribute("display")]
		public string DisplayName { get; set; } = string.Empty;

		public ConfigSchemaEnumMember()
		{
			Name = string.Empty;
			DisplayName = string.Empty;
		}
	}
}
