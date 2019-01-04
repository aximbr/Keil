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
	
#define LED1 (1U <<9)  //PF9
#define SWK1   (1U <<3) //PE3  

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
		sw_status = !(GPIOE->IDR & SWK1);  //lê status da chave
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
		if (sw_status) GPIOF->ODR ^= LED1; //toggle user LED1
			else GPIO_ResetBits(GPIOF, LED1);;  //reset user LED1
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
		  nokia5110_write_string(0,0,"          ");
	    if (sw_status) {nokia5110_write_string(0,0,"SW pressed ");}
			    else {nokia5110_write_string(0,0,"SW free ");}
			vTaskDelayUntil(&xLastWakeTime, xDelay);
		  
	}
}	
			
int main(void)
{
  GPIO_InitTypeDef SW_Init, LED_Init;
  SW_Init.GPIO_Pin = GPIO_Pin_3; //here we can use multiple Pins
	SW_Init.GPIO_Mode = GPIO_Mode_IN;
	SW_Init.GPIO_Speed = GPIO_Speed_25MHz;
	SW_Init.GPIO_OType = GPIO_OType_PP;
	SW_Init.GPIO_PuPd = GPIO_PuPd_UP;

  LED_Init.GPIO_Pin = GPIO_Pin_9; //here we can use multiple Pins
	LED_Init.GPIO_Mode = GPIO_Mode_OUT;
	LED_Init.GPIO_Speed = GPIO_Speed_25MHz;
	LED_Init.GPIO_OType = GPIO_OType_OD;
	LED_Init.GPIO_PuPd = GPIO_PuPd_NOPULL;		
	

   
	/*Inicializa módulo SPI e o módulo Nokia5110 */
  nokia5110_init();
	
	/*configura GPIOF e GPIOE para funções do LED e SW */	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);     //1) Habilita o clock da GPIOF
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);     //1) Habilita o clock da GPIOE
	GPIO_Init(GPIOF, &LED_Init);                              //3) Configura o LED em PF9
	GPIO_Init(GPIOE, &SW_Init);                               //4) Configura o SW em PE3
	
	
	
	/* cria a fila para receber o dado do botão */
  MyQ = xQueueCreate(1,sizeof(uint32_t));	 //cria uma fila para um dado do tipo uint32_t
	
	if (MyQ == NULL) 
	{ 
		//não consegui criar a fila }
	}
	
	/* cria as tasks do FreeRTOS */
	 resultado = xTaskCreate(vReadSwitch, "SW status", 100, NULL, 1, NULL);
	 resultado = xTaskCreate(vBlinkRedLed, "LED1", 100, NULL, 1, NULL);
	 resultado = xTaskCreate(vDiplay_String_Nokia, "Nokia 5110", 100, NULL, 1, NULL); 
	
// Startup of the FreeRTOS scheduler. The program should block here.
  vTaskStartScheduler();
// The following line should never be reached. Failure to allocate enough
// memory from the heap would be one reason.
for (;;);    
	
  }
	

/* End Main */



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
