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
}

void read_weather_sensors() {
	float bmp_temperature = bmp085_read_temperature();
	float bmp_pressure = bmp085_read_pressure();
	float dht_temperature = dht_read_temperature();
	float dht_humidity = dht_read_humidity();
	sensors.weather.temperature = dht_temperature;
	sensors.weather.humidity = dht_temperature;
	sensors.weather.pressure = bmp_pressure;
}

void read_gps_sensor() {
	// struct tm gps_time = gps_read_time();
	// if (diff > 3)
	//		ds3231_set_time(&gps_time);
	// location_t gps_location = gps_read_location();
	// float gps_speed = gps_read_speed();
	// float gps_altitude = gps_read_altitude();
}

void enable_position_sensors() {
	mpu6050_disable_sleep();
	//qmc5883l_enable_sleep();
	qmc5883l_set_mode(QMC5883L_MODE_CONTINOUS);
}

void disable_position_sensors() {
	mpu6050_enable_sleep();
	//qmc5883l_disable_sleep();
	qmc5883l_set_mode(QMC5883L_MODE_STANDBY);
}

void read_position_sensors() {
	DoubleVector mag_value = qmc5883l_read();
	int16_t mag_temperature = qmc5883l_read_temperature();
	DoubleVector mpu_acceleration = mpu6050_read_acceleration();
	DoubleVector mpu_angular_velocity = mpu6050_read_angular_velocity();
	// DO THE HOKUSPOKUS MAGIC
}

void sensors_loop() {
	ds3231_read_time(&sensors.time);
}
