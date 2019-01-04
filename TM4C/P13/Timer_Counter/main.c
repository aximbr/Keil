/*testes de TIMER & COUNTER */
/*                          */
/* JPL                      */

#include "TM4C123.h"

int main(void){
	
	/*inicializar o módulo do TIMER 0 */
	SYSCTL->RCGCTIMER |= (1UL <<0);
	//SYSCTL->RCGC1 |= (1UL <<16);
	
	/*inicializa o módulo do GPIOB (pino T0CCP0 = PB6) */
	SYSCTL->RCGCGPIO |= (1UL <<1);
	//SYSCTL->RCGC2 |= (1UL <<1);
	
	//for(int j=0;j<10;j++);
	
	/*configura o pino como output e usa função alternativa */
	GPIOB->DIR |= (1UL <<6);  //PB6 as Output
	GPIOB->DEN |= (1UL <<6);  //PB6 digital enable
	GPIOB->AFSEL |= (1UL <<6); //PB6 with Alternate function
	GPIOB->PCTL |= (7UL <<24); //PB6 with function 7 (comparator exit)
	
	/*configura o GPTM0/TimerA as 16 bits counter, periodic, no prescaler, load value xxx, sem interrupção */
	TIMER0->CTL = 0x00;        //Timer disable before configure
	TIMER0->CFG = 0x00;        //Restart configuration
	TIMER0->CFG |= (4UL <<0);  //Seleciona o modo de 16 bits
	TIMER0->TAMR |= (1UL << 11) | (1UL << 3) | (2UL << 0); //TAMR - set Timer A as periodic and PWM
	TIMER0->TAILR = 0xFFFF;     //Carrega Timer A com o valor máximo do contador
	TIMER0->TAMATCHR = 0x00FF;  //Carrega o valor de comparação com 
	TIMER0->CTL |= (1UL <<0);    //Enable Timer A and start counting
	
	
	
	while(1){
		
	}
	
	
}
