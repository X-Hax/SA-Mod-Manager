using ICSharpCode.AvalonEdit.Highlighting;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Xml;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using ICSharpCode.AvalonEdit.Highlighting.Xshd;
using System.CodeDom;
using ICSharpCode.AvalonEdit;
using SAModManager.Elements;

namespace SAModManager.Common
{
	/// <summary>
	/// Interaction logic for NewCode.xaml
	/// </summary>
	public partial class NewCode : Window
	{
		public EditCode Code = new();

		/// <summary>
		/// Initialize NewCode Window. Constructor can take a path to store for saving mods to the correct location.
		/// </summary>
		public NewCode(EditCode code = null)
		{
			InitializeComponent();

			if (code != null)
				Code = code;

            LabelCategory.Visibility = Visibility.Collapsed;
            CodeCategory.Visibility = Visibility.Collapsed;

			string optheme = App.IsLightTheme ? Properties.Resources.OpCodeSyntaxLight : Properties.Resources.OpCodeSyntaxDark;

            using (XmlTextReader reader = new XmlTextReader(new StringReader(optheme)))
			{
				CodeWriter.SyntaxHighlighting = HighlightingLoader.Load(reader, HighlightingManager.Instance);
			}

			Loaded += NewCode_Loaded;
		}

		private void NewCode_Loaded(object sender, RoutedEventArgs e)
		{
			SetBindings();
		}

		private void SetCodeWriterText(List<string> codeLines)
		{
			StringBuilder sb = new StringBuilder();
			foreach (string line in codeLines)
			{
				if (line != codeLines.Last())
					sb.AppendLine(line.ToString());
				else
					sb.Append(line);
			}

			CodeWriter.Text = sb.ToString();
		}

		private void SetBindings()
		{
			CodeName.SetBinding(TextBox.TextProperty, new Binding("Name")
			{
				Source = Code,
			});
			CodeAuthor.SetBinding(TextBox.TextProperty, new Binding("Author")
			{
				Source = Code,
			});
			CodeDescription.SetBinding(TextBox.TextProperty, new Binding("Description")
			{
				Source = Code,
			});
			IsRequired.SetBinding(CheckBox.IsCheckedProperty, new Binding("Required")
			{
				Source = Code,
			});
			radPatch.SetBinding(RadioButton.IsCheckedProperty, new Binding("IsPatch")
			{
				Source = Code
			});
			radCode.SetBinding(RadioButton.IsCheckedProperty, new Binding("IsChecked")
			{
				Source = radPatch,
				Converter = new BoolFlipConverter()
			});
			SetCodeWriterText(Code.CodeLines);
		}

		private EditCodeError ValidateCodeLine()
		{
			return EditCodeList.ValidateCodeLines(CodeWriter.Text);
		}

		private bool VerifyCodeWriterText()
		{
			foreach (string line in CodeWriter.Text.Split("\r\n"))
				if (line.Trim(' ').Length <= 0)
				{
					new MessageWindow(Lang.GetString("MessageWindow.Errors.CodeFail.Title"), 
						Lang.GetString("MessageWindow.Errors.CodeFailEmptyLine"),
						icon: MessageWindow.Icons.Error).ShowDialog();

					return false;
				}
				else if (line.StartsWith(';'))
					return true;
				else
				{
					EditCodeError validation = ValidateCodeLine();
					if (!validation.Validated)
					{
						new MessageWindow(Lang.GetString("MessageWindow.Errors.CodeFail.Title"), 
							(Lang.GetString("Messagewindow.Errors.CodeFailInvalid") + "\n\n" + validation.Error),
						icon: MessageWindow.Icons.Error).ShowDialog();
						
						return false;
					}
				}

			return true;
		}

		#region Window Functions
		private void CancelButton_Click(object sender, RoutedEventArgs e)
		{
			Code = null;
			this.Close();
		}

		private void SaveButton_Click(object sender, RoutedEventArgs e)
		{
			if (VerifyCodeWriterText())
			{
				Code.CodeLines.Clear();
				foreach (string line in CodeWriter.Text.Split("\r\n"))
					Code.CodeLines.Add(line);

				this.Close();
			}
		}
		#endregion
	}
}
