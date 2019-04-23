/* Implementação de controle da base de tempo (mytick) 
   o valor dessa variável é incrementada a cada milisegundo */
	 
#include "my_tick.h"
volatile uint32_t mytick;

void InitMyTick(void)
{ mytick = 0;
	__disable_irq();
	SysTick->CTRL = 0;
	SysTick->LOAD = (SystemCoreClock/1000U);
	SysTick->CTRL = 0x07;  /* habilita o SysTick com Interrupção na prioridade padrão */
	__enable_irq();
}

void MyTick(void)
{ mytick++; }

uint32_t GetMyTick(void)
{ return mytick; }

void My_Delay(uint32_t delay_ms){
	uint32_t ticknow = GetMyTick();
	while ((GetMyTick()- ticknow) < delay_ms);
}
