/*
 P14 - conexão ao display OLED baseado no SSD1136 usando I2C
 * Author: Jose P. Leitão
 * 2019-03-18
*******************************************************************************/
/* DriverLib Includes */
#include "driverlib.h"


/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "ssd1306.h"
#include "my_tick.h"



/* Global variables */
/* I2C Master Configuration Parameter */
const eUSCI_I2C_MasterConfig i2cConfig =
{
  EUSCI_B_I2C_CLOCKSOURCE_SMCLK,     // SMCLK Clock Source
  3000000,                           // SMCLK = 3MHz
  EUSCI_B_I2C_SET_DATA_RATE_1MBPS,   // Desired I2C Clock of 1 Mbps
  0,                                 // No byte counter threshold
  EUSCI_B_I2C_NO_AUTO_STOP           // No Autostop
};


/*prototypes for local functions */



/* Systick Handle - just increment our tick counter every 1 ms */
void SysTick_Handler(void) 
{
	MyTick();
}



int main (void)
{
	/* Halting the Watchdog */
    MAP_WDT_A_holdTimer();
	
	/* Insert system clock initialization code here. */
	/* configure MCU for DCO source @ 3Mhz */
 CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_3);
 CS_setDCOFrequency(3000000);  
	/* Configuring pins for peripheral/crystal usage */
 //GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ, GPIO_PIN3 | GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
	/* Just in case the user wants to use the getACLK, getMCLK, etc. functions,
   * let’s set the clock frequency in the code.
  */
 //CS_setExternalClockSourceFrequency(32000, 48000000);
  /* Starting HFXT in non-bypass mode without a timeout. Before we start
   * we have to change VCORE to 1 to support the 48MHz frequency */
 //PCM_setCoreVoltageLevel(PCM_VCORE1);
 //FlashCtl_setWaitState(FLASH_BANK0, 2);
 //FlashCtl_setWaitState(FLASH_BANK1, 2);
 //CS_startHFXT(false);
  /* Initializing MCLK to HFXT (effectively 48MHz) */
 //CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
	
	/* Initializing SMCLK to DCO (effectively 3MHz) */
 CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
 CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
 
 
	/* initialise  Systick  for 1 ms @ 48MHz clock */
	SysTick_enableModule();
	SysTick_setPeriod(SystemCoreClock/1000);
	SysTick_enableInterrupt();
	Interrupt_enableMaster();
	
	/* Configure I2C module */
	//Set primary function for P1.6 (UCB0SDA) and P1.7 (UCB0SCL)
	//Set primary function for P6.4 (UCB1SDA) and P6.5 (UCB1SCL)
	if (SSD1306_I2C_PORT == EUSCI_B0_BASE)
	    {	P1->SEL0 |= BIT6 | BIT7;}                // I2C pins
	else
	    { P6->SEL0 |= BIT4 | BIT5;}                // I2C pins
	
	I2C_disableModule(SSD1306_I2C_PORT);
	I2C_initMaster(SSD1306_I2C_PORT, &i2cConfig);
	I2C_enableModule(SSD1306_I2C_PORT);
		
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


