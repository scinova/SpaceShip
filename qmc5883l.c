#include "qmc5883l.h"
#include "i2c.h"

#define QMC5883L_ADDRESS 0x0d

#define QMC5883L_XOUT_H				0x00
#define QMC5883L_XOUT_L				0x01
#define QMC5883L_YOUT_H				0x02
#define QMC5883L_YOUT_L				0x03
#define QMC5883L_ZOUT_H				0x04
#define QMC5883L_ZOUT_L				0x05
#define QMC5883L_STATUS				0x06
#define DRDY	0 // data ready
#define OVL		1 // data overlow
#define DOR		2 // data skipped
#define QMC5883L_TOUT_H				0x07
#define QMC5883L_TOUT_L				0x08
#define QMC5883L_CONTROL_1			0x09
#define MODE_0	0 // mode
#define MODE_1	1
#define ODR_0	2 // output data rate
#define ODR_1	3
#define RNG_0	4 // range
#define RNG_1	5
#define OSR_0	6 // over sample ratio
#define OSR_1	7
#define ODR		(3 << ODR_0)
#define RNG		(3 << RNG_0)
#define OSR		(3 << OSR_0)
#define MODE	(3 << MODE_0)
#define QMC5883L_CONTROL_2			0x0a
#define SOFT_RST	7 // soft reset
#define ROL_PNT		6 //
#define INT_ENB		0 // interrupt enable
#define QMC5883L_SET_RESET_PERIOD	0x0b

void qmc5883l_init() {
	i2c_write_register(QMC5883L_ADDRESS, QMC5883L_SET_RESET_PERIOD, 1);
	i2c_write_register(QMC5883L_ADDRESS, QMC5883L_CONTROL_1,
			QMC5883L_MODE_CONTINOUS | QMC5883L_RANGE_2G | QMC5883L_OSR_512 | QMC5883L_ODR_10HZ);
}

DoubleVector qmc5883l_read() {
	uint8_t _buf[6];
	uint8_t *buf = &_buf[0];
	i2c_read_registers(QMC5883L_ADDRESS, QMC5883L_XOUT_H, 6, buf);
	DoubleVector v;
	v.x = (buf[0] | buf[1] >> 8) / 100.;
	v.y = (buf[2] | buf[3] >> 8) / 100.;
	v.z = (buf[4] | buf[5] >> 8) / 100.;
	return v;
}

int16_t qmc5883l_read_temperature() {
	return i2c_read_register16(QMC5883L_ADDRESS, QMC5883L_TOUT_H);
}

void qmc5883l_set_over_sample_ratio(qmc5883l_over_sample_ratio_t value) {
	i2c_write_register_bits(QMC5883L_ADDRESS, QMC5883L_CONTROL_1, value << OSR_0, OSR);
}

void qmc5883l_set_output_data_rate(qmc5883l_output_data_rate_t value) {
	i2c_write_register_bits(QMC5883L_ADDRESS, QMC5883L_CONTROL_1, value << ODR_0, ODR);
}

void qmc5883l_set_range(qmc5883l_range_t value) {
	i2c_write_register_bits(QMC5883L_ADDRESS, QMC5883L_CONTROL_1, value << RNG_0, RNG);
}

void qmc5883l_set_mode(qmc5883l_mode_t value) {
	i2c_write_register_bits(QMC5883L_ADDRESS, QMC5883L_CONTROL_1, value << MODE_0, MODE);
}

void qmc5883l_set_config(qmc5883l_mode_t mode, qmc5883l_output_data_rate_t odr,
		 qmc5883l_range_t range, qmc5883l_over_sample_ratio_t osr) {
	i2c_set_register_bits(QMC5883L_ADDRESS, QMC5883L_CONTROL_1,
			mode << MODE_0 | odr << ODR_0 | range << RNG_0 | osr << OSR_0);
}


void qmc5883l_enable_interrupts() {
	//i2c_set_register_bit(QMC5883L_ADDRESS, QMC5883L_CONTROL_2, INT_ENB);
}

void qmc5883l_disable_interrupts() {
	//i2c_clear_register_bit(QMC5883L_ADDRESS, QMC5883L_CONTROL_2, INT_ENB);
}

bool qmc5883l_data_ready() {
	return 1;//i2c_read_register_bit(QMC5883L_ADDRESS, QMC5883L_STATUS, DRDY);
}

bool qmc5883l_data_overflow() {
	return 1;//i2c_read_register_bit(QMC5883L_ADDRESS, QMC5883L_STATUS, OVL);
}

bool qmc5883l_data_skipped() {
	return 1;//i2c_read_register_bit(QMC5883L_ADDRESS, QMC5883L_STATUS, DOR);
}

void qmc5883l_soft_reset() {
	//i2c_set_register_bit(QMC5883L_ADDRESS, QMC5883L_CONTROL_2, SOFT_RST);
}

typedef enum {
	QMC5883L_DATA_READY    = 0b00000001,
	QMC5883L_DATA_OVERFLOW = 0b00000010,
	QMC5883L_DATA_SKIPPED  = 0b00000100
} QMC5883LStatus;
