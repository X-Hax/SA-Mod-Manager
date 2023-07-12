using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace ModManagerWPF.Common
{
	public class TestSpawn
	{
		private ComboBox testSpawnLvlBox { get; set; }
		private ComboBox comboBoxTestSpawnGameMode { get; set; }
		private ComboBox boxTestSpawnEvent { get; set; }
		private ComboBox boxCharacters { get; set; }
		private Dictionary<int, string> TestSpawnCutsceneList;
		private Dictionary<ushort, string> TestSpawnGameModeList;

		public static readonly List<string> TimeDay = new()
		{
			"Unset",
			"Day",
			"Evening",
			"Night"
		};

		public static readonly List<string> actsRegular = new()
		{
			"Act 1",
			"Act 2",
			"Act 3",
			"Act 4",
			"Act 5",
			"Act 6",
			"Act 7",
			"Act 8",
		};

		public static readonly List<string> acts1only = new()
		{
			"Act 1",
			"Act 2 (Unused)",
			"Act 3 (Unused)",
			"Act 4 (Unused)",
			"Act 5 (Unused)",
			"Act 6 (Unused)",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};

		public static readonly List<string> actsEC = new()
		{
			"Act 1 - Azure Blue World",
			"Act 2 - Windy and Ripply",
			"Act 3 - BIG fishes",
			"Act 4 (Unused)",
			"Act 5 (Unused)",
			"Act 6 (Unused)",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};

		public static readonly List<string> actsWV = new()
		{
			"Act 1 - Windy Hill",
			"Act 2 - Tornado",
			"Act 3 - The Air",
			"Act 4 (Unused)",
			"Act 5 (Unused)",
			"Act 6 (Unused)",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};

		public static readonly List<string> actsTP = new()
		{
			"Act 1 - Twinkle Cart",
			"Act 2 - Pleasure Castle",
			"Act 3 - Fakery Way",
			"Act 4 (Unused)",
			"Act 5 (Unused)",
			"Act 6 (Unused)",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};

		public static readonly List<string> actsSH = new()
		{
			"Act 1 - Run Through the Speed Highway",
			"Act 2 - Goin' Down",
			"Act 3 - At Dawn",
			"Act 4 (Unused)",
			"Act 5 (Unused)",
			"Act 6 (Unused)",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};

		public static readonly List<string> actsRM = new()
		{
			"Act 1 - Mt Red a Symbol of Thrill",
			"Act 2 - Red Hot Skull",
			"Act 3 - (Knuckles)",
			"Act 4 (Unused)",
			"Act 5 (Unused)",
			"Act 6 (Unused)",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};

		private static readonly List<string> actsSD = new()
		{
			"Act 1 - Skydeck A Go Go",
			"Act 2 - In the sky",
			"Act 3 - General Offensive",
			"Act 4 (Unused)",
			"Act 5 (Unused)",
			"Act 6 (Unused)",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};

		private static readonly List<string> actsLW = new()
		{
			"Act 1 - Tricky Maze",
			"Act 2 - Danger Chased by Rock",
			"Act 3 - Leading Lights",
			"Act 4 (Unused)",
			"Act 5 (Unused)",
			"Act 6 (Unused)",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};

		private static readonly List<string> actsIC = new()
		{
			"Act 1 - Snowy Mountain",
			"Act 2 - Limestone Cave",
			"Act 3 - Be Cool, Be Wild and Be Groovy",
			"Act 4 - Limestone Cave (Big)",
			"Act 5 (Unused)",
			"Act 6 (Unused)",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};

		private static readonly List<string> actsCas = new()
		{
			"Act 1 - The Dreamy Stage",
			"Act 2 - Dilapidated Way",
			"Act 3 - Blue Star",
			"Act 4 - Message from Nightopia",
			"Act 5 (Unused)",
			"Act 6 (Unused)",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};

		private static readonly List<string> actsFE = new()
		{
			"Act 1 - Mechanical Resonance",
			"Act 2 - Crank the Heat Up",
			"Act 3 - Mechanical Resonance 2",
			"Act 4 (Unused)",
			"Act 5 (Unused)",
			"Act 6 (Unused)",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};


		private static readonly List<string> actsHS = new()
		{
			"Act 1 - Bad Taste Aquarium",
			"Act 2 - Red Barrage Area",
			"Act 3 - (Gamma)",
			"Act 4 (Unused)",
			"Act 5 (Unused)",
			"Act 6 (Unused)",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};		
		
		
		private static readonly List<string> actsChaos = new()
		{
			"Act 1 - Boss CHAOS",
			"Act 2 (Unused)",
			"Act 3 (Unused)",
			"Act 4 (Unused)",
			"Act 5 (Unused)",
			"Act 6 (Unused)",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};

		private static readonly List<string> actsChaos6 = new()
		{
			"Act 1 - Boss CHAOS ver.6",
			"Act 2 (Knuckles)",
			"Act 3 (Unused)",
			"Act 4 (Unused)",
			"Act 5 (Unused)",
			"Act 6 (Unused)",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};


		private static readonly List<string> actsEH = new()
		{
			"Act 1 - Egg Mobile ...Boss",
			"Act 2 (Unused)",
			"Act 3 (Unused)",
			"Act 4 (Unused)",
			"Act 5 (Unused)",
			"Act 6 (Unused)",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};


		private static readonly List<string> actsEW = new()
		{
			"Act 1 - Militant Missionary Boss Egg Walker",
			"Act 2 (Unused)",
			"Act 3 (Unused)",
			"Act 4 (Unused)",
			"Act 5 (Unused)",
			"Act 6 (Unused)",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};

		private static readonly List<string> actsEV = new()
		{
			"Act 1 - Militant Missionary Boss Egg Viper",
			"Act 2 (Unused)",
			"Act 3 (Unused)",
			"Act 4 (Unused)",
			"Act 5 (Unused)",
			"Act 6 (Unused)",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};

		private static readonly List<string> actsZero = new()
		{
			"Act 1 - ZERO The Chase-master",
			"Act 2 (Unused)",
			"Act 3 (Unused)",
			"Act 4 (Unused)",
			"Act 5 (Unused)",
			"Act 6 (Unused)",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};

		private static readonly List<string> actsSS = new()
		{
			"Act 1 - Hall",
			"Act 2 - Station",
			"Act 3 - Sewers",
			"Act 4 - Main",
			"Act 5 - Hostel (Pool)",
			"Act 6 - Twinkle Park Area",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};

		private static readonly List<string> actsECOut = new()
		{
			"Act 1 - landing runway (UnTransformed)",
			"Act 2 - Main (Transformed)",
			"Act 3 - Bridge (Transformed)",
			"Act 4 - Captain Headquarters Room",
			"Act 5 - Hall",
			"Act 6 - Pool",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};

		private static readonly List<string> actsECIn = new()
		{
			"Act 1 - landing runway (UnTransformed)",
			"Act 2 - Main (Transformed)",
			"Act 3 - Bridge (Transformed)",
			"Act 4 - Captain Headquarters Room",
			"Act 5 - Hall",
			"Act 6 - Pool",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};

		private static readonly List<string> actsMR = new()
		{
			"Act 1 - Main",
			"Act 2 - Angel Island",
			"Act 3 - Jungle",
			"Act 4 - Final Egg",
			"Act 5 (Unused)",
			"Act 6 (Unused)",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};

		private static readonly List<string> actsPast = new()
		{
			"Act 1 - Main",
			"Act 2 - Altar",
			"Act 3 - Altar (Fire)",
			"Act 4 (Unused)",
			"Act 5 (Unused)",
			"Act 6 (Unused)",
			"Act 7 (Unused)",
			"Act 8 (Unused)",
		};

		private static readonly List<List<string>> acts = new()
		{
			actsRegular,
			actsEC,
			actsWV,
			actsTP,
			actsSH,
			actsRM,
			actsSD,
			actsLW,
			actsIC,
			actsCas,
			actsFE,
			actsRegular,
			actsHS,
			actsRegular,
			actsRegular,
			actsChaos,
			actsChaos,
			actsChaos,
			actsChaos6,
			actsChaos,
			actsEH,
			actsEW,
			actsEV,
			actsZero,
			acts1only,
			acts1only,
			actsSS,
			actsRegular,
			actsRegular,
			actsECOut,
			actsRegular,
			actsRegular,
			actsECIn,
			actsMR,
			actsPast,
			actsRegular,
			acts1only,
			acts1only,
			acts1only,
			acts1only,
			acts1only,
			acts1only,
			acts1only
		};

		public static List<string> GetNewAct(int index)
		{
			if (index > acts.Count)
			{
				return actsRegular;
			}

			return acts[index];
		}

		public TestSpawn(ref ComboBox testspawnlvlBox, ref ComboBox comboBoxTestSpawnGameMode, ref ComboBox boxTestSpawnEvent, ref ComboBox boxCharacter)
		{
			this.testSpawnLvlBox = testspawnlvlBox;
			this.comboBoxTestSpawnGameMode = comboBoxTestSpawnGameMode;
			this.boxTestSpawnEvent = boxTestSpawnEvent;
			this.boxCharacters = boxCharacter;

		}

		public void InitCharactersList()
		{
			boxCharacters.Items.Add("Sonic");
			boxCharacters.Items.Add("Eggman (unused)");
			boxCharacters.Items.Add("Tails");
			boxCharacters.Items.Add("Knuckles");
			boxCharacters.Items.Add("Tikal");
			boxCharacters.Items.Add("Amy");
			boxCharacters.Items.Add("E-102 Gamma");
			boxCharacters.Items.Add("Big");
		}

		public void InitLevels()
		{
			List<string> levels = new List<string>
			{
			"Hedgehog Hammer",
			"Emerald Coast",
			"Windy Valley",
			"Twinkle Park",
			"Speed Highway",
			"Red Mountain",
			"Sky Deck",
			"Lost World",
			"Ice Cap",
			"Casinopolis",
			"Final Egg",
			"Unused (11)",
			"Hot Shelter",
			"Unused (13)",
			"Unused (14)",
			"Chaos 0",
			"Chaos 2",
			"Chaos 4",
			"Chaos 6",
			"Perfect Chaos",
			"Egg Hornet",
			"Egg Walker",
			"Egg Viper",
			"ZERO",
			"E-101",
			"E-101R",
			"Station Square",
			"Station Square Unused (27)",
			"Station Square Unused (28)",
			"Egg Carrier Outside",
			"Unused (30)",
			"Unused (31)",
			"Egg Carrier Inside",
			"Mystic Ruins",
			"Past",
			"Twinkle Circuit",
			"Sky Chase 1",
			"Sky Chase 2",
			"Sand Hill",
			"Station Square Garden",
			"Egg Carrier Garden",
			"Mystic Ruins Garden",
			"Chao Race"
		};

			foreach (string level in levels) 
			{
				testSpawnLvlBox.Items.Add(level);
			}
		
		}

		public Dictionary<int, string> GetCutsceneList()
		{
			return TestSpawnCutsceneList;
		}

		public void InitCutsceneList()
		{
			TestSpawnCutsceneList = new Dictionary<int, string>();

			for (int i = 0; i < 385; i++)
			{
				string cutsceneName = Lang.GetString("TSCutscene" + i.ToString());

				if (cutsceneName.Contains("TSCutscene") || string.IsNullOrEmpty(cutsceneName))
					continue;

				TestSpawnCutsceneList.Add(i, cutsceneName);
			}

			boxTestSpawnEvent.Items.Clear();

			foreach (var item in TestSpawnCutsceneList)
			{
				boxTestSpawnEvent.Items.Add("EV" + item.Key.ToString("X4") + ": " + item.Value);
			}

		}

		public Dictionary<ushort, string> GetTestSpawnGameModeList()
		{
			return TestSpawnGameModeList;
		}
		public void InitGameModeList()
		{
			TestSpawnGameModeList = new Dictionary<ushort, string>
			{
				{ 4, "Action Stage" },
				{ 5, "Adventure" },
				{ 9, "Trial" },
				{ 10, "Mission" }
			};

			comboBoxTestSpawnGameMode.Items.Clear();

			foreach (var item in TestSpawnGameModeList)
			{
				comboBoxTestSpawnGameMode.Items.Add(item.Value);
			}
		}
	}
}
