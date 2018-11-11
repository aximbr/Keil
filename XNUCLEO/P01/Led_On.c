/*Etapa 1 - Acender o Led user1 em PA5 */
#include "stm32f10x.h"

int main(void){
/*inicializa a placa */
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;                 //1) Habilita o clock da GPIOA
	GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);    //2) Limpa a configuração e modo de PA5
	GPIOA->CRL |= GPIO_CRL_MODE5_1;                     //3) Configura para output vel max de 2Mhz, open drain             
	
	//liga o pino PA5
	GPIOA->BSRR |= (1UL <<5);
		
	while(1); //Loop infinito
}
