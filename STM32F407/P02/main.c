/*Etapa 2 - Piscar o Led D1 em PF10 */
#include "stm32f4xx.h"

int main(void){
/*local variable */
	uint32_t i;
	
/*setup GPIO F */
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN_Msk;     //1) Enable clock on GPIOF
	GPIOF->MODER |= GPIO_MODER_MODE10_0;         //2) Set PF10 mode (output)
		
//Turn on the LED D2
	GPIOF->ODR &= ~GPIO_ODR_OD10_Msk;
	
 //Endless loop
	while(1){ 
	     for(i=0; i<800000; i++);                //delay
	     GPIOF->ODR |= GPIO_ODR_OD10_Msk;        //turn off  PF10
	     for(i=0; i<800000; i++);                //delay
	     GPIOF->ODR &= ~GPIO_ODR_OD10_Msk;       //turn on PF10
	     }
}

