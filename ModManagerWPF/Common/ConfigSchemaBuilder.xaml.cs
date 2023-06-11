using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using ModManagerWPF;
using ModManagerWPF.Properties;

namespace ModManagerWPF.Common
{
	public partial class ConfigSchemaGrid : ResourceDictionary
	{
		public class ItemConfigSchemaProperty
		{
			List<string> TypesList = new List<string>()
			{
				new string("bool"),
				new string("int"),
				new string("float"),
				new string("string"),
			};

			public Button Delete;
			public TextBox Property;
			public TextBox Display;
			public ComboBox Type;
			public Grid TypeInfo;
			public TextBox HelpText;

			public ItemConfigSchemaProperty()
			{
				//Delete = (Button)
			}
		}

		public class ItemConfigSchemaGroup
		{

		}

		public class ItemConfigSchemaEnum
		{

		}
	}
}
