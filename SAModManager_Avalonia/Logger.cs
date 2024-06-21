using System;
using System.IO;

namespace SAModManager_Avalonia
{
    public static class Logger
    {
        private static readonly string logFilePath = "application.log";

        public static void Log(string message)
        {
            if (App.isDebug == false)
                return;

            try
            {
                string logMessage = $"{DateTime.Now:yyyy-MM-dd HH:mm:ss} - {message}";
                File.AppendAllText(logFilePath, logMessage + Environment.NewLine);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error writing to log file: {ex.Message}");
            }
        }
    }


}
