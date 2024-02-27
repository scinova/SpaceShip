#ifndef button_h
#define button_h

#include <stdint.h>
#include <stdbool.h>

#define SWITCH_CLOSE 1
#define SWITCH_RELEASE 2

#define BUTTON_PRESS 1
#define BUTTON_RELEASE 2
#define BUTTON_HOLD 3
#define BUTTON_RELEASEHOLD 4

#define ENCODER_LEFT 5
#define ENCODER_RIGHT 6

typedef struct {
	bool state;
	unsigned long _stamp;
} Switch;

typedef struct {
	bool state;
	unsigned long _stamp;
	bool _holding;
} Button;

typedef struct {
	bool state;
	unsigned long _stamp;
} Encoder;

uint8_t switch_debounce(Switch *sw, bool value);
uint8_t button_debounce(Button *button, bool value);
uint8_t encoder_debounce(Encoder *encoder, bool clk, bool dir);

#endif
