using System.IO;
using System.Xml.Serialization;

namespace SAModManager.Configuration.SA2
{
	[XmlRoot("Configs")]
    public class SA2GameConfig
    {
		[XmlAttribute("FullScreen")]
        public int FullScreen { get; set; } = 0;

		[XmlAttribute("Display")]
		public int Display { get; set; } = 0;

		[XmlAttribute("Res")]
		public int Res { get; set; } = 0;

		[XmlAttribute("Width")]
		public int Width { get; set; } = 640;

		[XmlAttribute("Height")]
		public int Height { get; set; } = 480;

		[XmlAttribute("RefreshRate")]
		public int RefreshRate { get; set; } = 60;

		[XmlAttribute("Language")]
		public int Language { get; set; } = 0;

		[XmlAttribute("Analytics")]
		public int Analytics { get; set; } = 0;

		public SA2GameConfig()
		{
			FullScreen = 0;
			Display = 0;
			Res = 0;
			Width = 640;
			Height = 480;
			RefreshRate = 60;
			Language = 0;
			Analytics = 0;
		}


		public void Serialize(string path)
		{
			try
			{
				XmlSerializer serializer = new(typeof(SA2GameConfig));
                using TextWriter writer = new StreamWriter(path);
                serializer.Serialize(writer, this);
            }
			catch
			{

			}
		}

		public static SA2GameConfig Deserialize(string path)
		{
			try
			{
				XmlSerializer serializer = new(typeof(SA2GameConfig));

                using TextReader reader = new StreamReader(path);
                return (SA2GameConfig)serializer.Deserialize(reader);
            }
			catch
			{
				return new SA2GameConfig();
			}
		}
    }
}
