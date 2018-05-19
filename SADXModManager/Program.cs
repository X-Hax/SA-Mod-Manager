using Microsoft.Win32;
using ModManagerCommon;
using ModManagerCommon.Forms;
using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Pipes;
using System.Linq;
using System.Threading;
using System.Windows.Forms;

namespace SADXModManager
{
	static class Program
	{
		private const string pipeName = "sadx-mod-manager";
		private const string protocol = "sadxmm:";
		private static readonly Mutex mutex = new Mutex(true, pipeName);
		public static UriQueue UriQueue;

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
			if (args.Length > 0 && args[0] == "urlhandler")
			{
				using (var hkcr = Registry.ClassesRoot)
				using (var key = hkcr.CreateSubKey("sadxmm"))
				{
					key.SetValue(null, "URL:SADX Mod Manager Protocol");
					key.SetValue("URL Protocol", string.Empty);
					using (var k2 = key.CreateSubKey("DefaultIcon"))
						k2.SetValue(null, Application.ExecutablePath + ",1");
					using (var k3 = key.CreateSubKey("shell"))
					using (var k4 = k3.CreateSubKey("open"))
					using (var k5 = k4.CreateSubKey("command"))
						k5.SetValue(null, $"\"{Application.ExecutablePath}\" \"%1\"");
				}
				return;
			}

			bool alreadyRunning;
			try { alreadyRunning = !mutex.WaitOne(0, true); }
			catch (AbandonedMutexException) { alreadyRunning = false; }

			if (args.Length > 1 && args[0] == "doupdate")
			{
				if (alreadyRunning)
					try { mutex.WaitOne(); }
					catch (AbandonedMutexException) { }
				Application.EnableVisualStyles();
				Application.SetCompatibleTextRenderingDefault(false);
				Application.Run(new LoaderManifestDialog(args[1]));
				return;
			}

			if (args.Length > 1 && args[0] == "cleanupdate")
			{
				if (alreadyRunning)
					try { mutex.WaitOne(); }
					catch (AbandonedMutexException) { }
				alreadyRunning = false;
				Thread.Sleep(1000);
				try
				{
					File.Delete(args[1] + ".7z");
					Directory.Delete(args[1], true);
				}
				catch { }
			}

			if (!alreadyRunning)
			{
				UriQueue = new UriQueue(pipeName);
			}

			List<string> uris = args
				.Where(x => x.Length > protocol.Length && x.StartsWith(protocol, StringComparison.Ordinal))
				.ToList();

			if (uris.Count > 0)
			{
				using (var pipe = new NamedPipeClientStream(".", pipeName, PipeDirection.Out))
				{
					pipe.Connect();

					var writer = new StreamWriter(pipe);
					foreach (string s in uris)
					{
						writer.WriteLine(s);
					}
					writer.Flush();
				}
			}

			if (alreadyRunning)
			{
				return;
			}

			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new MainForm());
			UriQueue.Close();
		}
	}
}
