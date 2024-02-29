#include "system.h"
#include "button.h"

#define DEBOUNCE_DURATION 20
#define HOLD_DURATION 1000

uint8_t switch_debounce(Switch *sw, bool value) {
	unsigned long now = uptime_ms();
	if (!sw->state && value && now - sw->_stamp > DEBOUNCE_DURATION) {
		sw->state = true;
		sw->_stamp = now;
		return SWITCH_CLOSED;
	}
	if (sw->state && !value && now - sw->_stamp > DEBOUNCE_DURATION) {
		sw->state = false;
		sw->_stamp = now;
		return SWITCH_RELEASED;
	}
	return 0;
}

uint8_t button_debounce(Button *button, bool value) {
	unsigned long now = uptime_ms();
	if (!button->state && value && now - button->_stamp > DEBOUNCE_DURATION) {
		button->state = true;
		button->_stamp = now;
		return BUTTON_PRESSED;
	}
	if (button->state && value && !button->_holding && now - button->_stamp > HOLD_DURATION) {
		button->_holding = true;
		return BUTTON_HOLD;
	}
	if (button->state && !value && now - button->_stamp > DEBOUNCE_DURATION) {
		button->state = false;
		button->_stamp = now;
		if (button->_holding) {
			button->_holding = false;
			return BUTTON_RELEASEDHOLD;
		}
		return BUTTON_RELEASED;
	}
	return 0;
}

uint8_t encoder_debounce(Encoder *enc, bool clk, bool dir) {
	unsigned long now = uptime_ms();
	if (!enc->state && clk) {
		enc->state = true;
		enc->_stamp = now;
		if (dir)
			return ENCODER_LEFT;
		else
			return ENCODER_RIGHT;
	}
	if (enc->state && !clk && now - enc->_stamp > DEBOUNCE_DURATION) {
		enc->state = false;
		enc->_stamp = now;
	}
	return 0;
}
