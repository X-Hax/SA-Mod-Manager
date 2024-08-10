using SAModManager.Configuration.SA2;
using SAModManager.UI;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;


namespace SAModManager.Controls.SA2
{
    /// <summary>
    /// Interaction logic for TestSpawn.xaml
    /// </summary>
    public partial class TestSpawn : UserControl
    {
        #region Variables
        private GameSettings GameProfile;
        public static Dictionary<int, string> EventNames;
        public static Dictionary<int, string> LevelNames { get; set; } = new();

        #region ComboBox Sources

        public static readonly List<string> CharacterNames = new()
        {
            "Sonic",
            "Shadow",
            "Tails",
            "Eggman",
            "Knuckles",
            "Rouge",
            "Mech Tails",
            "Mech Eggman",
            "Amy",
            "Metal Sonic",
            "Chao Walker",
            "Dark Chao Walker",
            "Tikal",
            "Chaos"
        };

        public static readonly List<string> Missions = new()
        {
            "Mission 1",
            "Mission 2",
            "Mission 3",
            "Mission 4",
            "Mission 5",
        };


        #endregion
        #endregion

        #region Converter

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

        public TestSpawn(ref object gameSettings)
        {
            GameProfile = (gameSettings as GameSettings);

            InitializeComponent();

            Loaded += TestSpawn_Loaded;
        }

        #region Internal Functions
        private void TestSpawn_Loaded(object sender, RoutedEventArgs e)
        {
            // Setup Item Sources
            GetSaves();
            InitTestSpawnLevelList();
            InitCutsceneList();
            SetBindings();


            tsComboEvent.SelectedIndex = EventNames.Keys.ToList<int>().IndexOf(GameProfile.TestSpawn.EventIndex);
            if (GameProfile.TestSpawn.UseManual || GameProfile.TestSpawn.UsePosition)
                expandAdvanced.IsExpanded = true;
        }

        private void btnTestSpawnLaunchGame_Click(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrEmpty(App.CurrentGame.gameDirectory))
            {
                new MessageWindow(Lang.GetString("MessageWindow.Errors.GamePathFailed.Title"), Lang.GetString("MessageWindow.Errors.GamePathFailed"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Error, MessageWindow.Buttons.OK).ShowDialog();
                return;
            }
            var mainWindow = ((MainWindow)Application.Current.MainWindow);

            string executablePath = mainWindow.EnabledMods?.Select(item => mainWindow.mods[item]?.EXEFile).FirstOrDefault(item => !string.IsNullOrEmpty(item)) ?? Path.Combine(App.CurrentGame.gameDirectory, App.CurrentGame.exeName);
          
            string commandLine = GetTestSpawnCommandLine();
            string folderPath = Path.GetDirectoryName(executablePath);

            ProcessStartInfo startInfo = new(executablePath)
            {
                WorkingDirectory = folderPath,
                Arguments = commandLine,
                UseShellExecute = true,
            };

            Process process = Process.Start(startInfo);
            try
            {
                process?.WaitForInputIdle(10000);
            } catch { }
        }

        private void tsComboLevel_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

            tsComboMission.SelectedIndex = 0;
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
                case "tsCheckPlayer2":
                    if (tsComboPlayer2.SelectedIndex < 0)
                        tsComboPlayer2.SelectedIndex = 0;

                    break;
                case "tsCheckLevel":
                    if (tsComboLevel.SelectedIndex < 0)
                        tsComboLevel.SelectedIndex = 0;

                    GameProfile.TestSpawn.UseCharacter = true;
                    break;
                case "tsCheckEvent":
                    if (tsComboEvent.SelectedIndex < 0)
                        tsComboEvent.SelectedIndex = 0;
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
                if (tsCheckCharacter.IsEnabled)
                    tsNumCharacter.Value = tsComboCharacter.SelectedIndex;
                if (tsCheckLevel.IsEnabled)
                    tsNumLevel.Value = tsComboLevel.SelectedIndex;
                if (tsComboMission.SelectedIndex > -1)
                    tsNumMission.Value = tsComboMission.SelectedIndex;

                tsCheckCharacter.IsChecked = false;
                tsCheckLevel.IsChecked = false;
                tsCheckPlayer2.IsChecked = false;
                tsCheckEvent.IsChecked = false;
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
            tsCheckPlayer2.SetBinding(CheckBox.IsCheckedProperty, new Binding("UsePlayer2")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsComboPlayer2.ItemsSource = CharacterNames;
            tsComboPlayer2.SetBinding(ComboBox.SelectedIndexProperty, new Binding("Player2Index")
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
            tsComboMission.SetBinding(ComboBox.SelectedIndexProperty, new Binding("MissionIndex")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsComboMission.ItemsSource = Missions;
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
            tsCheckManual.SetBinding(CheckBox.IsCheckedProperty, new Binding("UseManual")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsNumCharacter.SetBinding(NumberBox.ValueProperty, new Binding("CharacterIndex")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsNumLevel.SetBinding(NumberBox.ValueProperty, new Binding("LevelIndex")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsNumMission.SetBinding(NumberBox.ValueProperty, new Binding("MissionIndex")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsCheckPosition.SetBinding(CheckBox.IsCheckedProperty, new Binding("UsePosition")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsNumPosX.SetBinding(NumberBox.ValueProperty, new Binding("XPosition")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsNumPosY.SetBinding(NumberBox.ValueProperty, new Binding("YPosition")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsNumPosZ.SetBinding(NumberBox.ValueProperty, new Binding("ZPosition")
            {
                Source = GameProfile.TestSpawn,
                Mode = BindingMode.TwoWay
            });
            tsNumAngle.SetBinding(NumberBox.ValueProperty, new Binding("Rotation")
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
                string fullPath = Path.GetFullPath(Path.Combine(App.CurrentGame.gameDirectory, "resource\\gd_PC\\SAVEDATA"));

                //if savedata exists
                if (Directory.Exists(fullPath))
                {

                    string[] files = Directory.GetFiles(fullPath, "*", SearchOption.TopDirectoryOnly);

                    tsComboSave.BeginInit();

                    List<string> list = new();

                    //browse each save file of the user
                    foreach (string file in files)
                    {
                        string name = Path.GetFileNameWithoutExtension(file);
                        string nameDup = name.ToLower();

                        if (!nameDup.Contains("alf")) //skip chao garden save
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

        private static void InitTestSpawnLevelList()
        {
            LevelNames = new()
            {
                { 0, "Basic Test" },
                { 3, "Green Forest" },
                { 4, "White Jungle" },
                { 5, "Pumpkin Hill" },
                { 6, "Sky Rail" },
                { 7, "Aquatic Mine" },
                { 8, "Security Hall" },
                { 9, "Prison Lane" },
                { 10, "Metal Harbor" },
                { 11, "Iron Gate" },
                { 12, "Weapons Bed" },
                { 13, "City Escape" },
                { 14, "Radical Highway" },
                { 15, "Weapons Bed 2P" },
                { 16, "Wild Canyon" },
                { 17, "Mission Street" },
                { 18, "Dry Lagoon" },
                { 19, "Sonic Vs. Shadow 1" },
                { 20, "Tails Vs. Eggman 1" },
                { 21, "Sand Ocean" },
                { 22, "Crazy Gadget" },
                { 23, "Hidden Base" },
                { 24, "Eternal Engine" },
                { 25, "Death Chamber" },
                { 26, "Egg Quarters" },
                { 27, "Lost Colony" },
                { 28, "Pyramid Cave" },
                { 29, "Tails Vs. Eggman 2" },
                { 30, "Final Rush" },
                { 31, "Green Hill" },
                { 32, "Meteor Herd" },
                { 33, "Knuckles Vs. Rouge" },
                { 34, "Cannon's Core (Sonic)" },
                { 35, "Cannon's Core (Eggman)" },
                { 36, "Cannon's Core (Tails)" },
                { 37, "Cannon's Core (Rouge)" },
                { 38, "Cannon's Core (Knuckles)" },
                { 39, "Mission Street 2P" },
                { 40, "Final Chase" },
                { 41, "Wild Canyon 2P" },
                { 42, "Sonic Vs. Shadow 2" },
                { 43, "Cosmic Wall" },
                { 44, "Mad Space" },
                { 45, "Sand Ocean 2P" },
                { 46, "Dry Lagoon 2P" },
                { 47, "Pyramid Race" },
                { 48, "Hidden Base 2P" },
                { 49, "Pool Quest" },
                { 50, "Planet Quest" },
                { 51, "Deck Race" },
                { 52, "Downtown Race" },
                { 53, "Cosmic Wall 2P" },
                { 54, "Grind Race" },
                { 55, "Lost Colony 2P" },
                { 56, "Eternal Engine 2P" },
                { 57, "Metal Harbor 2P" },
                { 58, "Iron Gate 2P" },
                { 59, "Death Chamber 2P" },
                { 60, "Big Foot" },
                { 61, "Hot Shot" },
                { 62, "Flying Dog" },
                { 63, "King Boom Boo" },
                { 64, "Egg Golem (Sonic)" },
                { 65, "Biolizard" },
                { 66, "FinalHazard" },
                { 67, "Egg Golem (Eggman)" },
                { 70, "Route 101/Route 280" },
                { 71, "Kart Race" },
                { 90, "Chao World" }
            };

            //display level ID after the name.
            foreach (var pair in LevelNames.ToList())
            {
                // Update the value
                string updatedValue = pair.Value + " (" + pair.Key + ")";
                LevelNames.Remove(pair.Key);
                LevelNames.Add(pair.Key, updatedValue);
            }
        }

        private static void InitCutsceneList()
        {

            EventNames = new Dictionary<int, string>
            {
				{ 0, Lang.GetString("SA2EventName00") },
				{ 2, Lang.GetString("SA2EventName01") },
				{ 3, Lang.GetString("SA2EventName02") },
				{ 4, Lang.GetString("SA2EventName03") },
				{ 5, Lang.GetString("SA2EventName04") },
				{ 6, Lang.GetString("SA2EventName05") },
				{ 11, Lang.GetString("SA2EventName06") },
				{ 14, Lang.GetString("SA2EventName07") },
				{ 15, Lang.GetString("SA2EventName08") },
				{ 16, Lang.GetString("SA2EventName09") },
				{ 17, Lang.GetString("SA2EventName10") },
				{ 19, Lang.GetString("SA2EventName11") },
				{ 21, Lang.GetString("SA2EventName12") },
				{ 22, Lang.GetString("SA2EventName13") },
				{ 24, Lang.GetString("SA2EventName14") },
				{ 25, Lang.GetString("SA2EventName15") },
				{ 26, Lang.GetString("SA2EventName16") },
				{ 27, Lang.GetString("SA2EventName17") },
				{ 28, Lang.GetString("SA2EventName18") },
				{ 100, Lang.GetString("SA2EventName19") },
				{ 101, Lang.GetString("SA2EventName20") },
				{ 102, Lang.GetString("SA2EventName21") },
				{ 103, Lang.GetString("SA2EventName22") },
				{ 105, Lang.GetString("SA2EventName23") },
				{ 106, Lang.GetString("SA2EventName24") },
				{ 107, Lang.GetString("SA2EventName25") },
				{ 109, Lang.GetString("SA2EventName26") },
				{ 111, Lang.GetString("SA2EventName27") },
				{ 112, Lang.GetString("SA2EventName28") },
				{ 113, Lang.GetString("SA2EventName29") },
				{ 116, Lang.GetString("SA2EventName30") },
				{ 118, Lang.GetString("SA2EventName31") },
				{ 119, Lang.GetString("SA2EventName32") },
				{ 120, Lang.GetString("SA2EventName33") },
				{ 122, Lang.GetString("SA2EventName34") },
				{ 123, Lang.GetString("SA2EventName35") },
				{ 124, Lang.GetString("SA2EventName36") },
				{ 126, Lang.GetString("SA2EventName37") },
				{ 127, Lang.GetString("SA2EventName38") },
				{ 128, Lang.GetString("SA2EventName39") },
				{ 129, Lang.GetString("SA2EventName40") },
				{ 130, Lang.GetString("SA2EventName41") },
				{ 131, Lang.GetString("SA2EventName42") },
				{ 200, Lang.GetString("SA2EventName43") },
				{ 201, Lang.GetString("SA2EventName44") },
				{ 203, Lang.GetString("SA2EventName45") },
				{ 204, Lang.GetString("SA2EventName46") },
				{ 205, Lang.GetString("SA2EventName47") },
				{ 206, Lang.GetString("SA2EventName48") },
				{ 207, Lang.GetString("SA2EventName49") },
				{ 208, Lang.GetString("SA2EventName50") },
				{ 210, Lang.GetString("SA2EventName51") },
				{ 211, Lang.GetString("SA2EventName52") },
				{ 360, Lang.GetString("SA2EventName53") },
				{ 401, Lang.GetString("SA2EventName54") },
				{ 409, Lang.GetString("SA2EventName55") },
				{ 411, Lang.GetString("SA2EventName56") },
				{ 420, Lang.GetString("SA2EventName57") },
				{ 428, Lang.GetString("SA2EventName58") },
				{ 429, Lang.GetString("SA2EventName59") },
				{ 430, Lang.GetString("SA2EventName60") },
				{ 524, Lang.GetString("SA2EventName61") },
				{ 532, Lang.GetString("SA2EventName62") },
				{ 602, Lang.GetString("SA2EventName63") },
				{ 609, Lang.GetString("SA2EventName64") }
			};


            //display event ID after their name.
            foreach (var pair in EventNames.ToList())
            {
                string displayName = "EV" + pair.Key.ToString("D4") + ": " + pair.Value;
                EventNames.Remove(pair.Key);
                EventNames.Add(pair.Key, displayName);
            }
        }

        private static void AdjustCharIndex(ref int index)
        {
            if (index >= 9 && index < CharacterNames.Count) //super sonic / shadow and big aren't in the list, we adjust the offset from the ComboBox
            {
                index += 3;
            }
        }

        private string GetTestSpawnCommandLine()
        {
            List<string> cmdline = new();

            if (GameProfile.TestSpawn.CharacterIndex > -1)
                if (GameProfile.TestSpawn.UseCharacter || GameProfile.TestSpawn.UseManual)
                {
                    int charIndex = GameProfile.TestSpawn.CharacterIndex;
                    AdjustCharIndex(ref charIndex);
                    cmdline.Add($"-c {charIndex}");
                }
           
            if (GameProfile.TestSpawn.Player2Index > -1 && GameProfile.TestSpawn.UsePlayer2)
            {
                int charIndex = GameProfile.TestSpawn.Player2Index;
                AdjustCharIndex(ref charIndex);
                cmdline.Add($"-p2 {charIndex}");
            }

            if (GameProfile.TestSpawn.LevelIndex > -1 && GameProfile.TestSpawn.UseEvent == false)
            {
                int lvl_result = GameProfile.TestSpawn.LevelIndex;
                if (tsCheckManual.IsChecked == false)
                {
                    int currentIndex = 0;

                    foreach (var item in LevelNames)
                    {
                        if (currentIndex == lvl_result)
                        {
                            lvl_result = item.Key;
                            break;
                        }
                        currentIndex++;
                    }
                }

                cmdline.Add($"-l {lvl_result}");
            }

            if (GameProfile.TestSpawn.MissionIndex > -1)
                if (GameProfile.TestSpawn.UseLevel || GameProfile.TestSpawn.UseManual)
                    cmdline.Add($"-m {GameProfile.TestSpawn.MissionIndex}");

			if (GameProfile.TestSpawn.UsePosition)
				cmdline.Add($"-p {GameProfile.TestSpawn.XPosition} {GameProfile.TestSpawn.YPosition} {GameProfile.TestSpawn.ZPosition} -r {GameProfile.TestSpawn.Rotation}");

			if (GameProfile.TestSpawn.UseEvent && GameProfile.TestSpawn.EventIndex > -1)
                cmdline.Add($"-e {GameProfile.TestSpawn.EventIndex}");


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
