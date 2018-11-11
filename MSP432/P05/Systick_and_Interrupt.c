#include <stdint.h>
#include "msp.h"


/* Etapa 05 - fazer 1 led piscar usando Systick e interrupção */
/* Default clock do MSP432 é 3 MHZ */
/* não conseguí colocar ele para funcionar a 48MHz */

/* Global variables */
uint32_t tick_count;

void SysTick_Handler(void){
	tick_count++;
}
/* Função que cria delay em X *10ms considerando */
void delay_10ms(uint32_t delay_ms){
	tick_count =0;
	while(tick_count <delay_ms);
}
	
int main(void){
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;  // Stop watchdog timer	
	/*Clock Initialization on MSP432 to 48 MHz */
	/* START*/
CS->KEY = 0x695A;   //Password para ajustar o clock
CS->CTL0 = 0;
CS->CTL0 |= CS_CTL0_DCORSEL_1 | CS_CTL0_DCOEN;          // DCOR = 3 mhz
CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
          
// MCLK = 3 mhz
CS->KEY = 0x0000; // lock the clock registers
while((!(CS->STAT & CS_STAT_SMCLK_READY)) && (!(CS->STAT & CS_STAT_MCLK_READY))){}; // wait for clocks to ready
  /* END */
	
	
	/* inicializa a porta de GPIO */
	P2->DIR |= BIT0;                             // Configure P2.0 as output (Red LED)
  
	
	/* inicializa o Systick */
	//use reload com 80000   //para 16MHz
	//use reload com 480077  //para 48MHz
	//use reload com 33333   //para 3MHz
	SysTick->CTRL = 0;                                                     // 1) disable Systick during setup
	SysTick->LOAD = 33333;                                                 // 2) reload value for 10 ms @ 3 MHz
	SysTick->VAL = SysTick_VAL_CURRENT_Msk;                                // 3) clear the counter
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;  
	                                                                       // 4) enable Systick with core clock and interrupt
	
	           
	
  /*inicio do programa */	
  P2->OUT |= BIT0;    //Turn on red LED
    while (1){
			delay_10ms(25);                       //delay 250 ms
			P2->OUT ^= BIT0;;                     //Toggle Red LED (PF1)
			} 
  	}
