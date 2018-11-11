/*rotinas para tratar o clock do TM4C123 */
#include "clock_tm4c.h"
#include "TM4C123.h"

void clock_tm4c_init(void)
{
	/*
	1 - Habilita o MOSC
	2 - Define o clock do XTAL como 16 MHz (21)
	3 - Habilita o SYSCLOCK Divider
	4 - Define o SYSDIV = 4
	5 - Habilita o divisor do clock do PWM e define o divisor como 64
	6 - Habilita o power do PLL (desativa do PLLPWRDN)
	7 - Desativa o ByPass do PLL
	8 - Define como Clock Source o MOSC
	9 - Aguarda clock estabilizar
	10 - Desativa o clock interno
	Clock final: XTAL (16Mhz) -> PLL (400MHz)/2 -> 200/4 (SYSDIV) = 50 MHz
	*/
	
	
	
	  SYSCTL->RCC2 = 0x07802810;    /* set default value */
    SYSCTL->RCC  = 0x078E3AD1;    /* set default value */

    SYSCTL->RCC  = (RCC_Val  | (1UL<<11) | (1UL<<13)) & ~(1UL<<22); /* set value with BYPASS, PWRDN set, USESYSDIV reset */
    SYSCTL->RCC2 = (RCC2_Val | (1UL<<11) | (1UL<<13));              /* set value with BYPASS, PWRDN set */
    for (i = 0; i < 1000; i++);   /* wait a while */

    SYSCTL->RCC  = (RCC_Val  | (1UL<<11)) & ~(1UL<<22);             /* set value with BYPASS, USESYSDIV reset */
    SYSCTL->RCC2 = (RCC2_Val | (1UL<<11));                          /* set value with BYPASS */
    for (i = 0; i < 1000; i++);   /* wait a while */

    SYSCTL->RCC  = (RCC_Val  | (1<<11));                            /* set value with BYPASS */

    if ( (((RCC_Val  & (1UL<<13)) == 0) && ((RCC2_Val & (1UL<<31)) == 0)) ||
         (((RCC2_Val & (1UL<<13)) == 0) && ((RCC2_Val & (1UL<<31)) != 0))   ) {
      while ((SYSCTL->RIS & (1UL<<6)) != (1UL<<6));                 /* wait until PLL is locked */
    }

    SYSCTL->RCC  = (RCC_Val);                                       /* set value */
    SYSCTL->RCC2 = (RCC2_Val);                                      /* set value */
    for (i = 0; i < 10000; i++);   /* wait a while */
}