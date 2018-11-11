#include "msp.h"

/* Etapa 01 - Acender o Led */

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;  // Stop watchdog timer

    // The following code toggles P1.0 port
    P1->DIR |= BIT0;                             // Configure P1.0 Led Red
    P1->OUT |= BIT0;
    while(1){};
   
}
