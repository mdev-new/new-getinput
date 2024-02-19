#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "keyboard.h"

const char *stringifiedVKs[] = {
	"Undefined",
	"Left mouse button",
	"Right mouse button",
	"Control-break processing",
	"Middle mouse button",
	"X1 mouse button",
	"X2 mouse button",
	"Reserved",
	"BACKSPACE key",
	"TAB key",
	"Reserved",
	"Reserved",
	"CLEAR key",
	"ENTER key",
	"Unassigned",
	"Unassigned",
	"SHIFT key",
	"CTRL key",
	"ALT key",
	"PAUSE key",
	"CAPS LOCK key",
	"IME Kana mode",
	"IME Hangul mode",
	"IME On",
	"IME Junja mode",
	"IME final mode",
	"IME Hanja mode",
	"IME Kanji mode",
	"IME Off",
	"ESC key",
	"IME convert",
	"IME nonconvert",
	"IME accept",
	"IME mode change request",
	"SPACEBAR",
	"PAGE UP key",
	"PAGE DOWN key",
	"END key",
	"HOME key",
	"LEFT ARROW key",
	"UP ARROW key",
	"RIGHT ARROW key",
	"DOWN ARROW key",
	"SELECT key",
	"PRINT key",
	"EXECUTE key",
	"PRINT SCREEN key",
	"INS key",
	"DEL key",
	"HELP key",
	"0 key",
	"1 key",
	"2 key",
	"3 key",
	"4 key",
	"5 key",
	"6 key",
	"7 key",
	"8 key",
	"9 key",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"Undefined",
	"A key",
	"B key",
	"C key",
	"D key",
	"E key",
	"F key",
	"G key",
	"H key",
	"I key",
	"J key",
	"K key",
	"L key",
	"M key",
	"N key",
	"O key",
	"P key",
	"Q key",
	"R key",
	"S key",
	"T key",
	"U key",
	"V key",
	"W key",
	"X key",
	"Y key",
	"Z key",
	"Left Windows key",
	"Right Windows key",
	"Applications key",
	"Reserved",
	"Computer Sleep key",
	"Numeric keypad 0 key",
	"Numeric keypad 1 key",
	"Numeric keypad 2 key",
	"Numeric keypad 3 key",
	"Numeric keypad 4 key",
	"Numeric keypad 5 key",
	"Numeric keypad 6 key",
	"Numeric keypad 7 key",
	"Numeric keypad 8 key",
	"Numeric keypad 9 key",
	"Multiply key",
	"Add key",
	"Separator key",
	"Subtract key",
	"Decimal key",
	"Divide key",
	"F1 key",
	"F2 key",
	"F3 key",
	"F4 key",
	"F5 key",
	"F6 key",
	"F7 key",
	"F8 key",
	"F9 key",
	"F10 key",
	"F11 key",
	"F12 key",
	"F13 key",
	"F14 key",
	"F15 key",
	"F16 key",
	"F17 key",
	"F18 key",
	"F19 key",
	"F20 key",
	"F21 key",
	"F22 key",
	"F23 key",
	"F24 key",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"NUM LOCK key",
	"SCROLL LOCK key",
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
	"Left SHIFT key",
	"Right SHIFT key",
	"Left CONTROL key",
	"Right CONTROL key",
	"Left ALT key",
	"Right ALT key",
	"Browser Back key",
	"Browser Forward key",
	"Browser Refresh key",
	"Browser Stop key",
	"Browser Search key",
	"Browser Favorites key",
	"Browser Start and Home key",
	"Volume Mute key",
	"Volume Down key",
	"Volume Up key",
	"Next Track key",
	"Previous Track key",
	"Stop Media key",
	"Play/Pause Media key",
	"Start Mail key",
	"Select Media key",
	"Start Application 1 key",
	"Start Application 2 key",
	"Reserved",
	"Reserved",
	"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ;: key",
	"For any country/region, the + key",
	"For any country/region, the , key",
	"For any country/region, the - key",
	"For any country/region, the . key",
	"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the /? key",
	"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the `~ key",
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
	"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the [{ key",
	"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the \\| key",
	"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ]} key",
	"Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\" key",
	"Used for miscellaneous characters; it can vary by keyboard.",
	"Reserved",
	"OEM specific",
	"The <> keys on the US standard keyboard, or the \\| key on the non-US 102-key keyboard",
	"OEM specific",
	"OEM specific",
	"IME PROCESS key",
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
	"Attn key",
	"CrSel key",
	"ExSel key",
	"Erase EOF key",
	"Play key",
	"Zoom key",
	"Reserved",
	"PA1 key",
	"Clear key",
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
	WORD idx = 1, idx2 = 1;

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

