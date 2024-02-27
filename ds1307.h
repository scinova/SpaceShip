#ifndef ds1307_h
#define ds1307_h

#include <time.h>
#include "i2c.h"

typedef enum {
	DS1307_1HZ		= 0,
	DS1307_4KHZ		= 1,
	DS1307_8KHZ		= 2,
	DS1307_32KHZ	= 3
} ds1307_frequency_t;

void ds1307_init();
void ds1307_set_time(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
void ds1307_get_time(struct tm *);
uint8_t ds1307_second();
uint8_t ds1307_minute();
uint8_t ds1307_hour();
uint16_t ds1307_year();
uint8_t ds1307_month();
uint8_t ds1307_day();

bool ds1307_clock_is_enabled();
void ds1307_enable_clock();
void ds1307_disable_clock();
void ds1307_set_squarewave_frequency(ds1307_frequency_t);
void ds1307_enable_squarewave();
void ds1307_disable_squarewave();
void ds1307_write_out(bool);

uint8_t ds1307_read_nvram (uint8_t address);
void ds1307_read_nvram_block (uint8_t address, uint8_t *buf, uint8_t size);
void ds1307_write_nvram (uint8_t address, uint8_t data);
void ds1307_write_nvram_block (uint8_t address, uint8_t *buf, uint8_t size);

#endif
