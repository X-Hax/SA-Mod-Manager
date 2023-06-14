using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using ModManagerCommon.Forms;
using ModManagerWPF.Properties;

namespace ModManagerWPF.Schema
{
	public class ComboProperty : Control
	{
		public List<string> Values
		{
			get
			{
				return (List<string>)GetValue(ValuesProperty);
			}
			set
			{
				SetValue(ValuesProperty, value);
			}
		}
		public static readonly DependencyProperty ValuesProperty = DependencyProperty.Register("Values", typeof(List<string>), typeof(ComboProperty), new PropertyMetadata(new List<string>()));
	
		public ComboProperty(bool isEnum)
		{
			if (isEnum)
			{

			}
			else
			{
				Values = new List<string>()
				{
					"True",
					"False"
				};
			}
		}
	}

	public class IntegerProperty : Control
	{
		public int DefaultValue
		{
			get
			{
				return (int)GetValue(DefaultValueProperty);
			}
			set
			{
				SetValue(DefaultValueProperty, value);
			}
		}
		public static readonly DependencyProperty DefaultValueProperty = DependencyProperty.Register("DefaultValue", typeof(int), typeof(ComboProperty), new PropertyMetadata(0));

		public int MinimumValue
		{
			get
			{
				return (int)GetValue(MinimumValueProperty);
			}
			set
			{
				SetValue(MinimumValueProperty, value);
			}
		}
		public static readonly DependencyProperty MinimumValueProperty = DependencyProperty.Register("MinimumValue", typeof(int), typeof(ComboProperty), new PropertyMetadata(0));

		public int MaximumValue
		{
			get
			{
				return (int)GetValue(MaximumValueProperty);
			}
			set
			{
				SetValue(MaximumValueProperty, value);
			}
		}
		public static readonly DependencyProperty MaximumValueProperty = DependencyProperty.Register("MaximumValue", typeof(int), typeof(ComboProperty), new PropertyMetadata(0));
	}

	public class FloatProperty : Control
	{
		public float DefaultValue
		{
			get
			{
				return (float)GetValue(DefaultValueProperty);
			}
			set
			{
				SetValue(DefaultValueProperty, value);
			}
		}
		public static readonly DependencyProperty DefaultValueProperty = DependencyProperty.Register("DefaultValue", typeof(float), typeof(ComboProperty), new PropertyMetadata((float)0));

		public float MinimumValue
		{
			get
			{
				return (float)GetValue(MinimumValueProperty);
			}
			set
			{
				SetValue(MinimumValueProperty, value);
			}
		}
		public static readonly DependencyProperty MinimumValueProperty = DependencyProperty.Register("MinimumValue", typeof(float), typeof(ComboProperty), new PropertyMetadata(0));

		public float MaximumValue
		{
			get
			{
				return (float)GetValue(MaximumValueProperty);
			}
			set
			{
				SetValue(MaximumValueProperty, value);
			}
		}
		public static readonly DependencyProperty MaximumValueProperty = DependencyProperty.Register("MaximumValue", typeof(float), typeof(ComboProperty), new PropertyMetadata(0));
	}

	public class StringProperty : Control
	{
		public string Input
		{
			get
			{
				return (string)GetValue(TextProperty);
			}
			set
			{
				SetValue(TextProperty, value);
			}
		}
		public static readonly DependencyProperty TextProperty = DependencyProperty.Register("Text", typeof(string), typeof(ComboProperty), new PropertyMetadata(string.Empty));
	}

	public class PropertyGrid : Control
	{
		public string PropertyName
		{
			get
			{
				return (string)GetValue(PropertyNameProperty);
			}
			set
			{
				SetValue(PropertyNameProperty, value);
			}
		}
		public static readonly DependencyProperty PropertyNameProperty = DependencyProperty.Register("PropertyName", typeof(string), typeof(PropertyGrid), new PropertyMetadata(string.Empty));

		public string DisplayName
		{
			get
			{
				return (string)GetValue(DisplayNameProperty);
			}
			set
			{
				SetValue(DisplayNameProperty, value);
			}
		}
		public static readonly DependencyProperty DisplayNameProperty = DependencyProperty.Register("DisplayName", typeof(string), typeof(PropertyGrid), new PropertyMetadata(string.Empty));

		public List<string> PropertyType
		{
			get
			{
				return (List<string>)GetValue(PropertyTypeProperty);
			}
			set
			{
				SetValue(PropertyTypeProperty, value);
			}
		}
		public static readonly DependencyProperty PropertyTypeProperty = DependencyProperty.Register("PropertyType", typeof(List<string>), typeof(PropertyGrid), new PropertyMetadata(new List<string>()));

		public object PropertyElement
		{
			get
			{
				return (object)GetValue(PropertyElementProperty);
			}
			set
			{
				SetValue(PropertyElementProperty, value);
			}
		}
		public static readonly DependencyProperty PropertyElementProperty = DependencyProperty.Register("PropertyElement", typeof(object), typeof(PropertyGrid), new PropertyMetadata(null));

		public string HelpText
		{
			get
			{
				return (string)GetValue(HelpTextProperty);
			}
			set
			{
				SetValue(HelpTextProperty, value);
			}
		}
		public static readonly DependencyProperty HelpTextProperty = DependencyProperty.Register("HelpText", typeof(string), typeof(PropertyGrid), new PropertyMetadata(string.Empty));

		public void Load(ConfigSchemaProperty property)
		{
			if (property == null)
			{
				PropertyName = property.Name;
				DisplayName = property.DisplayName;

				HelpText = property.HelpText;
			}
		}

		public object GetProperty(string property)
		{
			switch (property)
			{
				case "bool":
					return new ComboProperty(false);
				case "string":
					return new StringProperty();
				case "int":
					return new IntegerProperty();
				case "float":
					return new FloatProperty();
				default:
					return GetEnumProperty(property);
			}
		}

		public object GetEnumProperty(string property)
		{
			return null;
		}
	}

	public class GroupGrid : Control
	{
		public string GroupName
		{
			get
			{
				return (string)GetValue(GroupNameProperty);
			}
			set
			{
				SetValue(GroupNameProperty, value);
			}
		}
		public static readonly DependencyProperty GroupNameProperty = DependencyProperty.Register("GroupName", typeof(string), typeof(GroupGrid), new PropertyMetadata(string.Empty));

		public string DisplayName
		{
			get
			{
				return (string)GetValue(DisplayNameProperty);
			}
			set
			{
				SetValue(DisplayNameProperty, value);
			}
		}
		public static readonly DependencyProperty DisplayNameProperty = DependencyProperty.Register("DisplayName", typeof(string), typeof(GroupGrid), new PropertyMetadata(string.Empty));

		public bool Delete()
		{
			return false;
		}
	}

	public class PropertyTree : Control
	{
		public List<GroupGrid> Groups { get; set; }
		
	}

	public class EnumGroup : Control
	{
		public string EnumName
		{
			get
			{
				return (string)GetValue(EnumNameProperty);
			}
			set
			{
				SetValue(EnumNameProperty, value);
			}
		}
		public static readonly DependencyProperty EnumNameProperty = DependencyProperty.Register("EnumName", typeof(string), typeof(GroupGrid), new PropertyMetadata(string.Empty));


	}
}
