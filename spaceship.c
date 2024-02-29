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

Button left_turn_button;
Button right_turn_button;
Button hazard_button;
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
	event = button_debounce(&left_turn_button, PORTF&1);
	signal_left(event);
	event = button_debounce(&right_turn_button, PORTF&2);
	signal_right(event);
	event = button_debounce(&hazard_button, PORTF&4);
	signal_hazard(event);
	switch_debounce(&visibility_light_switch, PORTF&8);
	switch_debounce(&lowbeam_light_switch, PORTK&16);
	switch_debounce(&highbeam_light_switch, PORTF&32);
	switch_debounce(&horn_switch, PORTF&64);
	switch_debounce(&brake_switch, PORTF&128);
	switch_debounce(&reverse_switch, PORTK&128);
	signal_visibility(visibility_light_switch.state);
	signal_lowbeam(lowbeam_light_switch.state);
	signal_highbeam(highbeam_light_switch.state);
	signal_brake(brake_switch.state);
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
