#include "lightcontrol.h"

#define LEVEL 0
#define HUE 1
#define SATURATION 2
#define TEMPERATURE 1

typedef struct {
	whitelight_t *light;
	uint8_t mode;
} whitelight_control_t;

typedef struct {
	colorlight_t *light;
	uint8_t mode;
} colorlight_control_t;

whitelight_control_t cabinlight_control;
colorlight_control_t roomlight_control;
colorlight_control_t outsidelight_control;

void lightcontrol_init(whitelight_t *cabinlight_, colorlight_t *roomlight_, colorlight_t *outsidelight_) {
	cabinlight_control.light = cabinlight_;
	roomlight_control.light = roomlight_;
	outsidelight_control.light = outsidelight_;
}

void colorlight_control_handle(colorlight_control_t *c, uint8_t e) {
	if (e == BUTTON_HOLD) {
		if (c->mode == LEVEL) {
			c->mode = HUE;
		} else if (c->mode == HUE || c->mode == SATURATION) {
			c->mode = LEVEL;
		}
	} else if (e == BUTTON_RELEASED) {
		if (c->mode == HUE) {
			c->mode = SATURATION;
		} else if (c->mode == SATURATION) {
			c->mode = LEVEL;
		} else if (c->mode == LEVEL) {
			lightFadeToggle((BaseLight *)c->light);
		}
	} else if (e == BUTTON_RELEASEDHOLD) {
	} else if (e == ENCODER_RIGHT) {
		if (c->mode == LEVEL) {
			lightIncreaseLevel((BaseLight *)c->light);
		} else if (c->mode == HUE) {
			lightIncreaseHue(c->light);
		} else if (c->mode == SATURATION) {
			lightIncreaseSaturation(c->light);
		}
	} else if (e == ENCODER_LEFT) {
		if (c->mode == LEVEL) {
			lightDecreaseLevel((BaseLight *)c->light);
		} else if (c->mode == HUE) {
			lightDecreaseHue(c->light);
		} else if (c->mode == SATURATION) {
			lightDecreaseSaturation(c->light);
		}
	}
}

void whitelight_control_handle(whitelight_control_t *c, uint8_t e) {
	if (e == BUTTON_HOLD) {
		if (c->mode == LEVEL) {
			c->mode = TEMPERATURE;
		} else if (c->mode == TEMPERATURE) {
			c->mode = LEVEL;
		}
	} else if (e == BUTTON_RELEASED) {
		if (c->mode == TEMPERATURE) {
			c->mode = LEVEL;
		} else if (c->mode == LEVEL) {
			lightToggle((BaseLight *)c->light);
		}
	} else if (e == BUTTON_RELEASEDHOLD) {
	} else if (e == ENCODER_RIGHT) {
		if (c->mode == LEVEL) {
			lightIncreaseLevel((BaseLight *)c->light);
		} else if (c->mode == TEMPERATURE) {
			lightIncreaseTemperature(c->light);
		}
	} else if (e == ENCODER_LEFT) {
		if (c->mode == LEVEL) {
			lightDecreaseLevel((BaseLight *)c->light);
		} else if (c->mode == TEMPERATURE) {
			lightDecreaseTemperature(c->light);
		}
	}
}

void cabinlight_control_handle(uint8_t event) {
	whitelight_control_handle(&cabinlight_control, event);
}

void roomlight_control_handle(uint8_t event) {
	colorlight_control_handle(&roomlight_control, event);
}

void outsidelight_control_handle(uint8_t event) {
	colorlight_control_handle(&outsidelight_control, event);
}
