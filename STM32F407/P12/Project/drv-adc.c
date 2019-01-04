/* Driver para SPI usando STM32F407ZG */
#include "main.h"
#include "drv-adc.h"
#include "stm32f4xx_adc.h"

void adc_init(void){

	ADC_InitTypeDef ADC_Config;
	ADC_CommonInitTypeDef ADC_Config_common;
	GPIO_InitTypeDef ADC_Pin_Config;
	
	ADC_Config.ADC_Resolution = ADC_Resolution_12b;
  ADC_Config.ADC_ScanConvMode = DISABLE;
  ADC_Config.ADC_ContinuousConvMode = DISABLE;
  ADC_Config.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_Config.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_Config.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Config.ADC_NbrOfConversion = 1;
	
	//Load default values for common group structure
	ADC_CommonStructInit(&ADC_Config_common);
	
	//Load default values for GPIO structure
	GPIO_StructInit(&ADC_Pin_Config);
	ADC_Pin_Config.GPIO_Pin = GPIO_Pin_0;
	ADC_Pin_Config.GPIO_Mode = GPIO_Mode_AN;
	
	//Enable clock for ADC Peripheric and GPIO Pins
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	
		
	// Configure PA0 (A0) as ADC input
	GPIO_Init(GPIOA, &ADC_Pin_Config);
   	
	// Configure ADC1
	// Use A0 as Analog input
	// Use as Vref=AVCC (3.3V) and Vref-=AVss (0V)
	// Sample resolution is 12 bits unsigned
	// Uses Software trigger
	// Uses PCLK2 (default)
	// Clock is not divided
	ADC_CommonInit(&ADC_Config_common);
	ADC_Init(ADC1, &ADC_Config);
	ADC_Cmd(ADC1, ENABLE);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_3Cycles);
	
}

uint16_t ADC0_In(void){
	uint16_t result;

	ADC_SoftwareStartConv(ADC1);	
	result = ADC_GetConversionValue(ADC1) & 0x0FFF;      //read result
  	
  return result;  
	
}
