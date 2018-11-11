#include <MKL25Z4.h>

/* Etapa 03 - Fazer 2 Led piscarem */

int main(void){
	int i;
	/*inicializa o modulo da Porta B*/
	SIM->SCGC5 |= SIM_SCGC5_PORTB(1);   //Habilita o clock na PortaB
	PORTB->PCR[18] |= PORT_PCR_MUX(1);  //Função de GPIO
	PORTB->PCR[19] |= PORT_PCR_MUX(1);  //Função de GPIO
	
	GPIOB->PDDR |=(1<<18) | (1<<19);  //Porta B como output (pinos 18 e 19)
	GPIOB->PCOR |=(1<<18) | (1<<19);  //coloca 0 na PORTA B pinos 18 e 19, onde está o LED Vermelho e Verde (acende com ZERO)
		
	while(1){
		for(i=500000;i>0;i--);
		GPIOB->PTOR |=(1<<18);  //Toggle o BIT18 na PORTA B, fazendo o LED Vermelho piscar
		for(i=500000;i>0;i--);
		GPIOB->PTOR |=(1<<19);  //Toggle o BIT19 na PORTA B, fazendo o LED Verde piscar
	}
		
}
