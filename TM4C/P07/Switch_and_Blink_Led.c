#include "tm4c123gh6pm.h"
#include <stdint.h>


/* Etapa 07 - controlar o piscar do LED através do switch */

/* Global variables */


#define LED_RED (1U <<1)
#define SW2  0x00000001UL
#define DEBOUNCE_TIME 50

/*Global Variables */
uint32_t SW_State, SW_Read_0, SW_Read_1, tick_count, SW_Read;

void SysTick_Handler(void){
// Atualiza o tick
	tick_count ++;
	
// debouncing para o switch SW2
	SW_Read = (GPIO_PORTF_AHB_DATA_R & SW2);
	if (SW_Read == 0) {
		SW_Read_0++;
		SW_Read_1 = 0;
		if (SW_Read_0 >= DEBOUNCE_TIME){
			SW_Read_0 = DEBOUNCE_TIME +1;
			SW_State = 0;}
		}
	else {
		SW_Read_1++;
	  SW_Read_0 = 0;}
	if (SW_Read_1 >= DEBOUNCE_TIME){
		  SW_Read_1 = DEBOUNCE_TIME +1;
		  SW_State = 1;}
}

void My_Delay(uint32_t pausa){
tick_count =0;
	while (tick_count < pausa);
}

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
  NVIC_ST_CTRL_R = 0;               // 1) disable Systick during setup
	NVIC_ST_RELOAD_R = (16000-1);     // 2) 1 ms for clock = 16MHz
	NVIC_ST_CURRENT_R = 0;            // 3) clear the counter
	NVIC_ST_CTRL_R = 0x00000007;      // 4) enable Systick with core clock with interrupt
	
	/* inicializa o estado da chave */
	SW_Read = GPIO_PORTF_AHB_DATA_R & SW2;
	SW_State = 0;
	SW_Read_0 = 0;
	SW_Read_1 = 0;
	
	
  /*inicio do programa */	

    while (1){
			while (!(SW_State)) {                  //Logica inversa da chave
				GPIO_PORTF_AHB_DATA_R ^= LED_RED;    //Pisca o Led, caso a chave pressionada
			  My_Delay(500);}                      //Intervalo da piscada 500 ms
			GPIO_PORTF_AHB_DATA_R &= ~(LED_RED);   //Apaga o Led se a chave não estiver pressionada
			}
} 

