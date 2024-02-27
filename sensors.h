#ifndef sensors_h
#define sensors_h

#include <time.h>

typedef struct {
	union {
		double v[3];
		struct {
			double x;
			double y;
			double z;
		};
	};
} Vector;

typedef struct {
	union {
		double v[2];
		struct {
			double latitude;
			double longitude;
		};
	};
} location_t;

typedef struct {
	time_t time;
	location_t location;
	double altitude;
	double pitch;
	double roll;
	double direction;
	double speed;
	double temperature;
	double humidity;
	double pressure;
} sensors_t;

sensors_t sensors;

void sensors_init();
void sensors_read();



#endif
