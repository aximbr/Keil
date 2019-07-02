/**
  ******************************************************************************
  * @file    main.c 
  * @author  José P. Leitão
  * @version V1.0
  * @date    2019-06-28
  * @brief   PWM signal on TPM0 with interrupt
  ******************************************************************************
  * @attention
  *
  * This simple experiment, load the TPM0 with its max value (0xFFFF),
  * Load C0V with a value that represets 10% or total time.
  * While counter <= C0V the output (PD0) is high, then goes low until Timeout,
	* and then restart again.
  * <h2><center>&copy; COPYRIGHT 2019 JPL</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "MKL25Z4.h"                    // Device header
#include <stdio.h>

/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
void Timer0_Init(void);
void SIM_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Init(void);
void TPM0_IRQHandler(void);

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	/*Main clock setup @ 48 MHz*/
	SystemInit();
	
	/* System Clocks Configuration */
  SIM_Configuration();

	/* LED Initialization */
	GPIO_Configuration();
		
	/* Timer 1 Initialization */
	Timer0_Init();
	
	/* Enable Interrupts */
	NVIC_Init();
	
	while(1) {
			

	}
		
		 	
}

void Timer0_Init(void){
	/* Clock = 48.000.000 Hz
	   Max Prescaler = 128  // 48000000/128 => counter clock is 375 KHz
	   Max Timer = 2^16 * (1/375K) = 0,1747 sec
	*/
	  TPM0->SC = 0x00;                 //clear configuration
	  TPM0->SC |= TPM_SC_PS(7);        //Set prescaler to 128
	  TPM0->SC |= TPM_SC_TOIE(1);      //enable Time-out interrupt
	  TPM0->MOD = 0xFFFF;              //Load with max value
	  //TPM0->CONF |= TPM_CONF_CSOO(1);  //one-shot mode
	  TPM0_C0SC = 0x00;
	  TPM0_C0SC |= TPM_CnSC_MSB(1) | TPM_CnSC_MSA(0) | TPM_CnSC_ELSB(1) | TPM_CnSC_ELSA(0);
	  TPM0_C0V  = 0x1999;
	  TPM0->SC |= TPM_SC_CMOD(1);      //enable TPM0
	

}


/* local functions */
void SIM_Configuration(void)
{
 /* TPM0 clock source  [Default values] */
 SIM->SOPT2 |= SIM_SOPT2_CLKOUTSEL(1); //enable MCGFLLCLK or MCGPLLCLK/2 as clock source to TPM modules
 SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL(1); //define MCGPLLCLK/2 as clock source to TPM modules
 /* Enable Clock on TPM0 */
 SIM->SCGC6 |= SIM_SCGC6_TPM0(1);      //Enable clock on TPM0
 /* GPIOD clock enable */
 SIM->SCGC5 |= SIM_SCGC5_PORTD(1);     //Enable clock on  Port D
	
}

void GPIO_Configuration(void)
{ 

  /* LED on PD.1 configuration */
	PORTD->PCR[1] |= PORT_PCR_MUX(1);  //Port D 1 as GPIO
	GPIOD->PDDR |= (1UL << 1);         //PD.1 as Output
	GPIOD->PSOR |= (1UL << 1);         //starts with Blue LED off
	
	/* Output of comparator TPM0, channel 0 */
	PORTD->PCR[0] |= PORT_PCR_MUX(4);  //Alternate Function 4 for PD.0
}

void NVIC_Init(void)
{
	__NVIC_SetPriority(TPM0_IRQn, 0);  //set interrupt priority (optional)
	__NVIC_EnableIRQ(TPM0_IRQn);       //enable TPM0 interrupt
	__enable_irq();                    //enable globally interrupt
}

void TPM0_IRQHandler(void)
{
	if ( TPM0->SC & TPM_SC_TOF_MASK )
		   {GPIOD->PTOR |= (1UL << 1);      //togle PD.1 Blue LED
			  TPM0->SC |= TPM_SC_TOF(1);}     //clear TOF Bit 
}
