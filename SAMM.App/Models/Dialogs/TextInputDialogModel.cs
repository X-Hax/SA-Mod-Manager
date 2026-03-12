using HtmlAgilityPack;
using ReactiveUI;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SAMM.App.Models.Dialogs
{
	public class TextInputDialogModel : ReactiveObject
	{
		private string textInput = string.Empty;

		public string TextInput
		{
			get => textInput;
			set => this.RaiseAndSetIfChanged(ref textInput, value);
		}

		public TextInputDialogModel() { }

		public TextInputDialogModel(string input)
		{
			TextInput = input;
		}
	}
}
