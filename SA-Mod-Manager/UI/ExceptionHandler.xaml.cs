using SAModManager.Profile;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;


namespace SAModManager.UI
{
    /// <summary>
    /// Interaction logic for ExceptionHandler.xaml
    /// </summary>
    public partial class ExceptionHandler : Window, INotifyPropertyChanged
	{
		private Exception exception;
		public string ErrorText { get; set; }

		public event PropertyChangedEventHandler PropertyChanged;

		protected virtual void OnPropertyChanged(string propertyName)
		{
			PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
		}

		private string ExceptionReport(bool markdown = false, bool report = false)
		{
			var error = new StringBuilder();

			if (report)
			{
				error.AppendLine("<!-- Please provide any relevant information as to what caused this exception -->");
				error.AppendLine("<!-- E.g. What you were doing, if this is reproducable and how, etc -->");
				error.AppendLine();
				error.AppendLine();

				error.AppendLine("<!-- Please do not modify following -->");
				error.AppendLine("## Exception Report");
			}

			if (markdown) 
				error.AppendLine("```");

			error.Append($"SAMM Information:");
            error.AppendLine("");
            error.AppendLine($"\tManager Version: {App.VersionString}");
			if (App.isFirstBoot)
				error.AppendLine($"\tFirst Boot: True");

			error.AppendLine($"\tCurrent Game: {App.CurrentGame.gameName}");
			error.AppendLine($"\tGame Directory: {App.CurrentGame.gameDirectory}");
            error.AppendLine($"\tManager Location: {App.StartDirectory}");
            error.AppendLine($"\tRunning as Admin: {Util.RunningAsAdmin().ToString()}");

            if (File.Exists(App.CurrentGame.loader.mlverPath))
				 error.AppendLine($"\tMod Loader Version (ID): {File.ReadAllText(App.CurrentGame.loader.mlverPath)}");
			if (markdown) 
				error.AppendLine("```");

			error.AppendLine("");

			if (exception != null)
			{
				if (markdown) 
					error.AppendLine("```");

				error.AppendLine($"Exception Information:");

				if (exception.GetType() != typeof(Exception))
					error.AppendLine($"\tType: {exception.GetType().Name}");

				error.AppendLine($"\tException: {exception.Message}");

				if (exception.Source != null)
					error.AppendLine($"\tSource: {exception.Source}");

				if (exception.TargetSite != null)
					error.AppendLine($"\tIn Function: {exception.TargetSite}");

				if (exception.StackTrace != null)
					error.AppendLine($"\tStackTrace: {exception.StackTrace.Replace("\n", "\n	")}");

				if (exception.InnerException != null)
					error.AppendLine($"\tInnerException: {exception.InnerException}");

				if (markdown) error.AppendLine("```");
				error.AppendLine($"");
				CreateCrashDump(error);
			}
			else
			{
				if (markdown) error.AppendLine("```");
				error.AppendLine("Exception was null. No Exception to Report. How did you even get to this point?");
				if (markdown) error.AppendLine("```");
			}

			if (report)
			{
				error.AppendLine("<!-- If you copied your settings, please paste them below this line -->");
				error.AppendLine();
			}

			error.AppendLine();

			return error.ToString();
		}

		private string SettingsReport()
		{
			var info = new StringBuilder();

			info.AppendLine();
			if (File.Exists(App.ManagerConfigFile))
			{
				info.AppendLine("```");
				info.AppendLine($"{File.ReadAllText(App.ManagerConfigFile)}");
				info.AppendLine("```");
			}
			else
				info.AppendLine("No Manager Settings File exists.");

			if (File.Exists(Path.Combine(App.CurrentGame.ProfilesDirectory, "Profiles.json")))
			{
				Profiles profiles = Profiles.Deserialize(Path.Combine(App.CurrentGame.ProfilesDirectory, "Profiles.json"));
				string profile = Path.Combine(App.CurrentGame.ProfilesDirectory, profiles.GetProfileFilename());
				info.AppendLine("```");
				info.AppendLine($"{File.ReadAllText(profile)}");
				info.AppendLine("```");
			}
			else
				info.AppendLine("No Profiles exist.");

			return info.ToString();
		}

		public ExceptionHandler(Exception e)
		{
			InitializeComponent();
			DataContext = this;
			exception = e;
		}

		private void Window_Loaded(object sender, RoutedEventArgs e)
		{
			ErrorText = ExceptionReport();
		}

		private void CopyError_Click(object sender, RoutedEventArgs e)
		{
			Clipboard.SetText(ExceptionReport(true));
		}

		private void ReportError_Click(object sender, RoutedEventArgs e)
		{
			MessageWindow message = new(Lang.GetString("MessageWindow.Information.Exception.Title"), Lang.GetString("MessageWindow.Information.Exception.Message"),
				button: MessageWindow.Buttons.YesNo, icon: MessageWindow.Icons.Information);

			message.ShowDialog();

			if (message.isYes)
				Clipboard.SetText(SettingsReport());

			string url = "https://github.com/X-Hax/SA-Mod-Manager/issues/new";
			url += $"?title=[Error+Report]:";  // Add Title
			url += $"&labels=exception+report"; // Add Label
			url += $"&body={Uri.EscapeDataString(ExceptionReport(true, true))}"; // Add Body

            var ps = new ProcessStartInfo(url)
            {
                UseShellExecute = true,
                Verb = "open"
            };
            Process.Start(ps);
		}

		private static void CreateCrashDump(StringBuilder error)
		{
			try
			{
				Util.CreateSafeDirectory(App.crashFolder);
                DateTime currentDateTime = DateTime.Now;
				string formattedDateTime = "log_" + currentDateTime.ToString("dd.MM.yyyy_HH.mm") + ".txt";
                error.AppendLine();
                File.WriteAllText(Path.Combine(App.crashFolder, formattedDateTime), error.ToString());

				if (File.Exists(App.ManagerConfigFile))
				{
					File.Copy(App.ManagerConfigFile, Path.Combine(App.crashFolder, "config_" + currentDateTime.ToString("dd.MM.yyyy_HH.mm") + ".json"));
                }
			}
			catch
			{ }
		}

        public static void UnhandledExceptionEventHandler(Exception e)
        {
            var window = new ExceptionHandler(e);
            Application.Current.MainWindow = window;
            window.ShowDialog();
        }

        private void IgnoreError_Click(object sender, RoutedEventArgs e)
		{
			this.Close();
		}
	}
}
