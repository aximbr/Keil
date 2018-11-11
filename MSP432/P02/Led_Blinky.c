#include "msp.h"

/*Etapa 02 - fazer o Led Piscar */

int main(void)
{
	int i;
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;  // Stop watchdog timer

    
    P1->DIR |= BIT0;                             // Configure P1.0 as output (LED Red)
    P1->OUT |= BIT0;
    while(1)
    {
      for(i=100000; i>0; i--);                  // Delay
			P1->OUT ^= BIT0;                         // Toggle P1.0
        
    }
}
