/*Etapa 1 - Acender o Led user1 em PB12 */
/*MCU STM32F103C8T6 */

#include "stm32f10x.h"
#include "GPIO_STM32F10x.h"

int main(void){
/*inicializa a placa */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);                //1) Habilita o clock da GPIOB
	GPIO_PinConfigure(GPIOB, 12, GPIO_OUT_PUSH_PULL, GPIO_MODE_OUT2MHZ);  //2) Configura para output vel max de 2Mhz, push-pull            
	GPIO_PinConfigure(GPIOB, 13, GPIO_OUT_PUSH_PULL, GPIO_MODE_OUT2MHZ);       
	
	//liga o pino PB12 (lógica invertida)
	GPIO_PinWrite(GPIOB, 12, 0);
  GPIO_PinWrite(GPIOB, 13, 1);	
		
	while(1); //Loop infinito
}
