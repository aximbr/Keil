/*Header para as funções de controle de tick */

#ifndef __MY_TICK_H__
#define __MY_TICK_H__

#include <stdint.h>

/* altere conforme a plataforma */
#include "stm32f10x.h"

#define HAL_MAX_DELAY 0xFFFFFFFFU

void InitMyTick(void);
void MyTick(void);
uint32_t GetMyTick(void);
void My_Delay(uint32_t delay_ms);


#endif //__MY_TICK_H__
