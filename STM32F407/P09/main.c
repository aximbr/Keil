/*Etapa 9 - piscar dois Leds utilizando duas tarefas do FreeRTOS */
#include "stm32f4xx.h"


#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>

#define LED1   9
#define LED2   10

void vBlinkLed1(void *pvParameters){
// Establish the task's period.
    const TickType_t xDelay = pdMS_TO_TICKS(500);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
// Block until the next release time.
		GPIOF->ODR |= (1UL << LED1); //reset o LED1
    vTaskDelayUntil(&xLastWakeTime, xDelay);
		GPIOF->ODR &= ~(1UL << LED1);       //set o LED1
		vTaskDelayUntil(&xLastWakeTime, xDelay);
		}
}

void vBlinkLed2(void *pvParameters){
// Establish the task's period.
    const TickType_t xDelay = pdMS_TO_TICKS(800);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
// Block until the next release time.
		GPIOF->ODR |= (1UL << LED2); //reset o LED2
    vTaskDelayUntil(&xLastWakeTime, xDelay);
		GPIOF->ODR &= ~(1UL << LED2);       //set o LED2
		vTaskDelayUntil(&xLastWakeTime, xDelay);
		}
}

int main(void){
/*variaveis locais */
		
/*inicializa a placa */
	
	/*setup GPIO F */
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN_Msk;                     //1) Enable clock on GPIOF
	GPIOF->MODER |= GPIO_MODER_MODE10_0 | GPIO_MODER_MODE9_0;    //2) Set PF10 and PF9 mode (output)
	
	//liga os LEDs LED1 e LED2
	GPIOF->ODR &= ~(1UL <<LED1);
	GPIOC->ODR &= ~(1UL <<LED2);
	
		//inicializa as duas tasks, com a mesma prioridade
	
	xTaskCreate(vBlinkLed1, "Led_1", 256, NULL, 2, NULL);
	xTaskCreate(vBlinkLed2, "Led_2", 256, NULL, 1, NULL);
	
// Startup of the FreeRTOS scheduler. The program should block here.
  vTaskStartScheduler();
// The following line should never be reached. Failure to allocate enough
// memory from the heap would be one reason.
for (;;);
}
