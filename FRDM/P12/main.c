/*Etapa 12                                                        */
/*Projeto utizando ADC, um potenciômetro como divisor de tensão   */
/*FreeRTOS e um display nokia 5110 para indicar o valor da tensão */
/*Autor: José P. Leitão                                           */
/*2018-11-15                                                      */
/*Keil v5                                                         */

#include "MKL25Z4.h"
#include "adc_FRDM.h"
#include "Nokia5110_FRDM.h"
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#define ADC_RESOLUTION  16

//protótipo de funções locais
uint32_t convert_adc(unsigned long x);
void vReadADC(void *pvParameters);
void vDiplay_String_Nokia(void *pvParameters);

// variáveis globais
xQueueHandle MyQ;  //fila para transferir dados de uma task para outra

// programa principal
int main(void){
		
	/* inicializa o módulo ADC0 */
	ADC0_Init();
	
	/*Inicializa módulo SPI e o módulo Nokia5110 */
	Nokia5110_Init();
	Nokia5110_Clear();
	
	/* cria a fila para receber o dado do botão */
  MyQ = xQueueCreate(1,sizeof(uint32_t));	 //cria uma fila para um dado do tipo uint32_t
	/* cria as tasks do FreeRTOS */
	 xTaskCreate(vReadADC, "Ler ADC", 100, NULL, 1, NULL);
	 xTaskCreate(vDiplay_String_Nokia, "Nokia 5110", 100, NULL, 1, NULL); 
	
// Startup of the FreeRTOS scheduler. The program should block here.
  vTaskStartScheduler();
// The following line should never be reached. Failure to allocate enough
// memory from the heap would be one reason.
for (;;);    
	
  
}

/*função que converte o valor lido do ADC */
uint32_t convert_adc(unsigned long x){
	uint32_t valor;
	
	valor = ((x * 3300) >> ADC_RESOLUTION); //resultado em milhares de milivolts
	return valor;
	
}

/*implementação das funções locais */

//Task para fazer ler o adc Fifo
void vReadADC(void *pvParameters){
	unsigned long adc_read;
	uint32_t my_valor;
	// Estabelece a duração da task
	  const TickType_t xDelay = pdMS_TO_TICKS(50);
    TickType_t xLastWakeTime = xTaskGetTickCount();
	
	while(1){
		adc_read = ADC0_In();
		my_valor = convert_adc( adc_read );
	
    // Block until the next release time.
		
		xQueueOverwrite( MyQ, &my_valor);
			
    vTaskDelayUntil(&xLastWakeTime, xDelay);
		}
}

//Task para exibir uma mensagem no display Nokia
void vDiplay_String_Nokia(void *pvParameters){
	uint32_t valor=0;
	const TickType_t xDelay = pdMS_TO_TICKS(500);
		
  TickType_t xLastWakeTime = xTaskGetTickCount();
	
	Nokia5110_SetCursor(0,0);
	Nokia5110_OutString("Projeto 12");
  for (;;) {
   // Block until the next release time.
		  xQueuePeek(MyQ, &valor, xDelay);
	    Nokia5110_SetCursor(0,1);
      Nokia5110_OutUDec( (unsigned short) valor );
      Nokia5110_SetCursor(7,1);
      Nokia5110_OutString("mV");		
			vTaskDelayUntil(&xLastWakeTime, xDelay);
		  //Nokia5110_Clear();
	}
}

