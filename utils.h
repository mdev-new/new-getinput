#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string.h>

inline int _max(int a, int b) {
	return ((a > b) ? a : b);
}

inline int _abs(int a) {
	return (a >= 0) ? a : -a;
}

char * itoa_(int i) {
	static char buffer[21] = { 0 };

	char* c = buffer + 19; // buffer[20] must be \0
	int x = _abs(i);

	do {
		*--c = 48 + x % 10;
	} while (x && (x /= 10));

	if (i < 0) *--c = 45;
	return c;
}

long getenvnum(const char* name) {
	static char buffer[32] = { 0 };
	return
		GetEnvironmentVariable(name, buffer, sizeof(buffer))
		? atol(buffer)
		: 0;
}

long getenvnum_ex(const char* name, int default_val) {
	static char buffer[32] = { 0 };
	return
		GetEnvironmentVariable(name, buffer, sizeof(buffer))
		? atol(buffer)
		: default_val;
}

char* readenv(const char* name) {
	static TCHAR buffer[127];
	GetEnvironmentVariable(name, buffer, sizeof buffer);
	return _strdup(buffer); // memory leaks go brr
}

void setenv(char* name, char *value) {
	SetEnvironmentVariable(name, value);
}

void usleep(__int64 usec) {
#ifndef WIN2K_BUILD
	static HANDLE timer = NULL;
	static LARGE_INTEGER ft;

	ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative value indicates relative time

	if (timer == NULL) [[unlikely]] {
		timer = CreateWaitableTimer(NULL, TRUE, NULL);
	}

	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
#else
	// todo: research
	// win2k probably supports waitable timers
	Sleep(usec / 1000);
#endif
}
