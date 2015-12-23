namespace SADXModManager
{
	partial class NewModDialog
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
			this.components = new System.ComponentModel.Container();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.buttonOK = new System.Windows.Forms.Button();
			this.buttonCancel = new System.Windows.Forms.Button();
			this.textModName = new System.Windows.Forms.TextBox();
			this.textModAuthor = new System.Windows.Forms.TextBox();
			this.checkOpenFolder = new System.Windows.Forms.CheckBox();
			this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
			this.textModDescription = new System.Windows.Forms.TextBox();
			this.textVersion = new System.Windows.Forms.TextBox();
			this.label4 = new System.Windows.Forms.Label();
			this.checkRedirectMainSave = new System.Windows.Forms.CheckBox();
			this.checkRedirectChaoSave = new System.Windows.Forms.CheckBox();
			this.label5 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(28, 15);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(62, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "Mod Name:";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(27, 93);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(63, 13);
			this.label2.TabIndex = 6;
			this.label2.Text = "Description:";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(49, 44);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(41, 13);
			this.label3.TabIndex = 2;
			this.label3.Text = "Author:";
			// 
			// buttonOK
			// 
			this.buttonOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonOK.Location = new System.Drawing.Point(136, 185);
			this.buttonOK.Name = "buttonOK";
			this.buttonOK.Size = new System.Drawing.Size(75, 23);
			this.buttonOK.TabIndex = 12;
			this.buttonOK.Text = "&OK";
			this.buttonOK.UseVisualStyleBackColor = true;
			this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
			// 
			// buttonCancel
			// 
			this.buttonCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.buttonCancel.Location = new System.Drawing.Point(217, 185);
			this.buttonCancel.Name = "buttonCancel";
			this.buttonCancel.Size = new System.Drawing.Size(75, 23);
			this.buttonCancel.TabIndex = 13;
			this.buttonCancel.Text = "&Cancel";
			this.buttonCancel.UseVisualStyleBackColor = true;
			// 
			// textModName
			// 
			this.textModName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textModName.Location = new System.Drawing.Point(96, 12);
			this.textModName.Name = "textModName";
			this.textModName.Size = new System.Drawing.Size(196, 20);
			this.textModName.TabIndex = 1;
			// 
			// textModAuthor
			// 
			this.textModAuthor.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textModAuthor.Location = new System.Drawing.Point(96, 41);
			this.textModAuthor.Name = "textModAuthor";
			this.textModAuthor.Size = new System.Drawing.Size(196, 20);
			this.textModAuthor.TabIndex = 3;
			// 
			// checkOpenFolder
			// 
			this.checkOpenFolder.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.checkOpenFolder.AutoSize = true;
			this.checkOpenFolder.Location = new System.Drawing.Point(12, 191);
			this.checkOpenFolder.Name = "checkOpenFolder";
			this.checkOpenFolder.Size = new System.Drawing.Size(81, 17);
			this.checkOpenFolder.TabIndex = 11;
			this.checkOpenFolder.Text = "Open folder";
			this.toolTip1.SetToolTip(this.checkOpenFolder, "Open the newly created mod\'s folder upon completion.");
			this.checkOpenFolder.UseVisualStyleBackColor = true;
			// 
			// toolTip1
			// 
			this.toolTip1.ShowAlways = true;
			// 
			// textModDescription
			// 
			this.textModDescription.AcceptsReturn = true;
			this.textModDescription.Location = new System.Drawing.Point(96, 93);
			this.textModDescription.Multiline = true;
			this.textModDescription.Name = "textModDescription";
			this.textModDescription.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.textModDescription.Size = new System.Drawing.Size(196, 64);
			this.textModDescription.TabIndex = 7;
			// 
			// textVersion
			// 
			this.textVersion.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textVersion.Location = new System.Drawing.Point(96, 67);
			this.textVersion.Name = "textVersion";
			this.textVersion.Size = new System.Drawing.Size(196, 20);
			this.textVersion.TabIndex = 5;
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(45, 70);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(45, 13);
			this.label4.TabIndex = 4;
			this.label4.Text = "Version:";
			// 
			// checkRedirectMainSave
			// 
			this.checkRedirectMainSave.AutoSize = true;
			this.checkRedirectMainSave.Location = new System.Drawing.Point(96, 163);
			this.checkRedirectMainSave.Name = "checkRedirectMainSave";
			this.checkRedirectMainSave.Size = new System.Drawing.Size(77, 17);
			this.checkRedirectMainSave.TabIndex = 9;
			this.checkRedirectMainSave.Text = "Main Save";
			this.checkRedirectMainSave.UseVisualStyleBackColor = true;
			// 
			// checkRedirectChaoSave
			// 
			this.checkRedirectChaoSave.AutoSize = true;
			this.checkRedirectChaoSave.Location = new System.Drawing.Point(179, 163);
			this.checkRedirectChaoSave.Name = "checkRedirectChaoSave";
			this.checkRedirectChaoSave.Size = new System.Drawing.Size(79, 17);
			this.checkRedirectChaoSave.TabIndex = 10;
			this.checkRedirectChaoSave.Text = "Chao Save";
			this.checkRedirectChaoSave.UseVisualStyleBackColor = true;
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(12, 164);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(78, 13);
			this.label5.TabIndex = 8;
			this.label5.Text = "Save Redirect:";
			// 
			// NewModDialog
			// 
			this.AcceptButton = this.buttonOK;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.buttonCancel;
			this.ClientSize = new System.Drawing.Size(304, 220);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.checkRedirectChaoSave);
			this.Controls.Add(this.checkRedirectMainSave);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.textVersion);
			this.Controls.Add(this.textModDescription);
			this.Controls.Add(this.checkOpenFolder);
			this.Controls.Add(this.textModAuthor);
			this.Controls.Add(this.textModName);
			this.Controls.Add(this.buttonCancel);
			this.Controls.Add(this.buttonOK);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "NewModDialog";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "New Mod";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Button buttonOK;
		private System.Windows.Forms.Button buttonCancel;
		private System.Windows.Forms.TextBox textModName;
		private System.Windows.Forms.TextBox textModAuthor;
		private System.Windows.Forms.CheckBox checkOpenFolder;
		private System.Windows.Forms.ToolTip toolTip1;
		private System.Windows.Forms.TextBox textModDescription;
        private System.Windows.Forms.TextBox textVersion;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.CheckBox checkRedirectMainSave;
        private System.Windows.Forms.CheckBox checkRedirectChaoSave;
        private System.Windows.Forms.Label label5;
    }
}