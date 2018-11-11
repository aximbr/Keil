/*Etapa 2 - Piscar o Led user1 em PA5 */
#include "stm32f10x.h"

int main(void){
/*variaveis locais */
	uint32_t i;
	
/*inicializa a placa */
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;                 //1) Habilita o clock da GPIOA
	GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);    //2) Limpa a configuração e modo de PA5
	GPIOA->CRL |= GPIO_CRL_MODE5_1;                     //3) Configura para output vel max de 2Mhz, open drain             
	
	//liga o pino PA5
	GPIOA->BSRR |= (1UL <<5);
	
  //Loop infinito	
	while(1){ 
	for(i=0; i<800000; i++);        //delay
	GPIOA->BSRR |= (1UL << (16+5)); //reset o PA5
	for(i=0; i<800000; i++);        //delay
	GPIOA->BSRR |= (1UL <<5);       //set o PA5
	}
}
