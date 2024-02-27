#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

extern void i2c_enable (uint32_t speed);
extern void i2c_disable ();

bool i2c_start ();
void i2c_stop ();
bool i2c_tx (uint8_t value);
uint8_t i2c_rx_ack ();
uint8_t i2c_rx_nack ();

void i2c_write_register (uint8_t addr, uint8_t reg, uint8_t value);
uint8_t i2c_read_register (uint8_t addr, uint8_t reg);
int16_t i2c_read_register16 (uint8_t addr, uint8_t reg);
uint16_t i2c_read_registerU16 (uint8_t addr, uint8_t reg);

void i2c_write_registers (uint8_t addr, uint8_t reg, size_t len, uint8_t *buf);
void i2c_read_registers (uint8_t addr, uint8_t reg, size_t len, uint8_t *buf);

uint8_t i2c_read_register_bits (uint8_t addr, uint8_t reg, uint8_t mask);
void i2c_write_register_bits (uint8_t addr, uint8_t reg, uint8_t bits, uint8_t mask);
void i2c_set_register_bits (uint8_t addr, uint8_t reg, uint8_t bits);
void i2c_clear_register_bits (uint8_t addr, uint8_t reg, uint8_t bits);

bool i2c_read_register_bit(uint8_t addr, uint8_t reg, uint8_t bitnr);
void i2c_write_register_bit(uint8_t addr, uint8_t reg, bool value, uint8_t b);
void i2c_set_register_bit(uint8_t addr, uint8_t reg, uint8_t b);
void i2c_clear_register_bit(uint8_t addr, uint8_t reg, uint8_t b);

#endif
