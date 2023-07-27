using System.Globalization;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows;
using System.Windows.Controls;

namespace ModManagerWPF.Elements
{
	public partial class NumericUpDown : TextBox
	{
		#region Internal Elements
		private RepeatButton incrementButton;
		private RepeatButton decrementButton;
		#endregion

		public enum DataType
		{
			Integer = 0,
			Float = 1,
			Double = 2,
		}

		#region Extended Variables
		private bool numericKeyPressedHandled = false;

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

		#endregion

		public NumericUpDown()
		{
			Loaded += NumericUpDown_Loaded;
			PreviewTextInput += NumValue_PreviewTextInput;
			PreviewMouseWheel += NumValue_PreviewMouseWheel;
		}

		private void NumValue_PreviewMouseWheel(object sender, MouseWheelEventArgs e)
		{
			Value += (e.Delta > 0) ? 1 : -1;
			e.Handled = true;
		}

		private void NumericUpDown_Loaded(object sender, RoutedEventArgs e)
		{
			incrementButton = Template.FindName("IncrementUp", this) as RepeatButton;
			decrementButton = Template.FindName("IncrementDown", this) as RepeatButton;

			if (incrementButton != null)
				incrementButton.Click += IncrementUp_Click;

			if (decrementButton != null)
				decrementButton.Click += IncrementDown_Click;

			Binding textBinding = new Binding("Value")
			{
				Source = this,
				Mode = BindingMode.TwoWay,
				BindsDirectlyToSource = true,
			};

			if (ValueType == DataType.Integer)
				textBinding.StringFormat = "{0:}";
			else
				textBinding.StringFormat = "{0:F2}";

			SetBinding(TextProperty, textBinding);
		}

		private static void OnValueChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
		{
			// Ensure the Value property stays within the specified bounds.
			NumericUpDown textBox = (NumericUpDown)d;
			double newValue = (double)e.NewValue;
			double min = textBox.MinValue;
			double max = textBox.MaxValue;

			// Restrict the value to the specified bounds.
			if (newValue < min)
				textBox.Value = min;
			else if (newValue > max)
				textBox.Value = max;
		}

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

		private void NumValue_PreviewTextInput(object sender, System.Windows.Input.TextCompositionEventArgs e)
		{
			var textBox = sender as TextBox;
			// Use SelectionStart property to find the caret position.
			// Insert the previewed text into the existing text in the textbox.
			var fullText = textBox.Text.Insert(textBox.SelectionStart, e.Text);

			double val;
			// If parsing is successful, set Handled to false
			e.Handled = !double.TryParse(fullText,
										 NumberStyles.AllowDecimalPoint | NumberStyles.AllowLeadingSign,
										 CultureInfo.InvariantCulture,
										 out val);
		}
	}
}
