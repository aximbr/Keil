/*
 P15 - conexão ao display OLED baseado no SSD1136 usando I2C
 * Author: Jose P. Leitão
 * 2019-04-23
 * Keil v5                                                        */

/* Includes *******************************************************/
#include "MKL25Z4.h"
#include "mcg.h"
#include <stdint.h>
#include "ssd1306.h"
#include "my_tick.h"
#include "HMC5883L.h"
#include "conv_to_string.h"



/* Global variables */

/* Defines ********************************************************/

#define ICR   0x0D    //Bus @ 48Mhz I2C baud rate 1MHz, see datasheet at pp706


/* Private macro -------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

 
/* Global variables ----------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
void clock_init(void);




/* Private functions ---------------------------------------------------------*/
/* Systick Handle - just increment our tick counter every 1 ms */
void SysTick_Handler(void) 
{
	MyTick();
}




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
  //clock_init();
	pll_init(8000000, LOW_POWER ,CRYSTAL, 4, 24, MCGOUT);
	
	/* initialise  Systick  for 1 ms @ 48MHz clock */
	InitMyTick();
	
	/* Configure I2C module */
	//Select alternate 1 for I2C0, Multipler = 1 and ICR = 0x0D => baud rate 1MHz @bus clock 48MHz
	i2c_Init(SSD1306_I2C_PORT, ALT1, MULT0, ICR);
	
			
	/* Display configuration ------------------------------------------------------*/
  ssd1306_Init();

	/* HMC5883 configuration ------------------------------------------------------*/
	HMC5883L_Initialize();
	
  
	  ssd1306_Fill(Black);
	  ssd1306_SetCursor(20, 30);
		if ( HMC5883L_TestConnection() )
		   ssd1306_WriteString("HMC init OK!", Font_7x10, White);
		else
			  ssd1306_WriteString("HMC init NOK!", Font_7x10, White);
	  ssd1306_UpdateScreen();
		
	//my_delay(3000);
	
	/* Infinite loop */
	
	
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

