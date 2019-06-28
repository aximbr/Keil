/* Periodic wide Timer with 32 bits */

#include "TM4C123.h"                    // Device header

#define BLUE (1U<<2)

//Prototypes
void wtimer1_periodic_32b(int seconds);

int main(void){
	SYSCTL->RCGCGPIO |= 0x20; //enable clock on GPIOF
	GPIOF->DIR |= BLUE; //enable output
	GPIOF->DEN |= BLUE; //digital enable
	

	while(1){
		GPIOF->DATA ^= BLUE;
		wtimer1_periodic_32b(1);
	}
	
}

void wtimer1_periodic_32b(int seconds){
	SYSCTL->RCGCWTIMER |= 0x02; //enable clock on wide timer 1
	
	WTIMER1->CTL &= ~(0x01); //disable timer
  WTIMER1->CFG = 0x04; //configure timer for 32 bits (maximum delay is 268,4 s)
//  WTIMER1->CFG = 0x00; //configure timer for 64 bits (maximum delay is huge!!

	WTIMER1->TAMR |= 0x02; //TAMR bits: periodic timer mode
	/* 1 sec = 16.000.000 counts @ 16 MHz Clock */
	WTIMER1->TAILR = 16000000 - 1; //Load Timer to represents 1 sec

	
	WTIMER1->ICR |= 0x01; //clear raw interrupt
	WTIMER1->CTL |= 0x01; //enable timer
	
	for (int j = 0; j < seconds; j++){
		while ( (WTIMER1->RIS & 0x01) == 0 ); //wait until timer expire
		WTIMER1->ICR |= 0x01; //clear interrupt
	}
}
