/* Tarefa 11: enviar uma mensagem continuamente para um display e piscar um led quando acionado um botão */
/* todas as tarefas usando o FreeRTOS */
/*                                    */
/* Autor: José P. Leitão              */
/* 2018-10-28                         */
#include "TM4C123.h"
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "nokia5110_tm4c.h"

#define LED_RED (1U <<1)
#define SW2  0x00000001UL

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
		sw_status = !(GPIOF_AHB->DATA & SW2);  //lê status da chave
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
		if (sw_status) GPIOF_AHB->DATA ^= LED_RED; //Toggle Red LED (PF1)
			else GPIOF_AHB->DATA &= ~LED_RED;  //apaga o LED
		vTaskDelayUntil(&xLastWakeTime, xDelay);
		}
}

//Task para exibir uma mensagem no display Nokia
void vDiplay_String_Nokia(void *pvParameters){
	uint32_t sw_status=0;
	const TickType_t xDelay = pdMS_TO_TICKS(500);
	unsigned char j;
	
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
			
int main(void)
{
	uint32_t delay;
	/*Inicializa módulo SPI e o módulo Nokia5110 */
	Nokia5110_Init();
	Nokia5110_Clear();
	
	/* Habilita GPIO da Porta F com o LED Vermelho e um Switch */
	SYSCTL->RCGCGPIO |= (1U << 5);      // 0x20U Enable clock for GPIOF
	delay = SYSCTL->RCGCGPIO;           // delay to allow stabilize
	SYSCTL->GPIOHBCTL |= (1U << 5);     // 0x20U Enable fast GPIO BUS
	GPIOF_AHB->LOCK = 0x4C4F434B;  // unlock GPIO Port F
	GPIOF_AHB->CR = 0x1F;          // allow changes to PF4-0
  GPIOF_AHB->DIR |= LED_RED;     // Set the GPIODIR AH (PF1) as output (00000010) 
  GPIOF_AHB->DEN |= LED_RED;     // Set the GPIODEN AH for Port F (Digital Function) (00000010) RED_LED
  GPIOF_AHB->DEN |= SW2;         // Set the GPIODEN AH for Port F (Digital Function) (00000001) SW2
	
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


