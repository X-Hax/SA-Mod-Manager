using SAMM.Configuration.Enumeration;
using SAMM.Configuration.SADX;
using SAMM.Utilities;
using System.ComponentModel;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Text.Json.Serialization;

namespace SAMM.Configuration
{
	public abstract class GraphicsSettings
	{
		#region Variables
		public int SelectedScreen { get; set; } = 1;

		public int ScreenMode { get; set; } = 0;

		public int HorizontalResolution { get; set; } = 640;

		public int VerticalResolution { get; set; } = 480;

		public int CustomWindowWidth { get; set; } = 640;

		public int CustomWindowHeight { get; set; } = 480;

		public bool Enable43ResolutionRatio { get; set; } = true;

		public bool EnableKeepResolutionRatio { get; set; } = false;

		public bool EnableResizableWindow { get; set; } = false;

		public bool EnablePauseOnInactive { get; set; } = false;

		public bool StretchToWindow { get; set; } = false;

		public bool DisableBorderImage { get; set; } = false;

		#endregion
	}

	public abstract class TestSpawnSettings
	{
		#region Variables
		public bool UseCharacter { get; set; } = false;

		public int CharacterIndex { get; set; } = -1;

		public bool UseLevel { get; set; } = false;

		public int LevelIndex { get; set; } = -1;

		public bool UseEvent { get; set; } = false;

		public int EventIndex { get; set; } = -1;

		public bool UseSave { get; set; } = false;

		public int SaveIndex { get; set; } = -1;

		public int GameTextLanguage { get; set; } = -1;

		public int GameVoiceLanguage { get; set; } = -1;

		public bool UseManual { get; set; } = false;

		public bool UsePosition { get; set; } = false;

		public float XPosition { get; set; } = 0.0f;

		public float YPosition { get; set; } = 0.0f;

		public float ZPosition { get; set; } = 0.0f;

		public int Rotation { get; set; } = 0;

		#endregion

		#region Variables
		public abstract string GenerateParameters();

		#endregion
	}

	public class DebugSettings
	{
		#region Variables
		/// <summary>
		/// Enables debug printing to the console window.
		/// </summary>
		[DefaultValue(false)]
		public bool EnableDebugConsole { get; set; }

		/// <summary>
		/// Enables debug printing in the game window.
		/// </summary>
		[DefaultValue(false)]
		public bool EnableDebugScreen { get; set; }

		/// <summary>
		/// Enables debug printing to a file.
		/// </summary>
		[DefaultValue(false)]
		public bool EnableDebugFile { get; set; }

		/// <summary>
		/// Enables crash log mini dump creation.
		/// </summary>
		[DefaultValue(true)]
		public bool EnableDebugCrashLog { get; set; } = true;

		/// <summary>
		/// Enables the console window to render.
		/// </summary>
		[DefaultValue(false)]
		public bool? EnableShowConsole { get { return null; } set { if (value.HasValue) EnableDebugConsole = value.Value; } }

		#endregion
	}

	public abstract class GameSettings<G,T> 
		where G : GraphicsSettings, new() 
		where T : TestSpawnSettings, new ()
	{
		[JsonPropertyName("Graphics")]
		public G GraphicsSettings { get; set; } = new G();

		[JsonPropertyName("TestSpawn")]
		public T TestSpawnSettings { get; set; } = new T();

		public DebugSettings DebugSettings { get; set; } = new DebugSettings();

		public Dictionary<string, bool> Patches { get; set; } = new Dictionary<string, bool>();

		public List<string> EnabledMods { get; set; } = new List<string>();

		public List<string> EnabledCodes { get; set; } = new List<string>();

		public List<string> ModsList { get; set; } = new List<string>();

		/// <summary>
		/// Serializes the JSON file to the specified path.
		/// </summary>
		/// <param name="path"></param>
		public void Serialize(string path)
		{
			JSON.Serialize(path, this);
		}

		public abstract void ToGameConfig(string path);

		/// <summary>
		/// Deserializes a <see cref="GameSettings"/> derived class to its appropriate object.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="path"></param>
		/// <returns><see cref="GameSettings"/> derived object.</returns>
		public static T Deserialize<T>(string path) where T : GameSettings
		{
			return JSON.Deserialize<T>(path);
		}
	}
}
