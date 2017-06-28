using System;
using System.Windows.Forms;

namespace SADXModManager.Controls
{
	public partial class ButtonControl : UserControl
	{
		public ButtonControl()
		{
			InitializeComponent();
		}

		public event EventHandler SetButtonClicked = delegate { };
		public event EventHandler ClearButtonClicked = delegate { };

		public override string Text { get { return label1.Text; } set { label1.Text = value; } }

		private void button1_Click(object sender, EventArgs e)
		{
			SetButtonClicked(this, EventArgs.Empty);
		}

		private void button2_Click(object sender, EventArgs e)
		{
			ClearButtonClicked(this, EventArgs.Empty);
		}
	}
}
