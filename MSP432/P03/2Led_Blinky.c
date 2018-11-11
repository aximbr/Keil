#include "msp.h"

/*Etapa 03 - fazer 2 Led Piscarem */

int main(void)
{
	int i;
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;  // Stop watchdog timer

    
    P2->DIR |= BIT0 |BIT2;                             // Configure P2.0 and P2.2 as output (Red and Blue LED)
    P2->OUT |= BIT0 |BIT2;    //Turn on both LEDs
    while(1)
    {
      for(i=100000; i>0; i--);                  // Delay
			P2->OUT ^= BIT0;                         // Toggle P2.0
      for(i=100000; i>0; i--);                  // Delay
			P2->OUT ^= BIT2;                         // Toggle P2.1
    }
}
