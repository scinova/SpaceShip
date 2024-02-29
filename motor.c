#include "motor.h"
#include <stdint.h>
#include <stdbool.h>

struct {
	uint8_t u;
	uint8_t v;
	uint8_t w;
} motor;

uint8_t cwstates[8] 	= {0, 0b001, 0b011, 0b010, 0b110, 0b100, 0b101, 0};
uint8_t ccwstates[8]	= {0, 0b101, 0b100, 0b110, 0b010, 0b011, 0b001, 0};

void drive_motor(float throttle, bool reverse, bool a, bool b, bool c) {
	uint8_t h = a << 2 | b << 1 | c;
	uint8_t out = (reverse) ? ccwstates[h] : cwstates[h];
	bool u = out & 1, v = out & 2, w = out & 4;
	motor.u = u ? 255 * throttle : 0;
	motor.v = v ? 255 * throttle : 0;
	motor.w = w ? 255 * throttle : 0;
}
