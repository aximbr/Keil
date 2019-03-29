// Header:
// File Name: i2c_driver.h
// Author: José P. Leitão
// Date: 2019-03-26

/* Header para a biblioteca i2c_driver */

#ifndef __I2C_DRIVER_H__
#define __I2C_DRIVER_H__

#include "MKL25Z4.h"
#include "stdbool.h"
 
#define ALT0 0
#define ALT1 1
#define ALT2 2
#define ALT3 3
#define MULT0 0
#define MULT1 1
#define MULT2 2
#define I2C_WRITE	0	// Master write
#define I2C_READ	1	// Master read
 
bool i2c_Init(I2C_MemMapPtr i2c, uint8_t alt, uint8_t mult, uint8_t icr);
void i2c_DisableAck(I2C_MemMapPtr i2c);
void i2c_EnableAck(I2C_MemMapPtr i2c);
void i2c_RepeatedStart(I2C_MemMapPtr i2c);
void i2c_EnterRxMode(I2C_MemMapPtr i2c);
void i2c_Start(I2C_MemMapPtr i2c);
void i2c_Stop(I2C_MemMapPtr i2c);
void i2c_Wait(I2C_MemMapPtr i2c);
void i2c_WriteByte(I2C_MemMapPtr i2c,uint8_t data);
 
void i2c_WriteRegister(I2C_MemMapPtr i2c,uint8_t SlaveAddress, uint8_t RegisterAddress, uint8_t data);
 
uint8_t i2c_ReadByte(I2C_MemMapPtr i2c);
 
uint8_t i2c_ReadRegister(I2C_MemMapPtr i2c,uint8_t SlaveAddress,uint8_t RegisterAddress);
 
uint8_t i2c_ReadMultRegister(I2C_MemMapPtr i2c,uint8_t SlaveAddress, uint8_t RegisterAddress,uint8_t n_data, uint8_t *res);
 
void i2c_WriteMultRegister(I2C_MemMapPtr i2c,uint8_t SlaveAddress, uint8_t RegisterAddress, uint8_t n_data, uint8_t *data);
 
void delay(uint32_t t);


void i2c_mem_write(I2C_MemMapPtr i2c, uint8_t SlaveAddress, uint8_t mem_address, uint8_t *buffer, uint32_t len);

#endif //__I2C_DRIVER_H__
