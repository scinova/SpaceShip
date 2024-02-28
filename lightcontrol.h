#include <stdint.h>
#include "button.h"
#include "light.h"

void lightcontrol_init(whitelight_t *cabinlight, colorlight_t *roomlight, colorlight_t *outsidelight);
void roomlight_control_handle(uint8_t event);
void cabinlight_control_handle(uint8_t event);
void outsidelight_control_handle(uint8_t event);
