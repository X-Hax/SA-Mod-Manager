using System.Collections.ObjectModel;
using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;
using SAMM.Utilities.INI;
using IniConfig = System.Collections.Generic.Dictionary<string, System.Collections.Generic.Dictionary<string, string>>;

namespace SAMM.Configuration.XML
{
	/// <summary>
	/// ConfigSchema class used for user created configuration menus within the SA Mod Manager.
	/// </summary>
	[XmlRoot(Namespace = "http://www.sonicretro.org")]
	public class ConfigSchema : NotifyObject
	{
		private ObservableCollection<ConfigSchemaGroup> configPropertyGroups = new ObservableCollection<ConfigSchemaGroup>();

		/// <summary>
		/// Property Groups within a ConfigSchema
		/// </summary>
		[XmlArray("Groups")]
		[XmlArrayItem("Group")]
		public ObservableCollection<ConfigSchemaGroup> ConfigPropertyGroups
		{
			get => configPropertyGroups;
			set => RaiseAndSetIfChanged<ObservableCollection<ConfigSchemaGroup>>(ref configPropertyGroups, value);
		}
		
		private ObservableCollection<ConfigSchemaEnum> configEnumGroups = new ObservableCollection<ConfigSchemaEnum>();

		/// <summary>
		/// Enumerations within a ConfigSchema
		/// </summary>
		[XmlArray("Enums")]
		[XmlArrayItem("Enum")]
		public ObservableCollection<ConfigSchemaEnum> ConfigEnumGroups
		{
			get => configEnumGroups;
			set => RaiseAndSetIfChanged<ObservableCollection<ConfigSchemaEnum>>(ref configEnumGroups, value);
		}

		/// <summary>
		/// Loads a ConfigSchema formatted xml file. If the deserializer returns null for any reason, this will return a new ConfigSchema.
		/// </summary>
		/// <param name="filename"></param>
		/// <returns></returns>
		public static ConfigSchema Load(string filename)
		{
			ConfigSchema schema;

			using (FileStream fs = File.OpenRead(filename))
			{
				XmlSerializer xs = new XmlSerializer(typeof(ConfigSchema));
				schema = (ConfigSchema)xs.Deserialize(fs);
			}

			if (schema == null)
				schema = new ConfigSchema();

			schema.UpdateProperties();

			return schema;
		}

		public ConfigSchema() { }

		/// <summary>
		/// This is required to be run after the Xml has been deserialized to properly link properties with their respective enumerator.
		/// </summary>
		public void UpdateProperties()
		{
			foreach (var propgroup in ConfigPropertyGroups)
			{
				foreach (var prop in propgroup.Properties)
				{
					if (prop.IsEnumType())
					{
						List<ConfigSchemaEnum> enumitems = configEnumGroups.ToList();
						ConfigSchemaEnum enumSchema = enumitems.Find(x => x.Name == prop.Type);

						if (enumSchema != null)
						{
							List<ConfigSchemaEnumMember> enumMembers = enumSchema.Members.ToList();
							prop.SetEnumItems(enumMembers);
						}
					}

					prop.ResetProperty();
				}
			}
		}

		public void WriteConfigFile(string filePath)
		{
			IniConfig config = new IniConfig();
			foreach (var group in ConfigPropertyGroups)
			{
				bool hasProps = false;
				Dictionary<string, string> configProps = new Dictionary<string, string>();
				foreach (var prop in group.Properties)
				{
					if (prop.AlwaysInclude || (!prop.IsValueDefault))
					{
						string writeProp = prop.Value.ToString();
						if (prop.IsEnumType())
							writeProp = prop.GetEnumValue();

						configProps.Add(prop.Name, writeProp);
						hasProps = true;
					}
				}

				if (hasProps)
					config.Add(group.Name, configProps);
			}

			IniFile.Save(config, filePath);
		}

		public void RevertOptions()
		{
			foreach (var group in ConfigPropertyGroups)
			{
				foreach (var prop in group.Properties)
				{
					prop.ResetProperty();
				}
			}
		}
	}
}
