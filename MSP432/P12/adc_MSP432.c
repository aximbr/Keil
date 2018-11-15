/* ADC LIB para TM4C */
#include "adc_msp432.h"
#include "MSP432.h"

// This is initialization function to set up the ADC on MSP432

void ADC0_Init(void){
    
	P5->SEL1 |= BIT5;     // Configure P5.5 (A0) as ADC input
  P5->SEL0 |= BIT5;
	
	// Configure ADC14
	// Use A0 as Analog input and stores sample at MEM[0]
	// Use as Vref=AVCC (3.3V) and Vref-=AVss (0V)
	// Sample resolution is 14 bits
	
	ADC14->CTL0 = ADC14_CTL0_CONSEQ_0 | ADC14_CTL0_ON;
	ADC14->CTL1 = ADC14_CTL1_RES__14BIT;
  ADC14->MCTL[0] |= ADC14_MCTLN_INCH_0; 
   
}


//------------ADC0_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 14-bit result of ADC conversion
uint16_t ADC0_In(void){
  uint16_t result;
		                                                  
  ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;    // 1) Start sampling/conversion
	result = ADC14->MEM[0] & 0x3FFF;                  // 2) read result
  ADC14->CTL0 &= ~ADC14_CTL0_SC ;                   // 3) disable Sample/Convert
	
  return result;  
}

