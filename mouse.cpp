#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "mouse.h"
#include "utils.h"

/*
	INPUTS: 
		limitMouseX
		limitMouseY

	OUTPUTS:
		click
		wheeldelta
		mousexpos
		mouseypos

*/

// todo intextinput

DWORD GETINPUT_SUB CALLBACK MousePosThread(void* data) {
	INPUT_RECORD ir[64];
	DWORD read;

	HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);

	while (true) {
		ReadConsoleInput(hStdIn, ir, 64, &read);

		for (int i = 0; i < read; i++) {
			processEvnt(ir[i]);
		}
	}

	return 0;
}

void MouseEventProc(MOUSE_EVENT_RECORD& record) {
	if (!inFocus) return;

	switch (record.dwEventFlags) {
	case MOUSE_MOVED: {
		int lmx = getenvnum("limitMouseX");
		int lmy = getenvnum("limitMouseY");

		int mouseX = record.dwMousePosition.X + 1;
		int mouseY = record.dwMousePosition.Y + 1;
		if (lmx && mouseX > lmx) mouseX = lmx;
		if (lmy && mouseY > lmy) mouseY = lmy;

		SetEnvironmentVariable("mousexpos", itoa_(mouseX));
		SetEnvironmentVariable("mouseypos", itoa_(mouseY));
		break;
	}

	case MOUSE_WHEELED:
		SetEnvironmentVariable(
			"wheeldelta",
			itoa_((signed short)(HIWORD(record.dwButtonState)) / WHEEL_DELTA)
		);
		break;

	default: break;
	}
}

void processEvnt(INPUT_RECORD ir) {
	switch (ir.EventType) {
	case MOUSE_EVENT: {
		MouseEventProc(ir.Event.MouseEvent);
		break;
	}

	// a terrible hack
	case KEY_EVENT: /*{
		if (inTextInput) {
			DWORD w;
			if (ir.Event.KeyEvent.wVirtualKeyCode == VK_RETURN) {
				inTextInput = false;
				break;
			}
			WriteConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &ir, 1, &w);
		}
		break;
	}*/

	case FOCUS_EVENT: // checked elsehow
	case WINDOW_BUFFER_SIZE_EVENT:
	case MENU_EVENT:
	default:
		break;
	}
}

void Mouse::init() {
	SetEnvironmentVariable("click", "0");
	SetEnvironmentVariable("wheeldelta", "0");
	SetEnvironmentVariable("mousexpos", "0");
	SetEnvironmentVariable("mouseypos", "0");

	HANDLE hReadThread = CreateThread(NULL, 0, MousePosThread, NULL, 0, NULL);
}

void Mouse::run() {
	static BYTE mouseclick = 0;

	mouseclick =
		(GetKeyState(VK_LBUTTON) & 0x80) >> 7 |
		(GetKeyState(VK_RBUTTON) & 0x80) >> 6 |
		(GetKeyState(VK_MBUTTON) & 0x80) >> 5;

	if (mouseclick && GetSystemMetrics(SM_SWAPBUTTON)) {
		mouseclick |= mouseclick & 0b11;
	}

	SetEnvironmentVariable("click", itoa_(mouseclick));
}
