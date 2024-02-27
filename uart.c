#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

#define BAUD 115200
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)

#define BUFFER_LEN 700
char buffer[BUFFER_LEN];
uint16_t head;
uint16_t tail;

void uart_init() {
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0); //transmit side of hardware
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01); //receive side of hardware
	UBRR0L = BAUD_PRESCALE; //set the baud to 9600, have to split it into the two registers
	UBRR0H = (BAUD_PRESCALE >> 8); //high end of baud register
	UCSR0B |= (1 << RXCIE0); //recieve data interrupt, makes sure we don't loose data
}

uint8_t uart_read() {
		if (tail == head)
			return 0;
		uint8_t c = buffer[tail];
		tail++;
		if (tail == BUFFER_LEN)
			tail = 0;
		return c;
}

void uart_send_char(uint8_t c) {
	while ((UCSR0A & (1 << UDRE0)) == 0);//make sure the data register is cleared
		UDR0 = c;
}

void uart_print(uint8_t *data) {
	uint8_t c;
	while ((c = *data++) != '\n')
		uart_send_char(c);
	uart_send_char('\n');
}

void uart_send_data(uint8_t *data, size_t len) {
	for (int i = 0; i < len; i++)
		uart_send_char(data[i]);
}

ISR(USART0_RX_vect) {//sets up the interrupt to recieve any data coming in
	buffer[head] = UDR0;
	head++;
	if (head == BUFFER_LEN)
		head = 0;
}
