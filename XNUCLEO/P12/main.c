/*Etapa 12                                                        */
/*Projeto utizando ADC, um potenci�metro como divisor de tens�o   */
/*FreeRTOS e um display nokia 5110 para indicar o valor da tens�o */
/*Autor: Jos� P. Leit�o                                           */
/*2018-11-16                                                      */
/*Keil v5                                                         */

#include "STM32f10x.h"
#include "adc_STM32F103.h"


int main(void){
	volatile uint16_t x;
	
	ADC0_Init();
	
	while(1){
		x = ADC0_In();
		
		__nop();
		
	}
	
}


