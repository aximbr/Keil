/**
  ******************************************************************************
  * @file    main.c 
  * @author  José P. Leitão
  * @version V1.0
  * @date    2019-04-11
  * @brief   P14
  ******************************************************************************
  * @attention
  *
  * Conenctar o display OLED baseado no SSD1306 via I2C
	* 
  *
  * <h2><center>&copy; COPYRIGHT 2019 JPLs</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <stdio.h>
#include "ssd1306.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
 

/* Private function prototypes -----------------------------------------------*/
void i2c_init(I2C_TypeDef* i2cx);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{

//Clock Initialization
	
//Modules initialization
	i2c_init(I2C1);
	ssd1306_Init();
	
	while(1){
		ssd1306_Fill(Black);
		for(int j=0; j<63; j +=10){
		   ssd1306_SetCursor(23, j); //centraliza a frase no meio
       ssd1306_WriteString("Hello World!", Font_7x10, White);
       ssd1306_UpdateScreen();
		   }
	}
	
}

void i2c_init(I2C_TypeDef* i2cx){
	I2C_InitTypeDef MyI2C_init;
	GPIO_InitTypeDef gpio_init;
	
	/* GPIO Ports Clock Enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/*pins PB6 (SCL1) PB7 (SDA1) */
	gpio_init.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpio_init.GPIO_Mode = GPIO_Mode_AF_OD;
	gpio_init.GPIO_Speed = GPIO_Speed_10MHz;
		  
	GPIO_Init(GPIOB, &gpio_init);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	I2C_StructInit(&MyI2C_init); //initialize with default values
	
	MyI2C_init.I2C_ClockSpeed = 100000;  //standart mode
	
	I2C_Init(i2cx, &MyI2C_init);
		
	
	
}

