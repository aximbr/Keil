/*Etapa 3 - Piscar 2 Led PA5 e PC9 */
#include "stm32f10x.h"

int main(void){
/*variaveis locais */
	uint32_t i;
	
/*inicializa a placa */
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;  //1) Habilita o clock da GPIOA e GPIOC
	GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);          //2) Limpa a configuração e modo de PA5
	GPIOC->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);          //2) Limpa a configuração e modo de PC9
	GPIOA->CRL |= GPIO_CRL_MODE5_1;                           //3) Configura para output vel max de 2Mhz, open drain             
	GPIOC->CRH |= GPIO_CRH_MODE9_1;                           //3) Configura para output vel max de 2Mhz, open drain 
	
	//liga o pino PA5 e PC9
	GPIOA->BSRR |= (1UL <<5);
	//GPIOC->BSRR |= (1UL <<9);
	
  //Loop infinito	
	while(1){ 
	for(i=0; i<800000; i++);        //delay
	GPIOA->BSRR |= (1UL << (16+5)); //reset o PA5
	for(i=0; i<800000; i++);        //delay
	GPIOC->BSRR |= (1UL << 9);      //set o PC9
	for(i=0; i<800000; i++);        //delay
	GPIOC->BSRR |= (1UL << (16+9)); //reset o PC9
	for(i=0; i<800000; i++);        //delay
	GPIOA->BSRR |= (1UL <<5);       //set o PA5
	}
}
