#include <stdint.h>
#include "UART.h"
#include "PLL.h"
#include <stdio.h>



int main(void){
  uint32_t i;
  
  PLL_Init();               // clock at 80 MHz
  UART_Init();              // initialize UART
    while(1){
    UART_OutString("teste \r\n");
		for (i=0; i<3000000; i++);

  }
}
