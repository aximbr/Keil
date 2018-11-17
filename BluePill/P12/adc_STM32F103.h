/*header file for ADC lib STM32F103 with CMSIS lib */
#ifndef ADC_STM32F103_H_
#define ADC_STM32F103_H_
#include <stdint.h>

// This initialization function sets up the ADC 
// Single Sample, 12-bits resolution
// A0 as input, store the convertion at MEM[0]
void ADC0_Init(void);



//------------ADC0_In------------
// Start a Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint16_t ADC0_In(void);


#endif
