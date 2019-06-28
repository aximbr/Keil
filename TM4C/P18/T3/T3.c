/* Periodic Timer with Prescaler */

#include "TM4C123.h"                    // Device header

#define BLUE (1U<<2)

//Prototypes
void timer0_with_prescaler(int seconds);

int main(void){
	SYSCTL->RCGCGPIO |= 0x20; //enable clock on GPIOF
	GPIOF->DIR |= BLUE; //enable output
	GPIOF->DEN |= BLUE; //digital enable
	

	while(1){
		GPIOF->DATA ^= BLUE;
		timer0_with_prescaler(1);
	}
	
}

void timer0_with_prescaler(int seconds){
	SYSCTL->RCGCTIMER |= 0x01; //enable clock on timer0
	
	TIMER0->CTL &= ~(0x01); //disable timer
  TIMER0->CFG = 0x04; //configure timer for 16 bits (maximum delay is 4 ms)
//TIMER0->CFG = 0x00; //configure timer for 32 bits (maximum delay is 268,4 s)

	TIMER0->TAMR |= 0x02; //TAMR bits: periodic timer mode
	/* system_clock/prescaler -> 16000000/256 = 62500 */
	TIMER0->TAILR = 62500 - 1; //Load Timer to represents 1 sec
	TIMER0->TAPR = 256 -1;
	
	TIMER0->ICR |= 0x01; //clear raw interrupt
	TIMER0->CTL |= 0x01; //enable timer
	
	for (int j = 0; j < seconds; j++){
		while ( (TIMER0->RIS & 0x01) == 0 ); //wait until timer expire
		TIMER0->ICR |= 0x01; //clear interrupt
	}
}
