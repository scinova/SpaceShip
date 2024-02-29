#include <stdbool.h>
#include <stdint.h>
#include "light.h"

void signaler_init(LedStrip *frontLedStrip, LedStrip *rearLedStrip);
void signal_left(bool enable);
void signal_right(bool enable);
void signal_hazard(bool enable);
void signal_visibility(bool enable);
void signal_lowbeam(bool enable);
void signal_highbeam(bool enable);
void signal_brake(bool enable);
void signal_reverse(bool enable);
void signaler_loop();
