#ifndef mpu6050_h
#define mpu6050_h

#include "i2c.h"
#include "vector.h"

typedef enum {
	MPU6050_AccelerometerRange2G	= 0,
	MPU6050_AccelerometerRange4G	= 1,
	MPU6050_AccelerometerRange8G	= 2,
	MPU6050_AccelerometerRange16G	= 3
} mpu6050_accelerometer_range_t;

typedef enum {
	MPU6050_GyroscopeRange250Deg	= 0,
	MPU6050_GyroscopeRange500Deg	= 1,
	MPU6050_GyroscopeRange1000Deg	= 2,
	MPU6050_GyroscopeRange2000Deg	= 3
} mpu6050_gyroscope_range_t;

typedef enum {
	MPU6050_WakeupFrequency1_25HZ	= 0,
	MPU6050_WakeupFrequency5HZ		= 1,
	MPU6050_WakeupFrequency20HZ		= 2,
	MPU6050_WakeupFrequency40HZ		= 3
} mpu6050_wakeup_frequency_t;

typedef enum {
	MPU6050_FSyncDisabled	= 0,
	MPU6050_FSyncTemp			= 1,
	MPU6050_FSyncGyroX		= 2,
	MPU6050_FSyncGyroY		= 3,
	MPU6050_FSyncGyroZ		= 4,
	MPU6050_FSyncAccelX		= 5,
	MPU6050_FSyncAccelY		= 6,
	MPU6050_FSyncAccelZ		= 7
} mpu6050_fsync_output_t;

typedef enum {
	MPU6050_Lowpass260Hz	= 0,
	MPU6050_Lowpass184Hz	= 1,
	MPU6050_Lowpass94Hz 	= 2,
	MPU6050_Lowpass44Hz 	= 3,
	MPU6050_Lowpass21Hz		= 4,
	MPU6050_Lowpass10Hz		= 5,
	MPU6050_Lowpass5Hz		= 6
} mpu6050_lowpass_filter_t;

typedef enum {
	MPU6050_HighpassDisable,
	MPU6050_Highpass5Hz,
	MPU6050_Highpass2_5HZ,
	MPU6050_Highpass1_25HZ,
	MPU6050_HighpassUnused,
	MPU6050_HighpassHold,
} mpu6050_highpass_filter_t;

typedef enum {
	MPU6050_Internal8MHz			= 0,
	MPU6050_PLLGyroscopeX			= 1,
	MPU6050_PLLGyroscopeY			= 2,
	MPU6050_PLLGyroscopeZ			= 3,
	MPU6050_PLLExternal_32K		= 4,
	MPU6050_PLLExternal_19MHz	= 5,
	MPU6050_ClockStopped			= 7
} mpu6050_clock_source_t;

void mpu6050_init();
bool mpu6050_identified();

DoubleVector mpu6050_read_acceleration();
DoubleVector mpu6050_read_angular_velocity();
float mpu6050_read_temperature();

void mpu6050_set_accelerometer_range (mpu6050_accelerometer_range_t);
void mpu6050_set_gyroscope_range (mpu6050_gyroscope_range_t);
void mpu6050_set_lowpass_filter (mpu6050_lowpass_filter_t);
void mpu6050_set_highpass_filter(mpu6050_highpass_filter_t);
void mpu6050_set_fsync_output (mpu6050_fsync_output_t);
void mpu6050_set_clock_source (mpu6050_clock_source_t);
void mpu6050_set_samplerate_divisor (uint8_t val);
void mpu6050_set_wakeup_frequency (mpu6050_wakeup_frequency_t);
void mpu6050_enable_cycle();
void mpu6050_disable_cycle();
void mpu6050_enable_sleep();
void mpu6050_disable_sleep();
void mpu6050_enable_temperature();
void mpu6050_disable_temperature();
void mpu6050_set_accelerator_standby (bool x, bool y, bool z);
void mpu6050_set_gyroscope_standby (bool x, bool y, bool z);

void mpu6050_set_motion_detection_threshold(uint8_t threshold);
void mpu6050_set_motion_detection_duration(uint8_t duration);
void mpu6050_enable_motion_interrupt();
void mpu6050_disable_motion_interrupt();

mpu6050_accelerometer_range_t mpu6050_get_accelerometer_range();
mpu6050_gyroscope_range_t mpu6050_get_gyroscope_range();
mpu6050_wakeup_frequency_t mpu6050_get_wakeup_frequency();
mpu6050_lowpass_filter_t mpu6050_get_filter_bandwidth();
mpu6050_highpass_filter_t mpu6050_get_highpass_filter();
mpu6050_fsync_output_t mpu6050_get_fsync_output();
mpu6050_clock_source_t mpu6050_get_clock_source();
uint8_t mpu6050_get_samplerate_divisor();

#endif
