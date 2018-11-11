/* Etapa 10 - conectar ao display via SPI */
#include <MKL25Z4.h>
#include <stdint.h>
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
	/* inicializa o Systick para 1ms */
  	SysTick_Config(SystemCoreClock/1000);
}



int main(void){
int j;
Nokia5110_Init(); //Init the LCD
MyDelay_init();

  while (1){
    Nokia5110_Clear();
    for (j=0; j<6; j++){
			Nokia5110_SetCursor(0, j);
      Nokia5110_OutString("Hello World!");
			//Nokia5110_OutChar(88);
      My_Delay(1000);
		//for (j=0; j<127; j++)
		//{lcdwrite(DATA, j);
		//My_Delay(500);
		}
		}	
  }

