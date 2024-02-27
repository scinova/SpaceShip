#include "sensors.h"
#include "ds3231.h"
#include "bmp085.h"
#include "mpu6050.h"
#include "qmc5883l.h"
#include "dhtxx.h"

void sensors_init() {
	ds3231_init();
	bmp085_init();
	mpu6050_init();
	qmc5883l_init();
	//dht_init();
}

void sensors_read() {
	time_t rtc_time = 0; //ds3231_read_time();
	// time_t gps_time = gps_read_time();
	// location_t gps_location = gps_read_location();
	// float gps_speed = gps_read_speed();
	// float gps_altitude = gps_read_altitude();
	//DoubleVector mag_value = qmc5883l_read();
	//int16_t mag_temperature = qmc5883l_read_temperature();
	//DoubleVector mpu_acceleration = mpu6050_read_acceleration();
	//DoubleVector mpu_angular_velocity = mpu6050_read_angular_velocity();
	float mpu_temperature = mpu6050_read_temperature();
	//float bmp_temperature = bmp085_read_temperature();
	//float bmp_pressure = bmp085_read_pressure();
	//float dht_temperature = dht_read_temperature();
	//float dht_humidity = dht_read_humidity();

	// DO THE HOKUSPOKUS MAGIC

	sensors.temperature = mpu_temperature;
	//sensors.humidity = dht_temperature;
	//sensors.pressure = bmp_pressure;
	sensors.time = rtc_time;
}
