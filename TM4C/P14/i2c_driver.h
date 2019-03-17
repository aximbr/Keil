// Header:
// File Name: i2c_driver.h
// Author: José P. Leitão
// Date: 2019-03-12

/* Header para a biblioteca i2c_driver */

#ifndef __I2C_DRIVER_H__
#define __I2C_DRIVER_H__
#include <stdint.h>
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

/******************************************************************************/
/*                                                                            */
/*                      Data Structures used by I2C Driver                    */
/*                                                                            */
/******************************************************************************/

/**
  * @brief  HAL I2C State structure definition
  */
typedef enum
{
  HAL_I2C_STATE_RESET             = 0x00,  /*!< I2C not yet initialized or disabled         */
  HAL_I2C_STATE_READY             = 0x01,  /*!< I2C initialized and ready for use           */
  HAL_I2C_STATE_BUSY              = 0x02,  /*!< I2C internal process is ongoing             */
  HAL_I2C_STATE_BUSY_TX           = 0x12,  /*!< Data Transmission process is ongoing        */
  HAL_I2C_STATE_BUSY_RX           = 0x22,  /*!< Data Reception process is ongoing           */
  HAL_I2C_STATE_MEM_BUSY_TX       = 0x32,  /*!< Memory Data Transmission process is ongoing */
  HAL_I2C_STATE_MEM_BUSY_RX       = 0x42,  /*!< Memory Data Reception process is ongoing    */
  HAL_I2C_STATE_TIMEOUT           = 0x03,  /*!< I2C timeout state                           */
  HAL_I2C_STATE_ERROR             = 0x04   /*!< I2C error state                             */
}hal_i2c_state_t;


typedef enum
{
	MASTER_STANDARD                = 0x00,  /*!< Master at Standard Speed  (100 kbps)        */
	MASTER_FAST_MODE               = 0x01,  /*!< Master at Fast Speed      (400 kbps)        */
	MASTER_FAST_PLUS               = 0x02,  /*!< Master at Fast Speed Plus (1 Mbps)          */
	MASTER_HIGH_SPEED              = 0x03,  /*!< Master at High Speed      (3.3 Mbps)        */
}hal_i2c_speed_t;


/**
  * @brief  I2C Configuration Structure definition
  */
typedef struct
{
	uint32_t ClockSpeed;       /*!< Specifies the clock frequency.
													This parameter must be set to a value lower than 400kHz */

	hal_i2c_speed_t Mode_Speed;        /*!< Specifies the I2C speed when operating as Master.
													This parameter can be a value of @ref hal_i2c_speed_t */

	uint32_t OwnAddress;      /*!< Specifies the first device own address.
													This parameter must be a 7-bit. */
	uint32_t ack_enable; 

	uint8_t master;
	
}i2c_init_t;


/** 
  * @brief  I2C handle Structure definition
  */

typedef struct
{
	I2C0_Type                  *Instance;  /*!< I2C registers base address     */

	i2c_init_t                 Init;       /*!< I2C communication parameters   */

	uint8_t                    *pBuffPtr;  /*!< Pointer to I2C transfer buffer */

	uint32_t                   XferSize;   /*!< I2C transfer size              */

	__IO uint32_t              XferCount;  /*!< I2C transfer counter           */

	hal_i2c_state_t            State;      /*!< I2C communication state        */
	uint32_t                   ErrorCode;     

}i2c_handle_t;

#define  RESET  0 
#define  SET    !RESET

/** 
  * @brief  HAL Status structures definition  
  */  
typedef enum 
{
  HAL_OK             = 0x00,
  HAL_ERROR          = 0x01,
  HAL_BUSY           = 0x02,
  HAL_TIMEOUT        = 0x03,
	HAL_I2C_ERROR_BERR = 0x04,
	HAL_I2C_ERROR_ARLO = 0x05,
	HAL_I2C_ERROR_OVR  = 0x06,
	HAL_I2C_ERROR_AF   = 0x07,
	HAL_I2C_ERROR_NONE = 0x08
} HAL_StatusTypeDef;

	
/******************************************************************************/
/*                                                                            */
/*                                 MACROS                                     */
/*                                                                            */
/******************************************************************************/

#define UNUSED(x) ((void)(x))
	
/* Macros to Enable Clock for diffrent I2C devices */

#define _HAL_RCGC_I2C0_CLK_ENABLE()       ( SYSCTL->RCGC1 |=  (1 << 12) )
#define _HAL_RCGC_I2C1_CLK_ENABLE()       ( SYSCTL->RCGC1 |=  (1 << 14) )

/* Macros to Enable Clock for diffrent GPIO devices */
#define _HAL_RCGC_GPIOB_CLK_ENABLE()      ( SYSCTL->RCGC2 |=  (1 << 1) )
#define _HAL_RCGC_GPIOA_CLK_ENABLE()      ( SYSCTL->RCGC2 |=  (1 << 0) )

/******************************************************************************/
/*                                                                            */
/*                      Driver exposed APIs                                   */
/*                                                                            */
/******************************************************************************/


void hal_i2c_init(i2c_handle_t *handle);


HAL_StatusTypeDef hal_i2c_mem_write(i2c_handle_t *handle, uint8_t slave_address, uint8_t mem_address, uint8_t mem_len, uint8_t *buffer, uint32_t len, uint32_t Timeout);


#endif //__I2C_DRIVER_H__
