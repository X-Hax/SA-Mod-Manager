using System.Collections.ObjectModel;
using System.Xml.Serialization;

namespace SAMM.Configuration.XML
{
	public class ConfigSchemaGroup : ConfigSchemaObject
	{
		private ObservableCollection<ConfigSchemaProperty> properties = new ObservableCollection<ConfigSchemaProperty>();

		/// <summary>
		/// List of associated properties with this group.
		/// </summary>
		[XmlElement("Property")]
		public ObservableCollection<ConfigSchemaProperty> Properties
		{
			get => properties;
			set => RaiseAndSetIfChanged<ObservableCollection<ConfigSchemaProperty>>(ref properties, value);
		}

		private bool defaultCollapsed = false;

		[XmlAttribute("defaultclosed")]
		public bool DefaultCollapsed
		{
			get => defaultCollapsed;
			set => RaiseAndSetIfChanged<bool>(ref defaultCollapsed, value);
		}

		public ConfigSchemaGroup()
		{
			Name = "Group";
		}

		public ConfigSchemaProperty GetProperty(string name)
		{
			return Properties.Single(a => a.Name == name);
		}
	}

}
