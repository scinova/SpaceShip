#include "system.h"
#include "light.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h> //malloc

#define FADEIN_DURATION 1000
#define FADEOUT_DURATION 800
#define LEVEL_STEPS 20
#define HUE_STEPS 32
#define SATURATION_STEPS 32
#define TEMPERATURE_STEPS 10

void lightIncreaseLevel(BaseLight *light) {
	if (light->mode == LIGHT_OFF)
		light->mode = LIGHT_ON;
	light->level += 1.0 / LEVEL_STEPS;
	if (light->level > 1)
		light->level = 1;
}

void lightDecreaseLevel(BaseLight *light) {
	light->level -= 1.0 / LEVEL_STEPS;
	if (light->level <= 0) {
		light->level = 0;
		light->mode = LIGHT_OFF;
	}
}

void lightSet(BaseLight *light, bool value) {
	if (value)
		lightOn(light);
	else
		lightOff(light);
}

void lightOn(BaseLight *light) {
	if (light->mode != LIGHT_ON) {
		light->mode = LIGHT_ON;
		light->level = 1;
	}
}

void lightOff(BaseLight *light) {
	if (light->mode != LIGHT_OFF) {
		light->mode = LIGHT_OFF;
		light->level = 0;
	}
}

void lightToggle(BaseLight *light) {
	if (light->mode == LIGHT_OFF || light->mode == LIGHT_FADING_OUT)
		lightOn(light);
	else if (light->mode == LIGHT_ON || light->mode == LIGHT_FADING_IN)
		lightOff(light);
}

void lightFadeIn(BaseLight *light) {
	if (light->mode == LIGHT_OFF || light->mode == LIGHT_FADING_OUT) {
		light->mode = LIGHT_FADING_IN;
		light->_fadeStart = uptime_ms();
		light->_fadeLevel = light->level;
		//loop();
	}
}

void lightFadeOut(BaseLight *light) {
	if (light->mode == LIGHT_ON || light->mode == LIGHT_FADING_IN) {
		light->mode = LIGHT_FADING_OUT;
		light->_fadeStart = uptime_ms();
		light->_fadeLevel = light->level;
	}
}

void lightFadeToggle(BaseLight *light) {
	if (light->mode == LIGHT_OFF || light->mode == LIGHT_FADING_OUT)
		lightFadeIn(light);
	else if (light->mode == LIGHT_ON || light->mode == LIGHT_FADING_IN)
		lightFadeOut(light);
}

void lightLoop(BaseLight *light) {
	switch (light->mode) {
		case LIGHT_FADING_IN:
			light->level = light->_fadeLevel + (1. - light->_fadeLevel) * (uptime_ms() - light->_fadeStart) / (double)FADEIN_DURATION;
			if (light->level >= 1) {
				light->level = 1;
				light->mode = LIGHT_ON;
			}
			break;
		case LIGHT_FADING_OUT:
			light->level = light->_fadeLevel - light->_fadeLevel * (double)(uptime_ms() - light->_fadeStart) / (double)FADEOUT_DURATION;
			if (light->level <= 0) {
				light->level = 0;
				light->mode = LIGHT_OFF;
			}
			break;
		case LIGHT_ON:
			break;
		case LIGHT_OFF:
			break;
	}
}

void lightIncreaseHue(ColorLight *light) {
	light->hue += 1.0 / HUE_STEPS;
	if (light->hue >= 1)
		light->hue -= 1;
}

void lightDecreaseHue(ColorLight *light) {
	light->hue -= 1.0 / HUE_STEPS;
	if (light->hue < 0)
		light->hue += 1;
}

void lightIncreaseSaturation(ColorLight *light) {
	light->saturation += 1.0 / SATURATION_STEPS;
	if (light->saturation > 1)
		light->saturation = 1;
}

void lightDecreaseSaturation(ColorLight *light) {
	light->saturation -= 1.0 / SATURATION_STEPS;
	if (light->saturation < 0)
		light->saturation = 0;
}

void lightIncreaseTemperature(WhiteLight *light) {
	light->temperature += 1.0 / TEMPERATURE_STEPS;
	if (light->temperature > 1)
		light->temperature = 1;
}

void lightDecreaseTemperature(WhiteLight *light) {
	light->temperature -= 1.0 / TEMPERATURE_STEPS;
	if (light->temperature < 0)
		light->temperature = 0;
}

RGB hsv2rgb(double h, double s, double v) {
	double r, g, b;
	double p, q, t, fract;
	h = h * 360.;
	(h == 360.) ? (h = 0.) : (h /= 60.);
	fract = h - floor(h);
	p = v * (1. - s);
	q = v * (1. - s * fract);
	t = v * (1. - s * (1. - fract));
	if (0 <= h && h < 1) {
		r = v; g = t; b = p;
	} else if (1 <= h && h < 2) {
		r = q; g = v; b = p;
	} else if (2 <= h && h < 3) {
		r = p; g = v; b = t;
	} else if (3 <= h && h < 4) {
		r = p; g = q; b = v;
	} else if (4 <= h && h < 5) {
		r = t; g = p; b = v;
	} else if (5 <= h && h < 6) {
		r = v; g = p; b = q;
	} else {
		r = 0; g = 0; b = 0;
	}
	return (RGB) {r, g, b};
}

RGB lightRGB(ColorLight *light) {
	return hsv2rgb(light->hue, light->saturation, light->level);
}

GRB8 lightGRB8(ColorLight *light) {
	RGB c = lightRGB(light);
	return (GRB8) {255 * c.g, 255 * c.r, 255 * c.b};
}

/*
RGB ColorLight::rgb() {
	return hsv2rgb(_hue, _saturation, _level);
}


int correct(double value) {
	return int(pow(pow(255, 0.33) * ((float)(value * 100) / (float)100), 1 / 0.33) + 0.5);
}

const uint8_t PROGMEM gamma[] = {
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
		1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
		2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
		5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
		10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
		17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
		25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
		37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
		51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
		69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
		90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
		115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
		144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
		177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
		215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255
	};*/


//RGB rgb = hsv2rgb(_hue, _saturation, _level);
//uint8_t r = pgm_read_byte(&gamma[(int)(rgb.r * 255.)]);
//uint8_t g = pgm_read_byte(&gamma[(int)(rgb.g * 255.)]);
//uint8_t b = pgm_read_byte(&gamma[(int)(rgb.b * 255.)]);

void LedStrip_init(LedStrip *s, uint8_t numpixels) {
	s->numpixels = numpixels;
	s->pixels = malloc(3 * numpixels);
};

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

GRB8 whitelightGRB8(WhiteLight *light) {
	float red = 255;
	float green = 255;
	float blue = 255;
	int temperature = 7000 / 100;

	if (temperature > 66) {
		red = temperature - 60;
	    red = 329.698727466 * pow(red, -0.1332047592);
	}
	if (temperature < 65) {
		if (temperature <= 19) {
			blue = 0;
		} else {
			blue = temperature - 10;
			blue = (138.5177312231 * log(blue)) - 305.0447927307;
		}
	}
	if (temperature <= 66){
		green = temperature;
		green = (99.4708025861 * log(green)) - 161.1195681661;
	} else {
		green = temperature - 60;
		green = 288.1221695283 * pow(green, -0.0755148492);
	}

	red = constrain(red, 0, 255);
	blue = constrain(blue, 0, 255);
	green = constrain(green, 0, 255);
	return (GRB8) {green, blue, red};
}
