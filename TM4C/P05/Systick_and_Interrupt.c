#include "tm4c123gh6pm.h"
#include <stdint.h>


/* Etapa 05 - fazer 1 led piscar usando Systick e com Interrupção */

/* Global variables */
uint32_t tick_count;

#define LED_RED (1U <<1)
void SysTick_Handler(void){
	tick_count ++;
}

/* Função que cria delay em X *10ms considerando clock de 16MHz */
void delay_10ms(uint32_t delay_ms){
	tick_count =0;
	while (tick_count < delay_ms);   //a cada 10 ms o Systick incrementa tick_count
}
	

int main(void){
	uint32_t delay;
	
	/* inicializa a porta de GPIO */
	SYSCTL_RCGCGPIO_R |= (1U << 5);   // 0x20U Enable clock for GPIOF
  delay = SYSCTL_RCGCGPIO_R;        // Just wait until the clock stabilize    
	SYSCTL_GPIOHBCTL_R |= (1U << 5);  // 0x20U Enable fast GPIO BUS
  GPIO_PORTF_AHB_DIR_R |= LED_RED;  //Set the GPIODIR AH (PF1) as output (00000010) 
  GPIO_PORTF_AHB_DEN_R |= LED_RED;  //Set the GPIODEN AH for Port F (Digital Function) (00000010)
	
	/* inicializa o Systick */
	
	NVIC_ST_CTRL_R = 0;               // 1) disable Systick during setup
	NVIC_ST_RELOAD_R = (80000-1);     // 2) 10 ms for clock = 16MHz
	NVIC_ST_CURRENT_R = 0;            // 3) clear the counter
	NVIC_ST_CTRL_R = 0x00000007;      // 4) enable Systick with core clock with interrupt

	
  /*inicio do programa */	
  GPIO_PORTF_AHB_DATA_R |= LED_RED;	//Turn ON Red LED (PF1)
    while (1){
			delay_10ms(100);                       //delay 250 ms
			GPIO_PORTF_AHB_DATA_R ^= LED_RED;     //Toggle Red LED (PF1)
			} 
  	}
