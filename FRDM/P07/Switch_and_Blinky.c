#include <MKL25Z4.h>


/* Etapa 07 - Comandar o piscar do LED através do Switch e uso de Systick com interrupção */
/* SW (A1 = PTB1)
 * LED (D13 = PTD1)
 */

/* Variável Global */
uint32_t tick_count;

/* Handle do Systick */
void SysTick_Handler(void){
	tick_count++;
}


/* Função que cria delay em X *10ms considerando clock de 8MHz */
void delay_1ms(uint32_t delay_ms){
  tick_count =0;
	while (tick_count < delay_ms);
}

int main(void){
	/*inicializa o SysTick */
	SysTick->CTRL = 0;                // 1) desabilita o Systick durante o setup
	SysTick->LOAD = 8000     ;        // 2) Valor para intervalo de 1 ms @ 8MHz
	SysTick->VAL = 0;                 // 3) Limpa o valor do contador
	SysTick->CTRL = 0x00000007;       // 4) Habilita o Systick com o Master Clock, com Interrupção
	
	/*inicializa o modulo da Porta B e D*/
	SIM->SCGC5 |= SIM_SCGC5_PORTB(1) | SIM_SCGC5_PORTD(1);   //Habilita o clock na PortaB e na PortaD
	PORTB->PCR[1] |= PORT_PCR_MUX(1);  //Função de GPIO
	PORTD->PCR[1] |= PORT_PCR_MUX(1);  //Função de GPIO
	
	GPIOB->PDDR = 0x00; //Porta B como input 
	
	GPIOD->PDDR |=(1<<1);   //Porta D como output (pinos 1)
	GPIOD->PCOR |=(1<<1);   //coloca 0 na PORTA D pino 1, onde está o LED Azul (acende com ZERO)
	
  	
	while(1){
		if(!(GPIOB->PDIR & (0x00000002))) {
			GPIOD->PTOR |=(1<<1);  //Toggle o BIT1 na PORTA D, fazendo o LED Azul piscar
			delay_1ms(250);
			}
		else GPIOD->PSOR |=(1<<1);
	}
}
