
using SAModManager.UI;
using System.Diagnostics;


namespace SAModManager
{
    public static class ASLR
    {
        public static void CheckIfEnabled()
        {
            try
            {
                string powerShellCommand = "Get-ProcessMitigation -System";

                // Set up the process to execute PowerShell
                ProcessStartInfo psi = new()
                {
                    FileName = "powershell",
                    Arguments = $"-Command \"{powerShellCommand}\"",
                    RedirectStandardOutput = true,
                    UseShellExecute = false,
                    CreateNoWindow = true
                };

                // Start the process
                Process process = new() { StartInfo = psi };
                process.Start();

                // Read the output
                string output = process.StandardOutput.ReadToEnd();
                process.WaitForExit();

                // Check if "ForceRelocateImages" is ON
                if (output.Contains("ForceRelocateImages                : ON"))
                {
                    new MessageWindow(Lang.GetString("MessageWindow.DefaultTitle.Warning"), Lang.GetString("MessageWindow.Warnings.ASLRDetected"), MessageWindow.WindowType.IconMessage, MessageWindow.Icons.Warning).ShowDialog();
                }
            }
            catch
            {

            }
        }
    }
}
