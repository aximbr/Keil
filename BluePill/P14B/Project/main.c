/**
  ******************************************************************************
  * @file    main.c 
  * @author  José P. Leitão
  * @version V1.0
  * @date    2019-01-01
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  
/*Defines --------------------------------------------------------------------*/



/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <stdio.h>
#include "my_tick.h"
#include "hal_i2c.h"
#include "ssd1306.h"

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
i2c_handle_t hi2c1; 
NVIC_InitTypeDef  NVIC_InitStructure;

/* Private function prototypes -----------------------------------------------*/
//void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_I2C1_Init(void);
void NVIC_Configuration(void);
void I2C1_ER_IRQHandler(void);
void I2C1_EV_IRQHandler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{

/*Hardware init */
	MX_GPIO_Init();
  MX_I2C1_Init();
	InitMyTick();
	
/* NVIC configuration ------------------------------------------------------*/
  NVIC_Configuration();
	
	ssd1306_Init();
	
	/* Infinite loop */
  
  while (1)
  {
    ssd1306_Fill(Black);
		for(int j=0; j<63; j +=10){
		   ssd1306_SetCursor(23, j);
       ssd1306_WriteString("Hello World!", Font_7x10, White);
       ssd1306_UpdateScreen();
		   }
	}
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef gpio_init;
	
	/* GPIO Ports Clock Enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/*pins PB6 (SCL1) PB7 (SDA1) */
	gpio_init.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpio_init.GPIO_Mode = GPIO_Mode_AF_OD;
	gpio_init.GPIO_Speed = GPIO_Speed_10MHz;
	
  
	GPIO_Init(GPIOB, &gpio_init);
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
void MX_I2C1_Init(void)
{
  _HAL_RCC_I2C1_CLK_ENABLE();
	
	hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_FM_DUTY_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRMODE_7BIT;
  hi2c1.Init.DualAddressMode = 0;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = 0;
  hi2c1.Init.NoStretchMode = I2C_ENABLE_CLK_STRETCH;
	
  //Enable Interrrupt Error and Event */
	//NVIC_EnableIRQ(I2C1_ER_IRQn);
	//NVIC_EnableIRQ(I2C1_EV_IRQn);
	
	hal_i2c_init(&hi2c1);
		
	hi2c1.Instance->CR1 |= I2C_REG_CR1_ENABLE_I2C;
  
}

/*Interrupt Handlers */
void I2C1_ER_IRQHandler(void)
{
	HAL_I2C_ER_IRQHandler(&hi2c1);
}

void I2C1_EV_IRQHandler(void)
{
	HAL_I2C_EV_IRQHandler(&hi2c1);
}

/**
  * @brief  Configures NVIC and Vector Table base location.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  /* Configure and enable I2C1 event interrupt -------------------------------*/
  NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

void SysTick_Handler(void){
	MyTick();
}

