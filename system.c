#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "system.h"

static volatile unsigned long _systick = 0;

ISR(TIMER0_OVF_vect) {
	_systick++;
}

uint64_t uptime_us() {
	return _systick * 1000;
}

uint32_t uptime_ms() {
	return _systick;
}

void system_init() {
	// TIMER 0
	TCCR0A |= _BV(COM0A1) | _BV(WGM00) | _BV(WGM01);
	TCCR0B |= _BV(CS00) | _BV(CS01); //clk/64
	TIMSK0 |= _BV(TOIE0); //enable overflow interupt
	// TIMER 2
	TCCR2A |= _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20) | _BV(WGM21); //fast PWM mode, non-inverting
	TCCR2B |= _BV(CS20) | _BV(CS21); //clk/64
	// PWM
#if defined(__AVR_ATmega2560__)
	DDRB |= _BV(7); // OC0A pin13
	DDRB |= _BV(4); // OC2A pin10
	DDRH |= _BV(6); // OC2B pin9
#elif defined(__AVR_ATmega328__)
	DDRD |= _BV(6); // OC0A PD6 pin6
	DDRD |= _BV(5); // OC0B PD5 pin5

	DDRB |= _BV(3); // OC2A PB3 pin11
	DDRD |= _BV(3); // OC2B PD2 pin3
#endif
#if defined(__AVR_ATmega2560__)
	PORTF |= 0xff; // pullups
	PORTK |= 0xff;
#endif
	DDRD |= _BV(0); // pin12
	sei();
}
