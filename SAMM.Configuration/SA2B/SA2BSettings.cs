using System.Text;
using System.Text.Json.Serialization;

namespace SAMM.Configuration.SA2B
{
	public class SA2BGraphicsSettings : GraphicsSettings
	{
		#region Enumeration
		public enum DisplayMode
		{
			Windowed,
			Borderless,
			CustomWindow,
			Fullscreen
		}

		#endregion

		#region Variables
		public bool SkipIntro { get; set; } = false;

		public int RefreshRate { get; set; } = 60;

		#endregion
	}

	public class SA2BTestSpawnSettings : TestSpawnSettings
	{
		#region Variables
		public bool UsePlayer2 { get; set; } = false;

		public int Player2Index { get; set; } = -1;

		public int MissionIndex { get; set; } = -1;

		public bool UseEventManual { get; set; } = false;

		#endregion

		#region Functions
		public override string GenerateParameters()
		{
			StringBuilder sb = new StringBuilder();

			if (CharacterIndex > -1 && (UseCharacter || UseManual))
				sb.Append($"-c {CharacterIndex} ");
			if (Player2Index > -1 && UsePlayer2)
				sb.Append($"-p2 {Player2Index} ");
			if (LevelIndex > -1 && (UseLevel || UseManual))
				sb.Append($"-l {LevelIndex} ");
			if (MissionIndex > -1 && (UseLevel || UseManual))
				sb.Append($"-m {MissionIndex} ");
			if (UsePosition)
				sb.Append($"-p {XPosition} {YPosition} {ZPosition} -r {Rotation} ");
			if (EventIndex > -1 && (UseEvent || UseEventManual))
				sb.Append($"-e {EventIndex}");
			// TODO: Reimplement Save stuff

			return sb.ToString();
		}

		#endregion
	}

	public class SA2BSettings : GameSettings
	{
		#region Variables
		[JsonPropertyName("GraphicSettings")]
		public SA2BGraphicsSettings GraphicsSettings { get; set; } = new SA2BGraphicsSettings();

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
