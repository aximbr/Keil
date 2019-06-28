/**
  ******************************************************************************
  * @file    main.c 
  * @author  José P. Leitão
  * @version V1.0
  * @date    2019-06-25
  * @brief   PWM on Timer1
  ******************************************************************************
  * @attention
  *
  * This simple experiment load timer 1 with period of 1 sec, and a value to 
  *  be compared (CCR1) that determines the duty cicle of PWM signal.
  *  The counter is setting to be Down-counter. While the counter value is
  *  less than CCR1 the output (TIM1_1) is high or the complementary output
  *  (TIM_1N) is low. When counter reaches zero (udpate event), the CCR1 is 
  *  incremented and new duty cicle is set.	
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
TIM_OCInitTypeDef  TIM_OCInitStructure = {0};
TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
uint16_t PrescalerValue = 0;
uint16_t CCR1Val = 0;

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
	
	/* Channel1 duty cycle (20%), the CCR1 must be 20% of ARR ( 1125*20/100 = 225 ) */
	while(1) {
	    if (CCR1Val < 1125) {TIM1->CCR1 = CCR1Val;}
			else  {CCR1Val =  0;}		
	}
	
}

void Timer1_Init(void){
	/* Clock = 72.000.000
	   Prescaler = 2048 -1  // 72000000/2048 = 35156
	   Load = 35156 -1  => 1 sec
	*/
/* -----------------------------------------------------------------------
   TIM1 Configuration: generate 1 PWM signal.
   The TIM1CLK frequency is set to SystemCoreClock (72 MHz), to get TIM1 counter
   clock at 1125 Hz the Prescaler is computed as following:
   - Prescaler = (TIM1CLK / TIM1 counter clock) - 1

   The TIM1 is running at 1 Hz: TIM1 Frequency = TIM1 counter clock/(ARR + 1)
                                                = 1125 / 1125 = 1 Hz
   TIM1 Channel1 duty cycle = (TIM1_CCR1/ TIM1_ARR)* 100 = 20%
----------------------------------------------------------------------- */
/* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock / 1125) - 1;
	

		
	MyTimer_Structure.TIM_ClockDivision = TIM_CKD_DIV1;
	MyTimer_Structure.TIM_CounterMode = TIM_CounterMode_Down;
	MyTimer_Structure.TIM_Period = 1125;
	MyTimer_Structure.TIM_Prescaler = PrescalerValue;
	MyTimer_Structure.TIM_RepetitionCounter = 0x00;

  TIM_TimeBaseInit(TIM1, &MyTimer_Structure);
//	TIM_SelectOnePulseMode(TIM1, TIM_OPMode_Repetitive);  //default
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
 /* GPIOC clock enable */
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
/* GPIOA clock enable */
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
/* GPIOB clock enable */
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
/* AFIO module clock enable */
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
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
	
/* TIM1_1 on PA.8 configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
/* TIM1_1N on PB.13 configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Remap TIM1_CH1 on PA8 pin */
// GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1, ENABLE);
	
}

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM1 global Interrupt */
  //NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	
}

/* TIM1_UP_IRQHandler(void) declared on stm32f10x_it.h and defined at stm32f10x_it.c */
/* TIM1_CC_IRQHandler(void) declared on stm32f10x_it.h and defined at stm32f10x_it.c */


