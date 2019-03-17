// Header:
// File Name: main.c
// Author: José P. Leitão
// Date: 2019-03-12

/* projeto 14 - criar um driver I2C para display OLED baseado no SD1306 */

/*Defines --------------------------------------------------------------------*/



/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "tm4c123.h"
#include "i2c_driver.h"
#include "my_tick.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
i2c_handle_t hi2c0; 


/* Private function prototypes -----------------------------------------------*/
//void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_I2C0_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{

/*Hardware init */
	//MX_GPIO_Init();
  MX_I2C0_Init();
	InitMyTick();
	
/* Display configuration ------------------------------------------------------*/
  
	
	ssd1306_Init();
	
	/* Infinite loop */
  
  while (1)
  {
    ssd1306_Fill(Black);
		for(int j=0; j<63; j +=10){
		   ssd1306_SetCursor(2, j);
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
  //GPIO_InitTypeDef gpio_init;
	
	/* GPIO Ports Clock Enable */
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/*pins PB6 (SCL1) PB7 (SDA1) */
	//gpio_init.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	//gpio_init.GPIO_Mode = GPIO_Mode_AF_OD;
	//gpio_init.GPIO_Speed = GPIO_Speed_10MHz;
	
  
	//GPIO_Init(GPIOB, &gpio_init);
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
void MX_I2C0_Init(void)
{
  //_HAL_RCC_I2C1_CLK_ENABLE();
	
	hi2c0.Instance = I2C0;
  hi2c0.Init.ClockSpeed = 100000;
  
	
  //Enable Interrrupt Error and Event */
	//NVIC_EnableIRQ(I2C1_ER_IRQn);
	//NVIC_EnableIRQ(I2C1_EV_IRQn);
	
	hal_i2c_init(&hi2c0);
		
	//hi2c0.Instance->CR1 |= I2C_REG_CR1_ENABLE_I2C;
  
}


void SysTick_Handler(void)
{
	MyTick();
}

