using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAModManager.ManagerSettings
{
	public class UpdateSettings
	{
		[DefaultValue(true)]
		public bool UpdateCheck { get; set; } = true;		// LoaderInfo.UpdateCheck
		[DefaultValue(true)]
		public bool ModUpdateCheck { get; set; } = true;	// LoaderInfo.ModUpdateCheck

		[DefaultValue(UpdateUnit.Weeks)]
		public UpdateUnit UpdateUnit { get; set; } = UpdateUnit.Weeks;	// LoaderInfo.UpdateUnit
		[DefaultValue(1)]
		public int UpdateFrequency { get; set; } = 1;		// LoaderInfo.UpdateFrequency

		[DefaultValue(0)] public long UpdateTime { get; set; }		// LoaderInfo.UpdateTime
		[DefaultValue(0)] public long ModUpdateTime { get; set; }	// LoaderInfo.ModUpdateTime
	}

	public class DebugSettings
	{
		[DefaultValue(false)]
		public bool DebugConsole { get; set; }      // SADXLoaderInfo.DebugConsole
		[DefaultValue(false)]
		public bool DebugScreen { get; set; }       // SADXLoaderInfo.DebugScreen
		[DefaultValue (false)]
		public bool DebugFile { get; set; }         // SADXLoaderInfo.DebugFile
		[DefaultValue(true)]
		public bool DebugCrashLog { get; set; } = true;     // SADXLoaderInfo.DebugCrashLog
		[DefaultValue(false)]
		public bool? ShowConsole { get { return null; } set { if (value.HasValue) DebugConsole = value.Value; } }   // SADXLoaderInfo.ShowConsole
	}

	public class ManagerSettings
	{
		[DefaultValue(0)]
		public int Theme { get; set; } = 0;         // SADXLoaderInfo.Theme
		[DefaultValue(0)]
		public int Language { get; set; } = 0;      // SADXLoaderInfo.Language
		[DefaultValue(false)]
		public bool managerOpen { get; set; }       // SADXLoaderInfo.managerOpen
		[DefaultValue(false)]
		public bool devMode { get; set; }           // SADXLoaderInfo.devMode
	}
}
