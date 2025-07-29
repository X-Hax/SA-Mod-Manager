using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

namespace SAMM.Utilities.Updater
{
	public static class ClientManager
	{
		public static HttpClient HttpClient { get; set; }
		private const int CD = 59;
		private const int CDAmount = 2;

		public static bool UpdateTimeElapsed(int amount, long previous)
		{
			if (previous <= 0 && amount < CDAmount)
				return true;

			DateTime previousTime = DateTime.FromFileTimeUtc(previous);

			// Get the current time
			DateTime currentTime = DateTime.UtcNow;

			// Calculate the time difference
			TimeSpan timeDifference = currentTime - previousTime;

			// Check if ~60 minutes have passed
			if (timeDifference.TotalMinutes >= CD)
			{
				return true;
			}

			return false;
		}

		public static void HandleRefreshUpdateCD(ref int updateCount, ref long updateTimeout)
		{
			long currentTime = DateTime.UtcNow.ToFileTimeUtc();

			if (updateCount >= CDAmount && updateTimeout <= 0)
			{
				updateTimeout = currentTime;
				return;
			}

			if (updateTimeout <= 0)
				return;


			DateTime previousTime = DateTime.FromFileTimeUtc(updateTimeout);

			// Get the current time
			DateTime curTime = DateTime.UtcNow;

			// Calculate the time difference
			TimeSpan timeDifference = curTime - previousTime;

			if (timeDifference.TotalMinutes > CD && updateTimeout > 0 && updateCount >= CDAmount)
			{
				updateCount = 0;
				updateTimeout = 0;
			}
		}

		public static void InitHttpClient()
		{
			ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls12;
			HttpClient = new();
			HttpClient.DefaultRequestHeaders.TryAddWithoutValidation("User-Agent", "SA-Mod-Manager");
		}
	}
}
