namespace SAMM.Utilities
{
	public static class Logger
	{
		private static readonly string logFilePath = "application.log";

		private static bool logEnabled = false;

		public static void InitLogger() 
		{ 
			logEnabled = true;
			File.WriteAllText(logFilePath, string.Empty);
		}

		public static void Log(string message)
		{
			if (logEnabled == false)
				return;

			try
			{
				string logMessage = $"{DateTime.Now:yyyy-MM-dd HH:mm:ss} - {message}{Environment.NewLine}";
				File.AppendAllTextAsync(logFilePath, message);
			}
			catch (Exception ex)
			{
				Console.WriteLine($"Error writing to log file: {ex.Message}");
			}
		}
	}
}
