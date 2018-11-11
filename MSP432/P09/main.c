/* Etapa 9 - piscar 2 LED´s usando duas tarefas do FreeRTOS */

#include "msp.h"
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>

void vBlinkRedLed(void *pvParameters){
// Establish the task's period.
    const TickType_t xDelay = pdMS_TO_TICKS(500);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
// Block until the next release time.
		P2->OUT ^= BIT0; //Toggle Red LED (PF2.0)
    vTaskDelayUntil(&xLastWakeTime, xDelay);
		}
}

void vBlinkBlueLed(void *pvParameters){
// Establish the task's period.
    const TickType_t xDelay = pdMS_TO_TICKS(1500);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
// Block until the next release time.
		P2->OUT ^= BIT2;  //Toggle Blue LED (PF2.2)
    vTaskDelayUntil(&xLastWakeTime, xDelay);
		}
}


int main(void){
			
	//Stop watchdo timer
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
	
	//Configura P2.0 e P2.2 como saída (Red Led e Blue Led)
	P2->DIR |= BIT0 |BIT2;                             
  P2->OUT |= BIT0 |BIT2;    //Turn on both LEDs

  	
	//inicializa as duas tasks, com a mesma prioridade
	
	xTaskCreate(vBlinkRedLed, "Red Led", 256, NULL, 1, NULL);
	xTaskCreate(vBlinkBlueLed, "Blue Led", 256, NULL, 1, NULL);
	
// Startup of the FreeRTOS scheduler. The program should block here.
  vTaskStartScheduler();
// The following line should never be reached. Failure to allocate enough
// memory from the heap would be one reason.
for (;;);
}

	