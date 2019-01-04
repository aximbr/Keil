/*Etapa 4 - Piscar o Leds D1 em PF9 com o SysTick e um contador */
#include "stm32f4xx.h"

void Systick_Wait(uint32_t delay){
	SysTick->LOAD = delay - 1;
	SysTick->VAL = 0;
  while((SysTick->CTRL & 0x00010000) == 0) { //aguarda até zerar o contador do SysTick 
	}	
}

/* Função que cria delay em X *1ms */
void My_delay(uint32_t delay_ms){
	uint32_t i;
	for (i=0; i< delay_ms; i++){
		Systick_Wait(SystemCoreClock/1000);}  //configura o SysTick para 1ms
	}

	
int main(void){
/*local variable */
	
	
/*setup GPIO F */
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN_Msk;                     //1) Enable clock on GPIOF
	GPIOF->MODER |= GPIO_MODER_MODE9_0;                          //2) Set PF10 and PF9 mode (output)
	
/* inicializa o Systick */
	__disable_irq();
	SysTick->CTRL = 0;             //desabilita o systick
	SysTick->LOAD = 0x00FFFFFF;    //carrega o valor maximo
	SysTick->CTRL = 0x5;           //habilita o SysTick sem interrupção
	__enable_irq();
	
		
 //Endless loop
	while(1){ 
	     GPIOF->ODR &= ~GPIO_ODR_OD9_Msk;       //turn on LED D1
		   My_delay(250);                         //delay
	     GPIOF->ODR |= GPIO_ODR_OD9_Msk;        //turn off LED D1
	     My_delay(250);                         //delay
	     }
}

