/**
  ******************************************************************************
  * @file    main.c 
  * @author  José P. Leitão
  * @version V1.0
  * @date    2019-01-01
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <stdio.h>

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_Init_Config;

/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
void delay(void){
	int i;
	for (i=0;i<1000000;i++);
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
GPIO_StructInit( &GPIO_Init_Config);
GPIO_Init_Config.GPIO_Pin = GPIO_Pin_13;
GPIO_Init_Config.GPIO_Mode = GPIO_Mode_Out_PP;

GPIO_Init( GPIOC, &GPIO_Init_Config);

	while(1){
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, 1);
		delay();
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, 0);
		delay();
	}
	
	
}
