/*Etapa 12                                                        */
/*Projeto utizando ADC, um potenci�metro como divisor de tens�o   */
/*FreeRTOS e um display nokia 5110 para indicar o valor da tens�o */
/*Autor: Jos� P. Leit�o                                           */
/*2018-11-13                                                      */
/*Keil v5                                                         */

#include "MSP432.h"
#include "adc_MSP432.h"


int main(void){
	volatile uint16_t x ;
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;                 // Stop WDT
	
	// init board components
	ADC0_Init();
	
	while(1){
		
		x = ADC0_In();
		__NOP;
		
	}
	
		
}
