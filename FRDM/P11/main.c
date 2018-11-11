/* Etapa 11 - piscar 1 LED e enviar mensagem para o display quando uma chave for pressionada */
/* todas as tarefas usando o FreeRTOS                                                        */
/* José P. Leitão                                                                            */
/* 2018-10-29                                                                                */

#include <MKL25Z4.h>
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "Nokia5110.h"

/* SW (A1 = PTB1)
 * LED (D13 = PTD1)
 */

#define LED_RED (1UL<<18)  //PTB18
#define SW      (1UL<<1)   //PTB1

portBASE_TYPE volatile resultado;
xQueueHandle MyQ;  //fila para transferir dados de uma task para outra

//Task para fazer ler o switch

void vReadSwitch(void *pvParameters){
	uint32_t sw_status=0;   //status inicial é released
// Estabelece a duração da task
	  const TickType_t xDelay = pdMS_TO_TICKS(50);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
// Block until the next release time.
		sw_status = !(GPIOB->PDIR & SW);  //lê status da chave
		xQueueSendToBack( MyQ, &sw_status, xDelay);
			
    //vTaskDelayUntil(&xLastWakeTime, xDelay);
		}
}

//Task para fazer o Led Vermelho Piscar

void vBlinkRedLed(void *pvParameters){
    uint32_t sw_status=0;
	// Estabelece a duração da task
	  const TickType_t xDelay = pdMS_TO_TICKS(250);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
		xQueueReceive(MyQ, &sw_status, xDelay);	
// Block until the next release time.
		if (sw_status) GPIOB->PTOR |= LED_RED; //Toggle Red LED 
			else GPIOB->PSOR |= LED_RED;  //apaga o LED
		vTaskDelayUntil(&xLastWakeTime, xDelay);
		}
}

//Task para exibir uma mensagem no display Nokia
void vDiplay_String_Nokia(void *pvParameters){
	uint32_t sw_status=0;
	const TickType_t xDelay = pdMS_TO_TICKS(500);
		
  TickType_t xLastWakeTime = xTaskGetTickCount();
  for (;;) {
   // Block until the next release time.
		  xQueueReceive(MyQ, &sw_status, xDelay);
	    Nokia5110_SetCursor(0,0);				
		  if (sw_status) {Nokia5110_OutString("SW pressed");}
			    else {Nokia5110_OutString("SW free");}
			vTaskDelayUntil(&xLastWakeTime, xDelay);
		  Nokia5110_Clear();
	}
}	
		

int main(void){
			
	/*inicializa o modulo da Porta B e D*/
	SIM->SCGC5 |= SIM_SCGC5_PORTB(1);   //Habilita o clock na PortaB 
	PORTB->PCR[1] |= PORT_PCR_MUX(1);  //Função de GPIO //SW
	PORTB->PCR[18] |= PORT_PCR_MUX(1); //Função de GPIO //Led
	PORTB->PCR[1] |= PORT_PCR_PE(1);   //Pull-up/down enable
	PORTB->PCR[1] |= PORT_PCR_PS(1);   //Pull-up enable
	
	GPIOB->PDDR &= ~SW; //Porta B como input 
	
	GPIOB->PDDR |= LED_RED;   //Porta B como output (pinos 18)
	GPIOB->PCOR |= LED_RED;   //coloca 0 na PORTA B pino 18, onde está o LED Vermelho (acende com ZERO)
	
	
	/*Inicializa módulo SPI e o módulo Nokia5110 */
	Nokia5110_Init();
	Nokia5110_Clear();
  	
	/* cria a fila para receber o dado do botão */
  MyQ = xQueueCreate(1,sizeof(uint32_t));	 //cria uma fila para um dado do tipo uint32_t
	
	if (MyQ == NULL) 
	{ 
		//não consegui criar a fila }
	}
	
	/* cria as tasks do FreeRTOS */
	 resultado = xTaskCreate(vReadSwitch, "SW2 status", 100, NULL, 1, NULL);
	 resultado = xTaskCreate(vBlinkRedLed, "Red Led", 100, NULL, 1, NULL);
	 resultado = xTaskCreate(vDiplay_String_Nokia, "Nokia 5110", 100, NULL, 1, NULL); 
	
// Startup of the FreeRTOS scheduler. The program should block here.
  vTaskStartScheduler();
// The following line should never be reached. Failure to allocate enough
// memory from the heap would be one reason.
for (;;);    
	
  }

