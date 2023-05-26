using Microsoft.Win32;
using ModManagerCommon;
using ModManagerCommon.Forms;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.IO.Pipes;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;

namespace ModManagerWPF
{
	/// <summary>
	/// Interaction logic for App.xaml
	/// </summary>
	/// 

	public partial class App : Application
	{
		private const string pipeName = "sadx-mod-manager";
		private const string protocol = "sadxmm:";
		const string datadllorigpath = "system/CHRMODELS_orig.dll";
		const string loaderdllpath = "mods/SADXModLoader.dll";
		const string datadllpath = "system/CHRMODELS.dll";
		private static readonly Mutex mutex = new Mutex(true, pipeName);
		public static UriQueue UriQueue;

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]

		protected override void OnStartup(StartupEventArgs e)
		{
			MainWindow = new MainWindow();
			MainWindow.Show();
			base.OnStartup(e);
		}

	}
}
