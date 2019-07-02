/**
  ******************************************************************************
  * @file    main.c 
  * @author  José P. Leitão
  * @version V1.0
  * @date    2019-06-28
  * @brief   periodic with TPM0 (KL25)
  ******************************************************************************
  * @attention
  *
  * This simple experiment, load the TPM0 with its max value (0xFFFF),
  * when the counter reaches this value the TOF Bit (Update Event) sets and
  * the Blue Led on PD.1	turns on
	* After reaches Timeout, the bit is clear and counter reset, the LED togles.
	* The time depends the clock setting, the prescaler.
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
	
	
	while(1) {
			if ( TPM0->SC & TPM_SC_TOF_MASK )
		   {GPIOD->PTOR |= (1UL << 1);      //togle PD.1 Blue LED
			  TPM0->SC |= TPM_SC_TOF(1);}     //clear TOF Bit

	}
		
		 	
}

void Timer0_Init(void){
	/* Clock = 48.000.000 Hz
	   Max Prescaler = 128  // 48000000/128 => counter clock is 375 KHz
	   Max Timer = 2^16 * (1/375K) = 0,1747 sec
	
	   Clock = 24.000.000 Hz
	   Max Prescaler = 128  // 24000000/128 => counter clock is 187,5 KhZ
	   Max Timer = 2^16 * (1/187,5K) = 0,3495 sec
	*/
	  TPM0->SC = 0x00;                 //clear configuration
	  TPM0->SC |= TPM_SC_PS(7);        //Set prescaler to 128
	  TPM0->MOD = 0xFFFF;              //Load with max value
	  //TPM0->CONF |= TPM_CONF_CSOO(1);  //one-shot mode
	  TPM0->SC |= TPM_SC_CMOD(1);      //enable TPM0
}


/* local functions */
void SIM_Configuration(void)
{
 /* TPM0 clock source */
 SIM->SOPT2 |= SIM_SOPT2_CLKOUTSEL(1); //enable MCGPLLCLK or MCGFLLCLK as clock source to TPM modules
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
}

