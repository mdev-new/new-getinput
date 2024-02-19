#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "misc_functionality.h"

HHOOK keyboardLowLevelHook = NULL;

//https://stackoverflow.com/questions/7009080/detecting-full-screen-mode-in-windows
bool isFullscreen(HWND windowHandle)
{
	MONITORINFO monitorInfo = { 0 };
	monitorInfo.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(MonitorFromWindow(windowHandle, MONITOR_DEFAULTTOPRIMARY), &monitorInfo);

	RECT windowRect;
	GetWindowRect(windowHandle, &windowRect);

	return windowRect.left == monitorInfo.rcMonitor.left
		&& windowRect.right == monitorInfo.rcMonitor.right
		&& windowRect.top == monitorInfo.rcMonitor.top
		&& windowRect.bottom == monitorInfo.rcMonitor.bottom;
}

void resizeConsoleIfNeeded(int *lastScreenX, int *lastScreenY) {
	int screenx = getenvnum("screenx");
	int screeny = getenvnum("screeny");
	
	int lastX = *lastScreenX;
	int lastY = *lastScreenY;

	if (screenx && screeny && (screenx != lastX) && (screeny != lastY)) {
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

		COORD consoleBufferSize = { screenx, screeny };
		SMALL_RECT windowInfo = { 0, 0, screenx - 1, screeny - 1 };

		SetConsoleWindowInfo(hOut, TRUE, &windowInfo);
		SetConsoleScreenBufferSize(hOut, consoleBufferSize);

		*lastScreenX = screenx;
		*lastScreenY = screeny;
	}

	return;
}

//https://cboard.cprogramming.com/windows-programming/69905-disable-alt-key-commands.html
LRESULT CALLBACK LowLevelKeyboardProc( int nCode, WPARAM wParam, LPARAM lParam )
{
	KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;

    if (nCode == HC_ACTION) {
		if (p->vkCode == VK_RETURN && p->flags & LLKHF_ALTDOWN) return 1; //disable alt-enter
		else if (p->vkCode == VK_F11) return 1;
	}

    return CallNextHookEx(keyboardLowLevelHook, nCode, wParam, lParam);
}

void Misc::init() {
	int noresize = getenvnum_ex("noresize", 0);

	if (noresize) {
		windowStyle &= ~(WS_SIZEBOX | WS_MAXIMIZEBOX);
		SetWindowLong(hCon, GWL_STYLE, windowStyle);
		DrawMenuBar(hCon);

		if (noresize == 2) {
			keyboardLowLevelHook = SetWindowsHookEx(
				WH_KEYBOARD_LL,
				LowLevelKeyboardProc,
				GetModuleHandle(NULL),
				0
			);
		}
	}
}

void Misc::run() {
	if (noresize) {
		SetWindowLong(hCon, GWL_STYLE, windowStyle);
		DrawMenuBar(hCon);

		if (!brutalNoResize && isFullscreen(hCon)) {
			SendMessage(hCon, WM_SYSCOMMAND, SC_RESTORE, 0);

			resizeConsoleIfNeeded(lastscreenx, lastscreeny); // what is this
		}
	}
}

void Misc::unload() {
	if(keyboardLowLevelHook != NULL) {
		UnhookWindowsHookEx(keyboardLowLevelHook);
	}
}
