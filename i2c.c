#include "i2c.h"
#include <util/twi.h>

void i2c_enable(uint32_t speed) {
	DDRC &= 0b11001111; /* make SDA and SCL inputs */
	PORTC &= 0b11001111;/* disable internal pullups on SDA and SCL */
	TWSR = 0; // no prescale
	TWBR = (F_CPU / speed - 16) / 2;
	TWCR = _BV(TWEN);
}

void i2c_disable () {
	TWCR &= ~_BV(TWEN);
}

static void i2c_wait () {
	while (!(TWCR & _BV(TWINT)));
}

bool i2c_start () {
	TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTA);
	i2c_wait ();
	return (TW_STATUS == TW_START || TW_STATUS == TW_REP_START);
}

void i2c_stop () {
	TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTO);
	while (TWCR & _BV(TWSTO));
}

bool i2c_tx (uint8_t byte) {
	TWDR = byte;
	TWCR = _BV(TWEN) | _BV(TWINT);
	i2c_wait();
	return (TW_STATUS == TW_MT_DATA_ACK);
}

uint8_t i2c_rx_ack () {
	TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);
	i2c_wait();
	return TWDR;
//	return (TW_STATUS == TW_MR_DATA_ACK);
}

uint8_t i2c_rx_nack () {
	TWCR = _BV(TWEN) | _BV(TWINT);
	i2c_wait();
	return TWDR;
//	return (TW_STATUS == TW_MR_DATA_NACK);
}

void i2c_write_registers(uint8_t addr, uint8_t reg, size_t len, uint8_t *buf) {
	i2c_start();
	i2c_tx((addr << 1) | TW_WRITE);
	i2c_tx(reg);
	for(; len > 0; len--)
		i2c_tx(*buf++);
	i2c_stop();
}

void i2c_write_register(uint8_t addr, uint8_t reg, uint8_t value) {
	i2c_start();
	i2c_tx((addr << 1) | TW_WRITE);
	i2c_tx(reg);
	i2c_tx(value);
	i2c_stop();
}

void i2c_read_registers(uint8_t addr, uint8_t reg, size_t len, uint8_t *buf) {
	i2c_start();
	i2c_tx((addr << 1) | TW_WRITE);
	i2c_tx(reg);
 	i2c_start();
	i2c_tx((addr << 1) | TW_READ);
	for(; len > 1; len--)
		*(buf++) = i2c_rx_ack();
	*(buf++) = i2c_rx_nack();
	i2c_stop();
}

uint8_t i2c_read_register(uint8_t addr, uint8_t reg) {
	i2c_start();
	i2c_tx((addr << 1) | TW_WRITE);
	i2c_tx(reg);
 	i2c_start();
	i2c_tx((addr << 1) | TW_READ);
	uint8_t value = i2c_rx_nack();
	i2c_stop();
	return value;
}

int16_t i2c_read_register16(uint8_t addr, uint8_t reg) {
	i2c_start();
	i2c_tx((addr << 1) | TW_WRITE);
	i2c_tx(reg);
 	i2c_start();
	i2c_tx((addr << 1) | TW_READ);
	uint8_t vh = i2c_rx_ack();
	uint8_t vl = i2c_rx_nack();
	int16_t v = (uint16_t)vh << 8 | vl;
	i2c_stop();
	return v;
}

uint16_t i2c_read_registerU16(uint8_t addr, uint8_t reg) {
	return (uint16_t)i2c_read_register16(addr, reg);
}

uint8_t i2c_read_register_bits(uint8_t addr, uint8_t reg, uint8_t mask) {
	return i2c_read_register(addr, reg) & ~mask;
}

void i2c_write_register_bits(uint8_t addr, uint8_t reg, uint8_t bits, uint8_t mask) {
	uint8_t v = i2c_read_register(addr, reg);
	i2c_write_register(addr, reg, (v & ~mask) | bits);
}

void i2c_set_register_bits(uint8_t addr, uint8_t reg, uint8_t bits) {
	uint8_t v = i2c_read_register(addr, reg);
	i2c_write_register(addr, reg, v | bits);
}

void i2c_clear_register_bits(uint8_t addr, uint8_t reg, uint8_t bits) {
	uint8_t v = i2c_read_register(addr, reg);
	i2c_write_register(addr, reg, v & ~bits);
}

void i2c_set_register_bit(uint8_t addr, uint8_t reg, uint8_t bitnr) {
	uint8_t v = i2c_read_register(addr, reg);
	i2c_write_register(addr, reg, v | (1 << bitnr));
}

void i2c_clear_register_bit(uint8_t addr, uint8_t reg, uint8_t bitnr) {
	uint8_t v = i2c_read_register(addr, reg);
	i2c_write_register(addr, reg, v & ~(1 << bitnr));
}

bool i2c_read_register_bit(uint8_t addr, uint8_t reg, uint8_t bitnr) {
	uint8_t v = i2c_read_register(addr, reg);
	return (v & (1 << bitnr)) != 0;
}
