#ifndef dhtxx_h
#define dhtxx_h

#include <stdint.h>

uint32_t counter;
uint32_t lens0[40];
uint32_t lens1[40];

typedef enum {
	DHT11,
	DHT12,
	DHT21,
	DHT22
} dht_t;

float dht_read_temperature();
float dht_read_humidity();
float dht_computeHeatIndex(float temperature, float humidity);
//float CtoF(float);
//float FtoC(float);

#endif
