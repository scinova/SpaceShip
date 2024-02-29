#ifndef sensors_h
#define sensors_h

#include <time.h>

struct {
	struct tm time;
	struct {
		double latitude;
		double longitude;
	} location;
	struct {
		double pitch;
		double roll;
		double direction;
		double speed;
	} orientation;
	struct {
		double temperature;
		double humidity;
		double pressure;
	} weather;
} sensors;

void sensors_init();
void read_weather_sensors();
void enable_position_sensors();
void disable_position_sensors();
void enable_motion_detect();
void disable_motion_detect();
void read_position_sensors();
void enable_gps_sensor();
void disable_gps_sensor();
void read_gps_sensor();
void sensors_loop();

#endif
