using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows;
using System.Windows.Controls;
using System.Text.RegularExpressions;
using System.Windows.Media;

namespace SAModManager.Elements
{
	public partial class ImageButton : Button
	{
		public enum DrawType
		{
			Path = 0,
			Fill = 1,
		}

		public Geometry Icon
		{
			get
			{
				return GetValue(IconProperty) as Geometry;
			}
			set
			{
				SetValue(IconProperty, value);
			}
		}

		public static readonly DependencyProperty IconProperty =
			DependencyProperty.Register("Icon", typeof(Geometry), typeof(ImageButton));

		public Brush ImageBrush
		{
			get
			{
				return GetValue(ImageBrushProperty) as Brush;
			}
			set
			{
				SetValue(ImageBrushProperty, value);
			}
		}

		public static readonly DependencyProperty ImageBrushProperty =
			DependencyProperty.Register("ImageBrush", typeof(Brush), typeof(ImageButton));

		public Brush ImageBrushDisabled
		{
			get
			{
				return GetValue(ImageBrushDisabledProperty) as Brush;
			}
			set
			{
				SetValue(ImageBrushDisabledProperty, value);
			}
		}

		public static readonly DependencyProperty ImageBrushDisabledProperty =
			DependencyProperty.Register("ImageBrushDisabled", typeof(Brush), typeof(ImageButton));

		public Brush ImageBrushHover
		{
			get
			{
				return GetValue(ImageBrushHoverProperty) as Brush;
			}
			set
			{
				SetValue(ImageBrushHoverProperty, value);
			}
		}

		public static readonly DependencyProperty ImageBrushHoverProperty =
			DependencyProperty.Register("ImageBrushHover", typeof(Brush), typeof(ImageButton));

		public double IconThickness
		{
			get { return (double)GetValue(IconThicknessProperty); }
			set { SetValue(IconThicknessProperty, value); }
		}

		public static readonly DependencyProperty IconThicknessProperty =
			DependencyProperty.Register("IconThickness", typeof(double), typeof(ImageButton));

		public bool IconFill
		{
			get { return (bool)GetValue(IconFillProperty); }
			set { SetValue(IconFillProperty, value); }
		}

		public static readonly DependencyProperty IconFillProperty =
			DependencyProperty.Register("IconFill", typeof(bool), typeof(ImageButton));
	}
}
