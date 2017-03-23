namespace SADXModManager.Forms
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
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.tabPage1 = new System.Windows.Forms.TabPage();
			this.tabPage2 = new System.Windows.Forms.TabPage();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.textGitHubRepo = new System.Windows.Forms.TextBox();
			this.textGitHubAttachment = new System.Windows.Forms.TextBox();
			this.label6 = new System.Windows.Forms.Label();
			this.label7 = new System.Windows.Forms.Label();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.textDirUrl = new System.Windows.Forms.TextBox();
			this.label8 = new System.Windows.Forms.Label();
			this.tabControl1.SuspendLayout();
			this.tabPage1.SuspendLayout();
			this.tabPage2.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(22, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(62, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "Mod Name:";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(21, 89);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(63, 13);
			this.label2.TabIndex = 6;
			this.label2.Text = "Description:";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(43, 37);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(41, 13);
			this.label3.TabIndex = 2;
			this.label3.Text = "Author:";
			// 
			// buttonOK
			// 
			this.buttonOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonOK.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.buttonOK.Location = new System.Drawing.Point(216, 250);
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
			this.buttonCancel.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.buttonCancel.Location = new System.Drawing.Point(297, 250);
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
			this.textModName.Location = new System.Drawing.Point(90, 6);
			this.textModName.Name = "textModName";
			this.textModName.Size = new System.Drawing.Size(256, 20);
			this.textModName.TabIndex = 1;
			// 
			// textModAuthor
			// 
			this.textModAuthor.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textModAuthor.Location = new System.Drawing.Point(90, 34);
			this.textModAuthor.Name = "textModAuthor";
			this.textModAuthor.Size = new System.Drawing.Size(256, 20);
			this.textModAuthor.TabIndex = 3;
			// 
			// checkOpenFolder
			// 
			this.checkOpenFolder.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.checkOpenFolder.AutoSize = true;
			this.checkOpenFolder.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.checkOpenFolder.Location = new System.Drawing.Point(12, 255);
			this.checkOpenFolder.Name = "checkOpenFolder";
			this.checkOpenFolder.Size = new System.Drawing.Size(87, 18);
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
			this.textModDescription.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textModDescription.Location = new System.Drawing.Point(90, 86);
			this.textModDescription.Multiline = true;
			this.textModDescription.Name = "textModDescription";
			this.textModDescription.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.textModDescription.Size = new System.Drawing.Size(256, 64);
			this.textModDescription.TabIndex = 7;
			// 
			// textVersion
			// 
			this.textVersion.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textVersion.Location = new System.Drawing.Point(90, 60);
			this.textVersion.Name = "textVersion";
			this.textVersion.Size = new System.Drawing.Size(256, 20);
			this.textVersion.TabIndex = 5;
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(39, 63);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(45, 13);
			this.label4.TabIndex = 4;
			this.label4.Text = "Version:";
			// 
			// checkRedirectMainSave
			// 
			this.checkRedirectMainSave.AutoSize = true;
			this.checkRedirectMainSave.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.checkRedirectMainSave.Location = new System.Drawing.Point(90, 156);
			this.checkRedirectMainSave.Name = "checkRedirectMainSave";
			this.checkRedirectMainSave.Size = new System.Drawing.Size(83, 18);
			this.checkRedirectMainSave.TabIndex = 9;
			this.checkRedirectMainSave.Text = "Main Save";
			this.checkRedirectMainSave.UseVisualStyleBackColor = true;
			// 
			// checkRedirectChaoSave
			// 
			this.checkRedirectChaoSave.AutoSize = true;
			this.checkRedirectChaoSave.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.checkRedirectChaoSave.Location = new System.Drawing.Point(90, 180);
			this.checkRedirectChaoSave.Name = "checkRedirectChaoSave";
			this.checkRedirectChaoSave.Size = new System.Drawing.Size(85, 18);
			this.checkRedirectChaoSave.TabIndex = 10;
			this.checkRedirectChaoSave.Text = "Chao Save";
			this.checkRedirectChaoSave.UseVisualStyleBackColor = true;
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(6, 158);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(78, 13);
			this.label5.TabIndex = 8;
			this.label5.Text = "Save Redirect:";
			// 
			// tabControl1
			// 
			this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tabControl1.Controls.Add(this.tabPage1);
			this.tabControl1.Controls.Add(this.tabPage2);
			this.tabControl1.Location = new System.Drawing.Point(12, 12);
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(360, 232);
			this.tabControl1.TabIndex = 14;
			// 
			// tabPage1
			// 
			this.tabPage1.Controls.Add(this.label1);
			this.tabPage1.Controls.Add(this.label5);
			this.tabPage1.Controls.Add(this.label2);
			this.tabPage1.Controls.Add(this.checkRedirectChaoSave);
			this.tabPage1.Controls.Add(this.label3);
			this.tabPage1.Controls.Add(this.checkRedirectMainSave);
			this.tabPage1.Controls.Add(this.textModName);
			this.tabPage1.Controls.Add(this.label4);
			this.tabPage1.Controls.Add(this.textModAuthor);
			this.tabPage1.Controls.Add(this.textVersion);
			this.tabPage1.Controls.Add(this.textModDescription);
			this.tabPage1.Location = new System.Drawing.Point(4, 22);
			this.tabPage1.Name = "tabPage1";
			this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
			this.tabPage1.Size = new System.Drawing.Size(352, 206);
			this.tabPage1.TabIndex = 0;
			this.tabPage1.Text = "Properties";
			this.tabPage1.UseVisualStyleBackColor = true;
			// 
			// tabPage2
			// 
			this.tabPage2.Controls.Add(this.groupBox2);
			this.tabPage2.Controls.Add(this.groupBox1);
			this.tabPage2.Location = new System.Drawing.Point(4, 22);
			this.tabPage2.Name = "tabPage2";
			this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
			this.tabPage2.Size = new System.Drawing.Size(352, 206);
			this.tabPage2.TabIndex = 1;
			this.tabPage2.Text = "Updates";
			this.tabPage2.UseVisualStyleBackColor = true;
			// 
			// groupBox1
			// 
			this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox1.Controls.Add(this.label7);
			this.groupBox1.Controls.Add(this.label6);
			this.groupBox1.Controls.Add(this.textGitHubAttachment);
			this.groupBox1.Controls.Add(this.textGitHubRepo);
			this.groupBox1.Location = new System.Drawing.Point(6, 7);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(340, 71);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "GitHub";
			// 
			// textGitHubRepo
			// 
			this.textGitHubRepo.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textGitHubRepo.Location = new System.Drawing.Point(103, 19);
			this.textGitHubRepo.Name = "textGitHubRepo";
			this.textGitHubRepo.Size = new System.Drawing.Size(231, 20);
			this.textGitHubRepo.TabIndex = 0;
			// 
			// textGitHubAttachment
			// 
			this.textGitHubAttachment.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textGitHubAttachment.Location = new System.Drawing.Point(103, 45);
			this.textGitHubAttachment.Name = "textGitHubAttachment";
			this.textGitHubAttachment.Size = new System.Drawing.Size(231, 20);
			this.textGitHubAttachment.TabIndex = 1;
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(6, 22);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(91, 13);
			this.label6.TabIndex = 2;
			this.label6.Text = "Repo (user/repo):";
			// 
			// label7
			// 
			this.label7.AutoSize = true;
			this.label7.Location = new System.Drawing.Point(33, 48);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(64, 13);
			this.label7.TabIndex = 3;
			this.label7.Text = "Attachment:";
			// 
			// groupBox2
			// 
			this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox2.Controls.Add(this.label8);
			this.groupBox2.Controls.Add(this.textDirUrl);
			this.groupBox2.Location = new System.Drawing.Point(6, 85);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(340, 46);
			this.groupBox2.TabIndex = 1;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Self-Hosted Directory";
			// 
			// textDirUrl
			// 
			this.textDirUrl.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textDirUrl.Location = new System.Drawing.Point(103, 19);
			this.textDirUrl.Name = "textDirUrl";
			this.textDirUrl.Size = new System.Drawing.Size(231, 20);
			this.textDirUrl.TabIndex = 0;
			// 
			// label8
			// 
			this.label8.AutoSize = true;
			this.label8.Location = new System.Drawing.Point(20, 22);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(77, 13);
			this.label8.TabIndex = 1;
			this.label8.Text = "Directory URL:";
			// 
			// NewModDialog
			// 
			this.AcceptButton = this.buttonOK;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.buttonCancel;
			this.ClientSize = new System.Drawing.Size(384, 285);
			this.Controls.Add(this.tabControl1);
			this.Controls.Add(this.checkOpenFolder);
			this.Controls.Add(this.buttonCancel);
			this.Controls.Add(this.buttonOK);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "NewModDialog";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "New Mod";
			this.tabControl1.ResumeLayout(false);
			this.tabPage1.ResumeLayout(false);
			this.tabPage1.PerformLayout();
			this.tabPage2.ResumeLayout(false);
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.groupBox2.ResumeLayout(false);
			this.groupBox2.PerformLayout();
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
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage tabPage1;
		private System.Windows.Forms.TabPage tabPage2;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.TextBox textDirUrl;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.TextBox textGitHubAttachment;
		private System.Windows.Forms.TextBox textGitHubRepo;
	}
}