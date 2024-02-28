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
	PORTF |= 0xff; // pullups
	PORTK |= 0xff;

	DDRB |= _BV(DDB7); // OC0A pin13
	DDRB |= _BV(DDB4); // OC2A pin10
	DDRH |= _BV(DDH6); // OC2B pin9

	DDRD |= _BV(DDD0); // pin12

	TCCR2A |= _BV(COM2A1) | _BV(COM2B1) | _BV(WGM20) | _BV(WGM21); //fast PWM mode, non-inverting
	TCCR2B |= _BV(CS20) | _BV(CS21); //clk/64 prescaler

	TCCR0A |= _BV(COM0A1) | _BV(WGM00) | _BV(WGM01);
	TCCR0B |= _BV(CS00) | _BV(CS01); //clk/64 prescaler
	TIMSK0 |= _BV(TOIE0); //enable overflow interupt
	sei();
}
