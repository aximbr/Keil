/*Etapa 5 - Piscar o Led PA5 com Systick usando interrupção */
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
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;                       //1) Habilita o clock da GPIOA
	GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);          //2) Limpa a configuração e modo de PA5
	GPIOA->CRL |= GPIO_CRL_MODE5_1;                           //3) Configura para output vel max de 2Mhz, open drain             
	
/* inicializa o Systick */
	__disable_irq();
	SysTick->CTRL = 0;                      //desabilita o systick
	SysTick->LOAD = SystemCoreClock/1000;   //carrega systick para tempo de 1 ms
	SysTick->CTRL = 0x7;                    //habilita o SysTick com interrupção
	__enable_irq();
	
/* Loop infinito */	
	while(1){ 
	GPIOA->BSRR |= (1UL <<5);       //liga o pino PA5
	My_delay(500);                  //delay
	GPIOA->BSRR |= (1UL <<(16+5));  //desliga o pino PA5
	My_delay(500);                  //delay
	}
}
void SysTick_Handler(void){
	tick_count++;
}
