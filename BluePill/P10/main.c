/*Nova tentativa de usa a comunicação com SPI */
/* MCU STM32F103C8T6 */
/*Comunicação via SPI1 com display NOKIA5110 */
#include "stm32f10x.h"
#include "GPIO_STM32F10x.h"
#include "nokia5110_stm32.h"



int main(void){
int j;
//unsigned char cx;
//Incluir rotina para configuração do Clock
//Default Clock interno (RC) com 8MHz
	

SPI1_init();
LCD_init();

  while (1){
    LCD_clear();
    for (j=0; j<6; j++){
			LCD_write_string(0,j,"Hello World!");
			delayMs(500);
		}			
  }
  	
}
