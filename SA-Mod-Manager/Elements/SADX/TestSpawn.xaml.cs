using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using SAModManager.Common;
using SAModManager.Configuration.SADX;
using SAModManager.Ini;

namespace SAModManager.Elements.SADX
{
    /// <summary>
    /// Interaction logic for TestSpawn.xaml
    /// </summary>
    public partial class TestSpawn : UserControl
    {
        #region Variables
        private GameSettings GameProfile;
        private Dictionary<string, SADXModInfo> GameMods;
        private List<string> SelectedMods;
        public static Dictionary<int, string> EventNames;

        #region ComboBox Sources
        public static readonly List<string> CharacterNames = new()
        {
            "Sonic",
            "Eggman (unused)",
            "Tails",
            "Knuckles",
            "Tikal (unused)",
            "Amy",
            "E-102 Gamma",
            "Big",
            "Metal Sonic"
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
            "Act 1 - City Hall Area",
            "Act 2 - Station",
            "Act 3 - Sewers",
            "Act 4 - Station Area",
            "Act 5 - Hotel (Pool)",
            "Act 6 - Twinkle Park Area",
        };

        private static readonly List<string> actsECOut = new()
        {
            "Act 1 - Landing Runway (UnTransformed)",
            "Act 2 - Bridge (Transformed)",
            "Act 3 - Deck (Transformed)",
            "Act 4 - Captain Headquarters Room",
            "Act 5 - Private Room",
            "Act 6 - Pool",
        };

        private static readonly List<string> actsECIn = new()
        {
            "Act 1 - Arsenal",
            "Act 2 - Main",
            "Act 3 - Hedgehog Hammer",
            "Act 4 - Prison Hall",
            "Act 5 - Water Tank",
            "Act 6 - Warp Hall",
        };

        private static readonly List<string> actsMR = new()
        {
            "Act 1 - Station",
            "Act 2 - Angel Island",
            "Act 3 - Jungle",
            "Act 4 - Final Egg",
        };

        private static readonly List<string> actsPast = new()
        {
            "Act 1 - Shrine (Main)",
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
                        return GameModeNames.Keys.ToList().IndexOf(key);
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
                        return GameModeNames.Keys.ToList()[key];
                    else
                        return -1;

                }

                return Binding.DoNothing;
            }
        }

        public class EventIndexConverter : IValueConverter
        {
            public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
            {
                if (value is int key)
                {
                    if (EventNames.ContainsKey(key))
                        return EventNames.Keys.ToList().IndexOf(key);
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
                        return EventNames.Keys.ToList()[key];
                    else
                        return -1;

                }

                return Binding.DoNothing;
            }
        }
        #endregion

        public TestSpawn(ref object gameSettings, Dictionary<string, SADXModInfo> modList, List<string> enabledMods)
        {
            GameProfile = (gameSettings as GameSettings);
            SelectedMods = (gameSettings as GameSettings).EnabledMods;
            GameMods = modList;

            InitializeComponent();

            Loaded += TestSpawn_Loaded;
        }

        #region Internal Functions
        private void TestSpawn_Loaded(object sender, RoutedEventArgs e)
        {
            // Setup Item Sources
            GetSaves();
            InitCutsceneList();
            tsComboTime.ItemsSource = TimeDay;
            tsComboTime.SelectedIndex = 0;
            SetBindings();

            tsComboEvent.SelectedIndex = EventNames.Keys.ToList<int>().IndexOf(GameProfile.TestSpawn.EventIndex);
            if (GameProfile.TestSpawn.UseManual || GameProfile.TestSpawn.UsePosition)
                expandAdvanced.IsExpanded = true;
        }

        private void btnTestSpawnLaunchGame_Click(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrEmpty(App.CurrentGame.gameDirectory))
            {
                new MessageWindow(Lang.GetString("MessageWindow.Errors.GamePathNotFound.Title"), Lang.GetString("MessageWindow.Errors.GamePathNotFound"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
                return;
            }

			string executablePath = SelectedMods?.Select(item => GameMods[item].EXEFile).FirstOrDefault(item => !string.IsNullOrEmpty(item)) ?? Path.Combine(App.CurrentGame.gameDirectory, App.CurrentGame.exeName);

            string commandLine = GetTestSpawnCommandLine();

            ProcessStartInfo startInfo = new(executablePath)
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
            tsComboAct.SelectedIndex = 0;
        }

        private void CheckBox_Checked(object sender, RoutedEventArgs e)
        {
            CheckBox box = sender as CheckBox;
            switch (box.Name)
            {
                case "tsCheckCharacter":
                    if (tsComboCharacter.SelectedIndex < 0)
                        tsComboCharacter.SelectedIndex = 0;
                    break;
                case "tsCheckLevel":
                    if (tsComboLevel.SelectedIndex < 0)
                        tsComboLevel.SelectedIndex = 0;
                    if (tsComboAct.SelectedIndex < 0)
                        tsComboAct.SelectedIndex = 0;
                    if (tsComboTime.SelectedIndex < 0)
                        tsComboTime.SelectedIndex = 0;

                    GameProfile.TestSpawn.UseCharacter = true;
                    break;
                case "tsCheckEvent":
                    if (tsComboEvent.SelectedIndex < 0)
                        tsComboEvent.SelectedIndex = 0;
                    break;
                case "tsCheckGameMode":
                    if (tsComboGameMode.SelectedIndex < 0)
                        tsComboGameMode.SelectedIndex = 0;
                    break;
                case "tsCheckSave":
                    if (tsComboSave.SelectedIndex < 0)
                        tsComboSave.SelectedIndex = 0;
                    break;
            }
        }

        private void tsCheckEvent_Click(object sender, RoutedEventArgs e)
        {
            if (GameProfile.TestSpawn.UseEvent)
                TestSpawnGrid.RowDefinitions[3].Height = new GridLength(1, GridUnitType.Auto);
            else
                TestSpawnGrid.RowDefinitions[3].Height = new GridLength(0);
        }

        private void tsCheckManual_Clicked(object sender, RoutedEventArgs e)
        {
            if (GameProfile.TestSpawn.UseManual)
            {
                tsNumCharacter.Value = GameProfile.TestSpawn.CharacterIndex;
                tsNumLevel.Value = GameProfile.TestSpawn.LevelIndex;
                tsNumAct.Value = GameProfile.TestSpawn.ActIndex;

                tsCheckCharacter.IsChecked = false;
                tsCheckLevel.IsChecked = false;
            }
            else
            {
                if (GameProfile.TestSpawn.CharacterIndex > -1)
                    tsCheckCharacter.IsChecked = true;
                if (GameProfile.TestSpawn.LevelIndex > -1)
                    tsCheckLevel.IsChecked = true;
            }
        }
        #endregion

        #region Private Functions
        private void SetBindings()
        {
            tsCheckCharacter.SetBinding(CheckBox.IsCheckedProperty, new Binding("UseCharacter")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsComboCharacter.ItemsSource = CharacterNames;
            tsComboCharacter.SetBinding(ComboBox.SelectedIndexProperty, new Binding("CharacterIndex")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsCheckLevel.SetBinding(CheckBox.IsCheckedProperty, new Binding("UseLevel")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsComboLevel.SetBinding(ComboBox.SelectedIndexProperty, new Binding("LevelIndex")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsComboLevel.ItemsSource = LevelNames;
            tsComboAct.SetBinding(ComboBox.SelectedIndexProperty, new Binding("ActIndex")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsCheckEvent.SetBinding(CheckBox.IsCheckedProperty, new Binding("UseEvent")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsComboEvent.SetBinding(ComboBox.SelectedIndexProperty, new Binding("EventIndex")
            {
                Source = GameProfile.TestSpawn,
                Converter = new EventIndexConverter(),
                Mode = BindingMode.TwoWay
            });
            tsCheckGameMode.SetBinding(CheckBox.IsCheckedProperty, new Binding("UseGameMode")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsComboGameMode.SetBinding(ComboBox.SelectedIndexProperty, new Binding("GameModeIndex")
            {
                Source = GameProfile.TestSpawn,
                Converter = new GameModeConverter(),
                ConverterParameter = GameModeNames,
                Mode = BindingMode.TwoWay
            });
            tsComboGameMode.ItemsSource = GameModeNames;
            tsComboGameMode.DisplayMemberPath = "Value";
            tsCheckSave.SetBinding(CheckBox.IsCheckedProperty, new Binding("UseSave")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsComboSave.SetBinding(ComboBox.SelectedIndexProperty, new Binding("SaveIndex")
            {
                Source = GameProfile.TestSpawn,
                Converter = new IndexOffsetConverter(),
                Mode = BindingMode.TwoWay
            });
            tsComboEvent.ItemsSource = EventNames;
            tsComboEvent.DisplayMemberPath = "Value";
			tsVoiceLanguage.SetBinding(ComboBox.SelectedIndexProperty, new Binding("GameVoiceLanguage")
			{
				Source = GameProfile.TestSpawn,
				Mode = BindingMode.TwoWay
			});
			tsTextLanguage.SetBinding(ComboBox.SelectedIndexProperty, new Binding("GameTextLanguage")
			{
				Source = GameProfile.TestSpawn,
				Mode = BindingMode.TwoWay
			});
            tsCheckManual.SetBinding(CheckBox.IsCheckedProperty, new Binding("UseManual")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsNumCharacter.SetBinding(NumericUpDown.ValueProperty, new Binding("CharacterIndex")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsNumLevel.SetBinding(NumericUpDown.ValueProperty, new Binding("LevelIndex")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsNumAct.SetBinding(NumericUpDown.ValueProperty, new Binding("ActIndex")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
			tsCheckPosition.SetBinding(CheckBox.IsCheckedProperty, new Binding("UsePosition")
			{
				Source = GameProfile.TestSpawn,
				Mode = BindingMode.TwoWay
			});
			tsNumPosX.SetBinding(NumericUpDown.ValueProperty, new Binding("XPosition")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsNumPosY.SetBinding(NumericUpDown.ValueProperty, new Binding("YPosition")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsNumPosZ.SetBinding(NumericUpDown.ValueProperty, new Binding("ZPosition")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsNumAngle.SetBinding(NumericUpDown.ValueProperty, new Binding("Rotation")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
        }

        public static string GetSaveNumber(string s)
        {
            string number = s[^2..];
        
            if (int.TryParse(number, out int result))
            {
                // The last two characters are numbers
                if (number[0] == '0')
                {
                    number = number[1..];
                }
  
            }

            return result > 0 ? number : s;
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

                        if (!nameDup.Contains("chao")) //skip chao garden save
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

        private void InitCutsceneList()
        {

            EventNames = new Dictionary<int, string>();

            for (int i = 0; i < 385; i++)
            {
                string cutsceneName = Lang.GetString("TSCutscene" + i.ToString());

                if (cutsceneName.Contains("TSCutscene") || string.IsNullOrEmpty(cutsceneName))
                    continue;

                string displayName = "EV" + i.ToString("X4") + ": " + cutsceneName;

                EventNames.Add(i, displayName);
            }
        }

        private static List<string> GetNewAct(int index)
        {
            if (index > acts.Count || index < 0)
            {

                return actsRegular;
            }

            return acts[index];
        }

        private string GetTestSpawnCommandLine()
        {
            List<string> cmdline = new List<string>();

            if (GameProfile.TestSpawn.CharacterIndex > -1)
                if (GameProfile.TestSpawn.UseCharacter || GameProfile.TestSpawn.UseManual)
                    cmdline.Add("-c " + GameProfile.TestSpawn.CharacterIndex.ToString());

            if (GameProfile.TestSpawn.LevelIndex > -1)
                if (GameProfile.TestSpawn.UseLevel || GameProfile.TestSpawn.UseManual)
                    cmdline.Add("-l " + GameProfile.TestSpawn.LevelIndex.ToString());

            if (GameProfile.TestSpawn.ActIndex > -1)
                if (GameProfile.TestSpawn.UseLevel || GameProfile.TestSpawn.UseManual)
                    cmdline.Add("-a " + GameProfile.TestSpawn.ActIndex.ToString());

            if (GameProfile.TestSpawn.UseLevel && tsComboTime.SelectedIndex > 0)
                cmdline.Add("-t " + (tsComboTime.SelectedIndex - 1).ToString());

            if (GameProfile.TestSpawn.UsePosition)
                cmdline.Add("-p " +
                    GameProfile.TestSpawn.XPosition.ToString() + " " +
                    GameProfile.TestSpawn.YPosition.ToString() + " " +
                    GameProfile.TestSpawn.ZPosition.ToString() + " -r " +
                    GameProfile.TestSpawn.Rotation.ToString());

            if (GameProfile.TestSpawn.UseEvent && GameProfile.TestSpawn.EventIndex > -1)
                cmdline.Add("-e " + GameProfile.TestSpawn.EventIndex.ToString());

            if (GameProfile.TestSpawn.UseGameMode && GameProfile.TestSpawn.GameModeIndex > -1)
                cmdline.Add("-g " + GameProfile.TestSpawn.GameModeIndex.ToString());

            if (GameProfile.TestSpawn.UseSave && GameProfile.TestSpawn.SaveIndex > -1)
            {
                string save = tsComboSave.SelectedValue.ToString();
                save = GetSaveNumber(save);
                cmdline.Add("-s " + "\"" + save + "\"");
            }


            return string.Join(" ", cmdline);
        }

        #endregion
    }
}
