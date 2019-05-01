// Header:
// File Name: hal_i2c.h
// Author: Jose P. Leitao
// Date: 2019-04-13

#ifndef __HAL_I2C_H__
#define __HAL_I2C_H__
#include <stdio.h>
#include "msp432.h"

#define I2C_SET_DATA_RATE_1MBPS                                 1000000UL
#define I2C_SET_DATA_RATE_400KBPS                                400000UL
#define I2C_SET_DATA_RATE_100KBPS                                100000UL


/******************************************************************************/
/*                                                                            */
/*                                I2C                                         */
/*                        Register Bit Defininitions                          */
/******************************************************************************/



/*******************  Bit definition for UCBxCTLW0 register  **********/
#define I2C_REG_CTLW0_UCMST                ((uint16_t)1 << 11)
#define I2C_REG_CTLW0_I2CMODE              EUSCI_B_CTLW0_MODE_3
#define I2C_REG_CTLW0_UCTR                 ((uint16_t)1 << 4)
#define I2C_REG_CTLW0_UCTXNACK             ((uint16_t)1 << 3)
#define I2C_REG_CTLW0_UCTXSTP              ((uint16_t)1 << 2)
#define I2C_REG_CTLW0_UCTXSTT              ((uint16_t)1 << 1)
#define I2C_REG_CTLW0_RST                  ((uint16_t)1 << 0)

/*******************  Bit definition for UCBxSTATW register  **********/
#define I2C_REG_STATW_UCBBUSY              ((uint32_t)1 << 4)

/*******************  Bit definition for UCBxIFX register  **********/
#define I2C_REG_IFG_UCTXIFG0               ((uint32_t)1 << 1)
#define I2C_REG_IFG_UCRXIFG0               ((uint32_t)1 << 0)

/* Public API */
void hal_i2c_init(EUSCI_B_Type *i2cx);
uint8_t hal_i2c_mem_read(EUSCI_B_Type *i2cx, uint16_t dev_add, uint8_t mem_add, uint8_t *value, uint8_t len);
uint8_t hal_i2c_mem_write(EUSCI_B_Type *i2cx, uint16_t dev_add, uint8_t mem_add, uint8_t *value, uint8_t len);

#endif //__HAL_I2C_H__
