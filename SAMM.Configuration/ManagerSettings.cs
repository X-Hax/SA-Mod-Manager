using SAMM.Configuration.Enumeration;
using System.ComponentModel;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace SAMM.Configuration
{
	public enum ManagerSettingsVersions
	{
		v0 = 0, // V0: Pre-Release
		v1 = 1, // V1: Initial Release
		v2 = 2, // V2: Migrated to GameEntry system for managing game selection setup.
		v3 = 3, // V3: Added Software Render Mode option (Visible option is "Disable Hardware Acceleration").

		MAX,
	}

	public class UpdateSettings
	{
		/// <summary>
		/// Checks for Manager Updates on Boot.
		/// </summary>
		[DefaultValue(true)]
		public bool EnableManagerBootCheck { get; set; } = true;

		/// <summary>
		/// Checks for Mod Updates on Boot.
		/// </summary>
		[DefaultValue(true)]
		public bool EnableModsBootCheck { get; set; } = true;

		/// <summary>
		/// Checks for Loader Updates on Boot.
		/// </summary>
		[DefaultValue(true)]
		public bool EnableLoaderBootCheck { get; set; } = true;

		/// <summary>
		/// Countdown storage for the update to timeout.
		/// </summary>
		public long UpdateTimeOutCD { get; set; } = 0;

		/// <summary>
		/// Number of times the Manager has run update checks.
		/// </summary>
		public int UpdateCheckCount { get; set; } = 0;
	}

	public class GameEntry
	{
		#region Variables
		/// <summary>
		/// <see cref="GameIDs"/> for the <see cref="GameEntry"/>.
		/// </summary>
		public GameIDs Type { get; set; }

		/// <summary>
		/// Name of the <see cref="GameEntry"/>.
		/// </summary>
		public string Name { get; set; } = GameIDs.Unsupported.ToString();

		/// <summary>
		/// Installation directory of the <see cref="GameEntry"/>.
		/// </summary>
		public string Directory { get; set; } = string.Empty;

		/// <summary>
		/// Executable name for the <see cref="GameEntry"/>
		/// </summary>
		public string Executable { get; set; }

		#endregion

		#region Constructors
		public GameEntry() { }

		#endregion

		#region Functions
		public void Create(string executablePath)
		{
			Executable = Path.GetFileName(executablePath);
			Directory = Path.GetDirectoryName(executablePath);

			switch (Executable)
			{
				default:
					Name = Path.GetFileNameWithoutExtension(Executable);
					Type = GameIDs.Unsupported;
					break;
				case "sonic.exe":
					Name = "Sonic Adventure DX";
					Type = GameIDs.SADX;
					break;
				case "sonic2app.exe":
				case "sonic2app_decrypted.exe":
					Name = "Sonic Adventure 2";
					Type = GameIDs.SA2;
					break;
			}
		}

		#endregion
	}

	public class ManagerSettings
	{
		#region Variables
		/// <summary>
		/// Versioning for the Manager Settings file.
		/// </summary>
		[DefaultValue((int)ManagerSettingsVersions.MAX - 1)]
		public int SettingsVersion { get; set; } = (int)(ManagerSettingsVersions.MAX - 1);

		/// <summary>
		/// Last Loaded Game
		/// </summary>
		[DefaultValue(-1)]
		public int CurrentSetGame { get; set; } = -1;

		/// <summary>
		/// The set Theme for the Manager.
		/// </summary>
		[DefaultValue(0)]
		public int Theme { get; set; } = 0;         // SADXLoaderInfo.Theme

		/// <summary>
		/// The set Language for the Manager.
		/// </summary>
		[DefaultValue(0)]
		public int Language { get; set; } = 0;      // SADXLoaderInfo.Language

		/// <summary>
		/// Defaulted to empty, but is updated when a mod is initially created to the last used Author name when creating a new mod.
		/// </summary>
		public string ModAuthor { get; set; } = string.Empty; //From mod creation: Save the Mod Author from user input, so they don't have to re type it.

		/// <summary>
		/// Enables Developer Options in the Manager.
		/// </summary>
		[DefaultValue(false)]
		public bool EnableDeveloperMode { get; set; }           // SADXLoaderInfo.devMode

		/// <summary>
		/// Keeps the Manager open when launching the game.
		/// </summary>
		[DefaultValue(true)]
		public bool KeepManagerOpen { get; set; } = true;   // SADXLoaderInfo.managerOpen

		/// <summary>
		/// Update Settings for the Manager.
		/// </summary>
		public UpdateSettings UpdateSettings { get; set; } = new();

		/// <summary>
		/// Advanced Settings for the Manager.
		/// </summary>
		//public AdvancedSettings AdvancedSettings { get; set; } = new();

		/// <summary>
		/// List of installed games.
		/// </summary>
		public List<GameEntry> GameEntries { get; set; } = new();

		/// <summary>
		/// The width of the Manager.
		/// </summary>
		[JsonPropertyName("managerWidth")]
		public double ManagerWidth { get; set; } = 700;

		/// <summary>
		/// The height of the Manager.
		/// </summary>
		[JsonPropertyName("managerHeight")]
		public double ManagerHeight { get; set; } = 1200;

		/// <summary>
		/// Maintains the order of all mods in the list.
		/// </summary>
		[DefaultValue(false)]
		public bool KeepModOrder { get; set; } = false;

		/// <summary>
		/// Sets the program to use software rendering.
		/// </summary>
		[DefaultValue(false)]
		public bool UseSoftwareRendering { get; set; } = false;

		#endregion

		#region Constructors
		public ManagerSettings() { }

		#endregion

		#region Functions
		/// <summary>
		/// Deserializes a Manager Settings CFG (JSON) file and returns a populated class.
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		public static ManagerSettings Deserialize(string path)
		{
			try
			{
				if (File.Exists(path))
				{
					string jsonContent = File.ReadAllText(path);

					ManagerSettings settings = JsonSerializer.Deserialize<ManagerSettings>(jsonContent);

					// Switch case to bump settings version. Allows for manual adjustment if needed for any version bumps.
					switch (settings.SettingsVersion)
					{
						default:
							if (settings.SettingsVersion < (int)(ManagerSettingsVersions.MAX - 1))
								settings.SettingsVersion = (int)(ManagerSettingsVersions.MAX - 1);
							break;
					}

					return settings;
				}
				else
				{
					return new();
				}

			}
			catch (Exception ex)
			{
				new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Error"), Lang.GetString("MessageWindow.Errors.ProfileLoad") + "\n\n" + ex.Message, MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error).ShowDialog();

			}

			return new();
		}

		/// <summary>
		/// Serializes a Manager Setting CFG (JSON) file.
		/// </summary>
		/// <param name="path"></param>
		public void Serialize(string path)
		{
			try
			{
				App.CreateConfigFolder();
				string jsonContent = JsonSerializer.Serialize(this, new JsonSerializerOptions { WriteIndented = true });
				File.WriteAllText(path, jsonContent);
			}
			catch
			{

			}
		}

		#endregion
	}
}
