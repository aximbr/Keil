/*Etapa 8 - carregar uma configuração simples com o FreeRTOS */
#include "stm32f4xx.h"
#include "SEGGER_SYSVIEW.h"

#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>

void vPeriodicTask(void *pvParameters)
{
// Establish the task's period.
const TickType_t xDelay = pdMS_TO_TICKS(1000);
      TickType_t xLastWakeTime = xTaskGetTickCount();
     
	for (;;) {
// Block until the next release time.
   vTaskDelayUntil(&xLastWakeTime, xDelay);
   }
}

int main()
{
SEGGER_SYSVIEW_Conf();

SEGGER_SYSVIEW_Start();
xTaskCreate(vPeriodicTask, "My Task", 30, NULL, 1, NULL);
// Startup of the FreeRTOS scheduler. The program should block here.
vTaskStartScheduler();
	
// The following line should never be reached. Failure to allocate enough
// memory from the heap would be one reason.
   for (;;);
	
}
