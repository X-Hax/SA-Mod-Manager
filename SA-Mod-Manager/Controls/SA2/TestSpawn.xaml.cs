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
        private Dictionary<string, SAModInfo> GameMods;
        private List<string> SelectedMods;
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
            "Super Sonic",
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

        public TestSpawn(ref object gameSettings, Dictionary<string, SAModInfo> modList, List<string> enabledMods)
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
                tsNumCharacter.Value = GameProfile.TestSpawn.CharacterIndex;
                tsNumLevel.Value = GameProfile.TestSpawn.LevelIndex;
                tsNumMission.Value = GameProfile.TestSpawn.MissionIndex;

                tsCheckCharacter.IsChecked = false;
                tsCheckLevel.IsChecked = false;
                tsCheckPlayer2.IsChecked = false;
            }
            else
            {
                if (GameProfile.TestSpawn.CharacterIndex > -1)
                    tsCheckCharacter.IsChecked = true;
                if (GameProfile.TestSpawn.LevelIndex > -1)
                    tsCheckLevel.IsChecked = true;
                if (GameProfile.TestSpawn.Player2Index > -1)
                    tsCheckPlayer2.IsChecked = true;
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
            tsNumMission.SetBinding(NumericUpDown.ValueProperty, new Binding("MissionIndex")
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

        private void InitTestSpawnLevelList()
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
        }

        private void InitCutsceneList()
        {

            EventNames = new Dictionary<int, string>
            {
                { 0, "Hero Story Intro" },
                { 2, "Sonic Encounters Shadow" },
                { 3, "Knuckles Meets Rouge" },
                { 4, "Tails Flies to Prison Island" },
                { 5, "Tails vs. Eggman Aftermath" },
                { 6, "Sonic Escapes from his Cell" },
                { 11, "Sonic and Friends Witness the ARK's Power" },
                { 14, "Where's Eggman?" },
                { 15, "Sonic And Tails intercept Eggman's Video." },
                { 16, "Locate Eggman's Pyramid Base" },
                { 17, "Knuckles Begins Looking for the Gate Keys" },
                { 19, "Eggman Ambushes Sonic inside the Base." },
                { 21, "Tails Explains His Plan to Destroy the Eclipse Cannon" },
                { 22, "Sonic Receives a Call for Ransom from Eggman" },
                { 24, "Knuckles vs. Rouge Aftermath" },
                { 25, "Sonic and Eggman Face Off" },
                { 26, "Sonic Saves Himself" },
                { 27, "Sonic and Shadow Final Battle" },
                { 28, "Hero Story Ending" },
                { 100, "Dark Story Intro" },
                { 101, "Eggman Awakens Shadow" },
                { 102, "Shadow Instructs Eggman" },
                { 103, "Rouge Meets Knuckles" },
                { 105, "Eggman Hears of Shadow's Highway Robbery" },
                { 106, "Shadow's Pathos" },
                { 107, "Shadow Encounters Sonic" },
                { 109, "Rouge Follows Eggman" },
                { 111, "Gathering of the Dark" },
                { 112, "Arrive on Prison Island" },
                { 113, "Eggman's Plan is Complicated by Tails and Amy" },
                { 116, "Shadow's Conscience Begins to Surface" },
                { 118, "Sonic vs. Shadow 1 Aftermath" },
                { 119, "Shadow's Past Life with Maria" },
                { 120, "Eggman Reworks His Plan" },
                { 122, "Shadow Spots the Heroes approaching the Pyramid Base" },
                { 123, "Eggman Fails to Ambush Sonic inside the base." },
                { 124, "Rouge Learns the Truth About Project Shadow." },
                { 126, "Knuckles vs. Rouge Aftermath" },
                { 127, "Eggman's Final Plan to Crush the Heroes" },
                { 128, "Eggman Sets a Diabolical Trap for Sonic" },
                { 129, "Mystery of Shadow the Hedgehog" },
                { 130, "Shadow and Sonic Final Battle" },
                { 131, "Dark Story Ending" },
                { 200, "Last Episode Intro" },
                { 201, "The ARK Falls" },
                { 203, "Gerald's Diary and Descent into Madness" },
                { 204, "Shadow and Amy." },
                { 205, "Gerald's Final Trump Card" },
                { 206, "Knuckles Stops the Chaos Emeralds" },
                { 207, "Sonic and Shadow Transform" },
                { 208, "Shadow gives his life to save the world" },
                { 210, "Last Episode Ending/Credits" },
                { 211, "Epilogue" },
                { 360, "SA2B Intro" },
                { 401, "SA2 Intro (FMV)" },
                { 409, "Prison Island Explodes" },
                { 411, "Eggman's Threat to Earth" },
                { 420, "Sonic and Friends go to Space (Hero)" },
                { 428, "The Eclipse Cannon Malfunctions" },
                { 429, "Dark Story Trailer" },
                { 430, "Last Story Trailer" },
                { 524, "Sonic and Friends go to Space (Dark)" },
                { 532, "Hero Story Trailer" },
                { 602, "Gerald's Revenge" },
                { 609, "The ARK is Neutralized" }
            };
        }


        private string GetTestSpawnCommandLine()
        {
            List<string> cmdline = new();

            if (GameProfile.TestSpawn.CharacterIndex > -1)
                if (GameProfile.TestSpawn.UseCharacter || GameProfile.TestSpawn.UseManual)
                    cmdline.Add("-c " + GameProfile.TestSpawn.CharacterIndex.ToString());

            if (GameProfile.TestSpawn.Player2Index > -1)
                cmdline.Add("-p2 " + GameProfile.TestSpawn.Player2Index.ToString());

            if (GameProfile.TestSpawn.LevelIndex > -1)
            {
                if (GameProfile.TestSpawn.UseLevel || GameProfile.TestSpawn.UseManual)
                {
                    int lvl = 0;
                    int lvl_result = 0;
                    foreach (var item in LevelNames)
                    {
                        if (lvl == GameProfile.TestSpawn.LevelIndex)
                        {
                            lvl_result = item.Key;
                            break;
                        }
                        lvl++;
                    }
                    cmdline.Add("-l " + lvl_result.ToString());
                }
            }

            if (GameProfile.TestSpawn.MissionIndex > -1)
                if (GameProfile.TestSpawn.UseLevel || GameProfile.TestSpawn.UseManual)
                    cmdline.Add("-m " + GameProfile.TestSpawn.MissionIndex.ToString());


            if (GameProfile.TestSpawn.UsePosition)
                cmdline.Add("-p " +
                    GameProfile.TestSpawn.XPosition.ToString() + " " +
                    GameProfile.TestSpawn.YPosition.ToString() + " " +
                    GameProfile.TestSpawn.ZPosition.ToString() + " -r " +
                    GameProfile.TestSpawn.Rotation.ToString());

            if (GameProfile.TestSpawn.UseEvent && GameProfile.TestSpawn.EventIndex > -1)
                cmdline.Add("-e " + GameProfile.TestSpawn.EventIndex.ToString());


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
