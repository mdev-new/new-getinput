#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "controller.h"
#include "mouse.h"
#include "keyboard.h"

class Application {
private:
public:
	Application() = delete; // fully static class
	static void init();
	static void run();
	static void unload();
};

void Application::init() {
    Controller::init();
    Mouse::init();
    Keyboard::init();
}

void Application::run() {
    Controller::run();
    Mouse::run();
    Keyboard::run();
}

void Application::unload() {
}

#include "Injector.h"

