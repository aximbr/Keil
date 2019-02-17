/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    2019-02-17
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Fazer um LED piscar usando um Timer (Systick) e recurso de Interrupção do micro controlador; 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_gpio.h"

/** @addtogroup STM32F0xx_StdPeriph_Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t volatile tick_count;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Função que cria delay em X *1ms */
void My_delay(uint32_t delay_ms){
	tick_count = 0;
	while (tick_count < delay_ms);
	}
	

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
GPIO_InitTypeDef MyGPIO;
MyGPIO.GPIO_Pin = GPIO_Pin_4;
MyGPIO.GPIO_Mode = GPIO_Mode_OUT;
MyGPIO.GPIO_Speed = GPIO_Speed_Level_1;
MyGPIO.GPIO_OType = GPIO_OType_PP;
MyGPIO.GPIO_PuPd = GPIO_PuPd_UP;
	
/* inicializa o Systick */
  __disable_irq();
	SysTick->CTRL = 0;                      //desabilita o systick
	SysTick->LOAD = SystemCoreClock/1000;   //carrega systick para tempo de 1 ms
	SysTick->CTRL = 0x7;                    //habilita o SysTick com interrupção
	__enable_irq();
	
	
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */ 
      
  /* Add your application code here
     */
	/*inicializa a placa */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); //1) Habilita clock na GPIOA
	GPIO_Init(GPIOA, &MyGPIO);                          //2) Inicializa a porta PA4, Push-pull, baixa velocidade           
	       

  /* Infinite loop */
  while (1)
  {
   GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
   My_delay(250);
   GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
   My_delay(250);
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
