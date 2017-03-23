using System;
using System.Windows.Forms;

namespace SADXModManager.Forms
{
	public partial class UpdateMessageDialog : Form
	{
		string message;

		public UpdateMessageDialog(string message)
		{
			InitializeComponent();
			this.message = message;
		}

		private void UpdateMessageDialog_Load(object sender, EventArgs e)
		{
			textBox1.Text = message;
		}
	}
}
