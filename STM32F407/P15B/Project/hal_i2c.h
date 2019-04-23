// Header:
// File Name: hal_i2c.h
// Author: Jose P. Leitao
// Date: 2019-04-13

#ifndef __HAL_I2C_H__
#define __HAL_I2C_H__
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

void hal_i2c_init(I2C_TypeDef* i2cx);

uint8_t hal_i2c_mem_read(I2C_TypeDef* i2cx, uint16_t dev_add, uint8_t mem_add, uint8_t *value, uint8_t len);

uint8_t hal_i2c_mem_read16(I2C_TypeDef* i2cx, uint16_t dev_add, uint8_t mem_add, uint16_t *value);

uint8_t hal_i2c_mem_write(I2C_TypeDef* i2cx, uint16_t dev_add, uint8_t mem_add, uint8_t *value, uint8_t len);

#endif //__HAL_I2C_H__
