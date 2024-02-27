#ifndef qmc5883l_h
#define qmc5883l_h

#include <stdint.h>
#include "i2c.h"
#include "vector.h"

typedef enum {
	QMC5883L_MODE_STANDBY	= 2,
	QMC5883L_MODE_CONTINOUS	= 1
} qmc5883l_mode_t;

typedef enum {
	QMC5883L_RANGE_2G = 0,
	QMC5883L_RANGE_8G = 1
} qmc5883l_range_t;

typedef enum {
	QMC5883L_ODR_10HZ	= 0,
	QMC5883L_ODR_50HZ	= 1,
	QMC5883L_ODR_100HZ	= 2,
	QMC5883L_ODR_200HZ	= 3
} qmc5883l_output_data_rate_t;

typedef enum {
	QMC5883L_OSR_512	= 0,
	QMC5883L_OSR_256	= 1,
	QMC5883L_OSR_128	= 2,
	QMC5883L_OSR_64		= 3
} qmc5883l_over_sample_ratio_t;

void qmc5883l_init();
DoubleVector qmc5883l_read();
int16_t qmc5883l_read_temperature();
void qmc5883l_enable_interrupts();
void qmc5883l_disable_interrupts();
void qmc5883l_set_range(qmc5883l_range_t range);
void qmc5883l_set_output_data_rate(qmc5883l_output_data_rate_t rate);
void qmc5883l_set_mode(qmc5883l_mode_t mode);
void qmc5883l_set_over_sample_ratio(qmc5883l_over_sample_ratio_t ratio);
void qmc5883l_set_config(qmc5883l_mode_t mode,
		qmc5883l_output_data_rate_t odr,
		qmc5883l_range_t range,
		qmc5883l_over_sample_ratio_t osr);
bool qmc5883l_data_ready();
bool qmc5883l_data_overflow();
bool qmc5883l_data_skipped();
void qmc5883l_soft_reset();

#endif
