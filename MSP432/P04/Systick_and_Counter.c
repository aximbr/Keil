#include <stdint.h>
#include "msp.h"


/* Etapa 04 - fazer 1 led piscar usando Systick e um contador */
/* Default clock do MSP432 é 3 MHZ */
/* não conseguí colocar ele para funcionar a 48MHz */

void Systick_Wait(uint32_t delay){
	SysTick->LOAD = delay - 1;
	SysTick->VAL = SysTick_VAL_CURRENT_Msk;
  while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0) { //check the count bit 
	}	
}
/* Função que cria delay em X *10ms considerando clock de 48MHz */
void delay_10ms(uint32_t delay_ms){
	uint32_t i;
	for (i=0; i< delay_ms; i++){
		//Systick_Wait(80000);   //para 16MHz
		//Systick_Wait(480077);    //para 48MHz
		Systick_Wait(33333);       //para 3MHz
	  }
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
	
	SysTick->CTRL = 0;                                                     // 1) disable Systick during setup
	SysTick->LOAD = SysTick_LOAD_RELOAD_Msk;                               // 2) maximum reload value
	SysTick->VAL = SysTick_VAL_CURRENT_Msk;                                // 3) clear the counter
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;  // 4) enable Systick with core clock
	
	           
	
  /*inicio do programa */	
  P2->OUT |= BIT0;    //Turn on red LED
    while (1){
			delay_10ms(25);                       //delay 250 ms
			P2->OUT ^= BIT0;;                     //Toggle Red LED (PF1)
			} 
  	}
