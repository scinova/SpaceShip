#ifndef system_h
#define system_h

#include <stdint.h>
#include <util/atomic.h>

#define BIT(x) ((uint8_t) 1U << (x))
#define REG(x) ((volatile uint8_t *) (x))

uint64_t uptime_us();
uint32_t uptime_ms();

inline uint32_t millis() {
	return uptime_ms();
}

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

#endif
