#include <stdbool.h>
#include <stdint.h>
#include "light.h"

//#define STRIP_CENTER_LEN 100
//#define STRIP_BLINKER_LEN 22
//#define STRIP_LEN (2 * STRIP_BLINKER_LEN + STRIP_CENTER_LEN)



typedef struct {
	bool left;
	bool right;
	bool hazard;
	bool visibility;
	bool lowbeam;
	bool highbeam;
	bool brake;
	bool reverse;
} blinker_status_t;

typedef struct {
	baselight_t *visibilitylight;
	baselight_t *lowbeamlight;
	baselight_t *highbeamlight;
	ledstrip_t *frontledstrip;
	ledstrip_t *rearledstrip;
	uint32_t _stamp;
	blinker_status_t status;
} signal_control_t;

signal_control_t s;




typedef struct {
	WhiteLight *visibilityLight;
	WhiteLight *lowbeamLight;
	WhiteLight *highbeamLight;

	LedStrip *frontLedStrip;
	LedStrip *rearLedStrip;
	uint32_t _stamp;
	blinker_status_t status;
} Signaler;

void signaler_init(LedStrip *frontLedStrip, LedStrip *rearLedStrip);
//void Signaler_feed(Signaler *s, bool sw_visibility, bool sw_left, bool sw_right, bool sw_hazard,
//		bool sw_lowbeam, bool sw_highbeam, bool sw_brake, bool sw_reverse);
void signaler_loop();
