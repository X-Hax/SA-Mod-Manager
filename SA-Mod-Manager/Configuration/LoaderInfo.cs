using SAModManager.Ini;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAModManager.Configuration
{
	public enum UpdateUnit
	{
		Always,
		Hours,
		Days,
		Weeks,
	}

	public class LoaderInfo
	{
		[DefaultValue(true)]
		public bool UpdateCheck { get; set; } = true;
		[DefaultValue(true)]
		public bool ModUpdateCheck { get; set; } = true;

		[DefaultValue(UpdateUnit.Weeks)]
		public UpdateUnit UpdateUnit { get; set; } = UpdateUnit.Weeks;
		[DefaultValue(1)]
		public int UpdateFrequency { get; set; } = 1;

		[DefaultValue(0)] public long UpdateTime { get; set; }
		[DefaultValue(0)] public long ModUpdateTime { get; set; }

		[DefaultValue(true)]
		public bool AutoClose { get; set; } = true;

		[IniName("Mod")]
		[IniCollection(IniCollectionMode.NoSquareBrackets, StartIndex = 1)]
		public List<string> Mods { get; set; }
		[IniName("Code")]
		[IniCollection(IniCollectionMode.NoSquareBrackets, StartIndex = 1)]
		public List<string> EnabledCodes { get; set; }

		public LoaderInfo()
		{
			Mods = new List<string>();
			EnabledCodes = new List<string>();
		}
	}
}
