using System;
using System.Collections.Generic;
using System.Text;

namespace SAMM.Configuration.SHDR
{
	public class ShadowSettings : GameSettings
	{
		public ShadowSettings()
		{
			hideConfig = true;
			hideDebug = true;
		}

		public override void ToGameConfig(string path) { }
	}
}
