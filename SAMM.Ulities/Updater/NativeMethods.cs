using System;

using System.ComponentModel;
using System.IO;

using System.Runtime.InteropServices;
using System.Text;


namespace SAMM.Utilities.Updater
{
	public static class NativeMethods
	{
		private static readonly IntPtr INVALID_HANDLE_VALUE = new IntPtr(-1);

		private const uint FILE_READ_EA = 0x0008;
		private const uint FILE_FLAG_BACKUP_SEMANTICS = 0x2000000;

		[DllImport("Kernel32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
		private static extern uint GetFinalPathNameByHandle(IntPtr hFile, IntPtr lpszFilePath, uint cchFilePath, uint dwFlags);

		[DllImport("Kernel32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
		private static extern uint GetFinalPathNameByHandle(IntPtr hFile, StringBuilder lpszFilePath, uint cchFilePath, uint dwFlags);

		[DllImport("kernel32.dll", SetLastError = true)]
		[return: MarshalAs(UnmanagedType.Bool)]
		private static extern bool CloseHandle(IntPtr hObject);

		[DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
		private static extern IntPtr CreateFile(string filename,
												[MarshalAs(UnmanagedType.U4)] uint access,
												[MarshalAs(UnmanagedType.U4)] FileShare share,
												IntPtr securityAttributes,
												[MarshalAs(UnmanagedType.U4)] FileMode creationDisposition,
												[MarshalAs(UnmanagedType.U4)] uint flagsAndAttributes,
												IntPtr templateFile);

		public static string GetFinalPathName(string path)
		{
			IntPtr handle = CreateFile(path,
									   FILE_READ_EA,
									   FileShare.ReadWrite | FileShare.Delete,
									   IntPtr.Zero,
									   FileMode.Open,
									   FILE_FLAG_BACKUP_SEMANTICS,
									   IntPtr.Zero);

			if (handle == INVALID_HANDLE_VALUE)
			{
				throw new Win32Exception();
			}

			try
			{
				// First, call the function with no parameters to retrieve the character count
				// of the reparsed name.
				//
				// Note that the count returned by this function, depending on Windows version
				// and function version (A/W), may or may not include consideration for the
				// null terminator.
				uint requiredCount = GetFinalPathNameByHandle(handle, IntPtr.Zero, 0, 0);

				if (requiredCount == 0)
				{
					throw new Win32Exception();
				}

				// Given the odd null terminator behavior, we're being safe here and adding 1
				// to the required capacity. We can't rely on the marhsalled StringBuilder to
				// do the right thing (which *also* adds 1 for the null terminator).
				int capacity = checked((int)(requiredCount + 1));
				var sb = new StringBuilder(capacity);

				uint finalCount = GetFinalPathNameByHandle(handle, sb, checked((uint)capacity), 0);

				if (finalCount == 0)
				{
					throw new Win32Exception();
				}

				string result = sb.ToString();

				// Manually handle the null terminator, if any.
				int terminatorIndex = result.IndexOf('\0');

				switch (terminatorIndex)
				{
					case -1:
						return result;

					case 0:
						return string.Empty;

					default:
						return result.Substring(0, terminatorIndex);
				}
			}
			finally
			{
				CloseHandle(handle);
			}
		}
	}
}
