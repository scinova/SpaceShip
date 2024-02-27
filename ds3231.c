#include "ds3231.h"
#include "i2c.h"
#include <time.h>

#define DS3231_ADDRESS 0x68

#define DS3231_TIME_REGISTER			0x00
#define	DS3231_ALARM1_REGISTER			0x07
#define	DS3231_ALARM2_REGISTER			0x0B
#define	DS3231_CONTROL_REGISTER			0x0E
#define EOSC	7	// (not) enable oscillator
#define BBSQW	6	// battery-backed square wave enable
#define CONV	5	// convert temperature
#define RS2		4	// rate select
#define RS1		3
#define INTCN	2	// interrupt control
#define A2IE	1	// alarm2 interrupt enable
#define A1IE	0	// alarm1 interrupt enable
#define	DS3231_STATUS_REGISTER			0x0F
#define	DS3231_TEMPERATURE_REGISTER		0x11

uint8_t bcd2bin(uint8_t val) {
	return val - 6 * (val >> 4);
}

uint8_t bin2bcd(uint8_t val) {
	return val + 6 * (val / 10);
}

void ds3231_init() {
	uint8_t buf[] = {
		0, 0, 0,
		1,
		1, 1, 0,
		0, 0, 0, 0, 0, 0, 0,
		0x80}; // oscillator disabled
	i2c_write_registers(DS3231_ADDRESS, 0, 15, (uint8_t *)&buf);
}

void ds3231_start_oscillator() {
	i2c_clear_register_bits(DS3231_ADDRESS, DS3231_CONTROL_REGISTER, EOSC);
}

void ds3231_stop_oscillator() {
	i2c_set_register_bits(DS3231_ADDRESS, DS3231_CONTROL_REGISTER, EOSC);
}

bool ds3231_oscillator_stopped() {
	return i2c_read_register(DS3231_ADDRESS, DS3231_STATUS_REGISTER) & EOSC;
}

void ds3231_set_time(struct tm *t) {
	uint8_t buffer[8] = {
			bin2bcd(t->tm_sec),
			bin2bcd(t->tm_min),
			bin2bcd(t->tm_hour),
			0, //dayofweek
			bin2bcd(t->tm_mday),
			bin2bcd(t->tm_mon),
			bin2bcd(t->tm_year - 2000U)};
	i2c_write_registers(DS3231_ADDRESS, DS3231_TIME_REGISTER, 7, (uint8_t*)&buffer);
}

void ds3231_read_time(struct tm *t) {
	uint8_t buffer[7] = {0,0,0,0,0,0,0};
	i2c_read_registers(DS3231_ADDRESS, DS3231_TIME_REGISTER, 7, (uint8_t*)&buffer);
	t->tm_year = bcd2bin(buffer[6]) + 2000U;
	t->tm_mon = bcd2bin(buffer[5] & 0x7F);
	t->tm_mday = bcd2bin(buffer[4]);
	t->tm_hour = bcd2bin(buffer[2]);
	t->tm_min = bcd2bin(buffer[1]);
	t->tm_sec = bcd2bin(buffer[0] & 0x7F);
}


/*
Ds3231SqwPinMode DS3231::readSqwPinMode() {
	int mode;
	mode = read_register(DS3231_CONTROL) & 0x1C;
	if (mode & 0x04)
			mode = DS3231_OFF;
	return static_cast<Ds3231SqwPinMode>(mode);
}

void DS3231::writeSqwPinMode(Ds3231SqwPinMode mode) {
	uint8_t ctrl = read_register(DS3231_CONTROL);

	ctrl &= ~0x04; // turn off INTCON
	ctrl &= ~0x18; // set freq bits to 0

	write_register(DS3231_CONTROL, ctrl | mode);
}

float DS3231::getTemperature() {
	uint8_t buffer[2] = {0,0};
	readRegisters(DS3231_REGISTER_TEMPERATURE, (uint8_t*)&buffer, 2);
	//XXXXX i2c_dev->write_then_read(buffer, 1, buffer, 2);
	return (float)buffer[0] + (buffer[1] >> 6) * 0.25f;
}

bool DS3231::setAlarm1(const DateTime &dt, Ds3231Alarm1Mode alarm_mode) {
	uint8_t ctrl = read_register(DS3231_CONTROL);
	if (!(ctrl & 0x04)) {
		return false;
	}

	uint8_t A1M1 = (alarm_mode & 0x01) << 7; // Seconds bit 7.
	uint8_t A1M2 = (alarm_mode & 0x02) << 6; // Minutes bit 7.
	uint8_t A1M3 = (alarm_mode & 0x04) << 5; // Hour bit 7.
	uint8_t A1M4 = (alarm_mode & 0x08) << 4; // Day/Date bit 7.
	uint8_t DY_DT = (alarm_mode & 0x10)
									<< 2; // Day/Date bit 6. Date when 0, day of week when 1.
	uint8_t day = (DY_DT) ? dowToDS3231(dt.dayOfTheWeek()) : dt.day();

	uint8_t buffer[5] = {DS3231_ALARM1, uint8_t(bin2bcd(dt.second()) | A1M1),
											 uint8_t(bin2bcd(dt.minute()) | A1M2),
											 uint8_t(bin2bcd(dt.hour()) | A1M3),
											 uint8_t(bin2bcd(day) | A1M4 | DY_DT)};
	//XXXXX i2c_dev->write(buffer, 5);

	write_register(DS3231_CONTROL, ctrl | 0x01); // AI1E

	return true;
}

bool DS3231::setAlarm2(const DateTime &dt, Ds3231Alarm2Mode alarm_mode) {
	uint8_t ctrl = read_register(DS3231_CONTROL);
	if (!(ctrl & 0x04)) {
		return false;
	}

	uint8_t A2M2 = (alarm_mode & 0x01) << 7; // Minutes bit 7.
	uint8_t A2M3 = (alarm_mode & 0x02) << 6; // Hour bit 7.
	uint8_t A2M4 = (alarm_mode & 0x04) << 5; // Day/Date bit 7.
	uint8_t DY_DT = (alarm_mode & 0x08)
									<< 3; // Day/Date bit 6. Date when 0, day of week when 1.
	uint8_t day = (DY_DT) ? dowToDS3231(dt.dayOfTheWeek()) : dt.day();

	uint8_t buffer[4] = {DS3231_ALARM2, uint8_t(bin2bcd(dt.minute()) | A2M2),
											 uint8_t(bin2bcd(dt.hour()) | A2M3),
											 uint8_t(bin2bcd(day) | A2M4 | DY_DT)};
	//XXXXX i2c_dev->write(buffer, 4);

	write_register(DS3231_CONTROL, ctrl | 0x02); // AI2E

	return true;
}

DateTime DS3231::getAlarm1() {
	uint8_t buffer[5] = {DS3231_ALARM1, 0, 0, 0, 0};
	//XXXXX i2c_dev->write_then_read(buffer, 1, buffer, 5);

	uint8_t seconds = bcd2bin(buffer[0] & 0x7F);
	uint8_t minutes = bcd2bin(buffer[1] & 0x7F);
	// Fetching the hour assumes 24 hour time (never 12)
	// because this library exclusively stores the time
	// in 24 hour format. Note that the DS3231 supports
	// 12 hour storage, and sets bits to indicate the type
	// that is stored.
	uint8_t hour = bcd2bin(buffer[2] & 0x3F);

	// Determine if the alarm is set to fire based on the
	// day of the week, or an explicit date match.
	bool isDayOfWeek = (buffer[3] & 0x40) >> 6;
	uint8_t day;
	if (isDayOfWeek) {
		// Alarm set to match on day of the week
		day = bcd2bin(buffer[3] & 0x0F);
	} else {
		// Alarm set to match on day of the month
		day = bcd2bin(buffer[3] & 0x3F);
	}

	// On the first week of May 2000, the day-of-the-week number
	// matches the date number.
	return DateTime(2000, 5, day, hour, minutes, seconds);
}

DateTime DS3231::getAlarm2() {
	uint8_t buffer[4] = {DS3231_ALARM2, 0, 0, 0};
	//XXXXX i2c_dev->write_then_read(buffer, 1, buffer, 4);

	uint8_t minutes = bcd2bin(buffer[0] & 0x7F);
	// Fetching the hour assumes 24 hour time (never 12)
	// because this library exclusively stores the time
	// in 24 hour format. Note that the DS3231 supports
	// 12 hour storage, and sets bits to indicate the type
	// that is stored.
	uint8_t hour = bcd2bin(buffer[1] & 0x3F);

	// Determine if the alarm is set to fire based on the
	// day of the week, or an explicit date match.
	bool isDayOfWeek = (buffer[2] & 0x40) >> 6;
	uint8_t day;
	if (isDayOfWeek) {
		// Alarm set to match on day of the week
		day = bcd2bin(buffer[2] & 0x0F);
	} else {
		// Alarm set to match on day of the month
		day = bcd2bin(buffer[2] & 0x3F);
	}

	// On the first week of May 2000, the day-of-the-week number
	// matches the date number.
	return DateTime(2000, 5, day, hour, minutes, 0);
}

Ds3231Alarm1Mode DS3231::getAlarm1Mode() {
	uint8_t buffer[5] = {DS3231_ALARM1, 0, 0, 0, 0};
	//XXXXX i2c_dev->write_then_read(buffer, 1, buffer, 5);

	uint8_t alarm_mode = (buffer[0] & 0x80) >> 7		// A1M1 - Seconds bit
											 | (buffer[1] & 0x80) >> 6	// A1M2 - Minutes bit
											 | (buffer[2] & 0x80) >> 5	// A1M3 - Hour bit
											 | (buffer[3] & 0x80) >> 4	// A1M4 - Day/Date bit
											 | (buffer[3] & 0x40) >> 2; // DY_DT

	// Determine which mode the fetched alarm bits map to
	switch (alarm_mode) {
	case DS3231_A1_PerSecond:
	case DS3231_A1_Second:
	case DS3231_A1_Minute:
	case DS3231_A1_Hour:
	case DS3231_A1_Date:
	case DS3231_A1_Day:
		return (Ds3231Alarm1Mode)alarm_mode;
	default:
		// Default if the alarm mode cannot be read
		return DS3231_A1_Date;
	}
}

Ds3231Alarm2Mode DS3231::getAlarm2Mode() {
	uint8_t buffer[4] = {DS3231_ALARM2, 0, 0, 0};
	//XXXXX i2c_dev->write_then_read(buffer, 1, buffer, 4);

	uint8_t alarm_mode = (buffer[0] & 0x80) >> 7		// A2M2 - Minutes bit
			 | (buffer[1] & 0x80) >> 6	// A2M3 - Hour bit
			 | (buffer[2] & 0x80) >> 5	// A2M4 - Day/Date bit
			 | (buffer[2] & 0x40) >> 3; // DY_DT

	// Determine which mode the fetched alarm bits map to
	switch (alarm_mode) {
	case DS3231_A2_PerMinute:
	case DS3231_A2_Minute:
	case DS3231_A2_Hour:
	case DS3231_A2_Date:
	case DS3231_A2_Day:
		return (Ds3231Alarm2Mode)alarm_mode;
	default:
		// Default if the alarm mode cannot be read
		return DS3231_A2_Date;
	}
}

void DS3231::disableAlarm(uint8_t alarm_num) {
	uint8_t ctrl = read_register(DS3231_CONTROL);
	ctrl &= ~(1 << (alarm_num - 1));
	write_register(DS3231_CONTROL, ctrl);
}

void DS3231::clearAlarm(uint8_t alarm_num) {
	uint8_t status = read_register(DS3231_STATUSREG);
	status &= ~(0x1 << (alarm_num - 1));
	write_register(DS3231_STATUSREG, status);
}

bool DS3231::alarmFired(uint8_t alarm_num) {
	return (read_register(DS3231_STATUSREG) >> (alarm_num - 1)) & 0x1;
}

void DS3231::enable32K(void) {
	uint8_t status = read_register(DS3231_STATUSREG);
	status |= (0x1 << 0x03);
	write_register(DS3231_STATUSREG, status);
}

void DS3231::disable32K(void) {
	uint8_t status = read_register(DS3231_STATUSREG);
	status &= ~(0x1 << 0x03);
	write_register(DS3231_STATUSREG, status);
}

bool DS3231::isEnabled32K(void) {
	return (read_register(DS3231_STATUSREG) >> 0x03) & 0x01;
}
*/
