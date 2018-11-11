/*Etapa 7 - Controlar o piscar do Led PA5 através de uma chave com Systick usando interrupção */
#include "stm32f10x.h"

/*Variaveis Globais */
uint32_t volatile tick_count;

/* Função que cria delay em X *1ms */
void My_delay(uint32_t delay_ms){
	tick_count = 0;
	while (tick_count < delay_ms);
	}
	
int main(void){
/*variaveis locais */

	
/*inicializa a placa */
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN |RCC_APB2ENR_IOPCEN;   //1) Habilita o clock da GPIOA e GPIOC
	GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);          //2) Limpa a configuração e modo de PA5
	GPIOA->CRL |= GPIO_CRL_MODE5_1;                           //3) Configura para output vel max de 2Mhz, open drain             
	GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);        //4) Limpa a configuração e modo de PC13
	GPIOC->CRH |= GPIO_CRH_CNF13_1;                           //5 Configura como Input, PULL-UP
	                                    
	
/* inicializa o Systick */
	__disable_irq();
	SysTick->CTRL = 0;                          //desabilita o systick
	SysTick->LOAD = (SystemCoreClock/1000)-1;   //carrega systick para tempo de 1 ms
	SysTick->CTRL = 0x7;                        //habilita o SysTick com interrupção
	__enable_irq();
	
/* Loop infinito */	
	while(1){
		if (!(GPIOC->IDR & (1UL<<13))) {
			GPIOA->BSRR |= (1UL <<5);       //liga o pino PA5
		  My_delay(500);                  //delay
		  GPIOA->BSRR |= (1UL <<(16+5));  //desliga o pino PA5
			My_delay(500);                  //delay
		}
	  GPIOA->BSRR |= (1UL <<(16+5));  //desliga o pino PA5
	}
}

void SysTick_Handler(void){
	tick_count++;
}
