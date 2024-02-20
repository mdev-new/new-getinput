#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "misc_functionality.h"

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
	if (nCode == HC_ACTION) {
		KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;

		if (p->vkCode == VK_RETURN && p->flags & LLKHF_ALTDOWN) return 1; //disable alt-enter
		else if (p->vkCode == VK_F11) return 1;
	}

	return CallNextHookEx(Misc::keyboardLowLevelHook, nCode, wParam, lParam);
}

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

		usleep(500);
	}

	return 0;
}


int noresize = 0;

void Misc::init() {
	noresize = getenvnum_ex("noresize", 0);

	if (noresize) {
		windowStyle &= ~(WS_SIZEBOX | WS_MAXIMIZEBOX);
		SetWindowLong(hCon, GWL_STYLE, windowStyle);
		DrawMenuBar(hCon);

		if (noresize == 2) {
			Misc::keyboardLowLevelHook = SetWindowsHookEx(
				WH_KEYBOARD_LL,
				LowLevelKeyboardProc,
				GetModuleHandle(NULL),
				0
			);
		}

		if(isFullscreen(hCon)) {
			SendMessage(hCon, WM_SYSCOMMAND, SC_RESTORE, 0);
		}
	}

#ifndef WIN2K_BUILD
	short rasterx, rastery;
	bool isRaster;

	CONSOLE_FONT_INFOEX cfi = {
		.cbSize = sizeof(CONSOLE_FONT_INFOEX),
		.nFont = 0,
		.dwFontSize = {0, 0},
		.FontFamily = FF_DONTCARE,
		.FontWeight = FW_NORMAL,
		.FaceName = L"Terminal"
	};

	WORD prevRasterX = -1;
	WORD prevRasterY = -1;

#endif

	HANDLE hModeThread = CreateThread(NULL, 0, ModeThread, NULL, 0, NULL);

}

void Misc::run() {
	static int lastscreenx = -1, lastscreeny = -1;
	static HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	static HANDLE hCon = GetConsoleWindow();

	if (noresize) {
		if (isFullscreen(hCon)) {
			SendMessage(hCon, WM_SYSCOMMAND, SC_RESTORE, 0);
			//resizeConsoleIfNeeded(lastscreenx, lastscreeny); // what is this
		}
	}

#ifndef WIN2K_BUILD
	static int prevRasterX = -1, prevRasterY = -1;
	int rasterx = getenvnum("rasterx");
	int rastery = getenvnum("rastery");
	bool isRaster = rasterx && rastery;

	// lets not set the font every frame
	if (isRaster && (prevRasterX != rasterx || prevRasterY != rastery)) {
		cfi.dwFontSize = { rasterx, rastery };
		SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
		prevRasterX = rasterx;
		prevRasterY = rastery;
	}
#endif
}

void Misc::unload() {
	if(Misc::keyboardLowLevelHook != NULL) {
		UnhookWindowsHookEx(Misc::keyboardLowLevelHook);
	}
}
