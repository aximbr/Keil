/* ADC LIB para FRDM */
#include "adc_STM32F103.h"
#include "STM32f10x.h"
#include "GPIO_STM32F10x.h"
#include "stm32f10x_adc.h"

// This is initialization function to set up the ADC on STM32F103 board

void ADC0_Init(void){
  
	ADC_InitTypeDef ADC_Config;
	
	ADC_Config.ADC_Mode = ADC_Mode_Independent;
  ADC_Config.ADC_ContinuousConvMode = DISABLE;
  ADC_Config.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_Config.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_Config.ADC_NbrOfChannel = 1;
  ADC_Config.ADC_ScanConvMode = DISABLE;
	
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   // active clock on Port A
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);    // active clock on ADC1
	
	
	
	// Configure PA0 (A0) as ADC input
	GPIO_PinConfigure(GPIOA, 0, GPIO_IN_ANALOG, GPIO_MODE_INPUT);
	
   	
	// Configure ADC1
	// Use A0 as Analog input
	// Use as Vref=AVCC (3.3V) and Vref-=AVss (0V)
	// Sample resolution is 12 bits unsigned
	// Uses Software trigger
	// Uses PCLK2 (default)
	// Clock is not divided
	
	ADC_Init(ADC1, &ADC_Config);
	ADC_Cmd(ADC1, ENABLE);
	ADC_RegularChannelConfig(ADC1, 0, 1, ADC_SampleTime_1Cycles5);
	
		 
}


//------------ADC0_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint16_t ADC0_In(void){
  uint16_t result;
	

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
	result = ADC_GetConversionValue(ADC1) & 0x0FFF;      //read result
  	
  return result;  
}

