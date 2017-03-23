namespace SADXModManager.Controls
{
	partial class ModUpdateDetails
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

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.groupDownloadDetails = new System.Windows.Forms.GroupBox();
			this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
			this.label1 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.labelDownloadPublished = new System.Windows.Forms.Label();
			this.labelSize = new System.Windows.Forms.Label();
			this.labelFileCount = new System.Windows.Forms.Label();
			this.groupReleaseDetails = new System.Windows.Forms.GroupBox();
			this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
			this.label6 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.labelReleaseName = new System.Windows.Forms.Label();
			this.labelReleaseTag = new System.Windows.Forms.Label();
			this.linkRelease = new System.Windows.Forms.LinkLabel();
			this.label7 = new System.Windows.Forms.Label();
			this.labelReleasePublished = new System.Windows.Forms.Label();
			this.groupDownloadDetails.SuspendLayout();
			this.tableLayoutPanel1.SuspendLayout();
			this.groupReleaseDetails.SuspendLayout();
			this.tableLayoutPanel2.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupDownloadDetails
			// 
			this.groupDownloadDetails.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupDownloadDetails.Controls.Add(this.tableLayoutPanel1);
			this.groupDownloadDetails.Location = new System.Drawing.Point(3, 3);
			this.groupDownloadDetails.Name = "groupDownloadDetails";
			this.groupDownloadDetails.Size = new System.Drawing.Size(314, 64);
			this.groupDownloadDetails.TabIndex = 0;
			this.groupDownloadDetails.TabStop = false;
			this.groupDownloadDetails.Text = "Download Details";
			// 
			// tableLayoutPanel1
			// 
			this.tableLayoutPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tableLayoutPanel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.tableLayoutPanel1.ColumnCount = 2;
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			this.tableLayoutPanel1.Controls.Add(this.label1, 0, 0);
			this.tableLayoutPanel1.Controls.Add(this.label3, 0, 2);
			this.tableLayoutPanel1.Controls.Add(this.label2, 0, 1);
			this.tableLayoutPanel1.Controls.Add(this.labelDownloadPublished, 1, 0);
			this.tableLayoutPanel1.Controls.Add(this.labelSize, 1, 1);
			this.tableLayoutPanel1.Controls.Add(this.labelFileCount, 1, 2);
			this.tableLayoutPanel1.Location = new System.Drawing.Point(3, 16);
			this.tableLayoutPanel1.Name = "tableLayoutPanel1";
			this.tableLayoutPanel1.RowCount = 3;
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
			this.tableLayoutPanel1.Size = new System.Drawing.Size(305, 39);
			this.tableLayoutPanel1.TabIndex = 3;
			// 
			// label1
			// 
			this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(3, 0);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(56, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "Published:";
			// 
			// label3
			// 
			this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(28, 26);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(31, 13);
			this.label3.TabIndex = 2;
			this.label3.Text = "Files:";
			// 
			// label2
			// 
			this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(29, 13);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(30, 13);
			this.label2.TabIndex = 1;
			this.label2.Text = "Size:";
			// 
			// labelDownloadPublished
			// 
			this.labelDownloadPublished.AutoSize = true;
			this.labelDownloadPublished.Location = new System.Drawing.Point(65, 0);
			this.labelDownloadPublished.Name = "labelDownloadPublished";
			this.labelDownloadPublished.Size = new System.Drawing.Size(123, 13);
			this.labelDownloadPublished.TabIndex = 3;
			this.labelDownloadPublished.Text = "labelDownloadPublished";
			// 
			// labelSize
			// 
			this.labelSize.AutoSize = true;
			this.labelSize.Location = new System.Drawing.Point(65, 13);
			this.labelSize.Name = "labelSize";
			this.labelSize.Size = new System.Drawing.Size(49, 13);
			this.labelSize.TabIndex = 4;
			this.labelSize.Text = "labelSize";
			// 
			// labelFileCount
			// 
			this.labelFileCount.AutoSize = true;
			this.labelFileCount.Location = new System.Drawing.Point(65, 26);
			this.labelFileCount.Name = "labelFileCount";
			this.labelFileCount.Size = new System.Drawing.Size(73, 13);
			this.labelFileCount.TabIndex = 5;
			this.labelFileCount.Text = "labelFileCount";
			// 
			// groupReleaseDetails
			// 
			this.groupReleaseDetails.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupReleaseDetails.Controls.Add(this.tableLayoutPanel2);
			this.groupReleaseDetails.Location = new System.Drawing.Point(3, 73);
			this.groupReleaseDetails.Name = "groupReleaseDetails";
			this.groupReleaseDetails.Size = new System.Drawing.Size(314, 74);
			this.groupReleaseDetails.TabIndex = 1;
			this.groupReleaseDetails.TabStop = false;
			this.groupReleaseDetails.Text = "Release Details";
			// 
			// tableLayoutPanel2
			// 
			this.tableLayoutPanel2.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.tableLayoutPanel2.ColumnCount = 2;
			this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			this.tableLayoutPanel2.Controls.Add(this.label7, 0, 0);
			this.tableLayoutPanel2.Controls.Add(this.labelReleasePublished, 1, 0);
			this.tableLayoutPanel2.Controls.Add(this.label6, 0, 3);
			this.tableLayoutPanel2.Controls.Add(this.label5, 0, 1);
			this.tableLayoutPanel2.Controls.Add(this.label4, 0, 2);
			this.tableLayoutPanel2.Controls.Add(this.labelReleaseName, 1, 2);
			this.tableLayoutPanel2.Controls.Add(this.linkRelease, 1, 1);
			this.tableLayoutPanel2.Controls.Add(this.labelReleaseTag, 1, 3);
			this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tableLayoutPanel2.Location = new System.Drawing.Point(3, 16);
			this.tableLayoutPanel2.Name = "tableLayoutPanel2";
			this.tableLayoutPanel2.RowCount = 4;
			this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.tableLayoutPanel2.Size = new System.Drawing.Size(308, 55);
			this.tableLayoutPanel2.TabIndex = 4;
			// 
			// label6
			// 
			this.label6.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(30, 39);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(29, 13);
			this.label6.TabIndex = 1;
			this.label6.Text = "Tag:";
			// 
			// label5
			// 
			this.label5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(24, 13);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(35, 13);
			this.label5.TabIndex = 5;
			this.label5.Text = "Page:";
			// 
			// label4
			// 
			this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(21, 26);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(38, 13);
			this.label4.TabIndex = 0;
			this.label4.Text = "Name:";
			// 
			// labelReleaseName
			// 
			this.labelReleaseName.AutoSize = true;
			this.labelReleaseName.Location = new System.Drawing.Point(65, 26);
			this.labelReleaseName.Name = "labelReleaseName";
			this.labelReleaseName.Size = new System.Drawing.Size(96, 13);
			this.labelReleaseName.TabIndex = 3;
			this.labelReleaseName.Text = "labelReleaseName";
			// 
			// labelReleaseTag
			// 
			this.labelReleaseTag.AutoSize = true;
			this.labelReleaseTag.Location = new System.Drawing.Point(65, 39);
			this.labelReleaseTag.Name = "labelReleaseTag";
			this.labelReleaseTag.Size = new System.Drawing.Size(87, 13);
			this.labelReleaseTag.TabIndex = 4;
			this.labelReleaseTag.Text = "labelReleaseTag";
			// 
			// linkRelease
			// 
			this.linkRelease.AutoSize = true;
			this.linkRelease.Location = new System.Drawing.Point(65, 13);
			this.linkRelease.Name = "linkRelease";
			this.linkRelease.Size = new System.Drawing.Size(62, 13);
			this.linkRelease.TabIndex = 6;
			this.linkRelease.TabStop = true;
			this.linkRelease.Text = "linkRelease";
			this.linkRelease.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkRelease_LinkClicked);
			// 
			// label7
			// 
			this.label7.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.label7.AutoSize = true;
			this.label7.Location = new System.Drawing.Point(3, 0);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(56, 13);
			this.label7.TabIndex = 7;
			this.label7.Text = "Published:";
			// 
			// labelReleasePublished
			// 
			this.labelReleasePublished.AutoSize = true;
			this.labelReleasePublished.Location = new System.Drawing.Point(65, 0);
			this.labelReleasePublished.Name = "labelReleasePublished";
			this.labelReleasePublished.Size = new System.Drawing.Size(114, 13);
			this.labelReleasePublished.TabIndex = 8;
			this.labelReleasePublished.Text = "labelReleasePublished";
			// 
			// ModUpdateDetails
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.groupReleaseDetails);
			this.Controls.Add(this.groupDownloadDetails);
			this.Margin = new System.Windows.Forms.Padding(0);
			this.Name = "ModUpdateDetails";
			this.Size = new System.Drawing.Size(320, 150);
			this.groupDownloadDetails.ResumeLayout(false);
			this.tableLayoutPanel1.ResumeLayout(false);
			this.tableLayoutPanel1.PerformLayout();
			this.groupReleaseDetails.ResumeLayout(false);
			this.tableLayoutPanel2.ResumeLayout(false);
			this.tableLayoutPanel2.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox groupDownloadDetails;
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label labelDownloadPublished;
		private System.Windows.Forms.Label labelSize;
		private System.Windows.Forms.Label labelFileCount;
		private System.Windows.Forms.GroupBox groupReleaseDetails;
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label labelReleaseName;
		private System.Windows.Forms.Label labelReleaseTag;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.LinkLabel linkRelease;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Label labelReleasePublished;
	}
}
