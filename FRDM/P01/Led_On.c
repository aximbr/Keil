#include <MKL25Z4.h>
/* Etapa 01 - Acender o Led */

int main(void){
	/*inicializa o modulo da Porta B*/
	SIM->SCGC5 |= SIM_SCGC5_PORTB(1);   //Habilita o clock na PortaB
	PORTB->PCR[19] |= PORT_PCR_MUX(1);  //Função de GPIO
	
	GPIOB->PDDR |=(1<<19);  //Porta B como output
	GPIOB->PCOR |=(1<<19);  //coloca 0 na PORTA B pino 19, onde está o LED Verde (acende com ZERO)
	
	
	while(1);
		
}
