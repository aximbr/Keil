/*Etapa 1 - Acender o Led user1 em PC13 */
/* Alterada para testar a PA.4 */

/*MCU STM32F103C8 */
#include "stm32f10x.h"
#include "GPIO_STM32F10x.h"
void delay(void){
	int i;
	for (i=0;i<1000000;i++);
}

int main(void){
/*inicializa a placa */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);                //1) Habilita o clock da GPIOA
	GPIO_PinConfigure(GPIOA, 4, GPIO_OUT_PUSH_PULL, GPIO_MODE_OUT2MHZ);  //2) Configura para output vel max de 2Mhz, push-pull            
	       
	
			
	while(1) //Loop infinito
	{ 
		GPIO_PinWrite(GPIOA, 4, 0);
		delay();
		GPIO_PinWrite(GPIOA, 4, 1);
		delay();
	}
}
