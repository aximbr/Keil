/**
  Projeto 15 - conexão de display OLED baseado no SSD1306 via I2C
	utiliza a biblioteca padrão STM32F4xxx
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdint.h>
#include <stdbool.h>
#include "hal_i2c.h"
#include "ssd1306.h"
#include "HMC5883L.h"
#include "conv_to_string.h"

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static __IO uint32_t uwTimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/
static void Delay(__IO uint32_t nTime);
 
 
/* Global variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/



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
	

/* Insert system clock initialization code here. */
	
	
/*Systick initalization */
	/* SysTick end of count event each 1ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000UL);
  
 

/* HW initialization */
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
		
	  Delay(3000);
	
	
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

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  uwTimingDelay = nTime;

  while(uwTimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
  }
}


