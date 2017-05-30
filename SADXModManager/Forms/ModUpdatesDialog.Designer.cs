namespace SADXModManager.Forms
{
	partial class ModUpdatesDialog
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
			this.buttonInstall = new System.Windows.Forms.Button();
			this.buttonLater = new System.Windows.Forms.Button();
			this.listModUpdates = new System.Windows.Forms.ListView();
			this.columnName = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.tabPageChanges = new System.Windows.Forms.TabPage();
			this.textChangeLog = new System.Windows.Forms.RichTextBox();
			this.tabPageDetails = new System.Windows.Forms.TabPage();
			this.modUpdateDetails = new SADXModManager.Controls.ModUpdateDetails();
			this.tabPageFiles = new System.Windows.Forms.TabPage();
			this.listFiles = new System.Windows.Forms.ListView();
			this.columnStatus = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnFile = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			this.tabControl1.SuspendLayout();
			this.tabPageChanges.SuspendLayout();
			this.tabPageDetails.SuspendLayout();
			this.tabPageFiles.SuspendLayout();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(169, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "The following mods have updates:";
			// 
			// buttonInstall
			// 
			this.buttonInstall.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonInstall.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.buttonInstall.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.buttonInstall.Location = new System.Drawing.Point(456, 246);
			this.buttonInstall.Name = "buttonInstall";
			this.buttonInstall.Size = new System.Drawing.Size(75, 23);
			this.buttonInstall.TabIndex = 2;
			this.buttonInstall.Text = "&Install now";
			this.buttonInstall.UseVisualStyleBackColor = true;
			this.buttonInstall.Click += new System.EventHandler(this.buttonInstall_Click);
			// 
			// buttonLater
			// 
			this.buttonLater.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.buttonLater.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.buttonLater.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.buttonLater.Location = new System.Drawing.Point(537, 246);
			this.buttonLater.Name = "buttonLater";
			this.buttonLater.Size = new System.Drawing.Size(75, 23);
			this.buttonLater.TabIndex = 3;
			this.buttonLater.Text = "Install &later";
			this.buttonLater.UseVisualStyleBackColor = true;
			// 
			// listModUpdates
			// 
			this.listModUpdates.CheckBoxes = true;
			this.listModUpdates.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnName});
			this.listModUpdates.Dock = System.Windows.Forms.DockStyle.Fill;
			this.listModUpdates.FullRowSelect = true;
			this.listModUpdates.HideSelection = false;
			this.listModUpdates.Location = new System.Drawing.Point(0, 0);
			this.listModUpdates.Name = "listModUpdates";
			this.listModUpdates.Size = new System.Drawing.Size(224, 215);
			this.listModUpdates.TabIndex = 0;
			this.listModUpdates.UseCompatibleStateImageBehavior = false;
			this.listModUpdates.View = System.Windows.Forms.View.Details;
			this.listModUpdates.ItemChecked += new System.Windows.Forms.ItemCheckedEventHandler(this.listModUpdates_ItemChecked);
			this.listModUpdates.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.listModUpdates_ItemSelectionChanged);
			// 
			// columnName
			// 
			this.columnName.Text = "Name";
			this.columnName.Width = 192;
			// 
			// splitContainer1
			// 
			this.splitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
			this.splitContainer1.Location = new System.Drawing.Point(12, 25);
			this.splitContainer1.Name = "splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.listModUpdates);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.tabControl1);
			this.splitContainer1.Size = new System.Drawing.Size(600, 215);
			this.splitContainer1.SplitterDistance = 224;
			this.splitContainer1.TabIndex = 1;
			// 
			// tabControl1
			// 
			this.tabControl1.Controls.Add(this.tabPageChanges);
			this.tabControl1.Controls.Add(this.tabPageFiles);
			this.tabControl1.Controls.Add(this.tabPageDetails);
			this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControl1.Location = new System.Drawing.Point(0, 0);
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(372, 215);
			this.tabControl1.TabIndex = 0;
			// 
			// tabPageChanges
			// 
			this.tabPageChanges.Controls.Add(this.textChangeLog);
			this.tabPageChanges.Location = new System.Drawing.Point(4, 22);
			this.tabPageChanges.Name = "tabPageChanges";
			this.tabPageChanges.Padding = new System.Windows.Forms.Padding(3);
			this.tabPageChanges.Size = new System.Drawing.Size(364, 189);
			this.tabPageChanges.TabIndex = 0;
			this.tabPageChanges.Text = "Changes";
			this.tabPageChanges.UseVisualStyleBackColor = true;
			// 
			// textChangeLog
			// 
			this.textChangeLog.Dock = System.Windows.Forms.DockStyle.Fill;
			this.textChangeLog.Location = new System.Drawing.Point(3, 3);
			this.textChangeLog.Name = "textChangeLog";
			this.textChangeLog.ReadOnly = true;
			this.textChangeLog.Size = new System.Drawing.Size(358, 183);
			this.textChangeLog.TabIndex = 0;
			this.textChangeLog.Text = "";
			// 
			// tabPageDetails
			// 
			this.tabPageDetails.AutoScroll = true;
			this.tabPageDetails.Controls.Add(this.modUpdateDetails);
			this.tabPageDetails.Location = new System.Drawing.Point(4, 22);
			this.tabPageDetails.Name = "tabPageDetails";
			this.tabPageDetails.Padding = new System.Windows.Forms.Padding(3);
			this.tabPageDetails.Size = new System.Drawing.Size(364, 189);
			this.tabPageDetails.TabIndex = 1;
			this.tabPageDetails.Text = "Details";
			this.tabPageDetails.UseVisualStyleBackColor = true;
			// 
			// modUpdateDetails
			// 
			this.modUpdateDetails.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.modUpdateDetails.Enabled = false;
			this.modUpdateDetails.Location = new System.Drawing.Point(3, 0);
			this.modUpdateDetails.Margin = new System.Windows.Forms.Padding(0);
			this.modUpdateDetails.Name = "modUpdateDetails";
			this.modUpdateDetails.Size = new System.Drawing.Size(361, 150);
			this.modUpdateDetails.TabIndex = 0;
			// 
			// tabPageFiles
			// 
			this.tabPageFiles.Controls.Add(this.listFiles);
			this.tabPageFiles.Location = new System.Drawing.Point(4, 22);
			this.tabPageFiles.Name = "tabPageFiles";
			this.tabPageFiles.Padding = new System.Windows.Forms.Padding(3);
			this.tabPageFiles.Size = new System.Drawing.Size(364, 189);
			this.tabPageFiles.TabIndex = 2;
			this.tabPageFiles.Text = "Files";
			this.tabPageFiles.UseVisualStyleBackColor = true;
			// 
			// listFiles
			// 
			this.listFiles.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnStatus,
            this.columnFile});
			this.listFiles.Dock = System.Windows.Forms.DockStyle.Fill;
			this.listFiles.FullRowSelect = true;
			this.listFiles.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.listFiles.Location = new System.Drawing.Point(3, 3);
			this.listFiles.MultiSelect = false;
			this.listFiles.Name = "listFiles";
			this.listFiles.Size = new System.Drawing.Size(358, 183);
			this.listFiles.TabIndex = 0;
			this.listFiles.UseCompatibleStateImageBehavior = false;
			this.listFiles.View = System.Windows.Forms.View.Details;
			// 
			// columnStatus
			// 
			this.columnStatus.Text = "Status";
			this.columnStatus.Width = 82;
			// 
			// columnFile
			// 
			this.columnFile.Text = "File";
			this.columnFile.Width = 248;
			// 
			// ModUpdatesDialog
			// 
			this.AcceptButton = this.buttonInstall;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.buttonLater;
			this.ClientSize = new System.Drawing.Size(624, 281);
			this.Controls.Add(this.splitContainer1);
			this.Controls.Add(this.buttonLater);
			this.Controls.Add(this.buttonInstall);
			this.Controls.Add(this.label1);
			this.MinimumSize = new System.Drawing.Size(320, 320);
			this.Name = "ModUpdatesDialog";
			this.ShowIcon = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Mod Updates Available";
			this.Load += new System.EventHandler(this.ModUpdatesDialog_Load);
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
			this.splitContainer1.ResumeLayout(false);
			this.tabControl1.ResumeLayout(false);
			this.tabPageChanges.ResumeLayout(false);
			this.tabPageDetails.ResumeLayout(false);
			this.tabPageFiles.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button buttonInstall;
		private System.Windows.Forms.Button buttonLater;
		private System.Windows.Forms.ListView listModUpdates;
		private System.Windows.Forms.ColumnHeader columnName;
		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.RichTextBox textChangeLog;
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage tabPageChanges;
		private System.Windows.Forms.TabPage tabPageDetails;
		private Controls.ModUpdateDetails modUpdateDetails;
		private System.Windows.Forms.TabPage tabPageFiles;
		private System.Windows.Forms.ListView listFiles;
		private System.Windows.Forms.ColumnHeader columnStatus;
		private System.Windows.Forms.ColumnHeader columnFile;
	}
}