// Header:
// File Name: hal_i2c.h
// Author: Jose P. Leitao
// Date: 2019-04-13

#ifndef __HAL_I2C_H__
#define __HAL_I2C_H__
#include <stdio.h>
#include "tm4c123.h"

/******************************************************************************/
/*                                                                            */
/*                                I2C                                         */
/*                        Register Bit Defininitions                          */
/******************************************************************************/

/*******************  Bit definition for I2C_MSA register  ********************/
//Master Slave Address (bits 7:1)
//Receive or Send bit (bit 0) 0=TX 1=RX


/*******************  Bit definition for I2C_MCS register  (Read Only)**********/
#define I2C_REG_MCS_CLKTO             ((uint32_t)1 << 7)
#define I2C_REG_MCS_BUSBSY            ((uint32_t)1 << 6)
#define I2C_REG_MCS_IDLE              ((uint32_t)1 << 5)
#define I2C_REG_MCS_ARBLST            ((uint32_t)1 << 4)
#define I2C_REG_MCS_DATACK            ((uint32_t)1 << 3)
#define I2C_REG_MCS_ADRACK            ((uint32_t)1 << 2)
#define I2C_REG_MCS_ERROR             ((uint32_t)1 << 1)
#define I2C_REG_MCS_BUSY              ((uint32_t)1 << 0)

/*******************  Bit definition for I2C_MCS register  (Write Only)*********/
#define I2C_REG_MCS_HS                ((uint32_t)1 << 4)
#define I2C_REG_MCS_ACK               ((uint32_t)1 << 3)
#define I2C_REG_MCS_STOP              ((uint32_t)1 << 2)
#define I2C_REG_MCS_START             ((uint32_t)1 << 1)
#define I2C_REG_MCS_RUN               ((uint32_t)1 << 0)

/*******************  Bit definition for I2C_MDR register  ********************/
//Data register (bits 7:0)

/*******************  Bit definition for I2C_MTPR register  ********************/
#define I2C_REG_MTPR_HS              ((uint32_t)1 << 7)
//TPR Value (bits 6:0)

/*******************  Bit definition for I2C_MIMR register  ********************/
#define I2C_REG_MIMR_CLKIM           ((uint32_t)1 << 1)
#define I2C_REG_MIMR_IM              ((uint32_t)1 << 0)

/*******************  Bit definition for I2C_MRIS register  ********************/
#define I2C_REG_MRIS_CLKRIS          ((uint32_t)1 << 1)
#define I2C_REG_MRIS_RIS             ((uint32_t)1 << 0)

/*******************  Bit definition for I2C_MMIS register  ********************/
#define I2C_REG_MMIS_CLKMIS          ((uint32_t)1 << 1)
#define I2C_REG_MMIS_MIS             ((uint32_t)1 << 0)


/*******************  Bit definition for I2C_MICR register  ********************/
#define I2C_REG_MICRS_CLKIC          ((uint32_t)1 << 1)
#define I2C_REG_MICR_IC              ((uint32_t)1 << 0)

/*******************  Bit definition for I2C_MCR register  ********************/
#define I2C_REG_MCR_GFE              ((uint32_t)1 << 6)
#define I2C_REG_MCR_SFE              ((uint32_t)1 << 5)
#define I2C_REG_MCR_MFE              ((uint32_t)1 << 4)
#define I2C_REG_MCR_LPBK             ((uint32_t)1 << 0)

/*******************  Bit definition for I2C_MCLKOCNT register  ********************/
//Master Clock Low Timeout Count (bits 7:0)

/*******************  Bit definition for I2C_MBMON register  ********************/
#define I2C_REG_MBMON_SDA            ((uint32_t)1 << 1)
#define I2C_REG_MBMON_SCL            ((uint32_t)1 << 0)

/*******************  Bit definition for I2C_MCR2 register  ********************/
//Master Configuration 2 (bits 6:4)

/*******************  Bit definition for I2C_CPP register  ********************/
#define I2C_REG_CPP_HS               ((uint32_t)1 << 0)

/*******************  Bit definition for I2C_CPC register  ********************/
#define I2C_REG_CPC_HS               ((uint32_t)1 << 0)


/* Macros to Enable Clock for diffrent I2C devices */

#define _HAL_RCGC_I2C0_CLK_ENABLE()       ( SYSCTL->RCGC1 |=  (1 << 12) )
#define _HAL_RCGC_I2C1_CLK_ENABLE()       ( SYSCTL->RCGC1 |=  (1 << 14) )

/* Macros to Enable Clock for diffrent GPIO devices */
#define _HAL_RCGC_GPIOB_CLK_ENABLE()      ( SYSCTL->RCGC2 |=  (1 << 1) )
#define _HAL_RCGC_GPIOA_CLK_ENABLE()      ( SYSCTL->RCGC2 |=  (1 << 0) )

/* Public API */
void hal_i2c_init(I2C0_Type* i2cx);
uint8_t hal_i2c_mem_read(I2C0_Type* i2cx, uint16_t dev_add, uint8_t mem_add, uint8_t *value, uint8_t len);
uint8_t hal_i2c_mem_write(I2C0_Type* i2cx, uint16_t dev_add, uint8_t mem_add, uint8_t *value, uint8_t len);

#endif //__HAL_I2C_H__
