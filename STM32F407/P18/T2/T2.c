/**
  Project 18 - Experience with Timers on STM32F407ZG
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"                  // Device header


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef MyTimer_Structure;

/* Private function prototypes -----------------------------------------------*/
void Timer1_Init(void);
void RCC_Configuration(void);
void GPIO_Configuration(void); 
 
/* Global variables ----------------------------------------------------------*/
uint32_t meuclock;

/* Private functions ---------------------------------------------------------*/

void Timer1_Init(void){
	/* default clock is using internal oscilator @ 16 MHz and no pre-scaler for
	   AHBus neither APxBus
	   Clock = 16.000.000
	   Prescaler = 16000 -1  // 16.000.000/16.000 = 1.000
	   Load = 1000-1  => 1 sec
	*/
	MyTimer_Structure.TIM_ClockDivision = TIM_CKD_DIV1;
	MyTimer_Structure.TIM_CounterMode = TIM_CounterMode_Up;
	MyTimer_Structure.TIM_Period = 1000-1;
	MyTimer_Structure.TIM_Prescaler = 16000-1;
	MyTimer_Structure.TIM_RepetitionCounter = 0x00;

  TIM_TimeBaseInit(TIM1, &MyTimer_Structure);
	//TIM_SelectOnePulseMode(TIM1, TIM_OPMode_Single);  //set for no periodic (one-shot)
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);
	
  TIM_Cmd(TIM1, ENABLE);	
}

/**
   * @brief  Configures the different system clocks.
   * @param  None
   * @retval None
   */
void RCC_Configuration(void)
{
 /* TIM1 clock enable */
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
 /* GPIOF clock enable */
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
}

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

/* LED on PF.9 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;

  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_WriteBit(GPIOF, GPIO_Pin_9, Bit_RESET); //turn-off LED
}



/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{

/* Insert system clock initialization code here. */
  SystemInit();
	
/* Enable clock to modules */	
  RCC_Configuration();

/* LED Initialization */
	GPIO_Configuration();
		
/* Timer 1 Initialization */
	Timer1_Init();
	
	
	while(1) {
			if ( TIM_GetFlagStatus(TIM1, TIM_FLAG_Update))
		   {GPIO_WriteBit(GPIOF, GPIO_Pin_9, (BitAction) (1- GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_9))); //toggle LED
				TIM_ClearFlag(TIM1,TIM_FLAG_Update);                                           //clear Timeout flag
			 }
		
	}
				 
}

