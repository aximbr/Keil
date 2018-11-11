/* Etapa 9 - piscar 2 Leds usando duas tarefas do FreeRTOS */
#include <MKL25Z4.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>

//Task para fazer o Led Vermelho Piscar

void vBlinkRedLed(void *pvParameters){
// Establish the task's period.
    const TickType_t xDelay = pdMS_TO_TICKS(500);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
// Block until the next release time.
		GPIOB->PTOR |=(1<<18);  //Toggle o BIT18 na PORTA B, fazendo o LED Vermelho piscar
    vTaskDelayUntil(&xLastWakeTime, xDelay);
		}
}

void vBlinkGreenLed(void *pvParameters){
// Establish the task's period.
    const TickType_t xDelay = pdMS_TO_TICKS(800);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
// Block until the next release time.
		GPIOB->PTOR |=(1<<19);  //Toggle o BIT19 na PORTA B, fazendo o LED Verde piscar
    vTaskDelayUntil(&xLastWakeTime, xDelay);
		}
}


int main(void){
/*inicializa o modulo da Porta B*/
	SIM->SCGC5 |= SIM_SCGC5_PORTB(1);   //Habilita o clock na PortaB
	PORTB->PCR[18] |= PORT_PCR_MUX(1);  //Função de GPIO
	PORTB->PCR[19] |= PORT_PCR_MUX(1);  //Função de GPIO
	
	GPIOB->PDDR |=(1<<18) | (1<<19);  //Porta B como output (pinos 18 e 19)
	GPIOB->PCOR |=(1<<18) | (1<<19);  //coloca 0 na PORTA B pinos 18 e 19, onde está o LED Vermelho e Verde (acende com ZERO)
		
//inicializa as duas tasks, com a mesma prioridade
	
	xTaskCreate(vBlinkRedLed, "Red Led", 256, NULL, 1, NULL);
	xTaskCreate(vBlinkGreenLed, "Green Led", 256, NULL, 1, NULL);
	
// Startup of the FreeRTOS scheduler. The program should block here.
  vTaskStartScheduler();
// The following line should never be reached. Failure to allocate enough
// memory from the heap would be one reason.
for (;;);
	
}
