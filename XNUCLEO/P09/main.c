/* Etapa 9 - piscar dois Leds usando duas tarefas do FreeRTOS */

#include "stm32f10x.h"
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>

void vBlinkLed1(void *pvParameters){
// Establish the task's period.
    const TickType_t xDelay = pdMS_TO_TICKS(500);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
// Block until the next release time.
		GPIOA->BSRR |= (1UL << (16+5)); //reset o PA5
    vTaskDelayUntil(&xLastWakeTime, xDelay);
		GPIOA->BSRR |= (1UL <<5);       //set o PA5
		vTaskDelayUntil(&xLastWakeTime, xDelay);
		}
}

void vBlinkLed2(void *pvParameters){
// Establish the task's period.
    const TickType_t xDelay = pdMS_TO_TICKS(800);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
// Block until the next release time.
		GPIOC->BSRR |= (1UL << 9);      //set o PC9
	  vTaskDelayUntil(&xLastWakeTime, xDelay);
	  GPIOC->BSRR |= (1UL << (16+9)); //reset o PC9
		vTaskDelayUntil(&xLastWakeTime, xDelay);
    }
}

int main(void){
/*variaveis locais */
		
/*inicializa a placa */
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;  //1) Habilita o clock da GPIOA e GPIOC
	GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);          //2) Limpa a configuração e modo de PA5
	GPIOC->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);          //2) Limpa a configuração e modo de PC9
	GPIOA->CRL |= GPIO_CRL_MODE5_1;                           //3) Configura para output vel max de 2Mhz, open drain             
	GPIOC->CRH |= GPIO_CRH_MODE9_1;                           //3) Configura para output vel max de 2Mhz, open drain 
	
	//liga o pino PA5 e PC9
	GPIOA->BSRR |= (1UL <<5);
	GPIOC->BSRR |= (1UL <<9);
	
		//inicializa as duas tasks, com a mesma prioridade
	
	xTaskCreate(vBlinkLed1, "Led_1", 256, NULL, 2, NULL);
	xTaskCreate(vBlinkLed2, "Led_2", 256, NULL, 1, NULL);
	
// Startup of the FreeRTOS scheduler. The program should block here.
  vTaskStartScheduler();
// The following line should never be reached. Failure to allocate enough
// memory from the heap would be one reason.
for (;;);
}
