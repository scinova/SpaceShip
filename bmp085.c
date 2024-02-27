#include <util/delay.h>
#include "i2c.h"
//#include <limits.h>
#include <math.h>
#include "bmp085.h"

static bmp085_calib_data _bmp085_coeffs;
static uint8_t _bmp085Mode;

void bmp085_set_mode(bmp085_mode_t mode) {
	_bmp085Mode = mode;
}

bool bmp085_identified() {
	return i2c_read_register(BMP085_ADDRESS, BMP085_REGISTER_CHIPID) == 0x55;
}

uint16_t bmp085_read_raw_temperature() {
	uint16_t t;
#if BMP085_USE_DATASHEET_VALS
	t = 27898;
#else
	i2c_write_register(BMP085_ADDRESS, BMP085_REGISTER_CONTROL, BMP085_CMD_MEASURE_TEMPERATURE);
	//delay(5);
	t = i2c_read_registerU16(BMP085_ADDRESS, BMP085_REGISTER_OUT_H);
#endif
	return t;
}


#define BMP085_USE_DATASHEET_VALS 0

void bmp085_readCoefficients(void) {
#if BMP085_USE_DATASHEET_VALS
	_bmp085_coeffs.ac1 = 408;
	_bmp085_coeffs.ac2 = -72;
	_bmp085_coeffs.ac3 = -14383;
	_bmp085_coeffs.ac4 = 32741;
	_bmp085_coeffs.ac5 = 32757;
	_bmp085_coeffs.ac6 = 23153;
	_bmp085_coeffs.b1 = 6190;
	_bmp085_coeffs.b2 = 4;
	_bmp085_coeffs.mb = -32768;
	_bmp085_coeffs.mc = -8711;
	_bmp085_coeffs.md = 2868;
	_bmp085Mode = 0;
#else
	_bmp085_coeffs.ac1 = i2c_read_register16(BMP085_ADDRESS, BMP085_REGISTER_AC1);
	_bmp085_coeffs.ac2 = i2c_read_register16(BMP085_ADDRESS, BMP085_REGISTER_AC2);
	_bmp085_coeffs.ac3 = i2c_read_register16(BMP085_ADDRESS, BMP085_REGISTER_AC3);
	_bmp085_coeffs.ac4 = i2c_read_registerU16(BMP085_ADDRESS, BMP085_REGISTER_AC4);
	_bmp085_coeffs.ac5 = i2c_read_registerU16(BMP085_ADDRESS, BMP085_REGISTER_AC5);
	_bmp085_coeffs.ac6 = i2c_read_registerU16(BMP085_ADDRESS, BMP085_REGISTER_AC6);
	_bmp085_coeffs.b1 = i2c_read_register16(BMP085_ADDRESS, BMP085_REGISTER_B1);
	_bmp085_coeffs.b2 = i2c_read_register16(BMP085_ADDRESS, BMP085_REGISTER_B2);
	_bmp085_coeffs.mb = i2c_read_register16(BMP085_ADDRESS, BMP085_REGISTER_MB);
	_bmp085_coeffs.mc = i2c_read_register16(BMP085_ADDRESS, BMP085_REGISTER_MC);
	_bmp085_coeffs.md = i2c_read_register16(BMP085_ADDRESS, BMP085_REGISTER_MD);
#endif
}

void bmp085_init() {
	bmp085_readCoefficients();
}


int32_t computeB5(int32_t ut) {
	int32_t X1 = (ut - (int32_t)_bmp085_coeffs.ac6) * ((int32_t)_bmp085_coeffs.ac5) >> 15;
	int32_t X2 = ((int32_t)_bmp085_coeffs.mc << 11) / (X1 + (int32_t)_bmp085_coeffs.md);
	return X1 + X2;
}


float bmp085_read_temperature() {
	int32_t UT, B5;
	float t;
	bmp085_read_raw_temperature(&UT);
#if BMP085_USE_DATASHEET_VALS
	UT = 27898;
	_bmp085_coeffs.ac6 = 23153;
	_bmp085_coeffs.ac5 = 32757;
	_bmp085_coeffs.mc = -8711;
	_bmp085_coeffs.md = 2868;
#endif
	B5 = computeB5(UT);
	t = (B5 + 8) >> 4;
	t /= 10;
	return t;
}

int32_t bmp085_read_raw_pressure() {
	int32_t pressure;
#if BMP085_USE_DATASHEET_VALS
	pressure = 23843;
#else
	uint8_t p8;
	uint16_t p16;
	int32_t p32;
	uint8_t oversampling = _bmp085Mode << 6;
	i2c_write_register(BMP085_ADDRESS, BMP085_REGISTER_CONTROL, BMP085_CMD_MEASURE_PRESSURE | oversampling);
	switch (_bmp085Mode) {
		case BMP085_MODE_ULTRALOWPOWER:
			_delay_ms(5);
			break;
		case BMP085_MODE_STANDARD:
			_delay_ms(8);
			break;
		case BMP085_MODE_HIGHRES:
			_delay_ms(14);
			break;
		case BMP085_MODE_ULTRAHIGHRES:
		default:
			_delay_ms(26);
			break;
	}
	p16 = i2c_read_registerU16(BMP085_ADDRESS, BMP085_REGISTER_OUT_H);
	p32 = (uint32_t)p16 << 8;
	p8 = i2c_read_register(BMP085_ADDRESS, BMP085_REGISTER_OUT_XL);
	p32 += p8;
	p32 >>= (8 - _bmp085Mode);
	pressure = p32;
#endif
	return pressure;
}

float bmp085_read_pressure() {
	float pressure;
	int32_t ut = 0, up = 0, compp = 0;
	int32_t x1, x2, b5, b6, x3, b3, p;
	uint32_t b4, b7;

	bmp085_read_raw_temperature(&ut);
	bmp085_read_raw_pressure(&up);

	// temperature compensation
	b5 = computeB5(ut);

	// pressure compensation
	b6 = b5 - 4000;
	x1 = (_bmp085_coeffs.b2 * ((b6 * b6) >> 12)) >> 11;
	x2 = (_bmp085_coeffs.ac2 * b6) >> 11;
	x3 = x1 + x2;
	b3 = (((((int32_t)_bmp085_coeffs.ac1) * 4 + x3) << _bmp085Mode) + 2) >> 2;
	x1 = (_bmp085_coeffs.ac3 * b6) >> 13;
	x2 = (_bmp085_coeffs.b1 * ((b6 * b6) >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = (_bmp085_coeffs.ac4 * (uint32_t)(x3 + 32768)) >> 15;
	b7 = ((uint32_t)(up - b3) * (50000 >> _bmp085Mode));

	if (b7 < 0x80000000)
		p = (b7 << 1) / b4;
	else
		p = (b7 / b4) << 1;

	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	compp = p + ((x1 + x2 + 3791) >> 4);

	pressure = compp;
	return pressure;
}
