namespace SADXModManager.Forms
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
			System.Windows.Forms.Label label10;
			System.Windows.Forms.Label label11;
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
			this.tabPage_Mouse = new System.Windows.Forms.TabPage();
			this.groupMouseButtons = new System.Windows.Forms.GroupBox();
			this.label9 = new System.Windows.Forms.Label();
			this.comboMouseButtons = new System.Windows.Forms.ComboBox();
			this.comboMouseActions = new System.Windows.Forms.ComboBox();
			this.label8 = new System.Windows.Forms.Label();
			this.groupMouseMode = new System.Windows.Forms.GroupBox();
			this.radioMouseModeRelease = new System.Windows.Forms.RadioButton();
			this.radioMouseModeHold = new System.Windows.Forms.RadioButton();
			this.tabPage_Controller = new System.Windows.Forms.TabPage();
			this.controllerConfigName = new System.Windows.Forms.TextBox();
			this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
			this.controllerConfigRemove = new System.Windows.Forms.Button();
			this.controllerConfigAdd = new System.Windows.Forms.Button();
			this.controllerConfigSelect = new System.Windows.Forms.ComboBox();
			this.toolTip_ConfigEditor = new System.Windows.Forms.ToolTip(this.components);
			label10 = new System.Windows.Forms.Label();
			label11 = new System.Windows.Forms.Label();
			this.tabControl1.SuspendLayout();
			this.tabPage_Video.SuspendLayout();
			this.groupBox_Graphics.SuspendLayout();
			this.groupBox_Screen.SuspendLayout();
			this.tabPage_Sound.SuspendLayout();
			this.groupBox_SoundVolume.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.numericBGMVol)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numericSoundVol)).BeginInit();
			this.groupBox_SoundConfig.SuspendLayout();
			this.tabPage_Mouse.SuspendLayout();
			this.groupMouseButtons.SuspendLayout();
			this.groupMouseMode.SuspendLayout();
			this.tabPage_Controller.SuspendLayout();
			this.SuspendLayout();
			// 
			// label10
			// 
			label10.AutoSize = true;
			label10.Location = new System.Drawing.Point(6, 9);
			label10.Name = "label10";
			label10.Size = new System.Drawing.Size(72, 13);
			label10.TabIndex = 1;
			label10.Text = "Configuration:";
			// 
			// label11
			// 
			label11.AutoSize = true;
			label11.Location = new System.Drawing.Point(40, 36);
			label11.Name = "label11";
			label11.Size = new System.Drawing.Size(38, 13);
			label11.TabIndex = 5;
			label11.Text = "Name:";
			// 
			// okButton
			// 
			this.okButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.okButton.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.okButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.okButton.Location = new System.Drawing.Point(183, 411);
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
			this.cancelButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.cancelButton.Location = new System.Drawing.Point(264, 411);
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
			this.tabControl1.Controls.Add(this.tabPage_Mouse);
			this.tabControl1.Controls.Add(this.tabPage_Controller);
			this.tabControl1.Location = new System.Drawing.Point(13, 13);
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(326, 392);
			this.tabControl1.TabIndex = 2;
			// 
			// tabPage_Video
			// 
			this.tabPage_Video.Controls.Add(this.groupBox_Graphics);
			this.tabPage_Video.Controls.Add(this.groupBox_Screen);
			this.tabPage_Video.Location = new System.Drawing.Point(4, 22);
			this.tabPage_Video.Name = "tabPage_Video";
			this.tabPage_Video.Padding = new System.Windows.Forms.Padding(3);
			this.tabPage_Video.Size = new System.Drawing.Size(318, 366);
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
			this.groupBox_Graphics.Location = new System.Drawing.Point(6, 86);
			this.groupBox_Graphics.Name = "groupBox_Graphics";
			this.groupBox_Graphics.Size = new System.Drawing.Size(306, 108);
			this.groupBox_Graphics.TabIndex = 1;
			this.groupBox_Graphics.TabStop = false;
			this.groupBox_Graphics.Text = "Graphics";
			// 
			// comboFog
			// 
			this.comboFog.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboFog.FlatStyle = System.Windows.Forms.FlatStyle.System;
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
			this.comboClip.FlatStyle = System.Windows.Forms.FlatStyle.System;
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
			this.label5.Location = new System.Drawing.Point(22, 76);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(76, 13);
			this.label5.TabIndex = 3;
			this.label5.Text = "Fog emulation:";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(46, 49);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(52, 13);
			this.label4.TabIndex = 2;
			this.label4.Text = "Clip level:";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(38, 22);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(60, 13);
			this.label3.TabIndex = 1;
			this.label3.Text = "Frame rate:";
			// 
			// comboFramerate
			// 
			this.comboFramerate.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboFramerate.FlatStyle = System.Windows.Forms.FlatStyle.System;
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
			this.groupBox_Screen.Controls.Add(this.label1);
			this.groupBox_Screen.Controls.Add(this.radioWindowMode);
			this.groupBox_Screen.Controls.Add(this.radioFullscreen);
			this.groupBox_Screen.Location = new System.Drawing.Point(6, 6);
			this.groupBox_Screen.Name = "groupBox_Screen";
			this.groupBox_Screen.Size = new System.Drawing.Size(306, 74);
			this.groupBox_Screen.TabIndex = 0;
			this.groupBox_Screen.TabStop = false;
			this.groupBox_Screen.Text = "Screen";
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
			this.radioWindowMode.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.radioWindowMode.Location = new System.Drawing.Point(104, 43);
			this.radioWindowMode.Name = "radioWindowMode";
			this.radioWindowMode.Size = new System.Drawing.Size(99, 18);
			this.radioWindowMode.TabIndex = 1;
			this.radioWindowMode.Text = "Window mode";
			this.radioWindowMode.UseVisualStyleBackColor = true;
			// 
			// radioFullscreen
			// 
			this.radioFullscreen.AutoSize = true;
			this.radioFullscreen.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.radioFullscreen.Location = new System.Drawing.Point(104, 19);
			this.radioFullscreen.Name = "radioFullscreen";
			this.radioFullscreen.Size = new System.Drawing.Size(79, 18);
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
			this.tabPage_Sound.Size = new System.Drawing.Size(318, 366);
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
			this.groupBox_SoundVolume.Location = new System.Drawing.Point(6, 77);
			this.groupBox_SoundVolume.Name = "groupBox_SoundVolume";
			this.groupBox_SoundVolume.Size = new System.Drawing.Size(306, 69);
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
			this.groupBox_SoundConfig.Size = new System.Drawing.Size(306, 65);
			this.groupBox_SoundConfig.TabIndex = 0;
			this.groupBox_SoundConfig.TabStop = false;
			this.groupBox_SoundConfig.Text = "Sound";
			// 
			// checkMusic
			// 
			this.checkMusic.AutoSize = true;
			this.checkMusic.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.checkMusic.Location = new System.Drawing.Point(122, 42);
			this.checkMusic.Name = "checkMusic";
			this.checkMusic.Size = new System.Drawing.Size(96, 18);
			this.checkMusic.TabIndex = 2;
			this.checkMusic.Text = "Enable Music";
			this.checkMusic.UseVisualStyleBackColor = true;
			// 
			// checkSound
			// 
			this.checkSound.AutoSize = true;
			this.checkSound.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.checkSound.Location = new System.Drawing.Point(6, 42);
			this.checkSound.Name = "checkSound";
			this.checkSound.Size = new System.Drawing.Size(104, 18);
			this.checkSound.TabIndex = 1;
			this.checkSound.Text = "Enable Sounds";
			this.checkSound.UseVisualStyleBackColor = true;
			// 
			// check3DSound
			// 
			this.check3DSound.AutoSize = true;
			this.check3DSound.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.check3DSound.Location = new System.Drawing.Point(6, 19);
			this.check3DSound.Name = "check3DSound";
			this.check3DSound.Size = new System.Drawing.Size(116, 18);
			this.check3DSound.TabIndex = 0;
			this.check3DSound.Text = "Enable 3D Sound";
			this.check3DSound.UseVisualStyleBackColor = true;
			// 
			// tabPage_Mouse
			// 
			this.tabPage_Mouse.Controls.Add(this.groupMouseButtons);
			this.tabPage_Mouse.Controls.Add(this.groupMouseMode);
			this.tabPage_Mouse.Location = new System.Drawing.Point(4, 22);
			this.tabPage_Mouse.Name = "tabPage_Mouse";
			this.tabPage_Mouse.Padding = new System.Windows.Forms.Padding(3);
			this.tabPage_Mouse.Size = new System.Drawing.Size(318, 366);
			this.tabPage_Mouse.TabIndex = 2;
			this.tabPage_Mouse.Text = "Mouse";
			this.tabPage_Mouse.UseVisualStyleBackColor = true;
			// 
			// groupMouseButtons
			// 
			this.groupMouseButtons.Controls.Add(this.label9);
			this.groupMouseButtons.Controls.Add(this.comboMouseButtons);
			this.groupMouseButtons.Controls.Add(this.comboMouseActions);
			this.groupMouseButtons.Controls.Add(this.label8);
			this.groupMouseButtons.Location = new System.Drawing.Point(6, 77);
			this.groupMouseButtons.Name = "groupMouseButtons";
			this.groupMouseButtons.Size = new System.Drawing.Size(306, 64);
			this.groupMouseButtons.TabIndex = 1;
			this.groupMouseButtons.TabStop = false;
			this.groupMouseButtons.Text = "Mouse buttons";
			// 
			// label9
			// 
			this.label9.AutoSize = true;
			this.label9.Location = new System.Drawing.Point(104, 18);
			this.label9.Name = "label9";
			this.label9.Size = new System.Drawing.Size(98, 13);
			this.label9.TabIndex = 3;
			this.label9.Text = "Button Assignment:";
			// 
			// comboMouseButtons
			// 
			this.comboMouseButtons.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboMouseButtons.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.comboMouseButtons.FormattingEnabled = true;
			this.comboMouseButtons.Items.AddRange(new object[] {
            "[None]",
            "Left Mouse Button",
            "Right Mouse Button",
            "Middle Mouse Button",
            "Other Mouse Button 1",
            "Left + Right Mouse Button",
            "Right + Left Mouse Button"});
			this.comboMouseButtons.Location = new System.Drawing.Point(107, 37);
			this.comboMouseButtons.Name = "comboMouseButtons";
			this.comboMouseButtons.Size = new System.Drawing.Size(148, 21);
			this.comboMouseButtons.TabIndex = 2;
			this.comboMouseButtons.SelectedIndexChanged += new System.EventHandler(this.comboMouseButtons_SelectedIndexChanged);
			// 
			// comboMouseActions
			// 
			this.comboMouseActions.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.comboMouseActions.FlatStyle = System.Windows.Forms.FlatStyle.System;
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
			// label8
			// 
			this.label8.AutoSize = true;
			this.label8.Location = new System.Drawing.Point(7, 20);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(40, 13);
			this.label8.TabIndex = 0;
			this.label8.Text = "Action:";
			// 
			// groupMouseMode
			// 
			this.groupMouseMode.Controls.Add(this.radioMouseModeRelease);
			this.groupMouseMode.Controls.Add(this.radioMouseModeHold);
			this.groupMouseMode.Location = new System.Drawing.Point(6, 6);
			this.groupMouseMode.Name = "groupMouseMode";
			this.groupMouseMode.Size = new System.Drawing.Size(306, 65);
			this.groupMouseMode.TabIndex = 0;
			this.groupMouseMode.TabStop = false;
			this.groupMouseMode.Text = "Mouse mode";
			// 
			// radioMouseModeRelease
			// 
			this.radioMouseModeRelease.AutoSize = true;
			this.radioMouseModeRelease.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.radioMouseModeRelease.Location = new System.Drawing.Point(6, 42);
			this.radioMouseModeRelease.Name = "radioMouseModeRelease";
			this.radioMouseModeRelease.Size = new System.Drawing.Size(119, 18);
			this.radioMouseModeRelease.TabIndex = 1;
			this.radioMouseModeRelease.TabStop = true;
			this.radioMouseModeRelease.Text = "Drag to accelerate";
			this.toolTip_ConfigEditor.SetToolTip(this.radioMouseModeRelease, "Click and drag to accelerate. Drag the opposite direction to decelerate.");
			this.radioMouseModeRelease.UseVisualStyleBackColor = true;
			// 
			// radioMouseModeHold
			// 
			this.radioMouseModeHold.AutoSize = true;
			this.radioMouseModeHold.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.radioMouseModeHold.Location = new System.Drawing.Point(6, 19);
			this.radioMouseModeHold.Name = "radioMouseModeHold";
			this.radioMouseModeHold.Size = new System.Drawing.Size(88, 18);
			this.radioMouseModeHold.TabIndex = 0;
			this.radioMouseModeHold.TabStop = true;
			this.radioMouseModeHold.Text = "Drag && Hold";
			this.toolTip_ConfigEditor.SetToolTip(this.radioMouseModeHold, "Drag and hold the mouse button to continue running. Release to stop.");
			this.radioMouseModeHold.UseVisualStyleBackColor = true;
			// 
			// tabPage_Controller
			// 
			this.tabPage_Controller.Controls.Add(this.controllerConfigName);
			this.tabPage_Controller.Controls.Add(label11);
			this.tabPage_Controller.Controls.Add(this.tableLayoutPanel1);
			this.tabPage_Controller.Controls.Add(this.controllerConfigRemove);
			this.tabPage_Controller.Controls.Add(this.controllerConfigAdd);
			this.tabPage_Controller.Controls.Add(label10);
			this.tabPage_Controller.Controls.Add(this.controllerConfigSelect);
			this.tabPage_Controller.Location = new System.Drawing.Point(4, 22);
			this.tabPage_Controller.Name = "tabPage_Controller";
			this.tabPage_Controller.Padding = new System.Windows.Forms.Padding(3);
			this.tabPage_Controller.Size = new System.Drawing.Size(318, 366);
			this.tabPage_Controller.TabIndex = 3;
			this.tabPage_Controller.Text = "Controller";
			this.tabPage_Controller.UseVisualStyleBackColor = true;
			// 
			// controllerConfigName
			// 
			this.controllerConfigName.Enabled = false;
			this.controllerConfigName.Location = new System.Drawing.Point(84, 33);
			this.controllerConfigName.Name = "controllerConfigName";
			this.controllerConfigName.Size = new System.Drawing.Size(121, 20);
			this.controllerConfigName.TabIndex = 6;
			this.controllerConfigName.TextChanged += new System.EventHandler(this.controllerConfigName_TextChanged);
			// 
			// tableLayoutPanel1
			// 
			this.tableLayoutPanel1.AutoSize = true;
			this.tableLayoutPanel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.tableLayoutPanel1.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Single;
			this.tableLayoutPanel1.ColumnCount = 2;
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
			this.tableLayoutPanel1.Location = new System.Drawing.Point(6, 59);
			this.tableLayoutPanel1.Name = "tableLayoutPanel1";
			this.tableLayoutPanel1.RowCount = 1;
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
			this.tableLayoutPanel1.Size = new System.Drawing.Size(3, 2);
			this.tableLayoutPanel1.TabIndex = 4;
			// 
			// controllerConfigRemove
			// 
			this.controllerConfigRemove.AutoSize = true;
			this.controllerConfigRemove.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.controllerConfigRemove.Enabled = false;
			this.controllerConfigRemove.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.controllerConfigRemove.Location = new System.Drawing.Point(253, 4);
			this.controllerConfigRemove.Name = "controllerConfigRemove";
			this.controllerConfigRemove.Size = new System.Drawing.Size(61, 22);
			this.controllerConfigRemove.TabIndex = 3;
			this.controllerConfigRemove.Text = "Remove";
			this.controllerConfigRemove.UseVisualStyleBackColor = true;
			this.controllerConfigRemove.Click += new System.EventHandler(this.controllerConfigRemove_Click);
			// 
			// controllerConfigAdd
			// 
			this.controllerConfigAdd.AutoSize = true;
			this.controllerConfigAdd.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.controllerConfigAdd.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.controllerConfigAdd.Location = new System.Drawing.Point(211, 4);
			this.controllerConfigAdd.Name = "controllerConfigAdd";
			this.controllerConfigAdd.Size = new System.Drawing.Size(40, 22);
			this.controllerConfigAdd.TabIndex = 2;
			this.controllerConfigAdd.Text = "Add";
			this.controllerConfigAdd.UseVisualStyleBackColor = true;
			this.controllerConfigAdd.Click += new System.EventHandler(this.controllerConfigAdd_Click);
			// 
			// controllerConfigSelect
			// 
			this.controllerConfigSelect.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.controllerConfigSelect.FlatStyle = System.Windows.Forms.FlatStyle.System;
			this.controllerConfigSelect.FormattingEnabled = true;
			this.controllerConfigSelect.Location = new System.Drawing.Point(84, 6);
			this.controllerConfigSelect.Name = "controllerConfigSelect";
			this.controllerConfigSelect.Size = new System.Drawing.Size(121, 21);
			this.controllerConfigSelect.TabIndex = 0;
			this.controllerConfigSelect.SelectedIndexChanged += new System.EventHandler(this.controllerConfigSelect_SelectedIndexChanged);
			// 
			// ConfigEditDialog
			// 
			this.AcceptButton = this.okButton;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(351, 446);
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
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ConfigEditDialog_FormClosing);
			this.Load += new System.EventHandler(this.ConfigEditDialog_Load);
			this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.ConfigEditDialog_KeyDown);
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
			this.tabPage_Mouse.ResumeLayout(false);
			this.groupMouseButtons.ResumeLayout(false);
			this.groupMouseButtons.PerformLayout();
			this.groupMouseMode.ResumeLayout(false);
			this.groupMouseMode.PerformLayout();
			this.tabPage_Controller.ResumeLayout(false);
			this.tabPage_Controller.PerformLayout();
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
		private System.Windows.Forms.TabPage tabPage_Mouse;
		private System.Windows.Forms.GroupBox groupMouseMode;
		private System.Windows.Forms.RadioButton radioMouseModeRelease;
		private System.Windows.Forms.RadioButton radioMouseModeHold;
		private System.Windows.Forms.GroupBox groupMouseButtons;
		private System.Windows.Forms.ComboBox comboMouseActions;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.ComboBox comboMouseButtons;
		private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TabPage tabPage_Controller;
        private System.Windows.Forms.ComboBox controllerConfigSelect;
        private System.Windows.Forms.Button controllerConfigAdd;
        private System.Windows.Forms.Button controllerConfigRemove;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TextBox controllerConfigName;
    }
}

