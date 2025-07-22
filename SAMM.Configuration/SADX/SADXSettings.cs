using SAMM.Utilities.INI;
using System.ComponentModel;
using System.Text;
using System.Text.Json.Serialization;

namespace SAMM.Configuration.SADX
{
	public class SADXGraphicsSettings: GraphicsSettings
	{
		#region Enumeration
		public enum FillMode
		{
			Stretch = 0,
			Fit = 1,
			Fill = 2
		}

		public enum DisplayMode
		{
			Windowed,
			Fullscreen,
			Borderless,
			CustomWindow
		}

		public enum RenderBackendOptions
		{
			Direct3D8 = 0,
			Direct3D9 = 1,
			Direct3D11 = 2,
		}

		#endregion

		#region Variables
		public bool EnableVSync { get; set; } = true;

		public int FillModeBackground { get; set; } = (int)FillMode.Fill;

		public int FillModeFMV { get; set; } = (int)FillMode.Fit;

		public bool EnableUIScaling { get; set; } = true;

		public bool EnableForcedMipmapping { get; set; } = true;

		public bool EnableForcedTextureFilter { get; set; } = true;

		public int GameFrameRate { get; set; } = 1;

		public int GameFogMode { get; set; } = 0;

		public int GameClipLevel { get; set; } = 0;

		public int ShowMouseInFullscreen { get; set; } = 0;

		public int RenderBackend { get; set; } = (int)RenderBackendOptions.Direct3D9;

		#endregion
	}

	public class SADXAudioSettings
	{
		#region Variables
		public bool EnableGameMusic { get; set; } = true;

		public bool EnableGameSound { get; set; } = true;

		public bool EnableGameSound3D { get; set; } = true;

		public bool EnableBassMusic { get; set; } = true;

		public bool EnableBassSFX { get; set; } = true;

		public int GameMusicVolume { get; set; } = 100;

		public int GameSoundVolume { get; set; } = 100;

		public int SEVolume { get; set; } = 100;

		#endregion
	}

	public class SADXControllerSettings
	{
		#region Variables
		public bool EnabledInputMod { get; set; } = true;

		public bool VanillaMouseUseDrag { get; set; } = false;

		public int VanillaMouseStart { get; set; } = 0;

		public int VanillaMouseAttack { get; set; } = 0;

		public int VanillaMouseJump { get; set; } = 0;

		public int VanillaMouseAction { get; set; } = 0;

		public int VanillaMouseFlute { get; set; } = 0;

		#endregion
	}

	public class SADXTestSpawnSettings : TestSpawnSettings
	{
		#region Variables
		public int ActIndex { get; set; } = 0;

		public bool UseGameMode { get; set; } = false;

		public int GameModeIndex { get; set; } = -1;

		#endregion

		#region Functions
		public override string GenerateParameters()
		{
			StringBuilder sb = new StringBuilder();
			if (CharacterIndex > -1 && (UseCharacter || UseManual))
				sb.Append($"-c {CharacterIndex} ");
			if (LevelIndex > -1 && (UseLevel || UseManual))
				sb.Append($"-l {LevelIndex} ");
			if (ActIndex > -1 && (UseLevel || UseManual))
				sb.Append($"-a {ActIndex} ");
			// TODO: Reimplement Time of Day
			if (UsePosition)
				sb.Append($"-p {XPosition} {YPosition} {ZPosition} -r {Rotation} ");
			if (EventIndex > -1 && UseEvent)
				sb.Append($"-e {EventIndex} ");
			if (GameModeIndex > -1 && UseGameMode)
				sb.Append($"-g {GameModeIndex} ");
			// TODO: Reimplement Save TestSpawn Stuff

			return sb.ToString();
		}

		#endregion
	}

	public class SADXGameConfiguration
	{
		public class SADXConfig
		{
			[IniName("framerate")]
			public int FrameRate { get; set; } = 1;
			[IniAlwaysInclude]
			[IniName("fogemulation")]
			public int Foglation { get; set; } = 0;
			[IniAlwaysInclude]
			[IniName("sound3d")]
			[DefaultValue(1)]
			public int Sound3D { get; set; } = 1;
			[IniAlwaysInclude]
			[IniName("screensize")]
			public int ScreenSize { get; set; } = 2;
			[IniAlwaysInclude]
			[IniName("cliplevel")]
			public int ClipLevel { get; set; } = 0;
			[IniAlwaysInclude]
			[IniName("sevoice")]
			[DefaultValue(1)]
			public int SEVoice { get; set; } = 1;
			[IniAlwaysInclude]
			[IniName("bgm")]
			[DefaultValue(1)]
			public int BGM { get; set; } = 1;
			[IniAlwaysInclude]
			[IniName("screen")]
			public int FullScreen { get; set; } = 0;
			[IniAlwaysInclude]
			[IniName("mousemode")]
			public int MouseMode { get; set; }
			[IniAlwaysInclude]
			[IniName("bgmv")]
			[DefaultValue(100)]
			public int BGMVolume { get; set; } = 100;
			[IniAlwaysInclude]
			[IniName("voicev")]
			[DefaultValue(100)]
			public int VoiceVolume { get; set; } = 100;
			[IniName("language")]
			public int Language { get; set; }
			[IniName("padconfig")]
			public string PadConfig { get; set; }

			[IniName("cmd0")]
			public ushort MouseStart { get; set; }
			[IniName("cmd1")]
			public ushort MouseAttack { get; set; }
			[IniName("cmd2")]
			public ushort MouseJump { get; set; }
			[IniName("cmd3")]
			public ushort MouseAction { get; set; }
			[IniName("cmd4")]
			public ushort MouseFlute { get; set; }
		}

		[IniName("sonicDX")]
		public SADXConfig GameConfig { get; set; } = new SADXConfig();
	}

	public class SADXSettings : GameSettings
	{
		#region Enumeration
		public enum Version
		{
			v0 = 0,     // Version 0: Original LoaderInfo version
			v1 = 1,     // Version 1: Initial version at launch
			v2 = 2,     // Version 2: Updated to include all settings, intended to be used as the only loaded file, now writes SADXLoaderInfo and SADXConfigFile.
			v3 = 3,     // Version 3: Added Graphics.StretchToWindow and Graphics.DisableBorderWindow.
			v4 = 4,     // Version 4: Removed old Patch system entirely, moving to new modular system.
			v5 = 5,		// Version 5: Rebuilt all settings with refactor, still compatible with previous versions. Removed deprecated data. Merged classes where possible.

			MAX,        // Do Not Modify, new versions are placed above this.
		}

		#endregion

		#region Variables
		[JsonPropertyName("Graphics")]
		public SADXGraphicsSettings GraphicsSettings { get; set; } = new SADXGraphicsSettings();

		[JsonPropertyName("Controller")]
		public SADXControllerSettings ControllerSettings { get; set; } = new SADXControllerSettings();

		[JsonPropertyName("Sound")]
		public SADXAudioSettings AudioSettings { get; set; } = new SADXAudioSettings();

		[JsonPropertyName("TestSpawn")]
		public SADXTestSpawnSettings TestSpawnSettings { get; set; } = new SADXTestSpawnSettings();

		#endregion

		#region Functions
		public override void Serialize(string path)
		{
			throw new NotImplementedException();
		}

		public override void Deserialize(string path) 
		{ 
			throw new NotImplementedException(); 
		}

		public override void ToGameConfig(string path)
		{
			throw new NotImplementedException();
		}

		#endregion
	}
}
