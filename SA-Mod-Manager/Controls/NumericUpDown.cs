using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows;
using System.Windows.Controls;
using System.Text.RegularExpressions;


namespace SAModManager.Controls
{
	public partial class NumericUpDown : TextBox
	{
		#region Internal Elements
		private RepeatButton incrementButton;
		private RepeatButton decrementButton;
		private TextBox textBox;
		private bool incrementLoaded = false;
		private bool decrementLoaded = false;
		#endregion

		public enum DataType
		{
			Integer = 0,
			Float = 1,
			Double = 2,
		}

		#region Extended Variables
		public DataType ValueType
		{
			get { return (DataType)GetValue(ValueTypeProperty); }
			set { SetValue(ValueTypeProperty, value); }
		}

		public static readonly DependencyProperty ValueTypeProperty =
			DependencyProperty.Register("ValueType", typeof(DataType), typeof(NumericUpDown));

		public double Value
		{
			get { return (double)GetValue(ValueProperty); }
			set { SetValue(ValueProperty, value); }
		}

		public static readonly DependencyProperty ValueProperty =
					DependencyProperty.Register("Value", typeof(double), typeof(NumericUpDown), new FrameworkPropertyMetadata(0.0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault, OnValueChanged));

		public double MaxValue
		{
			get { return (double)GetValue(MaxValueProperty); }
			set { SetValue(MaxValueProperty, value); }
		}

		public static readonly DependencyProperty MaxValueProperty =
			DependencyProperty.Register("MaxValue", typeof(double), typeof(NumericUpDown), new PropertyMetadata(double.PositiveInfinity));

		public double MinValue
		{
			get { return (double)GetValue(MinValueProperty); }
			set { SetValue(MinValueProperty, value); }
		}

		public static readonly DependencyProperty MinValueProperty =
			DependencyProperty.Register("MinValue", typeof(double), typeof(NumericUpDown), new PropertyMetadata(double.NegativeInfinity));

		public RoutedEventHandler ValueChanged
		{
			get { return (RoutedEventHandler)GetValue(ValueChangedProperty); }
			set { SetValue(ValueChangedProperty, value); }
		}

		public static readonly DependencyProperty ValueChangedProperty =
			DependencyProperty.Register("ValueChanged", typeof(RoutedEventHandler), typeof(NumericUpDown));

		#endregion

		public NumericUpDown()
		{
			Loaded += NumericUpDown_Loaded;
			PreviewTextInput += NumValue_PreviewTextInput;
			PreviewMouseWheel += NumValue_PreviewMouseWheel;
			KeyDown += NumericUpDown_KeyDown;
		}

		#region Invernal Functions
		private void NumericUpDown_Loaded(object sender, RoutedEventArgs e)
		{
			incrementButton = Template.FindName("IncrementUp", this) as RepeatButton;
			decrementButton = Template.FindName("IncrementDown", this) as RepeatButton;

			if (incrementButton != null)
				if (!incrementLoaded)
				{
					incrementButton.Click += IncrementUp_Click;
					incrementLoaded = true;
				}

			if (decrementButton != null)
				if (!decrementLoaded)
				{
					decrementButton.Click += IncrementDown_Click;
					decrementLoaded = true;
				}

			textBox = Template.FindName("NumValue", this) as TextBox;

			Binding textBinding = new Binding("Value")
			{
				RelativeSource = new RelativeSource(RelativeSourceMode.TemplatedParent),
				Mode = BindingMode.TwoWay,
			};

			textBinding.StringFormat = ValueType == DataType.Integer ? "{0}" : "{0:F2}";

			if (textBox != null)
				textBox.SetBinding(TextProperty, textBinding);
		}

		private static void OnValueChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
		{
			// Ensure the Value property stays within the specified bounds.
			NumericUpDown textBox = (NumericUpDown)d;
			double newValue = (double)e.NewValue;
			double min = textBox.MinValue;
			double max = textBox.MaxValue;

			if (newValue > min && newValue < max)
				textBox.Value = newValue;
			else
			{
				if (newValue < min)
					textBox.Value = min;
				else if (newValue > max)
					textBox.Value = max;
			}

			textBox.ValueChanged?.Invoke(textBox, new RoutedEventArgs());
		}

		private void NumValue_PreviewMouseWheel(object sender, MouseWheelEventArgs e)
		{
			Value += (e.Delta > 0) ? 1 : -1;
			e.Handled = true;
		}

		private void NumValue_PreviewTextInput(object sender, TextCompositionEventArgs e)
		{
			var regex = ValueType == DataType.Integer ? new Regex("[^0-9]+") : new Regex("[^0-9.-]+");
			
			if (regex.IsMatch(e.Text))
			{
				if (double.TryParse(e.Text, out double value))
				{
					Value = value;
				}
				e.Handled = true;
			}
		}

		private void NumericUpDown_KeyDown(object sender, KeyEventArgs e)
		{
			if (e.Key == Key.Enter)
			{
				MoveFocus(new TraversalRequest(FocusNavigationDirection.Next));
			}

			// Allow only numeric keys and some control keys (e.g., Backspace, Delete, Enter)
			if (!(e.Key >= Key.D0 && e.Key <= Key.D9) &&
				!(e.Key >= Key.NumPad0 && e.Key <= Key.NumPad9) &&
				!(e.Key == Key.Decimal || e.Key == Key.OemPeriod || e.Key == Key.OemComma || e.Key == Key.Back || e.Key == Key.Delete))
			{
				e.Handled = true;
			}

			if (e.Key == Key.Subtract || e.Key == Key.OemMinus)
			{
				Value *= -1.0f;
			}
		}

		#region Buttons
		private void IncrementUp_Click(object sender, RoutedEventArgs e)
		{
			if (ValueType == DataType.Integer)
				Value += 1;
			else
				Value += 0.1;
		}

		private void IncrementDown_Click(object sender, RoutedEventArgs e)
		{
			if (ValueType == DataType.Integer)
				Value -= 1;
			else
				Value -= 0.1;
		}
		#endregion
		#endregion

		#region Public Functions
		public int GetInt()
		{
			return (int)Value;
		}

		public float GetFloat()
		{
			return (float)Value;
		}

		public double GetDouble()
		{
			return (double)Value;
		}
		#endregion
	}
}
