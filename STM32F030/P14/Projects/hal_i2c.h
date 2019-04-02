/*Meu header para o hal_i2c */
#ifndef __HAL_I2C_H__
#define __HAL_I2C_H__

#include "stm32f0xx.h"
#include  <stdint.h>

void i2c_mem_write(I2C_TypeDef* I2Cx, uint8_t slave_address, uint8_t mem_address, uint8_t *buffer, uint32_t len);

#endif // __HAL_I2C_H__
