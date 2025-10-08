using ReactiveUI;
using SAMM.Configuration.Mods;
using SAMM.Utilities.INI;
using System.Collections.Generic;
using System.IO;
using IniConfig = System.Collections.Generic.Dictionary<string, System.Collections.Generic.Dictionary<string, string>>;

namespace SAMM.App.Models.Dialogs
{
	public class ModConfigDialogModel : ReactiveObject
	{
		private ModConfigModel modConfig = new ModConfigModel();
		public ModConfigModel ModConfig
		{
			get => modConfig;
			set => this.RaiseAndSetIfChanged(ref modConfig, value);
		}

		private IniConfig config = new IniConfig();
		public IniConfig Config
		{
			get { return config; }
			set { this.RaiseAndSetIfChanged(ref config, value); }
		}

		private string configPath = string.Empty;

		public ModConfigDialogModel(ModEntry entry) 
		{
			ModConfig = new ModConfigModel(ConfigSchema.Load(Path.Combine(entry.Directory, "configschema.xml")));
			configPath = Path.Combine(entry.Directory, "config.ini");
			if (File.Exists(configPath))
			{
				Config = IniFile.Load(configPath);

				foreach (var group in ModConfig.Groups)
				{
					if (Config.ContainsKey(group.Name))
					{
						foreach (var prop in group.Properties)
						{
							prop.UpdateFromConfigFile(Config[group.Name]);
						}
					}
				}
			}
		}

		public void ResetConfig()
		{
			ModConfig.RevertOptions();
		}

		public void WriteConfigFile()
		{
			ModConfig.WriteConfigFile(configPath);
		}
	}
}
