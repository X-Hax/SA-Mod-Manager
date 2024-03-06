using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace SAModManager.Controls
{
	/// <summary>
	/// Interaction logic for NumberBox.xaml
	/// </summary>
	public partial class NumberBox : UserControl
	{
		protected readonly CultureInfo Culture;
		protected Stopwatch timer;

		public enum ValueType
		{
			Double,
			Float,
			Integer,
		}

		#region Extended Properties
		#region Value Property
		public Decimal Value
		{
			get { return (Decimal)GetValue(ValueProperty); }
			set { SetValue(ValueProperty, value); }
		}

		public static readonly DependencyProperty ValueProperty =
			DependencyProperty.Register("Value", typeof(Decimal), typeof(NumberBox), new PropertyMetadata(0m, OnValueChanged, CoerceValue));

		private static void OnValueChanged(DependencyObject element, DependencyPropertyChangedEventArgs e)
		{
			var control = (NumberBox)element;

			control.Input.UndoLimit = 0;
			control.Input.UndoLimit = 1;

			control.ValueChanged?.Invoke(control, new RoutedEventArgs());
		}

		private static object CoerceValue(DependencyObject element, object baseValue)
		{
			var control = (NumberBox)element;
			var value = (Decimal)baseValue;

			control.CoerceValueToBounds(ref value);
			
			if (control.Input != null)
			{
				if (control.Type != ValueType.Integer)
					control.Input.Text = value.ToString("N", control.Culture);

				control.Input.Text = value.ToString("F", control.Culture);
			}

			var decimalCheck = control.GetDecimalTrailingCount(value.ToString(control.Culture));

			if (decimalCheck > control.DecimalCount)
				value = control.TruncateValue(value.ToString(control.Culture), decimalCheck);

			return value;
		}

		private void CoerceValueToBounds(ref Decimal value)
		{
			if (value < MinValue)
				value = MinValue;
			else if (value > MaxValue) 
				value = MaxValue;
		}

		private int GetDecimalTrailingCount(string valueString)
		{
			return valueString.SkipWhile(c => c.ToString(Culture)
			!= Culture.NumberFormat.NumberDecimalSeparator).Skip(1).Count();
		}

		private Decimal TruncateValue(string valueString, int decimalPlaces)
		{
			var endPoint = valueString.Length - (decimalPlaces - DecimalCount);
			var tempString = valueString.Substring(0, endPoint);

			return Decimal.Parse(tempString, Culture);
		}

		public RoutedEventHandler ValueChanged
		{
			get { return (RoutedEventHandler)GetValue(ValueChangedProperty); }
			set { SetValue(ValueChangedProperty, value); }
		}

		public static readonly DependencyProperty ValueChangedProperty =
			DependencyProperty.Register("ValueChanged", typeof(RoutedEventHandler), typeof(NumberBox));
		#endregion

		#region Type Property
		public ValueType Type
		{
			get { return (ValueType)GetValue(TypeProperty); }
			set { SetValue(TypeProperty, value); }
		}

		public static readonly DependencyProperty TypeProperty =
			DependencyProperty.Register("Type", typeof(ValueType), typeof(NumberBox), new PropertyMetadata(ValueType.Double));
		#endregion

		#region Decimal Count Property
		public int DecimalCount
		{
			get { return (int)GetValue(DecimalCountProperty); }
			set { SetValue(DecimalCountProperty, value); }
		}

		public static readonly DependencyProperty DecimalCountProperty =
			DependencyProperty.RegisterAttached("DecimalCount", typeof(int), typeof(NumberBox), new PropertyMetadata(0, OnDecimalCountChanged, CoerceDecimalCount));

		private static void OnDecimalCountChanged(DependencyObject element, DependencyPropertyChangedEventArgs e)
		{
			var control = (NumberBox)element;
			var decimalPlace = (int)e.NewValue;

			control.Culture.NumberFormat.NumberDecimalDigits = decimalPlace;

			control.InvalidateProperty(ValueProperty);
		}

		private static object CoerceDecimalCount(DependencyObject element, Object baseValue)
		{
			var decimalPlace = (int)baseValue;

			if (decimalPlace < 0)
				decimalPlace = 0;
			else if (decimalPlace > 28)
				decimalPlace = 28;

			return decimalPlace;
		}
		#endregion

		#region Max Value Property
		public Decimal MaxValue
		{
			get { return (Decimal)GetValue(MaxValueProperty); }
			set { SetValue(MaxValueProperty, value); }
		}

		public static readonly DependencyProperty MaxValueProperty =
			DependencyProperty.Register("MaxValue", typeof(Decimal), typeof(NumberBox), new PropertyMetadata(Decimal.MaxValue, OnMaxValueChanged, CoerceMaxValue));

		private static void OnMaxValueChanged(DependencyObject element, DependencyPropertyChangedEventArgs e)
		{
			var control = (NumberBox)element;
			var maxValue = (Decimal)e.NewValue;

			if (maxValue < control.MinValue)
				control.MinValue = maxValue;

			if (maxValue <= control.Value)
				control.Value = maxValue;
		}

		private static object CoerceMaxValue(DependencyObject element, Object baseValue)
		{
			var maxValue = (Decimal)baseValue;

			return maxValue;
		}
		#endregion

		#region Min Value Property
		public Decimal MinValue
		{
			get { return (Decimal)GetValue(MinValueProperty); }
			set { SetValue(MinValueProperty, value); }
		}

		public static readonly DependencyProperty MinValueProperty =
			DependencyProperty.Register("MinValue", typeof(Decimal), typeof(NumberBox), new PropertyMetadata(Decimal.MinValue));

		private static void OnMinValueChanged(DependencyObject element, DependencyPropertyChangedEventArgs e)
		{
			var control = (NumberBox)element;
			var minValue = (Decimal)e.NewValue;

			if (minValue < control.MaxValue)
				control.MaxValue = minValue;

			if (minValue <= control.Value)
				control.Value = minValue;
		}

		private static object CoerceMinValue(DependencyObject element, Object baseValue)
		{
			var minValue = (Decimal)baseValue;

			return minValue;
		}
		#endregion
		#endregion

		#region Routed Commands
		private void RemoveFocus()
		{
			Focusable = true;
			Focus();
			Focusable = false;
		}

		private readonly RoutedUICommand _increaseValueCommand =
			new RoutedUICommand("IncreaseValue", "IncreaseValue", typeof(NumberBox));

		private readonly RoutedUICommand _decreaseValueCommand =
			new RoutedUICommand("DecreaseValue", "DecreaseValue", typeof(NumberBox));

		private readonly RoutedUICommand _updateValueCommand = 
			new RoutedUICommand("UpdateValue", "UpdateValue", typeof(NumberBox));

		private readonly RoutedUICommand _cancelChangesCommand =
			new RoutedUICommand("CancelChanges", "CancelChanges", typeof(NumberBox));

		private void CancelChanges()
		{
			Input.Undo();
		}

		private readonly RoutedUICommand _negativeNumberCommand =
			new RoutedUICommand("NegativeNumber", "NegativeNumber", typeof(NumberBox));

		private void NegativeNumber()
		{
			var value = Value;

			if (MinValue < 0)
				value *= -1;

			Value = value;
		}
		#endregion

		#region Spinner Commands
		private void IncreaseValue(Decimal step = 1m)
		{
			Decimal value = ParseStringToDecimal(Input.Text);
			CoerceValueToBounds(ref value);

			if (value <= MaxValue)
				value += step;

			Value = value;
		}

		private void DecreaseValue(Decimal step = 1m)
		{
			Decimal value = ParseStringToDecimal(Input.Text);
			CoerceValueToBounds(ref value);

			if (value >= MinValue)
				value -= step;

			Value = value;
		}
		#endregion

		#region Constructors
		public NumberBox()
		{
			InitializeComponent();

			Culture = (CultureInfo)CultureInfo.CurrentCulture.Clone();
			Culture.NumberFormat.NumberDecimalDigits = DecimalCount;
		}

		public NumberBox(double defaultvalue, double minvalue, double maxvalue, int decicount = 2)
		{
			InitializeComponent();

			Culture = (CultureInfo)CultureInfo.CurrentCulture.Clone();
			Culture.NumberFormat.NumberDecimalDigits = DecimalCount;

			Value = (Decimal)defaultvalue;
			MinValue = (Decimal)minvalue;
			MaxValue = (Decimal)maxvalue;
			DecimalCount = decicount;
			Type = ValueType.Double;
		}

		public NumberBox(float defaultvalue, float minvalue, float maxvalue, int decicount = 2)
		{
			InitializeComponent();

			Culture = (CultureInfo)CultureInfo.CurrentCulture.Clone();
			Culture.NumberFormat.NumberDecimalDigits = DecimalCount;

			InitializeComponent();
			Value = (Decimal)defaultvalue;
			MinValue = (Decimal)minvalue;
			MaxValue = (Decimal)maxvalue;
			DecimalCount = decicount;
			Type = ValueType.Float;
		}

		public NumberBox(int defaultvalue, int minvalue, int maxvalue)
		{
			InitializeComponent();

			Culture = (CultureInfo)CultureInfo.CurrentCulture.Clone();
			Culture.NumberFormat.NumberDecimalDigits = DecimalCount;

			InitializeComponent();
			Value = (Decimal)defaultvalue;
			MinValue = (Decimal)minvalue;
			MaxValue = (Decimal)maxvalue;
			Type = ValueType.Integer;
		}
		#endregion

		#region TextBox Methods
		private Decimal ParseStringToDecimal(string source)
		{
			Decimal value;
			Decimal.TryParse(source, out value);
			
			return value;
		}

		private void Input_LostFocus(object sender, RoutedEventArgs e)
		{
			Value = ParseStringToDecimal(Input.Text);
		}
		#endregion

		#region Spinner Methods
		private void TopSpinner_Click(object sender, RoutedEventArgs e)
		{
			if (timer.ElapsedMilliseconds > 3000)
				IncreaseValue(5m);
			else
				IncreaseValue();
		}

		private void BottomSpinner_Click(object sender, RoutedEventArgs e)
		{
			if (timer.ElapsedMilliseconds > 3000)
				DecreaseValue(5m);
			else
				DecreaseValue();
		}

		private void Spinner_PreviewMouseDown(object sender, MouseButtonEventArgs e)
		{
			timer = new();
			timer.Start();
		}

		private void Spinner_PreviewMouseUp(object sender, MouseButtonEventArgs e)
		{
			timer.Stop();
		}
		#endregion

		#region Numbox Methods
		private void Setup()
		{
			Input.CommandBindings.Add(new CommandBinding(_increaseValueCommand, (a, b) => IncreaseValue()));
			Input.CommandBindings.Add(new CommandBinding(_decreaseValueCommand, (a, b) => DecreaseValue()));
			Input.CommandBindings.Add(new CommandBinding(_updateValueCommand, (a, b) => { Value = ParseStringToDecimal(Input.Text); }));
			Input.CommandBindings.Add(new CommandBinding(_cancelChangesCommand, (a, b) => CancelChanges()));
			Input.CommandBindings.Add(new CommandBinding(_negativeNumberCommand, (a, b) => NegativeNumber()));

			Input.InputBindings.Add(new KeyBinding(_increaseValueCommand, new KeyGesture(Key.Up)));
			Input.InputBindings.Add(new KeyBinding(_increaseValueCommand, Key.Up, ModifierKeys.Control));
			Input.InputBindings.Add(new KeyBinding(_decreaseValueCommand, new KeyGesture(Key.Down)));
			Input.InputBindings.Add(new KeyBinding(_updateValueCommand, new KeyGesture(Key.Enter)));
			Input.InputBindings.Add(new KeyBinding(_cancelChangesCommand, new KeyGesture(Key.Escape)));
			Input.InputBindings.Add(new KeyBinding(_negativeNumberCommand, new KeyGesture(Key.OemMinus)));
			Input.InputBindings.Add(new KeyBinding(_negativeNumberCommand, new KeyGesture(Key.Subtract)));
		}

		private void NumBox_Loaded(object sender, RoutedEventArgs e)
		{
			InvalidateProperty(ValueProperty);
			Setup();
		}

		#endregion
	}
}
