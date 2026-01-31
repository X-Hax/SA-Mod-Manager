using System.Xml.Serialization;

namespace SAMM.Configuration.XML
{
	public abstract class ConfigSchemaObject : NotifyObject
	{
		private string name = "Object";

		/// <summary>
		/// The name of the ConfigSchema item. This is what will be used when writing to an INI file.
		/// </summary>
		[XmlAttribute("name")]
		public string Name
		{
			get => name;
			set => RaiseAndSetIfChanged<string>(ref name, value);
		}

		private string displayName = string.Empty;

		/// <summary>
		/// Allows overriding the <see cref="Name"/> when displaying the item to the user.
		/// </summary>
		[XmlAttribute("display")]
		public string DisplayName
		{
			get => displayName;
			set => RaiseAndSetIfChanged<string>(ref displayName, value);
		}

		public override string ToString()
		{
			if (DisplayName.Length > 0)
				return DisplayName;
			else
				return Name;
		}
	}
}
