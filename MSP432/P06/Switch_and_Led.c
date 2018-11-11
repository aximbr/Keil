#include <stdint.h>
#include "msp.h"


/* Etapa 06 - Através de um Switch ligar e desligar um LED */
/* Default clock do MSP432 é 3 MHZ */
/* não conseguí colocar ele para funcionar a 48MHz */

/* Global variables */

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
  P1->DIR &= ~BIT4;                            // Configure P1.4 as input (S2)
	P1->REN |= BIT4;                             // Configura Pull-up em P1.4
	P1->OUT |= BIT4;                             // Habilita o Pull-up em P1.4
	
	
  /*inicio do programa */	
      while (1){
				if (!(P1->IN & BIT4)){   //testa a chave em S2 - lógica invertida
				P2->OUT ^= BIT0;         //muda o estado do led
			}
		} 
}
