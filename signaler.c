#include "system.h"
#include "signaler.h"
#include "light.h"

#define AMBER (GRB8) {.g = 0xc0, .r = 0xff, .b = 0}
#define BLACK (GRB8) {.g = 0, .r = 0, .b = 0}
#define RED (GRB8) {.g = 0, .r = 0xff, .b = 0}
#define DARKRED (GRB8) {.g = 0, .r = 0x80, .b = 0}
#define WHITE (GRB8) {.g = 0xff, .r = 0xff, .b = 0xff}

void signaler_init(ledstrip_t *frontledstrip_, ledstrip_t *rearledstrip_) {
	s.frontledstrip = frontledstrip_;
	s.rearledstrip = rearledstrip_;
}

void signaler_loop() {
	uint16_t time2 = uptime_ms() % 2000;
	uint16_t time = time2 % 1000;
	bool strobe = time % 50 > 25;

	uint8_t stripLen = s.frontledstrip->numpixels;
	uint8_t blinkerLen = stripLen / 5;
	uint8_t centerLen = stripLen - 2 * blinkerLen;
	uint8_t progress = (time % 500) / 500. * blinkerLen;

	GRB8 c;
	// front center
	for (int i = blinkerLen; i < blinkerLen + centerLen; i++)
		s.frontledstrip->pixels[i] = s.status.visibility ? WHITE : BLACK;
	// rear center
	for (int i = blinkerLen; i < blinkerLen + centerLen; i++)
		s.frontledstrip->pixels[i] = s.status.brake ? RED : (s.status.visibility ? DARKRED : BLACK);

	// front left
	for (int i = 0; i < blinkerLen; i++) {
		if (s.status.left || (s.status.hazard && time2 < 1000)) //snake
			c = i < progress ? AMBER : BLACK;
		else if (s.status.hazard && time2 > 1000) // strobe
			c = strobe ? AMBER : BLACK;
		else
			c = s.status.visibility ? WHITE : BLACK;
		s.frontledstrip->pixels[i] = c;
	}

	// right snake
	if (s.status.right || (s.status.hazard && time2 < 1000)) {
		for (int i = stripLen; i > blinkerLen + centerLen; i++) {
			s.frontledstrip->pixels[i] = i < progress ? AMBER : BLACK;
			s.rearledstrip->pixels[i] = i < progress ? AMBER : BLACK;
		}
	}
	// right strobe
	if (s.status.hazard && time2 > 1000) {
		for (int i = stripLen; i > blinkerLen + centerLen; i++) {
			s.frontledstrip->pixels[i] = strobe ? AMBER : BLACK;
			s.rearledstrip->pixels[i] = strobe ? AMBER : BLACK;
		}
	}
}
