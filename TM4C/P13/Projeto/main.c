/*Projeto 13 - utilizar um display de 7 segmentos com 4 dígitos 
* utilizado do shield mutifunction 
*
* Jose P. Leitao - 2018-12-08
*/

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"



#ifndef _BV
#define _BV(n) (1 << n)
#endif



/* Define shift register pins used for seven segment display */
#define LATCH_PORT  (GPIO_PORTA_BASE)
#define CLK_PORT    (GPIO_PORTA_BASE)
#define DATA_PORT   (GPIO_PORTA_BASE)

#define LATCH_PIN    (GPIO_PIN_5)  //D4 <-> A.5
#define CLK_PIN      (GPIO_PIN_6)  //D7 <-> A.6
#define DATA_PIN     (GPIO_PIN_7)  //D8 <-> A.7


//#define LATCH_PIN_FLAGS      (PIO_TYPE_PIO_OUTPUT_1 | PIO_DEFAULT)
//#define CLK_PIN_FLAGS        (PIO_TYPE_PIO_OUTPUT_1 | PIO_DEFAULT)
//#define DATA_PIN_FLAGS       (PIO_TYPE_PIO_OUTPUT_1 | PIO_DEFAULT)

#define bitSet(_pin)    GPIOPinWrite(GPIO_PORTA_BASE, _pin, _pin)
#define bitClear(_pin)  GPIOPinWrite(GPIO_PORTA_BASE, _pin, 0)

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
	/* Insert system clock initialization code here. */
  SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);	// set up the clock 
	
	/* initialise  Systick  for 1 ms @ 16MHz clock */
	SysTickPeriodSet(8000UL - 1);
	IntMasterEnable();
	SysTickIntEnable();
	SysTickEnable();
	
	/* configure the pins for Latch, Clock and Data */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	
	GPIOPinTypeGPIOOutput( LATCH_PORT, LATCH_PIN);
	GPIOPinTypeGPIOOutput( CLK_PORT, CLK_PIN);
	GPIOPinTypeGPIOOutput( DATA_PORT, DATA_PIN);
	
			
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
