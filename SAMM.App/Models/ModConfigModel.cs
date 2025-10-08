using Avalonia.Collections;
using ReactiveUI;
using SAMM.Configuration.Mods;
using SAMM.Utilities.INI;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Runtime.CompilerServices;
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
			set => this.RaiseAndSetIfChanged(ref val, value);
		}

		private string defaultValue = string.Empty;
		public string DefaultValue
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

		private string minValue = string.Empty;
		public string MinimumValue
		{
			get => minValue;
			set => this.RaiseAndSetIfChanged(ref minValue, value);
		}

		private string maxValue = string.Empty;
		public string MaximumValue
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

		public bool IsValueDefault
		{
			get => (Value.ToString().ToLowerInvariant() == DefaultValue.ToLowerInvariant());
		}

		public ModConfigPropertyModel() { }

		public ModConfigPropertyModel(ConfigSchemaProperty prop)
		{
			Name = prop.Name;
			DisplayName = prop.DisplayName;
			Type = prop.Type;
			DefaultValue = prop.DefaultValue;
			AlwaysInclude = prop.AlwaysInclude;
			MinimumValue = prop.MinValue;
			MaximumValue = prop.MaxValue;
			HelperText = prop.HelpText;
			UseSlider = prop.Slider;
			Value = GetPropertyDefaultValue();
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

		public object GetPropertyDefaultValue()
		{
			switch (type)
			{
				case "bool":
					return bool.Parse(DefaultValue);
				case "string":
				default:
					return (string)DefaultValue;
				case "int":
					return int.Parse(DefaultValue);
				case "float":
					return float.Parse(DefaultValue);
			}
		}

		private object GetConfigValue(string input)
		{
			switch (type)
			{
				case "bool":
					return bool.Parse(input);
				case "string":
				default:
					return (string)input;
				case "int":
					return int.Parse(input);
				case "float":
					return float.Parse(input);
			}
		}

		public void UpdateFromConfigFile(Dictionary<string, string> config)
		{
			if (config.ContainsKey(Name))
			{
				Value = GetConfigValue(config[Name]);
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

		private ObservableCollection<ModConfigPropertyModel> properties = new ObservableCollection<ModConfigPropertyModel>();
		public ObservableCollection<ModConfigPropertyModel> Properties
		{
			get => properties;
			set => this.RaiseAndSetIfChanged(ref properties, value);
		}

		public ModConfigGroupModel() { }

		public ModConfigGroupModel(ConfigSchemaGroup group)
		{
			Name = group.Name;
			DisplayName = group.DisplayName;
			foreach (var property in group.Properties)
			{
				Properties.Add(new ModConfigPropertyModel(property));
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
			foreach(var group in configSchema.Groups)
			{
				Groups.Add(new ModConfigGroupModel(group));
			}

			foreach (var group in configSchema.Enums)
			{
				ObservableCollection<ModConfigEnumMemberModel> conv = new ObservableCollection<ModConfigEnumMemberModel>();
				foreach (var member in group.Members)
				{
					conv.Add(new ModConfigEnumMemberModel(member));
				}

				Enums.Add(group.Name, conv);
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
						configProps.Add(prop.Name, prop.Value.ToString());
						hasProps = true;
					}
				}

				if (hasProps)
					config.Add(group.Name, configProps);
			}

			if (config.Count > 0)
				IniFile.Save(config, filePath);
		}

		public void RevertOptions()
		{
			foreach (var group in Groups)
			{
				foreach (var prop in group.Properties)
				{
					prop.Value = prop.GetPropertyDefaultValue();
				}
			}
		}
	}
}
