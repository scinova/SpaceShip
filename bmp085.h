#ifndef bmp085_h
#define bmp085_h

#include "i2c.h"

#define BMP085_ADDRESS 0x77

enum {
	BMP085_REGISTER_AC1			= 0xAA,
	BMP085_REGISTER_AC2			= 0xAC,
	BMP085_REGISTER_AC3			= 0xAE,
	BMP085_REGISTER_AC4			= 0xB0,
	BMP085_REGISTER_AC5			= 0xB2,
	BMP085_REGISTER_AC6			= 0xB4,
	BMP085_REGISTER_B1			= 0xB6,
	BMP085_REGISTER_B2			= 0xB8,
	BMP085_REGISTER_MB			= 0xBA,
	BMP085_REGISTER_MC			= 0xBC,
	BMP085_REGISTER_MD			= 0xBE,
	BMP085_REGISTER_CHIPID		= 0xD0,
	BMP085_REGISTER_VERSION		= 0xD1,
	BMP085_REGISTER_SOFTRESET	= 0xE0,
	BMP085_REGISTER_CONTROL		= 0xF4,
	BMP085_REGISTER_OUT			= 0xF6,
	BMP085_REGISTER_OUT_H		= 0xF6,
 	BMP085_REGISTER_OUT_L		= 0xF7,
	BMP085_REGISTER_OUT_XL		= 0xF8,
};

typedef enum {
	BMP085_CMD_MEASURE_TEMPERATURE	= 0b00101110,
	BMP085_CMD_MEASURE_PRESSURE		= 0b00110100,
	BMP085_CMD_MEASURE_PRESSURE2	= 0b01110100,
	BMP085_CMD_MEASURE_PRESSURE4	= 0b10110100,
	BMP085_CMD_MEASURE_PRESSURE8	= 0b11110100
} bmp085_measurement_t;

typedef enum {
	BMP085_MODE_ULTRALOWPOWER = 0,
	BMP085_MODE_STANDARD = 1,
	BMP085_MODE_HIGHRES = 2,
	BMP085_MODE_ULTRAHIGHRES = 3
} bmp085_mode_t;

typedef struct {
	int16_t ac1;
	int16_t ac2;
	int16_t ac3;
	uint16_t ac4;
	uint16_t ac5;
	uint16_t ac6;
	int16_t b1;
	int16_t b2;
	int16_t mb;
	int16_t mc;
	int16_t md;
} bmp085_calib_data;

void bmp085_init ();
void bmp085_set_mode(bmp085_mode_t mode);
float bmp085_read_temperature();
float bmp085_read_pressure();

//float pressureToAltitude(float seaLevel, float atmospheric);
//float seaLevelForAltitude(float altitude, float atmospheric);
//float pressureToAltitude(float seaLevel, float atmospheric, float temp);
//float seaLevelForAltitude(float altitude, float atmospheric, float temp);
//void softReset();
//void readCoefficients();
//void readRawTemperature(int32_t *temp);
//void readRawPressure(int32_t *pressure);
//int32_t computeB5(int32_t ut);
#endif
