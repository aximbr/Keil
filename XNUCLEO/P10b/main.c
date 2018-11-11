#include "stm32f10x.h"
#include "GPIO_STM32F10x.h"
#include "nokia5110.h"

int main(void){
	LCD5110_init();
	
	while(1){
		LCD5110_clear();
		LCD5110_write_string("Hello World");
	}
}