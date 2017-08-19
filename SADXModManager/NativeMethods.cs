using System;
using System.ComponentModel;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

namespace SADXModManager
{
	public static class NativeMethods
	{
		private static readonly IntPtr INVALID_HANDLE_VALUE = new IntPtr(-1);

		private const uint FILE_READ_EA = 0x0008;
		private const uint FILE_FLAG_BACKUP_SEMANTICS = 0x2000000;

		[DllImport("Kernel32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
		static extern uint GetFinalPathNameByHandle(IntPtr hFile, StringBuilder lpszFilePath, uint cchFilePath, uint dwFlags);

		[DllImport("kernel32.dll", SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		static extern bool CloseHandle(IntPtr hObject);

		[DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
		private static extern IntPtr CreateFile(
			string filename,
			[MarshalAs(UnmanagedType.U4)] uint access,
			[MarshalAs(UnmanagedType.U4)] FileShare share,
			IntPtr securityAttributes,
			[MarshalAs(UnmanagedType.U4)] FileMode creationDisposition,
			[MarshalAs(UnmanagedType.U4)] uint flagsAndAttributes,
			IntPtr templateFile);

		public static string GetFinalPathName(string path)
		{
			IntPtr handle = CreateFile(path, FILE_READ_EA, FileShare.ReadWrite | FileShare.Delete, IntPtr.Zero,
				FileMode.Open, FILE_FLAG_BACKUP_SEMANTICS, IntPtr.Zero);

			if (handle == INVALID_HANDLE_VALUE)
			{
				throw new Win32Exception();
			}

			try
			{
				var sb = new StringBuilder(1024);
				uint result = GetFinalPathNameByHandle(handle, sb, 1024, 0);

				if (result == 0)
				{
					throw new Win32Exception();
				}

				return sb.ToString();
			}
			finally
			{
				CloseHandle(handle);
			}
		}
	}
}
