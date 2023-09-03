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
using static System.Windows.Forms.LinkLabel;

namespace SAModManager.Common
{
    /// <summary>
    /// Interaction logic for NewCode.xaml
    /// </summary>
    public partial class NewCode : Window
    {
		private string CodesPath = string.Empty;
		public Code Code = new();

        public static readonly List<string> ignoreCodeKeys = new()
		{
			"Patch",
			"Code",
			"Author",
			"Description",
			"Category"
		};

		/// <summary>
		/// Initialize NewCode Window. Constructor can take a path to store for saving mods to the correct location.
		/// </summary>
        public NewCode(string codepath = "", string codefile = "codes.lst", Code code = null)
        {
            InitializeComponent();
			CodesPath = Path.Combine(codepath, codefile);

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

		static bool isLineAllowed(string line)
		{
			foreach (var word in ignoreCodeKeys)
			{
                if (line.Contains(word))
				{
					return false;
				}
            }

			return true;
		}

        static string FilterLogicLines(string filePath, string name)
        {
            StringBuilder filteredLines = new();

			int currentLine = 0;
			int startLine = 0;
			bool lineFound = false;

            using (StreamReader reader = new(filePath))
            {
                string line;
                while ((line = reader.ReadLine()) != null)
                {
					if (line.Contains(name) && !lineFound)
					{
						startLine = currentLine;
						lineFound = true;
					}

					if (lineFound)
					{
                        // Check if the line starts with "Author," "Description," or "Code"
                        if (isLineAllowed(line))
                        {
                            if (currentLine >= startLine)
							{
                                filteredLines.AppendLine(line);
                            }      
                        }
                        else if (currentLine > startLine)
                        {
                            break;
                        }
                    }

					currentLine++;
                }
            }

            return filteredLines.ToString();
        }

        private void SetCodeWriterText()
		{
			if (File.Exists(CodesPath))
			{
                CodeWriter.Text = FilterLogicLines(CodesPath, this.Code.Name);
            }
				
		}

		private void SaveCodeWriterLines()
		{
            File.WriteAllText(CodesPath, CodeWriter.Text);
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
			SetCodeWriterText();
		}

		private bool SaveCodeToFile()
		{
			SaveCodeWriterLines();
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
