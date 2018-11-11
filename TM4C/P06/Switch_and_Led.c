#include "tm4c123gh6pm.h"
#include <stdint.h>


/* Etapa 06 - ligar e desligar um led com um switch */

/* Global variables */


#define LED_RED (1U <<1)
#define SW2  0x00000001UL


int main(void){
	uint32_t delay;
	
	/* inicializa a porta de GPIO */
	SYSCTL_RCGCGPIO_R |= (1U << 5);      // 0x20U Enable clock for GPIOF
  delay = SYSCTL_RCGCGPIO_R;           // Just wait until the clock stabilize    
	SYSCTL_GPIOHBCTL_R |= (1U << 5);     // 0x20U Enable fast GPIO BUS
	GPIO_PORTF_AHB_LOCK_R = 0x4C4F434B;  // unlock GPIO Port F
  GPIO_PORTF_AHB_CR_R = 0x1F;          // allow changes to PF4-0
  GPIO_PORTF_AHB_DIR_R |= LED_RED;     // Set the GPIODIR AH (PF1) as output (00000010) 
  GPIO_PORTF_AHB_DEN_R |= LED_RED;     // Set the GPIODEN AH for Port F (Digital Function) (00000010) RED_LED
	GPIO_PORTF_AHB_DEN_R |= SW2;         // Set the GPIODEN AH for Port F (Digital Function) (00000001) SW2
	
	
	/* inicializa o Systick */
	
	
  /*inicio do programa */	

    while (1){
			if (!(GPIO_PORTF_AHB_DATA_R & SW2)){   //testa se SW2 está pressionada (lógica inversa)
			GPIO_PORTF_AHB_DATA_R ^= LED_RED;      //muda o estado do Led
			}
		} 
}
