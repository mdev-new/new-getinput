#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tlhelp32.h>

#define NOMANGLE extern "C"
#define EXPORT __declspec(dllexport)

DWORD getppid(char *target = NULL) {
	HANDLE hSnapshot;
	PROCESSENTRY32 pe32;
	DWORD ppid = -1, pid = GetCurrentProcessId();

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot != INVALID_HANDLE_VALUE) {
		ZeroMemory(&pe32, sizeof(pe32));
		pe32.dwSize = sizeof(pe32);
		if (Process32First(hSnapshot, &pe32)) {
			do {
				// todo target is untested
				if (target && pe32.th32ProcessID == pid && strcmp(target, pe32.szExeFile)) {
					ppid = pe32.th32ParentProcessID;
					break;
				} else if (!target && pe32.th32ProcessID == pid) {
					ppid = pe32.th32ParentProcessID;
					break;
				}
			} while (Process32Next(hSnapshot, &pe32));
		}
	}

	if (hSnapshot != INVALID_HANDLE_VALUE)
		CloseHandle(hSnapshot);

	return ppid;
}

NOMANGLE EXPORT BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD dwReason, LPVOID lpReserved) {

#ifdef WIN2K_BUILD
	hDll = hInst;
#endif

	// Check if not running inside rundll32
	// since rundll fires both the specified entry point and DllMain
	char name[MAX_PATH];
	GetModuleFileName(NULL, name, sizeof(name));
	int lastTvelveChars = (strlen(name) - 12);
	if(lastTvelveChars >= 0) {
		if(strcmp("rundll32.exe", name + lastTvelveChars) == 0) {
			return TRUE;
		}
	}

	if (dwReason == DLL_PROCESS_ATTACH) {
		Application::init();
		CreateThread(NULL, 0, &Application::run, NULL, 0, NULL);
	} else if(dwReason == DLL_PROCESS_DETACH) {
		Application::unload();
	}

	return TRUE;
}

NOMANGLE EXPORT void CALLBACK start(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow) {
	char filename[MAX_PATH];

#ifdef WIN2K_BUILD
	while (hDll == NULL);
	Sleep(100); //100ms delay
#else
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCSTR)&getppid,
		&hDll
	);
#endif

	if (!GetModuleFileName(hDll, filename, MAX_PATH)) return;

	int pid = getppid("cmd.exe");
	if(pid != -1) {
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid);

		int filename_len = strlen(filename);

		LPVOID lpBaseAddress = VirtualAllocEx(hProcess, NULL, filename_len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		WriteProcessMemory(hProcess, lpBaseAddress, filename, filename_len, NULL);

		void *loadLibrary = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

		LPTHREAD_START_ROUTINE startAddr = (LPTHREAD_START_ROUTINE)loadLibrary;
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, startAddr, lpBaseAddress, 0, NULL);
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
		CloseHandle(hProcess);
	}
	return;
}

NOMANGLE EXPORT void CALLBACK printver(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow) {
	MessageBox(NULL, VERSION, "Version", 0);
}
