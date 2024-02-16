using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAModManager.Configuration.SA2
{
	/// <summary>
	/// This class is legacy code and is only here for handling the old manager/loader system.
	/// </summary>
	public class SA2LoaderInfo : LoaderInfo
	{
		public bool DebugConsole { get; set; }
		public bool DebugScreen { get; set; }
		public bool DebugFile { get; set; }
		[DefaultValue(true)]
		public bool DebugCrashLog { get; set; } = true;
		public bool? ShowConsole { get { return null; } set { if (value.HasValue) DebugConsole = value.Value; } }
		[DefaultValue(true)]
		public bool PauseWhenInactive { get; set; }

		[DefaultValue(1)]
		public int ScreenNum { get; set; } = 1;
		[DefaultValue(true)]
		public bool WindowedFullscreen { get; set; } = true; //previously borderless window

		[DefaultValue(false)]
		public bool FullScreen { get; set; }
		public bool SkipIntro { get; set; }

		[DefaultValue(640)]
		public int HorizontalResolution { get; set; } = 640;
		[DefaultValue(480)]
		public int VerticalResolution { get; set; } = 480;
		[DefaultValue(1)]
		public int VoiceLanguage { get; set; } = 1;
		[DefaultValue(0)]
		public int TextLanguage { get; set; } = 0;
		public bool CustomWindowSize { get; set; }
		[DefaultValue(640)]
		public int WindowWidth { get; set; } = 640;
		[DefaultValue(480)]
		public int WindowHeight { get; set; } = 480;
		[DefaultValue(false)]
		public bool ResizableWindow { get; set; }
		[DefaultValue(false)]
		public bool MaintainAspectRatio { get; set; }

		#region Manager Settings
		public int Theme { get; set; } = 0;
		[DefaultValue(0)]
		public int Language { get; set; } = 0;
		[DefaultValue(true)]
		public bool InputModEnabled { get; set; }
		[DefaultValue(true)]
		public bool managerOpen { get; set; }
		[DefaultValue(false)]
		public bool devMode { get; set; }

		#endregion

		#region testSpawn
		[DefaultValue(-1)]
		public int TestSpawnLevel { get; set; } = -1;
		[DefaultValue(-1)]
		public int TestSpawnCharacter { get; set; } = -1;
		[DefaultValue(0)]
		public int TestSpawnPlayer2 { get; set; } = -1;
		[DefaultValue(0)]
		public int TestSpawnX { get; set; } = 0;
		[DefaultValue(0)]
		public int TestSpawnY { get; set; } = 0;
		[DefaultValue(0)]
		public int TestSpawnZ { get; set; } = 0;
		[DefaultValue(false)]
		public bool TestSpawnPositionEnabled { get; set; } = false;

		[DefaultValue(-1)]
		public int TestSpawnEvent { get; set; } = -1;
		[DefaultValue(-1)]
		public int TestSpawnSaveID { get; set; } = -1;
		#endregion


		#region Patches
		[DefaultValue(true)]
		public bool FramerateLimiter { get; set; } = true;
		[DefaultValue(true)]
		public bool DisableExitPrompt { get; set; } = true;
		[DefaultValue(true)]
		public bool SyncLoad { get; set; } = true;
		[DefaultValue(true)]
		public bool ExtendVertexBuffer { get; set; } = true; //incrase the vertex limit per mesh to 32k
		[DefaultValue(true)]
		public bool EnvMapFix { get; set; } = true;
		[DefaultValue(true)]
		public bool ScreenFadeFix { get; set; } = true;
		[DefaultValue(true)]
		public bool CECarFix { get; set; } = true; //intel GPU issue
		[DefaultValue(true)]
		public bool ParticlesFix { get; set; } = true; //intel GPU issue

		#endregion

		public SA2LoaderInfo()
		{
			Mods = new List<string>();
			EnabledCodes = new List<string>();
		}
	}
}
