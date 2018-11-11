#include "tm4c123gh6pm.h"
#include <stdint.h>


/* Etapa 03 - fazer 2 led piscarem */

#define LED_RED (1U <<1)
#define LED_BLUE (1U <<2)

int main(void){
	uint32_t delay;
	uint32_t i;
	
	SYSCTL_RCGCGPIO_R |= (1U << 5);   // 0x20U Enable clock for GPIOF
  delay = SYSCTL_RCGCGPIO_R;        // Just wait until the clock stabilize    
	SYSCTL_GPIOHBCTL_R |= (1U << 5);  // 0x20U Enable fast GPIO BUS
  GPIO_PORTF_AHB_DIR_R |= LED_RED | LED_BLUE;  //Set the GPIODIR AH (PF1 and PF2) as output (00000110) 
  GPIO_PORTF_AHB_DEN_R |= LED_RED | LED_BLUE;  //Set the GPIODEN AH for Port F (Digital Function) (00000110)

  GPIO_PORTF_AHB_DATA_R |= LED_RED | LED_BLUE;	//Turn ON Red LED (PF1) and Blue LED (PF2)
    while (1){
			for (i =0;i<500000;i++);                       //delay
			GPIO_PORTF_AHB_DATA_R ^= LED_RED; //Toggle Red LED (PF1)
			for (i =0;i<500000;i++);
			GPIO_PORTF_AHB_DATA_R ^= LED_BLUE; //Toggle Red LED (PF2)
     } 
  	}
