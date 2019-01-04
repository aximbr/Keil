/*Etapa 5 - Piscar o Leds D1 em PF9 com o SysTick e recurso de interrupção */
#include "stm32f4xx.h"

/*Variaveis Globais */
uint32_t volatile tick_count;

/* Função que cria delay em X *1ms */
void My_delay(uint32_t delay_ms){
	tick_count = 0;
	while (tick_count < delay_ms);
	}
	
void SysTick_Handler(void){
	tick_count++;
}

	
int main(void){
/*local variable */
	
	
/*setup GPIO F */
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN_Msk;                     //1) Enable clock on GPIOF
	GPIOF->MODER |= GPIO_MODER_MODE9_0;                          //2) Set PF10 and PF9 mode (output)
	
/* inicializa o Systick */
  __disable_irq();
	SysTick->CTRL = 0;                      //desabilita o systick
	SysTick->LOAD = SystemCoreClock/1000;   //carrega systick para tempo de 1 ms
	SysTick->CTRL = 0x7;                    //habilita o SysTick com interrupção
	__enable_irq();
	
		
 //Endless loop
	while(1){ 
	     GPIOF->ODR &= ~GPIO_ODR_OD9_Msk;       //turn on LED D1
		   My_delay(250);                         //delay
	     GPIOF->ODR |= GPIO_ODR_OD9_Msk;        //turn off LED D1
	     My_delay(250);                         //delay
	     }
}

