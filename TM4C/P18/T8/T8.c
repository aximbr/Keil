/* Periodic Timer with interrupt*/

#include "TM4C123.h"                    // Device header

#define BLUE (1U<<2)
#define RED  (1U<<1)


//Prototypes
void timer1_1Hz_init(void);
void timer2_10Hz_init(void);

int main(void){
	SYSCTL->RCGCGPIO |= 0x20; //enable clock on GPIOF
	GPIOF->DIR |= BLUE | RED; //enable output
	GPIOF->DEN |= BLUE | RED; //digital enable
	
	timer1_1Hz_init();
//	timer2_10Hz_init();
	
	__enable_irq();
	

	while(1){

	
	}
	
}

void timer1_1Hz_init(void){
	SYSCTL->RCGCTIMER |= (1U << 1); //enable clock on timer1
	
	TIMER1->CTL &= ~(0x01);    //disable timer
  TIMER1->CFG = 0x04;        //configure timer for 16 bits
	TIMER1->TAMR |= 0x02;      //TAMR bits: periodic timer mode
	TIMER1->TAPR = 250;        // 16.000.000/250 = 64.000
	TIMER1->TAILR = 64000;     //Load Timer with number that's count for 1 sec. Clock @ 16MHz
	TIMER1->ICR |= 0x01;       //clear raw interrupt
	TIMER1->IMR |= 0x01;       //interrup mask register
	TIMER1->CTL |= 0x01;       //enable timer
	
	NVIC->ISER[0] |= 0x00200000; //enable IRQ21 (D21) for ISER[0]
}

void timer2_10Hz_init(void){
	SYSCTL->RCGCTIMER |= (1U << 2); //enable clock on timer2
	
	TIMER2->CTL &= ~(0x01);    //disable timer
  TIMER2->CFG = 0x00;        //configure timer for 32 bits
	TIMER2->TAMR |= 0x02;      //TAMR bits: periodic timer mode
//	TIMER2->TAPR = 250;      // Prescaler only works on 16 bits mode
	TIMER2->TAILR = 160000000; //Load Timer with number that's count for 10 sec. Clock @ 16MHz
	TIMER2->ICR |= 0x01;       //clear raw interrupt
	TIMER2->IMR |= 0x01;       //interrup mask register
	TIMER2->CTL |= 0x01;       //enable timer
	
	NVIC->ISER[0] |= 0x00800000; //enable IRQ23 (D23) for ISER[0]
}

void TIMER1A_Handler(void){
	GPIOF->DATA ^= BLUE;
	TIMER1->ICR |= 0x01;
}

void TIMER2A_Handler(void){
	GPIOF->DATA ^= RED;
	TIMER2->ICR |= 0x01;
}
