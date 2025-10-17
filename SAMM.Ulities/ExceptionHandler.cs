using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAMM.Utilities
{
	public static class ExceptionHandler
	{
		public delegate void ExceptionReportEventHandler(Exception ex);
		private static event ExceptionReportEventHandler? ExceptionReportCallback;

		public static void Initialize(ExceptionReportEventHandler exceptionReportFunc)
		{
			ExceptionReportCallback += exceptionReportFunc;
		}

		public static void Throw(Exception ex)
		{
			if (ExceptionReportCallback != null)
				ExceptionReportCallback(ex);
		}
	}
}
