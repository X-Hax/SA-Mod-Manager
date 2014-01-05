namespace SADXModManager
{
    partial class MainForm
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
			System.Windows.Forms.GroupBox groupBox1;
			System.Windows.Forms.GroupBox groupBox2;
			System.Windows.Forms.Label label1;
			this.modDescription = new System.Windows.Forms.Label();
			this.modDownButton = new System.Windows.Forms.Button();
			this.modUpButton = new System.Windows.Forms.Button();
			this.modListView = new System.Windows.Forms.ListView();
			this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.verticalResolution = new System.Windows.Forms.NumericUpDown();
			this.horizontalResolution = new System.Windows.Forms.NumericUpDown();
			this.useCustomResolutionCheckBox = new System.Windows.Forms.CheckBox();
			this.disableCDCheckCheckBox = new System.Windows.Forms.CheckBox();
			this.dontFixWindowCheckBox = new System.Windows.Forms.CheckBox();
			this.SADXDebugCheckBox = new System.Windows.Forms.CheckBox();
			this.consoleCheckBox = new System.Windows.Forms.CheckBox();
			this.saveButton = new System.Windows.Forms.Button();
			this.saveAndPlayButton = new System.Windows.Forms.Button();
			this.installButton = new System.Windows.Forms.Button();
			this.configEditorButton = new System.Windows.Forms.Button();
			groupBox1 = new System.Windows.Forms.GroupBox();
			groupBox2 = new System.Windows.Forms.GroupBox();
			label1 = new System.Windows.Forms.Label();
			groupBox1.SuspendLayout();
			groupBox2.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.verticalResolution)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.horizontalResolution)).BeginInit();
			this.SuspendLayout();
			// 
			// groupBox1
			// 
			groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			groupBox1.Controls.Add(this.modDescription);
			groupBox1.Controls.Add(this.modDownButton);
			groupBox1.Controls.Add(this.modUpButton);
			groupBox1.Controls.Add(this.modListView);
			groupBox1.Location = new System.Drawing.Point(12, 12);
			groupBox1.Name = "groupBox1";
			groupBox1.Size = new System.Drawing.Size(360, 334);
			groupBox1.TabIndex = 1;
			groupBox1.TabStop = false;
			groupBox1.Text = "Mods";
			// 
			// modDescription
			// 
			this.modDescription.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.modDescription.Location = new System.Drawing.Point(7, 266);
			this.modDescription.Name = "modDescription";
			this.modDescription.Size = new System.Drawing.Size(347, 54);
			this.modDescription.TabIndex = 4;
			this.modDescription.Text = "Description: No mod selected.";
			// 
			// modDownButton
			// 
			this.modDownButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.modDownButton.AutoSize = true;
			this.modDownButton.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.modDownButton.Enabled = false;
			this.modDownButton.Location = new System.Drawing.Point(325, 48);
			this.modDownButton.Name = "modDownButton";
			this.modDownButton.Size = new System.Drawing.Size(29, 23);
			this.modDownButton.TabIndex = 2;
			this.modDownButton.Text = "↓";
			this.modDownButton.UseVisualStyleBackColor = true;
			this.modDownButton.Click += new System.EventHandler(this.modDownButton_Click);
			// 
			// modUpButton
			// 
			this.modUpButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.modUpButton.AutoSize = true;
			this.modUpButton.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.modUpButton.Enabled = false;
			this.modUpButton.Location = new System.Drawing.Point(325, 19);
			this.modUpButton.Name = "modUpButton";
			this.modUpButton.Size = new System.Drawing.Size(29, 23);
			this.modUpButton.TabIndex = 1;
			this.modUpButton.Text = "↑";
			this.modUpButton.UseVisualStyleBackColor = true;
			this.modUpButton.Click += new System.EventHandler(this.modUpButton_Click);
			// 
			// modListView
			// 
			this.modListView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.modListView.CheckBoxes = true;
			this.modListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
			this.modListView.FullRowSelect = true;
			this.modListView.HideSelection = false;
			this.modListView.Location = new System.Drawing.Point(6, 19);
			this.modListView.MultiSelect = false;
			this.modListView.Name = "modListView";
			this.modListView.Size = new System.Drawing.Size(313, 240);
			this.modListView.TabIndex = 0;
			this.modListView.UseCompatibleStateImageBehavior = false;
			this.modListView.View = System.Windows.Forms.View.Details;
			this.modListView.SelectedIndexChanged += new System.EventHandler(this.modListView_SelectedIndexChanged);
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "Name";
			this.columnHeader1.Width = 154;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "Author";
			this.columnHeader2.Width = 120;
			// 
			// groupBox2
			// 
			groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			groupBox2.Controls.Add(label1);
			groupBox2.Controls.Add(this.verticalResolution);
			groupBox2.Controls.Add(this.horizontalResolution);
			groupBox2.Controls.Add(this.useCustomResolutionCheckBox);
			groupBox2.Controls.Add(this.disableCDCheckCheckBox);
			groupBox2.Controls.Add(this.dontFixWindowCheckBox);
			groupBox2.Controls.Add(this.SADXDebugCheckBox);
			groupBox2.Controls.Add(this.consoleCheckBox);
			groupBox2.Location = new System.Drawing.Point(12, 352);
			groupBox2.Name = "groupBox2";
			groupBox2.Size = new System.Drawing.Size(360, 92);
			groupBox2.TabIndex = 2;
			groupBox2.TabStop = false;
			groupBox2.Text = "Options";
			// 
			// label1
			// 
			label1.AutoSize = true;
			label1.Location = new System.Drawing.Point(219, 66);
			label1.Name = "label1";
			label1.Size = new System.Drawing.Size(12, 13);
			label1.TabIndex = 7;
			label1.Text = "x";
			// 
			// verticalResolution
			// 
			this.verticalResolution.Location = new System.Drawing.Point(237, 64);
			this.verticalResolution.Maximum = new decimal(new int[] {
            2147483647,
            0,
            0,
            0});
			this.verticalResolution.Minimum = new decimal(new int[] {
            480,
            0,
            0,
            0});
			this.verticalResolution.Name = "verticalResolution";
			this.verticalResolution.Size = new System.Drawing.Size(62, 20);
			this.verticalResolution.TabIndex = 6;
			this.verticalResolution.Value = new decimal(new int[] {
            480,
            0,
            0,
            0});
			// 
			// horizontalResolution
			// 
			this.horizontalResolution.Location = new System.Drawing.Point(151, 64);
			this.horizontalResolution.Maximum = new decimal(new int[] {
            2147483647,
            0,
            0,
            0});
			this.horizontalResolution.Minimum = new decimal(new int[] {
            640,
            0,
            0,
            0});
			this.horizontalResolution.Name = "horizontalResolution";
			this.horizontalResolution.Size = new System.Drawing.Size(62, 20);
			this.horizontalResolution.TabIndex = 5;
			this.horizontalResolution.Value = new decimal(new int[] {
            640,
            0,
            0,
            0});
			// 
			// useCustomResolutionCheckBox
			// 
			this.useCustomResolutionCheckBox.AutoSize = true;
			this.useCustomResolutionCheckBox.Location = new System.Drawing.Point(6, 65);
			this.useCustomResolutionCheckBox.Name = "useCustomResolutionCheckBox";
			this.useCustomResolutionCheckBox.Size = new System.Drawing.Size(139, 17);
			this.useCustomResolutionCheckBox.TabIndex = 4;
			this.useCustomResolutionCheckBox.Text = "Use Custom Resolution:";
			this.useCustomResolutionCheckBox.UseVisualStyleBackColor = true;
			this.useCustomResolutionCheckBox.CheckedChanged += new System.EventHandler(this.useCustomResolutionCheckBox_CheckedChanged);
			// 
			// disableCDCheckCheckBox
			// 
			this.disableCDCheckCheckBox.AutoSize = true;
			this.disableCDCheckCheckBox.Location = new System.Drawing.Point(121, 42);
			this.disableCDCheckCheckBox.Name = "disableCDCheckCheckBox";
			this.disableCDCheckCheckBox.Size = new System.Drawing.Size(113, 17);
			this.disableCDCheckCheckBox.TabIndex = 3;
			this.disableCDCheckCheckBox.Text = "Disable CD Check";
			this.disableCDCheckCheckBox.UseVisualStyleBackColor = true;
			// 
			// dontFixWindowCheckBox
			// 
			this.dontFixWindowCheckBox.AutoSize = true;
			this.dontFixWindowCheckBox.Location = new System.Drawing.Point(6, 42);
			this.dontFixWindowCheckBox.Name = "dontFixWindowCheckBox";
			this.dontFixWindowCheckBox.Size = new System.Drawing.Size(109, 17);
			this.dontFixWindowCheckBox.TabIndex = 2;
			this.dontFixWindowCheckBox.Text = "Don\'t Fix Window";
			this.dontFixWindowCheckBox.UseVisualStyleBackColor = true;
			// 
			// SADXDebugCheckBox
			// 
			this.SADXDebugCheckBox.AutoSize = true;
			this.SADXDebugCheckBox.Location = new System.Drawing.Point(148, 19);
			this.SADXDebugCheckBox.Name = "SADXDebugCheckBox";
			this.SADXDebugCheckBox.Size = new System.Drawing.Size(155, 17);
			this.SADXDebugCheckBox.TabIndex = 1;
			this.SADXDebugCheckBox.Text = "Show SADX Debug Output";
			this.SADXDebugCheckBox.UseVisualStyleBackColor = true;
			// 
			// consoleCheckBox
			// 
			this.consoleCheckBox.AutoSize = true;
			this.consoleCheckBox.Location = new System.Drawing.Point(6, 19);
			this.consoleCheckBox.Name = "consoleCheckBox";
			this.consoleCheckBox.Size = new System.Drawing.Size(136, 17);
			this.consoleCheckBox.TabIndex = 0;
			this.consoleCheckBox.Text = "Show Console Window";
			this.consoleCheckBox.UseVisualStyleBackColor = true;
			// 
			// saveButton
			// 
			this.saveButton.Location = new System.Drawing.Point(104, 450);
			this.saveButton.Name = "saveButton";
			this.saveButton.Size = new System.Drawing.Size(75, 23);
			this.saveButton.TabIndex = 3;
			this.saveButton.Text = "Save";
			this.saveButton.UseVisualStyleBackColor = true;
			this.saveButton.Click += new System.EventHandler(this.saveButton_Click);
			// 
			// saveAndPlayButton
			// 
			this.saveAndPlayButton.AutoSize = true;
			this.saveAndPlayButton.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.saveAndPlayButton.Location = new System.Drawing.Point(12, 450);
			this.saveAndPlayButton.Name = "saveAndPlayButton";
			this.saveAndPlayButton.Size = new System.Drawing.Size(86, 23);
			this.saveAndPlayButton.TabIndex = 4;
			this.saveAndPlayButton.Text = "Save and Play";
			this.saveAndPlayButton.UseVisualStyleBackColor = true;
			this.saveAndPlayButton.Click += new System.EventHandler(this.saveAndPlayButton_Click);
			// 
			// installButton
			// 
			this.installButton.AutoSize = true;
			this.installButton.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.installButton.Location = new System.Drawing.Point(185, 450);
			this.installButton.Name = "installButton";
			this.installButton.Size = new System.Drawing.Size(76, 23);
			this.installButton.TabIndex = 5;
			this.installButton.Text = "Install loader";
			this.installButton.UseVisualStyleBackColor = true;
			this.installButton.Click += new System.EventHandler(this.installButton_Click);
			// 
			// configEditorButton
			// 
			this.configEditorButton.AutoSize = true;
			this.configEditorButton.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.configEditorButton.Location = new System.Drawing.Point(295, 450);
			this.configEditorButton.Name = "configEditorButton";
			this.configEditorButton.Size = new System.Drawing.Size(77, 23);
			this.configEditorButton.TabIndex = 6;
			this.configEditorButton.Text = "Config Editor";
			this.configEditorButton.UseVisualStyleBackColor = true;
			this.configEditorButton.Click += new System.EventHandler(this.configEditorButton_Click);
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoSize = true;
			this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.ClientSize = new System.Drawing.Size(384, 488);
			this.Controls.Add(this.configEditorButton);
			this.Controls.Add(this.installButton);
			this.Controls.Add(this.saveAndPlayButton);
			this.Controls.Add(this.saveButton);
			this.Controls.Add(groupBox2);
			this.Controls.Add(groupBox1);
			this.MaximizeBox = false;
			this.Name = "MainForm";
			this.Padding = new System.Windows.Forms.Padding(0, 0, 0, 3);
			this.ShowIcon = false;
			this.Text = "SADX Mod Manager";
			this.Load += new System.EventHandler(this.MainForm_Load);
			groupBox1.ResumeLayout(false);
			groupBox1.PerformLayout();
			groupBox2.ResumeLayout(false);
			groupBox2.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.verticalResolution)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.horizontalResolution)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListView modListView;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.Button modDownButton;
        private System.Windows.Forms.Button modUpButton;
        private System.Windows.Forms.CheckBox consoleCheckBox;
        private System.Windows.Forms.Button saveButton;
        private System.Windows.Forms.Button saveAndPlayButton;
        private System.Windows.Forms.Button installButton;
        private System.Windows.Forms.CheckBox SADXDebugCheckBox;
        private System.Windows.Forms.CheckBox disableCDCheckCheckBox;
        private System.Windows.Forms.CheckBox dontFixWindowCheckBox;
        private System.Windows.Forms.CheckBox useCustomResolutionCheckBox;
        private System.Windows.Forms.NumericUpDown horizontalResolution;
        private System.Windows.Forms.NumericUpDown verticalResolution;
        private System.Windows.Forms.Button configEditorButton;
        private System.Windows.Forms.Label modDescription;
    }
}

