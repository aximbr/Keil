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

#define LED4 (1U <<5)  //PC5
#define SW   (1U <<13) //PC13  

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
		sw_status = !(GPIOC->IDR & SW);  //lê status da chave
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
		if (sw_status) GPIOC->ODR ^= LED4; //toggle user LED4
			else GPIOC->BRR |= LED4;  //reset user LED4
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
	/*configura GPIOC para funções do LED e SW */	
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;                       //1) Habilita o clock da GPIOC
	GPIOC->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);          //2) Limpa a configuração e modo de PC5
	GPIOC->CRL |= GPIO_CRL_MODE5_1;                           //3) Configura para output vel max de 2Mhz, open drain             
	GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);        //4) Limpa a configuração e modo de PC13
	GPIOC->CRH |= GPIO_CRH_CNF13_1;                           //5 Configura como Input, PULL-UP
	
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
	 resultado = xTaskCreate(vBlinkRedLed, "LED4", 100, NULL, 1, NULL);
	 resultado = xTaskCreate(vDiplay_String_Nokia, "Nokia 5110", 100, NULL, 1, NULL); 
	
// Startup of the FreeRTOS scheduler. The program should block here.
  vTaskStartScheduler();
// The following line should never be reached. Failure to allocate enough
// memory from the heap would be one reason.
for (;;);    
	
  }
