/* using PWM Module of TM4C123 */

#include "TM4C123.h"                    // Device header

#define PWM_MOD0   (1UL << 20)

void pwm0_init(void);

int main(void){
	
	pwm0_init();
	
	while(1){
	}
	
}

void pwm0_init(void){
	
	SYSCTL->RCGC0 |= PWM_MOD0;       //enable clock on PWM0
	SYSCTL->RCGCGPIO |= (1UL << 1);  //enable clock on GPIOB
	
	/*configura o pino PB6 como output e usa função alternativa */
	GPIOB->DIR |= (1UL <<6);  //PB6 as Output
	GPIOB->DEN |= (1UL <<6);  //PB6 digital enable
	GPIOB->AFSEL |= (1UL <<6); //PB6 with Alternate function
	GPIOB->PCTL |= (4UL <<24); //PB6 with function 4 (M0PWM0 signal)
	
	SYSCTL->RCC |= (1UL << 20); //enable PWM DIV
	SYSCTL->RCC &= ~(7UL <<17); //set PWM DIV = 2 (000)
	
	PWM0->_0_CTL = 0x0;
	PWM0->_0_GENA = 0x008C;  //contador=LOAD -> saida_A=0; contador_decrescente=COMP_A -> saida_A=1
	PWM0->_0_GENB = 0x080C;  //contador=LOAD -> saida_B=1; contador_decrescente=COMP_B -> saida_B=0
	PWM0->_0_LOAD = 0x018F;  //valor do load = período total (tempo high + tempo low)
	PWM0->_0_CMPA = 0x012B;  //tempo do ciclo em LOW
	PWM0->_0_CMPB = 0x0063;  //tempo do ciclo em HIGH
	PWM0->_0_CTL |= 0x01;    //start PWM
	PWM0->ENABLE |= 0x0003;  //habilita as duas saidas 
}
