#include "system.h"
#include "signaler.h"
#include "light.h"
#include "button.h"

#define AMBER (GRB8) {.g = 0xc0, .r = 0xff, .b = 0}
#define BLACK (GRB8) {.g = 0, .r = 0, .b = 0}
#define RED (GRB8) {.g = 0, .r = 0xff, .b = 0}
#define DARKRED (GRB8) {.g = 0, .r = 0x80, .b = 0}
#define WHITE (GRB8) {.g = 0xff, .r = 0xff, .b = 0xff}

struct {
	baselight_t *visibilitylight;
	baselight_t *lowbeamlight;
	baselight_t *highbeamlight;
	ledstrip_t *frontledstrip;
	ledstrip_t *rearledstrip;
	uint32_t _stamp;
	struct {
		bool left;
		bool right;
		bool hazard;
		bool visibility;
		bool lowbeam;
		bool highbeam;
		bool brake;
		bool reverse;
	} status;
} signaler;

void signaler_init(ledstrip_t *frontledstrip_, ledstrip_t *rearledstrip_) {
	signaler.frontledstrip = frontledstrip_;
	signaler.rearledstrip = rearledstrip_;
}

void signal_left(uint8_t event) {
	if (signaler.status.hazard)
		return;
#ifdef TURN_BUTTONS
	if (event == BUTTON_PRESSED) {
		if (!signaler.status.left) {
			signaler.status.left = true;
			signaler.status.right = false;
		} else
			signaler.status.left = false;
	}
#else
	if (event == SWITCH_CLOSED) && !signaler.status.left) {
		signaler.status.left = true;
		signaler.status.right = false;
	}
	if (event == SWITCH_RELEASED)
		signaler.status.left = false;
#endif
}

void signal_right(uint8_t event) {
	if (signaler.status.hazard)
		return;
#ifdef TURN_BUTTONS
	if (event == BUTTON_PRESSED) {
		if (!signaler.status.right) {
			signaler.status.right = true;
			signaler.status.left = false;
		} else
			signaler.status.right = false;
	}
#else
	if (event == SWITCH_CLOSED) && !signaler.status.right) {
		signaler.status.right = true;
		signaler.status.left = false;
	}
	if (event == SWITCH_RELEASED)
		signaler.status.right = false;
#endif
}

void signal_hazard(uint8_t event) {
#ifdef HAZARD_BUTTON
	if (event == BUTTON_PRESSED) {
		if (!signaler.status.hazard) {
			signaler.status.hazard = true;
#ifdef TURN_BUTTONS
			signaler.status.right = false;
			signaler.status.left = false;
#endif
		} else
			signaler.status.hazard = false;
	}
#else
	if (event == SWITCH_CLOSED) && !signaler.status.hazard) {
		signaler.status.hazard = true;
		signaler.status.left = false;
		signaler.status.right = false;
	}
	if (event == SWITCH_RELEASED)
		signaler.status.right = false;
#endif
}

void signal_visibility(bool enable) {
	signaler.status.visibility = enable;
}

void signal_lowbeam(bool enable) {
	signaler.status.lowbeam = enable;
}

void signal_highbeam(bool enable) {
	signaler.status.highbeam = enable;
}

void signal_brake(bool enable) {
	signaler.status.brake = enable;
}

void signal_reverse(bool enable) {
	signaler.status.reverse = enable;
}

void signaler_loop() {
	uint16_t time2 = uptime_ms() % 2000;
	uint16_t time = time2 % 1000;
	bool strobe = time % 50 > 25;

	uint8_t stripLen = signaler.frontledstrip->numpixels;
	uint8_t blinkerLen = stripLen / 5;
	uint8_t centerLen = stripLen - 2 * blinkerLen;
	uint8_t progress = (time % 500) / 500. * blinkerLen;

	GRB8 c;
	// front center
	for (int i = blinkerLen; i < blinkerLen + centerLen; i++)
		signaler.frontledstrip->pixels[i] = signaler.status.visibility ? WHITE : BLACK;
	// rear center
	for (int i = blinkerLen; i < blinkerLen + centerLen; i++)
		signaler.frontledstrip->pixels[i] = signaler.status.brake ? RED : (signaler.status.visibility ? DARKRED : BLACK);

	// front left
	for (int i = 0; i < blinkerLen; i++) {
		if (signaler.status.left || (signaler.status.hazard && time2 < 1000)) //snake
			c = i < progress ? AMBER : BLACK;
		else if (signaler.status.hazard && time2 > 1000) // strobe
			c = strobe ? AMBER : BLACK;
		else
			c = signaler.status.visibility ? WHITE : BLACK;
		signaler.frontledstrip->pixels[i] = c;
	}

	// right snake
	if (signaler.status.right || (signaler.status.hazard && time2 < 1000)) {
		for (int i = stripLen; i > blinkerLen + centerLen; i++) {
			signaler.frontledstrip->pixels[i] = i < progress ? AMBER : BLACK;
			signaler.rearledstrip->pixels[i] = i < progress ? AMBER : BLACK;
		}
	}
	// right strobe
	if (signaler.status.hazard && time2 > 1000) {
		for (int i = stripLen; i > blinkerLen + centerLen; i++) {
			signaler.frontledstrip->pixels[i] = strobe ? AMBER : BLACK;
			signaler.rearledstrip->pixels[i] = strobe ? AMBER : BLACK;
		}
	}
}
