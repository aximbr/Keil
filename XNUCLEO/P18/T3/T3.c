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
  * when reaches this time the UIE Bit (Update Event) sets using interrupt
	* TIM1_UP_IRQHandler. This handler clear the bit and  togles on the LED.
	* The result is the LED  togles at 1 Hz rate.	
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
void NVIC_Configuration(void);

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
	
	/* initialize interruptions */
	NVIC_Configuration();
	
	
	while(1) {

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
	TIM_SelectOnePulseMode(TIM1, TIM_OPMode_Repetitive);
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); //enable Interrupt when Update Bit is set	
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

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM1 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
}

/* TIM1_UP_IRQHandler(void) declared on stm32f10x_it.h and defined at stm32f10x_it.c */

