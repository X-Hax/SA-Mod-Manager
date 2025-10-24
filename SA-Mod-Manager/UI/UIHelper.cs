
using SAModManager.Controls;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

namespace SAModManager.UI
{
    public static class UIHelper
    {
        private static readonly double LowOpacityIcon = 0.3;
        private static readonly double LowOpacityBtn = 0.7;

        public static void ToggleElement(ref UIElement elem, bool value)
        {
            elem.IsEnabled = value;
            elem.Opacity = value ? 1 : LowOpacityIcon;
        }

        public static void ToggleMenuItem(ref MenuItem item, bool value)
        {
            item.IsEnabled = value;
            item.Opacity = value ? 1 : LowOpacityIcon;
        }

        public static void ToggleImgButton(ref ImageButton btn, bool value)
        {
            btn.Opacity = value ? 1 : LowOpacityIcon;
            btn.IsEnabled = value;
        }

        public static void ToggleButton(ref Button btn, bool value)
        {
            if (value)
                EnableButton(ref btn);
            else
                DisableButton(ref btn);
        }

        public static void ToggleImage(ref Image img, bool value)
        {
            img?.SetValue(Image.OpacityProperty, value ? 1 : LowOpacityIcon);
        }

        public static void EnableButton(ref Button btn)
        {
            btn.IsEnabled = true;
            btn.Opacity = 1;
        }
        public static void DisableButton(ref Button btn)
        {
            btn.IsEnabled = false;
            btn.Opacity = LowOpacityBtn;
        }

        public static async void TempDisableSaveAndPlayButton(int ms)
        {
            var saveBtn = ((MainWindow)Application.Current.MainWindow)?.SaveAndPlayButton;
            if (saveBtn != null)
            {
                DisableButton(ref saveBtn);
                await Task.Delay(ms);
                EnableButton(ref saveBtn);
            }
        }

        public static void EnableElement(ref UIElement elem)
        {
            elem.IsEnabled = true;
            elem.Opacity = 1;
        }
    }
}
