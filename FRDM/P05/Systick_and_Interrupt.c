#include <MKL25Z4.h>


/* Etapa 05 - Fazer o Led piscar usando SysTick e interrupção */

/* Variável Global */
uint32_t tick_count;

/* Handle do Systick */
void SysTick_Handler(void){
	tick_count++;
}


/* Função que cria delay em X *10ms considerando clock de 8MHz */
void delay_10ms(uint32_t delay_ms){
  tick_count =0;
	while (tick_count < delay_ms);
}

int main(void){
	
	/*inicializa o SysTick */
	SysTick->CTRL = 0;                // 1) desabilita o Systick durante o setup
	SysTick->LOAD = 80000     ;       // 2) Valor para intervalo de 10 ms @ 8MHz
	SysTick->VAL = 0;                 // 3) Limpa o valor do contador
	SysTick->CTRL = 0x00000007;       // 4) Habilita o Systick com o Master Clock, com Interrupção
	
	
	/*inicializa o modulo da Porta D*/
	SIM->SCGC5 |= SIM_SCGC5_PORTD(1);   //Habilita o clock na PortaD
	PORTD->PCR[1] |= PORT_PCR_MUX(1);  //Função de GPIO
	
	GPIOD->PDDR |=(1<<1);  //Porta D como output (pinos 1)
	GPIOD->PCOR |=(1<<1);  //coloca 0 na PORTA D pino 1, onde está o LED Azul (acende com ZERO)
		
	while(1){
		delay_10ms(25);        //250 ms de delay
		GPIOD->PTOR |=(1<<1);  //Toggle o BIT1 na PORTA D, fazendo o LED Azul piscar
	}
}
