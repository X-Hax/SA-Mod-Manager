using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Resources;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace ModManagerWPF
{
	public static class Lang
	{
		public static string GetString(string key)
		{
			var resource = Application.Current.TryFindResource(key);

			if (resource is string str)
				return str;

			return key;
		}

		private readonly static List<string> RTLlanguageList = new()
		{
			"he-IL"
		};

		public static bool IsLanguageRTL()
		{
			foreach (var lang in RTLlanguageList) 
			{
				if (lang == App.CurrentLang.FileName) 
					return true;
			}

			return false;
		}
	}

	public class FlowDirectionHelper
	{
		private static FlowDirection _currentFlowDirection;

		public static FlowDirection CurrentFlowDirection
		{
			get { return _currentFlowDirection; }
			set
			{
				if (_currentFlowDirection != value)
				{
					_currentFlowDirection = value;
				}
			}
		}
		public static void UpdateFlowDirection()
		{
			CurrentFlowDirection = Lang.IsLanguageRTL() ? FlowDirection.RightToLeft : FlowDirection.LeftToRight;
		}
	}
}
