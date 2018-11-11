/* Etapa 11 - piscar 1 LED e enviar mensagem para o display quando uma chave for pressionada */
/* todas as tarefas usando o FreeRTOS                                                        */
/* José P. Leitão                                                                            */
/* 2018-10-29                                                                                */

#include "msp.h"
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "Nokia5110.h"

#define LED_RED BIT0   //P1.0
#define SW2     BIT4   //P1.4

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
		sw_status = !(P1->IN & SW2);  //lê status da chave
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
		if (sw_status) P1->OUT ^= LED_RED; //Toggle Red LED 
			else P1->OUT &= ~LED_RED;  //apaga o LED
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
			
	//Stop watchdo timer
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
	
	//Configura P1.0 como saída e P1.4 como entrada
	P1->DIR |= BIT0;
  P1->DIR &= ~BIT4;	
  P1->REN |= BIT4;                             // Configura Pull-up em P1.4
	P1->OUT |= BIT4;                             // Habilita o Pull-up em P1.4

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
