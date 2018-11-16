/*Etapa 12                                                        */
/*Projeto utizando ADC, um potenci�metro como divisor de tens�o   */
/*FreeRTOS e um display nokia 5110 para indicar o valor da tens�o */
/*Autor: Jos� P. Leit�o                                           */
/*2018-11-15                                                      */
/*Keil v5                                                         */

#include "MKL25Z4.h"
#include "adc_FRDM.h"

/*Variaveis Globais */


/*Prototipo fun��es locais */


int main(void){
	volatile uint16_t x;
	
	//initiate boards
	ADC0_Init();
	
	while(1) {
		
		x = ADC0_In();
		
		__nop();
	}
	
	
}
