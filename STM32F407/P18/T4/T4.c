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
TIM_OCInitTypeDef  TIM_OCInitStructure;
uint16_t CCR1Val = 0;

/* Private function prototypes -----------------------------------------------*/
void Timer1_Init(void);
void RCC_Configuration(void);
void GPIO_Configuration(void); 
void NVIC_Configuration(void);

/* Global variables ----------------------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

void Timer1_Init(void){
	/* default clock is internal oscilator @ 16 MHz and no pre-scaler for
	   AHBus neither APxBus
	   Clock = 16.000.000
	   Prescaler = 16000 -1  // 16.000.000/16.000 = 1.000
	   Load = 1000-1  => 1 sec
	   TIM1 Channel1 duty cycle = (CCR1Val/ load)* 100 = x %
	*/
	MyTimer_Structure.TIM_ClockDivision = TIM_CKD_DIV1;
	MyTimer_Structure.TIM_CounterMode = TIM_CounterMode_Down;
	MyTimer_Structure.TIM_Period = 1000-1;
	MyTimer_Structure.TIM_Prescaler = 16000-1;
	MyTimer_Structure.TIM_RepetitionCounter = 0x00;

  TIM_TimeBaseInit(TIM1, &MyTimer_Structure);

	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); //enable Interrupt when Update Bit is set	
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);
	
	/* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM1, ENABLE);
	
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);	//enable Interrupt when match counter and compare
	TIM_ClearFlag(TIM1,TIM_FLAG_CC1);
		
  TIM_CtrlPWMOutputs(TIM1, ENABLE);  //enable PWM Outputs
	
  TIM_Cmd(TIM1, ENABLE);	           //enable timer
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
 /* GPIOE clock enable */
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
 /* GPIOA clock enable */
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);


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
	
/* TIM1_1 on PE.9 configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
	
/* TIM1_1N on PA.7 configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM1);
	
}

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM1 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
	
	/* Enable the TIM1 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	
	NVIC_Init(&NVIC_InitStructure);
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
	
/* NVIC Initialization */
	NVIC_Configuration();
	
	
	while(1) {
			
			 }
						 
}

