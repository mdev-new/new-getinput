#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <timeapi.h>

#include "utils.h"
#include "config.h"

// todo
// compiler errors
// intextinput
// proper error checking & handling
// maybe hook into conhost to actually disable resize & maximize in wndproc

class Application {
private:
public:
	Application() = delete; // fully static class
	static void init();
	static void run();
	static void unload();
};

void Application::init() {

    SetEnvironmentVariable("getinputInitialized", "0");

#ifdef ENABLE_CONTROLLER
    Controller::init();
#endif

#ifdef ENABLE_MOUSE
    Mouse::init();
#endif

#ifdef ENABLE_KEYBOARD
    Keyboard::init();
#endif

#ifdef ENABLE_MISC
    Misc::init();
#endif

    SetEnvironmentVariable("getinputInitialized", "1");

}

// Win32 thread entry point
DWORD CALLBACK Application::run(void *data) {

    timeBeginPeriod(1);

	unsigned long long begin, took;
    bool inFocus = true;

    int sleep_time = 1000 / getenvnum_ex("getinput_tps", 40);

    while(true) {
        begin = GetTickCount64();

        inFocus = GetForegroundWindow() == hCon;

        if(inFocus) {

        #ifdef ENABLE_CONTROLLER
            Controller::run();
        #endif

        #ifdef ENABLE_MOUSE
            Mouse::run();
        #endif

        #ifdef ENABLE_KEYBOARD
            Keyboard::run();
        #endif

        #ifdef ENABLE_MISC
            Misc::run();
        #endif

        }

		took = GetTickCount64() - begin;
		Sleep(_max(0, sleep_time - took));
    }
}

void Application::unload() {

#ifdef ENABLE_MISC
    // Unload the low level keyboard hook, if it exists
    Misc::unload();
#endif

    timeEndPeriod();

}

#include "injector.h"
