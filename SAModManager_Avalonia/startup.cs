using System;
using System.Threading.Tasks;

namespace SAModManager_Avalonia
{
    public class Startup
    {
        public static async Task StartupCheck()
        {
            Console.WriteLine("Checking dependencies...");
            Util.ClearTempFolder();
        }
    }
}
