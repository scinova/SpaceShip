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

	// read knobs
	uint8_t event;
	if ((event = button_debounce(&button1, PORTK&1)))
		cabinlight_control_handle(event);
	if ((event = encoder_debounce(&encoder1, PORTK&2, PORTK&4)))
		cabinlight_control_handle(event);
	if ((event = button_debounce(&button2, PORTK&8)))
		roomlight_control_handle(event);
	if ((event = encoder_debounce(&encoder2, PORTK&16, PORTK&32)))
		roomlight_control_handle(event);

	// read switches
	switch_debounce(&left_turn_switch, PORTF&1);
	switch_debounce(&right_turn_switch, PORTF&2);
	switch_debounce(&hazard_switch, PORTF&4);
	switch_debounce(&visibility_light_switch, PORTF&8);
	switch_debounce(&lowbeam_light_switch, PORTK&16);
	switch_debounce(&highbeam_light_switch, PORTF&32);
	switch_debounce(&horn_switch, PORTF&64);
	switch_debounce(&brake_switch, PORTF&128);
	switch_debounce(&reverse_switch, PORTK&128);
	signal_left(left_turn_switch.state);
	signal_right(right_turn_switch.state);
	signal_hazard(hazard_switch.state);
	signal_visibility(visibility_light_switch.state);
	signal_lowbeam(lowbeam_light_switch.state);
	signal_highbeam(highbeam_light_switch.state);
	signal_brake(hazard_switch.state);
	signal_reverse(reverse_switch.state);
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
