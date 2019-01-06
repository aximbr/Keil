/*
 P13 - conexão ao MultiShield para utilizar o display de 8 segmentos
 * Author: Jose P. Leitão
 * 2019-01-06
*******************************************************************************/
/* DriverLib Includes */
#include "driverlib.h"


/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

#ifndef _BV
#define _BV(n) (1 << n)
#endif


/* Define shift register pins used for seven segment display */
#define LATCH_PORT  (GPIO_PORT_P4)
#define CLK_PORT    (GPIO_PORT_P4)
#define DATA_PORT   (GPIO_PORT_P4)

#define LATCH_PIN    (GPIO_PIN4)  //D4 <-> P4.4
#define CLK_PIN      (GPIO_PIN5)  //D7 <-> P4.5
#define DATA_PIN     (GPIO_PIN7)  //D8 <-> P4.7

#define bitSet(_pin)    GPIO_setOutputHighOnPin(GPIO_PORT_P4, _pin)
#define bitClear(_pin)  GPIO_setOutputLowOnPin(GPIO_PORT_P4, _pin)

#define REFRESH 1000  //Taxa de refresh ou base de tempo do contador (ms)

/* Global variables */

int volatile tick_count=0;  //contador de eventos

/* Segment byte maps for numbers 0 to 9 */
const uint8_t SEGMENT_MAP[] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0X80,0X90};
/* Byte maps to select digit 1 to 4 */
const uint8_t SEGMENT_SELECT[] = {0xF1,0xF2,0xF4,0xF8};

/*prototypes for local functions */
void WriteNumberToSegment(uint8_t Segment, uint8_t Value);

/* Systick Handle - just increment our tick counter every 1 ms */
void SysTick_Handler(void) 
{
	tick_count++;
}


/* Write a decimal number between 0 and 9 to one of the 4 digits of the display */
void WriteNumberToSegment(uint8_t Segment, uint8_t Value)
{
	uint8_t x;
	bitClear(LATCH_PIN);

	for (uint8_t i = 0; i < 8; i++)  {
		x = !!(SEGMENT_MAP[Value] & (1 << (7 - i)));
		if (x) bitSet(DATA_PIN);
		else bitClear(DATA_PIN);
		bitSet(CLK_PIN);
		bitClear(CLK_PIN);
	}

	for (uint8_t i = 0; i < 8; i++)  {
		x = !!(SEGMENT_SELECT[Segment] & (1 << (7 - i)));
		if (x) bitSet(DATA_PIN);
		else bitClear(DATA_PIN);
		bitSet(CLK_PIN);
		bitClear(CLK_PIN);
	}

	bitSet(LATCH_PIN);
}


int main (void)
{
	/* Halting the Watchdog */
    MAP_WDT_A_holdTimer();
	
	/* Insert system clock initialization code here. */
   
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
	
	/* initialise  Systick  for 1 ms @ 48MHz clock */
	SysTick_enableModule();
	SysTick_setPeriod(SystemCoreClock/1000);
	SysTick_enableInterrupt();
	Interrupt_enableMaster();
	
	
	
	/* configure the pins for Latch, Clock and Data */
	GPIO_setAsOutputPin(LATCH_PORT, LATCH_PIN); 
	GPIO_setAsOutputPin(CLK_PORT, CLK_PIN);
	GPIO_setAsOutputPin(DATA_PORT, DATA_PIN);
	
			
	tick_count = 0;    //reset tick count
		
		while (1)
		{
			for (int m=0; m<6; m++) {
				for (int c=0; c<10; c++) {
					for (int d=0;d<6;d++) {
						for (int u=0;u<10;u++) {
							while(tick_count < REFRESH){   //keeping display the numbers before increment a unit 
								WriteNumberToSegment(0 , m);
								WriteNumberToSegment(1 , c);
								WriteNumberToSegment(2 , d);
								WriteNumberToSegment(3 , u);
								
							}
							tick_count = 0;   //reset tick count 
						}
					}
				}
			} 
		}
	}


