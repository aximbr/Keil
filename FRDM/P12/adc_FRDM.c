/* ADC LIB para FRDM */
#include "adc_FRDM.h"
#include "MKL25Z4.h"

// This is initialization function to set up the ADC on FRDM MKL25Z board

void ADC0_Init(void){
  uint32_t delay;
  
	SIM->SCGC5 |= SIM_SCGC5_PORTE(1);     // active clock on Port E
	SIM->SCGC6 |= SIM_SCGC6_ADC0(1);      // active clock on ADC
	delay = SIM->SCGC5;                   // allow time to finish activating
	
	// Configure PTE20 (A0) as ADC input
	PORTE->PCR[20] |= PORT_PCR_MUX(0);    // analog function
   	
	// Configure ADC0
	// Use A0 as Analog input
	// Use as Vref=AVCC (3.3V) and Vref-=AVss (0V)
	// Sample resolution is 16 bits unsigned
	// Uses Software trigger
	// Uses BUS Clock
	// Clock is not divided
	ADC0_CFG1 = ADC_CFG1_ADIV(0) | ADC_CFG1_ADLSMP(0) | ADC_CFG1_MODE(3) | ADC_CFG1_ADICLK(0);
	ADC0_CFG2 = ADC_CFG2_MUXSEL(0) | ADC_CFG2_ADACKEN(0) | ADC_CFG2_ADHSC(0) | ADC_CFG2_ADLSTS(0);
	ADC0_SC2 = ADC_SC2_ADTRG(0) | ADC_SC2_REFSEL(0);
   
}


//------------ADC0_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 14-bit result of ADC conversion
uint16_t ADC0_In(void){
  uint16_t result;
	
//writing on SC1A we start a software trigger	
  ADC0_SC1A = ADC_SC1_DIFF(0) | ADC_SC1_ADCH(0);    // 1) Start sampling/conversion on A0 (PTE20)
	while (!(ADC0_SC1A & ADC_SC1_COCO_MASK)) {};      // 2) Wait until finish conversion
	result = ADC0_RA & 0xFFFF;                        // 2) read result
  	
  return result;  
}

