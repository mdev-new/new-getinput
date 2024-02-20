#if 0
#	define WIN2K_BUILD
#endif

#if 1 && !WIN2K_BUILD
#	define ENABLE_CONTROLLER
#	pragma comment(lib, "XInput9_1_0.lib")
#	include <xinput.h>
#	include "controller.h"
#endif

#if 1
#	define ENABLE_MOUSE
#	include "mouse.h"
#endif

#if 1
#	define ENABLE_KEYBOARD
#	include "keyboard.h"
#endif

#if 1
#	define ENABLE_MISC
#	include "misc_functionality.h"
#endif
