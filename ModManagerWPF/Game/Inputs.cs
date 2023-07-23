using IniFile;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ModManagerWPF.Game
{
	public class Inputs
	{

		static readonly int[] buttonIDs = { 16, 17, 3, 2, 1, 10, 9, 8 };
		int currentAction;
		System.Threading.Thread controllerThread;

		public class ControllerConfig
		{
			[IniName("buttons")]
			public int ButtonCount { get; set; }
			[IniName("pad")]
			[IniCollection(IniCollectionMode.NoSquareBrackets)]
			public int[] ButtonSettings { get; set; }
		}

		class ControllerConfigInternal
		{
			public string Name { get; set; }
			public int[] Buttons { get; set; }
		}

		public static ushort MouseToInt(ushort n)
		{
			switch (n)
			{
				case 258:
					return 5;
				case 513:
					return 6;
				default:
					return n;
			}
		}

		public static ushort IntToMouse(ushort n)
		{
			switch (n)
			{
				case 5:
					return 258;
				case 6:
					return 513;
				default:
					return n;
			}
		}

	}
}
