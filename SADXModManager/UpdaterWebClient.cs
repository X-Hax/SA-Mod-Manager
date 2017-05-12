using System;
using System.Net;

namespace SADXModManager
{
	public class UpdaterWebClient : WebClient
	{
		protected override WebRequest GetWebRequest(Uri address)
		{
			var request = base.GetWebRequest(address) as HttpWebRequest;

			if (request != null)
			{
				request.UserAgent = "sadx-mod-manager";
				request.Timeout = 5000;
			}

			return request;
		}
	}
}
