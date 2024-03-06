using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace SAModManager.Configuration
{
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
