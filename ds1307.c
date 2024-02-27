#include "ds1307.h"
#include "i2c.h"
#include <time.h>

static uint8_t bcd2dec(uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t dec2bcd(uint8_t val) { return val + 6 * (val / 10); }

#define ADDR 0x68

typedef enum {
	SECONDS = 0,
	MINUTES,
	HOURS,
	DAY, 	//day of week
	DATE,	//day of month
	MONTH,
	YEAR,
	CONTROL,
	NVRAM // 56 bytes, 0x08 to 0x3f
} ds1307_register_t;

// CONTROL
#define OUT			7
#define SQWE		4
#define RS1			1
#define RS0			0
#define RS			(1 << RS1) | (1 << RS0)
// SECONDS
#define CH			7 // clock halt
// HOURS
#define MODE12H	6 // 12h mode

void ds1307_enable_clock() {
	i2c_clear_register_bit(ADDR, SECONDS, CH);
}

void ds1307_disable_clock() {
	i2c_set_register_bit(ADDR, SECONDS, CH);
}

void ds1307_set_mode_24h() {
	i2c_clear_register_bit(ADDR, HOURS, MODE12H);
}

void ds1307_init() {
	ds1307_disable_clock();
	ds1307_disable_squarewave();
	ds1307_set_squarewave_frequency(0);
	ds1307_set_mode_24h();
	ds1307_enable_clock();
}

void ds1307_enable_squarewave() {
	i2c_set_register_bit(ADDR, CONTROL, SQWE);
}

void ds1307_disable_squarewave() {
	i2c_clear_register_bit(ADDR, CONTROL, SQWE);
}

void ds1307_set_squarewave_frequency(ds1307_frequency_t value) {
	i2c_write_register_bits(ADDR, CONTROL, value << RS0, RS0);
}

void ds1307_write_out(bool b) {
	i2c_write_register_bit(ADDR, CONTROL, b, OUT);
}

void ds1307_get_time(struct tm *t) {
	t->tm_sec = ds1307_second();
	t->tm_min = ds1307_minute();
	t->tm_hour = ds1307_hour();
	t->tm_year = ds1307_year() - 100;
	t->tm_mon = ds1307_month() - 1;
	t->tm_mday = ds1307_day();
}

uint8_t ds1307_second() {
	return bcd2dec(i2c_read_register(ADDR, SECONDS) & 0x7f);
}

uint8_t ds1307_minute() {
	return bcd2dec(i2c_read_register(ADDR, MINUTES));
}

uint8_t ds1307_hour() {
	return bcd2dec(i2c_read_register(ADDR, HOURS) & 0x3f);
}

uint16_t ds1307_year() {
	return bcd2dec(i2c_read_register(ADDR, YEAR)) + 2000;
}

uint8_t ds1307_month() {
	return bcd2dec(i2c_read_register(ADDR, MONTH));
}

uint8_t ds1307_day() {
	return bcd2dec(i2c_read_register(ADDR, DATE));
}

void ds1307_set_time(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
	bool had_clock = ds1307_clock_is_enabled();
	if (had_clock)
		ds1307_disable_clock();
	i2c_write_register(ADDR, YEAR, dec2bcd(year - 2000));
	i2c_write_register(ADDR, MONTH, dec2bcd(month));
	i2c_write_register(ADDR, DATE, dec2bcd(day));
	i2c_write_register_bits(ADDR, HOURS, dec2bcd(hour), 0x3f);
	i2c_write_register(ADDR, MINUTES, dec2bcd(minute));
	i2c_write_register_bits(ADDR, SECONDS, dec2bcd(second), 0x7f);
	if (had_clock)
		ds1307_enable_clock();	
}

bool ds1307_clock_is_enabled() {
	return i2c_read_register_bit(ADDR, SECONDS, CH) == 0;
}
