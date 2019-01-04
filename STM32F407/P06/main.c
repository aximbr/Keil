/*Etapa 6 - Através do switch K0 em PE4 acender o Leds D1 em PF9 */
#include "stm32f4xx.h"
#include <stdint.h>

#define SW_PIN  4
#define LED_PIN 9
	
int main(void){
/*local variable */
	uint32_t estado, i;
	
/*setup GPIO F */
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN_Msk | RCC_AHB1ENR_GPIOEEN_Msk;   //1) Enable clock on GPIOF and GPIOE
	GPIOF->MODER |= GPIO_MODER_MODE9_0;                                  //2) Set PF10 and PF9 mode (output)
	GPIOE->PUPDR |= GPIO_PUPDR_PUPD4_0;                                  //3) Set PE4 with PULL-UP (input)
	
/*start with LED off */
  GPIOF->ODR |= (1UL<< LED_PIN);	
		
//Endless loop
	while(1){ 
		   estado = !(GPIOE->IDR & (1UL<< SW_PIN));         //reverse logic for switch
	     if (estado) GPIOF->ODR ^= (1UL<< LED_PIN);
		   for(i=0;i<50;i++);
		}
}

