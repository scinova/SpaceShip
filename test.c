#include "mpu6050.h"

DoubleVector v;

int amain() {
	mpu6050_init();
	v = mpu6050_read_acceleration();
	while (1) {
		v = mpu6050_read_acceleration();
		v = mpu6050_read_angular_velocity();
	}
}
