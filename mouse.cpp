#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "mouse.h"
#include "utils.h"

DWORD GETINPUT_SUB CALLBACK ModeThread(void* data) {
	HANDLE hStdIn =  GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	DWORD modeIn, modeOut, inputModeRead, inputModeRead2;

#ifndef WIN2K_BUILD
	modeIn = (ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS) & ~(ENABLE_QUICK_EDIT_MODE);
	modeOut = ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT;
#else
#	ifndef ENABLE_EXTENDED_FLAGS
#		define ENABLE_EXTENDED_FLAGS 0x0080
#	endif

	modeIn = (ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	modeOut = 0;
#endif

	//DWORD textInputMode = modeIn | ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT;

	DWORD partial_cmdInMode = 143; // this (doesnt) seem to work

	while (1) {
		GetConsoleMode(hStdIn, &inputModeRead);
		inputModeRead2 = inputModeRead;
		
		while (inTextInput = ((GetConsoleMode(hStdIn, &inputModeRead), inputModeRead) != inputModeRead2)) Sleep(1);

		SetConsoleMode(hStdIn, modeIn);
		SetConsoleMode(hStdOut, modeOut);

#ifndef WIN2K_BUILD
		usleep(500);
#else
		Sleep(1);
#endif
	}

	return 0;
}

DWORD GETINPUT_SUB CALLBACK MousePosThread(void* data) {
	INPUT_RECORD ir[64];
	DWORD read;

	HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);

	while (1) {
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
		int lmx = getenvnum("limitMouseX"), lmy = getenvnum("limitMouseY");

		int mouseX = record.dwMousePosition.X + 1;
		int mouseY = record.dwMousePosition.Y + 1;
		if (lmx && mouseX > lmx) mouseX = lmx;
		if (lmy && mouseY > lmy) mouseY = lmy;

		ENV("mousexpos", itoa_(mouseX));
		ENV("mouseypos", itoa_(mouseY));
		break;
	}

	case MOUSE_WHEELED:
		ENV("wheeldelta", itoa_((signed short)(HIWORD(record.dwButtonState)) / WHEEL_DELTA));
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

}

void Mouse::run() {

}

