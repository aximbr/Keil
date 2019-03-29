// Header:
// File Name: i2c_driver.h
// Author: José P. Leitão
// Date: 2019-03-12

/* Header para a biblioteca i2c_driver */

#ifndef __I2C_DRIVER_H__
#define __I2C_DRIVER_H__
#include <stdint.h>
#include "msp432.h"


void hal_i2c_mem_write(uint32_t i2cx, uint8_t slave_address, uint8_t mem_address, uint8_t *buffer, uint32_t len);

#endif //__I2C_DRIVER_H__
