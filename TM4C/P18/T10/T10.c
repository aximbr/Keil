/* Periodic Timer with interrupt*/

#include "TM4C123.h"                    // Device header

#define RED  (1U<<1)


//Prototypes
void timer1_pwm_init(void);


int main(void){
	SYSCTL->RCGCGPIO |= 0x20; //enable clock on GPIOF
	GPIOF->DIR |= RED; //enable output
	GPIOF->DEN |= RED; //digital enable
	
	timer1_pwm_init();
	
	__enable_irq();
 

	while(1){
	}
	
}

void timer1_pwm_init(void){
	SYSCTL->RCGCTIMER |= (1U << 1); //enable clock on timer1
	
	TIMER1->CTL &= ~(0x01);    //disable timer
  TIMER1->CFG = 0x04;        //configure timer for 16 bits
	TIMER1->TAMR = 0x0;
	TIMER1->TAMR |= (1UL << 9);      //PWM int bit set
  TIMER1->TAMR |= (1UL << 3);      //TAMS bit set
  TIMER1->TAMR |= (2UL << 0);      //TAMR bits: periodic timer mode
	TIMER1->CTL |= (1UL << 6);    //TAPWML
  TIMER1->CTL |= (3UL << 2);  //enable int both edges	
//	TIMER1->TAPR = 250;        // 16.000.000/250 = 64.000
	TIMER1->TAILR = 64000;     //Load Timer with number that's count for 1 sec. Clock @ 16MHz
	TIMER1->TAMATCHR = 32000;  //match value
	TIMER1->ICR |= (1UL << 4);       //clear raw interrupt
	TIMER1->IMR |= (1UL << 4);       //interrup mask register
	TIMER1->CTL |= 0x01;       //enable timer
	
	NVIC->ISER[0] |= 0x00200000; //enable IRQ21 (D21) for ISER[0]
}


void TIMER1A_Handler(void){
	GPIOF->DATA ^= RED;
	TIMER1->ICR |= (1UL <<4);
}


