#include "dhtxx.h"
#include <avr/io.h>

#include <avr/interrupt.h>

#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

dht_t _type = DHT22;
uint8_t data[5];

void pinModeOutput() {
	DDRH |= _BV(PH4);
}
void pinModeInputPullup() {
	DDRH &= ~_BV(PH4);
	PORTH |= _BV(PH4);
}
bool readPin() {
	return (PINH & _BV(PH4)) != 0; 
}
void writePin(bool value) {
	if (value)
		PORTH |= _BV(PH4);
	else
		PORTH &= ~_BV(PH4);
}

uint32_t countUntil(bool level) {
	counter = 0;
	while (true) {
		if (counter++ >= 100000)
			return 100021;
		if (readPin() != level)
			break;
	}
	return counter;
}


bool dht_read() {
	pinModeInputPullup();
	_delay_ms(1);
	pinModeOutput();
	writePin(0);
	switch (_type) {
		case DHT11:
		case DHT12:
			_delay_ms(20); // datasheet says "at least 18ms"
			break;
		case DHT21:
		case DHT22:
			_delay_us(1100); // datasheet says "at least 1ms"
			break;
	}
	writePin(1);

	pinModeInputPullup();



	//_delay_us(55);
//	countUntil(false);
	cli();
	lens0[0] = countUntil(true);
	lens1[0] = countUntil(false);
	lens0[1] = countUntil(true);
	lens1[1] = countUntil(false);
	lens0[2] = countUntil(true);
	lens1[2] = countUntil(false);
	sei();
	return true;

	//cli();
	// 40 bits 0=50us/28us 1=50us/70us

	for (int i = 0; i < 40; i++) {
		uint32_t counter;
		while ((PINH & _BV(PH4)) == 0) { // count 0 time
			if (counter++ > 100000L)
				return false;
		}
		lens0[i] = counter;
		counter = 0;
		while ((PINH & _BV(PH4)) != 0) { // count 1 time
			if (counter++ >= 100000L)
				return false;
		}
		lens1[i] = counter;
	}
/*	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 8; y++) {
			int i = 8 * x + y;
			if (lens1[i] < lens0[i])
				data[x] |= 1 << y;
			else
				data[x] &= ~(1 << y);
		}
	}*/
	//if (data[4] != ((data[0] + data[1] + data[2] + data[3]) & 0xFF))
	//	return false;
	return true;
}


float dht_read_temperature() {
	dht_read();
	float f = 0;
	switch (_type) {
		case DHT11:
			f = data[2];
			if (data[3] & 0x80)
				f = -1 - f;
			f += (data[3] & 0x0f) * 0.1;
			break;
		case DHT12:
			f = data[2];
			f += (data[3] & 0x0f) * 0.1;
			if (data[2] & 0x80)
				f *= -1;
			break;
		case DHT22:
		case DHT21:
			f = ((uint16_t)(data[2] & 0x7F)) << 8 | data[3];
			f *= 0.1;
			if (data[2] & 0x80)
				f *= -1;
			break;
	}
	return f;
}

float dht_read_humidity() {
	dht_read();
	float f = 0;
	switch (_type) {
		case DHT11:
		case DHT12:
			f = data[0] + data[1] * 0.1;
			break;
		case DHT22:
		case DHT21:
			f = ((uint16_t)data[0]) << 8 | data[1];
			f *= 0.1;
			break;
	}
	return f;
}