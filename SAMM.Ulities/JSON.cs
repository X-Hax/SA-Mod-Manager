using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Threading.Tasks;

namespace SAMM.Utilities
{
	public static class JSON
	{
		/// <summary>
		/// Deserializes a supplied text file.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="filePath"></param>
		/// <returns></returns>
		public static T Deserialize<T>(string filePath)
		{
			if (!string.IsNullOrEmpty(filePath) && File.Exists(filePath))
			{
				string json = File.ReadAllText(filePath);
				return JsonSerializer.Deserialize<T>(json);
			}

			return default(T);
		}

		public static void Serialize<T>(string filePath, T obj)
		{
			string json = JsonSerializer.Serialize<T>(obj);
			File.WriteAllText(filePath, json);
		}
	}
}
