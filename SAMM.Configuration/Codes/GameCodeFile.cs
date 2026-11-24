using SAMM.Utilities;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Text;
using System.Xml.Serialization;

namespace SAMM.Configuration.Codes
{
	public class GameCodeFile : NotifyObject
	{
		#region Old Info Classes
		/// This region houses all of the old classes from the original Mod Managers that
		/// handled all of the reading/writing for the "cheat codes" that we supply for 
		/// SADX and SA2B. They're retained here for ease in ensuring continued compatilbility.
		/// 
		/// These have been replaced with the <see cref="GameCodeFile"/> and <see cref="GameCode"/>
		/// classes for general use. These private classes should generally not be modified.

		/// <summary>
		/// Copied from System.Windows.Forms. Licensed under MIT from the .NET Foundation. 
		/// </summary>
		[Flags]
		private enum Keys
		{
			/// <summary>
			///  The bit mask to extract a key code from a key value.
			/// </summary>
			KeyCode = 0x0000FFFF,

			/// <summary>
			///  The bit mask to extract modifiers from a key value.
			/// </summary>
			Modifiers = unchecked((int)0xFFFF0000),

			/// <summary>
			///  No key pressed.
			/// </summary>
			None = 0x00,

			/// <summary>
			///  The left mouse button.
			/// </summary>
			LButton = 0x01,

			/// <summary>
			///  The right mouse button.
			/// </summary>
			RButton = 0x02,

			/// <summary>
			///  The CANCEL key.
			/// </summary>
			Cancel = 0x03,

			/// <summary>
			///  The middle mouse button (three-button mouse).
			/// </summary>
			MButton = 0x04,

			/// <summary>
			///  The first x mouse button (five-button mouse).
			/// </summary>
			XButton1 = 0x05,

			/// <summary>
			///  The second x mouse button (five-button mouse).
			/// </summary>
			XButton2 = 0x06,

			/// <summary>
			///  The BACKSPACE key.
			/// </summary>
			Back = 0x08,

			/// <summary>
			///  The TAB key.
			/// </summary>
			Tab = 0x09,

			/// <summary>
			///  The CLEAR key.
			/// </summary>
			LineFeed = 0x0A,

			/// <summary>
			///  The CLEAR key.
			/// </summary>
			Clear = 0x0C,

			/// <summary>
			///  The RETURN key.
			/// </summary>
			Return = 0x0D,

			/// <summary>
			///  The ENTER key.
			/// </summary>
			Enter = Return,

			/// <summary>
			///  The SHIFT key.
			/// </summary>
			ShiftKey = 0x10,

			/// <summary>
			///  The CTRL key.
			/// </summary>
			ControlKey = 0x11,

			/// <summary>
			///  The ALT key.
			/// </summary>
			Menu = 0x12,

			/// <summary>
			///  The PAUSE key.
			/// </summary>
			Pause = 0x13,

			/// <summary>
			///  The CAPS LOCK key.
			/// </summary>
			Capital = 0x14,

			/// <summary>
			///  The CAPS LOCK key.
			/// </summary>
			CapsLock = 0x14,

			/// <summary>
			///  The IME Kana mode key.
			/// </summary>
			KanaMode = 0x15,

			/// <summary>
			///  The IME Hanguel mode key.
			/// </summary>
			HanguelMode = 0x15,

			/// <summary>
			///  The IME Hangul mode key.
			/// </summary>
			HangulMode = 0x15,

			/// <summary>
			///  The IME Junja mode key.
			/// </summary>
			JunjaMode = 0x17,

			/// <summary>
			///  The IME Final mode key.
			/// </summary>
			FinalMode = 0x18,

			/// <summary>
			///  The IME Hanja mode key.
			/// </summary>
			HanjaMode = 0x19,

			/// <summary>
			///  The IME Kanji mode key.
			/// </summary>
			KanjiMode = 0x19,

			/// <summary>
			///  The ESC key.
			/// </summary>
			Escape = 0x1B,

			/// <summary>
			///  The IME Convert key.
			/// </summary>
			IMEConvert = 0x1C,

			/// <summary>
			///  The IME NonConvert key.
			/// </summary>
			IMENonconvert = 0x1D,

			/// <summary>
			///  The IME Accept key.
			/// </summary>
			IMEAccept = 0x1E,

			/// <summary>
			///  The IME Accept key.
			/// </summary>
			IMEAceept = IMEAccept,

			/// <summary>
			///  The IME Mode change request.
			/// </summary>
			IMEModeChange = 0x1F,

			/// <summary>
			///  The SPACEBAR key.
			/// </summary>
			Space = 0x20,

			/// <summary>
			///  The PAGE UP key.
			/// </summary>
			Prior = 0x21,

			/// <summary>
			///  The PAGE UP key.
			/// </summary>
			PageUp = Prior,

			/// <summary>
			///  The PAGE DOWN key.
			/// </summary>
			Next = 0x22,

			/// <summary>
			///  The PAGE DOWN key.
			/// </summary>
			PageDown = Next,

			/// <summary>
			///  The END key.
			/// </summary>
			End = 0x23,

			/// <summary>
			///  The HOME key.
			/// </summary>
			Home = 0x24,

			/// <summary>
			///  The LEFT ARROW key.
			/// </summary>
			Left = 0x25,

			/// <summary>
			///  The UP ARROW key.
			/// </summary>
			Up = 0x26,

			/// <summary>
			///  The RIGHT ARROW key.
			/// </summary>
			Right = 0x27,

			/// <summary>
			///  The DOWN ARROW key.
			/// </summary>
			Down = 0x28,

			/// <summary>
			///  The SELECT key.
			/// </summary>
			Select = 0x29,

			/// <summary>
			///  The PRINT key.
			/// </summary>
			Print = 0x2A,

			/// <summary>
			///  The EXECUTE key.
			/// </summary>
			Execute = 0x2B,

			/// <summary>
			///  The PRINT SCREEN key.
			/// </summary>
			Snapshot = 0x2C,

			/// <summary>
			///  The PRINT SCREEN key.
			/// </summary>
			PrintScreen = Snapshot,

			/// <summary>
			///  The INS key.
			/// </summary>
			Insert = 0x2D,

			/// <summary>
			///  The DEL key.
			/// </summary>
			Delete = 0x2E,

			/// <summary>
			///  The HELP key.
			/// </summary>
			Help = 0x2F,

			/// <summary>
			///  The 0 key.
			/// </summary>
			D0 = 0x30, // 0

			/// <summary>
			///  The 1 key.
			/// </summary>
			D1 = 0x31, // 1

			/// <summary>
			///  The 2 key.
			/// </summary>
			D2 = 0x32, // 2

			/// <summary>
			///  The 3 key.
			/// </summary>
			D3 = 0x33, // 3

			/// <summary>
			///  The 4 key.
			/// </summary>
			D4 = 0x34, // 4

			/// <summary>
			///  The 5 key.
			/// </summary>
			D5 = 0x35, // 5

			/// <summary>
			///  The 6 key.
			/// </summary>
			D6 = 0x36, // 6

			/// <summary>
			///  The 7 key.
			/// </summary>
			D7 = 0x37, // 7

			/// <summary>
			///  The 8 key.
			/// </summary>
			D8 = 0x38, // 8

			/// <summary>
			///  The 9 key.
			/// </summary>
			D9 = 0x39, // 9

			/// <summary>
			///  The A key.
			/// </summary>
			A = 0x41,

			/// <summary>
			///  The B key.
			/// </summary>
			B = 0x42,

			/// <summary>
			///  The C key.
			/// </summary>
			C = 0x43,

			/// <summary>
			///  The D key.
			/// </summary>
			D = 0x44,

			/// <summary>
			///  The E key.
			/// </summary>
			E = 0x45,

			/// <summary>
			///  The F key.
			/// </summary>
			F = 0x46,

			/// <summary>
			///  The G key.
			/// </summary>
			G = 0x47,

			/// <summary>
			///  The H key.
			/// </summary>
			H = 0x48,

			/// <summary>
			///  The I key.
			/// </summary>
			I = 0x49,

			/// <summary>
			///  The J key.
			/// </summary>
			J = 0x4A,

			/// <summary>
			///  The K key.
			/// </summary>
			K = 0x4B,

			/// <summary>
			///  The L key.
			/// </summary>
			L = 0x4C,

			/// <summary>
			///  The M key.
			/// </summary>
			M = 0x4D,

			/// <summary>
			///  The N key.
			/// </summary>
			N = 0x4E,

			/// <summary>
			///  The O key.
			/// </summary>
			O = 0x4F,

			/// <summary>
			///  The P key.
			/// </summary>
			P = 0x50,

			/// <summary>
			///  The Q key.
			/// </summary>
			Q = 0x51,

			/// <summary>
			///  The R key.
			/// </summary>
			R = 0x52,

			/// <summary>
			///  The S key.
			/// </summary>
			S = 0x53,

			/// <summary>
			///  The T key.
			/// </summary>
			T = 0x54,

			/// <summary>
			///  The U key.
			/// </summary>
			U = 0x55,

			/// <summary>
			///  The V key.
			/// </summary>
			V = 0x56,

			/// <summary>
			///  The W key.
			/// </summary>
			W = 0x57,

			/// <summary>
			///  The X key.
			/// </summary>
			X = 0x58,

			/// <summary>
			///  The Y key.
			/// </summary>
			Y = 0x59,

			/// <summary>
			///  The Z key.
			/// </summary>
			Z = 0x5A,

			/// <summary>
			///  The left Windows logo key (Microsoft Natural Keyboard).
			/// </summary>
			LWin = 0x5B,

			/// <summary>
			///  The right Windows logo key (Microsoft Natural Keyboard).
			/// </summary>
			RWin = 0x5C,

			/// <summary>
			///  The Application key (Microsoft Natural Keyboard).
			/// </summary>
			Apps = 0x5D,

			/// <summary>
			///  The Computer Sleep key.
			/// </summary>
			Sleep = 0x5F,

			/// <summary>
			///  The 0 key on the numeric keypad.
			/// </summary>
			NumPad0 = 0x60,

			/// <summary>
			///  The 1 key on the numeric keypad.
			/// </summary>
			NumPad1 = 0x61,

			/// <summary>
			///  The 2 key on the numeric keypad.
			/// </summary>
			NumPad2 = 0x62,

			/// <summary>
			///  The 3 key on the numeric keypad.
			/// </summary>
			NumPad3 = 0x63,

			/// <summary>
			///  The 4 key on the numeric keypad.
			/// </summary>
			NumPad4 = 0x64,

			/// <summary>
			///  The 5 key on the numeric keypad.
			/// </summary>
			NumPad5 = 0x65,

			/// <summary>
			///  The 6 key on the numeric keypad.
			/// </summary>
			NumPad6 = 0x66,

			/// <summary>
			///  The 7 key on the numeric keypad.
			/// </summary>
			NumPad7 = 0x67,

			/// <summary>
			///  The 8 key on the numeric keypad.
			/// </summary>
			NumPad8 = 0x68,

			/// <summary>
			///  The 9 key on the numeric keypad.
			/// </summary>
			NumPad9 = 0x69,

			/// <summary>
			///  The Multiply key.
			/// </summary>
			Multiply = 0x6A,

			/// <summary>
			///  The Add key.
			/// </summary>
			Add = 0x6B,

			/// <summary>
			///  The Separator key.
			/// </summary>
			Separator = 0x6C,

			/// <summary>
			///  The Subtract key.
			/// </summary>
			Subtract = 0x6D,

			/// <summary>
			///  The Decimal key.
			/// </summary>
			Decimal = 0x6E,

			/// <summary>
			///  The Divide key.
			/// </summary>
			Divide = 0x6F,

			/// <summary>
			///  The F1 key.
			/// </summary>
			F1 = 0x70,

			/// <summary>
			///  The F2 key.
			/// </summary>
			F2 = 0x71,

			/// <summary>
			///  The F3 key.
			/// </summary>
			F3 = 0x72,

			/// <summary>
			///  The F4 key.
			/// </summary>
			F4 = 0x73,

			/// <summary>
			///  The F5 key.
			/// </summary>
			F5 = 0x74,

			/// <summary>
			///  The F6 key.
			/// </summary>
			F6 = 0x75,

			/// <summary>
			///  The F7 key.
			/// </summary>
			F7 = 0x76,

			/// <summary>
			///  The F8 key.
			/// </summary>
			F8 = 0x77,

			/// <summary>
			///  The F9 key.
			/// </summary>
			F9 = 0x78,

			/// <summary>
			///  The F10 key.
			/// </summary>
			F10 = 0x79,

			/// <summary>
			///  The F11 key.
			/// </summary>
			F11 = 0x7A,

			/// <summary>
			///  The F12 key.
			/// </summary>
			F12 = 0x7B,

			/// <summary>
			///  The F13 key.
			/// </summary>
			F13 = 0x7C,

			/// <summary>
			///  The F14 key.
			/// </summary>
			F14 = 0x7D,

			/// <summary>
			///  The F15 key.
			/// </summary>
			F15 = 0x7E,

			/// <summary>
			///  The F16 key.
			/// </summary>
			F16 = 0x7F,

			/// <summary>
			///  The F17 key.
			/// </summary>
			F17 = 0x80,

			/// <summary>
			///  The F18 key.
			/// </summary>
			F18 = 0x81,

			/// <summary>
			///  The F19 key.
			/// </summary>
			F19 = 0x82,

			/// <summary>
			///  The F20 key.
			/// </summary>
			F20 = 0x83,

			/// <summary>
			///  The F21 key.
			/// </summary>
			F21 = 0x84,

			/// <summary>
			///  The F22 key.
			/// </summary>
			F22 = 0x85,

			/// <summary>
			///  The F23 key.
			/// </summary>
			F23 = 0x86,

			/// <summary>
			///  The F24 key.
			/// </summary>
			F24 = 0x87,

			/// <summary>
			///  The NUM LOCK key.
			/// </summary>
			NumLock = 0x90,

			/// <summary>
			///  The SCROLL LOCK key.
			/// </summary>
			Scroll = 0x91,

			/// <summary>
			///  The left SHIFT key.
			/// </summary>
			LShiftKey = 0xA0,

			/// <summary>
			///  The right SHIFT key.
			/// </summary>
			RShiftKey = 0xA1,

			/// <summary>
			///  The left CTRL key.
			/// </summary>
			LControlKey = 0xA2,

			/// <summary>
			///  The right CTRL key.
			/// </summary>
			RControlKey = 0xA3,

			/// <summary>
			///  The left ALT key.
			/// </summary>
			LMenu = 0xA4,

			/// <summary>
			///  The right ALT key.
			/// </summary>
			RMenu = 0xA5,

			/// <summary>
			///  The Browser Back key.
			/// </summary>
			BrowserBack = 0xA6,

			/// <summary>
			///  The Browser Forward key.
			/// </summary>
			BrowserForward = 0xA7,

			/// <summary>
			///  The Browser Refresh key.
			/// </summary>
			BrowserRefresh = 0xA8,

			/// <summary>
			///  The Browser Stop key.
			/// </summary>
			BrowserStop = 0xA9,

			/// <summary>
			///  The Browser Search key.
			/// </summary>
			BrowserSearch = 0xAA,

			/// <summary>
			///  The Browser Favorites key.
			/// </summary>
			BrowserFavorites = 0xAB,

			/// <summary>
			///  The Browser Home key.
			/// </summary>
			BrowserHome = 0xAC,

			/// <summary>
			///  The Volume Mute key.
			/// </summary>
			VolumeMute = 0xAD,

			/// <summary>
			///  The Volume Down key.
			/// </summary>
			VolumeDown = 0xAE,

			/// <summary>
			///  The Volume Up key.
			/// </summary>
			VolumeUp = 0xAF,

			/// <summary>
			///  The Media Next Track key.
			/// </summary>
			MediaNextTrack = 0xB0,

			/// <summary>
			///  The Media Previous Track key.
			/// </summary>
			MediaPreviousTrack = 0xB1,

			/// <summary>
			///  The Media Stop key.
			/// </summary>
			MediaStop = 0xB2,

			/// <summary>
			///  The Media Play Pause key.
			/// </summary>
			MediaPlayPause = 0xB3,

			/// <summary>
			///  The Launch Mail key.
			/// </summary>
			LaunchMail = 0xB4,

			/// <summary>
			///  The Select Media key.
			/// </summary>
			SelectMedia = 0xB5,

			/// <summary>
			///  The Launch Application1 key.
			/// </summary>
			LaunchApplication1 = 0xB6,

			/// <summary>
			///  The Launch Application2 key.
			/// </summary>
			LaunchApplication2 = 0xB7,

			/// <summary>
			///  The Oem Semicolon key.
			/// </summary>
			OemSemicolon = 0xBA,

			/// <summary>
			///  The Oem 1 key.
			/// </summary>
			Oem1 = OemSemicolon,

			/// <summary>
			///  The Oem plus key.
			/// </summary>
			Oemplus = 0xBB,

			/// <summary>
			///  The Oem comma key.
			/// </summary>
			Oemcomma = 0xBC,

			/// <summary>
			///  The Oem Minus key.
			/// </summary>
			OemMinus = 0xBD,

			/// <summary>
			///  The Oem Period key.
			/// </summary>
			OemPeriod = 0xBE,

			/// <summary>
			///  The Oem Question key.
			/// </summary>
			OemQuestion = 0xBF,

			/// <summary>
			///  The Oem 2 key.
			/// </summary>
			Oem2 = OemQuestion,

			/// <summary>
			///  The Oem 3 key.
			/// </summary>
			Oem3 = 0xC0,

			/// <summary>
			///  The Oem tilde key.
			/// </summary>
			Oemtilde = Oem3,

			/// <summary>
			///  The Oem Open Brackets key.
			/// </summary>
			OemOpenBrackets = 0xDB,

			/// <summary>
			///  The Oem 4 key.
			/// </summary>
			Oem4 = OemOpenBrackets,

			/// <summary>
			///  The Oem Pipe key.
			/// </summary>
			OemPipe = 0xDC,

			/// <summary>
			///  The Oem 5 key.
			/// </summary>
			Oem5 = OemPipe,

			/// <summary>
			///  The Oem Close Brackets key.
			/// </summary>
			OemCloseBrackets = 0xDD,

			/// <summary>
			///  The Oem 6 key.
			/// </summary>
			Oem6 = OemCloseBrackets,

			/// <summary>
			///  The Oem 7 key.
			/// </summary>
			Oem7 = 0xDE,

			/// <summary>
			///  The Oem Quotes key.
			/// </summary>
			OemQuotes = Oem7,

			/// <summary>
			///  The Oem8 key.
			/// </summary>
			Oem8 = 0xDF,

			/// <summary>
			///  The Oem 102 key.
			/// </summary>
			Oem102 = 0xE2,

			/// <summary>
			///  The Oem Backslash key.
			/// </summary>
			OemBackslash = Oem102,

			/// <summary>
			///  The PROCESS KEY key.
			/// </summary>
			ProcessKey = 0xE5,

			/// <summary>
			///  The Packet KEY key.
			/// </summary>
			Packet = 0xE7,

			/// <summary>
			///  The ATTN key.
			/// </summary>
			Attn = 0xF6,

			/// <summary>
			///  The CRSEL key.
			/// </summary>
			Crsel = 0xF7,

			/// <summary>
			///  The EXSEL key.
			/// </summary>
			Exsel = 0xF8,

			/// <summary>
			///  The ERASE EOF key.
			/// </summary>
			EraseEof = 0xF9,

			/// <summary>
			///  The PLAY key.
			/// </summary>
			Play = 0xFA,

			/// <summary>
			///  The ZOOM key.
			/// </summary>
			Zoom = 0xFB,

			/// <summary>
			///  A constant reserved for future use.
			/// </summary>
			NoName = 0xFC,

			/// <summary>
			///  The PA1 key.
			/// </summary>
			Pa1 = 0xFD,

			/// <summary>
			///  The CLEAR key.
			/// </summary>
			OemClear = 0xFE,

			/// <summary>
			///  The SHIFT modifier key.
			/// </summary>
			Shift = 0x00010000,

			/// <summary>
			///  The  CTRL modifier key.
			/// </summary>
			Control = 0x00020000,

			/// <summary>
			///  The ALT modifier key.
			/// </summary>
			Alt = 0x00040000,
		}

		private enum CodeType
		{
			write8, write16, write32, writefloat,
			add8, add16, add32, addfloat,
			sub8, sub16, sub32, subfloat,
			mulu8, mulu16, mulu32, mulfloat,
			muls8, muls16, muls32,
			divu8, divu16, divu32, divfloat,
			divs8, divs16, divs32,
			modu8, modu16, modu32,
			mods8, mods16, mods32,
			shl8, shl16, shl32,
			shru8, shru16, shru32,
			shrs8, shrs16, shrs32,
			rol8, rol16, rol32,
			ror8, ror16, ror32,
			and8, and16, and32,
			or8, or16, or32,
			xor8, xor16, xor32,
			writenop,
			writejump,
			writecall,
			writeoff,
			ifeq8, ifeq16, ifeq32, ifeqfloat,
			ifne8, ifne16, ifne32, ifnefloat,
			ifltu8, ifltu16, ifltu32, ifltfloat,
			iflts8, iflts16, iflts32,
			ifltequ8, ifltequ16, ifltequ32, iflteqfloat,
			iflteqs8, iflteqs16, iflteqs32,
			ifgtu8, ifgtu16, ifgtu32, ifgtfloat,
			ifgts8, ifgts16, ifgts32,
			ifgtequ8, ifgtequ16, ifgtequ32, ifgteqfloat,
			ifgteqs8, ifgteqs16, ifgteqs32,
			ifmask8, ifmask16, ifmask32,
			ifkbkey,
			readreg8, readreg16, readreg32,
			writereg8, writereg16, writereg32,
			addreg8, addreg16, addreg32, addregfloat,
			subreg8, subreg16, subreg32, subregfloat,
			mulregu8, mulregu16, mulregu32, mulregfloat,
			mulregs8, mulregs16, mulregs32,
			divregu8, divregu16, divregu32, divregfloat,
			divregs8, divregs16, divregs32,
			modregu8, modregu16, modregu32,
			modregs8, modregs16, modregs32,
			shlreg8, shlreg16, shlreg32,
			shrregu8, shrregu16, shrregu32,
			shrregs8, shrregs16, shrregs32,
			rolreg8, rolreg16, rolreg32,
			rorreg8, rorreg16, rorreg32,
			andreg8, andreg16, andreg32,
			orreg8, orreg16, orreg32,
			xorreg8, xorreg16, xorreg32,
			writenopreg,
			ifeqreg8, ifeqreg16, ifeqreg32, ifeqregfloat,
			ifnereg8, ifnereg16, ifnereg32, ifneregfloat,
			ifltregu8, ifltregu16, ifltregu32, ifltregfloat,
			ifltregs8, ifltregs16, ifltregs32,
			iflteqregu8, iflteqregu16, iflteqregu32, iflteqregfloat,
			iflteqregs8, iflteqregs16, iflteqregs32,
			ifgtregu8, ifgtregu16, ifgtregu32, ifgtregfloat,
			ifgtregs8, ifgtregs16, ifgtregs32,
			ifgteqregu8, ifgteqregu16, ifgteqregu32, ifgteqregfloat,
			ifgteqregs8, ifgteqregs16, ifgteqregs32,
			ifmaskreg8, ifmaskreg16, ifmaskreg32,
			s8tos32, s16tos32, s32tofloat, u32tofloat, floattos32, floattou32,
			@else,
			endif,
			newregs
		}

		private enum ValueType
		{
			@decimal,
			hex
		}

		private class CodeLine
		{
			public CodeType Type { get; set; }
			[XmlElement(IsNullable = false)]
			public string Address { get; set; }
			public bool Pointer { get; set; }
			[XmlIgnore]
			public bool PointerSpecified { get { return Pointer; } set { } }
			[XmlIgnore]
			public List<int> Offsets { get; set; }
			[XmlArray("Offsets")]
			[XmlArrayItem("Offset")]
			public string[] OffsetStrings
			{
				get { return Offsets?.Select((a) => a.ToString("X")).ToArray(); }
				set { Offsets = value.Select((a) => int.Parse(a, System.Globalization.NumberStyles.HexNumber)).ToList(); }
			}
			[XmlIgnore]
			public bool OffsetStringsSpecified { get { return Offsets != null && Offsets.Count > 0; } set { } }
			[XmlElement(IsNullable = false)]
			public string Value { get; set; }
			public ValueType? ValueType { get; set; }
			public uint? RepeatCount { get; set; }
			[XmlIgnore]
			public bool RepeatCountSpecified { get { return RepeatCount.HasValue; } set { } }
			[XmlArray]
			public List<CodeLine> TrueLines { get; set; } = new List<CodeLine>();
			[XmlIgnore]
			public bool TrueLinesSpecified { get { return TrueLines.Count > 0 && IsIf; } set { } }
			[XmlArray]
			public List<CodeLine> FalseLines { get; set; } = new List<CodeLine>();
			[XmlIgnore]
			public bool FalseLinesSpecified { get { return FalseLines.Count > 0 && IsIf; } set { } }

			[XmlIgnore]
			public bool IsFloat
			{
				get
				{
					switch (Type)
					{
						case CodeType.writefloat:
						case CodeType.addfloat:
						case CodeType.subfloat:
						case CodeType.mulfloat:
						case CodeType.divfloat:
						case CodeType.ifeqfloat:
						case CodeType.ifnefloat:
						case CodeType.ifltfloat:
						case CodeType.iflteqfloat:
						case CodeType.ifgtfloat:
						case CodeType.ifgteqfloat:
						case CodeType.addregfloat:
						case CodeType.subregfloat:
						case CodeType.mulregfloat:
						case CodeType.divregfloat:
						case CodeType.ifeqregfloat:
						case CodeType.ifneregfloat:
						case CodeType.ifltregfloat:
						case CodeType.iflteqregfloat:
						case CodeType.ifgtregfloat:
						case CodeType.ifgteqregfloat:
							return true;
						default:
							return false;
					}
				}
			}

			[XmlIgnore]
			public bool IsIf
			{
				get
				{
					return Type >= CodeType.ifeq8 && Type <= CodeType.ifkbkey
						|| Type >= CodeType.ifeqreg8 && Type <= CodeType.ifmaskreg32;
				}
			}

			[XmlIgnore]
			public bool IsReg
			{
				get
				{
					if (IsIf)
					{
						if (TrueLines.Any((line) => line.IsReg))
							return true;
						if (FalseLines.Any((line) => line.IsReg))
							return true;
					}
					if (Address.StartsWith("r"))
						return true;
					if (Type >= CodeType.readreg8 && Type <= CodeType.ifmaskreg32)
						return true;
					return false;
				}
			}

			public string ToString()
			{
				StringBuilder sb = new StringBuilder();

				sb.Append(Type.ToString());
				sb.Append(" ");
				if (PointerSpecified)
					sb.Append("p");
				sb.Append(Address);
				if (OffsetStringsSpecified)
				{
					foreach (string s in OffsetStrings)
					{
						sb.Append($"|{s}");
					}
				}
				sb.Append(" ");
				sb.Append(Value);
				if (IsIf)
				{
					if (TrueLinesSpecified)
					{
						foreach (CodeLine line in TrueLines)
						{
							sb.Append($"\n{line.ToString()}");
						}
						if (FalseLinesSpecified)
							sb.Append("\nelse");
					}

					if (FalseLinesSpecified)
					{
						foreach (CodeLine line in FalseLines)
						{
							sb.Append($"\n{line.ToString()}");
						}
					}
					sb.Append("\nendif");
				}

				return sb.ToString();
			}

			public static void ProcessCodeLine(string filename, int linenum, string[] split, Code code)
			{
				var sb = new StringBuilder(split[1].TrimStart('"'));
				int i = 2;
				if (!split[1].EndsWith("\""))
					for (; i < split.Length; i++)
					{
						sb.AppendFormat(" {0}", split[i]);
						if (split[i].EndsWith("\"")) { ++i; break; }
					}
				code.Name = sb.ToString().TrimEnd('"');
				if (i < split.Length)
					for (; i < split.Length; i++)
					{
						if (split[i].StartsWith(";")) break;
						switch (split[i])
						{
							case "Required":
								code.Required = true;
								break;
							default:
								throw new Exception($"Unknown attribute {split[i]} in code \"{code.Name}\" in {filename}:line {linenum}");
						}
					}
			}
		}

		private class Code
		{
			[XmlAttribute("name")]
			public string Name { get; set; }
			[XmlAttribute("author")]
			public string Author { get; set; }
			[XmlAttribute("category")]
			public string Category { get; set; }
			[XmlAttribute("description")]
			public string Description { get; set; }
			[XmlAttribute("required")]
			public bool Required { get; set; }
			[XmlAttribute("patch")]
			public bool Patch { get; set; }
			[XmlElement("CodeLine")]
			public List<CodeLine> Lines { get; set; } = new List<CodeLine>();

			[XmlIgnore]
			public bool IsReg { get { return Lines.Any((line) => line.IsReg); } }

			public void ParseCodeLine(ref Stack<Tuple<List<CodeLine>, List<CodeLine>>> stack, string[] split, string line, int lineNum)
			{
				if (Enum.TryParse(split[0], out CodeType type))
				{
					switch (type)
					{
						case CodeType.@else:
							if (stack.Peek().Item2 == null)
								throw new FormatException($"Invalid code line \"{line}\" in {Name}:line {lineNum}");
							stack.Push(new Tuple<List<CodeLine>, List<CodeLine>>(stack.Pop().Item2, null));
							return;
						case CodeType.endif:
							if (stack.Count < 2)
								throw new FormatException($"Invalid code line \"{line}\" in {Name} :line  {lineNum}");
							stack.Pop();
							return;
						case CodeType.newregs:
							throw new FormatException($"Invalid code line \"{line}\" in {Name}:line {lineNum}");
						default:
							break;
					}
					CodeLine cl = new CodeLine() { Type = type };
					string address = split[1];
					if (address.StartsWith("p"))
					{
						cl.Pointer = true;
						string[] offs = address.Split('|');
						cl.Address = offs[0].Substring(1);
						if (offs.Length > 1)
						{
							cl.Offsets = new List<int>();
							for (int i = 1; i < offs.Length; i++)
								cl.Offsets.Add(int.Parse(offs[i], System.Globalization.NumberStyles.HexNumber));
						}
					}
					else
						cl.Address = address;
					int it = 2;
					switch (type)
					{
						case CodeType.s8tos32:
						case CodeType.s16tos32:
						case CodeType.s32tofloat:
						case CodeType.u32tofloat:
						case CodeType.floattos32:
						case CodeType.floattou32:
							cl.Value = "0";
							break;
						default:
							cl.Value = split[it++];
							break;
					}
					if (it < split.Length && !split[it].StartsWith(";"))
						cl.RepeatCount = uint.Parse(split[it++].Substring(1));
					stack.Peek().Item1.Add(cl);
					if (cl.IsIf)
						stack.Push(new Tuple<List<CodeLine>, List<CodeLine>>(cl.TrueLines, cl.FalseLines));
				}
				else
					throw new FormatException($"Invalid code line \"{line}\" in {Name}:line {lineNum}");
			}

			public Code(List<string> info)
			{
				int lineNum = 0;
				Stack<Tuple<List<CodeLine>, List<CodeLine>>> stack = new();

				foreach (string line in info)
				{	
					string trimmedLine = line.Trim(' ', '\t');

					if (line.Length <= 0 || line.StartsWith(";"))
						continue;

					string[] split = trimmedLine.Split(" ", StringSplitOptions.RemoveEmptyEntries);

					switch (split[0])
					{
						case "Patch":
						case "Code":
							if (split[0] == "Patch")
								Patch = true;

							int nameStart = line.IndexOf('"') + 1;
							int nameEnd = line.IndexOf('"', nameStart);
							Name = line.Substring(nameStart, nameEnd - nameStart);
							if (split[split.Length-1] == "Required")
								Required = true;

							stack.Push(new Tuple<List<CodeLine>, List<CodeLine>>(Lines, null));
							break;
						case "Author":
							int authorStart = line.IndexOf(" ") + 1;
							Author = line.Substring(authorStart);
							break;
						case "Category":
							int categoryStart = line.IndexOf(" ") + 1;
							Category = line.Substring(categoryStart);
							break;
						case "Description":
							int descStart = line.IndexOf(" ") + 1;
							Description = line.Substring(descStart);
							break;
						default:
							ParseCodeLine(ref stack, split, line, lineNum);
							break;
					}

					lineNum++;
				}
			}

			public Code(GameCode code)
			{
				Name = code.NameTag;
				Required = code.Required;
				Author = code.Author;
				Category = code.Category;
				Description = code.Description;
				Patch = code.IsPatch;

				int lineNum = 0;
				Stack<Tuple<List<CodeLine>, List<CodeLine>>> stack = new();
				stack.Push(new Tuple<List<CodeLine>, List<CodeLine>>(Lines, null));
				foreach (string line in code.AssemblyInstructions)
				{
					string[] split = line.Split(" ", StringSplitOptions.RemoveEmptyEntries);

					ParseCodeLine(ref stack, split, line, lineNum);
					lineNum++;
				}
			}

			public GameCode Convert()
			{
				GameCode code = new GameCode()
				{
					NameTag = Name,
					DisplayName = Name,
					Author = Author,
					Category = Category,
					Description = Description,
					Required = Required,
					IsPatch = Patch
				};

				foreach (CodeLine line in Lines)
					code.AssemblyInstructions.AddRange(line.ToString().Split("\n"));

				return code;
			}
		}

		#endregion

		private ObservableCollection<GameCode> codes = new ObservableCollection<GameCode>();
		/// <summary>
		/// List of <see cref="GameCode"/>s.
		/// </summary>
		public ObservableCollection<GameCode> Codes
		{
			get => codes;
			set => RaiseAndSetIfChanged(ref codes, value);
		}

		private void WriteJsonFile(string filepath)
		{
			string jsonfile = Path.Combine(Path.GetDirectoryName(filepath), "Codes.json");
			JSON.Serialize(jsonfile, this);
		}

		private void WriteCodes(List<CodeLine> lines, BinaryWriter bw)
		{
			foreach (CodeLine line in lines)
			{
				bw.Write((byte)line.Type);
				uint address;
				if (line.Address.StartsWith("r"))
					address = uint.Parse(line.Address.Substring(1), System.Globalization.NumberStyles.None, System.Globalization.NumberFormatInfo.InvariantInfo);
				else
					address = uint.Parse(line.Address, System.Globalization.NumberStyles.HexNumber);
				if (line.Pointer)
					address |= 0x80000000u;
				bw.Write(address);
				if (line.Pointer)
					if (line.Offsets != null)
					{
						bw.Write((byte)line.Offsets.Count);
						foreach (int off in line.Offsets)
							bw.Write(off);
					}
					else
						bw.Write((byte)0);
				if (line.Type == CodeType.ifkbkey)
					bw.Write((int)(Keys)Enum.Parse(typeof(Keys), line.Value));
				else
					switch (line.ValueType)
					{
						case null:
							if (line.Value.StartsWith("0x"))
								bw.Write(uint.Parse(line.Value.Substring(2), System.Globalization.NumberStyles.HexNumber, System.Globalization.NumberFormatInfo.InvariantInfo));
							else if (line.IsFloat)
								bw.Write(float.Parse(line.Value, System.Globalization.NumberStyles.Float, System.Globalization.NumberFormatInfo.InvariantInfo));
							else
								bw.Write(unchecked((int)long.Parse(line.Value, System.Globalization.NumberStyles.Integer, System.Globalization.NumberFormatInfo.InvariantInfo)));
							break;
						case ValueType.@decimal:
							if (line.IsFloat)
								bw.Write(float.Parse(line.Value, System.Globalization.NumberStyles.Float, System.Globalization.NumberFormatInfo.InvariantInfo));
							else
								bw.Write(unchecked((int)long.Parse(line.Value, System.Globalization.NumberStyles.Integer, System.Globalization.NumberFormatInfo.InvariantInfo)));
							break;
						case ValueType.hex:
							bw.Write(uint.Parse(line.Value, System.Globalization.NumberStyles.HexNumber, System.Globalization.NumberFormatInfo.InvariantInfo));
							break;
					}
				bw.Write(line.RepeatCount ?? 1);
				if (line.IsIf)
				{
					WriteCodes(line.TrueLines, bw);
					if (line.FalseLines.Count > 0)
					{
						bw.Write((byte)CodeType.@else);
						WriteCodes(line.FalseLines, bw);
					}
					bw.Write((byte)CodeType.endif);
				}
			}
		}

		public void WriteCodeData(BinaryWriter writer)
		{
			foreach (GameCode code in Codes)
			{
				Code writeCode = new Code(code);
				if (writeCode.IsReg)
					writer.Write((byte)CodeType.newregs);
				WriteCodes(writeCode.Lines, writer);
			}
		}

		public GameCodeFile() { }

		private static GameCodeFile ReadFromJson(string filepath)
		{
			if (Path.Exists(filepath))
				return JSON.Deserialize<GameCodeFile>(filepath);

			return new GameCodeFile();
		}

		private static GameCodeFile ReadFromList(string filepath)
		{
			GameCodeFile codeFile = new GameCodeFile();

			if (Path.Exists(filepath))
			{
				List<List<string>> codeEntries = new List<List<string>>();
				List<string> codeEntry = new List<string>();

				foreach (string line in File.ReadLines(filepath))
				{
					if (string.IsNullOrEmpty(line))
					{
						if (codeEntry.Count > 0)
						{
							codeEntries.Add(codeEntry);
							codeEntry = new List<string>();
						}
					}
					else
						codeEntry.Add(line);
				}

				if (codeEntry.Count > 0)
					codeEntries.Add(codeEntry);

				foreach (List<string> entry in codeEntries)
				{
					Code code = new Code(entry);
					codeFile.Codes.Add(code.Convert());
				}
			}

			return codeFile;
		}

		public static GameCodeFile FromFile(string filepath)
		{
			string ext = Path.GetExtension(filepath);
			GameCodeFile codeFile = new GameCodeFile();
			bool notJson = false;

			switch (ext)
			{
				case ".json":
					codeFile = ReadFromJson(filepath);
					List<Code> codes = new List<Code>();
					foreach (GameCode code in codeFile.Codes)
					{
						Code c = new Code(code);
						codes.Add(c);
					}
					break;
				case ".lst":
					codeFile = ReadFromList(filepath);
					notJson = true;
					break;
				default:
					Logger.Log($"{filepath} is an invalid file for a codes file.");
					break;
			}

			if (notJson)
				codeFile.WriteJsonFile(filepath);

			return codeFile;
		}
	}
}
