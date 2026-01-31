using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace SAMM.Configuration
{
	/// <summary>
	/// Base object with a basic implemented of the <see cref="INotifyPropertyChanged"/>.
	/// </summary>
	public class NotifyObject : INotifyPropertyChanged
	{
		public event PropertyChangedEventHandler? PropertyChanged;

		/// <summary>
		/// Invokes <see cref="PropertyChanged"/> through the <see cref="INotifyPropertyChanged"/> interface when the provided property is changed.
		/// </summary>
		/// <param name="propertyName"></param>
		protected void OnPropertyChanged(string propertyName) => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));

		/// <summary>
		/// Invokes <see cref="OnPropertyChanged(string)"/> if the provided value is not equal to the property being updated.
		/// 
		/// Used in a public property's setter.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="field"></param>
		/// <param name="value"></param>
		/// <param name="propertyName"></param>
		/// <returns></returns>
		protected bool RaiseAndSetIfChanged<T>(ref T field, T value, [CallerMemberName] string propertyName = "")
		{
			if (EqualityComparer<T>.Default.Equals(field, value)) return false;
			field = value;
			OnPropertyChanged(propertyName);
			return true;
		}
	}
}
