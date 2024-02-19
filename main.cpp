#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "config.h"

class Application {
private:
public:
	Application() = delete; // fully static class
	static void init();
	static void run();
	static void unload();
};

void Application::init() {

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

}

void Application::run() {

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

void Application::unload() {

#ifdef ENABLE_MISC
    Misc::unload();
#endif

}

#include "Injector.h"
