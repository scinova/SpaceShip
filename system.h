#ifndef system_h
#define system_h

#include <stdint.h>
#include <util/atomic.h>

#define BIT(x) ((uint8_t) 1U << (x))
#define REG(x) ((volatile uint8_t *) (x))

uint64_t uptime_us();
uint32_t uptime_ms();

static inline void spin(volatile uint32_t count) {
	while (count--)
		__asm__ ("nop");
}

static inline void delay_us(unsigned long us) {
	uint64_t until = uptime_us() + us;
	while (uptime_us() < until)
		spin(1);
}

static inline void delay_ms(unsigned long ms) {
	delay_us(ms * 1000);
}

inline void write_pwm0(uint8_t value) { OCR0A = value; }
inline void write_pwm1(uint8_t value) { OCR0B = value; }
inline void write_pwm2(uint8_t value) { OCR2A = value; }
inline void write_pwm3(uint8_t value) { OCR2B = value; }

#endif
