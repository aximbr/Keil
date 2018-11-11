/*Etapa 6 - Controlar o Led através do Switch */
/* Switch PC13 */
/* LED PA5 */

#include "stm32f10x.h"

int main(void){
/*variaveis locais */
	uint32_t estado, i;
	
/*inicializa a placa */
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN |RCC_APB2ENR_IOPCEN;   //1) Habilita o clock da GPIOA e GPIOC
	GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);          //2) Limpa a configuração e modo de PA5
	GPIOA->CRL |= GPIO_CRL_MODE5_1;                           //3) Configura para output vel max de 2Mhz, open drain             
	GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);        //4) Limpa a configuração e modo de PC13
	GPIOC->CRH |= GPIO_CRH_CNF13_1;                           //5 Configura como Input, PULL-UP
	//liga o pino PA5
	GPIOA->BSRR |= (1UL <<5);
	
	
  //Loop infinito	
	while(1){ 
		estado = !(GPIOC->IDR&(1UL<<13));
	  if (estado) GPIOA->ODR ^= (1UL<<5);
		for(i=0;i<50;i++);
		}
}
