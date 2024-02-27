#include <stdint.h>
#include "button.h"
#include "light.h"

typedef struct {
	WhiteLight *light;
	uint8_t mode;
} whitelight_control_t;

//void whitelight_control_init(whitelight_control_t *c, whitelight_t *l);
void whitelight_control_handle(whitelight_control_t *c, uint8_t event);

typedef struct {
	colorlight_t *light;
	uint8_t mode;
} colorlight_control_t;

//void colorlight_control_init(colorlight_control_t *c, colorlight_t *l);
void colorlight_control_handle(colorlight_control_t *c, uint8_t event);

void lightcontrol_init(whitelight_t *cabinlight, colorlight_t *roomlight);
void roomlight_control_handle(uint8_t event);
void cabinlight_control_handle(uint8_t event);
