#include "system.h"
#include "button.h"
#include "light.h"
#include "lightcontrol.h"
#include "ws2812b.h"
#include "signaler.h"
#include "sensors.h"
#include "pinout.h"

Button button1;
Button button2;
Encoder encoder1;
Encoder encoder2;

Switch left_turn_switch;
Switch right_turn_switch;
Switch hazard_switch;
Switch visibility_light_switch;
Switch lowbeam_light_switch;
Switch highbeam_light_switch;
Switch horn_switch;
Switch brake_switch;
Switch reverse_switch;

whitelight_t cabinlight;
colorlight_t roomlight;
colorlight_t outsidelight;

LedStrip frontLedStrip;
LedStrip rearLedStrip;
LedStrip stamStrip;

//sensors_t sensors;

signal_control_t signaler;

typedef struct {
	bool knob1_switch;
	bool knob1_clk;
	bool knob1_dir;
	bool knob2_switch;
	bool knob2_clk;
	bool knob2_dir;
	bool sig_left;
	bool sig_right;
	bool sig_hazard;
	bool sig_visibility;
	bool sig_lowbeam;
	bool sig_highbeam;
	bool brake;
	bool reverse;
} Switches;

Switches sw;

void setup() {
	//sensors_init();

	lightcontrol_init(&cabinlight, &roomlight, &outsidelight);
	signaler_init(&frontLedStrip, &rearLedStrip);

	LedStrip_init(&frontLedStrip, 30);
	LedStrip_init(&rearLedStrip, 30);
	LedStrip_init(&stamStrip, 12);
}

void loop() {
	// read knobs
	//sensors_read();

	// process
	uint8_t event;
	if ((event = button_debounce(&button1, sw.knob1_switch)))
		cabinlight_control_handle(event);
	if ((event = encoder_debounce(&encoder1, sw.knob1_clk, sw.knob1_dir)))
		cabinlight_control_handle(event);
	if ((event = button_debounce(&button2, sw.knob2_switch)))
		roomlight_control_handle(event);
	if ((event = encoder_debounce(&encoder2, sw.knob2_clk, sw.knob2_dir)))
		roomlight_control_handle(event);

/*	signaler.status.hazard = HAZARD_SWITCH;
	signaler.status.left = true;//TURN_LEFT_SWITCH & !HAZARD_SWITCH;
	signaler.status.right = TURN_RIGHT_SWITCH & !TURN_LEFT_SWITCH & !HAZARD_SWITCH;
	signaler.status.visibility = VISIBILITY_LIGHT_SWITCH;
	signaler.status.lowbeam = LOWBEAM_LIGHT_SWITCH;
	signaler.status.highbeam = HIGHBEAM_LIGHT_SWITCH;
	signaler.status.brake = BRAKE_SWITCH;
	signaler.status.reverse = REVERSE_SWITCH;
*/
	signaler_loop();

	lightLoop((baselight_t *)&roomlight);
	lightLoop((baselight_t *)&cabinlight);

	// hardware output

	RGB c = lightRGB(&roomlight);
	write_pwm0(255 * c.r);
	write_pwm2(255 * c.g);
	write_pwm3(255 * c.b);

	for (int i = 0; i < 12; i++)
		stamStrip.pixels[i] = lightGRB8(&roomlight);
	//ws2812b_writegrb((uint8_t *)stamStrip.pixels, 3 * 12);
	//ws2812b_writegrb((uint8_t *)frontLedStrip.pixels, 3 * 30);

	write_pwm0(uptime_ms() % 1000 > 500 ? 0 : 255);
}

int main() {
	system_init();
	setup();
	while (1) {
		loop();
	}
}
