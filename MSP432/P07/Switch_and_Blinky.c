#include <stdint.h>
#include "msp.h"

#define DEBOUNCE_TIME 50


/* Etapa 07 - Através de um Switch comandar o piscar de um LED com uso de interrupção */
/* LED = P2.0 */
/* SW  = P1.4 */
/* Default clock do MSP432 é 3 MHZ */
/* não conseguí colocar ele para funcionar a 48MHz */
/* configuração abaixo para rodar em 24 MHz, com o DCO (clock interno) */

/* Global variables */
uint32_t SW_State, SW_Read_0, SW_Read_1, tick_count, SW_Read;

/* Interrupt Handlers */
void SysTick_Handler(void){
// Atualiza o tick
	tick_count ++;
	
// debouncing para o switch SW2
	SW_Read = (P1->IN & BIT4);
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

/* Local functions */
void My_Delay(uint32_t pausa){
tick_count =0;
	while (tick_count < pausa);
}


int main(void){
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;  // Stop watchdog timer	
	/*Clock Initialization on MSP432 to 24 MHz */
	/* START*/
CS->KEY = 0x695A;   //Password para ajustar o clock
CS->CTL0 = 0;       //Limpa configuração
CS->CTL0 |= CS_CTL0_DCORSEL_4 | CS_CTL0_DCOEN;          // DCOR = 24 mhz
CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
// MCLK = 24 mhz
CS->KEY = 0x0000; // lock the clock registers
while((!(CS->STAT & CS_STAT_SMCLK_READY)) && (!(CS->STAT & CS_STAT_MCLK_READY))){}; // wait for clocks to ready
  /* END */
	
	/* inicializa o Systick */
	SysTick->CTRL = 0;                                                      // 1) disable Systick during setup
	SysTick->LOAD = 24000-1;                                                // 2) reload value for 1 ms @ 24 MHz
	SysTick->VAL = SysTick_VAL_CURRENT_Msk;                                 // 3) clear the counter
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;  
	                                                                        // 4) enable Systick with core clock and interrupt
	
	/* inicializa a porta de GPIO */
	P2->DIR |= BIT0;                             // Configure P2.0 as output (Red LED)
  P1->DIR &= ~BIT4;                            // Configure P1.4 as input (S2)
	P1->REN |= BIT4;                             // Configura Pull-up em P1.4
	P1->OUT |= BIT4;                             // Habilita o Pull-up em P1.4
	
	
  /*inicio do programa */	
	
	/* inicializa o estado da chave */
	SW_Read = (P1->IN & BIT4);
	SW_State = 0;
	SW_Read_0 = 0;
	SW_Read_1 = 0;
	
      while (1){
				while (!(SW_State)) {                  //Logica inversa da chave
				P2->OUT  ^= BIT0;    //Pisca o Led, caso a chave pressionada
			  My_Delay(500);}                      //Intervalo da piscada 500 ms
			  P2->OUT &= ~(BIT0);   //Apaga o Led se a chave não estiver pressionada
		} 
}
