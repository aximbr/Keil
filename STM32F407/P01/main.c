/*teste do STM32F407ZGT6 */
#include "stm32f4xx.h"
#include <stdint.h>


int main(void){
	uint32_t delay;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN_Msk;
	delay = RCC->AHB1ENR;
	
	GPIOF->MODER |= GPIO_MODER_MODE10_0;
	
	while(1) {
		GPIOF->ODR |= GPIO_ODR_OD10_Msk;
		for (int j=0; j<400000; j++);
		GPIOF->ODR &= ~GPIO_ODR_OD10_Msk;
	  for (int j=0; j<400000; j++);
	}
}

