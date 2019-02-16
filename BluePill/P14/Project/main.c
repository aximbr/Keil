/**
  ******************************************************************************
  * @file    main.c 
  * @author  José P. Leitão
  * @version P14
  * @date    2019-01-13
  * @brief   Project for test an I2C OLED Display based on SH1106 controller 
	*          usig a STM32F10x MCU
  ******************************************************************************
  * @attention
  *
  * 
  * 
  *
  * <h2><center>&copy; COPYRIGHT 2013 JPL</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "drv-clock.h"
#include <stdio.h>
#include "ssd1306_tests.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
I2C_InitTypeDef hi2c1; 
GPIO_InitTypeDef GPIO_InitStruct;

/* Private function prototypes -----------------------------------------------*/
static void My_I2C1_Init(void);

/* Private functions ---------------------------------------------------------*/
void init() {
    ssd1306_TestAll();
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
//Initialize clock
	
//Initialize Systick for 1ms
SysTick_Config(SystemCoreClock / 1000);
	
//Initialize GPIO Pins for I2C communictation
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
//Initialize I2C Peripheric
//PB6 = SCL  PB7 = SDA
  My_I2C1_Init();
	




	
	return 0;
	
}

/* I2C1 init function */
static void My_I2C1_Init(void)
{

  //hi2c1.Instance = I2C1;
  hi2c1.I2C_ClockSpeed = 100000*4;
	hi2c1.I2C_Mode = I2C_Mode_I2C;
  hi2c1.I2C_DutyCycle = I2C_DutyCycle_2;
  hi2c1.I2C_OwnAddress1 = 0;
	hi2c1.I2C_Ack = I2C_Ack_Disable;
	hi2c1.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  //hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  //hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  //hi2c1.Init.OwnAddress2 = 0;
  //hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  //hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  
	I2C_Init(I2C1, &hi2c1);
}
