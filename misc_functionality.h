#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class Misc {
private:
public:
	Misc() = delete; // fully static class
	static void init();
	static void run();
	static void unload();
	static HHOOK keyboardLowLevelHook = NULL;
};
