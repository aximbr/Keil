/* Conectar um display e exibir uma mensagem */
#include "MSP432.h"
#include "Nokia5110.h"

/*Global Variables */
uint32_t  tick_count;

void SysTick_Handler(void){
// Atualiza o tick
	tick_count ++;
}

void My_Delay(uint32_t pausa){
tick_count =0;
	while (tick_count < pausa);
}

void MyDelay_init(void){
	/* inicializa o Systick */
  
	SysTick_Config(SystemCoreClock /1000);
}

int main(void){
int j;
/*Clock Initialization on MSP432 to 24 MHz */
	/* START*/
CS->KEY = 0x695A;   //Password para ajustar o clock
CS->CTL0 = 0;       //Limpa configuração
CS->CTL0 |= CS_CTL0_DCORSEL_2 | CS_CTL0_DCOEN;          // DCOR = 6 mhz
CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
// MCLK = 6 mhz
CS->KEY = 0x0000; // lock the clock registers
while((!(CS->STAT & CS_STAT_SMCLK_READY)) && (!(CS->STAT & CS_STAT_MCLK_READY))){}; // wait for clocks to ready
  /* END */
	
	
Nokia5110_Init(); //Init the LCD
MyDelay_init();

  while (1){
    Nokia5110_Clear();
    for (j=0; j<6; j++){
			Nokia5110_SetCursor(0, j);
      Nokia5110_OutString("Hello World!");
      My_Delay(2000);
		}			
  }
}
