using System.Collections.ObjectModel;
using System.Text.Json.Serialization;

namespace SAMM.Configuration.Codes
{
	public class GameCode : NotifyObject
	{
		private string nameTag = string.Empty;
		/// <summary>
		/// The tag of the code that is saved to the codes list in the <see cref="GameSettings"/>.
		/// </summary>
		[JsonPropertyName("Name")]
		public string NameTag 
		{
			get => nameTag; 
			set => RaiseAndSetIfChanged(ref nameTag, value);
		}

		private List<string> assemblyInstructions = new List<string>();
		/// <summary>
		/// Assembly Instructions used for any code patches for SADX or SA2B.
		/// </summary>
		[JsonPropertyName("Assembly")]
		public List<string> AssemblyInstructions 
		{ 
			get => assemblyInstructions;
			set => RaiseAndSetIfChanged(ref assemblyInstructions, value); 
		}

		private string displayName = string.Empty;
		/// <summary>
		/// The default name displayed to users within the UI.
		/// </summary>
		[JsonPropertyName("InternalName")]
		public string DisplayName
		{
			get => displayName;
			set => RaiseAndSetIfChanged(ref displayName, value);
		}

		private string author = string.Empty;
		/// <summary>
		/// Name of the creator of the code.
		/// </summary>
		public string Author 
		{ 
			get => author;
			set => RaiseAndSetIfChanged(ref author, value); 
		}

		private string description = string.Empty;
		/// <summary>
		/// Brief description of what the code does.
		/// </summary>
		public string Description 
		{
			get => description;
			set => RaiseAndSetIfChanged(ref description, value);
		}

		private string category = string.Empty;
		/// <summary>
		/// The category of game modification that the code belongs to.
		/// </summary>
		public string Category 
		{
			get => category;
			set => RaiseAndSetIfChanged(ref category, value); 
		}

		private bool required = false;
		/// <summary>
		/// Forces the code to be enabled at all times.
		/// </summary>
		public bool Required 
		{
			get => required;
			set => RaiseAndSetIfChanged(ref required, value);
		}

		private bool isPatch = false;
		/// <summary>
		/// If true, sets the code to be handled as a patch which is applied only once on game initialization. 
		/// 
		/// When false, codes are run every frame.
		/// </summary>
		public bool IsPatch 
		{
			get => isPatch;
			set => RaiseAndSetIfChanged(ref isPatch, value);
		}

		private bool isChecked = false;
		/// <summary>
		/// Bound variable that is set based on the GameSettings. Not to be saved to the json file.
		/// </summary>
		[JsonIgnore]
		public bool IsChecked
		{
			get => isChecked;
			set => RaiseAndSetIfChanged(ref isChecked, value);
		}

		public GameCode() { }
	}
}
