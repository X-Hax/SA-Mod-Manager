using Avalonia.Collections;
using ReactiveUI;
using SAMM.Configuration.Mods;
using SAMM.Utilities.INI;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using IniConfig = System.Collections.Generic.Dictionary<string, System.Collections.Generic.Dictionary<string, string>>;

namespace SAMM.App.Models
{
	/// <summary>
	/// <see cref="ReactiveObject"/> implementation of a <see cref="ConfigSchemaProperty"/>.
	/// </summary>
	public class ModConfigPropertyModel : ReactiveObject
	{
		private string name = "Property";
		public string Name
		{
			get => name;
			set => this.RaiseAndSetIfChanged(ref name, value);
		}

		public string displayName = string.Empty;
		public string DisplayName
		{
			get => displayName;
			set => this.RaiseAndSetIfChanged(ref displayName, value);
		}

		private string type = "string";
		public string Type
		{
			get => type;
			set => this.RaiseAndSetIfChanged(ref type, value);
		}

		private object val = new object();
		public object Value
		{
			get => val;
			set
			{
				IsValueDefault = (value == DefaultValue);
				this.RaiseAndSetIfChanged(ref val, value);
			}
		}

		private object defaultValue = new object();
		public object DefaultValue
		{
			get => defaultValue;
			set => this.RaiseAndSetIfChanged(ref defaultValue, value);
		}

		private bool alwaysInclude = false;
		public bool AlwaysInclude
		{
			get => alwaysInclude;
			set => this.RaiseAndSetIfChanged(ref alwaysInclude, value);
		}

		private object minValue = new object();
		public object MinimumValue
		{
			get => minValue;
			set => this.RaiseAndSetIfChanged(ref minValue, value);
		}

		private object maxValue = new object();
		public object MaximumValue
		{
			get => maxValue;
			set => this.RaiseAndSetIfChanged(ref maxValue, value);
		}

		private string helperText = string.Empty;
		public string HelperText
		{
			get => helperText;
			set => this.RaiseAndSetIfChanged(ref helperText, value);
		}

		private bool useSlider = false;
		public bool UseSlider
		{
			get => useSlider;
			set => this.RaiseAndSetIfChanged(ref useSlider, value);
		}

		private bool isValueDefault = true;
		public bool IsValueDefault 
		{
			get => isValueDefault;
			set => this.RaiseAndSetIfChanged(ref isValueDefault, value);
		}

		public List<ModConfigEnumMemberModel> Items { get; private set; } = null;

		public override string ToString()
		{
			if (DisplayName.Length > 0)
				return DisplayName;
			else
				return Name;
		}

		public ModConfigPropertyModel() { }

		public ModConfigPropertyModel(ConfigSchemaProperty prop, AvaloniaDictionary<string, ObservableCollection<ModConfigEnumMemberModel>> items = null)
		{
			Name = prop.Name;
			DisplayName = prop.DisplayName;
			Type = prop.Type;
			AlwaysInclude = prop.AlwaysInclude;
			HelperText = prop.HelpText;
			UseSlider = prop.Slider;

			if (items.ContainsKey(Type))
				Items = items[Type].ToList();

			DefaultValue = StringPropertyToObject(prop.DefaultValue);
			MinimumValue = StringPropertyToObject(prop.MinValue, true);
			MaximumValue = StringPropertyToObject(prop.MaxValue, false);

			Value = DefaultValue;
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
						return null;
				case "string":
					if (propValue.Length > 0)
						return propValue;
					else
						return null;
				default:
					if (Items != null)
					{
						int i = Items.FindIndex(x => x.Name == propValue);
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
			return Items[(int)Value].Name;
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

		public void UpdateFromConfigFile(Dictionary<string, string> config)
		{
			if (config.ContainsKey(Name))
			{
				Value = StringPropertyToObject(config[Name]);
			}
		}
	}

	/// <summary>
	/// <see cref="ReactiveObject"/> implementation of a <see cref="ConfigSchemaGroup"/>.
	/// </summary>
	public class ModConfigGroupModel : ReactiveObject
	{
		private string name = "Group";
		public string Name
		{
			get => name;
			set => this.RaiseAndSetIfChanged(ref name, value);
		}

		private string displayName = string.Empty;
		public string DisplayName
		{
			get => displayName;
			set => this.RaiseAndSetIfChanged(ref displayName, value);
		}

		public override string ToString()
		{
			if (DisplayName.Length > 0)
				return DisplayName;
			else
				return Name;
		}

		private ObservableCollection<ModConfigPropertyModel> properties = new ObservableCollection<ModConfigPropertyModel>();
		public ObservableCollection<ModConfigPropertyModel> Properties
		{
			get => properties;
			set => this.RaiseAndSetIfChanged(ref properties, value);
		}

		public ModConfigGroupModel() { }

		public ModConfigGroupModel(ConfigSchemaGroup group, AvaloniaDictionary<string, ObservableCollection<ModConfigEnumMemberModel>> items = null)
		{
			Name = group.Name;
			DisplayName = group.DisplayName;
			foreach (var property in group.Properties)
			{
				Properties.Add(new ModConfigPropertyModel(property, items));
			}
		}
	}

	/// <summary>
	/// <see cref="ReactiveObject"/> implementation of a <see cref="ConfigSchemaEnumMember"/>.
	/// </summary>
	public class ModConfigEnumMemberModel : ReactiveObject
	{
		private string name = "Enum";
		public string Name
		{
			get => name;
			set => this.RaiseAndSetIfChanged(ref name, value);
		}

		private string displayName = string.Empty;
		public string DisplayName
		{
			get => displayName;
			set => this.RaiseAndSetIfChanged(ref displayName, value);
		}

		public override string ToString()
		{
			if (DisplayName.Length > 0)
				return DisplayName;
			else 
				return Name;
		}

		public ModConfigEnumMemberModel() { }

		public ModConfigEnumMemberModel(ConfigSchemaEnumMember enumMember)
		{
			Name = enumMember.Name;
			DisplayName = enumMember.DisplayName;
		}
	}

	/// <summary>
	/// <see cref="ReactiveObject"/> implementation of a <see cref="ConfigSchema"/>.
	/// </summary>
	public class ModConfigModel : ReactiveObject
	{
		private ObservableCollection<ModConfigGroupModel> groups = new ObservableCollection<ModConfigGroupModel>();
		public ObservableCollection<ModConfigGroupModel> Groups
		{
			get => groups;
			set => this.RaiseAndSetIfChanged(ref groups, value);
		}

		public AvaloniaDictionary<string, ObservableCollection<ModConfigEnumMemberModel>> enums = new AvaloniaDictionary<string, ObservableCollection<ModConfigEnumMemberModel>>();
		public AvaloniaDictionary<string, ObservableCollection<ModConfigEnumMemberModel>> Enums
		{
			get => enums;
			set => this.RaiseAndSetIfChanged(ref enums, value);
		}

		public ModConfigModel() { }

		public ModConfigModel(ConfigSchema configSchema)
		{
			foreach (var group in configSchema.Enums)
			{
				ObservableCollection<ModConfigEnumMemberModel> conv = new ObservableCollection<ModConfigEnumMemberModel>();
				foreach (var member in group.Members)
				{
					conv.Add(new ModConfigEnumMemberModel(member));
				}

				Enums.Add(group.Name, conv);
			}

			foreach (var group in configSchema.Groups)
			{
				Groups.Add(new ModConfigGroupModel(group, Enums));
			}
		}

		public void WriteConfigFile(string filePath)
		{
			IniConfig config = new IniConfig();
			foreach (var group in Groups)
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
			foreach (var group in Groups)
			{
				foreach (var prop in group.Properties)
				{
					prop.Value = prop.DefaultValue;
				}
			}
		}
	}
}
