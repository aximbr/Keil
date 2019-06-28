/**
  ******************************************************************************
  * @file    main.c 
  * @author  José P. Leitão
  * @version V1.0
  * @date    2019-06-28
  * @brief   periodic with Timer1
  ******************************************************************************
  * @attention
  *
  * This simple experiment, load the TIMER1 with value that represents 1 sec,
  * when reaches this time the UIE Bit (Update Event) sets and turn on the LED
	* just after 1 sec. Since there´s no repetion, the LED remains on.
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
		   {GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);} //turn-on LED
		else
		   {GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);}  //turn-off LED
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
	TIM_SelectOnePulseMode(TIM1, TIM_OPMode_Single);
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
 /* GPIOA clock enable */
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
}

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

/* LED on PA.5 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET); //turn-off LED
}

