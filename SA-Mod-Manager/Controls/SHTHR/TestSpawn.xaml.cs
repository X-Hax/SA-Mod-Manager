using SAModManager.Configuration.SHTHR;
using SAModManager.UI;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;


namespace SAModManager.Controls.SHTHR
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
            InitTestSpawnLevelList();
            InitCutsceneList();
            SetBindings();

            tsComboEvent.SelectedIndex = EventNames.Keys.ToList<int>().IndexOf(GameProfile.TestSpawn.EventIndex);
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
            }
            catch { }
        }


        private void CheckBox_Checked(object sender, RoutedEventArgs e)
        {
            CheckBox box = sender as CheckBox;
            switch (box.Name)
            {
                case "tsCheckLevel":
                    if (tsComboLevel.SelectedIndex < 0)
                        tsComboLevel.SelectedIndex = 0;
                    break;
                case "tsCheckEvent":
                    if (tsComboEvent.SelectedIndex < 0)
                        tsComboEvent.SelectedIndex = 0;
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

        #endregion

        #region Private Functions
        private void SetBindings()
        {
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
          
            tsComboEvent.ItemsSource = EventNames;
            tsComboEvent.DisplayMemberPath = "Value";

        }


        private static void InitTestSpawnLevelList()
        {
            LevelNames = new()
            {
                { 5, "Debug Test" },
                { 100, "Westopolis" },
                { 200, "Digital Circuit" },
                { 201, "Glyphic Canyon" },
                { 202, "Lethal Highway" },
                { 210, "Black Bull (Lethal Highway)" },
                { 300, "Cryptic Castle" },
                { 301, "Prison Island" },
                { 302, "Circus Park" },
                { 310, "Egg Breaker (Cryptic Castle)" },
                { 400, "Central City" },
                { 401, "The Doom" },
                { 402, "Sky Troops" },
                { 403, "Mad Matrix" },
                { 404, "Death Ruins" },
                { 410, "Heavy Dog" },
                { 411, "Egg Breaker (Mad Matrix)" },
                { 412, "Black Bull (Death Ruins)" },
                { 500, "The Ark" },
                { 501, "Air Fleet" },
                { 502, "Iron Jungle" },
                { 503, "Space Gadget" },
                { 504, "Lost Impact" },
                { 510, "Blue Falcon" },
                { 511, "Egg Breaker (Iron Jungle)" },
                { 600, "GUN Fortress" },
                { 601, "Black Comet" },
                { 602, "Lava Shelter" },
                { 603, "Cosmic Fall" },
                { 604, "Final Haunt" },
                { 610, "Black Doom (GUN Fortress)" },
                { 611, "Sonic & Diablon (GUN Fortress)" },
                { 612, "Egg Dealer (Black Comet)" },
                { 613, "Sonic & Diablon (Black Comet)" },
                { 614, "Egg Dealer (Lava Shelter)" },
                { 615, "Egg Dealer (Cosmic Fall)" },
                { 616, "Black Doom (Cosmic Fall)" },
                { 617, "Black Doom (Final Haunt)" },
                { 618, "Sonic & Diablon (Final Haunt)" },
                { 700, "The Last Way" },
                { 710, "Devil Doom" },
                { 800, "2-Player stage 1" },
                { 801, "2-Player stage 2" },
                { 802, "2-Player stage 3" },
                { 901, "Event (Cryptic Castle & Mad Matrix Intro)" },
                { 902, "Event 902 (President)" },
                { 903, "Event 903 (President)" },
                { 904, "Event 904 (Gun Fortress Intro)" },
                { 905, "Event 905 (Epilogue)" },
                { 906, "Event 906 (The Ark, Space Gadget and Lost Impact Intro)" },
                { 907, "Event 907 (Commander flashback)" },
                { 908, "Event 908 (Comet in Space)" },
                { 909, "Event 909 (The Last Way Intro)" },
                { 910, "Event 910 (Devil Doom Intro)" },
            };

            //display level ID after the name.
            foreach (var pair in LevelNames.ToList())
            {
                // Update the value
                string updatedValue = pair.Value + " (" + pair.Key + ")";
                LevelNames[pair.Key] = updatedValue;
            }
        }

        private static void InitCutsceneList()
        {

            EventNames = new Dictionary<int, string>
            {
                { 1001, Lang.GetString("SHTHREventName00") },
                { 1101, Lang.GetString("SHTHREventName01") },
                { 2001, Lang.GetString("SHTHREventName02") },
                { 2101, Lang.GetString("SHTHREventName03") },
                { 2102, Lang.GetString("SHTHREventName04") },
                { 2201, Lang.GetString("SHTHREventName05") },
                { 2211, Lang.GetString("SHTHREventName06") },
                { 2301, Lang.GetString("SHTHREventName07") },
                { 3001, Lang.GetString("SHTHREventName08") },
                { 3011, Lang.GetString("SHTHREventName09") },
                { 3101, Lang.GetString("SHTHREventName10") },
                { 3201, Lang.GetString("SHTHREventName11") },
                { 4001, Lang.GetString("SHTHREventName12") },
                { 4002, Lang.GetString("SHTHREventName13") },
                { 4101, Lang.GetString("SHTHREventName14") },
                { 4102, Lang.GetString("SHTHREventName15") },
                { 4111, Lang.GetString("SHTHREventName16") },
                { 4201, Lang.GetString("SHTHREventName17") },
                { 4202, Lang.GetString("SHTHREventName18") },
                { 4301, Lang.GetString("SHTHREventName19") },
                { 4311, Lang.GetString("SHTHREventName20") },
                { 4401, Lang.GetString("SHTHREventName21") },
                { 4411, Lang.GetString("SHTHREventName22") },
                { 5001, Lang.GetString("SHTHREventName23") },
                { 5011, Lang.GetString("SHTHREventName24") },
                { 5021, Lang.GetString("SHTHREventName25") },
                { 5022, Lang.GetString("SHTHREventName26") },
                { 5023, Lang.GetString("SHTHREventName27") },
                { 5024, Lang.GetString("SHTHREventName28") },
                { 5201, Lang.GetString("SHTHREventName29") },
                { 5211, Lang.GetString("SHTHREventName30") },
                { 5301, Lang.GetString("SHTHREventName31") },
                { 5302, Lang.GetString("SHTHREventName32") },
                { 5402, Lang.GetString("SHTHREventName33") },
                { 5403, Lang.GetString("SHTHREventName34") },
                { 6001, Lang.GetString("SHTHREventName35") },
                { 6101, Lang.GetString("SHTHREventName36") },
                { 6102, Lang.GetString("SHTHREventName37") },
                { 6103, Lang.GetString("SHTHREventName38") },
                { 6104, Lang.GetString("SHTHREventName39") },
                { 6201, Lang.GetString("SHTHREventName40") },
                { 6202, Lang.GetString("SHTHREventName41") },
                { 6203, Lang.GetString("SHTHREventName42") },
                { 6301, Lang.GetString("SHTHREventName43") },
                { 6302, Lang.GetString("SHTHREventName44") },
                { 6303, Lang.GetString("SHTHREventName45") },
                { 6401, Lang.GetString("SHTHREventName46") },
                { 6402, Lang.GetString("SHTHREventName47") },
                { 7001, Lang.GetString("SHTHREventName48") },
                { 7011, Lang.GetString("SHTHREventName49") },
                { 7101, Lang.GetString("SHTHREventName50") },
                { 7111, Lang.GetString("SHTHREventName51") },
                { 7201, Lang.GetString("SHTHREventName52") },
                { 7211, Lang.GetString("SHTHREventName53") },
                { 7301, Lang.GetString("SHTHREventName54") },
                { 7311, Lang.GetString("SHTHREventName55") },
                { 7401, Lang.GetString("SHTHREventName56") },
                { 7411, Lang.GetString("SHTHREventName57") },
                { 7501, Lang.GetString("SHTHREventName58") },
                { 7511, Lang.GetString("SHTHREventName59") },
                { 7601, Lang.GetString("SHTHREventName60") },
                { 7611, Lang.GetString("SHTHREventName61") },
                { 7701, Lang.GetString("SHTHREventName62") },
                { 7711, Lang.GetString("SHTHREventName63") },
                { 7801, Lang.GetString("SHTHREventName64") },
                { 7811, Lang.GetString("SHTHREventName65") },
                { 7901, Lang.GetString("SHTHREventName66") },
                { 7911, Lang.GetString("SHTHREventName67") },
                { 8001, Lang.GetString("SHTHREventName68") },
                { 8002, Lang.GetString("SHTHREventName69") },
                { 8003, Lang.GetString("SHTHREventName70") },
                { 8004, Lang.GetString("SHTHREventName71") },
                { 8005, Lang.GetString("SHTHREventName72") },
                { 8006, Lang.GetString("SHTHREventName73") },
                { 8101, Lang.GetString("SHTHREventName74") },
                { 8102, Lang.GetString("SHTHREventName75") },
                { 8103, Lang.GetString("SHTHREventName76") },
                { 8104, Lang.GetString("SHTHREventName77") },
                { 8105, Lang.GetString("SHTHREventName78") },
                { 8106, Lang.GetString("SHTHREventName79") },
                { 8201, Lang.GetString("SHTHREventName80") },
                { 8202, Lang.GetString("SHTHREventName81") },
                { 8203, Lang.GetString("SHTHREventName82") },
                { 8204, Lang.GetString("SHTHREventName83") }
            };


            //display event ID after their name.
            foreach (var pair in EventNames.ToList())
            {
                string displayName = "EV" + pair.Key.ToString("D4") + ": " + pair.Value;
                EventNames[pair.Key] = displayName;
            }
        }



        private string GetTestSpawnCommandLine()
        {
            List<string> cmdline = new();


            if (GameProfile.TestSpawn.LevelIndex > -1)
                if (GameProfile.TestSpawn.UseEvent == false)
                {
                    int lvl_result = GameProfile.TestSpawn.LevelIndex;

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


                    cmdline.Add($"-l {lvl_result}");
                }


            if (GameProfile.TestSpawn.EventIndex > -1)
                if (GameProfile.TestSpawn.UseEvent)
                    cmdline.Add($"-e {GameProfile.TestSpawn.EventIndex}");


            return string.Join(" ", cmdline);
        }

        #endregion
    }
}
