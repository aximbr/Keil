/*
 P14 - conexão ao display OLED baseado no SSD1136 usando I2C
 * Author: Jose P. Leitão
 * 2019-03-25
 * Keil v5                                                        */

/* Includes *******************************************************/
#include "MKL25Z4.h"
#include <stdint.h>
#include "ssd1306.h"
#include "my_tick.h"



/* Global variables */

/* Defines ********************************************************/

#define ICR   0x04    //Bus @ 24Mhz I2C baud rate 1 Mhz, see datasheet at pp706


/* Private macro -------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

 
/* Global variables ----------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/





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
 
	
	
	/* Insert system clock initialization code here. */
  SIM_COPC = 0x00U;                                                   
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;   /* Enable clock gate for ports to enable pin routing */
  /* SIM_CLKDIV1: OUTDIV1=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,OUTDIV4=1,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
  SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV4(0x01); /* Update system prescalers */
  /* SIM_SOPT2: PLLFLLSEL=0 */
  SIM_SOPT2 &= (uint32_t)~(uint32_t)(SIM_SOPT2_PLLFLLSEL_MASK); /* Select FLL as a clock source for various peripherals */
  /* SIM_SOPT1: OSC32KSEL=3 */
  SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(0x03); /* LPO 1kHz oscillator drives 32 kHz clock for various peripherals */
  /* SIM_SOPT2: TPMSRC=1 */
  SIM_SOPT2 = (uint32_t)((SIM_SOPT2 & (uint32_t)~(uint32_t)(
               SIM_SOPT2_TPMSRC(0x02)
              )) | (uint32_t)(
               SIM_SOPT2_TPMSRC(0x01)
              ));                      /* Set the TPM clock */
  /* Switch to FEI Mode */
  /* MCG_C1: CLKS=0,FRDIV=0,IREFS=1,IRCLKEN=1,IREFSTEN=0 */
  MCG_C1 = (MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK);                                                   
  /* MCG_C2: LOCRE0=0,??=0,RANGE0=0,HGO0=0,EREFS0=0,LP=0,IRCS=0 */
  MCG_C2 = 0x00U;                                                   
  /* MCG_C4: DMX32=1,DRST_DRS=1 */
  MCG_C4 = (uint8_t)((MCG_C4 & (uint8_t)~(uint8_t)(
            MCG_C4_DRST_DRS(0x02)
           )) | (uint8_t)(
            MCG_C4_DMX32_MASK |
            MCG_C4_DRST_DRS(0x01)
           ));                                                  
  /* OSC0_CR: ERCLKEN=1,??=0,EREFSTEN=0,??=0,SC2P=0,SC4P=0,SC8P=0,SC16P=0 */
  OSC0_CR = OSC_CR_ERCLKEN_MASK;                                                   
  /* MCG_C5: ??=0,PLLCLKEN0=0,PLLSTEN0=0,PRDIV0=0 */
  MCG_C5 = 0x00U;                                                   
  /* MCG_C6: LOLIE0=0,PLLS=0,CME0=0,VDIV0=0 */
  MCG_C6 = 0x00U;                                                   
  while((MCG_S & MCG_S_IREFST_MASK) == 0x00U) { /* Check that the source of the FLL reference clock is the internal reference clock. */
  }
  while((MCG_S & 0x0CU) != 0x00U) {    /* Wait until output of the FLL is selected */
  }
	
	
	/* initialise  Systick  for 1 ms @ 48MHz clock */
	InitMyTick();
	
	/* Configure I2C module */
	//Select alternate 1 for I2C0, Multipler = 1 and ICR = 0x14 => baud rate 100Khz
	i2c_Init(SSD1306_I2C_PORT, ALT1, MULT0, ICR);
	
			
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
