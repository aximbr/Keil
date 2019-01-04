/*Etapa 3 - Piscar os Leds D1 em PF9 e D2 em PF10 */
#include "stm32f4xx.h"

int main(void){
/*local variable */
	uint32_t i;
	
/*setup GPIO F */
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN_Msk;                     //1) Enable clock on GPIOF
	GPIOF->MODER |= GPIO_MODER_MODE10_0 | GPIO_MODER_MODE9_0;    //2) Set PF10 and PF9 mode (output)
		
 //Endless loop
	while(1){ 
	     GPIOF->ODR &= ~GPIO_ODR_OD9_Msk;       //turn on LED D1
		   for(i=0; i<800000; i++);               //delay
	     GPIOF->ODR |= GPIO_ODR_OD9_Msk;        //turn off LED D1
	     for(i=0; i<800000; i++);               //delay
	     GPIOF->ODR &= ~GPIO_ODR_OD10_Msk;      //turn on LED D2
		   for(i=0; i<800000; i++);               //delay
		   GPIOF->ODR |= GPIO_ODR_OD10_Msk;       //turn off LED D2
		   for(i=0; i<800000; i++);               //delay
	     }
}

