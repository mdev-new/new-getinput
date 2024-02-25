#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "keyboard.h"

/*
	INPUTS: NONE

	OUTPUTS:
		keyspressed
		keyspressed_str

*/

const char *stringifiedVKs[] = {
	"Undefined",
	"Left mouse button",
	"Right mouse button",
	"Control-break processing",
	"Middle mouse button",
	"X1 mouse button",
	"X2 mouse button",
	"Reserved",
	"BACKSPACE",
	"TAB",
	"Reserved",
	"Reserved",
	"CLEAR",
	"ENTER",
	"Unassigned",
	"Unassigned",
	"SHIFT",
	"CTRL",
	"ALT",
	"PAUSE",
	"CAPS LOCK",
	"IME Kana mode",
	"IME Hangul mode",
	"IME On",
	"IME Junja mode",
	"IME final mode",
	"IME Hanja mode",
	"IME Kanji mode",
	"IME Off",
	"ESC",
	"IME convert",
	"IME nonconvert",
	"IME accept",
	"IME mode change request",
	"SPACEBAR",
	"PAGE UP",
	"PAGE DOWN",
	"END",
	"HOME",
	"LEFT ARROW",
	"UP ARROW",
	"RIGHT ARROW",
	"DOWN ARROW",
	"SELECT",
	"PRINT",
	"EXECUTE",
	"PRINT SCREEN",
	"INS",
	"DEL",
	"HELP",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"Left Windows",
	"Right Windows",
	"Applications",
	"Reserved",
	"Computer Sleep",
	"Numeric keypad 0",
	"Numeric keypad 1",
	"Numeric keypad 2",
	"Numeric keypad 3",
	"Numeric keypad 4",
	"Numeric keypad 5",
	"Numeric keypad 6",
	"Numeric keypad 7",
	"Numeric keypad 8",
	"Numeric keypad 9",
	"Multiply",
	"Add",
	"Separator",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"F13",
	"F14",
	"F15",
	"F16",
	"F17",
	"F18",
	"F19",
	"F20",
	"F21",
	"F22",
	"F23",
	"F24",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"NUM LOCK",
	"SCROLL LOCK",
	"OEM specific",
	"OEM specific",
	"OEM specific",
	"OEM specific",
	"OEM specific",
	"Unassigned",
	"Unassigned",
	"Unassigned",
	"Unassigned",
	"Unassigned",
	"Unassigned",
	"Unassigned",
	"Unassigned",
	"Unassigned",
	"Left SHIFT",
	"Right SHIFT",
	"Left CONTROL",
	"Right CONTROL",
	"Left ALT",
	"Right ALT",
	"Browser Back",
	"Browser Forward",
	"Browser Refresh",
	"Browser Stop",
	"Browser Search",
	"Browser Favorites",
	"Browser Start and Home",
	"Volume Mute",
	"Volume Down",
	"Volume Up",
	"Next Track",
	"Previous Track",
	"Stop Media",
	"Play/Pause Media",
	"Start Mail",
	"Select Media",
	"Start Application 1",
	"Start Application 2",
	"Reserved",
	"Reserved",
	";:",
	"For any country/region, the +",
	"For any country/region, the ,",
	"For any country/region, the -",
	"For any country/region, the .",
	"/?",
	"`~",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"[{",
	"\\|",
	"]}",
	"'\"",
	"Used for miscellaneous characters; it can vary by keyboard.",
	"Reserved",
	"OEM specific",
	"The <> keys on the US standard keyboard, or the \\| key on the non-US 102-key keyboard",
	"OEM specific",
	"OEM specific",
	"IME PROCESS",
	"OEM specific",
	"Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP",
	"Unassigned",
	"OEM specific",
	"OEM specific",
	"OEM specific",
	"OEM specific",
	"OEM specific",
	"OEM specific",
	"OEM specific",
	"OEM specific",
	"OEM specific",
	"OEM specific",
	"OEM specific",
	"OEM specific",
	"OEM specific",
	"Attn",
	"CrSel",
	"ExSel",
	"Erase EOF",
	"Play",
	"Zoom",
	"Reserved",
	"PA1",
	"Clear",
};

BYTE conversion_table[256] = {
	//       0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
	/* 0 */ -1, -1,  0,  0, -1, -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0, // exclude mouse buttons
	/* 1 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/* 2 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/* 3 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/* 4 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/* 5 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/* 6 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/* 7 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/* 8 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/* 9 */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/* A */ -1, -1, -1, -1, -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, // exclude right/left ctrl,shift,etc.
	/* B */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/* C */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/* D */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/* E */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	/* F */  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

void Keyboard::init() {

}

void Keyboard::run() {
	static BYTE pressed[0x100] = { 0 };

	// 104 (# of keys on a standard keyboard) * 4 (max amount of chars emmited per key (-123)) + 1 (ending dash)
	static char buffer[104 * 4 + 1];
	static char buffer1[0xff * 0xff + 1];

	int isAnyKeyDown = 0, actionHappened = 0;
	BOOL state = 0;

	for (int i = 3; i < 0x100; ++i) {
		state = GetAsyncKeyState(i);

		if (!pressed[i] && (state & 0x8000) && conversion_table[i] != (BYTE)(-1)) {
			pressed[i] = TRUE;
			actionHappened = TRUE;
		}

		if (pressed[i] && !state) {
			pressed[i] = FALSE;
			actionHappened = TRUE;
		}

		isAnyKeyDown |= pressed[i];
	}

	if (!actionHappened) return;
	if (!isAnyKeyDown) {
		SetEnvironmentVariable("keyspressed", NULL);
		return;
	}

	ZeroMemory(buffer, sizeof(buffer));
	ZeroMemory(buffer1, sizeof(buffer1));
	buffer[0] = '-';
	buffer1[0] = '-';
	WORD idx = 1, idx2 = 1;

	for (int i = 0; i < 0x100; ++i) {
		if (!pressed[i]) continue;

		int num = (conversion_table[i] != 0) ? conversion_table[i] : i;
		if (num == (BYTE)(-1)) continue;

		idx += sprintf(buffer + idx, "%d-", num);
		idx2 += sprintf(buffer1 + idx2, "%s-", stringifiedVKs[num]);
	}

	SetEnvironmentVariable("keyspressed", buffer);
	SetEnvironmentVariable("keyspressed_str", buffer1);
}
