using ReactiveUI;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAMM.App.Models.Dialogs
{
	public class ExceptionHandlerModel : ReactiveObject
	{
		public Exception? Report { get; private set; }

		public string ExceptionReport
		{
			get
			{
				if (Report == null)
					return "Exception Report is null! This is a big problem.";
				else
				{
					StringBuilder sb = new StringBuilder();
					sb.AppendLine(Report.Message);
					sb.AppendLine();
					sb.AppendLine(Report.StackTrace);
					return sb.ToString();
				}
			}
		}

		public ExceptionHandlerModel() { }

		public ExceptionHandlerModel(Exception report)
		{
			Report = report;
		}
	}
}
