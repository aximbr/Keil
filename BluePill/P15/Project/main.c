/**
  ******************************************************************************
  * @file    main.c 
  * @author  José P. Leitão
  * @version V1.0
  * @date    2019-04-11
  * @brief   P15
  ******************************************************************************
  * @attention
  *
  * Conenctar o display OLED baseado no SSD1306 e o BMP280 via I2C. Tarefas contro-
	* ladas pelo FreeRTOS.
  *
  * <h2><center>&copy; COPYRIGHT 2019 JPL</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <stdio.h>
#include "hal_i2c.h"
#include "ssd1306.h"
#include "HMC5883L.h"
#include "conv_to_string.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint32_t tickcount;


/* Private function prototypes -----------------------------------------------*/
void my_delay(uint32_t d_ms);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	char zz[10];
	int16_t leitura[3];

//Clock Initialization
	
//Modules initialization
	SysTick_Config(SystemCoreClock/1000UL); //init SysTick for 1 ms interrupt
	
	hal_i2c_init(I2C1);
	ssd1306_Init();
  HMC5883L_Initialize();
	
	
	
		
	  ssd1306_Fill(Black);
	  ssd1306_SetCursor(20, 30);
		if ( HMC5883L_TestConnection() )
		   ssd1306_WriteString("HMC init OK!", Font_7x10, White);
		else
			  ssd1306_WriteString("HMC init NOK!", Font_7x10, White);
	  ssd1306_UpdateScreen();
		
	my_delay(3000);
	
	
	
	while(1){
		
		HMC5883L_GetHeading( leitura );
		
		ssd1306_Fill(Black);
	  ssd1306_DrawBox(2, 2, 129, 62);
	  ssd1306_DrawHLine(2, 129, 18);
	  ssd1306_SetCursor(20, 8);
	  ssd1306_WriteString("HMC5883L by JPL", Font_7x10, White);
		ssd1306_SetCursor(5, 30);
		ssd1306_WriteString("X : ", Font_7x10, White);
		ssd1306_SetCursor(33, 30);
		ssd1306_WriteString(itoa(leitura[0], zz, 10), Font_7x10, White);
		ssd1306_SetCursor(5, 40);
		ssd1306_WriteString("Y : ", Font_7x10, White);
		ssd1306_SetCursor(33, 40);
		ssd1306_WriteString(itoa(leitura[1], zz, 10), Font_7x10, White);
		ssd1306_SetCursor(5, 50);
		ssd1306_WriteString("Z : ", Font_7x10, White);
		ssd1306_SetCursor(33, 50);
		ssd1306_WriteString(itoa(leitura[2], zz, 10), Font_7x10, White);
		ssd1306_UpdateScreen();
				
	}
	
}

//implement my_delay function
void my_delay(uint32_t delay_ms){
	tickcount = 0;
	while (tickcount < delay_ms);
}

void SysTick_Handler(void){
	tickcount++;
}
