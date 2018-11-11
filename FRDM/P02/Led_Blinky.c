#include <MKL25Z4.h>

/* Etapa 02 - Fazer o Led piscar */

int main(void){
	int i;
	/*inicializa o modulo da Porta B*/
	SIM->SCGC5 |= SIM_SCGC5_PORTD(1);   //Habilita o clock na PortaB
	PORTD->PCR[1] |= PORT_PCR_MUX(1);  //Função de GPIO
	
	GPIOD->PDDR |=(1<<1);  //Porta B como output
	GPIOD->PCOR |=(1<<1);  //coloca 0 na PORTA B pino 19, onde está o LED Verde (acende com ZERO)
		
	while(1){
		for(i=500000;i>0;i--);
		GPIOD->PTOR |=(1<<1);  //Toggle o BIT19 na PORTA B, fazendo o LED Verde piscar
	}
		
}
