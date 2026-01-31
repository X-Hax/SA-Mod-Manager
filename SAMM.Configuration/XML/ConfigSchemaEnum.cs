using System.Collections.ObjectModel;
using System.Xml.Serialization;

namespace SAMM.Configuration.XML
{
	public class ConfigSchemaEnum : NotifyObject
	{
		private string name = "EnumGroup";

		[XmlAttribute("name")]
		public string Name
		{
			get => name;
			set => RaiseAndSetIfChanged<string>(ref name, value);
		}

		private ObservableCollection<ConfigSchemaEnumMember> members = new ObservableCollection<ConfigSchemaEnumMember>();
		
		[XmlElement("EnumMember")]
		public ObservableCollection<ConfigSchemaEnumMember> Members
		{
			get => members;
			set => RaiseAndSetIfChanged<ObservableCollection<ConfigSchemaEnumMember>>(ref members, value);
		}
	}
}
