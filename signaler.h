#include <stdbool.h>
#include <stdint.h>
#include "light.h"

#define TURN_BUTTONS true
#define HAZARD_BUTTON true

void signaler_init(LedStrip *frontLedStrip, LedStrip *rearLedStrip);
void signal_left(uint8_t event);
void signal_right(uint8_t event);
void signal_hazard(uint8_t event);
void signal_visibility(bool enable);
void signal_lowbeam(bool enable);
void signal_highbeam(bool enable);
void signal_brake(bool enable);
void signal_reverse(bool enable);
void signaler_loop();
