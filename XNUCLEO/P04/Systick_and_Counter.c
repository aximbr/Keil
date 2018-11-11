/*Etapa 4 - Piscar 0 Led PA5 com Systick e contador */
#include "stm32f10x.h"

void Systick_Wait(uint32_t delay){
	SysTick->LOAD = delay - 1;
	SysTick->VAL = 0;
  while((SysTick->CTRL & 0x00010000) == 0) { //aguarda até zerar o contador do SysTick 
	}	
}

/* Função que cria delay em X *1ms */
void My_delay(uint32_t delay_ms){
	uint32_t i;
	for (i=0; i< delay_ms; i++){
		Systick_Wait(SystemCoreClock/1000);}  //configura o SysTick para 1ms
	}
	
int main(void){
/*variaveis locais */

	
/*inicializa a placa */
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;                       //1) Habilita o clock da GPIOA
	GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);          //2) Limpa a configuração e modo de PA5
	GPIOA->CRL |= GPIO_CRL_MODE5_1;                           //3) Configura para output vel max de 2Mhz, open drain             
	
/* inicializa o Systick */
	__disable_irq();
	SysTick->CTRL = 0;             //desabilita o systick
	SysTick->LOAD = 0x00FFFFFF;    //carrega o valor maximo
	SysTick->CTRL = 0x5;           //habilita o SysTick sem interrupção
	__enable_irq();
	
/* Loop infinito */	
	while(1){ 
	GPIOA->BSRR |= (1UL <<5);       //liga o pino PA5
	My_delay(250);                  //delay
	GPIOA->BSRR |= (1UL <<(16+5)); //desliga o pino PA5
	My_delay(250);                  //delay
	}
}
