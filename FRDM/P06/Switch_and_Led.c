#include <MKL25Z4.h>


/* Etapa 06 - Controlar o Led através de um Switch */
/* SW (A1 = PTB1)
 * LED (D13 = PTD1)
 */

/* Variável Global */


/* Interrupt Handlers */


/* Funções locais */


int main(void){
	
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
			}
		else GPIOD->PSOR |=(1<<1);
	}
}
