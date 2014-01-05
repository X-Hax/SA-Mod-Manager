namespace SADXModManager
{
    partial class ConfigEditDialog
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
			this.okButton = new System.Windows.Forms.Button();
			this.cancelButton = new System.Windows.Forms.Button();
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.tabPage_Video = new System.Windows.Forms.TabPage();
			this.groupBox_Graphics = new System.Windows.Forms.GroupBox();
			this.comboFog = new System.Windows.Forms.ComboBox();
			this.comboClip = new System.Windows.Forms.ComboBox();
			this.label5 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.comboFramerate = new System.Windows.Forms.ComboBox();
			this.groupBox_Screen = new System.Windows.Forms.GroupBox();
			this.label2 = new System.Windows.Forms.Label();
			this.comboResolutionPreset = new System.Windows.Forms.ComboBox();
			this.label1 = new System.Windows.Forms.Label();
			this.radioWindowMode = new System.Windows.Forms.RadioButton();
			this.radioFullscreen = new System.Windows.Forms.RadioButton();
			this.tabPage_Sound = new System.Windows.Forms.TabPage();
			this.groupBox_SoundVolume = new System.Windows.Forms.GroupBox();
			this.numericBGMVol = new System.Windows.Forms.NumericUpDown();
			this.numericSoundVol = new System.Windows.Forms.NumericUpDown();
			this.label7 = new System.Windows.Forms.Label();
			this.label6 = new System.Windows.Forms.Label();
			this.groupBox_SoundConfig = new System.Windows.Forms.GroupBox();
			this.checkMusic = new System.Windows.Forms.CheckBox();
			this.checkSound = new System.Windows.Forms.CheckBox();
			this.check3DSound = new System.Windows.Forms.CheckBox();
			this.toolTip_ConfigEditor = new System.Windows.Forms.ToolTip(this.components);
			this.tabControls = new System.Windows.Forms.TabPage();
			this.groupMouseMode = new System.Windows.Forms.GroupBox();
			this.radioMouseModeHold = new System.Windows.Forms.RadioButton();
			this.radioMouseModeRelease = new System.Windows.Forms.RadioButton();
			this.groupMouseButtons = new System.Windows.Forms.GroupBox();
			this.label8 = new System.Windows.Forms.Label();
			this.comboMouseActions = new System.Windows.Forms.ComboBox();
			this.comboMouseButtons = new System.Windows.Forms.ComboBox();
			this.label9 = new System.Windows.Forms.Label();
			this.tabControl1.SuspendLayout();
			this.tabPage_Video.SuspendLayout();
			this.groupBox_Graphics.SuspendLayout();
			this.groupBox_Screen.SuspendLayout();
			this.tabPage_Sound.SuspendLayout();
			this.groupBox_SoundVolume.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.numericBGMVol)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numericSoundVol)).BeginInit();
			this.groupBox_SoundConfig.SuspendLayout();
			this.tabControls.SuspendLayout();
			this.groupMouseMode.SuspendLayout();
			this.groupMouseButtons.SuspendLayout();
			this.SuspendLayout();
			// 
			// okButton
			// 
			this.okButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.okButton.Location = new System.Drawing.Point(136, 311);
			this.okButton.Name = "okButton";
			this.okButton.Size = new System.Drawing.Size(75, 23);
			this.okButton.TabIndex = 100;
			this.okButton.Text = "&OK";
			this.okButton.UseVisualStyleBackColor = true;
			this.okButton.Click += new System.EventHandler(this.okButton_Click);
			// 
			// cancelButton
			// 
			this.cancelButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cancelButton.Location = new System.Drawing.Point(217, 311);
			this.cancelButton.Name = "cancelButton";
			this.cancelButton.Size = new System.Drawing.Size(75, 23);
			this.cancelButton.TabIndex = 101;
			this.cancelButton.Text = "&Cancel";
			this.cancelButton.UseVisualStyleBackColor = true;
			this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
			// 
			// tabControl1
			// 
			this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tabControl1.Controls.Add(this.tabPage_Video);
			this.tabControl1.Controls.Add(this.tabPage_Sound);
			this.tabControl1.Controls.Add(this.tabControls);
			this.tabControl1.Location = new System.Drawing.Point(13, 13);
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(279, 292);
			this.tabControl1.TabIndex = 2;
			// 
			// tabPage_Video
			// 
			this.tabPage_Video.Controls.Add(this.groupBox_Graphics);
			this.tabPage_Video.Controls.Add(this.groupBox_Screen);
			this.tabPage_Video.Location = new System.Drawing.Point(4, 22);
			this.tabPage_Video.Name = "tabPage_Video";
			this.tabPage_Video.Padding = new System.Windows.Forms.Padding(3);
			this.tabPage_Video.Size = new System.Drawing.Size(271, 266);
			this.tabPage_Video.TabIndex = 0;
			this.tabPage_Video.Text = "Video";
			this.tabPage_Video.UseVisualStyleBackColor = true;
			// 
			// groupBox_Graphics
			// 
			this.groupBox_Graphics.Controls.Add(this.comboFog);
			this.groupBox_Graphics.Controls.Add(this.comboClip);
			this.groupBox_Graphics.Controls.Add(this.label5);
			this.groupBox_Graphics.Controls.Add(this.label4);
			this.groupBox_Graphics.Controls.Add(this.label3);
			this.groupBox_Graphics.Controls.Add(this.comboFramerate);
			this.groupBox_Graphics.Location = new System.Drawing.Point(6, 112);
			this.groupBox_Graphics.Name = "groupBox_Graphics";
			this.groupBox_Graphics.Size = new System.Drawing.Size(259, 108);
			this.groupBox_Graphics.TabIndex = 1;
			this.groupBox_Graphics.TabStop = false;
			this.groupBox_Graphics.Text = "Graphics";
			// 
			// comboFog
			// 
			this.comboFog.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboFog.FormattingEnabled = true;
			this.comboFog.Items.AddRange(new object[] {
            "Auto",
            "Emulation"});
			this.comboFog.Location = new System.Drawing.Point(104, 73);
			this.comboFog.Name = "comboFog";
			this.comboFog.Size = new System.Drawing.Size(121, 21);
			this.comboFog.TabIndex = 5;
			this.toolTip_ConfigEditor.SetToolTip(this.comboFog, "Sets fog mode. Recommended: Auto\r\nOnly set to emulation if you have an archaic vi" +
        "deo card from\r\nbefore 2003.");
			// 
			// comboClip
			// 
			this.comboClip.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboClip.FormattingEnabled = true;
			this.comboClip.Items.AddRange(new object[] {
            "Far",
            "Normal",
            "Near"});
			this.comboClip.Location = new System.Drawing.Point(104, 46);
			this.comboClip.Name = "comboClip";
			this.comboClip.Size = new System.Drawing.Size(121, 21);
			this.comboClip.TabIndex = 4;
			this.toolTip_ConfigEditor.SetToolTip(this.comboClip, "Sets the render distance for objects. Recommended: Far\r\nFar shows distant objects" +
        ". Near does not show distant objects.");
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(6, 76);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(76, 13);
			this.label5.TabIndex = 3;
			this.label5.Text = "Fog emulation:";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(30, 49);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(52, 13);
			this.label4.TabIndex = 2;
			this.label4.Text = "Clip level:";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(22, 22);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(60, 13);
			this.label3.TabIndex = 1;
			this.label3.Text = "Frame rate:";
			// 
			// comboFramerate
			// 
			this.comboFramerate.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboFramerate.FormattingEnabled = true;
			this.comboFramerate.Items.AddRange(new object[] {
            "High",
            "Normal",
            "Low"});
			this.comboFramerate.Location = new System.Drawing.Point(104, 19);
			this.comboFramerate.Name = "comboFramerate";
			this.comboFramerate.Size = new System.Drawing.Size(121, 21);
			this.comboFramerate.TabIndex = 3;
			this.toolTip_ConfigEditor.SetToolTip(this.comboFramerate, "Sets the frame rate. Recommended: High\r\nChoose high for 60FPS, normal for 30FPS, " +
        "and low for 15FPS.");
			// 
			// groupBox_Screen
			// 
			this.groupBox_Screen.Controls.Add(this.label2);
			this.groupBox_Screen.Controls.Add(this.comboResolutionPreset);
			this.groupBox_Screen.Controls.Add(this.label1);
			this.groupBox_Screen.Controls.Add(this.radioWindowMode);
			this.groupBox_Screen.Controls.Add(this.radioFullscreen);
			this.groupBox_Screen.Location = new System.Drawing.Point(6, 6);
			this.groupBox_Screen.Name = "groupBox_Screen";
			this.groupBox_Screen.Size = new System.Drawing.Size(259, 100);
			this.groupBox_Screen.TabIndex = 0;
			this.groupBox_Screen.TabStop = false;
			this.groupBox_Screen.Text = "Screen";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(6, 69);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(92, 13);
			this.label2.TabIndex = 4;
			this.label2.Text = "Resolution preset:";
			// 
			// comboResolutionPreset
			// 
			this.comboResolutionPreset.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboResolutionPreset.FormattingEnabled = true;
			this.comboResolutionPreset.Items.AddRange(new object[] {
            "640x480",
            "800x600",
            "1024x768",
            "1152x864",
            "1280x1024"});
			this.comboResolutionPreset.Location = new System.Drawing.Point(104, 66);
			this.comboResolutionPreset.Name = "comboResolutionPreset";
			this.comboResolutionPreset.Size = new System.Drawing.Size(121, 21);
			this.comboResolutionPreset.TabIndex = 2;
			this.toolTip_ConfigEditor.SetToolTip(this.comboResolutionPreset, "Sets the screen/window resolution.\r\nNote: This is overriden by the custom resolut" +
        "ion setting.\r\nIt\'s recommended you use that instead.");
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(25, 21);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(73, 13);
			this.label1.TabIndex = 2;
			this.label1.Text = "Screen mode:";
			// 
			// radioWindowMode
			// 
			this.radioWindowMode.AutoSize = true;
			this.radioWindowMode.Location = new System.Drawing.Point(104, 43);
			this.radioWindowMode.Name = "radioWindowMode";
			this.radioWindowMode.Size = new System.Drawing.Size(93, 17);
			this.radioWindowMode.TabIndex = 1;
			this.radioWindowMode.Text = "Window mode";
			this.radioWindowMode.UseVisualStyleBackColor = true;
			// 
			// radioFullscreen
			// 
			this.radioFullscreen.AutoSize = true;
			this.radioFullscreen.Location = new System.Drawing.Point(104, 19);
			this.radioFullscreen.Name = "radioFullscreen";
			this.radioFullscreen.Size = new System.Drawing.Size(73, 17);
			this.radioFullscreen.TabIndex = 0;
			this.radioFullscreen.Text = "Fullscreen";
			this.radioFullscreen.UseVisualStyleBackColor = true;
			// 
			// tabPage_Sound
			// 
			this.tabPage_Sound.Controls.Add(this.groupBox_SoundVolume);
			this.tabPage_Sound.Controls.Add(this.groupBox_SoundConfig);
			this.tabPage_Sound.Location = new System.Drawing.Point(4, 22);
			this.tabPage_Sound.Name = "tabPage_Sound";
			this.tabPage_Sound.Padding = new System.Windows.Forms.Padding(3);
			this.tabPage_Sound.Size = new System.Drawing.Size(271, 266);
			this.tabPage_Sound.TabIndex = 1;
			this.tabPage_Sound.Text = "Sound";
			this.tabPage_Sound.UseVisualStyleBackColor = true;
			// 
			// groupBox_SoundVolume
			// 
			this.groupBox_SoundVolume.Controls.Add(this.numericBGMVol);
			this.groupBox_SoundVolume.Controls.Add(this.numericSoundVol);
			this.groupBox_SoundVolume.Controls.Add(this.label7);
			this.groupBox_SoundVolume.Controls.Add(this.label6);
			this.groupBox_SoundVolume.Location = new System.Drawing.Point(6, 84);
			this.groupBox_SoundVolume.Name = "groupBox_SoundVolume";
			this.groupBox_SoundVolume.Size = new System.Drawing.Size(259, 76);
			this.groupBox_SoundVolume.TabIndex = 1;
			this.groupBox_SoundVolume.TabStop = false;
			this.groupBox_SoundVolume.Text = "Volume";
			// 
			// numericBGMVol
			// 
			this.numericBGMVol.Location = new System.Drawing.Point(92, 43);
			this.numericBGMVol.Name = "numericBGMVol";
			this.numericBGMVol.Size = new System.Drawing.Size(41, 20);
			this.numericBGMVol.TabIndex = 4;
			this.toolTip_ConfigEditor.SetToolTip(this.numericBGMVol, "Controls the BGM volume");
			// 
			// numericSoundVol
			// 
			this.numericSoundVol.Location = new System.Drawing.Point(92, 17);
			this.numericSoundVol.Name = "numericSoundVol";
			this.numericSoundVol.Size = new System.Drawing.Size(41, 20);
			this.numericSoundVol.TabIndex = 3;
			this.toolTip_ConfigEditor.SetToolTip(this.numericSoundVol, "Controls the volume of sound effects and voices");
			// 
			// label7
			// 
			this.label7.AutoSize = true;
			this.label7.Location = new System.Drawing.Point(11, 45);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(75, 13);
			this.label7.TabIndex = 3;
			this.label7.Text = "Music volume:";
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(8, 19);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(78, 13);
			this.label6.TabIndex = 2;
			this.label6.Text = "Sound volume:";
			// 
			// groupBox_SoundConfig
			// 
			this.groupBox_SoundConfig.Controls.Add(this.checkMusic);
			this.groupBox_SoundConfig.Controls.Add(this.checkSound);
			this.groupBox_SoundConfig.Controls.Add(this.check3DSound);
			this.groupBox_SoundConfig.Location = new System.Drawing.Point(6, 6);
			this.groupBox_SoundConfig.Name = "groupBox_SoundConfig";
			this.groupBox_SoundConfig.Size = new System.Drawing.Size(259, 72);
			this.groupBox_SoundConfig.TabIndex = 0;
			this.groupBox_SoundConfig.TabStop = false;
			this.groupBox_SoundConfig.Text = "Sound";
			// 
			// checkMusic
			// 
			this.checkMusic.AutoSize = true;
			this.checkMusic.Location = new System.Drawing.Point(122, 44);
			this.checkMusic.Name = "checkMusic";
			this.checkMusic.Size = new System.Drawing.Size(90, 17);
			this.checkMusic.TabIndex = 2;
			this.checkMusic.Text = "Enable Music";
			this.checkMusic.UseVisualStyleBackColor = true;
			// 
			// checkSound
			// 
			this.checkSound.AutoSize = true;
			this.checkSound.Location = new System.Drawing.Point(7, 44);
			this.checkSound.Name = "checkSound";
			this.checkSound.Size = new System.Drawing.Size(98, 17);
			this.checkSound.TabIndex = 1;
			this.checkSound.Text = "Enable Sounds";
			this.checkSound.UseVisualStyleBackColor = true;
			// 
			// check3DSound
			// 
			this.check3DSound.AutoSize = true;
			this.check3DSound.Location = new System.Drawing.Point(7, 20);
			this.check3DSound.Name = "check3DSound";
			this.check3DSound.Size = new System.Drawing.Size(110, 17);
			this.check3DSound.TabIndex = 0;
			this.check3DSound.Text = "Enable 3D Sound";
			this.check3DSound.UseVisualStyleBackColor = true;
			// 
			// tabControls
			// 
			this.tabControls.Controls.Add(this.groupMouseButtons);
			this.tabControls.Controls.Add(this.groupMouseMode);
			this.tabControls.Location = new System.Drawing.Point(4, 22);
			this.tabControls.Name = "tabControls";
			this.tabControls.Padding = new System.Windows.Forms.Padding(3);
			this.tabControls.Size = new System.Drawing.Size(271, 266);
			this.tabControls.TabIndex = 2;
			this.tabControls.Text = "Mouse";
			this.tabControls.UseVisualStyleBackColor = true;
			// 
			// groupMouseMode
			// 
			this.groupMouseMode.Controls.Add(this.radioMouseModeRelease);
			this.groupMouseMode.Controls.Add(this.radioMouseModeHold);
			this.groupMouseMode.Location = new System.Drawing.Point(6, 6);
			this.groupMouseMode.Name = "groupMouseMode";
			this.groupMouseMode.Size = new System.Drawing.Size(259, 66);
			this.groupMouseMode.TabIndex = 0;
			this.groupMouseMode.TabStop = false;
			this.groupMouseMode.Text = "Mouse mode";
			// 
			// radioMouseModeHold
			// 
			this.radioMouseModeHold.AutoSize = true;
			this.radioMouseModeHold.Location = new System.Drawing.Point(6, 19);
			this.radioMouseModeHold.Name = "radioMouseModeHold";
			this.radioMouseModeHold.Size = new System.Drawing.Size(82, 17);
			this.radioMouseModeHold.TabIndex = 0;
			this.radioMouseModeHold.TabStop = true;
			this.radioMouseModeHold.Text = "Drag && Hold";
			this.toolTip_ConfigEditor.SetToolTip(this.radioMouseModeHold, "Drag and hold the mouse button to continue running. Release to stop.");
			this.radioMouseModeHold.UseVisualStyleBackColor = true;
			// 
			// radioMouseModeRelease
			// 
			this.radioMouseModeRelease.AutoSize = true;
			this.radioMouseModeRelease.Location = new System.Drawing.Point(6, 42);
			this.radioMouseModeRelease.Name = "radioMouseModeRelease";
			this.radioMouseModeRelease.Size = new System.Drawing.Size(113, 17);
			this.radioMouseModeRelease.TabIndex = 1;
			this.radioMouseModeRelease.TabStop = true;
			this.radioMouseModeRelease.Text = "Drag to accelerate";
			this.toolTip_ConfigEditor.SetToolTip(this.radioMouseModeRelease, "Click and drag to accelerate. Drag the opposite direction to decelerate.");
			this.radioMouseModeRelease.UseVisualStyleBackColor = true;
			// 
			// groupMouseButtons
			// 
			this.groupMouseButtons.Controls.Add(this.label9);
			this.groupMouseButtons.Controls.Add(this.comboMouseButtons);
			this.groupMouseButtons.Controls.Add(this.comboMouseActions);
			this.groupMouseButtons.Controls.Add(this.label8);
			this.groupMouseButtons.Enabled = false;
			this.groupMouseButtons.Location = new System.Drawing.Point(7, 79);
			this.groupMouseButtons.Name = "groupMouseButtons";
			this.groupMouseButtons.Size = new System.Drawing.Size(258, 65);
			this.groupMouseButtons.TabIndex = 1;
			this.groupMouseButtons.TabStop = false;
			this.groupMouseButtons.Text = "Mouse buttons";
			// 
			// label8
			// 
			this.label8.AutoSize = true;
			this.label8.Location = new System.Drawing.Point(7, 20);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(40, 13);
			this.label8.TabIndex = 0;
			this.label8.Text = "Action:";
			// 
			// comboMouseActions
			// 
			this.comboMouseActions.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboMouseActions.FormattingEnabled = true;
			this.comboMouseActions.Items.AddRange(new object[] {
            "Start",
            "Attack/Cancel",
            "Jump/Confirm",
            "Action",
            "Flute"});
			this.comboMouseActions.Location = new System.Drawing.Point(6, 37);
			this.comboMouseActions.Name = "comboMouseActions";
			this.comboMouseActions.Size = new System.Drawing.Size(92, 21);
			this.comboMouseActions.TabIndex = 1;
			this.comboMouseActions.SelectedIndexChanged += new System.EventHandler(this.comboMouseActions_SelectedIndexChanged);
			// 
			// comboMouseButtons
			// 
			this.comboMouseButtons.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboMouseButtons.FormattingEnabled = true;
			this.comboMouseButtons.Items.AddRange(new object[] {
            "Left Mouse Button",
            "Right Mouse Button",
            "Middle Mouse Button",
            "Other Mouse Button 1",
            "Left + Right Mouse Button",
            "Right + Left Mouse Button",
            "None"});
			this.comboMouseButtons.Location = new System.Drawing.Point(104, 37);
			this.comboMouseButtons.Name = "comboMouseButtons";
			this.comboMouseButtons.Size = new System.Drawing.Size(148, 21);
			this.comboMouseButtons.TabIndex = 2;
			this.comboMouseButtons.SelectedIndexChanged += new System.EventHandler(this.comboMouseButtons_SelectedIndexChanged);
			// 
			// label9
			// 
			this.label9.AutoSize = true;
			this.label9.Location = new System.Drawing.Point(104, 18);
			this.label9.Name = "label9";
			this.label9.Size = new System.Drawing.Size(41, 13);
			this.label9.TabIndex = 3;
			this.label9.Text = "Button:";
			// 
			// ConfigEditDialog
			// 
			this.AcceptButton = this.okButton;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(304, 346);
			this.Controls.Add(this.tabControl1);
			this.Controls.Add(this.cancelButton);
			this.Controls.Add(this.okButton);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "ConfigEditDialog";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Configuration Editor";
			this.Load += new System.EventHandler(this.ConfigEditDialog_Load);
			this.tabControl1.ResumeLayout(false);
			this.tabPage_Video.ResumeLayout(false);
			this.groupBox_Graphics.ResumeLayout(false);
			this.groupBox_Graphics.PerformLayout();
			this.groupBox_Screen.ResumeLayout(false);
			this.groupBox_Screen.PerformLayout();
			this.tabPage_Sound.ResumeLayout(false);
			this.groupBox_SoundVolume.ResumeLayout(false);
			this.groupBox_SoundVolume.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.numericBGMVol)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numericSoundVol)).EndInit();
			this.groupBox_SoundConfig.ResumeLayout(false);
			this.groupBox_SoundConfig.PerformLayout();
			this.tabControls.ResumeLayout(false);
			this.groupMouseMode.ResumeLayout(false);
			this.groupMouseMode.PerformLayout();
			this.groupMouseButtons.ResumeLayout(false);
			this.groupMouseButtons.PerformLayout();
			this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.Button cancelButton;
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage tabPage_Video;
		private System.Windows.Forms.GroupBox groupBox_Graphics;
		private System.Windows.Forms.ComboBox comboFog;
		private System.Windows.Forms.ComboBox comboClip;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.ComboBox comboFramerate;
		private System.Windows.Forms.GroupBox groupBox_Screen;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.ComboBox comboResolutionPreset;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.RadioButton radioWindowMode;
		private System.Windows.Forms.RadioButton radioFullscreen;
		private System.Windows.Forms.TabPage tabPage_Sound;
		private System.Windows.Forms.GroupBox groupBox_SoundVolume;
		private System.Windows.Forms.GroupBox groupBox_SoundConfig;
		private System.Windows.Forms.CheckBox checkMusic;
		private System.Windows.Forms.CheckBox checkSound;
		private System.Windows.Forms.CheckBox check3DSound;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.NumericUpDown numericBGMVol;
		private System.Windows.Forms.NumericUpDown numericSoundVol;
		private System.Windows.Forms.ToolTip toolTip_ConfigEditor;
		private System.Windows.Forms.TabPage tabControls;
		private System.Windows.Forms.GroupBox groupMouseMode;
		private System.Windows.Forms.RadioButton radioMouseModeRelease;
		private System.Windows.Forms.RadioButton radioMouseModeHold;
		private System.Windows.Forms.GroupBox groupMouseButtons;
		private System.Windows.Forms.ComboBox comboMouseActions;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.ComboBox comboMouseButtons;
		private System.Windows.Forms.Label label9;
    }
}

