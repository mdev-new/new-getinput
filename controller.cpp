#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <xinput.h>

#include "controller.h"

/*
	INPUTS:
		ctrl_deadzone

	OUTPUTS:
		ctrl1_ltrig
		ctrl1_rtrig
		ctrl1_lthumbx
		ctrl1_lthumby
		ctrl1_rthumbx
		ctrl1_rthumby
		ctrl1_btns
		ctrl2_ltrig
		ctrl2_rtrig
		ctrl2_lthumbx
		ctrl2_lthumby
		ctrl2_rthumbx
		ctrl2_rthumby
		ctrl2_btns
		ctrl3_ltrig
		ctrl3_rtrig
		ctrl3_lthumbx
		ctrl3_lthumby
		ctrl3_rthumbx
		ctrl3_rthumby
		ctrl3_btns
		ctrl4_ltrig
		ctrl4_rtrig
		ctrl4_lthumbx
		ctrl4_lthumby
		ctrl4_rthumbx
		ctrl4_rthumby
		ctrl4_btns

*/

struct controller_value {
	unsigned short bitmask;
	const char* str;
};

struct vec2i {
	int x, y;
};

const char* ControllerEnvNames[] = {
	"ctrl1_ltrig",
	"ctrl1_rtrig",
	"ctrl1_lthumbx",
	"ctrl1_lthumby",
	"ctrl1_rthumbx",
	"ctrl1_rthumby",
	"ctrl2_ltrig",
	"ctrl2_rtrig",
	"ctrl2_lthumbx",
	"ctrl2_lthumby",
	"ctrl2_rthumbx",
	"ctrl2_rthumby",
	"ctrl3_ltrig",
	"ctrl3_rtrig",
	"ctrl3_lthumbx",
	"ctrl3_lthumby",
	"ctrl3_rthumbx",
	"ctrl3_rthumby",
	"ctrl4_ltrig",
	"ctrl4_rtrig",
	"ctrl4_lthumbx",
	"ctrl4_lthumby",
	"ctrl4_rthumbx",
	"ctrl4_rthumby"
};

const char* ControllerBtnEnvNames[] = {
	"ctrl1_btns",
	"ctrl2_btns",
	"ctrl3_btns",
	"ctrl4_btns"
};

controller_value ctrl_values[] = {
	{ 0x0001, "dpad_up" },
	{ 0x0002, "dpad_down" },
	{ 0x0004, "dpad_left" },
	{ 0x0008, "dpad_right" },
	{ 0x0010, "start" },
	{ 0x0020, "back" },
	{ 0x0040, "lthumb" },
	{ 0x0080, "rthumb" },
	{ 0x0100, "lshouldr" },
	{ 0x0200, "rshouldr" },
	{ 0x1000, "btn_a" },
	{ 0x2000, "btn_b" },
	{ 0x4000, "btn_x" },
	{ 0x8000, "btn_y" }
};

inline vec2i process_stick(vec2i axes, short deadzone) {
	const int deadzone_squared = deadzone * deadzone;

	if (axes.x * axes.x < deadzone_squared) {
		axes.x = 0;
	}

	if (axes.y * axes.y < deadzone_squared) {
		axes.y = 0;
	}

	return axes;
}

void Controller::init() {
	Controller::deadzone = (int)((float)(getenvnum_ex("ctrl_deadzone", 24)) / 100.f * (float)(0x7FFF));
}

void Controller::run() {
	static char buffer[256] = { 0 };
	static char varName[16] = "controller";

	XINPUT_STATE state;
	DWORD dwResult, size = 0;

	for (char i = 0; i < 4; i++) {
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		dwResult = XInputGetState(i, &state);
		 
		if (dwResult == ERROR_SUCCESS) { /* controller is connected */
			ZeroMemory(buffer, sizeof buffer);

			vec2i left_stick = process_stick({ state.Gamepad.sThumbLX, state.Gamepad.sThumbLY }, Controller::deadzone);
			vec2i right_stick = process_stick({ state.Gamepad.sThumbRX, state.Gamepad.sThumbRY }, Controller::deadzone);

			SetEnvironmentVariable(ControllerEnvNames[i * 6 + 0], itoa_(state.Gamepad.bLeftTrigger));
			SetEnvironmentVariable(ControllerEnvNames[i * 6 + 1], itoa_(state.Gamepad.bRightTrigger));
			SetEnvironmentVariable(ControllerEnvNames[i * 6 + 2], itoa_(left_stick.x));
			SetEnvironmentVariable(ControllerEnvNames[i * 6 + 3], itoa_(left_stick.y));
			SetEnvironmentVariable(ControllerEnvNames[i * 6 + 4], itoa_(right_stick.x));
			SetEnvironmentVariable(ControllerEnvNames[i * 6 + 5], itoa_(right_stick.y));

			int result = 0;
			for (WORD var = 0; var < 14; var++) {
				if (result = (state.Gamepad.wButtons & ctrl_values[var].bitmask)) {
					if (result) {
						buffer[size++] = ',';
					}

					sprintf(buffer + size, "%s", ctrl_values[var].str);
				}
			}

			SetEnvironmentVariable(ControllerBtnEnvNames[i], buffer);
		} else {
			SetEnvironmentVariable(varName, NULL);
		}
	}
}
