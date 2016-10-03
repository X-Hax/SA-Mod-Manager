namespace SADXModManager
{
	partial class UpdateMessageDialog
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.label1 = new System.Windows.Forms.Label();
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.label2 = new System.Windows.Forms.Label();
			this.noButton = new System.Windows.Forms.Button();
			this.yesButton = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(170, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "The following updates were found:";
			// 
			// textBox1
			// 
			this.textBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textBox1.Location = new System.Drawing.Point(12, 25);
			this.textBox1.Multiline = true;
			this.textBox1.Name = "textBox1";
			this.textBox1.ReadOnly = true;
			this.textBox1.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.textBox1.Size = new System.Drawing.Size(260, 183);
			this.textBox1.TabIndex = 1;
			// 
			// label2
			// 
			this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(12, 211);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(193, 13);
			this.label2.TabIndex = 2;
			this.label2.Text = "Do you want to download the updates?";
			// 
			// noButton
			// 
			this.noButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.noButton.DialogResult = System.Windows.Forms.DialogResult.No;
			this.noButton.Location = new System.Drawing.Point(197, 227);
			this.noButton.Name = "noButton";
			this.noButton.Size = new System.Drawing.Size(75, 23);
			this.noButton.TabIndex = 3;
			this.noButton.Text = "&No";
			this.noButton.UseVisualStyleBackColor = true;
			// 
			// yesButton
			// 
			this.yesButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.yesButton.DialogResult = System.Windows.Forms.DialogResult.Yes;
			this.yesButton.Location = new System.Drawing.Point(116, 227);
			this.yesButton.Name = "yesButton";
			this.yesButton.Size = new System.Drawing.Size(75, 23);
			this.yesButton.TabIndex = 4;
			this.yesButton.Text = "&Yes";
			this.yesButton.UseVisualStyleBackColor = true;
			// 
			// UpdateMessageDialog
			// 
			this.AcceptButton = this.yesButton;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.noButton;
			this.ClientSize = new System.Drawing.Size(284, 262);
			this.Controls.Add(this.yesButton);
			this.Controls.Add(this.noButton);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.textBox1);
			this.Controls.Add(this.label1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "UpdateMessageDialog";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.Text = "SADX Mod Manager";
			this.Load += new System.EventHandler(this.UpdateMessageDialog_Load);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox textBox1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Button noButton;
		private System.Windows.Forms.Button yesButton;
	}
}