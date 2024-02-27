#ifndef _ds3231_h
#define _ds3231_h

#include "i2c.h"
#include <time.h>

typedef enum {
	DS3231_SquareWave1Hz  = 0,
	DS3231_SquareWave1kHz = 1,
	DS3231_SquareWave4kHz = 2,
	DS3231_SquareWave8kHz = 3
} ds3231_squarewave_frequency_t ;

void ds3231_init();
void ds3231_set_time(struct tm *t);
void ds3231_read_time(struct tm *t);
float read_temperature();

void ds3231_set_squarewave_frequency(ds3231_squarewave_frequency_t frequency);
void ds3231_enable_squarewave();
void ds3231_disable_squarewave();

void ds3231_start_oscillator();
void ds3231_stop_oscillator();
uint8_t ds3231_year();
uint16_t ds3231_month();
uint8_t ds3231_hour();
uint8_t ds3231_minute();
uint8_t ds3231_second();

//	bool setAlarm1(const DateTime &dt, Ds3231Alarm1Mode alarm_mode);
//	bool setAlarm2(const DateTime &dt, Ds3231Alarm2Mode alarm_mode);
//	DateTime getAlarm1();
//	DateTime getAlarm2();
//	Ds3231Alarm1Mode getAlarm1Mode();
//	Ds3231Alarm2Mode getAlarm2Mode();
//	void disableAlarm(uint8_t alarm_num);
//	void clearAlarm(uint8_t alarm_num);
//	bool alarmFired(uint8_t alarm_num);
//	void enable32K(void);
//	void disable32K(void);
//	bool isEnabled32K(void);

///enum ds3231_alarm1_modet {
//	DS3231_A1_PerSecond = 0x0F, /**< Alarm once per second */
//	DS3231_A1_Second = 0x0E,		/**< Alarm when seconds match */
//	DS3231_A1_Minute = 0x0C,		/**< Alarm when minutes and seconds match */
//	DS3231_A1_Hour = 0x08,			/**< Alarm when hours, minutes and seconds match */
//	DS3231_A1_Date = 0x00,			/**< Alarm when date (day of month), hours, minutes and seconds match */
//	DS3231_A1_Day = 0x10				/**< Alarm when day (day of week), hours, minutes and seconds match */
//};

//*enum Ds3231Alarm2Mode {
//	DS3231_A2_PerMinute = 0x7, /**< Alarm once per minute (whenever seconds are 0) */
//	DS3231_A2_Minute = 0x6,		/**< Alarm when minutes match */
//	DS3231_A2_Hour = 0x4,			/**< Alarm when hours and minutes match */
//	DS3231_A2_Date = 0x0,			/**< Alarm when date (day of month), hours and minutes match */
//	DS3231_A2_Day = 0x8				/**< Alarm when day (day of week), hoursand minutes match */
//};

#endif
