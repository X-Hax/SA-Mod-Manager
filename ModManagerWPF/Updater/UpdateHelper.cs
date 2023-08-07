using SAModManager.Common;
using System;
using System.IO;
using System.Net;

namespace SAModManager.Updater
{
    public class UpdateHelper
    {
        private const int CD = 59;
        private const int CDAmount = 2;
        public static bool UpdateTimeElapsed(int amount, long previous)
        {
            if (previous <= 0 && amount < CDAmount)
                return true;

            DateTime previousTime = DateTime.FromFileTimeUtc(previous);

            // Get the current time
            DateTime currentTime = DateTime.UtcNow;

            // Calculate the time difference
            TimeSpan timeDifference = currentTime - previousTime;

            // Check if ~60 minutes have passed
            if (timeDifference.TotalMinutes >= CD)
            {
                return true;
            }

            return false;
        }

        public static void HandleRefreshUpdateCD()
        {
            long currentTime = DateTime.UtcNow.ToFileTimeUtc();

            if (App.configIni.UpdateSettings.UpdateCheckCount >= CDAmount && App.configIni.UpdateSettings.UpdateTimeOutCD <= 0)
            {
                App.configIni.UpdateSettings.UpdateTimeOutCD = currentTime;
                return;
            }

            if (App.configIni.UpdateSettings.UpdateTimeOutCD <= 0)
                return;


            DateTime previousTime = DateTime.FromFileTimeUtc(App.configIni.UpdateSettings.UpdateTimeOutCD);

            // Get the current time
            DateTime curTime = DateTime.UtcNow;

            // Calculate the time difference
            TimeSpan timeDifference = curTime - previousTime;

            if (timeDifference.TotalMinutes > CD && App.configIni.UpdateSettings.UpdateTimeOutCD > 0 && App.configIni.UpdateSettings.UpdateCheckCount >= CDAmount)
            {
                App.configIni.UpdateSettings.UpdateCheckCount = 0;
                App.configIni.UpdateSettings.UpdateTimeOutCD = 0;
            }
        }
    }
}
