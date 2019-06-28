/**
  ******************************************************************************
  * @file    main.c 
  * @author  José P. Leitão
  * @version V1.0
  * @date    2019-06-25
  * @brief   On-Shot with Timer1
  ******************************************************************************
  * @attention
  *
  * This simple experiment, load the TIMER1 with value that represents 1 sec,
  * when reaches this time the UIE Bit (Update Event) sets, togles on the LED
	* and reset the UIE Bit, just to restart the process. The result is the LED
  * togles at 1 Hz rate.	
  * <h2><center>&copy; COPYRIGHT 2019 JPL</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <stdio.h>

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef MyTimer_Structure;
	

/* Private function prototypes -----------------------------------------------*/
void Timer1_Init(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	/* System Clocks Configuration */
  RCC_Configuration();

	/* LED Initialization */
	GPIO_Configuration();
		
	/* Timer 1 Initialization */
	Timer1_Init();
	
	
	while(1) {
	
      if ( TIM_GetFlagStatus(TIM1, TIM_FLAG_Update))
		   {GPIO_WriteBit(GPIOC, GPIO_Pin_13, !(GPIO_ReadOutputDataBit( GPIOC, GPIO_Pin_13))); //togle LED
			  TIM_ClearFlag(TIM1,TIM_FLAG_Update);}         //clear update flag

	}
	
}

void Timer1_Init(void){
	/* Clock = 72.000.000
	   Prescaler = 2048 -1  // 72000000/2048 = 35156
	   Load = 35156 -1  => 1 sec
	*/
	MyTimer_Structure.TIM_ClockDivision = TIM_CKD_DIV1;
	MyTimer_Structure.TIM_CounterMode = TIM_CounterMode_Up;
	MyTimer_Structure.TIM_Period = 35156-1;
	MyTimer_Structure.TIM_Prescaler = 2048-1;
	MyTimer_Structure.TIM_RepetitionCounter = 0x00;

  TIM_TimeBaseInit(TIM1, &MyTimer_Structure);
	TIM_SelectOnePulseMode(TIM1, TIM_OPMode_Repetitive);  //default 
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
 /* GPIOC clock enable */
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
}

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

/* LED on PC.13 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET); //turn-off LED
}

