/*header file for ADC lib MSP432 */
#ifndef ADC_MSP432_H_
#define ADC_MSP432_H_
#include <stdint.h>

// This initialization function sets up the ADC 
// Single Sample, 14-bits resolution
// A0 as input, store the convertion at MEM[0]
void ADC0_Init(void);



//------------ADC0_In------------
// Start a Analog to digital conversion
// Input: none
// Output: 14-bit result of ADC conversion
uint16_t ADC0_In(void);


#endif
