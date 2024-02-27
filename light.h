#ifndef light_h
#define light_h
#include <stdbool.h>

#define LIGHT_OFF 0
#define	LIGHT_FADING_IN 1
#define LIGHT_ON 2
#define LIGHT_FADING_OUT 3

typedef struct {
	uint8_t mode;
	double level;
	unsigned long  _fadeStart;
	double _fadeLevel;
} baselight_t;
typedef baselight_t BaseLight;

typedef struct {
	uint8_t mode;
	double level;
	unsigned long  _fadeStart;
	double _fadeLevel;
	double hue;
	double saturation;
} colorlight_t;
typedef colorlight_t ColorLight;

typedef struct {
	uint8_t mode;
	double level;
	unsigned long  _fadeStart;
	double _fadeLevel;
	double temperature;
} whitelight_t;
typedef whitelight_t WhiteLight;

void lightIncreaseLevel(BaseLight *light);
void lightDecreaseLevel(BaseLight *light);
void lightSet(BaseLight *light, bool value);
void lightOn(BaseLight *light);
void lightOff(BaseLight *light);
void lightToggle(BaseLight *light);
void lightFadeIn(BaseLight *light);
void lightFadeOut(BaseLight *light);
void lightFadeToggle(BaseLight *light);
void lightLoop(BaseLight *light);

void lightIncreaseHue(ColorLight *light);
void lightDecreaseHue(ColorLight *light);
void lightIncreaseSaturation(ColorLight *light);
void lightDecreaseSaturation(ColorLight *light);

void lightIncreaseTemperature(WhiteLight *light);
void lightDecreaseTemperature(WhiteLight *light);

typedef struct {
	double r; // ∈ [0, 1]
	double g; // ∈ [0, 1]
	double b; // ∈ [0, 1]
} RGB;

typedef struct {
	uint8_t g;
	uint8_t r;
	uint8_t b;
} GRB8;

RGB hsv2rgb(double hue, double saturation, double level);
GRB8 lightGRB8(ColorLight *light);
GRB8 whitelightGRB8(WhiteLight *light);
RGB lightRGB(ColorLight *light);

typedef struct {
	GRB8 *pixels;
	uint8_t numpixels;
} ledstrip_t;
typedef ledstrip_t LedStrip;

void LedStrip_init(LedStrip *s, uint8_t numpixels);


#endif
