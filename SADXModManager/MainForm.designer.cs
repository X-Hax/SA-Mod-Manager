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
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.Label label3;
            System.Windows.Forms.Label label1;
            System.Windows.Forms.Label label2;
            System.Windows.Forms.GroupBox groupBox2;
            System.Windows.Forms.Label label5;
            System.Windows.Forms.Label label15;
            System.Windows.Forms.Label label16;
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.comboTextLanguage = new System.Windows.Forms.ComboBox();
            this.comboVoiceLanguage = new System.Windows.Forms.ComboBox();
            this.label9 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.pauseWhenInactiveCheckBox = new System.Windows.Forms.CheckBox();
            this.disableCDCheckCheckBox = new System.Windows.Forms.CheckBox();
            this.screenNumComboBox = new System.Windows.Forms.ComboBox();
            this.stretchFullscreenCheckBox = new System.Windows.Forms.CheckBox();
            this.forceAspectRatioCheckBox = new System.Windows.Forms.CheckBox();
            this.windowedFullscreenCheckBox = new System.Windows.Forms.CheckBox();
            this.verticalResolution = new System.Windows.Forms.NumericUpDown();
            this.horizontalResolution = new System.Windows.Forms.NumericUpDown();
            this.buttonRefreshModList = new System.Windows.Forms.Button();
            this.modDescription = new System.Windows.Forms.Label();
            this.modListView = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.saveButton = new System.Windows.Forms.Button();
            this.saveAndPlayButton = new System.Windows.Forms.Button();
            this.installButton = new System.Windows.Forms.Button();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPageMods = new System.Windows.Forms.TabPage();
            this.configureModButton = new System.Windows.Forms.Button();
            this.modBottomButton = new System.Windows.Forms.Button();
            this.modTopButton = new System.Windows.Forms.Button();
            this.buttonNewMod = new System.Windows.Forms.Button();
            this.modDownButton = new System.Windows.Forms.Button();
            this.modUpButton = new System.Windows.Forms.Button();
            this.tabPageCodes = new System.Windows.Forms.TabPage();
            this.codesCheckedListBox = new System.Windows.Forms.CheckedListBox();
            this.tabPageGraphics = new System.Windows.Forms.TabPage();
            this.groupBox_WindowMode = new System.Windows.Forms.GroupBox();
            this.checkWindowResize = new System.Windows.Forms.CheckBox();
            this.customWindowSizeCheckBox = new System.Windows.Forms.CheckBox();
            this.maintainWindowAspectRatioCheckBox = new System.Windows.Forms.CheckBox();
            this.windowHeight = new System.Windows.Forms.NumericUpDown();
            this.windowWidth = new System.Windows.Forms.NumericUpDown();
            this.groupBox_Visuals = new System.Windows.Forms.GroupBox();
            this.buttonUpdateD3D8to9 = new System.Windows.Forms.Button();
            this.checkPolyBuff = new System.Windows.Forms.CheckBox();
            this.checkBoxEnableD3D9 = new System.Windows.Forms.CheckBox();
            this.label7 = new System.Windows.Forms.Label();
            this.checkScaleHud = new System.Windows.Forms.CheckBox();
            this.label6 = new System.Windows.Forms.Label();
            this.comboFmvFill = new System.Windows.Forms.ComboBox();
            this.comboFog = new System.Windows.Forms.ComboBox();
            this.comboBackgroundFill = new System.Windows.Forms.ComboBox();
            this.comboClip = new System.Windows.Forms.ComboBox();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.comboFramerate = new System.Windows.Forms.ComboBox();
            this.forceMipmappingCheckBox = new System.Windows.Forms.CheckBox();
            this.forceTextureFilterCheckBox = new System.Windows.Forms.CheckBox();
            this.groupBox_Display = new System.Windows.Forms.GroupBox();
            this.comboResolutionPreset = new System.Windows.Forms.ComboBox();
            this.radioWindowMode = new System.Windows.Forms.RadioButton();
            this.checkVsync = new System.Windows.Forms.CheckBox();
            this.radioFullscreen = new System.Windows.Forms.RadioButton();
            this.tabPageInput = new System.Windows.Forms.TabPage();
            this.groupMouseMode = new System.Windows.Forms.GroupBox();
            this.label17 = new System.Windows.Forms.Label();
            this.comboMouseButtons = new System.Windows.Forms.ComboBox();
            this.radioMouseModeRelease = new System.Windows.Forms.RadioButton();
            this.radioMouseModeHold = new System.Windows.Forms.RadioButton();
            this.comboMouseActions = new System.Windows.Forms.ComboBox();
            this.label18 = new System.Windows.Forms.Label();
            this.groupBoxController = new System.Windows.Forms.GroupBox();
            this.controllerConfigName = new System.Windows.Forms.TextBox();
            this.controllerConfigSelect = new System.Windows.Forms.ComboBox();
            this.controllerConfigAdd = new System.Windows.Forms.Button();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.controllerConfigRemove = new System.Windows.Forms.Button();
            this.tabPageSound = new System.Windows.Forms.TabPage();
            this.groupBox_SoundAdvanced = new System.Windows.Forms.GroupBox();
            this.checkBASS = new System.Windows.Forms.CheckBox();
            this.groupBox_SoundVolume = new System.Windows.Forms.GroupBox();
            this.labelVoiceVol = new System.Windows.Forms.Label();
            this.labelMusicVol = new System.Windows.Forms.Label();
            this.trackBarVoiceVol = new System.Windows.Forms.TrackBar();
            this.label13 = new System.Windows.Forms.Label();
            this.trackBarMusicVol = new System.Windows.Forms.TrackBar();
            this.label14 = new System.Windows.Forms.Label();
            this.groupBox_SoundConfig = new System.Windows.Forms.GroupBox();
            this.checkMusic = new System.Windows.Forms.CheckBox();
            this.checkSound = new System.Windows.Forms.CheckBox();
            this.check3DSound = new System.Windows.Forms.CheckBox();
            this.tabPageOptions = new System.Windows.Forms.TabPage();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.buttonSaveProfile = new System.Windows.Forms.Button();
            this.buttonLoadProfile = new System.Windows.Forms.Button();
            this.profileNameBox = new System.Windows.Forms.ComboBox();
            this.installURLHandlerButton = new System.Windows.Forms.Button();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.buttonCheckForUpdates = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.checkUpdateModsStartup = new System.Windows.Forms.CheckBox();
            this.numericUpdateFrequency = new System.Windows.Forms.NumericUpDown();
            this.comboUpdateFrequency = new System.Windows.Forms.ComboBox();
            this.checkUpdateStartup = new System.Windows.Forms.CheckBox();
            this.tabPageDebug = new System.Windows.Forms.TabPage();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.crashLogCheckBox = new System.Windows.Forms.CheckBox();
            this.consoleCheckBox = new System.Windows.Forms.CheckBox();
            this.screenCheckBox = new System.Windows.Forms.CheckBox();
            this.fileCheckBox = new System.Windows.Forms.CheckBox();
            this.groupBox12 = new System.Windows.Forms.GroupBox();
            this.labelTestSpawnWarning = new System.Windows.Forms.Label();
            this.labelTestSpawnTime = new System.Windows.Forms.Label();
            this.comboBoxTestSpawnTime = new System.Windows.Forms.ComboBox();
            this.checkBoxTestSpawnAngleHex = new System.Windows.Forms.CheckBox();
            this.checkBoxTestSpawnPosition = new System.Windows.Forms.CheckBox();
            this.buttonTestSpawnPlay = new System.Windows.Forms.Button();
            this.comboBoxTestSpawnEvent = new System.Windows.Forms.ComboBox();
            this.checkBoxTestSpawnEvent = new System.Windows.Forms.CheckBox();
            this.checkBoxTestSpawnCharacter = new System.Windows.Forms.CheckBox();
            this.checkBoxTestSpawnLevel = new System.Windows.Forms.CheckBox();
            this.comboBoxTestSpawnCharacter = new System.Windows.Forms.ComboBox();
            this.numericUpDownTestSpawnAngle = new System.Windows.Forms.NumericUpDown();
            this.labelTestSpawnAngle = new System.Windows.Forms.Label();
            this.labelTestSpawnY = new System.Windows.Forms.Label();
            this.numericUpDownTestSpawnSaveID = new System.Windows.Forms.NumericUpDown();
            this.checkBoxTestSpawnSave = new System.Windows.Forms.CheckBox();
            this.labelTestSpawnX = new System.Windows.Forms.Label();
            this.labelTestSpawnAct = new System.Windows.Forms.Label();
            this.labelTestSpawnZ = new System.Windows.Forms.Label();
            this.numericUpDownTestSpawnAct = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownTestSpawnZ = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownTestSpawnY = new System.Windows.Forms.NumericUpDown();
            this.comboBoxTestSpawnLevel = new System.Windows.Forms.ComboBox();
            this.numericUpDownTestSpawnX = new System.Windows.Forms.NumericUpDown();
            this.toolTip = new System.Windows.Forms.ToolTip(this.components);
            this.modContextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.openFolderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.checkForUpdatesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.verifyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.forceUpdateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.uninstallToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
            this.developerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.generateManifestToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            label3 = new System.Windows.Forms.Label();
            label1 = new System.Windows.Forms.Label();
            label2 = new System.Windows.Forms.Label();
            groupBox2 = new System.Windows.Forms.GroupBox();
            label5 = new System.Windows.Forms.Label();
            label15 = new System.Windows.Forms.Label();
            label16 = new System.Windows.Forms.Label();
            groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.verticalResolution)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.horizontalResolution)).BeginInit();
            this.tabControl1.SuspendLayout();
            this.tabPageMods.SuspendLayout();
            this.tabPageCodes.SuspendLayout();
            this.tabPageGraphics.SuspendLayout();
            this.groupBox_WindowMode.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.windowHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.windowWidth)).BeginInit();
            this.groupBox_Visuals.SuspendLayout();
            this.groupBox_Display.SuspendLayout();
            this.tabPageInput.SuspendLayout();
            this.groupMouseMode.SuspendLayout();
            this.groupBoxController.SuspendLayout();
            this.tabPageSound.SuspendLayout();
            this.groupBox_SoundAdvanced.SuspendLayout();
            this.groupBox_SoundVolume.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarVoiceVol)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarMusicVol)).BeginInit();
            this.groupBox_SoundConfig.SuspendLayout();
            this.tabPageOptions.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpdateFrequency)).BeginInit();
            this.tabPageDebug.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox12.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTestSpawnAngle)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTestSpawnSaveID)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTestSpawnAct)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTestSpawnZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTestSpawnY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTestSpawnX)).BeginInit();
            this.modContextMenu.SuspendLayout();
            this.SuspendLayout();
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new System.Drawing.Point(6, 22);
            label3.Name = "label3";
            label3.Size = new System.Drawing.Size(44, 13);
            label3.TabIndex = 0;
            label3.Text = "Screen:";
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new System.Drawing.Point(140, 50);
            label1.Name = "label1";
            label1.Size = new System.Drawing.Size(12, 13);
            label1.TabIndex = 7;
            label1.Text = "x";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new System.Drawing.Point(219, 22);
            label2.Name = "label2";
            label2.Size = new System.Drawing.Size(12, 13);
            label2.TabIndex = 13;
            label2.Text = "x";
            // 
            // groupBox2
            // 
            groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            groupBox2.Controls.Add(this.comboTextLanguage);
            groupBox2.Controls.Add(this.comboVoiceLanguage);
            groupBox2.Controls.Add(this.label9);
            groupBox2.Controls.Add(this.label8);
            groupBox2.Controls.Add(this.pauseWhenInactiveCheckBox);
            groupBox2.Controls.Add(this.disableCDCheckCheckBox);
            groupBox2.Location = new System.Drawing.Point(6, 6);
            groupBox2.Name = "groupBox2";
            groupBox2.Size = new System.Drawing.Size(390, 88);
            groupBox2.TabIndex = 0;
            groupBox2.TabStop = false;
            groupBox2.Text = "Misc.";
            // 
            // comboTextLanguage
            // 
            this.comboTextLanguage.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboTextLanguage.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.comboTextLanguage.FormattingEnabled = true;
            this.comboTextLanguage.Items.AddRange(new object[] {
            "Japanese",
            "English",
            "French",
            "Spanish",
            "German"});
            this.comboTextLanguage.Location = new System.Drawing.Point(125, 57);
            this.comboTextLanguage.Name = "comboTextLanguage";
            this.comboTextLanguage.Size = new System.Drawing.Size(97, 21);
            this.comboTextLanguage.TabIndex = 7;
            this.toolTip.SetToolTip(this.comboTextLanguage, "Text language at startup. Loading a save file overrides this.");
            // 
            // comboVoiceLanguage
            // 
            this.comboVoiceLanguage.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboVoiceLanguage.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.comboVoiceLanguage.FormattingEnabled = true;
            this.comboVoiceLanguage.Items.AddRange(new object[] {
            "Japanese",
            "English"});
            this.comboVoiceLanguage.Location = new System.Drawing.Point(11, 57);
            this.comboVoiceLanguage.Name = "comboVoiceLanguage";
            this.comboVoiceLanguage.Size = new System.Drawing.Size(98, 21);
            this.comboVoiceLanguage.TabIndex = 6;
            this.toolTip.SetToolTip(this.comboVoiceLanguage, "Voice language at startup. Loading a save file overrides this.");
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(123, 40);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(78, 13);
            this.label9.TabIndex = 4;
            this.label9.Text = "Text language:";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(9, 40);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(84, 13);
            this.label8.TabIndex = 3;
            this.label8.Text = "Voice language:";
            // 
            // pauseWhenInactiveCheckBox
            // 
            this.pauseWhenInactiveCheckBox.AutoSize = true;
            this.pauseWhenInactiveCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.pauseWhenInactiveCheckBox.Location = new System.Drawing.Point(125, 19);
            this.pauseWhenInactiveCheckBox.Name = "pauseWhenInactiveCheckBox";
            this.pauseWhenInactiveCheckBox.Size = new System.Drawing.Size(135, 18);
            this.pauseWhenInactiveCheckBox.TabIndex = 1;
            this.pauseWhenInactiveCheckBox.Text = "Pause When Inactive";
            this.toolTip.SetToolTip(this.pauseWhenInactiveCheckBox, "Uncheck to allow the game to run in the background.");
            this.pauseWhenInactiveCheckBox.UseVisualStyleBackColor = true;
            // 
            // disableCDCheckCheckBox
            // 
            this.disableCDCheckCheckBox.AutoSize = true;
            this.disableCDCheckCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.disableCDCheckCheckBox.Location = new System.Drawing.Point(6, 19);
            this.disableCDCheckCheckBox.Name = "disableCDCheckCheckBox";
            this.disableCDCheckCheckBox.Size = new System.Drawing.Size(119, 18);
            this.disableCDCheckCheckBox.TabIndex = 0;
            this.disableCDCheckCheckBox.Text = "Disable CD Check";
            this.toolTip.SetToolTip(this.disableCDCheckCheckBox, "Allows you to run the game without the disc.");
            this.disableCDCheckCheckBox.UseVisualStyleBackColor = true;
            // 
            // label5
            // 
            label5.AutoSize = true;
            label5.Location = new System.Drawing.Point(6, 49);
            label5.Name = "label5";
            label5.Size = new System.Drawing.Size(60, 13);
            label5.TabIndex = 17;
            label5.Text = "Resolution:";
            // 
            // label15
            // 
            label15.AutoSize = true;
            label15.Location = new System.Drawing.Point(270, 60);
            label15.Name = "label15";
            label15.Size = new System.Drawing.Size(38, 13);
            label15.TabIndex = 12;
            label15.Text = "Name:";
            // 
            // label16
            // 
            label16.AutoSize = true;
            label16.Location = new System.Drawing.Point(269, 15);
            label16.Name = "label16";
            label16.Size = new System.Drawing.Size(39, 13);
            label16.TabIndex = 8;
            label16.Text = "Profile:";
            // 
            // screenNumComboBox
            // 
            this.screenNumComboBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.screenNumComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.screenNumComboBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.screenNumComboBox.FormattingEnabled = true;
            this.screenNumComboBox.Items.AddRange(new object[] {
            "All Screens"});
            this.screenNumComboBox.Location = new System.Drawing.Point(56, 19);
            this.screenNumComboBox.Name = "screenNumComboBox";
            this.screenNumComboBox.Size = new System.Drawing.Size(315, 21);
            this.screenNumComboBox.TabIndex = 1;
            this.toolTip.SetToolTip(this.screenNumComboBox, "The screen to put the game on.");
            this.screenNumComboBox.SelectedIndexChanged += new System.EventHandler(this.screenNumComboBox_SelectedIndexChanged);
            // 
            // stretchFullscreenCheckBox
            // 
            this.stretchFullscreenCheckBox.AutoSize = true;
            this.stretchFullscreenCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.stretchFullscreenCheckBox.Location = new System.Drawing.Point(104, 97);
            this.stretchFullscreenCheckBox.Name = "stretchFullscreenCheckBox";
            this.stretchFullscreenCheckBox.Size = new System.Drawing.Size(108, 18);
            this.stretchFullscreenCheckBox.TabIndex = 3;
            this.stretchFullscreenCheckBox.Text = "Scale to Screen";
            this.toolTip.SetToolTip(this.stretchFullscreenCheckBox, "Coupled with Borderless Fullscreen, you can downsample the game from resolutions " +
        "higher than your screen supports.");
            this.stretchFullscreenCheckBox.UseVisualStyleBackColor = true;
            // 
            // forceAspectRatioCheckBox
            // 
            this.forceAspectRatioCheckBox.AutoSize = true;
            this.forceAspectRatioCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.forceAspectRatioCheckBox.Location = new System.Drawing.Point(226, 73);
            this.forceAspectRatioCheckBox.Name = "forceAspectRatioCheckBox";
            this.forceAspectRatioCheckBox.Size = new System.Drawing.Size(141, 18);
            this.forceAspectRatioCheckBox.TabIndex = 10;
            this.forceAspectRatioCheckBox.Text = "Force 4:3 Aspect Ratio";
            this.forceAspectRatioCheckBox.UseVisualStyleBackColor = true;
            this.forceAspectRatioCheckBox.CheckedChanged += new System.EventHandler(this.forceAspectRatioCheckBox_CheckedChanged);
            // 
            // windowedFullscreenCheckBox
            // 
            this.windowedFullscreenCheckBox.AutoSize = true;
            this.windowedFullscreenCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.windowedFullscreenCheckBox.Location = new System.Drawing.Point(9, 97);
            this.windowedFullscreenCheckBox.Name = "windowedFullscreenCheckBox";
            this.windowedFullscreenCheckBox.Size = new System.Drawing.Size(81, 18);
            this.windowedFullscreenCheckBox.TabIndex = 2;
            this.windowedFullscreenCheckBox.Text = "Borderless";
            this.toolTip.SetToolTip(this.windowedFullscreenCheckBox, "Enables borderless fullscreen mode.\r\nYou have to enable fullscreen mode for it to" +
        " work.");
            this.windowedFullscreenCheckBox.UseVisualStyleBackColor = true;
            // 
            // verticalResolution
            // 
            this.verticalResolution.Location = new System.Drawing.Point(157, 47);
            this.verticalResolution.Maximum = new decimal(new int[] {
            2147483647,
            0,
            0,
            0});
            this.verticalResolution.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.verticalResolution.Name = "verticalResolution";
            this.verticalResolution.Size = new System.Drawing.Size(62, 20);
            this.verticalResolution.TabIndex = 8;
            this.verticalResolution.Value = new decimal(new int[] {
            480,
            0,
            0,
            0});
            this.verticalResolution.ValueChanged += new System.EventHandler(this.verticalResolution_ValueChanged);
            // 
            // horizontalResolution
            // 
            this.horizontalResolution.Location = new System.Drawing.Point(72, 47);
            this.horizontalResolution.Maximum = new decimal(new int[] {
            2147483647,
            0,
            0,
            0});
            this.horizontalResolution.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.horizontalResolution.Name = "horizontalResolution";
            this.horizontalResolution.Size = new System.Drawing.Size(62, 20);
            this.horizontalResolution.TabIndex = 6;
            this.horizontalResolution.Value = new decimal(new int[] {
            640,
            0,
            0,
            0});
            this.horizontalResolution.ValueChanged += new System.EventHandler(this.horizontalResolution_ValueChanged);
            // 
            // buttonRefreshModList
            // 
            this.buttonRefreshModList.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.buttonRefreshModList.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.buttonRefreshModList.Location = new System.Drawing.Point(6, 288);
            this.buttonRefreshModList.Name = "buttonRefreshModList";
            this.buttonRefreshModList.Size = new System.Drawing.Size(75, 23);
            this.buttonRefreshModList.TabIndex = 3;
            this.buttonRefreshModList.Text = "&Refresh";
            this.buttonRefreshModList.UseVisualStyleBackColor = true;
            this.buttonRefreshModList.Click += new System.EventHandler(this.buttonRefreshModList_Click);
            // 
            // modDescription
            // 
            this.modDescription.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.modDescription.Location = new System.Drawing.Point(6, 317);
            this.modDescription.Name = "modDescription";
            this.modDescription.Size = new System.Drawing.Size(390, 60);
            this.modDescription.TabIndex = 6;
            this.modDescription.Text = "Description: No mod selected.";
            // 
            // modListView
            // 
            this.modListView.AllowDrop = true;
            this.modListView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.modListView.CheckBoxes = true;
            this.modListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3});
            this.modListView.FullRowSelect = true;
            this.modListView.HideSelection = false;
            this.modListView.Location = new System.Drawing.Point(6, 6);
            this.modListView.Name = "modListView";
            this.modListView.Size = new System.Drawing.Size(361, 276);
            this.modListView.TabIndex = 0;
            this.modListView.UseCompatibleStateImageBehavior = false;
            this.modListView.View = System.Windows.Forms.View.Details;
            this.modListView.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.modListView_ItemCheck);
            this.modListView.ItemDrag += new System.Windows.Forms.ItemDragEventHandler(this.modListView_ItemDrag);
            this.modListView.SelectedIndexChanged += new System.EventHandler(this.modListView_SelectedIndexChanged);
            this.modListView.DragDrop += new System.Windows.Forms.DragEventHandler(this.modListView_DragDrop);
            this.modListView.DragEnter += new System.Windows.Forms.DragEventHandler(this.modListView_DragEnter);
            this.modListView.DragOver += new System.Windows.Forms.DragEventHandler(this.modListView_DragOver);
            this.modListView.MouseClick += new System.Windows.Forms.MouseEventHandler(this.modListView_MouseClick);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Name";
            this.columnHeader1.Width = 180;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Author";
            this.columnHeader2.Width = 100;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Version";
            this.columnHeader3.Width = 75;
            // 
            // saveButton
            // 
            this.saveButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.saveButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.saveButton.Location = new System.Drawing.Point(88, 412);
            this.saveButton.Name = "saveButton";
            this.saveButton.Size = new System.Drawing.Size(75, 22);
            this.saveButton.TabIndex = 102;
            this.saveButton.Text = "&Save";
            this.saveButton.UseVisualStyleBackColor = true;
            this.saveButton.Click += new System.EventHandler(this.saveButton_Click);
            // 
            // saveAndPlayButton
            // 
            this.saveAndPlayButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.saveAndPlayButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.saveAndPlayButton.Location = new System.Drawing.Point(4, 412);
            this.saveAndPlayButton.Name = "saveAndPlayButton";
            this.saveAndPlayButton.Size = new System.Drawing.Size(78, 22);
            this.saveAndPlayButton.TabIndex = 101;
            this.saveAndPlayButton.Text = "Save && &Play";
            this.saveAndPlayButton.UseVisualStyleBackColor = true;
            this.saveAndPlayButton.Click += new System.EventHandler(this.saveAndPlayButton_Click);
            // 
            // installButton
            // 
            this.installButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.installButton.AutoSize = true;
            this.installButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.installButton.Location = new System.Drawing.Point(168, 412);
            this.installButton.Name = "installButton";
            this.installButton.Size = new System.Drawing.Size(80, 22);
            this.installButton.TabIndex = 103;
            this.installButton.Text = "Install loader";
            this.installButton.UseVisualStyleBackColor = true;
            this.installButton.Click += new System.EventHandler(this.installButton_Click);
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPageMods);
            this.tabControl1.Controls.Add(this.tabPageCodes);
            this.tabControl1.Controls.Add(this.tabPageGraphics);
            this.tabControl1.Controls.Add(this.tabPageInput);
            this.tabControl1.Controls.Add(this.tabPageSound);
            this.tabControl1.Controls.Add(this.tabPageOptions);
            this.tabControl1.Controls.Add(this.tabPageDebug);
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Margin = new System.Windows.Forms.Padding(0, 0, 0, 3);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(410, 406);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPageMods
            // 
            this.tabPageMods.Controls.Add(this.configureModButton);
            this.tabPageMods.Controls.Add(this.modBottomButton);
            this.tabPageMods.Controls.Add(this.modTopButton);
            this.tabPageMods.Controls.Add(this.buttonNewMod);
            this.tabPageMods.Controls.Add(this.buttonRefreshModList);
            this.tabPageMods.Controls.Add(this.modDescription);
            this.tabPageMods.Controls.Add(this.modDownButton);
            this.tabPageMods.Controls.Add(this.modUpButton);
            this.tabPageMods.Controls.Add(this.modListView);
            this.tabPageMods.Location = new System.Drawing.Point(4, 22);
            this.tabPageMods.Name = "tabPageMods";
            this.tabPageMods.Padding = new System.Windows.Forms.Padding(3, 3, 3, 3);
            this.tabPageMods.Size = new System.Drawing.Size(402, 380);
            this.tabPageMods.TabIndex = 0;
            this.tabPageMods.Text = "Mods";
            this.tabPageMods.UseVisualStyleBackColor = true;
            // 
            // configureModButton
            // 
            this.configureModButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.configureModButton.Enabled = false;
            this.configureModButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.configureModButton.Location = new System.Drawing.Point(87, 288);
            this.configureModButton.Name = "configureModButton";
            this.configureModButton.Size = new System.Drawing.Size(75, 23);
            this.configureModButton.TabIndex = 9;
            this.configureModButton.Text = "Configure...";
            this.configureModButton.UseVisualStyleBackColor = true;
            this.configureModButton.Click += new System.EventHandler(this.configureModButton_Click);
            // 
            // modBottomButton
            // 
            this.modBottomButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.modBottomButton.AutoSize = true;
            this.modBottomButton.Enabled = false;
            this.modBottomButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.modBottomButton.Location = new System.Drawing.Point(369, 114);
            this.modBottomButton.Name = "modBottomButton";
            this.modBottomButton.Size = new System.Drawing.Size(31, 29);
            this.modBottomButton.TabIndex = 8;
            this.modBottomButton.Text = "⤓";
            this.modBottomButton.UseVisualStyleBackColor = true;
            this.modBottomButton.Click += new System.EventHandler(this.modBottomButton_Click);
            // 
            // modTopButton
            // 
            this.modTopButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.modTopButton.AutoSize = true;
            this.modTopButton.Enabled = false;
            this.modTopButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.modTopButton.Location = new System.Drawing.Point(369, 6);
            this.modTopButton.Name = "modTopButton";
            this.modTopButton.Size = new System.Drawing.Size(31, 29);
            this.modTopButton.TabIndex = 7;
            this.modTopButton.Text = "⤒";
            this.modTopButton.UseVisualStyleBackColor = true;
            this.modTopButton.Click += new System.EventHandler(this.modTopButton_Click);
            // 
            // buttonNewMod
            // 
            this.buttonNewMod.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonNewMod.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.buttonNewMod.Location = new System.Drawing.Point(292, 288);
            this.buttonNewMod.Name = "buttonNewMod";
            this.buttonNewMod.Size = new System.Drawing.Size(75, 23);
            this.buttonNewMod.TabIndex = 5;
            this.buttonNewMod.Text = "&New...";
            this.buttonNewMod.UseVisualStyleBackColor = true;
            this.buttonNewMod.Click += new System.EventHandler(this.buttonNewMod_Click);
            // 
            // modDownButton
            // 
            this.modDownButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.modDownButton.AutoSize = true;
            this.modDownButton.Enabled = false;
            this.modDownButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.modDownButton.Location = new System.Drawing.Point(369, 78);
            this.modDownButton.Name = "modDownButton";
            this.modDownButton.Size = new System.Drawing.Size(31, 29);
            this.modDownButton.TabIndex = 2;
            this.modDownButton.Text = "↓";
            this.modDownButton.UseVisualStyleBackColor = true;
            this.modDownButton.Click += new System.EventHandler(this.modDownButton_Click);
            // 
            // modUpButton
            // 
            this.modUpButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.modUpButton.AutoSize = true;
            this.modUpButton.Enabled = false;
            this.modUpButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.modUpButton.Location = new System.Drawing.Point(369, 42);
            this.modUpButton.Name = "modUpButton";
            this.modUpButton.Size = new System.Drawing.Size(31, 29);
            this.modUpButton.TabIndex = 1;
            this.modUpButton.Text = "↑";
            this.modUpButton.UseVisualStyleBackColor = true;
            this.modUpButton.Click += new System.EventHandler(this.modUpButton_Click);
            // 
            // tabPageCodes
            // 
            this.tabPageCodes.Controls.Add(this.codesCheckedListBox);
            this.tabPageCodes.Location = new System.Drawing.Point(4, 22);
            this.tabPageCodes.Name = "tabPageCodes";
            this.tabPageCodes.Size = new System.Drawing.Size(402, 380);
            this.tabPageCodes.TabIndex = 1;
            this.tabPageCodes.Text = "Codes";
            this.tabPageCodes.UseVisualStyleBackColor = true;
            // 
            // codesCheckedListBox
            // 
            this.codesCheckedListBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.codesCheckedListBox.FormattingEnabled = true;
            this.codesCheckedListBox.Location = new System.Drawing.Point(0, 0);
            this.codesCheckedListBox.Name = "codesCheckedListBox";
            this.codesCheckedListBox.Size = new System.Drawing.Size(402, 380);
            this.codesCheckedListBox.TabIndex = 0;
            this.codesCheckedListBox.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.codesCheckedListBox_ItemCheck);
            // 
            // tabPageGraphics
            // 
            this.tabPageGraphics.AutoScroll = true;
            this.tabPageGraphics.Controls.Add(this.groupBox_WindowMode);
            this.tabPageGraphics.Controls.Add(this.groupBox_Visuals);
            this.tabPageGraphics.Controls.Add(this.groupBox_Display);
            this.tabPageGraphics.Location = new System.Drawing.Point(4, 22);
            this.tabPageGraphics.Name = "tabPageGraphics";
            this.tabPageGraphics.Padding = new System.Windows.Forms.Padding(3, 3, 3, 3);
            this.tabPageGraphics.Size = new System.Drawing.Size(402, 380);
            this.tabPageGraphics.TabIndex = 2;
            this.tabPageGraphics.Text = "Graphics";
            this.tabPageGraphics.UseVisualStyleBackColor = true;
            // 
            // groupBox_WindowMode
            // 
            this.groupBox_WindowMode.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox_WindowMode.Controls.Add(this.checkWindowResize);
            this.groupBox_WindowMode.Controls.Add(this.customWindowSizeCheckBox);
            this.groupBox_WindowMode.Controls.Add(this.maintainWindowAspectRatioCheckBox);
            this.groupBox_WindowMode.Controls.Add(this.windowHeight);
            this.groupBox_WindowMode.Controls.Add(this.windowWidth);
            this.groupBox_WindowMode.Controls.Add(label2);
            this.groupBox_WindowMode.Location = new System.Drawing.Point(6, 133);
            this.groupBox_WindowMode.Name = "groupBox_WindowMode";
            this.groupBox_WindowMode.Size = new System.Drawing.Size(390, 75);
            this.groupBox_WindowMode.TabIndex = 3;
            this.groupBox_WindowMode.TabStop = false;
            this.groupBox_WindowMode.Text = "Window mode";
            // 
            // checkWindowResize
            // 
            this.checkWindowResize.AutoSize = true;
            this.checkWindowResize.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.checkWindowResize.Location = new System.Drawing.Point(6, 48);
            this.checkWindowResize.Name = "checkWindowResize";
            this.checkWindowResize.Size = new System.Drawing.Size(120, 18);
            this.checkWindowResize.TabIndex = 16;
            this.checkWindowResize.Text = "Resizable Window";
            this.toolTip.SetToolTip(this.checkWindowResize, "Allows the window to be resized and dynamically adjusts resolution to match.");
            this.checkWindowResize.UseVisualStyleBackColor = true;
            // 
            // customWindowSizeCheckBox
            // 
            this.customWindowSizeCheckBox.AutoSize = true;
            this.customWindowSizeCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.customWindowSizeCheckBox.Location = new System.Drawing.Point(6, 20);
            this.customWindowSizeCheckBox.Name = "customWindowSizeCheckBox";
            this.customWindowSizeCheckBox.Size = new System.Drawing.Size(135, 18);
            this.customWindowSizeCheckBox.TabIndex = 11;
            this.customWindowSizeCheckBox.Text = "Custom Window Size:";
            this.toolTip.SetToolTip(this.customWindowSizeCheckBox, "Allows you to specify a fixed size for the window in windowed mode.");
            this.customWindowSizeCheckBox.UseVisualStyleBackColor = true;
            this.customWindowSizeCheckBox.CheckedChanged += new System.EventHandler(this.customWindowSizeCheckBox_CheckedChanged);
            // 
            // maintainWindowAspectRatioCheckBox
            // 
            this.maintainWindowAspectRatioCheckBox.AutoSize = true;
            this.maintainWindowAspectRatioCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.maintainWindowAspectRatioCheckBox.Location = new System.Drawing.Point(151, 48);
            this.maintainWindowAspectRatioCheckBox.Name = "maintainWindowAspectRatioCheckBox";
            this.maintainWindowAspectRatioCheckBox.Size = new System.Drawing.Size(136, 18);
            this.maintainWindowAspectRatioCheckBox.TabIndex = 15;
            this.maintainWindowAspectRatioCheckBox.Text = "Maintain Aspect Ratio";
            this.toolTip.SetToolTip(this.maintainWindowAspectRatioCheckBox, "Forces the window\'s aspect ratio to match the current custom resolution\'s.");
            this.maintainWindowAspectRatioCheckBox.UseVisualStyleBackColor = true;
            this.maintainWindowAspectRatioCheckBox.CheckedChanged += new System.EventHandler(this.maintainWindowAspectRatioCheckBox_CheckedChanged);
            // 
            // windowHeight
            // 
            this.windowHeight.Location = new System.Drawing.Point(237, 19);
            this.windowHeight.Maximum = new decimal(new int[] {
            2147483647,
            0,
            0,
            0});
            this.windowHeight.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.windowHeight.Name = "windowHeight";
            this.windowHeight.Size = new System.Drawing.Size(62, 20);
            this.windowHeight.TabIndex = 14;
            this.windowHeight.Value = new decimal(new int[] {
            480,
            0,
            0,
            0});
            this.windowHeight.ValueChanged += new System.EventHandler(this.windowHeight_ValueChanged);
            // 
            // windowWidth
            // 
            this.windowWidth.Location = new System.Drawing.Point(151, 19);
            this.windowWidth.Maximum = new decimal(new int[] {
            2147483647,
            0,
            0,
            0});
            this.windowWidth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.windowWidth.Name = "windowWidth";
            this.windowWidth.Size = new System.Drawing.Size(62, 20);
            this.windowWidth.TabIndex = 12;
            this.windowWidth.Value = new decimal(new int[] {
            640,
            0,
            0,
            0});
            // 
            // groupBox_Visuals
            // 
            this.groupBox_Visuals.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox_Visuals.Controls.Add(this.buttonUpdateD3D8to9);
            this.groupBox_Visuals.Controls.Add(this.checkPolyBuff);
            this.groupBox_Visuals.Controls.Add(this.checkBoxEnableD3D9);
            this.groupBox_Visuals.Controls.Add(this.label7);
            this.groupBox_Visuals.Controls.Add(this.checkScaleHud);
            this.groupBox_Visuals.Controls.Add(this.label6);
            this.groupBox_Visuals.Controls.Add(this.comboFmvFill);
            this.groupBox_Visuals.Controls.Add(this.comboFog);
            this.groupBox_Visuals.Controls.Add(this.comboBackgroundFill);
            this.groupBox_Visuals.Controls.Add(this.comboClip);
            this.groupBox_Visuals.Controls.Add(this.label10);
            this.groupBox_Visuals.Controls.Add(this.label11);
            this.groupBox_Visuals.Controls.Add(this.label12);
            this.groupBox_Visuals.Controls.Add(this.comboFramerate);
            this.groupBox_Visuals.Controls.Add(this.forceMipmappingCheckBox);
            this.groupBox_Visuals.Controls.Add(this.forceTextureFilterCheckBox);
            this.groupBox_Visuals.Location = new System.Drawing.Point(6, 214);
            this.groupBox_Visuals.Name = "groupBox_Visuals";
            this.groupBox_Visuals.Size = new System.Drawing.Size(390, 164);
            this.groupBox_Visuals.TabIndex = 1;
            this.groupBox_Visuals.TabStop = false;
            this.groupBox_Visuals.Text = "Visuals";
            // 
            // buttonUpdateD3D8to9
            // 
            this.buttonUpdateD3D8to9.Location = new System.Drawing.Point(226, 135);
            this.buttonUpdateD3D8to9.Name = "buttonUpdateD3D8to9";
            this.buttonUpdateD3D8to9.Size = new System.Drawing.Size(100, 23);
            this.buttonUpdateD3D8to9.TabIndex = 13;
            this.buttonUpdateD3D8to9.Text = "Update D3D8to9";
            this.buttonUpdateD3D8to9.UseVisualStyleBackColor = true;
            this.buttonUpdateD3D8to9.Visible = false;
            this.buttonUpdateD3D8to9.Click += new System.EventHandler(this.buttonUpdateD3D8to9_Click);
            // 
            // checkPolyBuff
            // 
            this.checkPolyBuff.AutoSize = true;
            this.checkPolyBuff.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.checkPolyBuff.Location = new System.Drawing.Point(226, 114);
            this.checkPolyBuff.Name = "checkPolyBuff";
            this.checkPolyBuff.Size = new System.Drawing.Size(143, 18);
            this.checkPolyBuff.TabIndex = 0;
            this.checkPolyBuff.Text = "Disable Vertex Color Fix";
            this.toolTip.SetToolTip(this.checkPolyBuff, "The Mod Loader fixes an issue with the game\'s renderer ignoring object vertex col" +
        "ors. Tick this checkbox to disable the fix.");
            this.checkPolyBuff.UseVisualStyleBackColor = true;
            // 
            // checkBoxEnableD3D9
            // 
            this.checkBoxEnableD3D9.AutoSize = true;
            this.checkBoxEnableD3D9.Enabled = false;
            this.checkBoxEnableD3D9.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.checkBoxEnableD3D9.Location = new System.Drawing.Point(226, 18);
            this.checkBoxEnableD3D9.Name = "checkBoxEnableD3D9";
            this.checkBoxEnableD3D9.Size = new System.Drawing.Size(119, 18);
            this.checkBoxEnableD3D9.TabIndex = 12;
            this.checkBoxEnableD3D9.Text = "Enable Direct3D 9";
            this.toolTip.SetToolTip(this.checkBoxEnableD3D9, "Use Direct3D 8 to 9 wrapper (recommended). Required for the Lantern Engine mod.");
            this.checkBoxEnableD3D9.UseVisualStyleBackColor = true;
            this.checkBoxEnableD3D9.Click += new System.EventHandler(this.checkBoxEnableD3D9_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(47, 133);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(73, 13);
            this.label7.TabIndex = 3;
            this.label7.Text = "FMV fill mode:";
            // 
            // checkScaleHud
            // 
            this.checkScaleHud.AutoSize = true;
            this.checkScaleHud.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.checkScaleHud.Location = new System.Drawing.Point(226, 90);
            this.checkScaleHud.Name = "checkScaleHud";
            this.checkScaleHud.Size = new System.Drawing.Size(81, 18);
            this.checkScaleHud.TabIndex = 0;
            this.checkScaleHud.Text = "UI Scaling";
            this.toolTip.SetToolTip(this.checkScaleHud, "Enable scaling for UI elements (recommended).");
            this.checkScaleHud.UseVisualStyleBackColor = true;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(11, 106);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(109, 13);
            this.label6.TabIndex = 1;
            this.label6.Text = "Background fill mode:";
            // 
            // comboFmvFill
            // 
            this.comboFmvFill.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboFmvFill.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.comboFmvFill.FormattingEnabled = true;
            this.comboFmvFill.Items.AddRange(new object[] {
            "Stretch [Default]",
            "Fit",
            "Fill"});
            this.comboFmvFill.Location = new System.Drawing.Point(128, 130);
            this.comboFmvFill.Name = "comboFmvFill";
            this.comboFmvFill.Size = new System.Drawing.Size(89, 21);
            this.comboFmvFill.TabIndex = 4;
            // 
            // comboFog
            // 
            this.comboFog.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboFog.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.comboFog.FormattingEnabled = true;
            this.comboFog.Items.AddRange(new object[] {
            "Auto",
            "Emulation"});
            this.comboFog.Location = new System.Drawing.Point(96, 71);
            this.comboFog.Name = "comboFog";
            this.comboFog.Size = new System.Drawing.Size(121, 21);
            this.comboFog.TabIndex = 11;
            this.toolTip.SetToolTip(this.comboFog, "Sets fog mode. Recommended: Auto\r\nOnly set to emulation if you have an archaic vi" +
        "deo card from\r\nbefore 2003.");
            // 
            // comboBackgroundFill
            // 
            this.comboBackgroundFill.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBackgroundFill.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.comboBackgroundFill.FormattingEnabled = true;
            this.comboBackgroundFill.Items.AddRange(new object[] {
            "Stretch [Default]",
            "Fit",
            "Fill"});
            this.comboBackgroundFill.Location = new System.Drawing.Point(128, 103);
            this.comboBackgroundFill.Name = "comboBackgroundFill";
            this.comboBackgroundFill.Size = new System.Drawing.Size(89, 21);
            this.comboBackgroundFill.TabIndex = 2;
            // 
            // comboClip
            // 
            this.comboClip.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboClip.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.comboClip.FormattingEnabled = true;
            this.comboClip.Items.AddRange(new object[] {
            "High (best)",
            "Low",
            "Lowest"});
            this.comboClip.Location = new System.Drawing.Point(96, 44);
            this.comboClip.Name = "comboClip";
            this.comboClip.Size = new System.Drawing.Size(121, 21);
            this.comboClip.TabIndex = 10;
            this.toolTip.SetToolTip(this.comboClip, "Sets detail level. Recommended: High.\r\nAt lower detail level the game skips drawi" +
        "ng distant\r\nobjects, disables some effects and pauses some animations.");
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(12, 74);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(76, 13);
            this.label10.TabIndex = 8;
            this.label10.Text = "Fog emulation:";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(26, 47);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(62, 13);
            this.label11.TabIndex = 7;
            this.label11.Text = "Detail level:";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(28, 20);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(60, 13);
            this.label12.TabIndex = 6;
            this.label12.Text = "Frame rate:";
            // 
            // comboFramerate
            // 
            this.comboFramerate.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboFramerate.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.comboFramerate.FormattingEnabled = true;
            this.comboFramerate.Items.AddRange(new object[] {
            "60 FPS",
            "30 FPS",
            "15 FPS"});
            this.comboFramerate.Location = new System.Drawing.Point(96, 17);
            this.comboFramerate.Name = "comboFramerate";
            this.comboFramerate.Size = new System.Drawing.Size(121, 21);
            this.comboFramerate.TabIndex = 9;
            this.toolTip.SetToolTip(this.comboFramerate, "Sets the target framerate. Recommended: 60 FPS.\r\nHigher framerate is recommended " +
        "for smoother gameplay.");
            // 
            // forceMipmappingCheckBox
            // 
            this.forceMipmappingCheckBox.AutoSize = true;
            this.forceMipmappingCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.forceMipmappingCheckBox.Location = new System.Drawing.Point(226, 42);
            this.forceMipmappingCheckBox.Name = "forceMipmappingCheckBox";
            this.forceMipmappingCheckBox.Size = new System.Drawing.Size(119, 18);
            this.forceMipmappingCheckBox.TabIndex = 1;
            this.forceMipmappingCheckBox.Text = "Force Mipmapping";
            this.toolTip.SetToolTip(this.forceMipmappingCheckBox, "Generates mipmaps for all textures that don\'t have them.");
            this.forceMipmappingCheckBox.UseVisualStyleBackColor = true;
            // 
            // forceTextureFilterCheckBox
            // 
            this.forceTextureFilterCheckBox.AutoSize = true;
            this.forceTextureFilterCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.forceTextureFilterCheckBox.Location = new System.Drawing.Point(226, 66);
            this.forceTextureFilterCheckBox.Name = "forceTextureFilterCheckBox";
            this.forceTextureFilterCheckBox.Size = new System.Drawing.Size(137, 18);
            this.forceTextureFilterCheckBox.TabIndex = 0;
            this.forceTextureFilterCheckBox.Text = "Force Texture Filtering";
            this.toolTip.SetToolTip(this.forceTextureFilterCheckBox, "Smooths scaled UI textures by changing the filter method from Point to Linear.");
            this.forceTextureFilterCheckBox.UseVisualStyleBackColor = true;
            // 
            // groupBox_Display
            // 
            this.groupBox_Display.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox_Display.Controls.Add(this.comboResolutionPreset);
            this.groupBox_Display.Controls.Add(this.radioWindowMode);
            this.groupBox_Display.Controls.Add(label5);
            this.groupBox_Display.Controls.Add(this.checkVsync);
            this.groupBox_Display.Controls.Add(this.radioFullscreen);
            this.groupBox_Display.Controls.Add(this.stretchFullscreenCheckBox);
            this.groupBox_Display.Controls.Add(label3);
            this.groupBox_Display.Controls.Add(this.screenNumComboBox);
            this.groupBox_Display.Controls.Add(this.forceAspectRatioCheckBox);
            this.groupBox_Display.Controls.Add(this.horizontalResolution);
            this.groupBox_Display.Controls.Add(this.windowedFullscreenCheckBox);
            this.groupBox_Display.Controls.Add(label1);
            this.groupBox_Display.Controls.Add(this.verticalResolution);
            this.groupBox_Display.Location = new System.Drawing.Point(6, 6);
            this.groupBox_Display.Name = "groupBox_Display";
            this.groupBox_Display.Size = new System.Drawing.Size(390, 121);
            this.groupBox_Display.TabIndex = 0;
            this.groupBox_Display.TabStop = false;
            this.groupBox_Display.Text = "Display";
            // 
            // comboResolutionPreset
            // 
            this.comboResolutionPreset.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboResolutionPreset.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.comboResolutionPreset.FormattingEnabled = true;
            this.comboResolutionPreset.Items.AddRange(new object[] {
            "640x480",
            "800x600",
            "1024x768",
            "1152x864",
            "1280x960",
            "1280x1024",
            "Native",
            "1/2x Native",
            "2x Native",
            "720p",
            "1080p",
            "4K"});
            this.comboResolutionPreset.Location = new System.Drawing.Point(226, 46);
            this.comboResolutionPreset.Name = "comboResolutionPreset";
            this.comboResolutionPreset.Size = new System.Drawing.Size(145, 21);
            this.comboResolutionPreset.TabIndex = 18;
            this.toolTip.SetToolTip(this.comboResolutionPreset, "Allows you to select pre-set values for the resolution.");
            this.comboResolutionPreset.SelectedIndexChanged += new System.EventHandler(this.comboResolutionPreset_SelectedIndexChanged);
            // 
            // radioWindowMode
            // 
            this.radioWindowMode.AutoSize = true;
            this.radioWindowMode.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.radioWindowMode.Location = new System.Drawing.Point(104, 73);
            this.radioWindowMode.Name = "radioWindowMode";
            this.radioWindowMode.Size = new System.Drawing.Size(99, 18);
            this.radioWindowMode.TabIndex = 1;
            this.radioWindowMode.Text = "Window mode";
            this.radioWindowMode.UseVisualStyleBackColor = true;
            // 
            // checkVsync
            // 
            this.checkVsync.AutoSize = true;
            this.checkVsync.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.checkVsync.Location = new System.Drawing.Point(226, 97);
            this.checkVsync.Name = "checkVsync";
            this.checkVsync.Size = new System.Drawing.Size(100, 18);
            this.checkVsync.TabIndex = 4;
            this.checkVsync.Text = "Enable V-sync";
            this.toolTip.SetToolTip(this.checkVsync, "Limit the game\'s framerate by the monitor refresh rate.");
            this.checkVsync.UseVisualStyleBackColor = true;
            // 
            // radioFullscreen
            // 
            this.radioFullscreen.AutoSize = true;
            this.radioFullscreen.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.radioFullscreen.Location = new System.Drawing.Point(9, 73);
            this.radioFullscreen.Name = "radioFullscreen";
            this.radioFullscreen.Size = new System.Drawing.Size(79, 18);
            this.radioFullscreen.TabIndex = 0;
            this.radioFullscreen.Text = "Fullscreen";
            this.radioFullscreen.UseVisualStyleBackColor = true;
            // 
            // tabPageInput
            // 
            this.tabPageInput.Controls.Add(this.groupMouseMode);
            this.tabPageInput.Controls.Add(this.groupBoxController);
            this.tabPageInput.Location = new System.Drawing.Point(4, 22);
            this.tabPageInput.Name = "tabPageInput";
            this.tabPageInput.Padding = new System.Windows.Forms.Padding(3, 3, 3, 3);
            this.tabPageInput.Size = new System.Drawing.Size(402, 380);
            this.tabPageInput.TabIndex = 6;
            this.tabPageInput.Text = "Input";
            this.tabPageInput.UseVisualStyleBackColor = true;
            // 
            // groupMouseMode
            // 
            this.groupMouseMode.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupMouseMode.Controls.Add(this.label17);
            this.groupMouseMode.Controls.Add(this.comboMouseButtons);
            this.groupMouseMode.Controls.Add(this.radioMouseModeRelease);
            this.groupMouseMode.Controls.Add(this.radioMouseModeHold);
            this.groupMouseMode.Controls.Add(this.comboMouseActions);
            this.groupMouseMode.Controls.Add(this.label18);
            this.groupMouseMode.Location = new System.Drawing.Point(6, 6);
            this.groupMouseMode.Name = "groupMouseMode";
            this.groupMouseMode.Size = new System.Drawing.Size(390, 68);
            this.groupMouseMode.TabIndex = 15;
            this.groupMouseMode.TabStop = false;
            this.groupMouseMode.Text = "Mouse";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(233, 19);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(98, 13);
            this.label17.TabIndex = 3;
            this.label17.Text = "Button Assignment:";
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
            this.comboMouseButtons.Location = new System.Drawing.Point(236, 35);
            this.comboMouseButtons.Name = "comboMouseButtons";
            this.comboMouseButtons.Size = new System.Drawing.Size(148, 21);
            this.comboMouseButtons.TabIndex = 2;
            this.comboMouseButtons.SelectedIndexChanged += new System.EventHandler(this.comboMouseButtons_SelectedIndexChanged);
            // 
            // radioMouseModeRelease
            // 
            this.radioMouseModeRelease.AutoSize = true;
            this.radioMouseModeRelease.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.radioMouseModeRelease.Location = new System.Drawing.Point(6, 42);
            this.radioMouseModeRelease.Name = "radioMouseModeRelease";
            this.radioMouseModeRelease.Size = new System.Drawing.Size(120, 18);
            this.radioMouseModeRelease.TabIndex = 1;
            this.radioMouseModeRelease.TabStop = true;
            this.radioMouseModeRelease.Text = "Drag to Accelerate";
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
            this.radioMouseModeHold.UseVisualStyleBackColor = true;
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
            this.comboMouseActions.Location = new System.Drawing.Point(138, 35);
            this.comboMouseActions.Name = "comboMouseActions";
            this.comboMouseActions.Size = new System.Drawing.Size(92, 21);
            this.comboMouseActions.TabIndex = 1;
            this.comboMouseActions.SelectedIndexChanged += new System.EventHandler(this.comboMouseActions_SelectedIndexChanged);
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(135, 19);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(40, 13);
            this.label18.TabIndex = 0;
            this.label18.Text = "Action:";
            // 
            // groupBoxController
            // 
            this.groupBoxController.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBoxController.Controls.Add(label16);
            this.groupBoxController.Controls.Add(this.controllerConfigName);
            this.groupBoxController.Controls.Add(this.controllerConfigSelect);
            this.groupBoxController.Controls.Add(label15);
            this.groupBoxController.Controls.Add(this.controllerConfigAdd);
            this.groupBoxController.Controls.Add(this.tableLayoutPanel1);
            this.groupBoxController.Controls.Add(this.controllerConfigRemove);
            this.groupBoxController.Location = new System.Drawing.Point(6, 80);
            this.groupBoxController.Name = "groupBoxController";
            this.groupBoxController.Size = new System.Drawing.Size(390, 293);
            this.groupBoxController.TabIndex = 14;
            this.groupBoxController.TabStop = false;
            this.groupBoxController.Text = "Controller (DirectInput)";
            // 
            // controllerConfigName
            // 
            this.controllerConfigName.Enabled = false;
            this.controllerConfigName.Location = new System.Drawing.Point(272, 76);
            this.controllerConfigName.Name = "controllerConfigName";
            this.controllerConfigName.Size = new System.Drawing.Size(112, 20);
            this.controllerConfigName.TabIndex = 13;
            this.controllerConfigName.TextChanged += new System.EventHandler(this.controllerConfigName_TextChanged);
            // 
            // controllerConfigSelect
            // 
            this.controllerConfigSelect.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.controllerConfigSelect.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.controllerConfigSelect.FormattingEnabled = true;
            this.controllerConfigSelect.Location = new System.Drawing.Point(272, 31);
            this.controllerConfigSelect.Name = "controllerConfigSelect";
            this.controllerConfigSelect.Size = new System.Drawing.Size(112, 21);
            this.controllerConfigSelect.TabIndex = 7;
            this.controllerConfigSelect.SelectedIndexChanged += new System.EventHandler(this.controllerConfigSelect_SelectedIndexChanged);
            // 
            // controllerConfigAdd
            // 
            this.controllerConfigAdd.AutoSize = true;
            this.controllerConfigAdd.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.controllerConfigAdd.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.controllerConfigAdd.Location = new System.Drawing.Point(272, 102);
            this.controllerConfigAdd.Name = "controllerConfigAdd";
            this.controllerConfigAdd.Size = new System.Drawing.Size(40, 22);
            this.controllerConfigAdd.TabIndex = 9;
            this.controllerConfigAdd.Text = "Add";
            this.controllerConfigAdd.UseVisualStyleBackColor = true;
            this.controllerConfigAdd.Click += new System.EventHandler(this.controllerConfigAdd_Click);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.AutoSize = true;
            this.tableLayoutPanel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.tableLayoutPanel1.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Single;
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            this.tableLayoutPanel1.Location = new System.Drawing.Point(6, 19);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle());
            this.tableLayoutPanel1.Size = new System.Drawing.Size(3, 2);
            this.tableLayoutPanel1.TabIndex = 11;
            // 
            // controllerConfigRemove
            // 
            this.controllerConfigRemove.AutoSize = true;
            this.controllerConfigRemove.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.controllerConfigRemove.Enabled = false;
            this.controllerConfigRemove.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.controllerConfigRemove.Location = new System.Drawing.Point(323, 102);
            this.controllerConfigRemove.Name = "controllerConfigRemove";
            this.controllerConfigRemove.Size = new System.Drawing.Size(61, 22);
            this.controllerConfigRemove.TabIndex = 10;
            this.controllerConfigRemove.Text = "Remove";
            this.controllerConfigRemove.UseVisualStyleBackColor = true;
            this.controllerConfigRemove.Click += new System.EventHandler(this.controllerConfigRemove_Click);
            // 
            // tabPageSound
            // 
            this.tabPageSound.Controls.Add(this.groupBox_SoundAdvanced);
            this.tabPageSound.Controls.Add(this.groupBox_SoundVolume);
            this.tabPageSound.Controls.Add(this.groupBox_SoundConfig);
            this.tabPageSound.Location = new System.Drawing.Point(4, 22);
            this.tabPageSound.Name = "tabPageSound";
            this.tabPageSound.Padding = new System.Windows.Forms.Padding(3, 3, 3, 3);
            this.tabPageSound.Size = new System.Drawing.Size(402, 380);
            this.tabPageSound.TabIndex = 4;
            this.tabPageSound.Text = "Sound";
            this.tabPageSound.UseVisualStyleBackColor = true;
            // 
            // groupBox_SoundAdvanced
            // 
            this.groupBox_SoundAdvanced.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox_SoundAdvanced.Controls.Add(this.checkBASS);
            this.groupBox_SoundAdvanced.Location = new System.Drawing.Point(6, 197);
            this.groupBox_SoundAdvanced.Name = "groupBox_SoundAdvanced";
            this.groupBox_SoundAdvanced.Size = new System.Drawing.Size(390, 41);
            this.groupBox_SoundAdvanced.TabIndex = 4;
            this.groupBox_SoundAdvanced.TabStop = false;
            this.groupBox_SoundAdvanced.Text = "Advanced";
            // 
            // checkBASS
            // 
            this.checkBASS.AutoSize = true;
            this.checkBASS.Location = new System.Drawing.Point(6, 19);
            this.checkBASS.Name = "checkBASS";
            this.checkBASS.Size = new System.Drawing.Size(92, 17);
            this.checkBASS.TabIndex = 0;
            this.checkBASS.Text = "Disable BASS";
            this.toolTip.SetToolTip(this.checkBASS, "Revert to the original game\'s music/voice playback system that can only use WMA f" +
        "iles. Normally you should leave this checkbox unchecked.");
            this.checkBASS.UseVisualStyleBackColor = true;
            // 
            // groupBox_SoundVolume
            // 
            this.groupBox_SoundVolume.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox_SoundVolume.Controls.Add(this.labelVoiceVol);
            this.groupBox_SoundVolume.Controls.Add(this.labelMusicVol);
            this.groupBox_SoundVolume.Controls.Add(this.trackBarVoiceVol);
            this.groupBox_SoundVolume.Controls.Add(this.label13);
            this.groupBox_SoundVolume.Controls.Add(this.trackBarMusicVol);
            this.groupBox_SoundVolume.Controls.Add(this.label14);
            this.groupBox_SoundVolume.Location = new System.Drawing.Point(6, 77);
            this.groupBox_SoundVolume.Name = "groupBox_SoundVolume";
            this.groupBox_SoundVolume.Size = new System.Drawing.Size(390, 114);
            this.groupBox_SoundVolume.TabIndex = 3;
            this.groupBox_SoundVolume.TabStop = false;
            this.groupBox_SoundVolume.Text = "Volume";
            // 
            // labelVoiceVol
            // 
            this.labelVoiceVol.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.labelVoiceVol.AutoSize = true;
            this.labelVoiceVol.Location = new System.Drawing.Point(359, 67);
            this.labelVoiceVol.Name = "labelVoiceVol";
            this.labelVoiceVol.Size = new System.Drawing.Size(25, 13);
            this.labelVoiceVol.TabIndex = 7;
            this.labelVoiceVol.Text = "100";
            // 
            // labelMusicVol
            // 
            this.labelMusicVol.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.labelMusicVol.AutoSize = true;
            this.labelMusicVol.Location = new System.Drawing.Point(359, 21);
            this.labelMusicVol.Name = "labelMusicVol";
            this.labelMusicVol.Size = new System.Drawing.Size(25, 13);
            this.labelMusicVol.TabIndex = 6;
            this.labelMusicVol.Text = "100";
            // 
            // trackBarVoiceVol
            // 
            this.trackBarVoiceVol.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.trackBarVoiceVol.BackColor = System.Drawing.SystemColors.Window;
            this.trackBarVoiceVol.Location = new System.Drawing.Point(89, 65);
            this.trackBarVoiceVol.Margin = new System.Windows.Forms.Padding(0);
            this.trackBarVoiceVol.Maximum = 100;
            this.trackBarVoiceVol.Name = "trackBarVoiceVol";
            this.trackBarVoiceVol.Size = new System.Drawing.Size(267, 45);
            this.trackBarVoiceVol.TabIndex = 5;
            this.trackBarVoiceVol.TickFrequency = 10;
            this.trackBarVoiceVol.Value = 100;
            this.trackBarVoiceVol.ValueChanged += new System.EventHandler(this.trackBarVoiceVol_ValueChanged);
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(6, 21);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(75, 13);
            this.label13.TabIndex = 3;
            this.label13.Text = "Music volume:";
            // 
            // trackBarMusicVol
            // 
            this.trackBarMusicVol.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.trackBarMusicVol.BackColor = System.Drawing.SystemColors.Window;
            this.trackBarMusicVol.LargeChange = 10;
            this.trackBarMusicVol.Location = new System.Drawing.Point(89, 18);
            this.trackBarMusicVol.Margin = new System.Windows.Forms.Padding(0);
            this.trackBarMusicVol.Maximum = 100;
            this.trackBarMusicVol.Name = "trackBarMusicVol";
            this.trackBarMusicVol.Size = new System.Drawing.Size(267, 45);
            this.trackBarMusicVol.TabIndex = 4;
            this.trackBarMusicVol.TickFrequency = 10;
            this.trackBarMusicVol.Value = 100;
            this.trackBarMusicVol.ValueChanged += new System.EventHandler(this.trackBarMusicVol_ValueChanged);
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(7, 67);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(74, 13);
            this.label14.TabIndex = 2;
            this.label14.Text = "Voice volume:";
            // 
            // groupBox_SoundConfig
            // 
            this.groupBox_SoundConfig.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox_SoundConfig.Controls.Add(this.checkMusic);
            this.groupBox_SoundConfig.Controls.Add(this.checkSound);
            this.groupBox_SoundConfig.Controls.Add(this.check3DSound);
            this.groupBox_SoundConfig.Location = new System.Drawing.Point(6, 6);
            this.groupBox_SoundConfig.Name = "groupBox_SoundConfig";
            this.groupBox_SoundConfig.Size = new System.Drawing.Size(390, 65);
            this.groupBox_SoundConfig.TabIndex = 2;
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
            this.toolTip.SetToolTip(this.check3DSound, "Enable or disable surround sound.");
            this.check3DSound.UseVisualStyleBackColor = true;
            // 
            // tabPageOptions
            // 
            this.tabPageOptions.Controls.Add(this.groupBox3);
            this.tabPageOptions.Controls.Add(this.installURLHandlerButton);
            this.tabPageOptions.Controls.Add(this.groupBox4);
            this.tabPageOptions.Controls.Add(groupBox2);
            this.tabPageOptions.Location = new System.Drawing.Point(4, 22);
            this.tabPageOptions.Name = "tabPageOptions";
            this.tabPageOptions.Padding = new System.Windows.Forms.Padding(3, 3, 3, 3);
            this.tabPageOptions.Size = new System.Drawing.Size(402, 380);
            this.tabPageOptions.TabIndex = 3;
            this.tabPageOptions.Text = "Options";
            this.tabPageOptions.UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox3.Controls.Add(this.buttonSaveProfile);
            this.groupBox3.Controls.Add(this.buttonLoadProfile);
            this.groupBox3.Controls.Add(this.profileNameBox);
            this.groupBox3.Location = new System.Drawing.Point(6, 196);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(390, 58);
            this.groupBox3.TabIndex = 4;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Profiles";
            // 
            // buttonSaveProfile
            // 
            this.buttonSaveProfile.Enabled = false;
            this.buttonSaveProfile.Location = new System.Drawing.Point(266, 19);
            this.buttonSaveProfile.Name = "buttonSaveProfile";
            this.buttonSaveProfile.Size = new System.Drawing.Size(75, 23);
            this.buttonSaveProfile.TabIndex = 2;
            this.buttonSaveProfile.Text = "Save";
            this.toolTip.SetToolTip(this.buttonSaveProfile, "Saves all current settings to the profile with the selected name.");
            this.buttonSaveProfile.UseVisualStyleBackColor = true;
            this.buttonSaveProfile.Click += new System.EventHandler(this.buttonSaveProfile_Click);
            // 
            // buttonLoadProfile
            // 
            this.buttonLoadProfile.Enabled = false;
            this.buttonLoadProfile.Location = new System.Drawing.Point(185, 19);
            this.buttonLoadProfile.Name = "buttonLoadProfile";
            this.buttonLoadProfile.Size = new System.Drawing.Size(75, 23);
            this.buttonLoadProfile.TabIndex = 1;
            this.buttonLoadProfile.Text = "Load";
            this.toolTip.SetToolTip(this.buttonLoadProfile, "Loads the profile with the selected name.");
            this.buttonLoadProfile.UseVisualStyleBackColor = true;
            this.buttonLoadProfile.Click += new System.EventHandler(this.buttonLoadProfile_Click);
            // 
            // profileNameBox
            // 
            this.profileNameBox.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
            this.profileNameBox.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
            this.profileNameBox.FormattingEnabled = true;
            this.profileNameBox.Location = new System.Drawing.Point(6, 21);
            this.profileNameBox.Name = "profileNameBox";
            this.profileNameBox.Size = new System.Drawing.Size(169, 21);
            this.profileNameBox.TabIndex = 0;
            this.toolTip.SetToolTip(this.profileNameBox, "Type in or select the name of a profile to load/save.");
            this.profileNameBox.TextChanged += new System.EventHandler(this.profileNameBox_TextChanged);
            // 
            // installURLHandlerButton
            // 
            this.installURLHandlerButton.AutoSize = true;
            this.installURLHandlerButton.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.installURLHandlerButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.installURLHandlerButton.Location = new System.Drawing.Point(6, 260);
            this.installURLHandlerButton.Name = "installURLHandlerButton";
            this.installURLHandlerButton.Size = new System.Drawing.Size(113, 22);
            this.installURLHandlerButton.TabIndex = 3;
            this.installURLHandlerButton.Text = "Install URL Handler";
            this.installURLHandlerButton.UseVisualStyleBackColor = true;
            this.installURLHandlerButton.Click += new System.EventHandler(this.installURLHandlerButton_Click);
            // 
            // groupBox4
            // 
            this.groupBox4.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox4.Controls.Add(this.buttonCheckForUpdates);
            this.groupBox4.Controls.Add(this.label4);
            this.groupBox4.Controls.Add(this.checkUpdateModsStartup);
            this.groupBox4.Controls.Add(this.numericUpdateFrequency);
            this.groupBox4.Controls.Add(this.comboUpdateFrequency);
            this.groupBox4.Controls.Add(this.checkUpdateStartup);
            this.groupBox4.Location = new System.Drawing.Point(6, 100);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(390, 90);
            this.groupBox4.TabIndex = 2;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Updates";
            // 
            // buttonCheckForUpdates
            // 
            this.buttonCheckForUpdates.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.buttonCheckForUpdates.Location = new System.Drawing.Point(259, 55);
            this.buttonCheckForUpdates.Name = "buttonCheckForUpdates";
            this.buttonCheckForUpdates.Size = new System.Drawing.Size(75, 23);
            this.buttonCheckForUpdates.TabIndex = 5;
            this.buttonCheckForUpdates.Text = "Check Now";
            this.buttonCheckForUpdates.UseVisualStyleBackColor = true;
            this.buttonCheckForUpdates.Click += new System.EventHandler(this.buttonCheckForUpdates_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 40);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(60, 13);
            this.label4.TabIndex = 2;
            this.label4.Text = "Frequency:";
            // 
            // checkUpdateModsStartup
            // 
            this.checkUpdateModsStartup.AutoSize = true;
            this.checkUpdateModsStartup.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.checkUpdateModsStartup.Location = new System.Drawing.Point(133, 19);
            this.checkUpdateModsStartup.Name = "checkUpdateModsStartup";
            this.checkUpdateModsStartup.Size = new System.Drawing.Size(141, 18);
            this.checkUpdateModsStartup.TabIndex = 1;
            this.checkUpdateModsStartup.Text = "Check mods on startup";
            this.checkUpdateModsStartup.UseVisualStyleBackColor = true;
            // 
            // numericUpdateFrequency
            // 
            this.numericUpdateFrequency.Location = new System.Drawing.Point(133, 57);
            this.numericUpdateFrequency.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numericUpdateFrequency.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpdateFrequency.Name = "numericUpdateFrequency";
            this.numericUpdateFrequency.Size = new System.Drawing.Size(120, 20);
            this.numericUpdateFrequency.TabIndex = 4;
            this.numericUpdateFrequency.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // comboUpdateFrequency
            // 
            this.comboUpdateFrequency.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboUpdateFrequency.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.comboUpdateFrequency.FormattingEnabled = true;
            this.comboUpdateFrequency.Items.AddRange(new object[] {
            "Always",
            "Hours",
            "Days",
            "Weeks"});
            this.comboUpdateFrequency.Location = new System.Drawing.Point(6, 56);
            this.comboUpdateFrequency.Name = "comboUpdateFrequency";
            this.comboUpdateFrequency.Size = new System.Drawing.Size(121, 21);
            this.comboUpdateFrequency.TabIndex = 3;
            this.comboUpdateFrequency.SelectedIndexChanged += new System.EventHandler(this.comboUpdateFrequency_SelectedIndexChanged);
            // 
            // checkUpdateStartup
            // 
            this.checkUpdateStartup.AutoSize = true;
            this.checkUpdateStartup.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.checkUpdateStartup.Location = new System.Drawing.Point(6, 19);
            this.checkUpdateStartup.Name = "checkUpdateStartup";
            this.checkUpdateStartup.Size = new System.Drawing.Size(113, 18);
            this.checkUpdateStartup.TabIndex = 0;
            this.checkUpdateStartup.Text = "Check on startup";
            this.checkUpdateStartup.UseVisualStyleBackColor = true;
            // 
            // tabPageDebug
            // 
            this.tabPageDebug.Controls.Add(this.groupBox1);
            this.tabPageDebug.Controls.Add(this.groupBox12);
            this.tabPageDebug.Location = new System.Drawing.Point(4, 22);
            this.tabPageDebug.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.tabPageDebug.Name = "tabPageDebug";
            this.tabPageDebug.Size = new System.Drawing.Size(402, 380);
            this.tabPageDebug.TabIndex = 5;
            this.tabPageDebug.Text = "Debug";
            this.tabPageDebug.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.crashLogCheckBox);
            this.groupBox1.Controls.Add(this.consoleCheckBox);
            this.groupBox1.Controls.Add(this.screenCheckBox);
            this.groupBox1.Controls.Add(this.fileCheckBox);
            this.groupBox1.Location = new System.Drawing.Point(6, 6);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(390, 42);
            this.groupBox1.TabIndex = 7;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Debug Messages";
            // 
            // crashLogCheckBox
            // 
            this.crashLogCheckBox.AutoSize = true;
            this.crashLogCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.crashLogCheckBox.Location = new System.Drawing.Point(197, 19);
            this.crashLogCheckBox.Name = "crashLogCheckBox";
            this.crashLogCheckBox.Size = new System.Drawing.Size(99, 18);
            this.crashLogCheckBox.TabIndex = 3;
            this.crashLogCheckBox.Text = "Crash Handler";
            this.toolTip.SetToolTip(this.crashLogCheckBox, "Displays the crash address and generates a crash dump when the game crashes.");
            this.crashLogCheckBox.UseVisualStyleBackColor = true;
            // 
            // consoleCheckBox
            // 
            this.consoleCheckBox.AutoSize = true;
            this.consoleCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.consoleCheckBox.Location = new System.Drawing.Point(6, 19);
            this.consoleCheckBox.Name = "consoleCheckBox";
            this.consoleCheckBox.Size = new System.Drawing.Size(70, 18);
            this.consoleCheckBox.TabIndex = 0;
            this.consoleCheckBox.Text = "Console";
            this.toolTip.SetToolTip(this.consoleCheckBox, "Shows a console and displays debug messages.");
            this.consoleCheckBox.UseVisualStyleBackColor = true;
            // 
            // screenCheckBox
            // 
            this.screenCheckBox.AutoSize = true;
            this.screenCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.screenCheckBox.Location = new System.Drawing.Point(76, 19);
            this.screenCheckBox.Name = "screenCheckBox";
            this.screenCheckBox.Size = new System.Drawing.Size(66, 18);
            this.screenCheckBox.TabIndex = 1;
            this.screenCheckBox.Text = "Screen";
            this.toolTip.SetToolTip(this.screenCheckBox, "Displays debug messages ingame.");
            this.screenCheckBox.UseVisualStyleBackColor = true;
            // 
            // fileCheckBox
            // 
            this.fileCheckBox.AutoSize = true;
            this.fileCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.fileCheckBox.Location = new System.Drawing.Point(142, 19);
            this.fileCheckBox.Name = "fileCheckBox";
            this.fileCheckBox.Size = new System.Drawing.Size(48, 18);
            this.fileCheckBox.TabIndex = 2;
            this.fileCheckBox.Text = "File";
            this.toolTip.SetToolTip(this.fileCheckBox, "Logs debug messages to mods/SADXModLoader.log");
            this.fileCheckBox.UseVisualStyleBackColor = true;
            // 
            // groupBox12
            // 
            this.groupBox12.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox12.Controls.Add(this.labelTestSpawnWarning);
            this.groupBox12.Controls.Add(this.labelTestSpawnTime);
            this.groupBox12.Controls.Add(this.comboBoxTestSpawnTime);
            this.groupBox12.Controls.Add(this.checkBoxTestSpawnAngleHex);
            this.groupBox12.Controls.Add(this.checkBoxTestSpawnPosition);
            this.groupBox12.Controls.Add(this.buttonTestSpawnPlay);
            this.groupBox12.Controls.Add(this.comboBoxTestSpawnEvent);
            this.groupBox12.Controls.Add(this.checkBoxTestSpawnEvent);
            this.groupBox12.Controls.Add(this.checkBoxTestSpawnCharacter);
            this.groupBox12.Controls.Add(this.checkBoxTestSpawnLevel);
            this.groupBox12.Controls.Add(this.comboBoxTestSpawnCharacter);
            this.groupBox12.Controls.Add(this.numericUpDownTestSpawnAngle);
            this.groupBox12.Controls.Add(this.labelTestSpawnAngle);
            this.groupBox12.Controls.Add(this.labelTestSpawnY);
            this.groupBox12.Controls.Add(this.numericUpDownTestSpawnSaveID);
            this.groupBox12.Controls.Add(this.checkBoxTestSpawnSave);
            this.groupBox12.Controls.Add(this.labelTestSpawnX);
            this.groupBox12.Controls.Add(this.labelTestSpawnAct);
            this.groupBox12.Controls.Add(this.labelTestSpawnZ);
            this.groupBox12.Controls.Add(this.numericUpDownTestSpawnAct);
            this.groupBox12.Controls.Add(this.numericUpDownTestSpawnZ);
            this.groupBox12.Controls.Add(this.numericUpDownTestSpawnY);
            this.groupBox12.Controls.Add(this.comboBoxTestSpawnLevel);
            this.groupBox12.Controls.Add(this.numericUpDownTestSpawnX);
            this.groupBox12.Location = new System.Drawing.Point(6, 55);
            this.groupBox12.Name = "groupBox12";
            this.groupBox12.Size = new System.Drawing.Size(390, 240);
            this.groupBox12.TabIndex = 6;
            this.groupBox12.TabStop = false;
            this.groupBox12.Text = "Test Spawn";
            // 
            // labelTestSpawnWarning
            // 
            this.labelTestSpawnWarning.AutoSize = true;
            this.labelTestSpawnWarning.Location = new System.Drawing.Point(91, 202);
            this.labelTestSpawnWarning.Name = "labelTestSpawnWarning";
            this.labelTestSpawnWarning.Size = new System.Drawing.Size(293, 13);
            this.labelTestSpawnWarning.TabIndex = 36;
            this.labelTestSpawnWarning.Text = "Overriding levels or characters in events may cause crashes.";
            this.labelTestSpawnWarning.Visible = false;
            // 
            // labelTestSpawnTime
            // 
            this.labelTestSpawnTime.AutoSize = true;
            this.labelTestSpawnTime.Location = new System.Drawing.Point(243, 47);
            this.labelTestSpawnTime.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.labelTestSpawnTime.Name = "labelTestSpawnTime";
            this.labelTestSpawnTime.Size = new System.Drawing.Size(33, 13);
            this.labelTestSpawnTime.TabIndex = 35;
            this.labelTestSpawnTime.Text = "Time:";
            this.labelTestSpawnTime.TextAlign = System.Drawing.ContentAlignment.TopRight;
            this.toolTip.SetToolTip(this.labelTestSpawnTime, "Act ID (zero-based).");
            // 
            // comboBoxTestSpawnTime
            // 
            this.comboBoxTestSpawnTime.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTestSpawnTime.FormattingEnabled = true;
            this.comboBoxTestSpawnTime.Items.AddRange(new object[] {
            "Unset",
            "Day",
            "Evening",
            "Night"});
            this.comboBoxTestSpawnTime.Location = new System.Drawing.Point(281, 44);
            this.comboBoxTestSpawnTime.Name = "comboBoxTestSpawnTime";
            this.comboBoxTestSpawnTime.Size = new System.Drawing.Size(61, 21);
            this.comboBoxTestSpawnTime.TabIndex = 34;
            // 
            // checkBoxTestSpawnAngleHex
            // 
            this.checkBoxTestSpawnAngleHex.AutoSize = true;
            this.checkBoxTestSpawnAngleHex.Enabled = false;
            this.checkBoxTestSpawnAngleHex.Location = new System.Drawing.Point(326, 122);
            this.checkBoxTestSpawnAngleHex.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.checkBoxTestSpawnAngleHex.Name = "checkBoxTestSpawnAngleHex";
            this.checkBoxTestSpawnAngleHex.Size = new System.Drawing.Size(45, 17);
            this.checkBoxTestSpawnAngleHex.TabIndex = 33;
            this.checkBoxTestSpawnAngleHex.Text = "Hex";
            this.toolTip.SetToolTip(this.checkBoxTestSpawnAngleHex, "Display character Y rotation as hexadecimal.");
            this.checkBoxTestSpawnAngleHex.UseVisualStyleBackColor = true;
            this.checkBoxTestSpawnAngleHex.CheckedChanged += new System.EventHandler(this.checkBoxTestSpawnAngleHex_CheckedChanged);
            // 
            // checkBoxTestSpawnPosition
            // 
            this.checkBoxTestSpawnPosition.AutoSize = true;
            this.checkBoxTestSpawnPosition.Location = new System.Drawing.Point(6, 71);
            this.checkBoxTestSpawnPosition.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.checkBoxTestSpawnPosition.Name = "checkBoxTestSpawnPosition";
            this.checkBoxTestSpawnPosition.Size = new System.Drawing.Size(66, 17);
            this.checkBoxTestSpawnPosition.TabIndex = 23;
            this.checkBoxTestSpawnPosition.Text = "Position:";
            this.toolTip.SetToolTip(this.checkBoxTestSpawnPosition, "Force character spawn position.");
            this.checkBoxTestSpawnPosition.UseVisualStyleBackColor = true;
            this.checkBoxTestSpawnPosition.CheckedChanged += new System.EventHandler(this.checkBoxTestSpawnPosition_CheckedChanged);
            // 
            // buttonTestSpawnPlay
            // 
            this.buttonTestSpawnPlay.Location = new System.Drawing.Point(5, 197);
            this.buttonTestSpawnPlay.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.buttonTestSpawnPlay.Name = "buttonTestSpawnPlay";
            this.buttonTestSpawnPlay.Size = new System.Drawing.Size(76, 22);
            this.buttonTestSpawnPlay.TabIndex = 31;
            this.buttonTestSpawnPlay.Text = "Play";
            this.toolTip.SetToolTip(this.buttonTestSpawnPlay, "Run the game with the above Test Spawn settings.");
            this.buttonTestSpawnPlay.UseVisualStyleBackColor = true;
            this.buttonTestSpawnPlay.Click += new System.EventHandler(this.buttonTestSpawnPlay_Click);
            // 
            // comboBoxTestSpawnEvent
            // 
            this.comboBoxTestSpawnEvent.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTestSpawnEvent.Enabled = false;
            this.comboBoxTestSpawnEvent.FormattingEnabled = true;
            this.comboBoxTestSpawnEvent.Location = new System.Drawing.Point(67, 148);
            this.comboBoxTestSpawnEvent.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.comboBoxTestSpawnEvent.Name = "comboBoxTestSpawnEvent";
            this.comboBoxTestSpawnEvent.Size = new System.Drawing.Size(319, 21);
            this.comboBoxTestSpawnEvent.TabIndex = 14;
            this.toolTip.SetToolTip(this.comboBoxTestSpawnEvent, "Start the game on a specific cutscene.");
            // 
            // checkBoxTestSpawnEvent
            // 
            this.checkBoxTestSpawnEvent.AutoSize = true;
            this.checkBoxTestSpawnEvent.Location = new System.Drawing.Point(6, 150);
            this.checkBoxTestSpawnEvent.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.checkBoxTestSpawnEvent.Name = "checkBoxTestSpawnEvent";
            this.checkBoxTestSpawnEvent.Size = new System.Drawing.Size(57, 17);
            this.checkBoxTestSpawnEvent.TabIndex = 22;
            this.checkBoxTestSpawnEvent.Text = "Event:";
            this.toolTip.SetToolTip(this.checkBoxTestSpawnEvent, "Start the game on a specific cutscene.");
            this.checkBoxTestSpawnEvent.UseVisualStyleBackColor = true;
            this.checkBoxTestSpawnEvent.CheckedChanged += new System.EventHandler(this.checkBoxTestSpawnEvent_CheckedChanged);
            // 
            // checkBoxTestSpawnCharacter
            // 
            this.checkBoxTestSpawnCharacter.AutoSize = true;
            this.checkBoxTestSpawnCharacter.Location = new System.Drawing.Point(6, 46);
            this.checkBoxTestSpawnCharacter.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.checkBoxTestSpawnCharacter.Name = "checkBoxTestSpawnCharacter";
            this.checkBoxTestSpawnCharacter.Size = new System.Drawing.Size(75, 17);
            this.checkBoxTestSpawnCharacter.TabIndex = 32;
            this.checkBoxTestSpawnCharacter.Text = "Character:";
            this.toolTip.SetToolTip(this.checkBoxTestSpawnCharacter, "Start the game with a specific character.");
            this.checkBoxTestSpawnCharacter.UseVisualStyleBackColor = true;
            this.checkBoxTestSpawnCharacter.CheckedChanged += new System.EventHandler(this.checkBoxTestSpawnCharacter_CheckedChanged);
            // 
            // checkBoxTestSpawnLevel
            // 
            this.checkBoxTestSpawnLevel.AutoSize = true;
            this.checkBoxTestSpawnLevel.Location = new System.Drawing.Point(6, 21);
            this.checkBoxTestSpawnLevel.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.checkBoxTestSpawnLevel.Name = "checkBoxTestSpawnLevel";
            this.checkBoxTestSpawnLevel.Size = new System.Drawing.Size(55, 17);
            this.checkBoxTestSpawnLevel.TabIndex = 20;
            this.checkBoxTestSpawnLevel.Text = "Level:";
            this.toolTip.SetToolTip(this.checkBoxTestSpawnLevel, "Start the game on a specific level.");
            this.checkBoxTestSpawnLevel.UseVisualStyleBackColor = true;
            this.checkBoxTestSpawnLevel.CheckedChanged += new System.EventHandler(this.checkBoxTestSpawnLevel_CheckedChanged);
            // 
            // comboBoxTestSpawnCharacter
            // 
            this.comboBoxTestSpawnCharacter.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTestSpawnCharacter.Enabled = false;
            this.comboBoxTestSpawnCharacter.FormattingEnabled = true;
            this.comboBoxTestSpawnCharacter.Items.AddRange(new object[] {
            "Sonic",
            "Eggman (unused)",
            "Tails",
            "Knuckles",
            "Tikal (unused)",
            "Amy",
            "Gamma",
            "Big",
            "Metal Sonic"});
            this.comboBoxTestSpawnCharacter.Location = new System.Drawing.Point(83, 44);
            this.comboBoxTestSpawnCharacter.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.comboBoxTestSpawnCharacter.Name = "comboBoxTestSpawnCharacter";
            this.comboBoxTestSpawnCharacter.Size = new System.Drawing.Size(149, 21);
            this.comboBoxTestSpawnCharacter.TabIndex = 22;
            this.toolTip.SetToolTip(this.comboBoxTestSpawnCharacter, "Start the game with a specific character.");
            // 
            // numericUpDownTestSpawnAngle
            // 
            this.numericUpDownTestSpawnAngle.Enabled = false;
            this.numericUpDownTestSpawnAngle.Location = new System.Drawing.Point(242, 120);
            this.numericUpDownTestSpawnAngle.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.numericUpDownTestSpawnAngle.Maximum = new decimal(new int[] {
            65536,
            0,
            0,
            0});
            this.numericUpDownTestSpawnAngle.Name = "numericUpDownTestSpawnAngle";
            this.numericUpDownTestSpawnAngle.Size = new System.Drawing.Size(80, 20);
            this.numericUpDownTestSpawnAngle.TabIndex = 30;
            this.toolTip.SetToolTip(this.numericUpDownTestSpawnAngle, "Character Y Rotation.");
            // 
            // labelTestSpawnAngle
            // 
            this.labelTestSpawnAngle.AutoSize = true;
            this.labelTestSpawnAngle.Enabled = false;
            this.labelTestSpawnAngle.Location = new System.Drawing.Point(201, 122);
            this.labelTestSpawnAngle.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.labelTestSpawnAngle.Name = "labelTestSpawnAngle";
            this.labelTestSpawnAngle.Size = new System.Drawing.Size(37, 13);
            this.labelTestSpawnAngle.TabIndex = 29;
            this.labelTestSpawnAngle.Text = "Angle:";
            this.toolTip.SetToolTip(this.labelTestSpawnAngle, "Character Y Rotation.");
            // 
            // labelTestSpawnY
            // 
            this.labelTestSpawnY.AutoSize = true;
            this.labelTestSpawnY.Enabled = false;
            this.labelTestSpawnY.Location = new System.Drawing.Point(114, 97);
            this.labelTestSpawnY.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.labelTestSpawnY.Name = "labelTestSpawnY";
            this.labelTestSpawnY.Size = new System.Drawing.Size(17, 13);
            this.labelTestSpawnY.TabIndex = 24;
            this.labelTestSpawnY.Text = "Y:";
            this.toolTip.SetToolTip(this.labelTestSpawnY, "Character Y Position.");
            // 
            // numericUpDownTestSpawnSaveID
            // 
            this.numericUpDownTestSpawnSaveID.Enabled = false;
            this.numericUpDownTestSpawnSaveID.Location = new System.Drawing.Point(121, 175);
            this.numericUpDownTestSpawnSaveID.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.numericUpDownTestSpawnSaveID.Maximum = new decimal(new int[] {
            99,
            0,
            0,
            0});
            this.numericUpDownTestSpawnSaveID.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpDownTestSpawnSaveID.Name = "numericUpDownTestSpawnSaveID";
            this.numericUpDownTestSpawnSaveID.Size = new System.Drawing.Size(51, 20);
            this.numericUpDownTestSpawnSaveID.TabIndex = 17;
            this.toolTip.SetToolTip(this.numericUpDownTestSpawnSaveID, "File ID to load, i.e. 1 is SonicAdventureDX01.snc.");
            this.numericUpDownTestSpawnSaveID.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // checkBoxTestSpawnSave
            // 
            this.checkBoxTestSpawnSave.AutoSize = true;
            this.checkBoxTestSpawnSave.Location = new System.Drawing.Point(6, 177);
            this.checkBoxTestSpawnSave.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.checkBoxTestSpawnSave.Name = "checkBoxTestSpawnSave";
            this.checkBoxTestSpawnSave.Size = new System.Drawing.Size(111, 17);
            this.checkBoxTestSpawnSave.TabIndex = 16;
            this.checkBoxTestSpawnSave.Text = "Save Redirection:";
            this.toolTip.SetToolTip(this.checkBoxTestSpawnSave, "Force the game to load a specific save file.");
            this.checkBoxTestSpawnSave.UseVisualStyleBackColor = true;
            this.checkBoxTestSpawnSave.CheckStateChanged += new System.EventHandler(this.checkBoxTestSpawnSave_CheckStateChanged);
            // 
            // labelTestSpawnX
            // 
            this.labelTestSpawnX.AutoSize = true;
            this.labelTestSpawnX.Enabled = false;
            this.labelTestSpawnX.Location = new System.Drawing.Point(9, 97);
            this.labelTestSpawnX.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.labelTestSpawnX.Name = "labelTestSpawnX";
            this.labelTestSpawnX.Size = new System.Drawing.Size(17, 13);
            this.labelTestSpawnX.TabIndex = 23;
            this.labelTestSpawnX.Text = "X:";
            this.toolTip.SetToolTip(this.labelTestSpawnX, "Character X Position.");
            // 
            // labelTestSpawnAct
            // 
            this.labelTestSpawnAct.AutoSize = true;
            this.labelTestSpawnAct.Enabled = false;
            this.labelTestSpawnAct.Location = new System.Drawing.Point(250, 21);
            this.labelTestSpawnAct.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.labelTestSpawnAct.Name = "labelTestSpawnAct";
            this.labelTestSpawnAct.Size = new System.Drawing.Size(26, 13);
            this.labelTestSpawnAct.TabIndex = 5;
            this.labelTestSpawnAct.Text = "Act:";
            this.toolTip.SetToolTip(this.labelTestSpawnAct, "Act ID (zero-based).");
            // 
            // labelTestSpawnZ
            // 
            this.labelTestSpawnZ.AutoSize = true;
            this.labelTestSpawnZ.Enabled = false;
            this.labelTestSpawnZ.Location = new System.Drawing.Point(221, 97);
            this.labelTestSpawnZ.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.labelTestSpawnZ.Name = "labelTestSpawnZ";
            this.labelTestSpawnZ.Size = new System.Drawing.Size(17, 13);
            this.labelTestSpawnZ.TabIndex = 25;
            this.labelTestSpawnZ.Text = "Z:";
            this.toolTip.SetToolTip(this.labelTestSpawnZ, "Character Z Position.");
            // 
            // numericUpDownTestSpawnAct
            // 
            this.numericUpDownTestSpawnAct.Enabled = false;
            this.numericUpDownTestSpawnAct.Location = new System.Drawing.Point(281, 20);
            this.numericUpDownTestSpawnAct.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.numericUpDownTestSpawnAct.Maximum = new decimal(new int[] {
            6,
            0,
            0,
            0});
            this.numericUpDownTestSpawnAct.Name = "numericUpDownTestSpawnAct";
            this.numericUpDownTestSpawnAct.Size = new System.Drawing.Size(61, 20);
            this.numericUpDownTestSpawnAct.TabIndex = 2;
            this.toolTip.SetToolTip(this.numericUpDownTestSpawnAct, "Act ID (zero-based).");
            // 
            // numericUpDownTestSpawnZ
            // 
            this.numericUpDownTestSpawnZ.Enabled = false;
            this.numericUpDownTestSpawnZ.Location = new System.Drawing.Point(242, 95);
            this.numericUpDownTestSpawnZ.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.numericUpDownTestSpawnZ.Maximum = new decimal(new int[] {
            999999,
            0,
            0,
            0});
            this.numericUpDownTestSpawnZ.Minimum = new decimal(new int[] {
            999999,
            0,
            0,
            -2147483648});
            this.numericUpDownTestSpawnZ.Name = "numericUpDownTestSpawnZ";
            this.numericUpDownTestSpawnZ.Size = new System.Drawing.Size(80, 20);
            this.numericUpDownTestSpawnZ.TabIndex = 28;
            this.toolTip.SetToolTip(this.numericUpDownTestSpawnZ, "Character Z Position.");
            // 
            // numericUpDownTestSpawnY
            // 
            this.numericUpDownTestSpawnY.Enabled = false;
            this.numericUpDownTestSpawnY.Location = new System.Drawing.Point(135, 95);
            this.numericUpDownTestSpawnY.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.numericUpDownTestSpawnY.Maximum = new decimal(new int[] {
            999999,
            0,
            0,
            0});
            this.numericUpDownTestSpawnY.Minimum = new decimal(new int[] {
            999999,
            0,
            0,
            -2147483648});
            this.numericUpDownTestSpawnY.Name = "numericUpDownTestSpawnY";
            this.numericUpDownTestSpawnY.Size = new System.Drawing.Size(80, 20);
            this.numericUpDownTestSpawnY.TabIndex = 27;
            this.toolTip.SetToolTip(this.numericUpDownTestSpawnY, "Character Y Position.");
            // 
            // comboBoxTestSpawnLevel
            // 
            this.comboBoxTestSpawnLevel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTestSpawnLevel.Enabled = false;
            this.comboBoxTestSpawnLevel.FormattingEnabled = true;
            this.comboBoxTestSpawnLevel.Items.AddRange(new object[] {
            "Hedgehog Hammer",
            "Emerald Coast",
            "Windy Valley",
            "Twinkle Park",
            "Speed Highway",
            "Red Mountain",
            "Sky Deck",
            "Lost World",
            "Ice Cap",
            "Casinopolis",
            "Final Egg",
            "Unused (11)",
            "Hot Shelter",
            "Unused (13)",
            "Unused (14)",
            "Chaos 0",
            "Chaos 2",
            "Chaos 4",
            "Chaos 6",
            "Perfect Chaos",
            "Egg Hornet",
            "Egg Walker",
            "Egg Viper",
            "ZERO",
            "E-101",
            "E-101R",
            "Station Square",
            "Station Square Unused (27)",
            "Station Square Unused (28)",
            "Egg Carrier Outside",
            "Unused (30)",
            "Unused (31)",
            "Egg Carrier Inside",
            "Mystic Ruins",
            "Past",
            "Twinkle Circuit",
            "Sky Chase 1",
            "Sky Chase 2",
            "Sand Hill",
            "Station Square Garden",
            "Egg Carrier Garden",
            "Mystic Ruins Garden",
            "Chao Race"});
            this.comboBoxTestSpawnLevel.Location = new System.Drawing.Point(83, 19);
            this.comboBoxTestSpawnLevel.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.comboBoxTestSpawnLevel.Name = "comboBoxTestSpawnLevel";
            this.comboBoxTestSpawnLevel.Size = new System.Drawing.Size(149, 21);
            this.comboBoxTestSpawnLevel.TabIndex = 0;
            this.toolTip.SetToolTip(this.comboBoxTestSpawnLevel, "Start the game on a specific level.");
            // 
            // numericUpDownTestSpawnX
            // 
            this.numericUpDownTestSpawnX.Enabled = false;
            this.numericUpDownTestSpawnX.Location = new System.Drawing.Point(30, 95);
            this.numericUpDownTestSpawnX.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.numericUpDownTestSpawnX.Maximum = new decimal(new int[] {
            999999,
            0,
            0,
            0});
            this.numericUpDownTestSpawnX.Minimum = new decimal(new int[] {
            999999,
            0,
            0,
            -2147483648});
            this.numericUpDownTestSpawnX.Name = "numericUpDownTestSpawnX";
            this.numericUpDownTestSpawnX.Size = new System.Drawing.Size(80, 20);
            this.numericUpDownTestSpawnX.TabIndex = 26;
            this.toolTip.SetToolTip(this.numericUpDownTestSpawnX, "Character X Position.");
            // 
            // modContextMenu
            // 
            this.modContextMenu.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.modContextMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openFolderToolStripMenuItem,
            this.toolStripMenuItem1,
            this.checkForUpdatesToolStripMenuItem,
            this.verifyToolStripMenuItem,
            this.forceUpdateToolStripMenuItem,
            this.uninstallToolStripMenuItem,
            this.toolStripMenuItem3,
            this.developerToolStripMenuItem});
            this.modContextMenu.Name = "modContextMenu";
            this.modContextMenu.Size = new System.Drawing.Size(171, 148);
            // 
            // openFolderToolStripMenuItem
            // 
            this.openFolderToolStripMenuItem.Name = "openFolderToolStripMenuItem";
            this.openFolderToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
            this.openFolderToolStripMenuItem.Text = "Open folder";
            this.openFolderToolStripMenuItem.Click += new System.EventHandler(this.openFolderToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(167, 6);
            // 
            // checkForUpdatesToolStripMenuItem
            // 
            this.checkForUpdatesToolStripMenuItem.Name = "checkForUpdatesToolStripMenuItem";
            this.checkForUpdatesToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
            this.checkForUpdatesToolStripMenuItem.Text = "Check for updates";
            this.checkForUpdatesToolStripMenuItem.Click += new System.EventHandler(this.checkForUpdatesToolStripMenuItem_Click);
            // 
            // verifyToolStripMenuItem
            // 
            this.verifyToolStripMenuItem.Name = "verifyToolStripMenuItem";
            this.verifyToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
            this.verifyToolStripMenuItem.Text = "Verify integrity";
            this.verifyToolStripMenuItem.Click += new System.EventHandler(this.verifyToolStripMenuItem_Click);
            // 
            // forceUpdateToolStripMenuItem
            // 
            this.forceUpdateToolStripMenuItem.Name = "forceUpdateToolStripMenuItem";
            this.forceUpdateToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
            this.forceUpdateToolStripMenuItem.Text = "Force update";
            this.forceUpdateToolStripMenuItem.Click += new System.EventHandler(this.forceUpdateToolStripMenuItem_Click);
            // 
            // uninstallToolStripMenuItem
            // 
            this.uninstallToolStripMenuItem.Name = "uninstallToolStripMenuItem";
            this.uninstallToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
            this.uninstallToolStripMenuItem.Text = "Uninstall";
            this.uninstallToolStripMenuItem.Click += new System.EventHandler(this.uninstallToolStripMenuItem_Click);
            // 
            // toolStripMenuItem3
            // 
            this.toolStripMenuItem3.Name = "toolStripMenuItem3";
            this.toolStripMenuItem3.Size = new System.Drawing.Size(167, 6);
            // 
            // developerToolStripMenuItem
            // 
            this.developerToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.generateManifestToolStripMenuItem});
            this.developerToolStripMenuItem.Name = "developerToolStripMenuItem";
            this.developerToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
            this.developerToolStripMenuItem.Text = "Developer";
            // 
            // generateManifestToolStripMenuItem
            // 
            this.generateManifestToolStripMenuItem.Name = "generateManifestToolStripMenuItem";
            this.generateManifestToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
            this.generateManifestToolStripMenuItem.Text = "Generate manifest";
            this.generateManifestToolStripMenuItem.Click += new System.EventHandler(this.generateManifestToolStripMenuItem_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(408, 462);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.installButton);
            this.Controls.Add(this.saveAndPlayButton);
            this.Controls.Add(this.saveButton);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(422, 495);
            this.Name = "MainForm";
            this.Padding = new System.Windows.Forms.Padding(0, 0, 0, 3);
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Show;
            this.Text = "SADX Mod Manager";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.Shown += new System.EventHandler(this.MainForm_Shown);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.MainForm_KeyDown);
            groupBox2.ResumeLayout(false);
            groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.verticalResolution)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.horizontalResolution)).EndInit();
            this.tabControl1.ResumeLayout(false);
            this.tabPageMods.ResumeLayout(false);
            this.tabPageMods.PerformLayout();
            this.tabPageCodes.ResumeLayout(false);
            this.tabPageGraphics.ResumeLayout(false);
            this.groupBox_WindowMode.ResumeLayout(false);
            this.groupBox_WindowMode.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.windowHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.windowWidth)).EndInit();
            this.groupBox_Visuals.ResumeLayout(false);
            this.groupBox_Visuals.PerformLayout();
            this.groupBox_Display.ResumeLayout(false);
            this.groupBox_Display.PerformLayout();
            this.tabPageInput.ResumeLayout(false);
            this.groupMouseMode.ResumeLayout(false);
            this.groupMouseMode.PerformLayout();
            this.groupBoxController.ResumeLayout(false);
            this.groupBoxController.PerformLayout();
            this.tabPageSound.ResumeLayout(false);
            this.groupBox_SoundAdvanced.ResumeLayout(false);
            this.groupBox_SoundAdvanced.PerformLayout();
            this.groupBox_SoundVolume.ResumeLayout(false);
            this.groupBox_SoundVolume.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarVoiceVol)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarMusicVol)).EndInit();
            this.groupBox_SoundConfig.ResumeLayout(false);
            this.groupBox_SoundConfig.PerformLayout();
            this.tabPageOptions.ResumeLayout(false);
            this.tabPageOptions.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpdateFrequency)).EndInit();
            this.tabPageDebug.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox12.ResumeLayout(false);
            this.groupBox12.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTestSpawnAngle)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTestSpawnSaveID)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTestSpawnAct)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTestSpawnZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTestSpawnY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTestSpawnX)).EndInit();
            this.modContextMenu.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListView modListView;
        private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.Button saveButton;
        private System.Windows.Forms.Button saveAndPlayButton;
		private System.Windows.Forms.Button installButton;
        private System.Windows.Forms.NumericUpDown horizontalResolution;
        private System.Windows.Forms.NumericUpDown verticalResolution;
        private System.Windows.Forms.Label modDescription;
		private System.Windows.Forms.Button buttonRefreshModList;
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage tabPageMods;
		private System.Windows.Forms.TabPage tabPageCodes;
		private System.Windows.Forms.CheckedListBox codesCheckedListBox;
		private System.Windows.Forms.CheckBox windowedFullscreenCheckBox;
		private System.Windows.Forms.Button buttonNewMod;
		private System.Windows.Forms.CheckBox forceAspectRatioCheckBox;
		private System.Windows.Forms.CheckBox stretchFullscreenCheckBox;
		private System.Windows.Forms.ComboBox screenNumComboBox;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		private System.Windows.Forms.TabPage tabPageGraphics;
		private System.Windows.Forms.GroupBox groupBox_Display;
		private System.Windows.Forms.CheckBox forceMipmappingCheckBox;
		private System.Windows.Forms.ToolTip toolTip;
		private System.Windows.Forms.CheckBox forceTextureFilterCheckBox;
		private System.Windows.Forms.CheckBox customWindowSizeCheckBox;
		private System.Windows.Forms.NumericUpDown windowWidth;
		private System.Windows.Forms.NumericUpDown windowHeight;
		private System.Windows.Forms.CheckBox maintainWindowAspectRatioCheckBox;
		private System.Windows.Forms.Button modDownButton;
		private System.Windows.Forms.Button modUpButton;
		private System.Windows.Forms.CheckBox checkVsync;
		private System.Windows.Forms.CheckBox checkScaleHud;
		private System.Windows.Forms.ContextMenuStrip modContextMenu;
		private System.Windows.Forms.ToolStripMenuItem openFolderToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem uninstallToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
		private System.Windows.Forms.ToolStripMenuItem developerToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem generateManifestToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem checkForUpdatesToolStripMenuItem;
		private System.Windows.Forms.TabPage tabPageOptions;
		private System.Windows.Forms.CheckBox pauseWhenInactiveCheckBox;
		private System.Windows.Forms.CheckBox disableCDCheckCheckBox;
		private System.Windows.Forms.GroupBox groupBox4;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.CheckBox checkUpdateModsStartup;
		private System.Windows.Forms.NumericUpDown numericUpdateFrequency;
		private System.Windows.Forms.ComboBox comboUpdateFrequency;
		private System.Windows.Forms.CheckBox checkUpdateStartup;
		private System.Windows.Forms.GroupBox groupBox_Visuals;
		private System.Windows.Forms.Button buttonCheckForUpdates;
		private System.Windows.Forms.ToolStripMenuItem verifyToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem forceUpdateToolStripMenuItem;
		private System.Windows.Forms.CheckBox checkWindowResize;
		private System.Windows.Forms.ComboBox comboResolutionPreset;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.ComboBox comboFmvFill;
		private System.Windows.Forms.ComboBox comboBackgroundFill;
		private System.Windows.Forms.Button installURLHandlerButton;
		private System.Windows.Forms.Button modBottomButton;
		private System.Windows.Forms.Button modTopButton;
		private System.Windows.Forms.Button configureModButton;
		private System.Windows.Forms.ComboBox comboTextLanguage;
		private System.Windows.Forms.ComboBox comboVoiceLanguage;
		private System.Windows.Forms.Label label9;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.GroupBox groupBox_WindowMode;
		private System.Windows.Forms.RadioButton radioWindowMode;
		private System.Windows.Forms.RadioButton radioFullscreen;
		private System.Windows.Forms.ComboBox comboFog;
		private System.Windows.Forms.ComboBox comboClip;
		private System.Windows.Forms.Label label10;
		private System.Windows.Forms.Label label11;
		private System.Windows.Forms.Label label12;
		private System.Windows.Forms.ComboBox comboFramerate;
		private System.Windows.Forms.TabPage tabPageSound;
		private System.Windows.Forms.GroupBox groupBox_SoundVolume;
		private System.Windows.Forms.Label label13;
		private System.Windows.Forms.Label label14;
		private System.Windows.Forms.GroupBox groupBox_SoundConfig;
		private System.Windows.Forms.CheckBox checkMusic;
		private System.Windows.Forms.CheckBox checkSound;
		private System.Windows.Forms.CheckBox check3DSound;
        private System.Windows.Forms.TrackBar trackBarVoiceVol;
        private System.Windows.Forms.TrackBar trackBarMusicVol;
        private System.Windows.Forms.Label labelVoiceVol;
        private System.Windows.Forms.Label labelMusicVol;
        private System.Windows.Forms.TabPage tabPageDebug;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox consoleCheckBox;
        private System.Windows.Forms.CheckBox screenCheckBox;
        private System.Windows.Forms.CheckBox fileCheckBox;
        private System.Windows.Forms.GroupBox groupBox12;
        private System.Windows.Forms.CheckBox checkBoxTestSpawnPosition;
        private System.Windows.Forms.Button buttonTestSpawnPlay;
        private System.Windows.Forms.ComboBox comboBoxTestSpawnEvent;
        private System.Windows.Forms.CheckBox checkBoxTestSpawnEvent;
        private System.Windows.Forms.CheckBox checkBoxTestSpawnCharacter;
        private System.Windows.Forms.CheckBox checkBoxTestSpawnLevel;
        private System.Windows.Forms.ComboBox comboBoxTestSpawnCharacter;
        private System.Windows.Forms.NumericUpDown numericUpDownTestSpawnAngle;
        private System.Windows.Forms.Label labelTestSpawnAngle;
        private System.Windows.Forms.Label labelTestSpawnY;
        private System.Windows.Forms.NumericUpDown numericUpDownTestSpawnSaveID;
        private System.Windows.Forms.CheckBox checkBoxTestSpawnSave;
        private System.Windows.Forms.Label labelTestSpawnX;
        private System.Windows.Forms.Label labelTestSpawnAct;
        private System.Windows.Forms.Label labelTestSpawnZ;
        private System.Windows.Forms.NumericUpDown numericUpDownTestSpawnAct;
        private System.Windows.Forms.NumericUpDown numericUpDownTestSpawnZ;
        private System.Windows.Forms.NumericUpDown numericUpDownTestSpawnY;
        private System.Windows.Forms.ComboBox comboBoxTestSpawnLevel;
        private System.Windows.Forms.NumericUpDown numericUpDownTestSpawnX;
        private System.Windows.Forms.CheckBox checkBoxTestSpawnAngleHex;
		private System.Windows.Forms.GroupBox groupBox_SoundAdvanced;
		private System.Windows.Forms.CheckBox checkBASS;
        private System.Windows.Forms.CheckBox checkBoxEnableD3D9;
        private System.Windows.Forms.Label labelTestSpawnTime;
        private System.Windows.Forms.ComboBox comboBoxTestSpawnTime;
		private System.Windows.Forms.Button buttonUpdateD3D8to9;
		private System.Windows.Forms.Label labelTestSpawnWarning;
		private System.Windows.Forms.GroupBox groupBox3;
		private System.Windows.Forms.ComboBox profileNameBox;
		private System.Windows.Forms.Button buttonSaveProfile;
		private System.Windows.Forms.Button buttonLoadProfile;
		private System.Windows.Forms.CheckBox checkPolyBuff;
		private System.Windows.Forms.TabPage tabPageInput;
		private System.Windows.Forms.TextBox controllerConfigName;
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
		private System.Windows.Forms.Button controllerConfigRemove;
		private System.Windows.Forms.Button controllerConfigAdd;
		private System.Windows.Forms.ComboBox controllerConfigSelect;
		private System.Windows.Forms.GroupBox groupMouseMode;
		private System.Windows.Forms.Label label17;
		private System.Windows.Forms.ComboBox comboMouseButtons;
		private System.Windows.Forms.RadioButton radioMouseModeRelease;
		private System.Windows.Forms.RadioButton radioMouseModeHold;
		private System.Windows.Forms.ComboBox comboMouseActions;
		private System.Windows.Forms.Label label18;
		private System.Windows.Forms.GroupBox groupBoxController;
		private System.Windows.Forms.CheckBox crashLogCheckBox;
	}
}