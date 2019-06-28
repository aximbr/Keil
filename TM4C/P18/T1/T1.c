/* One shot Timer */

#include "TM4C123.h"                    // Device header

#define RED (1U<<1)

//Prototypes
void timer0_oneshot(int mdelay);

int main(void){
	SYSCTL->RCGCGPIO |= 0x20; //enable clock on GPIOF
	GPIOF->DIR |= RED; //enable output
	GPIOF->DEN |= RED; //digital enable
	
	while(1){
		GPIOF->DATA ^= RED;
		timer0_oneshot(500);
	}
	
}

void timer0_oneshot(int mdelay){
	SYSCTL->RCGCTIMER |= 0x01; //enable clock on timer0
	
	TIMER0->CTL &= ~(0x01); //disable timer
//	TIMER0->CFG = 0x04; //configure timer for 16 bits (maximum delay is 4 ms)
	TIMER0->CFG = 0x00; //configure timer for 32 bits (maximum delay is 268,4 s)

	TIMER0->TAMR |= 0x01; //TAMR bits: one shot timer mode
	TIMER0->TAILR = 16000 * mdelay - 1; //Load Timer with number that represents the delay in msec. Clock @ 16MHz
	TIMER0->ICR |= 0x01; //clear raw interrupt
	TIMER0->CTL |= 0x01; //enable timer
	while ( (TIMER0->RIS & 0x01) == 0 ); //wait until timer expire
}
