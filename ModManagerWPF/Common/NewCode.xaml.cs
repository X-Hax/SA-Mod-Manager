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
using System.Windows.Shapes;
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
		private string CodesPath = string.Empty;
		public Code Code = new();

		/// <summary>
		/// Initialize NewCode Window. Constructor can take a path to store for saving mods to the correct location.
		/// </summary>
        public NewCode(string codepath = "", string codefile = "codes.lst", Code code = null)
        {
            InitializeComponent();
			CodesPath = codepath + codefile;

			if (code != null)
				Code = code;

			using (XmlTextReader reader = new XmlTextReader(new StringReader(Properties.Resources.OpCodeSyntaxDark)))
			{
				CodeWriter.SyntaxHighlighting = HighlightingLoader.Load(reader, HighlightingManager.Instance);
			}

			Loaded += NewCode_Loaded;
		}

		private void NewCode_Loaded(object sender, RoutedEventArgs e)
		{
			SetBindings();
		}

		private void SetCodeWriterText(List<CodeLine> codeLines)
		{
			StringBuilder sb = new StringBuilder();
			foreach (CodeLine line in codeLines)
			{
				sb.AppendLine(line.ToString());
			}

			CodeWriter.Text = sb.ToString();
		}

		private void SaveCodeWriterLines()
		{
			
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
			radPatch.SetBinding(RadioButton.IsCheckedProperty, new Binding("Patch")
			{
				Source = Code
			});
			radCode.SetBinding(RadioButton.IsCheckedProperty, new Binding("IsChecked")
			{
				Source = radPatch,
				Converter = new BoolFlipConverter()
			});
			SetCodeWriterText(Code.Lines);
		}

		private bool SaveCodeToFile()
		{
			return true;
		}

		#region Window Functions
		private void CancelButton_Click(object sender, RoutedEventArgs e)
		{
			this.Close();
		}

		private void SaveButton_Click(object sender, RoutedEventArgs e)
		{
			if (SaveCodeToFile())
				this.Close();
		}
		#endregion
	}
}
