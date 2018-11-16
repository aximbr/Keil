/*Etapa 12                                                        */
/*Projeto utizando ADC, um potenciômetro como divisor de tensão   */
/*FreeRTOS e um display nokia 5110 para indicar o valor da tensão */
/*Autor: José P. Leitão                                           */
/*2018-11-15                                                      */
/*Keil v5                                                         */

#include "MKL25Z4.h"
#include "adc_FRDM.h"

/*Variaveis Globais */


/*Prototipo funções locais */


int main(void){
	volatile uint16_t x;
	
	//initiate boards
	ADC0_Init();
	
	while(1) {
		
		x = ADC0_In();
		
		__nop();
	}
	
	
}
