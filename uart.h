#ifndef _uart_h
#define _uart_h

#include <stddef.h>

void uart_init();
uint8_t uart_get();
void uart_send_char(uint8_t c);
void uart_send_data(uint8_t *data, size_t len);
void uart_print(uint8_t *text);

#endif
