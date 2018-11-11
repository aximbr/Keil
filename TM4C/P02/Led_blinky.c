#include "tm4c123gh6pm.h"
#include <stdint.h>

/* Etapa 02 - fazer o led piscar */

#define LED_RED (1U <<1)

int main(void){
	uint32_t delay;
	uint32_t i;
	
	SYSCTL_RCGCGPIO_R |= (1U << 5);   // 0x20U Enable clock for GPIOF
  delay = SYSCTL_RCGCGPIO_R;        // Just wait until the clock stabilize    
	SYSCTL_GPIOHBCTL_R |= (1U << 5);  // 0x20U Enable fast GPIO BUS
  GPIO_PORTF_AHB_DIR_R |= LED_RED;  //Set the GPIODIR AH (PF1) as output (00000010) 
  GPIO_PORTF_AHB_DEN_R |= LED_RED;  //Set the GPIODEN AH for Port F (Digital Function) (00000010)

  GPIO_PORTF_AHB_DATA_BITS_R[LED_RED] |= LED_RED; //Turn ON Red LED (PF1) 
    while (1){
			for (i =0;i<500000;i++);                       //delay
			GPIO_PORTF_AHB_DATA_BITS_R[LED_RED] ^= LED_RED; //Toggle Red LED (PF1)
     } 
  	}
