using System;
using System.CodeDom;
using System.Collections.Generic;
using System.Diagnostics;
using System.DirectoryServices.ActiveDirectory;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using SAModManager.Common;
using SAModManager.IniSettings.SADX;

namespace SAModManager.Elements.SADX
{
	/// <summary>
	/// Interaction logic for TestSpawn.xaml
	/// </summary>
	public partial class TestSpawn : UserControl
	{
		#region Variables
		public bool IsCharacterChecked
		{
			get { return (bool)GetValue(IsCharacterCheckedProperty); }
			set { SetValue(IsCharacterCheckedProperty, value); }
		}
		public static readonly DependencyProperty IsCharacterCheckedProperty =
			DependencyProperty.Register("IsCharacterChecked", typeof(bool), typeof(TestSpawn));

		public bool IsLevelChecked
		{
			get { return (bool)GetValue(IsLevelCheckedProperty); }
			set { SetValue(IsLevelCheckedProperty, value); }
		}
		public static readonly DependencyProperty IsLevelCheckedProperty =
			DependencyProperty.Register("IsLevelChecked", typeof(bool), typeof(TestSpawn));

		public bool IsEventChecked
		{
			get { return (bool)GetValue(IsEventCheckedProperty); }
			set { SetValue(IsEventCheckedProperty, value); }
		}
		public static readonly DependencyProperty IsEventCheckedProperty =
			DependencyProperty.Register("IsEventChecked", typeof(bool), typeof(TestSpawn));

		public bool IsSaveChecked
		{
			get { return (bool)GetValue(IsSaveCheckedProperty); }
			set { SetValue(IsSaveCheckedProperty, value); }
		}
		public static readonly DependencyProperty IsSaveCheckedProperty =
			DependencyProperty.Register("IsSaveChecked", typeof(bool), typeof(TestSpawn));

		public bool IsGameModeChecked
		{
			get { return (bool)GetValue(IsGameModeCheckedProperty); }
			set { SetValue(IsGameModeCheckedProperty, value); }
		}
		public static readonly DependencyProperty IsGameModeCheckedProperty =
			DependencyProperty.Register("IsGameModeChecked", typeof(bool), typeof(TestSpawn));

		private TestSpawnSettings SpawnSettings;
		private Dictionary<string, SADXModInfo> GameMods;

		#region ComboBox Sources
		public static readonly List<string> CharacterNames = new()
		{
			"Sonic",
			"Eggman (unused)",
			"Tails",
			"Knuckles",
			"Tikal",
			"Amy",
			"E-102 Gamma",
			"Big"
		};

		public static readonly List<string> LevelNames = new()
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

		public static readonly Dictionary<int, string> GameModeNames = new()
		{
			{ 4, "Action Stage" },
			{ 5, "Adventure" },
			{ 9, "Trial" },
			{ 10, "Mission" }
		};

		#region Act Lists
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
		};

		public static readonly List<string> actsEC = new()
		{
			"Act 1 - Azure Blue World",
			"Act 2 - Windy and Ripply",
			"Act 3 - BIG fishes",
		};

		public static readonly List<string> actsWV = new()
		{
			"Act 1 - Windy Hill",
			"Act 2 - Tornado",
			"Act 3 - The Air",
		};

		public static readonly List<string> actsTP = new()
		{
			"Act 1 - Twinkle Cart",
			"Act 2 - Pleasure Castle",
			"Act 3 - Fakery Way",
		};

		public static readonly List<string> actsSH = new()
		{
			"Act 1 - Run Through the Speed Highway",
			"Act 2 - Goin' Down",
			"Act 3 - At Dawn",
		};

		public static readonly List<string> actsRM = new()
		{
			"Act 1 - Mt Red a Symbol of Thrill",
			"Act 2 - Red Hot Skull",
			"Act 3 - (Knuckles)",
		};

		private static readonly List<string> actsSD = new()
		{
			"Act 1 - Skydeck A Go Go",
			"Act 2 - In the sky",
			"Act 3 - General Offensive",
		};

		private static readonly List<string> actsLW = new()
		{
			"Act 1 - Tricky Maze",
			"Act 2 - Danger Chased by Rock",
			"Act 3 - Leading Lights",
		};

		private static readonly List<string> actsIC = new()
		{
			"Act 1 - Snowy Mountain",
			"Act 2 - Limestone Cave",
			"Act 3 - Be Cool, Be Wild and Be Groovy",
			"Act 4 - Limestone Cave (Big)",
		};

		private static readonly List<string> actsCas = new()
		{
			"Act 1 - The Dreamy Stage",
			"Act 2 - Dilapidated Way",
			"Act 3 - Blue Star",
			"Act 4 - Message from Nightopia",
		};

		private static readonly List<string> actsFE = new()
		{
			"Act 1 - Mechanical Resonance",
			"Act 2 - Crank the Heat Up",
			"Act 3 - Mechanical Resonance 2",
		};

		private static readonly List<string> actsHS = new()
		{
			"Act 1 - Bad Taste Aquarium",
			"Act 2 - Red Barrage Area",
			"Act 3 - (Gamma)",
		};

		private static readonly List<string> actsChaos = new()
		{
			"Act 1 - Boss CHAOS",
		};

		private static readonly List<string> actsChaos6 = new()
		{
			"Act 1 - Boss CHAOS ver.6",
			"Act 2 (Knuckles)",
		};

		private static readonly List<string> actsEH = new()
		{
			"Act 1 - Egg Mobile ...Boss",
		};

		private static readonly List<string> actsEW = new()
		{
			"Act 1 - Militant Missionary Boss Egg Walker",
		};

		private static readonly List<string> actsEV = new()
		{
			"Act 1 - Militant Missionary Boss Egg Viper",
		};

		private static readonly List<string> actsZero = new()
		{
			"Act 1 - ZERO The Chase-master",
		};

		private static readonly List<string> actsSS = new()
		{
			"Act 1 - Hall",
			"Act 2 - Station",
			"Act 3 - Sewers",
			"Act 4 - Main",
			"Act 5 - Hostel (Pool)",
			"Act 6 - Twinkle Park Area",
		};

		private static readonly List<string> actsECOut = new()
		{
			"Act 1 - landing runway (UnTransformed)",
			"Act 2 - Main (Transformed)",
			"Act 3 - Bridge (Transformed)",
			"Act 4 - Captain Headquarters Room",
			"Act 5 - Hall",
			"Act 6 - Pool",
		};

		private static readonly List<string> actsECIn = new()
		{
			"Act 1 - landing runway (UnTransformed)",
			"Act 2 - Main (Transformed)",
			"Act 3 - Bridge (Transformed)",
			"Act 4 - Captain Headquarters Room",
			"Act 5 - Hall",
			"Act 6 - Pool",
		};

		private static readonly List<string> actsMR = new()
		{
			"Act 1 - Main",
			"Act 2 - Angel Island",
			"Act 3 - Jungle",
			"Act 4 - Final Egg",
		};

		private static readonly List<string> actsPast = new()
		{
			"Act 1 - Main",
			"Act 2 - Altar",
			"Act 3 - Altar (Fire)",
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
		#endregion
		#endregion
		#endregion

		#region Converter
		public class GameModeConverter : IValueConverter
		{
			public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
			{
				if (value is int key)
				{
					if (GameModeNames.ContainsKey(key))
					{
						List<int> keys = GameModeNames.Keys.ToList();
						return keys.IndexOf(key);
					}
					else
						return -1;
				}

				return Binding.DoNothing;
			}

			public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
			{
				if (value is int key)
				{
					if (key > -1)
					{
						List<int> keys = GameModeNames.Keys.ToList();
						return keys[key];
					}
					else
						return -1;
					
				}

				return Binding.DoNothing;
			}
		}
		#endregion

		public TestSpawn(GameSettings gameSettings, Dictionary<string, SADXModInfo> mods)
		{
			SpawnSettings = gameSettings.TestSpawn;
			GameMods = mods;
			InitializeComponent();

			Loaded += TestSpawn_Loaded;
		}

		#region Internal Functions
		private void TestSpawn_Loaded(object sender, RoutedEventArgs e)
		{
			// Setup Item Sources
			tsComboCharacter.ItemsSource = CharacterNames;
			tsComboLevel.ItemsSource = LevelNames;
			tsComboGameMode.ItemsSource = GameModeNames;
			tsComboGameMode.DisplayMemberPath = "Value";

			GetSaves();

			SetBindings();
		}

		private void btnTestSpawnLaunchGame_Click(object sender, RoutedEventArgs e)
		{
			if (string.IsNullOrEmpty(App.CurrentGame.gameDirectory))
			{
				new MessageWindow(Lang.GetString("MessageWindow.Errors.GamePathNotFound.Title"), Lang.GetString("MessageWindow.Errors.GamePathNotFound"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
				return;
			}

			string executablePath = GameSettings.EnabledMods.Select(item => GameMods[item].EXEFile).FirstOrDefault(item => !string.IsNullOrEmpty(item)) ?? Path.Combine(App.CurrentGame.gameDirectory, App.CurrentGame.exeName);

			string commandLine = GetTestSpawnCommandLine();

			ProcessStartInfo startInfo = new ProcessStartInfo(executablePath)
			{
				WorkingDirectory = App.CurrentGame.gameDirectory,
				Arguments = commandLine
			};

			Process.Start(startInfo);
		}

		private void tsComboLevel_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			ComboBox t = sender as ComboBox;

			tsComboAct.ItemsSource = GetNewAct(t.SelectedIndex);
			SpawnSettings.ActIndex = 0;
		}

		private void tsCheckCharacter_Checked(object sender, RoutedEventArgs e)
		{
			if (tsComboCharacter.SelectedIndex < 0)
				tsComboCharacter.SelectedIndex = 0;
		}

		private void tsCheckLevel_Checked(object sender, RoutedEventArgs e)
		{
			if (tsComboLevel.SelectedIndex < 0)
				tsComboLevel.SelectedIndex = 0;
			if (tsComboAct.SelectedIndex < 0)
				tsComboAct.SelectedIndex = 0;
			if (tsComboTime.SelectedIndex < 0)
				tsComboTime.SelectedIndex = 0;
		}

		private void tsCheckEvent_Click(object sender, RoutedEventArgs e)
		{
			if (IsEventChecked)
			{
				//IsCharacterChecked = true;

				TestSpawnGrid.RowDefinitions[3].Height = new GridLength(1, GridUnitType.Auto);
			}
		}

		private void tsCheckEvent_Unchecked(object sender, RoutedEventArgs e)
		{
			TestSpawnGrid.RowDefinitions[3].Height = new GridLength(0);
		}

		private void tsCheckManual_Clicked(object sender, RoutedEventArgs e)
		{
			if (SpawnSettings.UseManual)
			{
				tsNumCharacter.Value = SpawnSettings.CharacterIndex;
				tsNumLevel.Value = SpawnSettings.LevelIndex;
				tsNumAct.Value = SpawnSettings.ActIndex;

				IsCharacterChecked = false;
				IsLevelChecked = false;
			}
			else
			{
				if (SpawnSettings.CharacterIndex > -1)
					IsCharacterChecked = true;
				if (SpawnSettings.LevelIndex > -1)
					IsLevelChecked = true;
			}
		}

		private string GetTestSpawnCommandLine()
		{
			/*
			List<string> cmdline = new List<string>();

			bool advanced = tsCheckManual.IsChecked == true;

			if (IsLevelChecked)
				cmdline.Add("-l " + (advanced ? GameSettings.TestSpawn.LevelIndex.ToString()))

			if (tsCheckLevel.IsChecked.GetValueOrDefault() && tsComboLevel.SelectedIndex > -1)
				cmdline.Add("-l " + (advanced ? tsNumLevel.Value.ToString() : tsComboLevel.SelectedIndex.ToString()) + " -a " + (advanced ? tsNumAct.Value.ToString() : tsComboAct.SelectedIndex.ToString()));

			if (tsCheckCharacter.IsChecked == true && tsComboCharacter.SelectedIndex > -1)
				cmdline.Add("-c " + (advanced ? tsNumCharacter.Value.ToString() : tsComboCharacter.SelectedIndex.ToString()));

			if (tsCheckPosition.IsChecked == true)
				cmdline.Add("-p " + tsNumPosX.Value.ToString() + " " +
					tsNumPosY.Value.ToString() + " " +
					tsNumPosZ.Value.ToString() + " -r " +
					tsNumAngle.Value.ToString());

			if (tsCheckEvent.IsChecked == true && tsComboEvent.SelectedIndex > -1)
			{
				int ev = 0;
				int ev_result = 0;

				foreach (var item in TS.GetCutsceneList())
				{
					if (ev == tsComboEvent.SelectedIndex)
					{
						ev_result = item.Key;
						break;
					}
					ev++;
				}
				cmdline.Add("-e " + ev_result.ToString());
			}
			if (tsComboTime.SelectedIndex > 0)
				cmdline.Add("-t " + (tsComboTime.SelectedIndex - 1).ToString());

			if (tsCheckGameMode.IsChecked == true && tsComboGameMode.SelectedIndex > -1)
			{
				uint gm = 0;
				uint gm_result = 0;
				foreach (var item in TS.GetTestSpawnGameModeList())
				{
					if (gm == tsComboGameMode.SelectedIndex)
					{
						gm_result = item.Key;
						break;
					}
					gm++;
				}
				cmdline.Add("-g " + gm_result.ToString());
			}

			if (tsCheckSave.IsChecked == true)
			{
				string save = tsComboSave.SelectedValue.ToString();
				save = Util.GetSaveNumber(save);
				cmdline.Add("-s " + save);
			}

			return string.Join(" ", cmdline);
			*/
			return "";
		}
		#endregion

		#region Private Functions
		private void SetBindings()
		{
			IsCharacterChecked = (SpawnSettings.CharacterIndex > -1) ? true : false;
			tsCheckCharacter.SetBinding(CheckBox.IsCheckedProperty, new Binding("IsCharacterChecked")
			{
				Source = this,
				Mode = BindingMode.TwoWay
			});
			tsComboCharacter.SetBinding(ComboBox.SelectedIndexProperty, new Binding("CharacterIndex")
			{
				Source = SpawnSettings,
				Mode = BindingMode.TwoWay
			});
			IsLevelChecked = (SpawnSettings.CharacterIndex > -1) ? true : false;
			tsCheckLevel.SetBinding(CheckBox.IsCheckedProperty, new Binding("IsLevelChecked")
			{
				Source = this,
				Mode = BindingMode.TwoWay
			});
			tsComboLevel.SetBinding(ComboBox.SelectedIndexProperty, new Binding("LevelIndex")
			{
				Source = SpawnSettings,
				Mode = BindingMode.TwoWay
			});
			tsComboAct.SetBinding(ComboBox.SelectedIndexProperty, new Binding("ActIndex")
			{
				Source = SpawnSettings,
				Mode = BindingMode.TwoWay
			});
			IsEventChecked = (SpawnSettings.EventIndex > -1) ? true : false;
			tsCheckEvent.SetBinding(CheckBox.IsCheckedProperty, new Binding("IsEventChecked")
			{
				Source = this,
				Mode = BindingMode.TwoWay
			});
			tsComboEvent.SetBinding(ComboBox.SelectedIndexProperty, new Binding("EventIndex")
			{
				Source = SpawnSettings,
				Mode = BindingMode.TwoWay
			});
			IsGameModeChecked = (SpawnSettings.GameModeIndex > -1) ? true : false;
			tsCheckGameMode.SetBinding(CheckBox.IsCheckedProperty, new Binding("IsGameModeChecked")
			{
				Source = this,
				Mode = BindingMode.TwoWay
			});
			tsComboGameMode.SetBinding(ComboBox.SelectedIndexProperty, new Binding("GameModeIndex")
			{
				Source = SpawnSettings,
				Converter = new GameModeConverter(),
				ConverterParameter = GameModeNames,
				Mode = BindingMode.TwoWay
			});
			IsSaveChecked = (SpawnSettings.SaveIndex > -1) ? true : false;
			tsCheckSave.SetBinding(CheckBox.IsCheckedProperty, new Binding("IsSaveChecked")
			{
				Source = this,
				Mode = BindingMode.TwoWay
			});
			tsComboSave.SetBinding(ComboBox.SelectedIndexProperty, new Binding("SaveIndex")
			{
				Source = SpawnSettings,
				Mode = BindingMode.TwoWay
			});

			tsCheckManual.SetBinding(CheckBox.IsCheckedProperty, new Binding("UseManual")
			{
				Source = SpawnSettings,
				Mode = BindingMode.TwoWay
			});
			tsNumCharacter.Value = SpawnSettings.CharacterIndex;
			tsNumLevel.Value = SpawnSettings.LevelIndex;
			tsNumAct.Value = SpawnSettings.ActIndex;
			tsNumPosX.SetBinding(NumericUpDown.ValueProperty, new Binding("XPosition")
			{
				Source = SpawnSettings,
				Mode = BindingMode.TwoWay
			});
			tsNumPosY.SetBinding(NumericUpDown.ValueProperty, new Binding("YPosition")
			{
				Source = SpawnSettings,
				Mode = BindingMode.TwoWay
			});
			tsNumPosZ.SetBinding(NumericUpDown.ValueProperty, new Binding("ZPosition")
			{
				Source = SpawnSettings,
				Mode = BindingMode.TwoWay
			});
			tsNumAngle.SetBinding(NumericUpDown.ValueProperty, new Binding("Rotation")
			{
				Source = SpawnSettings,
				Mode = BindingMode.TwoWay
			});
		}

		private void GetSaves()
		{
			if (App.CurrentGame.loader.installed)
			{
				string fullPath = Path.Combine(App.CurrentGame.gameDirectory, "SAVEDATA");

				//if savedata exists
				if (Directory.Exists(fullPath))
				{
					string targetExtension = ".snc";

					string[] files = Directory.GetFiles(fullPath, "*" + targetExtension, SearchOption.TopDirectoryOnly);

					tsComboSave.BeginInit();

					List<string> list = new List<string>();

					//browse each save file of the user
					foreach (string file in files)
					{
						string name = Path.GetFileNameWithoutExtension(file);
						string nameDup = name.ToLower();

						if (nameDup.Contains("sonicdx")) //skip chao garden save
							list.Add(name);
					}

					//sort just in case the order is wrong
					list.Sort((x, y) => String.Compare(x, y, StringComparison.Ordinal));

					//finally, add all the saves in the comboBox

					tsComboSave.ItemsSource = list;

					tsComboSave.EndInit();
				}
			}
		}
		#endregion

		#region Public Functions
		public static List<string> GetNewAct(int index)
		{
			if (index > acts.Count || index < 0)
			{
				return actsRegular;
			}

			return acts[index];
		}

		public void Save(GameSettings settings)
		{
			settings.TestSpawn = SpawnSettings;
		}
		#endregion
	}
}
