#include "mpu6050.h"
#include "i2c.h"
#include <avr/io.h>

#define ADDRESS 0x69

typedef enum {
	SELF_TEST_X =			0x0D,
	SELF_TEST_Y =			0x0E,
	SELF_TEST_Z =			0x0F,
	SELF_TEST_A =			0x10,
	SMPLRT_DIV =			0x19,
	CONFIG =					0x1A,
	GYRO_CONFIG =			0x1B,
	ACCEL_CONFIG =		0x1C,
	MOT_THR							= 0x1F,
	MOT_DUR							= 0x20,
	INT_PIN_CONFIG			= 0x37,
	INT_ENABLE 					= 0x38,
	INT_STATUS					= 0x3A,
	ACCEL_OUT 					= 0x3B,
	ACCEL_XOUT_H 				= 0x3B,
	ACCEL_XOUT_L 				= 0x3C,
	ACCEL_YOUT_H 				= 0x3D,
	ACCEL_YOUT_L 				= 0x3E,
	ACCEL_ZOUT_H 				= 0x3F,
	ACCEL_ZOUT_L 				= 0x40,
	TEMP_OUT_H =		0x41,
	TEMP_OUT_L =		0x42,
	GYRO_OUT =			0x43,
	GYRO_XOUT_H =		0x43,
	GYRO_XOUT_L =		0x44,
	GYRO_YOUT_H =		0x45,
	GYRO_YOUT_L =		0x46,
	GYRO_ZOUT_H =		0x47,
	GYRO_ZOUT_L =		0x48,
	SIGNAL_PATH_RESET		=	0x68, // write only
	USER_CTRL 					= 0x6A,
	PWR_MGMT_1 					= 0x6B,
	PWR_MGMT_2 					= 0x6C,
	WHO_AM_I 						= 0x75 // device id (=0x55)
} register_t;

// GYRO_CONFIG
#define XG_ST						7
#define YG_ST						6
#define ZG_ST						5
#define FS_SEL_1				4
#define FS_SEL_0				3
#define FS_SEL					_BV(FS_SEL_1) | _BV(FS_SEL_0)
// CONFIG
#define EXT_SYNC_SET_2	5
#define EXT_SYNC_SET_1	4
#define EXT_SYNC_SET_0	3
#define DLPF_CFG_2			2
#define DLPF_CFG_1			1
#define DLPF_CFG_0			0
#define EXT_SYNC_SET		_BV(EXT_SYNC_SET_2)) | _BV(EXT_SYNC_SET_1) | _BV(EXT_SYNC_SET_0)
#define DLP_CFG					_BV(DLP_CFG_2)) | _BV(DLP_CFG_1) | _BV(DLP_CFG_0)
// ACCEL_CONFIG
#define XA_ST						7
#define YA_ST						6
#define ZA_ST						5
#define AFS_SEL_1				4
#define AFS_SEL_0				3
#define AFS_SEL					_BV(AFS_SEL_1) | _BV(AFS_SEL_0)
// SIGNAL_PATH_RESET
#define GYRO_RESET			2
#define ACCEL_RESET			1
#define TEMP_RESET			0
// USER_CTL
#define FIFO_EN						6
#define I2C_MST_EN				5
#define I2C_IF_DIS				4 //only on 6000
#define FIFO_RESET				2
#define I2C_MASTER_RESET	1
#define SIG_COND_RESET		0
// PWR_MGMT1
#define DEVICE_RESET		7
#define	SLEEP						6
#define	CYCLE						5
#define	TEMP_DIS				3
#define	CLKSEL_2				2
#define	CLKSEL_1				1
#define	CLKSEL_0				0
#define CLKSEL					_BV(CLKSEL_2) | _BV(CLKSEL_1) | _BV(CLKSEL_0)
// PWR_MGMT2
#define LP_WAKE_CTRL_1	7
#define LP_WAKE_CTRL_0	6
#define STBY_XA					5
#define STBY_YA					4
#define STBY_ZA					3
#define STBY_XG					2
#define STBY_YG					1
#define STBY_ZG					0
#define LP_WAKE_CTRL		_BV(LP_WAKE_CTRL_1) | _BV(LP_WAKE_CTRL_0)
// INT_PIN_CFG
#define INT_LEVEL				7
#define INT_OPEN				6
#define LATCH_INT_EN		5
#define INT_RD_CLEAR		4
#define	FSYNC_INT_EN		3
#define I2C_BYPASS_EN		2
// INT_ENABLE
#define MOT_EN			6 //???
#define FIFO_OFLOW_EN		4
#define I2C_MST_INT_EN	3
#define DATA_RDY_EN			2
// INT_STATUS
#define MOT_INT					6 //???
#define FIFO_OFLOW_INT	4
#define I2C_MST_INT			3
#define DATA_RDY_INT		2

void mpu6050_init() {
	mpu6050_set_accelerometer_range(MPU6050_AccelerometerRange2G);
	mpu6050_set_gyroscope_range(MPU6050_GyroscopeRange500Deg);
	//mpu6050_setFilterBandwidth(BAND_260_HZ);
	mpu6050_set_samplerate_divisor(0);
	mpu6050_set_clock_source(MPU6050_PLLGyroscopeX);
}

void mpu6050_reset() {
	i2c_set_register_bit(ADDRESS, PWR_MGMT_1, DEVICE_RESET);
	while (i2c_read_register_bit(ADDRESS, PWR_MGMT_1, DEVICE_RESET)); //wait 100ms
	i2c_set_register_bits(ADDRESS, SIGNAL_PATH_RESET, _BV(GYRO_RESET) | _BV(ACCEL_RESET) | _BV(TEMP_RESET));
}

float mpu6050_read_temperature() {
	int16_t raw = i2c_read_register16(ADDRESS, TEMP_OUT_H);
	return (raw / 340.0) + 36.53;
}

DoubleVector mpu6050_read_acceleration() {
	int16_t xyz[3];
	i2c_read_registers(ADDRESS, ACCEL_OUT, 6, (uint8_t *)&xyz);
	// todo: scale values
	DoubleVector v = {xyz[0], xyz[1], xyz[2]};
	return v;
}

DoubleVector mpu6050_read_angular_velocity() {
	int16_t xyz[3];
	i2c_read_registers(ADDRESS, GYRO_OUT, 6, (uint8_t *)&xyz);
	// todo: scale values
	DoubleVector v = {xyz[0], xyz[1], xyz[2]};
	return v;
}

void mpu6050_enable_sleep() {
	i2c_set_register_bit(ADDRESS, PWR_MGMT_1, SLEEP);
}

void mpu6050_disable_sleep() {
	i2c_clear_register_bit(ADDRESS, PWR_MGMT_1, SLEEP);
}

void mpu6050_enable_cycle() {
	i2c_set_register_bit(ADDRESS, PWR_MGMT_1, CYCLE);
}

void mpu6050_disable_cycle() {
	i2c_clear_register_bit(ADDRESS, PWR_MGMT_1, CYCLE);
}

mpu6050_wakeup_frequency_t mpu6050_get_wakeup_frequency() {
	return i2c_read_register_bits(ADDRESS, PWR_MGMT_2, LP_WAKE_CTRL) >> LP_WAKE_CTRL_0;
}

void mpu6050_set_wakeup_frequency(mpu6050_wakeup_frequency_t freq) {
	i2c_write_register_bits(ADDRESS, PWR_MGMT_2, freq << LP_WAKE_CTRL_0, LP_WAKE_CTRL);
}

mpu6050_gyroscope_range_t mpu6050_get_gyroscope_range() {
	return (mpu6050_gyroscope_range_t)i2c_read_register_bits(ADDRESS, GYRO_CONFIG, FS_SEL) << FS_SEL_0;
}

void mpu6050_set_gyroscope_range(mpu6050_gyroscope_range_t range) {
	i2c_write_register_bits(ADDRESS, GYRO_CONFIG, range << FS_SEL_0, FS_SEL);
}

mpu6050_accelerometer_range_t mpu6050_get_accelerometer_range() {
	return (mpu6050_accelerometer_range_t)i2c_read_register_bits(ADDRESS, ACCEL_CONFIG, AFS_SEL) >> AFS_SEL_0;
}

void mpu6050_set_accelerometer_range(mpu6050_accelerometer_range_t range) {
	i2c_write_register_bits(ADDRESS, ACCEL_CONFIG, range << AFS_SEL_0, AFS_SEL);
}

void mpu6050_set_clock_source(mpu6050_clock_source_t source) {
	i2c_write_register_bits(ADDRESS, PWR_MGMT_1, source << CLKSEL_0, CLKSEL);
}

mpu6050_clock_source_t mpu6050_get_clock_source() {
	return i2c_read_register_bits(ADDRESS, PWR_MGMT_1, CLKSEL) << CLKSEL_0;
}

uint8_t mpu6050_get_samplerate_divisor () {
	return i2c_read_register(ADDRESS, SMPLRT_DIV);
}

void mpu6050_set_samplerate_divisor(uint8_t value) {
	i2c_write_register(ADDRESS, SMPLRT_DIV, value);
}

mpu6050_lowpass_filter_t mpu6050_get_lowpass_filter() {
	return i2c_read_register_bits(ADDRESS, CONFIG, 0b00000111);
}

void mpu_set_lowpass_filter(mpu6050_lowpass_filter_t value) {
	i2c_write_register_bits(ADDRESS, CONFIG, value, 0b00000111);
}

mpu6050_highpass_filter_t mpu6050_get_highpass_filter() {
	return i2c_read_register_bits(ADDRESS, ACCEL_CONFIG, 0b00000111);
}

void mpu6050_set_highpass_filter(mpu6050_highpass_filter_t v) {
	i2c_write_register_bits(ADDRESS, ACCEL_CONFIG, v, 0b00000111);
}

bool mpu6050_identified() {
	uint8_t chip_id = i2c_read_register(ADDRESS, WHO_AM_I);
	return chip_id == 0x68;
}

void mpu6050_set_gyroscope_standby(bool x, bool y, bool z) {
	uint8_t bits = (x ? _BV(STBY_XG) : 0) | (y ? _BV(STBY_YG) : 0) | (z ? _BV(STBY_ZG) : 0);
	uint8_t mask = _BV(STBY_XG) | _BV(STBY_YG) | _BV(STBY_ZG);
	i2c_write_register_bits(ADDRESS, PWR_MGMT_2, bits, mask);
}

void mpu6050_set_accelerometer_standby(bool x, bool y, bool z) {
	uint8_t bits = (x ? _BV(STBY_XA) : 0) | (y ? _BV(STBY_YA) : 0) | (z ? _BV(STBY_ZA) : 0);
	uint8_t mask = _BV(STBY_XA) | _BV(STBY_YA) | _BV(STBY_ZA);
	i2c_write_register_bits(ADDRESS, PWR_MGMT_2, bits, mask);
}

void mpu6050_enable_temperature() {
		i2c_clear_register_bit(ADDRESS, PWR_MGMT_1, TEMP_DIS);
}

void mpu6050_disable_temperature() {
		i2c_set_register_bit(ADDRESS, PWR_MGMT_1, TEMP_DIS);
}

void mpu6050_set_motion_detection_threshold(uint8_t threshold) {
	i2c_write_register(ADDRESS, MOT_THR, threshold);
}

void mpu6050_set_motion_detection_duration(uint8_t duration) {
	i2c_write_register(ADDRESS, MOT_DUR, duration);
}

void mpu6050_enable_motion_interrupt() {
	i2c_set_register_bit(ADDRESS, INT_ENABLE, MOT_EN);
}
void mpu6050_disable_motion_interrupt() {
	i2c_clear_register_bit(ADDRESS, INT_ENABLE, MOT_EN);
}
