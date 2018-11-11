/* Tarefa 11: enviar uma mensagem continuamente para um display e piscar um led quando acionado um botão */
/* todas as tarefas usando o FreeRTOS */
/*                                    */
/* Autor: José P. Leitão              */
/* 2018-10-28                         */
#include "stm32f10x.h"
#include "GPIO_STM32F10x.h"
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "nokia5110_stm32.h"

#define LED  (1U <<13)  //PC13
#define SW   (1U <<0)   //PA0  

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
		sw_status = !(GPIOA->IDR & SW);  //lê status da chave
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
		if (sw_status) GPIO_PinWrite(GPIOC, 13, ~(GPIO_PinRead(GPIOC,13))); //toggle PC13
			else GPIO_PinWrite(GPIOC,13,1);  //turn off led on PC13
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
		  LCD_write_string(0,0,"          ");
	    if (sw_status) {LCD_write_string(0,0,"SW pressed ");}
			    else {LCD_write_string(0,0,"SW free ");}
			vTaskDelayUntil(&xLastWakeTime, xDelay);
		  
	}
}	
			
int main(void)
{
	/*configura GPIOA e GPIOC para funções do LED e SW */	
	//1) Habilita o clock da GPIOA e GPIOC
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	
	//2) Configura para output vel max de 2Mhz, open drain 
	GPIO_PinConfigure(GPIOA, 0, GPIO_IN_PULL_UP, GPIO_MODE_INPUT);
	
	//3 Configura PA0 como Input, PULL-UP
	GPIO_PinConfigure(GPIOC, 13, GPIO_OUT_OPENDRAIN, GPIO_MODE_OUT2MHZ);
	
	/*Inicializa módulo SPI e o módulo Nokia5110 */
	SPI1_init();
	LCD_init();
	LCD_clear();
	
	/* cria a fila para receber o dado do botão */
  MyQ = xQueueCreate(1,sizeof(uint32_t));	 //cria uma fila para um dado do tipo uint32_t
	
	if (MyQ == NULL) 
	{ 
		//não consegui criar a fila }
	}
	
	/* cria as tasks do FreeRTOS */
	 resultado = xTaskCreate(vReadSwitch, "SW status", 100, NULL, 1, NULL);
	 resultado = xTaskCreate(vBlinkRedLed, "LED", 100, NULL, 1, NULL);
	 resultado = xTaskCreate(vDiplay_String_Nokia, "Nokia 5110", 100, NULL, 1, NULL); 
	
// Startup of the FreeRTOS scheduler. The program should block here.
  vTaskStartScheduler();
// The following line should never be reached. Failure to allocate enough
// memory from the heap would be one reason.
for (;;);    
	
  }
