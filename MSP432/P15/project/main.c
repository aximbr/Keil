/*
 P14 - conexão ao display OLED baseado no SSD1136 usando I2C
 * Author: Jose P. Leitão
 * 2019-03-18
*******************************************************************************/
/* DriverLib Includes */
#include "msp432.h"
#include "driverlib.h"


/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "ssd1306.h"
#include "HMC5883L.h"
#include "conv_to_string.h"
#include "my_tick.h"

/* Global variables */

/*prototypes for local functions */
void clock_init(void);
void systick_init(void);

/*MAIN Program */
int main (void)
{
	char zz[10];
	int16_t leitura[3];

	/* Halting the Watchdog */
  MAP_WDT_A_holdTimer();
	
	/* Insert system clock initialization code here. */
  clock_init();
	
	/* initialise  Systick  for 1 ms @ 48MHz clock */
	systick_init();
		
	/* HW init ------------------------------------------------------*/
  hal_i2c_init(EUSCI_B0);
	ssd1306_Init();

	HMC5883L_Initialize();

	
	ssd1306_Fill(Black);
  ssd1306_SetCursor(20, 30);
  if ( HMC5883L_TestConnection() )
     ssd1306_WriteString("HMC init OK!", Font_7x10, White);
		else
			  ssd1306_WriteString("HMC init NOK!", Font_7x10, White);
	  ssd1306_UpdateScreen();
		
	My_Delay(3000);
	
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

void clock_init(void){
	/* configure MCU for DCO source @ 3Mhz */
 CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_3);
 CS_setDCOFrequency(3000000);  
	/* Configuring pins for peripheral/crystal usage */
 GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ, GPIO_PIN3 | GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
	/* Just in case the user wants to use the getACLK, getMCLK, etc. functions,
   * let’s set the clock frequency in the code.
  */
 CS_setExternalClockSourceFrequency(32000, 48000000);
  /* Starting HFXT in non-bypass mode without a timeout. Before we start
   * we have to change VCORE to 1 to support the 48MHz frequency */
 PCM_setCoreVoltageLevel(PCM_VCORE1);
 FlashCtl_setWaitState(FLASH_BANK0, 2);
 FlashCtl_setWaitState(FLASH_BANK1, 2);
 CS_startHFXT(false);
  /* Initializing MCLK to HFXT (effectively 48MHz) */
 CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
	
	/* Initializing SMCLK to DCO (effectively 3MHz) */
 CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
 CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);	
}

void systick_init(void){
	//SysTick_enableModule();
	//SysTick_setPeriod(SystemCoreClock/1000);
	//SysTick_enableInterrupt();
	//Interrupt_enableMaster();
	InitMyTick();
}

/* Systick Handle - just increment our tick counter every 1 ms */
void SysTick_Handler(void) 
{
	MyTick();
}
