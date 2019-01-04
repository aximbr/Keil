/*Etapa 7 - Através do switch K0 em PE4 pisca o Leds D1 em PF9 com uso do SysTick e interrupção */
#include "stm32f4xx.h"
#include <stdint.h>

#define SW_PIN  4
#define LED_PIN 9

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
	uint32_t estado;
	
/*setup GPIO F */
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN_Msk | RCC_AHB1ENR_GPIOEEN_Msk;   //1) Enable clock on GPIOF and GPIOE
	GPIOF->MODER |= GPIO_MODER_MODE9_0;                                  //2) Set PF10 and PF9 mode (output)
	GPIOE->PUPDR |= GPIO_PUPDR_PUPD4_0;                                  //3) Set PE4 with PULL-UP (input)

/* inicializa o Systick */
  __disable_irq();
	SysTick->CTRL = 0;                      //desabilita o systick
	SysTick->LOAD = SystemCoreClock/1000;   //carrega systick para tempo de 1 ms
	SysTick->CTRL = 0x7;                    //habilita o SysTick com interrupção
	__enable_irq();
		
/*start with LED off */
  GPIOF->ODR |= (1UL<< LED_PIN);	
		
//Endless loop
	while(1){ 
		   estado = !(GPIOE->IDR & (1UL<< SW_PIN));         //reverse logic for switch
	     if (estado) {
				 GPIOF->ODR &= ~(1UL << LED_PIN);       //turn on LED D1
		     My_delay(250);                         //delay
	       GPIOF->ODR |= (1UL << LED_PIN);        //turn off LED D1
	       My_delay(250);                         //delay
			 }
		}
}

