/* Etapa 9 - fazer dois leds piscarem utilizando duas tarefas no FreeRTOS */
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include "tm4c123gh6pm.h"

#define LED_RED  (1U <<1)
#define LED_BLUE (1U <<2)

//Task para fazer o Led Vermelho Piscar

void vBlinkRedLed(void *pvParameters){
// Establish the task's period.
    const TickType_t xDelay = pdMS_TO_TICKS(500);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
// Block until the next release time.
		GPIO_PORTF_AHB_DATA_R ^= LED_RED; //Toggle Red LED (PF1)
    vTaskDelayUntil(&xLastWakeTime, xDelay);
		}
}

void vBlinkBlueLed(void *pvParameters){
// Establish the task's period.
    const TickType_t xDelay = pdMS_TO_TICKS(1500);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
// Block until the next release time.
		GPIO_PORTF_AHB_DATA_R ^= LED_BLUE; //Toggle Blue LED (PF2)
    vTaskDelayUntil(&xLastWakeTime, xDelay);
		}
}

int main(void){
	uint32_t delay;
	
	
	//inicialização dos LEDs
	SYSCTL_RCGCGPIO_R |= (1U << 5);   // 0x20U Enable clock for GPIOF
  delay = SYSCTL_RCGCGPIO_R;        // Just wait until the clock stabilize    
	SYSCTL_GPIOHBCTL_R |= (1U << 5);  // 0x20U Enable fast GPIO BUS
  GPIO_PORTF_AHB_DIR_R |= LED_RED | LED_BLUE;  //Set the GPIODIR AH (PF1 and PF2) as output (00000110) 
  GPIO_PORTF_AHB_DEN_R |= LED_RED | LED_BLUE;  //Set the GPIODEN AH for Port F (Digital Function) (00000110)

  GPIO_PORTF_AHB_DATA_R |= LED_RED | LED_BLUE;	//Turn ON Red LED (PF1) and Blue LED (PF2)
	
	//inicializa as duas tasks, com a mesma prioridade
	
	xTaskCreate(vBlinkRedLed, "Red Led", 256, NULL, 1, NULL);
	xTaskCreate(vBlinkBlueLed, "Blue Led", 256, NULL, 1, NULL);
	
// Startup of the FreeRTOS scheduler. The program should block here.
  vTaskStartScheduler();
// The following line should never be reached. Failure to allocate enough
// memory from the heap would be one reason.
for (;;);
}

	