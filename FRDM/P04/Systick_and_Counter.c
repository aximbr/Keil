#include <MKL25Z4.h>


/* Etapa 04 - Fazer o Led piscar usando SysTick e contador */

/* Função para ajustar o SysTick */
void Systick_Wait(uint32_t delay){
	SysTick->LOAD = delay - 1;
	SysTick->VAL = 0;
  while((SysTick->CTRL & 0x00010000) == 0) { //check the count bit 
	}	
}
/* Função que cria delay em X *10ms considerando clock de 8MHz */
void delay_10ms(uint32_t delay_ms){
	uint32_t i;
	for (i=0; i< delay_ms; i++){
		Systick_Wait(80000);}
	}

int main(void){
	
	/*inicializa o SysTick */
	SysTick->CTRL = 0;                // 1) desabilita o Systick durante o setup
	SysTick->LOAD = 0x00FFFFFF;       // 2) Valor máximo do Reload
	SysTick->VAL = 0;                 // 3) Limpa o valor do contador
	SysTick->CTRL = 0x00000005;       // 4) Habilita o Systick com o Master Clock, sem Interrupção
	
	
	/*inicializa o modulo da Porta B*/
	SIM->SCGC5 |= SIM_SCGC5_PORTB(1);   //Habilita o clock na PortaB
	PORTB->PCR[18] |= PORT_PCR_MUX(1);  //Função de GPIO
	
	GPIOB->PDDR |=(1<<18);  //Porta B como output (pinos 18)
	GPIOB->PCOR |=(1<<18);  //coloca 0 na PORTA B pinos 18, onde está o LED Vermelho (acende com ZERO)
		
	while(1){
		delay_10ms(100);        //1 segundo de delay
		GPIOB->PTOR |=(1<<18);  //Toggle o BIT18 na PORTA B, fazendo o LED Vermelho piscar
	}
}
