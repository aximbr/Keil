/**
  Tarefa 11: enviar uma mensagem continuamente para um display e piscar um led quando acionado um botão 
  todas as tarefas usando o FreeRTOS
                                   
  Autor: José P. Leitão
  2018-10-28
	utiliza a biblioteca padrão STM32F4xxx
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "drv-nokia5110.h"
#include "drv-adc.h"
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>


/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
	
//protótipo de funções locais
uint32_t convert_adc(unsigned long x);
void vReadADC(void *pvParameters);
void vDiplay_String_Nokia(void *pvParameters);


#define ADC_RESOLUTION  12

// variáveis globais
xQueueHandle MyQ;  //fila para transferir dados de uma task para outra

			
int main(void)
{
  /*Inicializa módulo SPI e o módulo Nokia5110 */
  nokia5110_init();
	
	/*Inicializa móduloa ADC */
	adc_init();
	
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
/* End Main */

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
	
	nokia5110_set_XY(0,0);
	nokia5110_write_string("Projeto 12");
	
  for (;;) {
   // Block until the next release time.
		  xQueuePeek(MyQ, &valor, xDelay);
	    nokia5110_set_XY(0,1);
      nokia5110_OutUDec( (unsigned short) valor );
		  nokia5110_set_XY(7,1);
      nokia5110_write_string("mV");		
			vTaskDelayUntil(&xLastWakeTime, xDelay);
		  
	}
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
