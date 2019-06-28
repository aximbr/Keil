/* Periodic 16 bits Timer */

#include "TM4C123.h"                    // Device header

#define BLUE (1U<<2)

//Prototypes
void timer0_periodic(int time);

int main(void){
	SYSCTL->RCGCGPIO |= 0x20; //enable clock on GPIOF
	GPIOF->DIR |= BLUE; //enable output
	GPIOF->DEN |= BLUE; //digital enable
	

	while(1){
		GPIOF->DATA ^= BLUE;
		timer0_periodic(1000);
	}
	
}

void timer0_periodic(int time){
	SYSCTL->RCGCTIMER |= 0x01; //enable clock on timer0
	
	TIMER0->CTL &= ~(0x01); //disable timer
  TIMER0->CFG = 0x04; //configure timer for 16 bits (maximum delay is 4 ms)
//TIMER0->CFG = 0x00; //configure timer for 32 bits (maximum delay is 268,4 s)

	TIMER0->TAMR |= 0x02; //TAMR bits: periodic timer mode
	TIMER0->TAILR = 16000 - 1; //Load Timer with number that's count for 1 msec. Clock @ 16MHz
	TIMER0->ICR |= 0x01; //clear raw interrupt
	TIMER0->CTL |= 0x01; //enable timer
	
	for (int j = 0; j < time; j++){
		while ( (TIMER0->RIS & 0x01) == 0 ); //wait until timer expire
		TIMER0->ICR |= 0x01; //clear interrupt
	}
}
